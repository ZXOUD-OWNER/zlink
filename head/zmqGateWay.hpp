/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240620
 * FilePath: /zlink/head/zmqGateWay.hpp
 * Description: ZeroMQ gateway
 */
#pragma once
#include "system.hpp"

class ZmqGateway
{
private:
    zsock_t *_sock = nullptr;
    zactor_t *_actor = nullptr;
    zpoller_t *_poller = nullptr;
    std::string _routerIP;
    int _routerPort;

public:
    ZmqGateway(const nlohmann::json &value);
    ~ZmqGateway();
    /**
     * @description: ZeroMQ worker threads are used to receive requests and send back responses
     * @param {zsock_t} *pipe: descriptors for communicating with worker threads
     * @param {void} *args: passing the 'this' pointer to worker
     */
    static void worker(zsock_t *pipe, void *args);
    /**
     * @description: satrt worker thread
     */
    void start();
};
