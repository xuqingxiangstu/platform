#ifndef ICDFRAMEADAPTER_H
#define ICDFRAMEADAPTER_H

/**
 * @file icdFrameAdapter.h
 * @brief 帧解析与仿真适配器，加载各种帧
 * @author xqx
 * @date 20200210
 * @version 1.0
 */


#include "icdframeadapter_global.h"

#include "../common/frameObj.h"

#include <unordered_map>
#include <memory>

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /**
         * @brief The icdFrameAdapter class
         * ICD帧解析器
         */
        class ICDFRAMEADAPTERSHARED_EXPORT icdFrameAdapter
        {
        public:
            icdFrameAdapter();
            ~icdFrameAdapter();

        public:
            /**
             * @brief init  初始化
             * @param[in] xmlPath 文件路径
             * @exception catch(runtime_error)
             */
            void init(const std::string &xmlPath);

            /**
             * @brief getFrameObj   获取帧句柄
             * @param key   关键字（xml-> key）
             * @return 帧句柄
             * - nullptr:获取失败
             * - !nullptr:帧句柄
             */
            std::shared_ptr<frameObj> getFrameObj(const std::string &key);

        private:
            std::unordered_map<std::string, std::shared_ptr<frameObj>> mFrameManagement; ///< 帧管理适配器
        };
    }
}
#endif // ICDFRAMEADAPTER_H
