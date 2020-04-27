#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcpserver_global.h"

#include <mutex>
#include <memory>
#include <QByteArray>
#include <QTcpSocket>
#include <QTcpServer>

namespace Pf
{
    namespace PfBus
    {
        class TCPSERVERSHARED_EXPORT tcpServer : public QObject
        {
        Q_OBJECT
        public:
            tcpServer(QObject *parent = 0);
            ~tcpServer();
        public:
            void init(std::string remotIp, int remotPort);

            bool sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen);

            bool receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut = 0xFFFFFFFF);

        public slots:
            void onNewConnection();
            void onConnect();
            void onDisConnect();

            void onSendMsg(QByteArray);
        private:
            QTcpServer *mServer;
            QTcpSocket *mSocket;
            bool mIsConnect;
            std::mutex mMutex;
        };
    }
}

#endif // TCPCLIENT_H
