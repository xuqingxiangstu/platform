#ifndef RELAYCTRL_H
#define RELAYCTRL_H

/**
 * @file relayCtrl.h
 * @brief 继电器打开、关闭操作
 * @author xqx
 * @date 2019-12-31
 * @version V1.0
 * @copyright (C)2019 BJTU
 */

#include "relayctrl_global.h"

#include "../niVisa/visa.h"

#include <string>

namespace Pf
{
    namespace PfNi
    {

        /**
         * @brief The RelayCtrl class
         * 继电器操作：初始化、打开、关闭
         * 适用与PXI继电器模块：PXI-2569等系列板卡         
         */
        class RELAYCTRLSHARED_EXPORT RelayCtrl
        {
        public:
            RelayCtrl();
            ~RelayCtrl();
        public:
            /**
             * @brief initCard  初始化板卡
             * @param[in] rName 板卡虚拟名称（NiMax中查看）
             * @exception   catch(runtime_error)
             */
            void initCard(const std::string &rName);

            /**
             * @brief openRelay 打开继电器
             * @param channel   通道数(从0开始)
             * @note 调用之前必须初始化板卡
             * @see RelayCtrl::initCard()
             * @return 是否成功
             * - true：成功
             * - false：失败         *
             */
            bool openRelay(const int &channel);

            /**
             * @brief closeRelay 关闭继电器
             * @param channel   通道数(从0开始)
             * @note 调用之前必须初始化板卡
             * @see RelayCtrl::initCard()
             * @return 是否成功
             * - true：成功
             * - false：失败
             */
            bool closeRelay(const int &channel);

            /**
             * @brief setChannelSize    设置板卡通道总数
             * @param size  通道数
             */
            void setChannelSize(const int &size);

            /**
             * @brief getChannelSize    获取通道数
             * @return 通道数
             */
            int getChannelSize();
        private:
            ViSession mSession; ///< 设备句柄
            int mChannelSize;   ///< 通道数
        };
    }
}

#endif // RELAYCTRL_H
