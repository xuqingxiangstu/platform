#ifndef ADAPTER_H
#define ADAPTER_H

/**
 * @file adapter.h
 * @brief 适配器接口抽象基类
 * @author xqx
 * @date 2020-01-03
 * @copyright (C)2020 BJTU
 */

#include "../../PfCommon/TinyXml/tinyxml.h"

#include <string>
#include <mutex>

/** 获取变量名称 **/
#define VAR_NAME(var) (#var)

namespace Pf
{    
    /**
     * @brief 适配器包括PfBus、PfNi等
     */
    namespace PfAdapter
    {

        /**
         * @brief The PfAdapter class
         * 适配器基类接口
         */
        class Adapter
        {
        public:
            Adapter(){}
            virtual ~Adapter(){}
        public:
            /**
             * @brief init  初始化接口
             * @param[in] ele xml元素节点
             */
            virtual void init(const TiXmlElement *ele) {}

            /**
             * @brief sendMsg   发送消息
             * @param[in] msg 消息首地址
             * @param[in] msgSize 消息个数
             * @return 是否成功
             * - true：成功
             * - false：失败
             */
            virtual bool sendMsg(const char *msg, const int &msgSize) { return false; }

            /**
             * @brief receiveMsg    接收消息
             * @param[out] msg  接收消息首地址
             * @param[out] rcvSize 实际接收到消息长度
             * @param[in] maxRcvSize 最大接收消息长度
             * @param[in] timeOut 超时等待时间(ms)
             * @return 是否成功
             * - true：成功
             * - false：失败
             */
            virtual bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) { return false; }

            /**
             * @brief getClassName  获取类名称
             * @return 类名称
             */
            virtual std::string getClassName(){return "";}

            /**
             * @brief getId 获取此类对应的设备ID
             * @return 设备ID
             */
            virtual std::string getId(){return "";}

            /**
             * @brief getAttribute  获取属性值，具体实现参看各类
             * @param[in] attr 属性
             * @param[out] value 值
             * @return 是否正确
             */
            virtual int getAttribute(const std::string &attr, void *value){return 0;}

            /**
             * @brief setAttribute  设置属性值
             * @param[in] attr 属性
             * @param[in] value 值
             * @return 是否正确
             */
            virtual int setAttribute(const std::string &attr, const void *value){return 0;}

            /**
             * @brief writeValue    写数据，具体实现参考各类
             * @param[in] value 首地址
             * @param[in] size 长度
             * @return  是否成功
             * - true:成功
             * - false:失败
             */
            virtual bool writeValue(const double *value, const int size){return false;}

            /**
             * @brief readValue 读取数据
             * @param[in] value 首地址
             * @param[out] rSzie 读取长度
             * @return  是否成功
             * - true:成功
             * - false:失败
             */
            virtual bool readValue(double *value, int &rSzie){return false;}

        };

        /** 类导出函数指针 **/
        typedef Adapter *(*LOAD_STEP_LIB)();
    }
}

#endif // PFDEVOBJ_H
