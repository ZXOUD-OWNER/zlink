#pragma once
#include "system.hpp"

class redisClient
{
private:
    std::string _connectIP;
    int _port;
    redisContext *_client = nullptr;

public:
    redisClient(const nlohmann::json &value);
    ~redisClient();
    bool connect();
    void exeCommand(redisReplyWrap &reply, const std::string &order);
};
namespace CUtil
{
    std::string ConstructResponseMsgRedis(const nlohmann::json &req);
}
