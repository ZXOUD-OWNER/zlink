/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240622
 * FilePath: /zlink/src/Singleton.cpp
 * Description: initial server connect configuration from config.json
 */
#include "head.hpp"
#include "Singleton.hpp"

Singleton::Singleton()
{
    try
    {
        std::filesystem::path p("./config.json");
        std::ifstream ifs(p, std::ifstream::binary);
        std::string content((std::istreambuf_iterator<char>(ifs)), {});
        _conf = nlohmann::json::parse(content);
        if (_conf.find("EnableDataBaseType") == _conf.end() || _conf.find("IOThreadNumber") == _conf.end() || _conf.find("RouterIP") == _conf.end() || _conf.find("RouterPort") == _conf.end())
        {
            LOG(FATAL) << "config.json not has enough data" << " func stack is " << CUtil::printTrace();
        }
        auto ioThreadNumber = _conf.find("IOThreadNumber").value().get<int>();
        // Init global zmq of IO thread Numbers
        zsys_set_io_threads(ioThreadNumber);
        _enableDataBaseType = _conf.find("EnableDataBaseType").value().get<int>();
        initRedisOrder();
        initPgsqlOrder();
    }
    catch (const std::exception &e)
    {
        LOG(FATAL) << "occur exception! err si " << e.what() << " func stack is " << CUtil::printTrace();
    }
};

void Singleton::initRedisOrder()
{
    _redisOrder.reserve(100 * 2);
    _redisOrder["verifyCheckCode"] = 0;
    _redisOrder["SetCheckCode"] = 1;
    _redisOrder["CheckAccount"] = 2;
    _redisOrder["CheckCodeExist"] = 3;
}

void Singleton::initPgsqlOrder()
{
    _pgsqlOrder.reserve(100 * 2);
    _pgsqlOrder["Register"] = 0;
    _pgsqlOrder["Login"] = 1;
    _pgsqlOrder["ChangerPassword"] = 2;
}