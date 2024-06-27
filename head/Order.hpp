#pragma once
#include "system.hpp"
#include "redisClient.hpp"
#include "pgsqlClient.hpp"
// The parse function is used to provide the execution statement for the corresponding database
// constructResponse is used to construct the response or result of the database execution statement

namespace Redis
{
    class VerifyCheckCode : NonCopyable
    {
    private:
        nlohmann::json _order;
        nlohmann::json _result;
        static std::string _redisResponse;

    public:
        nlohmann::json constructResponse(const nlohmann::json &order, redisClient &memoryData);
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
        nlohmann::json constructResponse(const nlohmann::json &order, redisClient &memoryData);
        SetCheckCode();
        inline ~SetCheckCode()
        {
        }
    };
}

namespace SQL
{
    class SQLOperation
    {
    protected:
        nlohmann::json _order;
        nlohmann::json _result;
    protected:
        bool isOrderRight(const nlohmann::json &order, std::string orType);
        bool sqlExec(std::string sql, pgsqlClient &memoryData, pqxx::result &reply);
    public:
        virtual nlohmann::json constructResponse(const nlohmann::json &order, pgsqlClient &memoryData) = 0;
        SQLOperation();
        virtual ~SQLOperation()
        {
        }
    };

    class Register : public NonCopyable, public SQLOperation
    {
    private:
        static std::string _sqlResponse;
    public:
        nlohmann::json constructResponse(const nlohmann::json &order, pgsqlClient &memoryData);
        Register();
        inline ~Register()
        {
        }
    };

    class Login : public NonCopyable, public SQLOperation
    {
    private:
        static std::string _sqlResponse;
    public:
        nlohmann::json constructResponse(const nlohmann::json &order, pgsqlClient &memoryData);
        Login();
        inline ~Login()
        {
        }
    };

    class ChangerPassword : public NonCopyable, public SQLOperation
    {
    private:
        static std::string _sqlResponse;
        
    public:
        nlohmann::json constructResponse(const nlohmann::json &order, pgsqlClient &memoryData);
        ChangerPassword();
        inline ~ChangerPassword()
        {
        }
    };

}