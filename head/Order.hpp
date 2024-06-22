#pragma once
#include "system.hpp"

// The parse function is used to provide the execution statement for the corresponding database
// constructResponse is used to construct the response or result of the database execution statement

namespace Redis
{
    class VerifyCheckCode : NonCopyable
    {
    private:
        nlohmann::json _order;
        nlohmann::json _result;
        static std::string _redisResponse;

    public:
        nlohmann::json constructResponse(const nlohmann::json &order, redisClient &memoryData);
        VerifyCheckCode();
        inline ~VerifyCheckCode()
        {
        }
    };
}