#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcpServer_global.h"

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
            /**
             * @brief getAttribute 获取类属性的信息
             * @param attr 成员名字
             * @param value 属性的具体信息
             * @return 属性获取是否成功
             *- true 接收成功
             *- false 接收失败
             */
            bool getAttribute(const std::string &attr, void *value);
        public slots:
            void onNewConnection();
            void onConnect();
            void onDisConnect();

            void onSendMsg(QByteArray);
        private:
            std::string m_remotIp;
            std::string m_remotPort;
            QTcpServer *mServer;
            QTcpSocket *mSocket;
            bool mIsConnect;
            std::mutex mMutex;
        };
    }
}

#endif // TCPCLIENT_H
