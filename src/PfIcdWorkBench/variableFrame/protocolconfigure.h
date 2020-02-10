#ifndef PROTOCOLCONFIGURE_H
#define PROTOCOLCONFIGURE_H

/**
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

        class protocolConfigure
        {
        public:
            /// 变长帧帧类型
            using variableStorageType = protocolStorage<unsigned int, int, int, int, int, int, int, int, int, int, int, int, std::string, int, int, int, int, std::string, int>;
            /// 变长帧帧索引
            enum variableStorageIndex
            {
                variable_head_init_index,
                variable_head_size_index,
                variable_frame_len_start_index,
                variable_frame_len_byte_size_index,
                variable_cal_len_start_index,
                variable_cal_len_to_end_index,
                variable_code_start_index,
                variable_code_size_index,
                variable_code_bit_start_index,
                variable_code_bit_size_index,
                variable_cnt_to_end_index,
                variable_cnt_size_index,
                variable_check_type_index,
                variable_frame_check_to_end_index,
                variable_frame_check_size_index,
                variable_cal_check_start_index,
                variable_cal_check_to_end_index,
                variable_cnt_is_continue_index,
                variable_len_deal_index
            };

        public:
            protocolConfigure();
            /**
             * @param Worksheet *   excel表sheet句柄
             */
            void init(QXlsx::Worksheet *);

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
