#include <QCoreApplication>
#include "BroadcastUdp.h"
#include <QDebug>
#include <iostream>
#include "udpTest.h"

/**
 * @brief 网络收发例程
 * @example PfBus/netWorkDemo/main.cpp
 */

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    udpTest udp;
    //广播测试
    //udp.BroadcastUdpTest();
    //组播测试
    udp.MulticastUdpTest();
    //单播测试
    //udp.UnicastUdpTest();



    return a.exec();
}


