#include <QCoreApplication>

#include <memory>

#include "PfAdapterManager/pfadaptermanager.h"

#include <QDateTime>
#include <iostream>
#include <unistd.h>

/**
 * @brief 适配器数据收发例程
 * @example PfAdapter/main.cpp
 */

using namespace Pf;

void test()
{   
    //step1：实例化
    std::shared_ptr<PfAdapter::PfAdapterManager> obj = std::make_shared<PfAdapter::PfAdapterManager>();

    //step2：初始化配置文件
    obj->init("./devcfg.xml");

    //step3：获取适配器句柄
    PfAdapter::Adapter *ada = nullptr;

    std::string id = "cmdSend";

    if(obj->getAdapter(id, &ada))
    {        
        //step4：总线收发数据
        std::string msg = "hello word ";
        if(ada->sendMsg(msg.c_str(), msg.size()))
        {
            std::cout << "send ok" << std::endl;
        }
        else
        {
            std::cout << "send error" << std::endl;
        }


        char str[1024] = {0};        
        int rcvSize=0;
        int maxRcvSize = 1024;
        unsigned int timeOut = 500;

        while(1)
        {            
            if(ada->receiveMsg(str, rcvSize, maxRcvSize, timeOut))
            {
                std::cout << "receive ok " << str << rcvSize << std::endl;
            }
            else
            {
                std::cout << "receive error" << std::endl;
            }

            for(int index = 0; index < 0xFFFFFFF; index++);
        }
    }
    else
    {
        std::cout << "get adapter error id(" << id << ")" << std::endl;
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
        std::cout << err.what() << std::endl;
    }

    return a.exec();
}
