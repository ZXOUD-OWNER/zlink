#include "head.hpp"
#include <mimalloc-new-delete.h>

int main(int argc, char **argv)
{
    LogMQ mq(argv[0]);
    std::vector<ZmqGateway> zGateArr;
    auto count = Singleton::getInstance().getConf().find("BackendDealerThreadNumber").value().get<int>();
    for (int i = 0; i < count; i++)
    {
        zGateArr.emplace_back(Singleton::getInstance().getConf());
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}