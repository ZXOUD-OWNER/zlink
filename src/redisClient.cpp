#include "head.hpp"

RedisClient::RedisClient(const nlohmann::json &value)
{
    if (value.find("MemoryDataBase") == value.end())
    {
        LOG(FATAL) << "config.json not config MemoryDataBase info";
    }
    auto value2 = value.find("MemoryDataBase").value();
    if (value2.find("Redis") == value.end())
    {
        LOG(FATAL) << "config.json not config redis info";
    }
    auto value3 = value2.find("Redis").value();

    _connectIP = value3.find("RedisConnectIP").value().get<std::string>();
    _port = value3.find("RedisConnectPort").value().get<int>();
    connect();
}

RedisClient::~RedisClient()
{
    if (_client != nullptr)
    {
        redisFree(_client);
        _client = nullptr;
    }
}

bool RedisClient::connect()
{
    _client = redisConnect(_connectIP.c_str(), _port);
    if (_client == nullptr || _client->err)
    {
        LOG(ERROR) << "redis Connect err! func stack is " << CUtil::printTrace();
        if (_client->err)
        {
            LOG(ERROR) << "redis Connect err is " << _client->errstr;
        }
        return false;
    }
    return true;
}

void RedisClient::exeCommand(RedisReplyWrap &reply, const std::string &order)
{
    auto *replyTemp = static_cast<redisReply *>(redisCommand(_client, order.c_str()));
    if (replyTemp == nullptr)
    {
        LOG(ERROR) << "Failed to execute command: " << _client->errstr << " func stack is " << CUtil::printTrace();
        return;
    }
    reply._reply = replyTemp;
}

void RedisClient::startTransaction()
{
    _transactionOrderNum = 0;
    redisAppendCommand(_client, "MULTI");
}

void RedisClient::addCommandToTransaction(const std::string &order)
{
    redisAppendCommand(_client, order.c_str());
    ++_transactionOrderNum;
}

void RedisClient::exeTransaction(std::vector<RedisReplyWrap> &replyArry)
{
    redisAppendCommand(_client, "EXEC");
    RedisReplyWrap temp;
    for (decltype(_transactionOrderNum) i = 0; i < _transactionOrderNum; i++)
    {
        redisGetReply(_client, (void **)(&temp._reply));
        replyArry.emplace_back(temp._reply);
    }
}
