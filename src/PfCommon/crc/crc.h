#ifndef CRC_H
#define CRC_H

#include "crc_global.h"

/**
 * @file crc.h
 * @brief   crc计算类，包括crc16、累加和等计算
 * @author  xqx
 * @date    20190412
 * @version v1.0
 */

namespace Pf
{
    namespace PfCommon
    {
        /**
         * @brief The Crc class
         * 校验和计算
         */
        class CRCSHARED_EXPORT Crc
        {
        public:
            /**
             * @brief calCrc16      crc16计算
             * @param[in] pbDataBuf     待计算数据首地址
             * @param[in] dwNumOfBytes  待计算数据长度
             * @return  crc结果
             */
            static unsigned short calCrc16(const unsigned char *pbDataBuf, unsigned int dwNumOfBytes);

            /**
             * @brief calSum        累加和计算
             * @param[in] DataBuf       待计算数据首地址
             * @param[in] dwNumOfBytes  待计算数据长度
             * @return crc结果
             */
            static unsigned char calSum(const unsigned char *DataBuf, unsigned int dwNumOfBytes);

            /**
             * @brief calMd5        MD5值计算
             * @param DataBuf       待计算首地址
             * @param dwNumOfBytes  计算长度
             * @return              MD5值
             */
            static std::string calMd5(const unsigned char *DataBuf, unsigned int dwNumOfBytes);

            /**
             * @brief xOr           异或和计算
             * @param dataBuf       待计算首地址
             * @param dwNumOfBytes  计算长度
             * @return              计算结果
             */
            static unsigned char xOr(const unsigned char *dataBuf, unsigned int dwNumOfBytes);
        };
    }
}

#endif // CRC_H
