#include "head.hpp"

zmqGateWay::zmqGateWay(const nlohmann::json &value)
{
    if (value.find("RouterIP") == value.end() || value.find("RouterPort") == value.end())
    {
        LOG(FATAL) << "config.json not config zmqGateWay info";
    }
    _routerIP = value.find("RouterIP").value().get<std::string>();
    _routerPort = value.find("RouterPort").value().get<int>();
    start();
}

zmqGateWay::~zmqGateWay()
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

void zmqGateWay::worker(zsock_t *pipe, void *args)
{
    auto &zmqGate = *(static_cast<zmqGateWay *>(args));
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

        zmsg_t *old_msg = zmsg_recv(zmqGate._sock);
        if (!old_msg)
            break; //  Interrupted

        std::string content;
        // content.append("hello hundang kkkkkkk");
        zmsg_t *new_msg = zmsg_new();
        zframe_t *frame1 = zmsg_pop(old_msg);
        zframe_t *frame2 = zmsg_pop(old_msg);
        // zframe_t *frame3 = zmsg_pop(old_msg);
        CUtil::GetZMsg(old_msg, content);
        auto json = nlohmann::json::parse(content);
        std::string msg;
        switch (Singleton::getInstance().GetDataBaseType())
        {
        case 0:
            msg = CUtil::ConstructResponseMsgRedis(json);
            break;
        case 1:

        default:
            LOG(FATAL) << "not support specify dataBase" << " func stack is " << CUtil::Print_trace();
            break;
        }

        // auto k = zmsg_size(old_msg);
        zmsg_append(new_msg, &frame1);
        zmsg_append(new_msg, &frame2);

        zframe_t *frame4 = zframe_new(msg.c_str(), msg.length());
        zmsg_append(new_msg, &frame4); // can not use zmsg_pushstr,has problem

        zmsg_send(&new_msg, zmqGate._sock);
    }
}

void zmqGateWay::start()
{
    _actor = zactor_new(worker, this);
}
