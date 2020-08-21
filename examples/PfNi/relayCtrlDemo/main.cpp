#include <QCoreApplication>

#include "relayctrl.h"
#include <memory>
#include <iostream>

/**
 * @brief 继电器打开关闭例程
 * @example PfNi/relayCtrlDemo/main.cpp
 */

using namespace Pf;

void relayCtrlDemo()
{
    //step1:实例化
    std::shared_ptr<PfNi::RelayCtrl> obj = std::make_shared<PfNi::RelayCtrl>();

    //step2:初始化板卡
    obj->initCard("PXI2569", 90);

    //step3:设置板卡通道数（按照实际设置)
    //obj->setChannelSize(90);

    //step4:控制

    //step4-1:打开第1路继电器
    obj->openRelay(0);

    //step4-2:关闭第1路继电器
    obj->closeRelay(0);

    //note 函数运行结束后句柄会释放关闭所有继电器
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        relayCtrlDemo();
    }
    catch(std::runtime_error err)
    {
        std::cout << err.what() << std::endl;
    }

    return a.exec();
}
