#ifndef TYPE_H
#define TYPE_H


#include <tuple>
#include <vector>

/**
 * @brief 自定义宏
 */

#define BIGENDIAN		"大端"
#define SAMLLENDIAN		"小端"

#define CRC_CHECK		"CRC"
#define SUM_CHECK		"累加和"

/** 变量字符串 **/
#define VAR_NAME(var) (#var)

/**
 * @brief 自定义类型
 */

namespace Pf
{
    /**
     * @brief icd仿真解析
     */
    namespace PfIcdWorkBench
    {
        /// 数据转换
        typedef union
        {
            unsigned char u8Buf[8];
            char i8Buf[8];
            unsigned char u8Value;
            unsigned short u16Value;
            unsigned int u32Value;
            unsigned long long u64Value;
            char i8Value;
            short i16Value;
            int i32Value;
            long long i64Value;

            float f32Value;
            double d64Value;
        }unDataConvert;

        /// 解析后输出原始类型
        using icdOutSrcValueType = std::tuple<std::string, __int64>;

        /// 解析后输出换算后类型
        using icdOutConvertValueType = std::tuple<std::string, std::string>;

        /// 仿真输入类型
        using icdInValueType = std::tuple<std::string, __int64>;

        /// byteArray类型
        using byteArray = std::vector<unsigned char>;

    }
}

#endif // TYPE_H
