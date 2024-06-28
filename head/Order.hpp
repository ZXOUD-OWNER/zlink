/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: A529yyds(email:1041389196@qq.com)
 * create: 20240620
 * FilePath: /zlink/head/Order.hpp
 * Description: send command to redis or PostgreSQL, then converted reply to json data
 */
#pragma once
#include "system.hpp"
#include "redisClient.hpp"
#include "pgsqlClient.hpp"

namespace redis
{
    class VerifyCheckCode : NonCopyable
    {
    private:
        nlohmann::json _order;
        nlohmann::json _result;
        static std::string _redisResponse;

    public:
        /**
         * @description: construct json response of verifying checkcode to redis
         * @param {json} &order: verify check code
         * @param {RedisClient} &memoryData: redis client
         * @return {json}: json data converted from redis verity results
         */
        nlohmann::json constructResponse(const nlohmann::json &order, RedisClient &memoryData);
        VerifyCheckCode();
        inline ~VerifyCheckCode()
        {
        }
    };

    class SetCheckCode : NonCopyable
    {
    private:
        nlohmann::json _order;
        nlohmann::json _result;
        static std::string _redisResponse;

    public:
        /**
         * @description: construct json response of setting checkcode to redis
         * @param {json} &order: set command
         * @param {RedisClient} &memoryData: redis client
         * @return {json}:json data converted from redis set results
         */
        nlohmann::json constructResponse(const nlohmann::json &order, RedisClient &memoryData);
        SetCheckCode();
        inline ~SetCheckCode()
        {
        }
    };
}

namespace pgsql
{
    class PgSqlOperation
    {
    protected:
        nlohmann::json _order;
        nlohmann::json _result;

    protected:
        /**
         * @description: some exec operations to PostgreSQL, used in specific operation as common interface
         * @param {string} cmd: PostgreSQL exec command
         * @param {PgsqlClient} &memoryData: PostgreSQL client
         * @param {result} &reply: reply from PostgreSQL server, most cases is a row of data
         * @return {bool}: result as follows, success/fail
         */
        bool sqlExec(std::string cmd, PgsqlClient &memoryData, pqxx::result &reply);

    public:
        virtual nlohmann::json constructResponse(const nlohmann::json &order, PgsqlClient &memoryData) = 0;
        PgSqlOperation();
        virtual ~PgSqlOperation()
        {
        }
    };

    class Register : public NonCopyable, public PgSqlOperation
    {
    private:
        static std::string _sqlResponse;

    public:
        /**
         * @description: construct json response of register to PostgreSQL
         * @param {json} &order: register order as json
         * @param {PgsqlClient} &memoryData: PostgreSQL client
         * @return {json} reply
         */
        nlohmann::json constructResponse(const nlohmann::json &order, PgsqlClient &memoryData);
        Register();
        inline ~Register()
        {
        }
    };

    class Login : public NonCopyable, public PgSqlOperation
    {
    private:
        static std::string _sqlResponse;

    public:
        /**
         * @description: construct json response of login to PostgreSQL
         * @param {json} &order: login order as json
         * @param {PgsqlClient} &memoryDataa: PostgreSQL client
         * @return {json} PostgreSQL reply
         */
        nlohmann::json constructResponse(const nlohmann::json &order, PgsqlClient &memoryData);
        Login();
        inline ~Login()
        {
        }
    };

    class ChangerPassword : public NonCopyable, public PgSqlOperation
    {
    private:
        static std::string _sqlResponse;

    public:
        /**
         * @description: construct json response of changer password to PostgreSQL
         * @param {json} &order: change user table infos like password order as json
         * @param {PgsqlClient} &memoryData: PostgreSQL client
         * @return {json} PostgreSQL reply
         */
        nlohmann::json constructResponse(const nlohmann::json &order, PgsqlClient &memoryData);
        ChangerPassword();
        inline ~ChangerPassword()
        {
        }
    };

}