/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240620
 * FilePath: /zlink/head/interactionLogic.hpp
 * Description: order operation entry
 */
#pragma once
#include "system.hpp"
#include "Singleton.hpp"
template <typename B>
class InteractionLogic : NonCopyable
{
private:
    nlohmann::json _logicConf;
    B _dataBase;

public:
    inline InteractionLogic(const nlohmann::json &conf)
        : _dataBase(conf)
    {
        _logicConf = conf.find("LogicConf").value();
    }
    inline ~InteractionLogic()
    {
    }
    /**
     * @description: exec the template T's order.The class T must define one functions, it is nlohmann::json constructResponse(RedisReplyWrap)
     * @param {json} &order: exec operation
     * @param {json} &result: server reply result
     */
    template <typename T>
    void inline exeOrder(const nlohmann::json &order, nlohmann::json &result)
    {
        T orderParse;
        result = orderParse.constructResponse(order, _dataBase);
    }
};
