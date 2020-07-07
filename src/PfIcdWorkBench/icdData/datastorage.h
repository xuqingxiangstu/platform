#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "../common/type.h"

#include "icddata_global.h"

/**
 * @brief   数据存储类，根据源数据及待提取信息获取数据
 * @author  xqx
 * @date    20190407
 */

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The dataStorage class
         * 数据存储类，获取及设置数据
         */
        class ICDDATASHARED_EXPORT dataStorage
        {
        public:
            dataStorage();
            dataStorage(const dataStorage &) = delete;
            dataStorage &operator = (const dataStorage &) = delete;
            /**
             * @brief getData       获取某个位置的数据
             * @param[in] u8Data        源码首地址
             * @param[in] u32Size       源码长度
             * @param[in] byteStartPos  起始字节
             * @param[in] byteSize      字节长度
             * @param[in] bitStartPos   起始位
             * @param[in] bitSize       位长度
             * @param[in] dataType      大端/小端
             * @return  数据
             */
            unsigned __int64 getData(const unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const std::string &dataType = SAMLLENDIAN);

            unDataConvert getAutoData(const unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const std::string &dataType = SAMLLENDIAN);

            /**
             * @brief setData       设置某个位置的数据
             * @param[in/out] u8Data        数据首地址
             * @param[in] u32Size       数据长度
             * @param[in] byteStartPos  起始字节
             * @param[in] byteSize      字节长度
             * @param[in] bitStartPos   起始位
             * @param[in] bitSize       位长度
             * @param[in] value         待写入数据
             * @param[in] dataType      大段/小端
             */
            void setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const int &value, const std::string &dataType = SAMLLENDIAN);

            void setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const unsigned int &value, const std::string &dataType = SAMLLENDIAN);

            /**
             * @brief setData       设置某个位置的数据
             * @param[in/out] u8Data        数据首地址
             * @param[in] u32Size       数据长度
             * @param[in] byteStartPos  起始字节
             * @param[in] byteSize      字节长度
             * @param[in] bitStartPos   起始位
             * @param[in] bitSize       位长度
             * @param[in] value         待写入数据
             * @param[in] dataType      大段/小端
             */
            void setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const double &value, const std::string &dataType = SAMLLENDIAN);

            /**
             * @brief setData       设置某个位置的数据
             * @param[in/out] u8Data        数据首地址
             * @param[in] u32Size       数据长度
             * @param[in] byteStartPos  起始字节
             * @param[in] byteSize      字节长度
             * @param[in] bitStartPos   起始位
             * @param[in] bitSize       位长度
             * @param[in] value         待写入数据
             * @param[in] dataType      大段/小端
             */
            void setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const float &value, const std::string &dataType = SAMLLENDIAN);            
        };
    }
}

#endif // DATASTORAGE_H
