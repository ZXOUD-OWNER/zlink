#pragma once
#include <mimalloc-override.h>
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
#include <unordered_map>
#include <utility>
#include <variant>
#define WORKER_READY "\001"

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
