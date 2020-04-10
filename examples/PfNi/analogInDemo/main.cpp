#include <QCoreApplication>

#include <iostream>
#include <memory>

#include <QDateTime>

#include "analogin.h"

/**
 * @brief 模拟量输入例子
 * @example PfNi/analogInDemo/main.cpp
 */

using namespace Pf;

//通道个数
#define CHANNLE_SIZE 10

//数据缓冲器
float64 values[CHANNLE_SIZE] = {0};

//数据委托 数据回调函数，在此函数中读取数据
int32 analogDataDelegate(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    std::cout << QDateTime::currentDateTime().toString("hh:mm:ss.zzz").toStdString() << "-> ";

    //获取板卡操作句柄
    PfNi::AnalogIn *obj = (PfNi::AnalogIn*)callbackData;

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

void test()
{
    //step1：实例化句柄
    std::shared_ptr<PfNi::AnalogIn> obj = std::make_shared<PfNi::AnalogIn>();

    //step2：初始化

    //step2-1：初始化板卡
    std::vector<int> channels;  //按照实际板卡通道数，可通过Ni-Max查询

    for(int index = 0; index < CHANNLE_SIZE; index++)
        channels.push_back(index);

    obj->initCard("PXI6375_1", channels);

    //step2-2：设置采样点及采样率(200ms=1000/5触发一次回调函数)
    obj->setSamplingRateAndSize(5, 1);

    //step3：注册回调函数
    obj->registerInEvent(analogDataDelegate, (void*)obj.get());

    //step4：启动采集任务
    obj->startTask();

    //note 如果不加while(1) 则obj将自动释放
    while(1);   //等待接收数据
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
