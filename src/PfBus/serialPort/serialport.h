/**
 * @file serialport.h
 * @author fjt
 * @brief 串口初始化、参数配置、收发等函数封装
 * @version V1.0
 * @date 2019-2-19
 * @copyright (C)2019 北京交通大学智能测控技术中心
 */
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "serialport_global.h"

#include <Windows.h>
#include <string>


namespace Pf{
    namespace PfBus
    {
        /** 串口异常信息 */
        enum
        {
            COM_NOERROR = 0,     ///< 没有错误
            COM_NOMEMORY = 1,    ///< 内存不够
            COM_INITRDBUF = 2,   ///< 不能初始化读缓存
            COM_INITWRBUF = 3,   ///< 不能初始化写缓存
            COM_OPENPORT = 4,    ///< 不能打开端口
            COM_SETMASK = 5,     ///< 不能设置端口事件MASK
            COM_BUFSIZE = 6,     ///< 不能设置端口缓存
            COM_GETPARAM = 7,    ///< 不能得到端口参数
            COM_SETPARAM = 8,    ///< 不能设置端口参数
            COM_CRRDEVENT = 9,   ///< 不能创建端口异步读事件
            COM_CRWREVENT = 10,  ///< 不能创建端口异步写事件
            COM_CRRDTHREAD = 11, ///< 不能创建端口读线程
            COM_CRWRTHREAD = 12, ///< 不能创建端口写线程
            COM_CLOSERDT = 13,   ///< 不能关闭端口读线程
            COM_CLOSEWRT = 14,   ///< 不能关闭端口写线程
            COM_TIMEOUTS = 15,   ///< 不能设置端口超时
        };
        /** 串口初始化设置 */
        typedef struct
        {
            unsigned long u32BandRate;  ///< 波特率
            unsigned char u8ByteSize;   ///< 数据位 4-8
            unsigned char u8StopBits;   ///< 停止位 0，1，2 = 1bit, 1.5bit, 2bit
            unsigned char u8Parity;     ///< 0-4=None,Odd,Even,Mark,Space
            unsigned long u32FBinary;   ///< 二进制模式
            unsigned long u32FParity;   ///< 校验使能
        }COM_STRUCT;
        /**
         * @brief The BroadcastUdp class
         * 串口初始化、参数配置、收发等函数封装
         */
        class SERIALPORTSHARED_EXPORT SerialPort
        {
            public:

                HANDLE m_hSerialPort;///< 串口句柄
                COM_STRUCT m_comStruct;///< 串口参数设置结构体
            public:
                SerialPort();
                ~SerialPort();
                /**
                 * @brief SndUartData 发送串口数据
                 * @param[in] u8SndBuf 发送数据内容
                 * @param[in] dw16DataLength 发送数据长度
                 * @return 数据发送是否成功
                 *- true 发送成功
                 *- false 发送失败
                 */
                bool SndUartData(unsigned char *u8SndBuf, DWORD dw16DataLength);
                /**
                 * @brief RcvUartData 接收串口数据
                 * @param[in] u8RcvBuf 接收串口数据内容
                 * @param[in] dw16DataLength 最大接收数据长度(实际接收数据若超过此长度限制，则超出数据丢失)
                 * @param[out] u16Bytes 接收数据长度
                 * @param[in] u32TimeOut 超时时间
                 * @return
                 */
                bool RcvUartData(unsigned char *u8RcvBuf, DWORD dw16DataLength, DWORD * u16Bytes, unsigned int u32TimeOut);
                /**
                 * @brief Init 串口初始化
                 * @param[in] name 串口名称
                 * @param[in] u32BandRate 波特率
                 * @param[in] u8Bytesize 数据位  4-8
                 * @param[in] u8StopBit 停止位  0，1，2 = 1bit, 1.5bit, 2bit
                 * @param[in] u8Parity 奇偶校验位 0-4=None,Odd,Even,Mark,Space
                 * @param[in] u32FBinary 二进制使能位 默认为1
                 * @param[in] u32FParity 奇偶校验使能位 默认为1
                 */
                void Init(std::string name,unsigned long u32BandRate,unsigned char u8Bytesize,unsigned char u8StopBit,
                          unsigned char u8Parity, unsigned long u32FBinary=1, unsigned long u32FParity=1);
            private:
                std::string m_strPortName;///< 串口名称
                OVERLAPPED m_ioRead;     ///< read overlapped structure
                OVERLAPPED m_ioWrite;    ///< write overlapped structure
                /**
                 * @brief Open 打开串口函数
                 * @param tcComName 第几路comx号，x表1，2，3，4，5，6.....
                 */
                void Open(TCHAR * tcComName);
                /**
                 * @brief SetDCB 设置串口参数包括波特率，停止位、数据位、奇偶校验位等
                 */
                void SetDCB();
        };
    }
}
#endif // SERIALPORT_H
