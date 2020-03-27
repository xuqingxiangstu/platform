#include "MulticastUdp.h"

#include <sstream>

#include <chrono>

namespace Pf
{
    namespace PfBus
    {
        MulticastUdp::MulticastUdp()
        {
            mSocket = std::make_shared<QUdpSocket>();
        }

        MulticastUdp::~MulticastUdp()
        {
        }

        void MulticastUdp::init(std::string strLocalIp, std::string strLocalPort, std::string strRemoteIp, std::string strRemotePort)
        {
            bool flag_bind,flag_join = false;
            std::ostringstream strErr;
            remoteip = QString::fromStdString(strRemoteIp);
            sendPort = QString::fromStdString(strRemotePort);
            QString rcvPort = QString::fromStdString(strLocalPort);

            if(strLocalIp != "")
                flag_bind = mSocket->bind(QHostAddress(QString::fromStdString(strLocalIp)),rcvPort.toInt(),QUdpSocket::ShareAddress);//绑定所有ip来接收组播组信息
            else
                flag_bind = mSocket->bind(QHostAddress::AnyIPv4,rcvPort.toInt(),QUdpSocket::ShareAddress);//绑定所有ip来接收组播组信息

            flag_join = mSocket->joinMulticastGroup(QHostAddress(remoteip));//加入组播组ip：224.0.0.1
            mSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);//设置缓冲区

            if(!(flag_bind&flag_join))
            {
                strErr.str("");
                strErr << "[ERROR] MulticastUdp set_IP_Failed localip = (" << strLocalIp << ") localport = (" << strLocalPort << ")"		\
                    << " remoteip = (" << strRemoteIp << ") remoteport = (" << strRemotePort << ")" ;
                throw std::runtime_error(strErr.str());
            }
        }
        void MulticastUdp::unInit()
        {
            delete this;
        }

        bool MulticastUdp::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
        {
            std::ostringstream strErr;
            bool Res = false;
            int sendLen = mSocket->writeDatagram((char*)u8Msg, u32MsgLen, QHostAddress(remoteip),sendPort.toInt());
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

        bool MulticastUdp::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
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
    }
}
