#include "head.hpp"

int main(int argc, char **argv)
{
    Log_MQ mq(argv[0]);
    std::vector<zmqGateWay> zGateArr;
    auto count = Singleton::getInstance().GetConf().find("BackendDealerThreadNumber").value().get<int>();
    for (size_t i = 0; i < count; i++)
    {
        zGateArr.emplace_back(Singleton::getInstance().GetConf());
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}