﻿#pragma once

/**
 * @file BroadcastUdp.h
 * @author JJ
 * @brief UDP广播(Qt方式实现)
 * @version V1.0
 * @date 2020-02-19
 * @copyright (C)2020 北京交通大学智能测控技术中心
 */

#ifndef BROADCASTUDP_H
#define BROADCASTUDP_H

#include "udp_global.h"

#include <memory>

namespace Pf
{
    namespace PfBus
    {
        /**
         * @brief The BroadcastUdp class
         * UDP广播
         */
        class UDPSHARED_EXPORT BroadcastUdp
        {
            public:
                BroadcastUdp();
                ~BroadcastUdp();
            public:
                /**
                 * @brief init 初始化广播接口
                 * @param[in] strLocalIp 本地IP地址(若为空或0.0.0.0，则默认从本地所有网卡发送)
                 * @param[in] strLocalPort 本地端口号
                 * @param[in] strRemoteIp 目标IP地址
                 * @param[in] strRemotePort 目标端口号
                 */
                void init(std::string strLocalIp, std::string strLocalPort, std::string strRemoteIp, std::string strRemotePort);
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
                 * @param[in] u32RcvMax 最大接收数据长度(实际接收数据若超过此长度限制，则超出数据丢失)
                 * @param[in] u32TimeOut 超时时间(以ms为单位，当前0xFFFFFFFF表示阻塞接收)
                 * @return 数据是否接收成功
                 *- true 接收成功
                 *- false 接收失败
                 */
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
            private:
                QString getHostIpAddress();
            private:
                std::shared_ptr<QUdpSocket> mSocket;      ///< 通信接口
                QHostAddress sendaddrees; ///< 发送地址
                QString sendPort;         ///< 发送端口
        };
    }
}

#endif // BROADCASTUDP_H
