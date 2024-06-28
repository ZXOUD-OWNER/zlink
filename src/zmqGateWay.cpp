#include "head.hpp"

ZmqGateway::ZmqGateway(const nlohmann::json &value)
{
    if (value.find("RouterIP") == value.end() || value.find("RouterPort") == value.end())
    {
        LOG(FATAL) << "config.json not config ZmqGateway info";
    }
    _routerIP = value.find("RouterIP").value().get<std::string>();
    _routerPort = value.find("RouterPort").value().get<int>();
    start();
}

ZmqGateway::~ZmqGateway()
{
    if (_poller != nullptr)
    {
        zpoller_destroy(&_poller);
    }
    _poller = nullptr;
    if (_sock != nullptr)
    {
        zsock_destroy(&_sock);
    }
    _sock = nullptr;
    if (_actor != nullptr)
    {
        zactor_destroy(&_actor);
    }
    _actor = nullptr;
}

void ZmqGateway::worker(zsock_t *pipe, void *args)
{
    auto &zmqGate = *(static_cast<ZmqGateway *>(args));
    zsock_signal(pipe, 0);
    std::string tcpStr = "tcp://";
    tcpStr += zmqGate._routerIP + ":" + std::to_string(zmqGate._routerPort);
    zmqGate._sock = zsock_new_req(tcpStr.c_str());
    if (zmqGate._sock == nullptr)
    {
        LOG(ERROR) << "connect zio err!";
        return;
    }
    zmqGate._poller = zpoller_new(pipe, zmqGate._sock, NULL);
    zpoller_set_nonstop(zmqGate._poller, true);
    //  Tell broker we're ready for work
    zframe_t *frame = zframe_new(WORKER_READY, 1);
    zframe_send(&frame, zmqGate._sock, 0);
    //  Process messages as they arrive
    while (true)
    {
        zsock_t *ready = static_cast<zsock_t *>(zpoller_wait(zmqGate._poller, -1));
        if (ready == NULL)
            continue; // Interrupted
        else if (ready == pipe)
            break; // Shutdown
        else
            assert(ready == zmqGate._sock); // Data Available

        zmsg_t *oldMsg = zmsg_recv(zmqGate._sock);
        if (!oldMsg)
            break; // Interrupted
        std::string content;
        zmsg_t *newMsg = zmsg_new();
        zframe_t *frame1 = zmsg_pop(oldMsg);
        zframe_t *frame2 = zmsg_pop(oldMsg);
        CUtil::getZMsg(oldMsg, content);
        auto json = nlohmann::json::parse(content);
        std::string msg;
        switch (Singleton::getInstance().getDataBaseType())
        {
        case 0:
            msg = CUtil::constructResponseMsgRedis(json);
            break;
        case 1:
            msg = CUtil::constructResponseMsgPgSQL(json);
        default:
            LOG(FATAL) << "not support specify dataBase" << " func stack is " << CUtil::printTrace();
            break;
        }
        zmsg_append(newMsg, &frame1);
        zmsg_append(newMsg, &frame2);
        zframe_t *frame4 = zframe_new(msg.c_str(), msg.length());
        zmsg_append(newMsg, &frame4); // can not use zmsg_pushstr,has problem
        zmsg_send(&newMsg, zmqGate._sock);
    }
}

void ZmqGateway::start()
{
    _actor = zactor_new(worker, this);
}
