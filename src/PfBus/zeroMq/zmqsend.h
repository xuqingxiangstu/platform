#ifndef ZMQSEND_H
#define ZMQSEND_H

/**
 * @file zmqSend.h
 * @brief zeromq数据发送
 * @author xqx
 * @version V1.0
 * @date 2019-12-30
 * @copyright 北京交通大学智能测控技术中心
 */

#include "./zmq/zmq.hpp"
#include "./zmq/zmq.h"
#include "zeromq_global.h"

#include <string>
#include <mutex>
#include <memory>

namespace Pf
{
    /**
     * @brief 总线相关收发
     * 包括 网络（单播、组播、广播）、串口、zeromq等总线     
     */
    namespace PfBus
    {
        /**
         * @brief The zmqSend class
         *  发送数据
         */
        class ZEROMQSHARED_EXPORT zmqSend
        {
        public:
            zmqSend();
            ~zmqSend();
        public:
            /**
             * @brief init  初始化
             * @param[in] remoteIp  目标IP地址
             * @param[in] remotePort    目标端口号
             * @exception 初始化异常
             * @see zmqMqDemo
             *
             */
            void init(const std::string &remoteIp, const std::string remotePort);

            /**
             * @brief sendMsg   发送消息
             * @param[in] u8Msg 消息首地址
             * @param[in] u32MsgLen 消息长度
             * @return 发送状态
             * - true:成功
             * - false:失败
             */
            bool sendMsg(unsigned char *u8Msg, const unsigned int &u32MsgLen);
        private:
            std::mutex  m_Mutex;    ///< 发送锁
            zmq::context_t mCtx;    ///< 环境
            std::shared_ptr<zmq::socket_t> mSocket; ///< 网络句柄
            std::string mIpAddr;    ///< 目标IP地址
            std::string mPort;      ///< 目标端口号
            std::string mConAddr;
        };
    }
}

#endif // ZMQSEND_H
