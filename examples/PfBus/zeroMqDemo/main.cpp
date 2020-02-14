#include <QCoreApplication>

#include <memory>
#include <iostream>
#include <thread>
#include "zmqsend.h"

#include "zmqreceive.h"

/**
 * @brief zmq收发例程
 * @example PfBus/zeroMqDemo/main.cpp
 */

using namespace Pf;

void testSend(std::string rIp, std::string rPort)
{
    std::cout << "testZmqSend" << std::endl;

    //step1 实例化
    std::shared_ptr<PfBus::zmqSend> obj = std::make_shared<PfBus::zmqSend>();

    try
    {
        //step2 初始化
        obj->init(rIp, rPort);

        //step3 发送数据
        std::string msg = "hello word 123";
        if(obj->sendMsg((unsigned char*)msg.c_str(), msg.size()))
        {
            std::cout << " send ok " << std::endl;
        }
        else
        {
            std::cout << "send error" << std::endl;
        }
    }
    catch(std::runtime_error err)
    {
        std::cout << "init error -> " << err.what() << std::endl;
    }

}

void testRcv(std::string rIp, std::string rPort)
{
    std::cout << "testZmqRcv" << std::endl;

    //step1 实例化
    std::shared_ptr<PfBus::zmqReceive> obj = std::make_shared<PfBus::zmqReceive>();

    try
    {
        //step2 初始化
        obj->init(rIp, rPort);

        //step3 接收数据
        unsigned char rBuf[1024] = {0};
        unsigned int rSize = 0;
        if(obj->receiveMsg(rBuf, &rSize))
        {
            std::string sMsg((const char*)rBuf, rSize);

            std::cout << " rcv ok -> " << sMsg << std::endl;
        }
        else
        {
            std::cout << "receive error" << std::endl;
        }
    }
    catch(std::runtime_error err)
    {
        std::cout << "init error -> " << err.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //step1:启动接收
    std::thread exeThread([&]()
    {
        testRcv("*", "6001");
    });

    //step2 发送数据
    testSend("localhost", "6001");

    std::cout << "end" << std::endl;

    exeThread.join();

    return a.exec();
}
