#include "head.hpp"
#include "Order.hpp"

std::string redis::VerifyCheckCode::_redisResponse("{ \
    \"sender\":\"redisService\", \
    \"receive\":\"web\",\
    \"level\":\"user\",\
    \"success\":\"\",\
    \"correct\":\"\",\
    \"result\":\"verifyCheckCode\"\
}");

std::string redis::SetCheckCode::_redisResponse("{ \
    \"sender\":\"redisService\", \
    \"receive\":\"web\",\
    \"level\":\"user\",\
    \"success\":\"\",\
    \"result\":\"SetCheckCode\"\
}");

std::string pgsql::Register::_sqlResponse("{ \
    \"sender\":\"dataBaseService\", \
    \"receive\":\"web\",\
    \"level\":\"user\",\
    \"email\":\"\",\
    \"result\":\"register\",\
    \"success\":0\
}");

std::string pgsql::Login::_sqlResponse("{ \
    \"sender\":\"dataBaseService\", \
    \"receive\":\"web\",\
    \"level\":\"user\",\
    \"result\":\"login\",\
    \"success\":0,\
    \"correct\":0\
}");

std::string pgsql::ChangerPassword::_sqlResponse("{ \
    \"sender\":\"dataBaseService\", \
    \"receive\":\"web\",\
    \"level\":\"user\",\
    \"email\":\"\",\
    \"result\":\"ChangerPassword\",\
    \"success\":0\
}");

redis::VerifyCheckCode::VerifyCheckCode()
{
}

nlohmann::json redis::VerifyCheckCode::constructResponse(const nlohmann::json &order, RedisClient &memoryData)
{
    auto iter = order.find("CheckCode");
    auto iter2 = order.find("CheckCodeKeyName");
    nlohmann::json result;
    result = nlohmann::json::parse(_redisResponse);
    if (iter == order.end() || iter2 == order.end())
    {
        LOG(ERROR) << "not found CheckCode;order is " << order.dump();
        //....please construct err response Json
        result["success"] = 0;
        result["correct"] = 0;
        return result;
    }
    std::string checkcode = iter.value().get<std::string>();
    std::string checkCodeKeyName = iter2.value().get<std::string>();
    RedisReplyWrap redisReplyTemp;
    std::string str = "Get ";
    str += checkCodeKeyName;
    memoryData.exeCommand(redisReplyTemp, str);
    if (redisReplyTemp._reply == nullptr)
    {
        //....please construct err response Json
        result["success"] = 0;
        result["correct"] = 0;
        return result;
    }

    std::string redisContainCheckCode;
    switch (redisReplyTemp._reply->type)
    {
    case REDIS_REPLY_STRING:
        redisContainCheckCode.append(redisReplyTemp._reply->str);
        break;
    default:
        LOG(ERROR) << "redis order exec err! order is " << str << CUtil::printTrace();
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

redis::SetCheckCode::SetCheckCode()
{
}

nlohmann::json redis::SetCheckCode::constructResponse(const nlohmann::json &order, RedisClient &memoryData)
{
    auto iter = order.find("CheckCode");
    auto iter2 = order.find("CheckCodeKeyName");
    nlohmann::json result;
    result = nlohmann::json::parse(_redisResponse);
    result["success"] = 0;
    if (iter == order.end() || iter2 == order.end())
    {
        LOG(ERROR) << "not found CheckCode;order is " << order.dump();
        //....please construct err response Json
        return result;
    }
    std::string checkcode = iter.value().get<std::string>();
    std::string checkCodeKeyName = iter2.value().get<std::string>();
    RedisReplyWrap redisReplyTemp;
    std::string cmd = std::format("SET {} {}", checkCodeKeyName, checkcode);
    memoryData.exeCommand(redisReplyTemp, cmd);
    if (redisReplyTemp._reply && strcmp(redisReplyTemp._reply->str, "OK") == 0)
    {
        //....please construct err response Json
        result["success"] = 1;
    }
    return result;
}

pgsql::PgSqlOperation::PgSqlOperation()
{
}

pgsql::Register::Register()
{
}

bool pgsql::PgSqlOperation::sqlExec(std::string cmd, PgsqlClient &memoryData, pqxx::result &reply)
{
    bool ret = false;
    // try exec a command to pgsql, if error, throw command error and others
    try
    {
        reply = memoryData.execCommandOneSql(cmd);
        ret = true;
    }
    catch (const pqxx::sql_error &e)
    {
        LOG(ERROR) << "SQL error: " << e.what() << std::endl;
        LOG(ERROR) << "Query was: " << e.query() << std::endl;
    }
    catch (const std::exception &e)
    {
        LOG(ERROR) << "Error: " << e.what() << std::endl;
    }
    return ret;
}

nlohmann::json pgsql::Register::constructResponse(const nlohmann::json &order, PgsqlClient &memoryData)
{
    nlohmann::json result;
    auto iter0 = order.find("username");
    auto iter1 = order.find("password");
    auto iter2 = order.find("email");
    auto iter3 = order.find("space_total");
    auto iter4 = order.find("used_space");
    std::string username = iter0.value().get<std::string>();
    std::string password = iter1.value().get<std::string>();
    std::string email = iter2.value().get<std::string>();
    std::string space_total = iter3.value().get<std::string>();
    std::string used_space = iter4.value().get<std::string>();
    result = nlohmann::json::parse(_sqlResponse);
    result["email"] = email;
    result["success"] = 0;
    if (iter0 == order.end() || iter1 == order.end() || iter2 == order.end() || iter3 == order.end() || iter4 == order.end())
    {
        LOG(ERROR) << "The required fields are blank, please check!Cur order is " << order.dump();
        return result;
    }
    // insert infos to users table, and get the new row's id
    std::string cmd = std::format("INSERT INTO users (username, password, email) VALUES ('{}', '{}', '{}') RETURNING id;", username, password, email);
    pqxx::result reply;
    if (sqlExec(cmd, memoryData, reply))
    {
        int user_id = reply.size() > 0 ? reply[0][0].as<int>() : 0;
        // insert infos to usersResource table
        cmd = std::format("INSERT INTO usersResource (user_id, space_total, used_space) VALUES ({}, '{}', '{}');", user_id, space_total, used_space);
        result["success"] = sqlExec(cmd, memoryData, reply);
    }
    return result;
}

pgsql::Login::Login()
{
}

nlohmann::json pgsql::Login::constructResponse(const nlohmann::json &order, PgsqlClient &memoryData)
{
    nlohmann::json result;
    auto iter0 = order.find("email");
    result = nlohmann::json::parse(_sqlResponse);
    result["success"] = 0;
    result["correct"] = 0;
    if (iter0 == order.end())
    {
        LOG(ERROR) << "The required fields are blank, please check!Cur order is " << order.dump();
        return result;
    }
    std::string email = iter0.value().get<std::string>();
    // select the password corresponding to the email
    std::string cmd = std::format("select * from users where email = '{}';", email);
    pqxx::result reply;
    if (sqlExec(cmd, memoryData, reply))
    {
        result["success"] = 1;
        std::string searchPwd = reply.size() > 0 ? reply[0][2].as<std::string>() : "";
        if (searchPwd == order.find("password").value().get<std::string>())
        {
            result["correct"] = 1;
        }
    }
    return result;
}

pgsql::ChangerPassword::ChangerPassword()
{
}

nlohmann::json pgsql::ChangerPassword::constructResponse(const nlohmann::json &order, PgsqlClient &memoryData)
{
    nlohmann::json result;
    auto iter0 = order.find("email");
    auto iter1 = order.find("password");
    std::string email = iter0.value().get<std::string>();
    std::string password = iter1.value().get<std::string>();
    result = nlohmann::json::parse(_sqlResponse);
    result["email"] = email;
    result["success"] = 0;
    if (iter0 == order.end())
    {
        LOG(ERROR) << "The required fields are blank, please check!Cur order is " << order.dump();
        return result;
    }
    // update users table row;
    std::string cmd = std::format("update users set password = '{}' where email = '{}' RETURNING *;", password, email);
    pqxx::result reply;
    if (sqlExec(cmd, memoryData, reply))
    {
        std::string searchPwd = reply.size() > 0 ? reply[0][2].as<std::string>() : "";
        if (searchPwd == password)
        {
            result["success"] = 1;
        }
    }
    return result;
}