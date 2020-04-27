#include "tcpserver.h"

#include <sstream>

#include <QHostAddress>

namespace Pf
{
    namespace PfBus
    {

        tcpServer::tcpServer(QObject *parent)
            :QObject(parent)
        {
            mIsConnect = false;
            //mSocket = new QTcpSocket();
            mServer = new QTcpServer();
        }

        tcpServer::~tcpServer()
        {
            if(mIsConnect)
            {
                mServer->close();
            }
        }

        void tcpServer::init(std::string remotIp, int remotPort)
        {
            mServer->listen(QHostAddress(remotIp.c_str()), remotPort);
            connect(mServer, &QTcpServer::newConnection, this, &tcpServer::onNewConnection);
#if 0
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
#endif
        }

        void tcpServer::onNewConnection()
        {
            mSocket = mServer->nextPendingConnection();
            connect(mSocket, &QTcpSocket::disconnected, this, &tcpServer::onDisConnect);
            mIsConnect = true;

            qDebug() << "onNewConnection";

        }

        void tcpServer::onConnect()
        {
            //connect(mSocket, &QTcpSocket::disconnected, this, &tcpClient::onDisConnect);
            //mIsConnect = true;
        }

        void tcpServer::onDisConnect()
        {
            mIsConnect = false;
            qDebug() << "onDisConnect";
        }

        bool tcpServer::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
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

        void tcpServer::onSendMsg(QByteArray msg)
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

        bool tcpServer::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
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
