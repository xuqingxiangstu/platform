#ifndef ANALOGIN_H
#define ANALOGIN_H

/**
 * @file AnalogIn.h
 * @brief 模拟量采集，适用于PXI采集板卡
 * 启动任务后为连续采样，直至停止任务
 * @author xqx
 * @version V1.0
 * @date 2020-01-02
 * @copyright (C)2020 BJTU
 */

#include "analogin_global.h"

#include "../niDAQmx/NIDAQmx.h"

#include <vector>
#include <memory>

namespace Pf
{
    namespace PfNi
    {
        /**
         * @brief The AnalogIn class
         * 模拟量采集
         */
        class ANALOGINSHARED_EXPORT AnalogIn
        {
        public:
            AnalogIn();
            ~AnalogIn();
        public:
            /**
             * @brief initCard  初始化模拟量输入板卡
             * @param[in] rName 板卡虚拟名称（max中查看）
             * @param[in] channelNums 通道数
             */
            void initCard(const std::string &rName, const std::vector<int> &channelNums);

            /**
             * @brief readValue 读取模拟量值
             * @param[out] rValue 模拟量输入值：和初始化时通道一一对应
             * @param[out] rSize 读取个数
             * @see AnalogIn::initCard()
             * @return 是否成功
             * - true：读取成功
             * - false：读取失败
             */
            bool readValue(float64 *rValue, int &rSize);

            /**
             * @brief startTask 启动采集任务
             */
            void startTask();

            /**
             * @brief stopTask  停止采集任务
             */
            void stopTask();

            /**
             * @brief getChannelSize    获取通道个数
             * @return 通道数
             */
            int getChannelSize();

            /**
             * @brief setSamplingSize   设置采用率和采样点(T = 1000/rate)
             * @param[in] rate 采样率
             * @param[in] size 采样点
             */
            bool setSamplingRateAndSize(const int &rate, const int &size);

            /**
             * @brief getSamplingSize   获取采样点
             * @return 采样点
             */
            int getSamplingSize();

            /**
             * @brief getSamplingRate   获取采样率
             * @return 采用率
             */
            int getSamplingRate();

            /**
             * @brief registerInEvent   注册模拟量输入事件
             * 启动任务后根据采样点与采样率参数触发回掉函数，在回掉函数里读取数据
             * @param[in] ptr   回掉函数
             * @param[in] data 回掉函数参数
             * @return 注册结果
             * - 0：成功
             * - 1：任务已启动
             * - 2：任务句柄为null
             * @note 调用startTask()之后才会触发此事件
             * @see AnalogIn::setSamplingRate() AnalogIn::startTask()
             */
            int registerInEvent(DAQmxEveryNSamplesEventCallbackPtr ptr, void *data);
        private:
            TaskHandle mTaskHandle; ///< 任务句柄
            bool isStarting;    ///< 任务启动标志            
            int mChannelSize; ///< 通道数
            int mSamplingRate; ///< 采样率
            int mSamplingSize; ///< 采样点
        };
    }
}

#endif // ANALOGIN_H
