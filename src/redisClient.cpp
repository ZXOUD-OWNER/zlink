#include "head.hpp"

redisClient::redisClient(const nlohmann::json &value)
{
    if (value.find("MemoryDataBase") == value.end())
    {
        LOG(FATAL) << "config.json not config MemoryDataBase info";
    }
    auto value2 = value.find("MemoryDataBase").value().object();
    if (value.find("Redis") == value.end())
    {
        LOG(FATAL) << "config.json not config redis info";
    }
    auto value3 = value2.find("Redis").value().object();

    _connectIP = value3.find("RedisConnectIP").value().get<std::string>();
    _port = value3.find("RedisConnectPort").value().get<int>();
    auto res = connect();
}

redisClient::~redisClient()
{
    if (_client != nullptr)
    {
        redisFree(_client);
        _client = nullptr;
    }
}

bool redisClient::connect()
{
    _client = redisConnect(_connectIP.c_str(), _port);
    if (_client == nullptr || _client->err)
    {
        LOG(ERROR) << "redis Connect err! func stack is " << CUitl::Print_trace();
        if (_client->err)
        {
            LOG(ERROR) << "redis Connect err is " << _client->errstr;
        }
        return false;
    }
    return true;
}

void redisClient::exeCommand(redisReplyWrap &reply, const std::string &order)
{
    auto *replyTemp = static_cast<redisReply *>(redisCommand(_client, order.c_str()));
    if (replyTemp == nullptr)
    {
        LOG(ERROR) << "Failed to execute command: " << _client->errstr << " func stack is " << CUitl::Print_trace();
        return;
    }

    if (replyTemp->type == REDIS_REPLY_STATUS && strcmp(replyTemp->str, "OK") == 0)
    {
        LOG(INFO) << "Successfully set key";
    }
    else
    {
        LOG(WARNING) << "Failed to set key";
    }

    reply.reply = replyTemp;
}
