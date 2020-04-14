#ifndef PROTOCOLCONFIGURE_H
#define PROTOCOLCONFIGURE_H

/**
 * @file protocolconfigure.h
 * @brief The protocolConfigure class
 * 变长帧协议配置类，主要存储帧头、校验方式等信息
 * @author xqx
 * @date 20200210
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
            /** 变长帧帧类型 **/
            using variableStorageType = protocolStorage<unsigned int, int, int, int, int, int, int, int, int, int, int, int, std::string, int, int, int, int, std::string, int>;
            /** 变长帧帧索引 **/
            enum variableStorageIndex
            {
                variable_head_init_index,   ///< 帧头值
                variable_head_size_index,   ///< 帧头长度
                variable_frame_len_start_index, ///< 帧长度参数起始位置
                variable_frame_len_byte_size_index, ///< 帧长度参数字节数
                variable_cal_len_start_index, ///< 帧长度计算起始位置
                variable_cal_len_to_end_index, ///< 帧长度计算不包含的尾部长度
                variable_code_start_index, ///< 帧标识起始位置
                variable_code_size_index, ///< 帧标识字节数
                variable_code_bit_start_index, ///< 帧标识起始bit位
                variable_code_bit_size_index, ///< 帧标识bit位数
                variable_cnt_to_end_index, ///< 帧计数起始距尾部位置
                variable_cnt_size_index, ///< 帧计数字节数
                variable_check_type_index, ///< 校验方式
                variable_frame_check_to_end_index, ///< 帧校验起始距尾部位置
                variable_frame_check_size_index, ///< 帧校验字节数
                variable_cal_check_start_index, ///< 帧校验计算起始位置
                variable_cal_check_to_end_index, ///< 帧校验计算不包含的尾部长度
                variable_cnt_is_continue_index, ///< 不同包的帧计数连续
                variable_len_deal_index ///< 帧长度处理倍数
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
            /// 参数正确性校验
            void checkParamValid(unsigned int, int, int, int, int, int, int, int, int, int, int, int, std::string, int, int, int, int, std::string, int);
        private:
            std::shared_ptr<variableStorageType> mProtocolMsg;  ///<变长帧协议存储
        };
    }
}
#endif // PROTOCOLCONFIGURE_H
