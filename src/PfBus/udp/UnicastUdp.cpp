#include "UnicastUdp.h"

#include <sstream>

#include <QNetworkInterface>
#include <QList>

#include <chrono>
#include <thread>

#include <QDebug>

namespace Pf
{
    namespace PfBus
    {

        UnicastUdp::UnicastUdp(void)
        {
            mSocket = std::make_shared<QUdpSocket>();            
        }

        UnicastUdp::~UnicastUdp(void)
        {            
        }

        void UnicastUdp::unInit()
        {
            delete this;
        }

        void UnicastUdp::init(std::string strLocalIp, std::string strLocalPort, std::string strRemoteIp, std::string strRemotePort)
        {
            bool flag = false;
            std::ostringstream strErr;
            //sendaddrees.setAddress(QString::fromStdString(strRemoteIp));
            sendaddrees = strRemoteIp;
            //sendPort = QString::fromStdString(strRemotePort);
            sendPort = strRemotePort;

            QString rcvPort = QString::fromStdString(strLocalPort);
            QString localip;
            if(strLocalIp != "")
            {
                localip = QString::fromStdString(strLocalIp);
            }
            else
            {
                localip = "0.0.0.0";
            }
            flag = mSocket->bind(QHostAddress(localip),rcvPort.toInt());//绑定本地接收端IP及端口(针对单播接收)
            if(!flag)
            {
                strErr.str("");
                strErr << "[ERROR] UnicastUdp set_IP_Failed localip = (" << strLocalIp << ") localport = (" << strLocalPort << ")"		\
                    << " remoteip = (" << strRemoteIp << ") remoteport = (" << strRemotePort << ")" ;
                throw std::runtime_error(strErr.str());
            }

        }

        void UnicastUdp::init(std::string strLocalIp, std::string strLocalPort)
        {
            bool flag = false;
            std::ostringstream strErr;
            QString rcvPort = QString::fromStdString(strLocalPort);
            QString localip;
            if(strLocalIp != "")
            {
                localip = QString::fromStdString(strLocalIp);
            }
            else
            {
                localip = "0.0.0.0";
            }
            flag = mSocket->bind(QHostAddress(localip),rcvPort.toInt());//绑定本地接收端IP及端口(针对单播接收)
            if(!flag)
            {
                strErr.str("");
                strErr << "[ERROR] UnicastUdp set_IP_Failed localip = (" << strLocalIp << ") localport = (" << strLocalPort << ")"		\
                    ;
                throw std::runtime_error(strErr.str());
            }
        }

        bool UnicastUdp::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen, std::string remotIp, std::string remotPort)
        {
            std::unique_lock<std::mutex> lk(mMutex);

            return _send(u8Msg, u32MsgLen, remotIp, remotPort);

        }

        bool UnicastUdp::_send(unsigned char *u8Msg, unsigned int u32MsgLen, std::string remotIp, std::string remotPort)
        {
            std::ostringstream strErr;
            bool Res = false;

            int sendLen = mSocket->writeDatagram((char*)u8Msg,u32MsgLen,QHostAddress(QString::fromStdString(remotIp)),std::atoi(remotPort.c_str()));
            if(sendLen != u32MsgLen)
            {
                Res = false;
            }
            else
            {
                Res = true;
            }
            return Res;

        }

        bool UnicastUdp::atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, std::string remotIp, std::string remotPort)
        {
            std::unique_lock<std::mutex> lk(mMutex);

            _send((unsigned char*)sMsg, sMsgSize, remotIp, remotPort);

            return _receive((unsigned char*)rMsg, (unsigned int*)&rcvSize, 1024, interval);
        }

        bool UnicastUdp::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
        {
            std::unique_lock<std::mutex> lk(mMutex);

            return _send(u8Msg, u32MsgLen, sendaddrees, sendPort);
        }

        bool UnicastUdp::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {            
            std::unique_lock<std::mutex> lk(mMutex);

            return _receive(u8Msg, u32MsgLen, u32RcvMax, u32TimeOut);
        }

        bool UnicastUdp::_receive(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            int RecvLen = 0;

            auto beginTime = std::chrono::high_resolution_clock::now();

            bool bRes = false;
            QByteArray baRecv;
            while(1)
            {
                if(mSocket->hasPendingDatagrams())
                {
                    baRecv.resize(mSocket->pendingDatagramSize());
                    RecvLen += mSocket->readDatagram((char*)u8Msg+RecvLen,baRecv.size());
                    *u32MsgLen = RecvLen;
                    bRes = true;
                    break;
                }

                auto endTime = std::chrono::high_resolution_clock::now();
                auto elapsedTime= std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);

                if(elapsedTime.count() >= u32TimeOut)   //超时退出
                {
                    break;
                }
            }
            return bRes;
        }

        QString UnicastUdp::getHostIpAddress()
        {
            QString strIpAddress;
            QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
            // 获取第一个本主机的IPv4地址
            int nListSize = ipAddressesList.size();
            for (int i = 0; i < nListSize; ++i)
            {
                   if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                       ipAddressesList.at(i).toIPv4Address()) {
                       strIpAddress = ipAddressesList.at(i).toString();
                       break;
                   }
             }
             // 如果没有找到，则以本地IP地址为IP
             if (strIpAddress.isEmpty())
                strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
             return strIpAddress;
        }

    }
}
