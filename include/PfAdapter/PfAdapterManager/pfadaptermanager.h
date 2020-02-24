#ifndef PFADAPTERMANAGER_H
#define PFADAPTERMANAGER_H

/**
 * @file pfadapter.h
 * @brief 适配器抽象类
 * 通过该类可实现总线、NI板卡控制等设备的接口抽象
 * @author xqx
 * @date 2020-01-03
 * @copyright (C)2020 BJTU
 */

#include "pfadaptermanager_global.h"
#include "../adapter/Adapter.h"

#include <unordered_map>
#include <memory>

namespace Pf
{
    namespace PfAdapter
    {
        /**
         * @brief The PfAdapterManager class
         * 适配器抽象管理类，由唯一设备ID索引
         * @note xml文件配置如下
         * @code
         *  <?xml version="1.0"?>
         *  <configure>
         *      <!-- 根据各适配器配置进行填充，具体配置详见各适配器init注解-->
         *      <dev id="" class="adapter">
         *      </dev>
         *  </configure>
         * @endcode
         */
        class PFADAPTERMANAGERSHARED_EXPORT PfAdapterManager
        {
        public:
            PfAdapterManager();
            ~PfAdapterManager();
        public:
            /**
             * @brief init  初始化
             * @param xmlPath 文件路径
             * @exception catch(runtime_error)
             */
            void init(const std::string &xmlPath);

            /**
             * @brief getAdapter    获取适配器
             * @param[in] id 设备ID
             * @param[out] adpter  适配器
             * @return 是否成功
             * - true：成功
             * - false：失败
             */
            bool getAdapter(const std::string &id, Adapter **adpter);

            /**
             * @brief setAdapter    设置适配器
             * @param[in] id 设备ID
             * @param[in] obj 句柄
             */
            void setAdapter(const std::string &id, Adapter *adpter);
        private:
            std::unordered_map<std::string, std::shared_ptr<Adapter>> mManagement; ///<  适配器管理
        };
    }
}
#endif // PFADAPTERMANAGER_H
