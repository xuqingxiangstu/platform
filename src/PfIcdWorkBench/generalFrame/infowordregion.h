#ifndef INFOWORDREGION_H
#define INFOWORDREGION_H

/**
 * @file infoWordRegion.h
 * @brief 信息字域信息
 * @author xqx
 * @date 20200219
 */

#include <xlsxdocument.h>
#include <xlsxworksheet.h>
#include <xlsxworkbook.h>

#include <unordered_map>

#include "../common/protocolstorage.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The infoWordRegion class
         * 信息存储
         */
        class infoWordRegion
        {
        private:
            /** 子帧存储类型定义 **/
            using storageType = protocolStorage<std::string, std::string, int, int, int, int, std::string, unsigned int, std::string>;
        public:
            /** 存储类型索引 **/
            enum storageIndexType
            {
                sub_param_id_index,             ///< 参数代号
                sub_param_name_index,           ///< 参数名称索引
                sub_param_start_post_index,     ///< 起始字节位置索引
                sub_param_byte_size_index,      ///< 字节个数索引
                sub_param_bit_start_pos_index,  ///< 起始位索引
                sub_param_bit_size_index,       ///< 位个数索引
                sub_param_small_big_index,      ///< 大小端索引
                sub_init_value_index,           ///< 初始化值索引
                /*sub_param_src_min_index,
                sub_param_src_max_index,
                sub_param_mean_min_index,       ///< 工程下限索引
                sub_param_mean_max_index,       ///< 工程上限索引
                */
                sub_param_category_index,       ///< 参数类型索引
                /*sub_param_a_index,              ///< 计算公式y = a * lsb * x + b中的a索引
                sub_param_b_index,              ///< 计算公式y = a * lsb * x + b中的b索引
                sub_param_lsb_index,            ///< 计算公式y = a * lsb * x + b中的lsb索引*/
            };
        public:
            infoWordRegion();
        public:
            /**
             * @brief init  初始化信息字配置
             * @param sheet Excel表格Sheet
             */
            void init(QXlsx::Worksheet *sheet);

            std::shared_ptr<infoWordRegion> clone();
        public:
            std::vector<std::shared_ptr<storageType>> mStorages; ///< 存储对象
        };

        /**
         * @brief The infoWordRegionManager class
         * 信息域管理
         */
        class infoWordRegionManager
        {
        public:
            void init(QXlsx::Workbook *);
            std::shared_ptr<infoWordRegion> getRegion(const unsigned int &code);
            std::shared_ptr<infoWordRegionManager> clone();
        private:
            using keyType = unsigned int;
            using valueType = std::shared_ptr<infoWordRegion>;

            std::unordered_map<keyType, valueType> mManager;
        };
    }
}

#endif // INFOWORDREGION_H
