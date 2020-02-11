#include "datacalc.h"

#include "datatype.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataCalc::dataCalc()
        {
            ///@note 此处没有采用 new 办法创建计算公式是因为考虑new比较耗时间，放在栈上比较省时间
        }

        std::string dataCalc::getData(std::string type, __int64 srcData, double a, double b, double lsb, int precision)
        {
            std::string result = "";

            /// 根据找到计算类进行计算，后续可添加

            if(type == intType)
            {
                intCalc cal;
                result = cal.data(srcData, a, b, lsb, precision);
            }
            else if(type == floatType)
            {
                floatCalc cal;
                result = cal.data(srcData, a, b, lsb, precision);
            }
            else if(type == boolType)
            {
                intCalc cal;
                result = cal.data(srcData, a, b, lsb, precision);
            }
            else
            {
                intCalc cal;
                result = cal.data(srcData, a, b, lsb, precision);
            }

            return result;
        }
    }
}
