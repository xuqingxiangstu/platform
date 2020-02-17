#include <QCoreApplication>

#include <memory>

#include "ModelManager/modelmanager.h"

#include "./randaModel/randamodel.h"
#include "./waveformModel/waveformModel.h"
#include "./temperatureModel/temperatureModel.h"

#include <QDateTime>
#include <iostream>
#include <unistd.h>

#include <Windows.h>
#include "timer.h"
#include <QDebug>

using float64 = double;

Algorithm *ada;
Timer *tm = new Timer();

//step1：实例化
std::shared_ptr<ModelManager> obj = std::make_shared<ModelManager>();

//通道个数
#define CHANNLE_SIZE 2
//数据缓冲器
float64 values[CHANNLE_SIZE] = {0};


void test()
{
    //step2：初始化配置文件
    obj->init("./simulationMdl.xml");

    //step3：获取适配器句柄
    Algorithm *ada = nullptr;

    std::string id = "send";

    if(obj->getAlgorithm(id, &ada))
    {
#if 0
        //从配置文件加载参数及属性

        tm->g_ada = ada;
        tm->g_pTimer->start(1000);
#else
        //修改参数或属性

        /*RandaModel
        Algorithm *tmpA1 = new RandaModel();
        obj->setAlgorithm(id, tmpA1);
        tmpA1->init({8,9});
        */
        /*waveformModel
        Algorithm *tmpA1 = new waveformModel();
        obj->setAlgorithm(id, tmpA1);
        tmpA1->init({5, 10, 10, 0});
        */

        /*temperatureModel*/
        Algorithm *tmpA1 = new temperatureModel();
        obj->setAlgorithm(id, tmpA1);
        tmpA1->init({0, 20, 10, 0.5, 0});

        valueType changeType = 2;
        tmpA1->setAttribute("changeType",(void*)&changeType);
        valueType initValue = 6;
        tmpA1->setAttribute("initValue",(void*)&initValue);
        tm->g_ada = tmpA1;
        tm->g_pTimer->start(1000);
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
