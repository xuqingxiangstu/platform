#include "datacalc.h"

#include "datatype.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataCalc::dataCalc()
        {

#ifdef USE_NEW
            //初始化各计算
            mDataManager[intType] = std::make_shared<intCalc>();
            mDataManager[floatType] = std::make_shared<floatCalc>();
            mDataManager[boolType] = std::make_shared<intCalc>();
            mDataManager[uint8Type] = std::make_shared<intCalc>();
            mDataManager[uint16Type] = std::make_shared<intCalc>();
            mDataManager[uint32Type] = std::make_shared<intCalc>();
            mDataManager[ieee32Type] = std::make_shared<floatCalc>();
            mDataManager[ieee64Type] = std::make_shared<doubleCalc>();
            //mDataManager[ncharType] = std::make_shared<floatCalc>();

#else
            //@note 此处没有采用 new 办法创建计算公式是因为考虑new比较耗时间，放在栈上比较省时间
#endif
        }

        std::string dataCalc::getData(std::string type, const __int64 &srcData, const double &a, const double &b, const double &lsb, const int &precision)
        {

            std::string result = "";

#ifdef USE_NEW
            auto itor = mDataManager.find(type);
            if(itor != mDataManager.end())
            {
                result = (itor->second)->data(srcData, a, b, lsb, precision);
            }
#else

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
#endif
            return result;
        }
    }
}
