#ifndef DATACONVERT_H
#define DATACONVERT_H

#include <memory>

#include "icddata_global.h"

/**
 * @brief   数据转换类，依据原始数据及类型进行转换
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
             * @param inValue   x
             * @param a
             * @param b
             * @param lsb
             * @return 计算结果
             */
            virtual __int64 data(double inValue, double a, double b, double lsb){return 0;}
        };

        /**
         * @brief The floatConvert class    浮点型转换
         */
        class ICDDATASHARED_EXPORT floatConvert : public convert
        {
        public:
            __int64 data(double inValue, double a, double b, double lsb);
        };

        /**
         * @brief The intConvert class  整形转换
         */
        class ICDDATASHARED_EXPORT intConvert : public convert
        {
        public:
            __int64 data(double inValue, double a, double b, double lsb);
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
             * @param type      数据类型（参考 dataType.h 定义)
             * @param inValue
             * @param a
             * @param b
             * @param lsb
             * @return
             */
            __int64 convert(std::string type, double inValue, double a, double b, double lsb);
        };
    }
}


#endif // DATACONVERT_H
