#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "tcpclient_global.h"

#include <atomic>
#include <mutex>
#include <memory>
#include <QByteArray>
#include <QMutex>
#include <QTcpSocket>
#include <QQueue>
#include <QWaitCondition>

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

            void setTrigger(bool state){mIsTrigger = state;}

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
            void onConnect();
            void onDisConnect();

            void onSendMsg(QByteArray);

            void onRcvMsg();
        signals:
            void rcvMsg(QByteArray);
        private:
            std::string m_remotIp;
            std::string m_remotPort;
            QTcpSocket *mSocket;
            bool mIsConnect;
            std::mutex mMutex;
            std::atomic_bool mIsTrigger;
        };
    }
}

#endif // TCPCLIENT_H
