#pragma once
#include "system.hpp"
#include "Singleton.hpp"
template <typename B>
class interactionLogic : NonCopyable
{
private:
    nlohmann::json _logicConf;
    B _dataBase;

public:
    inline interactionLogic(const nlohmann::json &Conf)
        : _dataBase(Conf)
    {
        _logicConf = Conf.find("LogicConf").value();
    }
    inline ~interactionLogic()
    {
    }
    // The class T must define two functions, one of which is std::string parse(const nlohmann::json &)，orther one is nlohmann::json constructResponse(redisReplyWrap)
    template <typename T>
    void inline exeOrder(const nlohmann::json &order, nlohmann::json &result)
    {
        T OrderParse;
        result = OrderParse.constructResponse(order, _dataBase);
    }
};
