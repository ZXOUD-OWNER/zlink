/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240622
 * FilePath: /zlink/head/Singleton.hpp
 * Description: initial server connect configuration from config.json
 */
#pragma once
#include "system.hpp"
#include "Order.hpp"

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
    //@description: init redis order type as a map
    void initRedisOrder();

public:
    /**
     * @description: get Singleton instance
     * @return {Singleton} an instance
     */
    inline static Singleton &getInstance()
    {
        static Singleton instance;
        return instance;
    }
    /**
     * @description: get the contexts of the server configuration file
     * @return {json} server configuration context
     */
    inline const auto &GetConf()
    {
        return _conf;
    }
    /**
     * @description: get data base type
     * @return {int} database type
     */
    inline int GetDataBaseType()
    {
        return _EnableDataBaseType;
    }
    /**
     * @description: get redis order
     * @return {std::unordered_map<std::string, int>} redis order map
     */
    inline const std::unordered_map<std::string, int> &GetRedisOrder()
    {
        return _redisOrder;
    }
};
