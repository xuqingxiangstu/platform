#ifndef DIGITALIO_H
#define DIGITALIO_H

/**
 * @file DigitalIo.h
 * @brief 数字IO输入、输出操作
 * 适用于PXI数字IO控制板卡，例如：PXI-6528
 * @author xqx
 * @date 2019-12-31
 * @version V1.0
 * @copyright (C)2019 BJTU
 */

#include "digitalio_global.h"

#include "../niDAQmx/NIDAQmx.h"
#include <memory>
#include <vector>

/**
 * @brief 平台命名空间
 */
namespace Pf
{
    /**
     * @brief NI板卡控制
     * 继电器、模拟量输入、模拟量输出、数字量输入、数字量输出、串口    
     */
    namespace PfNi
    {
        /**
         * @brief The DigitalIn class
         * 数字IO输入
         * 适用于PXI数字IO控制板卡，例如：PXI-6528
         *
         */
        class DIGITALIOSHARED_EXPORT DigitalIn
        {

        public:
            DigitalIn();
            ~DigitalIn();
        public:
            /**
             * @brief initCard  初始化板卡
             * @param[in] rName 板卡虚拟名称（NiMax中查看）
             * @param[in] ports 输入端口号，NiMax中查看
             * @note 此处端口号为实际Max面板查看端口号不是索引
             * @exception   catch(runtime_error)
             */
            void initCard(const std::string &rName, const std::vector<int> &ports);

            /**
             * @brief readValue 读取输入值
             * @param[out] value    与初始化端口一一对应（一个端口为8个输入信号）
             * @note 调用之前必须初始化板卡
             * @see DigitalOut::initCard()
             * @return 读取状态
             * - true：成功
             * - false：失败
             */
            bool readValue(std::vector<unsigned char> &value);

            /**
             * @brief getPortSize   获取port个数
             * @return port数
             */
            int getPortSize(){return mPortSize;}
        private:
            TaskHandle mTaskHandle; ///< DAQ任务
            unsigned int mPortSize;  ///< port总数
            std::shared_ptr<unsigned char> mDiData; ///< 输入信号数据缓冲区
        };

        /**
         * @brief The DigitalOut class
         * 数字IO输出
         * 适用于PXI数字IO控制板卡，例如：PXI-6528
         */
        class DIGITALIOSHARED_EXPORT DigitalOut
        {

        public:
            DigitalOut();
            ~DigitalOut();
        public:
            /**
             * @brief initCard  初始化板卡
             * @param[in] rName 板卡虚拟名称（NiMax中查看）
             * @param[in] ports 输入端口号，NiMax中查看
             * @note 此处端口号为实际Max面板查看端口号不是索引
             * @exception   catch(runtime_error)
             */
            void initCard(const std::string &rName, const std::vector<int> &ports);

            /**
             * @brief setValue  设置输出状态
             * @param[in] portIndex 第几个port(每个port有8路)
             * @note 此处端口号为索引
             * @param[in] channel 某个port的第几通道数(0~7)
             * @param[in] state 状态
             * - true： 高状态
             * - false：低状态
             * @note 调用之前必须初始化板卡
             * @see DigitalOut::initCard()
             * @return 返回状态
             * - -1：成功
             * - 0：port索引错误
             * - 1：channel错误
             */
            int setValue(const int &portIndex, const unsigned char &channel, const bool &state);

            /**
             * @brief getPortSize   获取port个数
             * @return port数
             */
            int getPortSize(){return mPortSize;}
        private:
            TaskHandle mTaskHandle; ///< DAQ任务
            unsigned int mPortSize;  ///< port总数
            std::shared_ptr<unsigned char> mDoData; ///< 输出信号数据缓冲区
        };
    }
}
#endif // DIGITALIO_H
