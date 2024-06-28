#include "head.hpp"

PgSqlClient::PgSqlClient(const nlohmann::json &value)
    : _conn(PgSqlClient::parse(value))
{
    if (_conn.is_open())
    {
        LOG(INFO) << "database connect success! str is " << _connectStr;
    }
    else
    {
        LOG(FATAL) << "database connect FATAL! str is " << _connectStr;
    }
}

PgSqlClient::PgSqlClient(std::string_view view)
{
    _connectStr = view;
    _conn = std::move(pqxx::connection(_connectStr));
    if (_conn.is_open())
    {
        LOG(INFO) << "database connect success! str is " << _connectStr;
    }
    else
    {
        LOG(FATAL) << "database connect FATAL! str is " << _connectStr;
    }
}

PgSqlClient::~PgSqlClient()
{
}

std::string PgSqlClient::parse(const nlohmann::json &value)
{
    std::string conn;
    auto iter = value.find("SQLDataBase");
    if (iter != value.end())
    {
        auto pgsqlIter = iter.value().find("Postgres");
        if (pgsqlIter != iter.value().end())
        {
            auto postgresIpIter = pgsqlIter.value().find("PostgresIP");
            auto postgresPortIter = pgsqlIter.value().find("PostgresPort");
            auto dbNameIter = pgsqlIter.value().find("dbname");
            auto userIter = pgsqlIter.value().find("user");
            auto passwdIter = pgsqlIter.value().find("passwd");
            if (postgresIpIter == pgsqlIter.value().end() || postgresPortIter == pgsqlIter.value().end() || dbNameIter == pgsqlIter.value().end() || userIter == pgsqlIter.value().end())
            {
                LOG(FATAL) << "sql conf not found necessary info";
            }
            else
            {
                if (passwdIter == pgsqlIter.value().end())
                {
                    conn += "dbname=" + dbNameIter.value().get<std::string>() + " user=" + userIter.value().get<std::string>() + " hostaddr=" + postgresIpIter.value().get<std::string>() + " port=" + std::to_string(postgresPortIter.value().get<int>());
                }
                else
                {
                    conn += "dbname=" + dbNameIter.value().get<std::string>() + " user=" + userIter.value().get<std::string>() + " hostaddr=" + postgresIpIter.value().get<std::string>() + " port=" + std::to_string(postgresPortIter.value().get<int>()) + " password=" + postgresPortIter.value().get<std::string>();
                }
            }
        }
        else
        {
            LOG(FATAL) << "sql conf not found Postgres";
        }
    }
    else
    {
        LOG(FATAL) << "sql conf not found ";
    }
    return conn;
}

pqxx::result PgSqlClient::execCommandOneSql(std::string_view view)
{
    pqxx::result res;
    pqxx::nontransaction work(_conn);
    res = work.exec(view);
    return res;
}
