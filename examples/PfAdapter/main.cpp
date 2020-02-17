#include <QCoreApplication>

#include <memory>

#include "PfAdapterManager/pfadaptermanager.h"

#include "../PfAdapter/AnalogInAdapter/AnalogInAdapter.h"
#include "../PfAdapter/AnalogOutAdapter/AnalogOutAdapter.h"
#include "../PfAdapter/DigitalIoAdapter/DigitalIoAdapter.h"
#include "../PfAdapter/DigitalOutAdapter/DigitalOutAdapter.h"
#include "../PfAdapter/RelayCtrlAdapter/RelayCtrlAdapter.h"
//#include "../PfNi/analogIn/analogin.h"

#include <QDateTime>
#include <iostream>
#include <unistd.h>

using namespace Pf;

using float64 = double;

//step1：实例化
std::shared_ptr<PfAdapter::PfAdapterManager> obj = std::make_shared<PfAdapter::PfAdapterManager>();

//通道个数
#define CHANNLE_SIZE 2
//数据缓冲器
float64 values[CHANNLE_SIZE] = {0};
#if 0
//数据委托 数据回调函数，在此函数中读取数据
int32 analogDataDelegate(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    std::cout << QDateTime::currentDateTime().toString("hh:mm:ss.zzz").toStdString() << "-> ";

    //获取板卡操作句柄
    PfAdapter::AnalogInAdapter *obj = (PfAdapter::AnalogInAdapter*)callbackData;

    //读取数据
    int rSize = 0;
    if(obj->readValue(values, rSize))
    {
        for(int index = 0; index < rSize; index++)
        {
            std::cout << std::to_string(values[index]);
            std::cout << " ";
        }

        std::cout << std::endl;
    }
}
#endif

void test()
{   
    //step2：初始化配置文件
    obj->init("./devcfg.xml");

    //step3：获取适配器句柄
    PfAdapter::Adapter *ada = nullptr;

    std::string id = "cmdSend";

    if(obj->getAdapter(id, &ada))
    {
        //int rate = 5;
        //ada->setAttribute("Rate", &rate);
        /*{
            "port":0,
            "channel":1,
            "state":true
        }

        ada->sendMsg()*/

        //ada->getAttribute();
#if 1
        //step4：总线收发数据
        std::string msg = "hello word ffffffffffffffffff";
        if(ada->sendMsg(msg.c_str(), msg.size()))
        {
            std::cout << "send ok" << std::endl;
        }
        else
        {
            std::cout << "send error" << std::endl;
        }


        char str[1024] = {0};
        //std::string msgg = "6666666";
        //char *msg = msgg.c_str();
        int rcvSize=0;
        int maxRcvSize = 1024;
        unsigned int timeOut = 500;
        //ada->receiveMsg(str, rcvSize, maxRcvSize, timeOut);


        while(1)
        {
            //sleep(500);
            if(ada->receiveMsg(str, rcvSize, maxRcvSize, timeOut))
            {
                std::cout << "receive ok " << str << rcvSize << std::endl;
            }
            else
            {
                std::cout << "receive error" << std::endl;
            }
        }
#else
        //step4：非总线操作
        //if(PfAdapter::BroadcastAdapter *tmp = dynamic_cast<PfAdapter::BroadcastAdapter*>(ada))
        if(PfAdapter::AnalogInAdapter *tmp = dynamic_cast<PfAdapter::AnalogInAdapter*>(ada))
        {
            /*getAttribute测试
              int rate = 0;
              ada->getAttribute("SampleRate", &rate);
            */
            /*BroadcastAdapte收发测试
              std::string msg = "hello word";
              tmp->sendMsg(msg.c_str(), msg.size());
            */
            /*RelayCtrlAdapter继电器开关测试
              step4:控制
              step4-1:打开第1路继电器
              tmp->openRelayChannel(2);
              step4-2:关闭第1路继电器
              tmp->closeRelay(0);
              std::cout << "open ok" << std::endl;
            */


            //step2-2：设置采样点及采样率(200ms=1000/5触发一次回调函数)
            //tmp->setSamplingRateAndSize(5, 1);

            //step3：注册回调函数
            ////tmp->registerInEvent(analogDataDelegate, (void*)tmp);

            //step4：启动采集任务
            ////tmp->startTask();

            //note 如果不加while(1) 则obj将自动释放
            while(1);   //等待接收数据
        }
        else
        {
            std::cout << "dynamic_cast faild" << std::endl;
        }

#endif
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
