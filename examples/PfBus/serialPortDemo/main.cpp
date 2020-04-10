#include <QCoreApplication>

#include "serialport.h"
#include <QDebug>
#include <iostream>

/**
 * @brief 串口收发例程
 * @example PfBus/serialPortDemo/main.cpp
 */
using namespace Pf;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PfBus::SerialPort serialport;
    //串口初始化
    //串口名
    std::string name = "COM1";
    //波特率,
    unsigned long u32BandRate = 9600;
    //数据位  4-8
    unsigned char u8Bytesize = 8;
    //停止位  0，1，2 = 1bit, 1.5bit, 2bit
    unsigned char u8StopBit = 0;
    //奇偶校验位 0-4=None,Odd,Even,Mark,Space
    unsigned char u8Parity = 0;
    serialport.Init(name, u32BandRate, u8Bytesize, u8StopBit, u8Parity);
    //发送数据
    std::string msg = "0x666666666";

    //Step.2 数据发送
    serialport.SndUartData((unsigned char*)msg.c_str(), msg.size());

    //发送结果
    std::cout << "SerialportTest SendData: " + msg + " <Send Completed>" <<"\n";

    const DWORD BUF_SIZE = 10240;
    unsigned char buf[BUF_SIZE] = {0};
    DWORD rcvSize = 0;
    unsigned int u32TimeOut = 1000000;
    while(1)
    {
        //Sleep(500);
        if(serialport.RcvUartData(buf, BUF_SIZE, &rcvSize, u32TimeOut))
        {
            std::string str="";
            int strNum=strlen((char*)buf);
            for(int i=0; i<strNum; i++)
                str.append(1,buf[i]);
            std::cout << "SerialportTest RcvData: " + str + " <Receive Completed>" <<"\n";;
        }
    }

    return a.exec();
}

