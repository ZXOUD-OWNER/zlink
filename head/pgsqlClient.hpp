/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240626
 * FilePath: /zlink/head/pgsqlClient.hpp
 * Description: PostgreSQL client command exec
 */
#pragma once
#include "system.hpp"

class PgsqlClient
{
private:
    std::string _connectStr;
    pqxx::connection _conn;

private:
    /**
     * @description: parse json data to SQL statement
     * @return {string} one SQL statement
     */
    static std::string parse(const nlohmann::json &value);

public:
    PgsqlClient(const nlohmann::json &value);
    PgsqlClient(std::string_view view);
    /**
     * @description: exec one SQL command to PostgreSQL server
     * @param {string_view} view: one SQL statements
     * @return {pqxx::result} a row data of PostgreSQL reply
     */
    pqxx::result execCommandOneSql(std::string_view view);
    ~PgsqlClient();
};
