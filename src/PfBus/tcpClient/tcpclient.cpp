#include "tcpclient.h"

#include <sstream>

#include <QHostAddress>

namespace Pf
{
    namespace PfBus
    {

        tcpClient::tcpClient(QObject *parent)
            :QObject(parent)
        {
            mIsConnect = false;
            mSocket = new QTcpSocket();
        }

        tcpClient::~tcpClient()
        {
            if(mIsConnect)
            {
                mSocket->close();
            }
        }

        void tcpClient::init(std::string remotIp, int remotPort)
        {
            std::ostringstream strErr;

            mSocket->connectToHost(QHostAddress(remotIp.c_str()), remotPort, QTcpSocket::ReadWrite);

            //connect(mSocket, &QTcpSocket::connected, this, &tcpClient::onConnect);

            if(mSocket->waitForConnected())
            {
               onConnect();
            }
            else
            {
                strErr.str("");
                strErr << "[ERROR] connect server Failed (" << " remoteip = (" << remotIp << ") remoteport = (" << std::dec << remotPort << ")" ;
                throw std::runtime_error(strErr.str());
            }
        }

        void tcpClient::onConnect()
        {
            connect(mSocket, &QTcpSocket::disconnected, this, &tcpClient::onDisConnect);
            mIsConnect = true;
        }

        void tcpClient::onDisConnect()
        {
            mIsConnect = false;
        }

        bool tcpClient::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            bool res = false;

            if(!mIsConnect)
                return res;

            std::unique_lock<std::mutex> lk(mMutex);
            //if(mSocket->waitForReadyRead(u32TimeOut))
            {
                *u32MsgLen = mSocket->read((char*)u8Msg, u32RcvMax);
                if(*u32MsgLen > 0)
                {
                    res = true;
                }
            }

            return res;
        }

        void tcpClient::onSendMsg(QByteArray msg)
        {
            std::unique_lock<std::mutex> lk(mMutex);
            int sendLen = mSocket->write(msg);
            if(sendLen != msg.size())
            {
                //Res = false;
            }
            else
            {
               // Res = true;
            }
        }

        bool tcpClient::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
        {
            bool res = false;

            if(!mIsConnect)
                return res;

            std::unique_lock<std::mutex> lk(mMutex);
            int sendLen = mSocket->write((const char*)u8Msg, u32MsgLen);
            if(sendLen != u32MsgLen)
            {
                res = false;
            }
            else
            {
                res = true;
            }
            return res;
        }
    }
}
