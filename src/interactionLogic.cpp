#include "head.hpp"

interactionLogic::interactionLogic(const nlohmann::json &Conf)
    : _redis(Conf)
{
    auto _logicConf = Conf.find("LogicConf").value().object();
}

interactionLogic::~interactionLogic()
{
}

// void interactionLogic::exeOrder(const nlohmann::json &order, nlohmann::json &result)
// {
//     const std::unordered_map<std::string, RedisOrder> &redisOrder = Singleton::getInstance().GetRedisOrder();
//     auto orderIter = order.find("order");
//     if (orderIter == order.end())
//     {
//         LOG(ERROR) << "req has err! not found specify field(order). req is " << order.dump();
//         return;
//     }
//     std::string orderStr = orderIter.value().get<std::string>();

//     auto iter = redisOrder.find("orderStr");
//     if (iter != redisOrder.end())
//     {
//         }
//     else
//     {
//         LOG(ERROR) << "The command corresponding to Redis parsing logic has not been developed.order is " << orderStr;
//         return;
//     }
// }
