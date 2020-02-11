#include "dataconvert.h"

#include "datatype.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataConvert::dataConvert()
        {

        }

        __int64 dataConvert::convert(std::string type, double inValue, double a, double b, double lsb)
        {
            __int64 resultValue = 0;


            /// 根据找到计算类进行计算，后续可添加

            if(type == intType)
            {
                intConvert cal;
                resultValue = cal.data(inValue, a, b, lsb);
            }
            else if(type == floatType)
            {
                floatConvert cal;
                resultValue = cal.data(inValue, a, b, lsb);
            }
            else if(type == boolType)
            {
                intConvert cal;
                resultValue = cal.data(inValue, a, b, lsb);
            }
            else
            {
                intConvert cal;
                resultValue = cal.data(inValue, a, b, lsb);
            }

            return resultValue;
        }

        __int64 floatConvert::data(double inValue, double a, double b, double lsb)
        {
            __int64 result = 0;

            return result;
        }

        __int64 intConvert::data(double inValue, double a, double b, double lsb)
        {
            double result = (inValue - b) / (a * lsb);

            return result;
        }
    }
}
