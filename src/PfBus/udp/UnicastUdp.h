#pragma once

/**
 * @file UnicastUdp.h
 * @author JJ
 * @brief UDP单播(Qt方式实现)
 * @version V1.0
 * @date 2020-02-19
 * @copyright (C)2020 北京交通大学智能测控技术中心
 */

#ifndef UNICASTUDP_H
#define UNICASTUDP_H

#include "udp_global.h"
#include <mutex>
#include <memory>

namespace Pf
{
    namespace PfBus
    {
        /**
         * @brief The UnicastUdp class
         * UDP单播
         */
        class UDPSHARED_EXPORT UnicastUdp
        {
        public:
            UnicastUdp();
            ~UnicastUdp();
        public:
            /**
             * @brief init 初始化单播接口
             * @param[in] localIp 本地IP地址(若为空或0.0.0.0，则默认从本地所有网卡发送)
             * @param[in] localPort 本地端口号
             * @param[in] remotIp 目标IP地址
             * @param[in] remotPort 目标端口号
             */
            void init(std::string localIp, std::string localPort, std::string remotIp, std::string remotPort);

            void init(std::string localIp, std::string localPort);

            bool sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen, std::string remotIp, std::string remotPort);

            void unInit();
            /**
             * @brief sendMsg 发送数据
             * @param[in] u8Msg 发送数据内容
             * @param[in] u32MsgLen 发送数据长度
             * @return 数据是否发送成功
             *- true 发送成功
             *- false 发送失败
             */
            bool sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen);
            /**
             * @brief receiveMsg 接收数据
             * @param[out] u8Msg 接收数据内容
             * @param[out] u32MsgLen 接收数据长度
             * @param[in] u32RcvMax 最大接收数据长度(实际接收数据若超过词长度限制，怎超出数据丢失)
             * @param[in] u32TimeOut 超时时间(以ms为单位，当前0xFFFFFFFF表示阻塞接收)
             * @return 数据接收是否成功
             *- true 接收成功
             *- false 接收失败
             */
            bool receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut = 0xFFFFFFFF);

            bool atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, std::string remotIp, std::string remotPort);
        private:
            bool _receive(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut = 0xFFFFFFFF);
            bool _send(unsigned char *u8Msg, unsigned int u32MsgLen, std::string remotIp, std::string remotPort);
        private:
            /**
             * @brief getHostIpAddress 获取本地IP地址
             * @return 本地IP地址
             */
            QString getHostIpAddress();
        private:
            std::shared_ptr<QUdpSocket> mSocket;      ///< 通信接口
            std::string sendaddrees; ///< 发送地址
            std::string sendPort;         ///< 发送端口
            std::mutex mMutex;
        };
    }
}
#endif// UNICASTUDP_H
