/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240622
 * FilePath: /zlink/src/CUtil.cpp
 * Description:
 */
#include "head.hpp"

std::string CUtil::constructResponseMsgRedis(const nlohmann::json &req)
{
    thread_local InteractionLogic<RedisClient> interaction(Singleton::getInstance().getConf());
    nlohmann::json resJson;

    auto orderIter = req.find("order");
    if (orderIter == req.end())
    {
        LOG(ERROR) << "req has err! not found specify field(order). req is " << req.dump();
        return std::string("-1");
    }
    std::string orderStr = orderIter.value().get<std::string>();
    const auto &orderMap = Singleton::getInstance().GetRedisOrder();
    auto iter = orderMap.find(orderStr);
    if (iter == orderMap.end())
    {
        LOG(ERROR) << "The command corresponding to Redis parsing logic has not been developed.order is " << orderStr;
        return std::string("-1");
    }

    switch (iter->second)
    {
    case 0:
        interaction.exeOrder<redis::VerifyCheckCode>(req, resJson);
        break;
    case 1:
        interaction.exeOrder<redis::SetCheckCode>(req, resJson);
        break;
    case 2:
        interaction.exeOrder<redis::CheckAccount>(req, resJson);
        break;
    case 3:
        interaction.exeOrder<redis::CheckCodeExist>(req, resJson);
        break;
    default:
        LOG(ERROR) << "The command corresponding to Redis parsing logic has not been developed.order is " << orderStr;
        break;
    }

    return std::string(resJson.dump());
}

std::string CUtil::constructResponseMsgPgSQL(const nlohmann::json &req)
{
    thread_local InteractionLogic<PgsqlClient> interaction(Singleton::getInstance().getConf());
    nlohmann::json resJson;

    auto orderIter = req.find("order");
    if (orderIter == req.end())
    {
        LOG(ERROR) << "req has err! not found specify field(order). req is " << req.dump();
        return std::string("-1");
    }
    std::string orderStr = orderIter.value().get<std::string>();
    const auto &orderMap = Singleton::getInstance().GetRedisOrder();
    auto iter = orderMap.find(orderStr);
    if (iter == orderMap.end())
    {
        LOG(ERROR) << "The command corresponding to Redis parsing logic has not been developed.order is " << orderStr;
        return std::string("-1");
    }

    switch (iter->second)
    {
    case 0:
        interaction.exeOrder<pgsql::Register>(req, resJson);
        break;
    case 1:
        interaction.exeOrder<pgsql::Login>(req, resJson);
        break;
    case 2:
        interaction.exeOrder<pgsql::ChangerPassword>(req, resJson);
        break;
    default:
        LOG(ERROR) << "The command corresponding to Redis parsing logic has not been developed.order is " << orderStr;
        break;
    }

    return std::string(resJson.dump());
}

std::string CUtil::printTrace()
{
    unw_cursor_t cursor;
    unw_context_t context;
    std::string temp;
    // init context
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    // traverse the call stack
    while (unw_step(&cursor) > 0)
    {
        unw_word_t offset, pc;
        char fname[64];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        fname[0] = '\0';
        unw_get_proc_name(&cursor, fname, sizeof(fname), &offset);
        temp.append(fname);
        temp.append("() ");
        temp.append("+0x");
        temp.append(std::to_string(offset));
        temp.append("\n");
    }
    return temp;
}
