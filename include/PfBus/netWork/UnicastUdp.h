﻿#pragma once

/**
 * @file UnicastUdp.h
 * @author xqx
 * @brief UDP单播
 * @version V1.0
 * @date 2017-07-02
 * @copyright (C)2017 北京交通大学智能测控技术中心
 */

#ifndef UNICASTUDP_H
#define UNICASTUDP_H

#include "network_global.h"

namespace Pf
{
    namespace PfBus
    {
        /**
         * @brief The UnicastUdp class
         * UDP单播
         */
        class NETWORKSHARED_EXPORT UnicastUdp
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
        private:

            SOCKET	mSock;///< 通信接口
            sockaddr_in		msAddr;///< 发送地址
            sockaddr_in		mrAddr;///< 接收地址
        };
    }
}
#endif// UNICASTUDP_H
