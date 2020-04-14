#ifndef NISERIALPORT_H
#define NISERIALPORT_H

/**
 * @file serialPort.h
 * @brief NiVisa串口控制
 * @author xqx
 * @date 2020-01-02
 * @copyright (C)2020 BJTU
 */

#include "niserialport_global.h"

#include "../niVisa/visa.h"

namespace Pf
{
    namespace PfNi
    {
        /**
         * @brief The NiSerialPort class
         * 串口收发数据(采用NI-Visa)         
         */
        class NISERIALPORTSHARED_EXPORT NiSerialPort
        {
        public:
            NiSerialPort();
            ~NiSerialPort();
        public:
            /**
             * @brief init  初始化串口
             * @param[in] rName 串口名称
             * @param[in] parity 校验位
             * - NONE：无校验
             * - EVEN：偶校验
             * - ODD：奇校验
             * @param[in] baud 波特率
             * @param[in] dataBits 数据位
             * - 5
             * - 6
             * - 7
             * - 8
             * @param[in] stopBits  数据位
             * - 1
             * - 1.5
             * - 2
             * @exception catch(runtime_error)
             */
            void init(const std::string &rName, const std::string &parity, const int &baud, const int &dataBits, const std::string stopBits);
            /**
             * @brief sendMsg   发送消息
             * @param[in] u8Msg 消息首地址
             * @param[in] u32MsgLen 消息长度
             * @return 发送状态
             * - true:成功
             * - false:失败
             */
            bool sendMsg(unsigned char *u8Msg, const unsigned int &u32MsgLen);

            /**
             * @brief receiveMsg    接收远程IP地址数据
             * @param[out] u8Msg         接收缓冲区
             * @param[out] u32MsgLen     接收到的长度
             * @return  接收状态
             * - true: 有数据
             * - false: 无数据
             */
            bool receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen);
        private:
            /**
             * @brief getParityV    根据用户输入获取校验位
             * @param[in] parity 校验位
             * @return 校验位对应VI参数
             */
            ViAttrState getParityV(const std::string & parity);

            /**
             * @brief getStopBitsV    根据用户输入获取停止位
             * @param[in] stopBits 停止位
             * @return 停止位对应VI参数
             */
            ViAttrState getStopBitsV(const std::string &stopBits);
        private:
            ViSession mSession; ///< 资源管理
            ViSession mInstrHandle; ///< 串口句柄
        };
    }
}

#endif // SERIALPORT_H
