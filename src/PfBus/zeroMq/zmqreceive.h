#ifndef ZMQRECEIVE_H
#define ZMQRECEIVE_H

/**
 * @file zmqReceive.h
 * @brief zeromq接收
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
    namespace PfBus
    {
        /**
         * @brief The zmqReceive class
         *  数据接收中间件        
         */
        class ZEROMQSHARED_EXPORT zmqReceive
        {
        public:
            virtual ~zmqReceive();
            zmqReceive();
        public:
            /**
             * @brief init  初始化zmq接收
             * @param remoteIp[in]  远程IP地址
             * - *：接收任意IP
             * - ip地址
             * @param[in] remotePort    远程端口
             */
            void init(const std::string &remoteIp, const std::string &remotePort);

            /**
             * @brief receiveMsg    接收远程IP地址数据
             * @param[out] u8Msg         接收缓冲区
             * @param[out] u32MsgLen     接收到的长度
             * @param[in] u32RcvSize    待接收最大长度
             * @param[in] u32TimeOut    超时时间（ms)
             * @return  接收状态
             * - true: 有数据
             * - false: 无数据
             */
            bool receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvSize = 1024, const unsigned int u32TimeOut = 0xFFFFFFFF);
        private:
            /**
             * @brief clearMsg  清空已有消息
             */
            void clearMsg();
        private:
            std::mutex  m_Mutex;    ///< 接收锁
            zmq::context_t mCtx;    ///< zmq环境
            std::shared_ptr<zmq::socket_t> mSocket; ///< 网络句柄
            std::string mIpAddr;    ///< 远程IP地址
            std::string mPort;      ///< 远程端口
        };
    }
}
#endif // ZMQRECEIVE_H
