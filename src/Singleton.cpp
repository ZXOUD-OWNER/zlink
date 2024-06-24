#include "head.hpp"

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
            LOG(FATAL) << "config.json not has enough data" << " func stack is " << CUtil::Print_trace();
        }

        auto IOThreadNumber = _conf.find("IOThreadNumber").value().get<int>();
        zsys_set_io_threads(IOThreadNumber); // Init global zmq of IO thread Numbers
        _EnableDataBaseType = _conf.find("EnableDataBaseType").value().get<int>();
    }
    catch (const std::exception &e)
    {
        LOG(FATAL) << "occur exception! err si " << e.what() << " func stack is " << CUtil::Print_trace();
    }
};

// // Used to initialize the relationship between the instruction string and the number, which is convenient for the exeOrder function in interactionLogic of CUtil::ConstructResponseMsgRedis. The selection of class template parameters
void Singleton::initRedisOrder()
{
    _redisOrder.reserve(100 * 2);
    _redisOrder["verifyCheckCode"] = 0;

    // _redisOrder.insert(std::make_pair<std::string, RedisOrder>("verifyCheckCode", Redis::VerifyCheckCode()));
    // auto res = conf.find("MemoryDataBase");
    // if (res != conf.end())
    // {
    //     res = (*res).find("redis");
    //     if (res != conf.end())
    //     {
    //         res = (*res).find("Order");
    //         if (res != conf.end())
    //         {
    //             auto orderArr = res->array();
    //             int count = orderArr.size();
    //         }
    //     }
    // }
}
