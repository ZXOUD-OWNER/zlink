#pragma once
#include "system.hpp"

struct CUtil
{
    static std::string Print_trace();
    static std::string ConstructResponseMsgRedis(const nlohmann::json &req);
    static inline void GetZMsg(zmsg_t *msg, std::string &msgStr)
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
};
