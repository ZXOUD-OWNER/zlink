#include "head.hpp"

std::string Redis::VerifyCheckCode::_redisResponse("{ \
    \"sender\":\"redisService\", \
    \"receive\":\"web\",\
    \"level\":\"user\",\
    \"success\":\"\",\
    \"correct\":\"\",\
    \"result\":\"verifyCheckCode\"\
}");

Redis::VerifyCheckCode::VerifyCheckCode()
{
}

nlohmann::json Redis::VerifyCheckCode::constructResponse(const nlohmann::json &order, redisClient &memoryData)
{
    auto Iter = order.find("CheckCode");
    auto Iter2 = order.find("CheckCodeKeyName");
    nlohmann::json result;
    result = nlohmann::json::parse(_redisResponse);

    if (Iter == order.end() || Iter2 == order.end())
    {
        LOG(ERROR) << "not found CheckCode;order is " << order.dump();
        //....please construct err response Json

        result["success"] = 0;
        result["correct"] = 0;
        return result;
    }

    std::string checkcode = Iter.value().get<std::string>();
    std::string CheckCodeKeyName = Iter2.value().get<std::string>();
    redisReplyWrap redisReply;

    std::string str = "Get ";
    str += CheckCodeKeyName;
    memoryData.exeCommand(redisReply, str);
    if (redisReply.reply == nullptr)
    {
        //....please construct err response Json
        result["success"] = 0;
        result["correct"] = 0;
        return result;
    }

    std::string redisContainCheckCode;
    switch (redisReply.reply->type)
    {
    case REDIS_REPLY_STRING:
        redisContainCheckCode.append(redisReply.reply->str);
        break;
    default:
        LOG(ERROR) << "redis order exec err! order is " << str << CUtil::Print_trace();
        result["success"] = 0;
        result["correct"] = 0;
        return result;
    }

    result["success"] = 1;
    if (checkcode == redisContainCheckCode)
    {
        result["correct"] = 1;
    }
    else
    {
        result["correct"] = 0;
    }

    return result;
}