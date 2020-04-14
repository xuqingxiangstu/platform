#ifndef RELAYCTRLADAPTER_H
#define RELAYCTRLADAPTER_H

#include "RelayCtrlAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfNi/relayCtrl/relayctrl.h"

#include <memory>

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The RelayCtrlAdapter class
       * 继电器控制适配器
       * @note xml配置如下
       * @code
       * <!--
       * 继电器控制适配器
       * id：全局唯一，需按实际更改
       * class：固定不变
       * -->
       * <dev id="PXI2569" class="RelayCtrlAdapter">
       * <!-- 设备名称，Ni-Max中查看 -->
       * <rname>PXI2569</rname>
       * <!-- 通道数，NI-Max查看-->
       * <channelSize>100</channelSize>
       * </dev>
       * @endcode
       */
      class RELAYCTRLADAPTERSHARED_EXPORT RelayCtrlAdapter : public Adapter
      {
      public:
          RelayCtrlAdapter();
          ~RelayCtrlAdapter();
      public:
          void init(const TiXmlElement *ele) override;
          bool sendMsg(const char *msg, const int &msgSize) override;
          bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          std::string getClassName() override;
          std::string getId() override;
          /**
           * @brief getAttribute 获取属性
           * @param attr
           *- ChannelSize 通道数
           * @param value
           *- ChannelSize int
           * @return 属性获取结果
           *- -1 获取成功
           *- 0 不存在该属性
           */
          int getAttribute(const std::string &attr, void *value) override;
          int setAttribute(const std::string &attr, const void *value) override;
          bool writeValue(const double *value, const int size) override;
          bool readValue(double *value, int &rSzie) override;
      private:
          std::shared_ptr<PfNi::RelayCtrl> rlyCtl; ///< 继电器控制实例
          std::mutex mMutex;   ///< 消息锁
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          RELAYCTRLADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // RELAYCTRLADAPTER_H
    
    
