#ifndef SUBPROTOCOL_H
#define SUBPROTOCOL_H

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>

#include <string>
#include <tuple>
#include <memory>

#include "protocolstorage.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        class subProtocol
        {
        public:
            /// map key为pair时
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
        public:
            /// 子帧帧信息索引
            enum
            {
                subFrameCodeIndex,  ///< 帧识别码索引
                subInsideCodeIndex, ///< 子帧识别码索引
                subFrameLenIndex,   ///< 帧长度索引
                subBeyondIndex,     ///< 数据偏移索引
                subStorageIndex     ///< 子帧信息存储索引
            };
        public:

            /// 子帧存储类型定义
            using subStorageType = protocolStorage<std::string, std::string, int, int, int, int, std::string, int, int, int, int>;

            /// 子帧存储key类型定义
            using subMapKeyType = std::pair<unsigned int, unsigned int>;

            ///子帧存储value类型定义
            using subMapValueType = std::tuple<unsigned int, unsigned int, int, int, std::vector<std::shared_ptr<subStorageType>>>;

            /// 子帧存储类型索引
            enum subStorageIndexType
            {
                sub_param_id_index,
                sub_param_name_index,
                sub_param_start_post_index,
                sub_param_byte_size_index,
                sub_param_bit_start_pos_index,
                sub_param_bit_size_index,
                sub_param_small_big_index,
                sub_param_src_min_index,
                sub_param_src_max_index,
                sub_param_mean_min_index,
                sub_param_mean_max_index
            };

        public:
            ///初始化
            virtual void init(QXlsx::Workbook *){}

            /**
             * @brief isExist       根据帧识别码、子帧识别码判断是否存在
             * @param frameCode     帧识别码
             * @param insideCode    子帧识别码
             * @return true/false
             */
            virtual bool isExist(unsigned int frameCode, unsigned int insideCode = 0){return false;}

            /**
             * @brief getMsgLen     根据帧识别码、子帧识别码获取消息帧长度
             * @param frameCode     帧识别码
             * @param insideCode    子帧识别码
             * @return  帧长度
             */
            virtual int getMsgLen(unsigned int frameCode, unsigned int insideCode = 0){return 0;}

            /**
             * @brief getMsgBeyond  根据帧识别码、子帧识别码获取数据偏移
             * @param frameCode     帧识别码
             * @param insideCode    子帧识别码
             * @return  数据偏移量
             */
            virtual int getMsgBeyond(unsigned int frameCode, unsigned int insideCode = 0){return 0;}

            /**
             * @brief getMaxByteSize    根据帧识别码、子帧识别码获取子帧中存在的最大字节数
             * @param frameCode         帧识别码
             * @param insideCode        子帧识别码
             * @return  最大字节数
             */
            virtual int getMaxByteSize(unsigned int frameCode, unsigned int insideCode = 0){return 0;}

            /**
             * @brief clone 类克隆（深拷贝）
             * @return 类句柄
             */
            virtual std::shared_ptr<subProtocol> clone(){return nullptr;}

            /**
             * @brief curProtocol   获取当前协议类型
             * @return  协议类型
             */
            virtual std::string curProtocol(){return "";}
        };
    }
}

#endif
