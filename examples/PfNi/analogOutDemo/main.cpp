#include <QCoreApplication>

#include <iostream>
#include <sstream>
#include <memory>
#include <thread>
#include <chrono>

#include "analogout.h"

/**
 * @brief 模拟量输出例子
 * @example PfNi/analogOutDemo/main.cpp
 */

using namespace Pf;

void test()
{
    //step1：实例化句柄
    std::shared_ptr<PfNi::AnalogOut> obj = std::make_shared<PfNi::AnalogOut>();

    //step2：初始化

    //step2-1：初始化板卡
    std::vector<int> channels;  //按照实际板卡通道数，可通过Ni-Max查询
    channels.push_back(16);
    channels.push_back(17);
    channels.push_back(18);
    channels.push_back(19);

    obj->initCard("PXI6704_1", channels);

    //step2-2：设置采样点及采样率(20ms填充数据)
    obj->setSamplingRateAndSize(50, 1);

    PfNi::AnalogOut::OutType outType = PfNi::AnalogOut::AmpsOut;

    double minV = 0, maxV = 0;
#if 0
    /***************电压输出******************/
    outType = PfNi::AnalogOut::VoltsOut;
    minV = -10.0;
    maxV = 10.0;
#else
    /***************电流输出******************/
    outType = PfNi::AnalogOut::AmpsOut;
    minV = 0.00;
    maxV = 0.02;
#endif

    //step3：设置输出类型及最大最小值
    //obj->setOutParam(outType, minV, maxV);


    //20200113
    //obj->initCard("PXI6704_1", channels, 50, 1, "AmpsOut", minV, maxV);

    //step4：启动DAQ输出任务
    obj->startTask();

    //临时缓冲区
    std::shared_ptr<float64> tmpData = std::make_shared<float64>(obj->getChannelSize() * obj->getSamplingSize());
    memset(tmpData.get(), 0, obj->getChannelSize() * obj->getSamplingSize() * sizeof(float64));

    int index = 0;

    //step5：启动数据输出线程
    std::thread exeThread([&](){
        while(1)
        {
            //step5-1：每个通道产生随机数(最大值最小值之间数据)
            for(index = 0; index < obj->getChannelSize(); index++)
            {
                tmpData.get()[index] = (rand() % ((long long)(maxV * 10000.0 - minV * 10000.0 + 1)) + minV) / 10000.0;
            }

            //step5-2：输出
            obj->writeValue(tmpData.get());

            std::this_thread::sleep_for(std::chrono::milliseconds(20)); //20ms产生一次数据
        }
    });

    exeThread.join();
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
