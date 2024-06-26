#pragma once
#include "system.hpp"

class pgsqlClient
{
private:
    std::string _connectStr;
    nlohmann::json json;
    pqxx::connection _conn;

private:
    static std::string parse(const nlohmann::json &value);

public:
    pgsqlClient(const nlohmann::json &value);
    pgsqlClient(std::string_view view);
    pqxx::result execCommandOneSql(std::string_view view);
    ~pgsqlClient();
};
