#ifndef SUBPROTOCOL_H
#define SUBPROTOCOL_H

/**
 * @file subProtocol.h
 * @brief 子帧协议抽象接口
 * @author xqx
 * @date 2020-02-10
 * @version V1.0
 */

#include <string>
#include <tuple>
#include <memory>

#include "protocolstorage.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The subProtocol class
         * 子帧协议抽象
         */
        class subProtocol
        {

        public:
            /** 子帧帧信息索引 **/
            enum
            {
                subFrameCodeIndex,  ///< 帧识别码索引
                subInsideCodeIndex, ///< 子帧识别码索引
                subFrameLenIndex,   ///< 帧长度索引
                subBeyondIndex,     ///< 数据偏移索引
                subStorageIndex     ///< 子帧信息存储索引
            };
        public:

            /** 子帧存储类型定义 **/
            using subStorageType = protocolStorage<std::string, std::string, int, int, int, int, std::string, unsigned int, double, double, std::string, double, double, double>;

            /** 子帧存储key类型定义 **/
            using subMapKeyType = std::pair<unsigned int, unsigned int>;

            /** 子帧存储value类型定义 **/
            using subMapValueType = std::tuple<unsigned int, unsigned int, int, int, std::vector<std::shared_ptr<subStorageType>>>;

            /** 子帧存储类型索引 **/
            enum subStorageIndexType
            {
                sub_param_id_index,             ///< 参数代号索引
                sub_param_name_index,           ///< 参数名称索引
                sub_param_start_post_index,     ///< 起始字节位置索引
                sub_param_byte_size_index,      ///< 字节个数索引
                sub_param_bit_start_pos_index,  ///< 起始位索引
                sub_param_bit_size_index,       ///< 位个数索引
                sub_param_small_big_index,      ///< 大小端索引
                sub_init_value_index,           ///< 初始化值索引
                /*sub_param_src_min_index,
                sub_param_src_max_index,*/
                sub_param_mean_min_index,       ///< 工程下限索引
                sub_param_mean_max_index,       ///< 工程上限索引

                sub_param_category_index,       ///< 参数类型索引
                sub_param_a_index,              ///< 计算公式y = a * lsb * x + b中的a索引
                sub_param_b_index,              ///< 计算公式y = a * lsb * x + b中的b索引
                sub_param_lsb_index,            ///< 计算公式y = a * lsb * x + b中的lsb索引
            };

        public:
            /**
             * @brief init  初始化子帧信息
             * @param[in] book excel 工作簿
             */
            virtual void init(QXlsx::Workbook *book){}

            /**
             * @brief isExist       根据帧识别码、子帧识别码判断是否存在
             * @param[in] frameCode     帧识别码
             * @param[in] insideCode    子帧识别码
             * @return 是否存在
             * - true：存在
             * - false：不存在
             */
            virtual bool isExist(unsigned int frameCode, unsigned int insideCode = 0){return false;}

            /**
             * @brief getMsgLen     根据帧识别码、子帧识别码获取消息帧长度
             * @param[in] frameCode     帧识别码
             * @param[in] insideCode    子帧识别码
             * @return  帧长度
             */
            virtual int getMsgLen(unsigned int frameCode, unsigned int insideCode = 0){return 0;}

            /**
             * @brief getMsgBeyond  根据帧识别码、子帧识别码获取数据偏移
             * @param[in] frameCode     帧识别码
             * @param[in] insideCode    子帧识别码
             * @return  数据偏移量
             */
            virtual int getMsgBeyond(unsigned int frameCode, unsigned int insideCode = 0){return 0;}

            /**
             * @brief getMaxByteSize    根据帧识别码、子帧识别码获取子帧中存在的最大字节数
             * @param[in] frameCode         帧识别码
             * @param[in] insideCode        子帧识别码
             * @return  最大字节数
             */
            virtual int getMaxByteSize(unsigned int frameCode, unsigned int insideCode = 0){return 0;}


            /**
             * @brief curProtocol   获取当前协议类型
             * @return  协议类型
             */
            virtual std::string curProtocol(){return "";}
        };
    }
}

#endif
