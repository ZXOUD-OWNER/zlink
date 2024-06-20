#include "head.hpp"

Order::UserInit::UserInit()
{
    auto &json = Singleton::getInstance().GetConf();
}

Order::UserInit::~UserInit()
{
}

Order::ServerInit::ServerInit()
{
}

nlohmann::json Order::ServerInit::constructResponse(redisReplyWrap &replyW)
{
    nlohmann::json value;
    return value;
}

std::string Order::ServerInit::parse(const nlohmann::json &order)
{
    std::string str;
    return str;
}
