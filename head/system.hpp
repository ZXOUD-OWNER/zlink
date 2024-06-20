#pragma once
#include <czmq.h>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <glog/logging.h>
#include <libunwind.h>
#include <hiredis/hiredis.h>
#include <format>
#include "nlohmann/json.hpp"

#define WORKER_READY "\001"
namespace CUitl
{
    inline std::string Print_trace()
    {
        unw_cursor_t cursor;
        unw_context_t context;
        std::string temp;
        // Init context
        unw_getcontext(&context);
        unw_init_local(&cursor, &context);

        // traverse the call stack
        while (unw_step(&cursor) > 0)
        {
            unw_word_t offset, pc;
            char fname[64];

            unw_get_reg(&cursor, UNW_REG_IP, &pc);
            fname[0] = '\0';
            unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
            temp.append(fname);
            temp.append("() ");
            temp.append("+0x");
            temp.append(std::to_string(offset));
            temp.append("\n");
        }
        return temp;
    }
    inline void GetZMsg(zmsg_t *msg, std::string &msgStr)
    {
        char *frame_content;
        while ((frame_content = zmsg_popstr(msg)) != NULL)
        {
            // handle frame content
            msgStr.append(frame_content);
            printf("Frame content: %s\n", frame_content);
            free(frame_content); // free memory
        }
    }
}

class NonCopyable
{
protected:
    NonCopyable() = default;  // Allow default constructor
    ~NonCopyable() = default; // Allow default destructor

    NonCopyable(const NonCopyable &) = delete;            // Disallow copy constructor
    NonCopyable &operator=(const NonCopyable &) = delete; // Disallow assignment operator

    NonCopyable(NonCopyable &&) = delete;            // Disallow move constructor
    NonCopyable &operator=(NonCopyable &&) = delete; // Disallow move assignment operator
};

class Singleton
{
private:
    inline Singleton()
    {
        try
        {
            std::filesystem::path p("./config.json");
            std::ifstream ifs(p, std::ifstream::binary);
            std::string content((std::istreambuf_iterator<char>(ifs)), {});
            conf = nlohmann::json::parse(content);

            auto IOThreadNumber = conf.find("IOThreadNumber").value().get<int>();
            zsys_set_io_threads(IOThreadNumber); // Init global zmq of IO thread Numbers
        }
        catch (const std::exception &e)
        {
            LOG(FATAL) << "occur exception! err si " << e.what() << " func stack is " << CUitl::Print_trace();
        }
    };
    ~Singleton(){};
    Singleton(const Singleton &);
    Singleton &operator=(const Singleton &);

private:
    nlohmann::json conf;

public:
    inline static Singleton &getInstance()
    {
        static Singleton instance;
        return instance;
    }
    inline const auto &GetConf()
    {
        return conf;
    }
};

struct redisReplyWrap : NonCopyable
{
    redisReply *reply = nullptr;
    inline ~redisReplyWrap()
    {
        if (reply != nullptr)
        {
            freeReplyObject(reply);
            reply = nullptr;
        }
    }
};