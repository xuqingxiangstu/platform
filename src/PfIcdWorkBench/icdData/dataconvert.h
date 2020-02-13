#ifndef DATACONVERT_H
#define DATACONVERT_H

#include "icddata_global.h"

#include <memory>
#include <unordered_map>

/**
 * @file    dataConvert.h
 * @brief   数据转换类，依据原始数据及类型进行转换
 * @author  xqx
 * @date    20190410
 */

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The convert class 数据转换抽象类
         */
        class ICDDATASHARED_EXPORT convert
        {
        public:

            /**
             * @brief data      通过公式（y = a * x * lsb + b）计算数据
             * @param[in] inValue   x
             * @param[in] a
             * @param[in] b
             * @param[in] lsb
             * @return 计算结果
             */
            virtual __int64 data(const double &inValue, const double &a, const double &b, const double &lsb){return 0;}
        };

        /**
         * @brief The floatConvert class    浮点型转换
         */
        class ICDDATASHARED_EXPORT floatConvert : public convert
        {
        public:
            __int64 data(const double &inValue, const double &a, const double &b, const double &lsb) override
            {
                __int64 result = 0;

                return result;
            }
        };

        /**
         * @brief The intConvert class  整形转换
         */
        class ICDDATASHARED_EXPORT intConvert : public convert
        {
        public:
            __int64 data(const double &inValue, const double &a, const double &b, const double &lsb) override
            {
                double result = (inValue - b) / (a * lsb);

                return result;
            }
        };

        /**
         * @brief The dataConvert class 数据转换管理类，通过输入数据类型及公式计算结果
         */
        class ICDDATASHARED_EXPORT dataConvert
        {
        public:
            dataConvert();
            dataConvert(const dataConvert &) = delete;
            dataConvert &operator = (const dataConvert &) = delete;

            /**
             * @brief convert   数据转换接口
             * @param[in] type      数据类型（参考 dataType.h 定义)
             * @param[in] inValue
             * @param[in] a
             * @param[in] b
             * @param[in] lsb
             * @return
             */
            __int64 getData(const std::string &type, const double &inValue, const double &a, const double &b, const double &lsb);
        private:
#ifdef USE_NEW
            std::unordered_map<std::string, std::shared_ptr<convert>> mDataManager;    ///< 数据管理句柄
#endif
        };
    }
}


#endif // DATACONVERT_H
