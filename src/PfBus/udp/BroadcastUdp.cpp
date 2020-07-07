#include "BroadcastUdp.h"

#include <sstream>
#include <QNetworkInterface>
#include <QList>

#include <chrono>

namespace Pf
{
    namespace PfBus
    {
        BroadcastUdp::BroadcastUdp()
        {            
            mSocket = std::make_shared<QUdpSocket>();
        }


        BroadcastUdp::~BroadcastUdp()
        {
        }

        void BroadcastUdp::unInit()
        {
            delete this;
        }

        void BroadcastUdp::init(std::string strLocalIp, std::string strLocalPort, std::string strRemoteIp, std::string strRemotePort)
        {
            bool flag= false;
            std::ostringstream strErr;
            sendaddrees.setAddress(QString::fromStdString(strRemoteIp));
            sendPort = QString::fromStdString(strRemotePort);
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
                strErr << "[ERROR] BroadcastUdp set_IP_Failed localip = (" << strLocalIp << ") localport = (" << strLocalPort << ")"		\
                    << " remoteip = (" << strRemoteIp << ") remoteport = (" << strRemotePort << ")" ;
                throw std::runtime_error(strErr.str());
            }

        }


        bool BroadcastUdp::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
        {
            std::ostringstream strErr;
            bool Res = false;
            int sendLen = mSocket->writeDatagram((char*)u8Msg,u32MsgLen,QHostAddress::Broadcast,sendPort.toInt());
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

        bool BroadcastUdp::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            int RecvLen = 0;

            bool bRes = false;
            QHostAddress host;
            quint16 port;
            QByteArray baRecv;

            if(mSocket->waitForReadyRead(u32TimeOut))
            {
                if(mSocket->hasPendingDatagrams())
                {
                    baRecv.resize(mSocket->pendingDatagramSize());
                    RecvLen += mSocket->readDatagram((char*)u8Msg+RecvLen,baRecv.size(), &host, &port);
                    *u32MsgLen = RecvLen;
                    if(RecvLen > 0)
                    {
                        bRes = true;
                    }

                    bRes = true;
                }
            }

            return bRes;
        }
        bool BroadcastUdp::getAttribute(const std::string &attr, void *value)
        {
            bool result=true;
            if(attr=="remoteIp")
            {
                *((std::string *)value)=sendaddrees.toString().toStdString(); ///< 发送地址

            }
            else if(attr=="remotePort")
            {
                *((std::string *)value)=sendPort.toStdString();//端口号
            }
            else
            {
                result=false;
            }
            return result;
        }
        QString BroadcastUdp::getHostIpAddress()
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
