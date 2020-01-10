#include <QCoreApplication>

#include "niserialport.h"
#include <iostream>

#include <memory>

/**
 * @brief 串口收发例程（NIVisa)
 * @example PfNi/serialPortDemo/main.cpp
 */

using namespace Pf;

void test()
{
    //step1：实例化
    std::shared_ptr<PfNi::NiSerialPort> obj = std::make_shared<PfNi::NiSerialPort>();

    //step2：初始化
    obj->init("COM2", "NONE", 115200, 8, "1");

    //step3：发送数据
    std::string msg = "hello word";
    if(!obj->sendMsg((unsigned char*)msg.c_str(), msg.size()))
    {
        std::cout << "send error" << std::endl;
    }

    //step3：接收数据
    unsigned char rBuf[1024] = {0};
    unsigned int rSize = 0;
    if(obj->receiveMsg(rBuf, &rSize))
    {
        std::string sMsg((const char*)rBuf, rSize);

        std::cout << " rcv ok -> " << sMsg << std::endl;
    }
    else
    {
        std::cout << "rcv error" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        test();
    }
    catch(std::runtime_error err)
    {
        std::cout << err.what();
    }


    return a.exec();
}
