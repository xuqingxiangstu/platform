#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "tcpclient_global.h"

#include <mutex>
#include <memory>
#include <QByteArray>
#include <QTcpSocket>

namespace Pf
{
    namespace PfBus
    {
        class TCPCLIENTSHARED_EXPORT tcpClient : public QObject
        {
        Q_OBJECT
        public:
            tcpClient(QObject *parent = 0);
            ~tcpClient();
        public:
            void init(std::string remotIp, int remotPort);

            bool sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen);

            bool receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut = 0xFFFFFFFF);

        public slots:
            void onConnect();
            void onDisConnect();

            void onSendMsg(QByteArray);
        private:
            QTcpSocket *mSocket;
            bool mIsConnect;
            std::mutex mMutex;
        };
    }
}

#endif // TCPCLIENT_H
