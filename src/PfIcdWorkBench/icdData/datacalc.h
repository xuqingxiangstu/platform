#ifndef DATACALC_H
#define DATACALC_H

#include <memory>
#include <unordered_map>
#include <sstream>

#include "icddata_global.h"

/**
 * @brief   数据计算类，包括浮点数、整数等计算，可自定义计算类
 * @author  xqx
 * @date    20190410
 */

namespace Pf
{
    namespace PfIcdWorkBench
    {
         /**
         * @brief The calc class    数据计算抽象类
         */
        class ICDDATASHARED_EXPORT calc
        {
        public:
            /**
             * @brief data      原始数据通过公式进行计算(a * x * lsb + b)
             * @param srcData   原始数据
             * @param a
             * @param b
             * @param lsb
             * @param precision 精度
             * @return          计算后值
             */
            virtual std::string data(__int64 srcData, double a, double b, double lsb, int precision = 0){return "";}
        };

        /**
         * @brief The floatCalc class   浮点数计算
         */
        class ICDDATASHARED_EXPORT floatCalc : public calc
        {
        public:
            /**
             * @brief data      原始数据通过公式进行计算(a * x * lsb + b)
             * @param srcData   原始数据
             * @param a
             * @param b
             * @param lsb
             * @return          计算后值
             */
            std::string data(__int64 srcData, double a, double b, double lsb, int precision = 0)
            {
                std::ostringstream result;

                precision == 0 ? result.precision() : result.precision(precision);
                result << (double)(srcData * a * lsb + b);

                return result.str();
            }
        };

        /**
         * @brief The intCalc class   十进制计算
         */
        class ICDDATASHARED_EXPORT intCalc : public calc
        {
        public:
            /**
             * @brief data      原始数据通过公式进行计算(a * x * lsb + b)
             * @param srcData   原始数据
             * @param a
             * @param b
             * @param lsb
             * @return          计算后值
             */
            std::string data(__int64 srcData, double a, double b, double lsb, int precision = 0)
            {
                std::ostringstream result;

                result << std::dec << (int)(srcData * a * lsb + b);

                return result.str();
            }
        };


        /**
         * @brief The dataCalc class    数据计算管理类
         */
        class ICDDATASHARED_EXPORT dataCalc
        {
        public:
            dataCalc();
            dataCalc(const dataCalc &) = delete;
            dataCalc &operator = (const dataCalc &) = delete;

            std::string getData(std::string type, __int64 srcData, double e, double b, double lsb, int precision = 0);
        };
    }
}
#endif // DATACALC_H
