#pragma once
#include "system.hpp"

class zmqGateWay
{
private:
    zsock_t *_sock = nullptr;
    zactor_t *_actor = nullptr;
    zpoller_t *_poller = nullptr;
    std::string _routerIP;
    int _routerPort;

public:
    zmqGateWay(const nlohmann::json &value);
    ~zmqGateWay();
    static void worker(zsock_t *pipe, void *args);
    void start();
};
