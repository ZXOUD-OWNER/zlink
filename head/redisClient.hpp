/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240620
 * FilePath: /zlink/head/redisClient.hpp
 * Description: redis client command exec
 */
#pragma once
#include "system.hpp"

class RedisClient
{
private:
    std::string _connectIP;
    int _port;
    redisContext *_client = nullptr;

public:
    RedisClient(const nlohmann::json &value);
    ~RedisClient();
    /**
     * @description: connect redis server with ip and port
     * @return {bool} connect status
     */
    bool connect();
    /**
     * @description: exec order to redis server, then redis will reply. Wrap reply
     * @param {RedisReplyWrap} &reply :redis server reply wrap
     * @param {string} &order: order of some redis operations
     */
    void exeCommand(RedisReplyWrap &reply, const std::string &order);
};
