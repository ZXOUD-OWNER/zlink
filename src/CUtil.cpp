#include "head.hpp"

std::string CUtil::ConstructResponseMsgRedis(const nlohmann::json &req)
{
    thread_local interactionLogic interaction(Singleton::getInstance().GetConf());
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
        interaction.exeOrder<Redis::VerifyCheckCode>(req, resJson);
        break;

    default:
        LOG(ERROR) << "The command corresponding to Redis parsing logic has not been developed.order is " << orderStr;
        break;
    }

    return std::string(resJson.dump());
}

std::string CUtil::Print_trace()
{
    unw_cursor_t cursor;
    unw_context_t context;
    std::string temp;
    // Init context
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
