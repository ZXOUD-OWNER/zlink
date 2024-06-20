#pragma once
#include "system.hpp"

namespace Order
{
    class UserInit : NonCopyable
    {
    private:
        nlohmann::json _order;
        nlohmann::json _result;

    public:
        std::string parse(const nlohmann::json &order);
        nlohmann::json constructResponse(redisReplyWrap &replyW);
        UserInit();
        ~UserInit();
    };

    class ServerInit : NonCopyable
    {
    private:
        nlohmann::json _order;
        nlohmann::json _result;

    public:
        std::string parse(const nlohmann::json &order);
        nlohmann::json constructResponse(redisReplyWrap &replyW);
        ServerInit();
        inline ~ServerInit()
        {
        }
    };
}