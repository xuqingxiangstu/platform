#ifndef PROTOCOLCONFIGURE_H
#define PROTOCOLCONFIGURE_H

/**
 * @file protocolconfigure.h
 * @brief The protocolConfigure class
 * 网络通信帧协议配置类，主要存储帧头、校验方式等信息
 * @author xqx
 * @date 20200218
 * @version 1.0
 */

#include "../common/protocolstorage.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The protocolConfigure class
         * 协议配置类：包括帧头、帧长、校验和等参数
         */
        class protocolConfigure
        {
        public:
            /** 帧类型 **/
            using generalStorageType = protocolStorage<int, unsigned int, int, int, int, int, int, int, int, int, int, int, int, int, int, std::string, int, int, int, int>;
            /** 帧索引 **/
            enum generalStorageIndex
            {
                general_infohead_size_index, ///< 信息头长度
                general_head_init_index,   ///< 帧头值
                general_head_size_index,   ///< 帧头长度
                general_frame_len_start_index, ///< 帧长度参数起始位置
                general_frame_len_byte_size_index, ///< 帧长度参数字节数
                general_cal_len_start_index, ///< 帧长度计算起始位置
                general_cal_len_to_end_index, ///< 帧长度计算不包含的尾部长度
                general_code_start_index, ///< 帧类型起始位置
                general_code_size_index, ///< 帧类型字节数
                general_info_type_start_index, ///< 信息字类型起始位置
                general_info_type_size_index, ///< 信息字类型字节数
                general_info_cnt_start_index, ///< 信息字个数起始位置
                general_info_cnt_size_index, ///< 信息字个数字节数
                general_cnt_start_index, ///< 帧计数起始位置
                general_cnt_size_index, ///< 帧计数字节数                
                general_check_type_index, ///< 校验方式
                general_frame_check_start_index, ///< 帧校验起始位置
                general_frame_check_size_index, ///< 帧校验字节数
                general_cal_check_start_index, ///< 帧校验计算起始位置
                general_cal_check_to_end_index ///< 帧校验计算不包含的尾部长度
            };

        public:
            protocolConfigure();
            /**
             * @param Worksheet *   excel表sheet句柄
             */
            void init(QXlsx::Worksheet *);

            /**
             * @brief clone 类克隆，深拷贝
             * @return 类句柄
             */
            std::shared_ptr<protocolConfigure> clone();

            /**
             * @brief getMessage    获取相应消息
             * @param index         消息索引
             * @return 相应消息内容
             */
            template<size_t index>
            auto getMessage()
            {
                return mProtocolMsg->getMessage<index>();
            }

        private:
            std::shared_ptr<generalStorageType> mProtocolMsg;  ///<变长帧协议存储
        };
    }
}
#endif // PROTOCOLCONFIGURE_H
