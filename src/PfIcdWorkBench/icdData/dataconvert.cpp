#include "dataconvert.h"

#include "datatype.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataConvert::dataConvert()
        {
#ifdef USE_NEW
            //初始化各转换公式
            mDataManager[intType] = std::make_shared<intConvert>();
            mDataManager[floatType] = std::make_shared<floatConvert>();
            mDataManager[boolType] = std::make_shared<intConvert>();
#else

#endif
        }

        __int64 dataConvert::getData(const std::string &type, const double &inValue, const double &a, const double &b, const double &lsb)
        {
            __int64 resultValue = 0;

#ifdef USE_NEW
            auto itor = mDataManager.find(type);
            if(itor != mDataManager.end())
            {
                resultValue = (itor->second)->data(inValue, a, b, lsb);
            }
#else
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
#endif
            return resultValue;
        }      
    }
}
