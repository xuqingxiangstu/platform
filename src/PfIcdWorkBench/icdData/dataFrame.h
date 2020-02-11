#ifndef DATAFRAME_H
#define DATAFRAME_H

#include "datastorage.h"
#include "../common/subProtocol.h"
#include "icddata_global.h"

#include <tuple>
#include <vector>

/**
 *  @brief  帧参数解析
 *  @author xqx
 *  @date   20190409
 */

namespace Pf
{
    namespace PfIcdWorkBench
    {
        class ICDDATASHARED_EXPORT dataFrame
        {
        public:
            dataFrame();
            dataFrame(const dataFrame &) = delete;
            dataFrame &operator = (const dataFrame &) = delete;

            /**
             * @brief parse     参数解析
             * @param inInfo    参数信息
             * @param outValue  解析后的值
             * @param inBuf     源码首地址
             * @param inSize    源码长度
             * @param beyond    解析偏移量
             */
            void parse(const std::vector<std::shared_ptr<subProtocol::subStorageType>> inInfo, std::vector<icdOutConvertValueType> &outValue, const unsigned char *inBuf, const unsigned int inSize, const int beyond = 0);

            void simulation(unsigned char *outValue, const int size, const std::vector<std::shared_ptr<subProtocol::subStorageType>> inInfo, const int beyond = 0, const std::vector<icdInValueType> inValue = {});
        };
    }
}

#endif // FRAME_H
