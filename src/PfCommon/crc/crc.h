#ifndef CRC_H
#define CRC_H

#include "crc_global.h"

/**
 * @brief   crc计算类，包括crc16、累加和等计算
 * @author  xqx
 * @date    20190412
 * @version v1.0
 */

class CRCSHARED_EXPORT Crc
{
public:
    /**
     * @brief calCrc16      crc16计算
     * @param pbDataBuf     待计算数据首地址
     * @param dwNumOfBytes  待计算数据长度
     * @return  crc结果
     */
    static unsigned short calCrc16(const unsigned char *pbDataBuf, unsigned int dwNumOfBytes);

    /**
     * @brief calSum        累加和计算
     * @param DataBuf       待计算数据首地址
     * @param dwNumOfBytes  待计算数据长度
     * @return crc结果
     */
    static unsigned char calSum(const unsigned char *DataBuf, unsigned int dwNumOfBytes);
};

#endif // CRC_H
