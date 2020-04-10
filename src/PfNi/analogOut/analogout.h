#ifndef ANALOGOUT_H
#define ANALOGOUT_H

/**
 * @file AnalogOut.h
 * @brief 模拟量输出(电压/电流)，适用于PXI模拟量输出板卡
 * 启动任务后为连续输出，直至停止任务
 * @author xqx
 * @version V1.0 
 * @date 2020-01-02
 * @copyright (C)2020 BJTU
 */

#include "analogout_global.h"

#include "../niDAQmx/NIDAQmx.h"

#include <vector>
#include <memory>
namespace Pf
{
    namespace PfNi
    {   
        /**
         * @brief The AnalogOut class
         * 模拟量输出(电流/电压)
         */
        class ANALOGOUTSHARED_EXPORT AnalogOut
        {
        public:
            /** 模拟量输出类型 */
            enum OutType{
                AmpsOut, ///< 电流输出
                VoltsOut   ///< 电压输出
            };
        public:
            AnalogOut();
            ~AnalogOut();
        public:
            /**
             * @brief initCard  初始化模拟量输出板卡
             * @param[in] rName 板卡虚拟名称（max中查看）
             * @param[in] channelNums 通道数(按照实际max中通道)
             */
            void initCard(const std::string &rName, const std::vector<int> &channelNums);

            /**
             * @brief setOutType    设置模拟量输出参数
             * @param[in] type  类型
             * @param[in] minValue 最小输出
             * @param[in] maxValue 最大输出
             * @see AnalogOut::OutType
             */
            void setOutParam(const OutType &type, const double &minValue, const double &maxValue);

            /**
             * @brief writeValue 读取模拟量值
             * @param[int] value 模拟量输出值：和初始化时通道一一对应
             * @see AnalogIn::initCard()
             * @return 是否成功
             * - true：成功
             * - false：失败
             */
            bool writeValue(const std::vector<float64> &value);

            /**
             * @brief writeValue 读取模拟量值
             * @param[int] value 模拟量输出值：和初始化时通道一一对应
             * @see AnalogIn::initCard()
             * @return 是否成功
             * - true：成功
             * - false：失败
             */
            bool writeValue(const float64 *value);

            /**
             * @brief startTask 启动采集任务
             */
            void startTask();

            /**
             * @brief stopTask  停止采集任务
             */
            void stopTask();

            /**
             * @brief setSamplingSize   设置采用率和采样点
             * @param[in] rate 采样率
             * @param[in] size 采样点
             */
            bool setSamplingRateAndSize(const int &rate, const int &size);

            /**
             * @brief getChannelSize    获取通道个数
             * @return 通道数
             */
            int getChannelSize();

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
        private:
            TaskHandle mTaskHandle; ///< 任务句柄
            bool isStarting;    ///< 任务启动标志            
            int mChannelSize; ///< 通道数
            int mSamplingRate; ///< 采样率
            int mSamplingSize; ///< 采样点
            OutType mOutType; ///< 输出类型
            double mMinValue; ///< 最小值
            double mMaxValue; ///< 最大值
        };
    }
}
#endif // ANALOGOUT_H
