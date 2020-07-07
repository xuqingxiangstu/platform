#include "tcpclient.h"

#include <sstream>
#include <QDebug>
#include <QHostAddress>
#include <string.h>

namespace Pf
{
    namespace PfBus
    {

        tcpClient::tcpClient(QObject *parent)
            :QObject(parent),
              mIsTrigger(false)
        {
            mIsConnect = false;
            mSocket = new QTcpSocket();


            qDebug() << "-----tcpClient init ------";
        }

        tcpClient::~tcpClient()
        {
            qDebug() << "-----tcpClient close";
            if(mIsConnect)
            {
                mSocket->close();
            }
        }

        void tcpClient::init(std::string remotIp, int remotPort)
        {
            std::ostringstream strErr;

            m_remotIp=remotIp;
            m_remotPort=remotPort;
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
            connect(mSocket, &QTcpSocket::readyRead, this, &tcpClient::onRcvMsg);
            mIsConnect = true;
        }

        void tcpClient::onDisConnect()
        {
            qDebug() << "tcpClient DisConnect";
            mIsConnect = false;
        }
        bool tcpClient::getAttribute(const std::string &attr, void *value)
        {
            bool result=true;
            if(attr=="remoteIp")
            {
                *((std::string *)value)=m_remotIp; ///< 发送地址

            }
            else if(attr=="remotePort")
            {
                *((std::string *)value)=m_remotPort;//端口号
            }
            else
            {
                result=false;
            }
            return result;
        }

        void tcpClient::onRcvMsg()
        {
            if(mIsTrigger)
                emit rcvMsg(mSocket->readAll());
        }

        bool tcpClient::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            bool res = false;

            if(mIsTrigger)
                return false;

            qint64 readSize = 0;

            if(!mIsConnect)
                return res;

            std::unique_lock<std::mutex> lk(mMutex);
            //if(mSocket->waitForReadyRead(u32TimeOut))
            {
                readSize = mSocket->bytesAvailable();

                if(readSize > 0)
                {
                    if(readSize < u32RcvMax)
                    {
                        *u32MsgLen = mSocket->read((char*)u8Msg, readSize);
                    }
                    else
                    {
                        *u32MsgLen = mSocket->read((char*)u8Msg, u32RcvMax);
                    }

                    if(*u32MsgLen > 0)
                    {
                        res = true;
                    }
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
