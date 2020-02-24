#ifndef DATAFRAME_H
#define DATAFRAME_H

#include "datastorage.h"
#include "../common/subProtocol.h"
#include "icddata_global.h"

#include <tuple>
#include <vector>

/**
 *  @file dataFrame.h
 *  @brief  帧参数解析
 *  @author xqx
 *  @date   20190409
 */

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The dataFrame class
         * 帧参数解析
         */
        class ICDDATASHARED_EXPORT dataFrame
        {
        public:
            dataFrame();
            dataFrame(const dataFrame &) = delete;
            dataFrame &operator = (const dataFrame &) = delete;

            /**
             * @brief parse     参数解析
             * @param[in] inInfo    参数信息
             * @param[out] outValue  解析后的值
             * @param[in] inBuf     源码首地址
             * @param[in] inSize    源码长度
             * @param[in] beyond    解析偏移量
             */
            void parse(const std::vector<std::shared_ptr<subProtocol::subStorageType>> &inInfo, std::vector<icdOutConvertValueType> &outValue, const unsigned char *inBuf, const unsigned int &inSize, const int &beyond = 0);

            /**
             * @brief simulation 参数仿真
             * @param[out] outValue 仿真数据首地址
             * @param[in] frameSize 帧最大长度
             * @param[in] inInfo 帧配置信息
             * @param[in] beyond 数据偏移量
             * @param[in] inValue 数据数据值
             */
            void simulation(unsigned char *outValue, const int &frameSize, const std::vector<std::shared_ptr<subProtocol::subStorageType>> &inInfo, const int &beyond = 0, const std::vector<icdInValueType> &inValue = {});
        };
    }
}

#endif // FRAME_H
