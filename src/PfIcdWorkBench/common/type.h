#ifndef TYPE_H
#define TYPE_H

/**
 * @file type.h
 * @brief ICD类型及宏定义定义
 * @author xqx
 * @date 20200213
 * @version v1.0
 */


#include <tuple>
#include <vector>

#ifdef __int64
#undef __int64
#endif

#define __int64 long long

/** 设置工程编码格式 **/
#define Utf8_Coding "utf-8"

/** 大端 **/
#define BIGENDIAN		"大端"

/** 小端 **/
#define SAMLLENDIAN		"小端"

/** crc 校验 **/
#define CRC_CHECK		"CRC"

/** 累加和校验 **/
#define SUM_CHECK		"累加和"

/** 变量字符串 **/
#define VAR_NAME(var) (#var)

namespace Pf
{   
    namespace PfIcdWorkBench
    {
        /**
         * @brief The pair_hash struct
         * unordered_map 自定义hasher
         */
        struct pair_hash
        {
            template<class T1, class T2>
            std::size_t operator() (const std::pair<T1, T2>& p) const
            {
                auto h1 = std::hash<T1>{}(p.first);
                auto h2 = std::hash<T2>{}(p.second);
                return h1 == h2;
            }
        };

        /** 数据转换结构体 **/
        typedef union
        {
            unsigned char u8Buf[16]; ///< 无符号8位数组
            char i8Buf[16];          ///< 有符号8位数组
            unsigned char u8Value;  ///< 无符号8位
            unsigned short u16Value;///< 无符号16位
            unsigned int u32Value;  ///< 无符号32位
            unsigned long long u64Value; ///< 无符号64位
            char i8Value; ///< 有符号8位
            short i16Value; ///< 有符号16位
            int i32Value; ///< 有符号32位
            long long i64Value; ///< 有符号64位

            float f32Value; ///< 32位浮点
            double d64Value; ///< 64位浮点
        }unDataConvert;

        using icdOutSrcValueType = std::tuple<std::string, __int64>; ///< 解析后输出原始类型

        using icdOutConvertValueType = std::tuple<std::string, std::string>; ///< 解析后输出换算后类型

        using icdInValueType = std::tuple<std::string, __int64>; ///< 仿真输入类型

        using byteArray = std::vector<unsigned char>; ///< byteArray类型

    }
}

#endif // TYPE_H
