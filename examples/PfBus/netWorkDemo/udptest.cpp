#include "udpTest.h"
#include "BroadcastUdp.h"
#include "MulticastUdp.h"
#include "UnicastUdp.h"
#include <QDebug>
#include <iostream>

using namespace Pf;

udpTest::udpTest()
{

}

void udpTest::BroadcastUdpTest()
{

    PfBus::BroadcastUdp boardUdp;
    //本地IP
    std::string strLocalIp = "192.168.34.103";
    //本地端口
    std::string strLocalPort = "7000";
    //目标IP
    std::string strRemoteIp = "255.255.255.255";
    //本地端口
    std::string strRemotePort = "6000";
    //Step.1 通信接口初始化
    boardUdp.init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);

    //数据
    std::string msg = "0x66";

    //Step.2 数据发送
    boardUdp.sendMsg((unsigned char*)msg.c_str(), msg.size());

    //发送结果
    std::cout << "BroadcastUdpTest SendData: " + msg + " <Send Completed>" <<"\n";


    const int BUF_SIZE = 10240;
    unsigned char buf[BUF_SIZE] = {0};
    unsigned int rcvSize = 0;

    if(boardUdp.receiveMsg(buf, &rcvSize, BUF_SIZE))
    {
        std::string str="";
        int strNum=strlen((char*)buf);
        for(int i=0; i<strNum; i++)
            str.append(1,buf[i]);
        std::cout << "BroadcastUdpTest RcvData: " + str + " <Receive Completed>" <<"\n";;
    }
}


void udpTest::MulticastUdpTest()
{
    PfBus::MulticastUdp multUdp;
    //本地IP
    std::string strLocalIp = "192.168.34.103";
    //本地端口
    std::string strLocalPort = "7000";
    //目标IP
    std::string strRemoteIp = "224.0.0.100";
    //本地端口
    std::string strRemotePort = "6000";
    //Step.1 通信接口初始化
    multUdp.init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);

    //数据
    std::string msg = "0x555";

    //Step.2 数据发送
    multUdp.sendMsg((unsigned char*)msg.c_str(), msg.size());

    //发送结果
    std::cout << "MulticastUdpTest SendData: " + msg + " <Send Completed>" <<"\n";

    const int BUF_SIZE = 10240;
    unsigned char buf[BUF_SIZE] = {0};
    unsigned int rcvSize = 0;

    if(multUdp.receiveMsg(buf, &rcvSize, BUF_SIZE))
    {
        std::string str="";
        int strNum=strlen((char*)buf);
        for(int i=0; i<strNum; i++)
            str.append(1,buf[i]);
        std::cout << "MulticastUdpTest RcvData: " + str + " <Receive Completed>" <<"\n";;
    }
}

void udpTest::UnicastUdpTest()
{
    PfBus::UnicastUdp UniUdp;
    //本地IP
    std::string strLocalIp = "192.168.34.103";
    //本地端口
    std::string strLocalPort = "7000";
    //目标IP
    std::string strRemoteIp = "192.168.34.108";
    //本地端口
    std::string strRemotePort = "6000";
    //Step.1 通信接口初始化
    UniUdp.init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);

    //数据
    std::string msg = "0x666666666";

    //Step.2 数据发送
    UniUdp.sendMsg((unsigned char*)msg.c_str(), msg.size());

    //发送结果
    std::cout << "UnicastUdpTest SendData: " + msg + " <Send Completed>" <<"\n";

    const int BUF_SIZE = 10240;
    unsigned char buf[BUF_SIZE] = {0};
    unsigned int rcvSize = 0;

    if(UniUdp.receiveMsg(buf, &rcvSize, BUF_SIZE))
    {
        std::string str="";
        int strNum=strlen((char*)buf);
        for(int i=0; i<strNum; i++)
            str.append(1,buf[i]);
        std::cout << "UnicastUdpTest RcvData: " + str + " <Receive Completed>" <<"\n";;
    }
}
