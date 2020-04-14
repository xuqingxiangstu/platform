#ifndef DATATYPE_H
#define DATATYPE_H

/**
 * @brief   数据转换类型定义
 * @author  xqx
 * @date    20190412
 */

#include <string>

namespace Pf
{
    namespace PfIcdWorkBench
    {
        const std::string floatType = "连续量";    ///< 浮点类型
        const std::string boolType = "离散量";     ///< 离散量类型
        const std::string intType = "整型";        ///< 整数类型
        const std::string uint8Type = "UINT8";
        const std::string uint16Type = "UINT16";
        const std::string uint32Type = "UINT32";
        const std::string ieee32Type = "IEEE32";
        const std::string ieee64Type = "IEEE64";
        const std::string ncharType = "NCHAR";
    }
}

#endif // DATATYPE_H
