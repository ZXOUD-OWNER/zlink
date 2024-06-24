#pragma once
#include "system.hpp"
#include "Singleton.hpp"

class interactionLogic : NonCopyable
{
private:
    nlohmann::json _logicConf;
    redisClient _redis;

public:
    interactionLogic(const nlohmann::json &Conf);
    ~interactionLogic();
    // The class T must define two functions, one of which is std::string parse(const nlohmann::json &)，orther one is nlohmann::json constructResponse(redisReplyWrap)
    template <typename T>
    void inline exeOrder(const nlohmann::json &order, nlohmann::json &result)
    {
        T OrderParse;
        result = OrderParse.constructResponse(order, _redis);
    }

    void exeOrder(const nlohmann::json &order, nlohmann::json &result);
};
