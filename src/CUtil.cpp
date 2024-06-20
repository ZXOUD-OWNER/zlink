#include "head.hpp"

std::string CUtil::ConstructResponseMsgRedis(const nlohmann::json &req)
{
    thread_local interactionLogic interaction(Singleton::getInstance().GetConf());
    nlohmann::json resJson;
    interaction.exeOrder<Order::ServerInit>(req, resJson);
    std::string res(resJson.dump());
    return res;
}