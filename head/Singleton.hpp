#pragma once
#include "system.hpp"
#include "Order.hpp"
// using RedisOrder = std::variant<Redis::ServerInit, Redis::UserInit, Redis::VerifyCheckCode>;

class Singleton
{
private:
    Singleton();
    ~Singleton(){};
    Singleton(const Singleton &);
    Singleton &operator=(const Singleton &);

private:
    nlohmann::json _conf;
    int _EnableDataBaseType = -1;
    std::unordered_map<std::string, int> _redisOrder;

private:
    void initRedisOrder();

public:
    inline static Singleton &getInstance()
    {
        static Singleton instance;
        return instance;
    }
    inline const auto &GetConf()
    {
        return _conf;
    }
    inline int GetDataBaseType()
    {
        return _EnableDataBaseType;
    }
    inline const std::unordered_map<std::string, int> &GetRedisOrder()
    {
        return _redisOrder;
    }
};