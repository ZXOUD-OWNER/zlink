#include "head.hpp"

interactionLogic::interactionLogic(const nlohmann::json &Conf)
    : _redis(Conf)
{
    auto _logicConf = Conf.find("LogicConf").value().object();
}

interactionLogic::~interactionLogic()
{
}
