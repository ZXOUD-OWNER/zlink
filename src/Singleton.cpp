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

        auto IOThreadNumber = _conf.find("IOThreadNumber").value().get<int>();
        // Init global zmq of IO thread Numbers
        zsys_set_io_threads(IOThreadNumber);
        _EnableDataBaseType = _conf.find("EnableDataBaseType").value().get<int>();
    }
    catch (const std::exception &e)
    {
        LOG(FATAL) << "occur exception! err si " << e.what() << " func stack is " << CUtil::printTrace();
    }
};

/*
    Used to initialize the relationship between the instruction string and the number,
which is convenient for the exeOrder function in InteractionLogic of CUtil::constructResponseMsgRedis.
    The selection of class template parameters
*/
void Singleton::initRedisOrder()
{
    _redisOrder.reserve(100 * 2);
    _redisOrder["verifyCheckCode"] = 0;
}
