#ifndef BROADCASTADAPTER_H
#define BROADCASTADAPTER_H

#include "BroadcastAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfBus/udp/BroadcastUdp.h"

#include <memory>

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The BroadcastAdapter class
       * 广播适配器
       * @note xml配置如下
       * @code
       *  <!--
       *      广播配置
       *      id：全局唯一
       *      class：固定不变
       *  -->
       *  <dev id="send" class="BroadcastAdapter">
       *      <!--绑定本地IP地址，如无需绑定则不配置此项-->
       *      <localIp>192.168.34.2</localIp>
       *      <!-- 本地端口号 -->
       *      <localPort>2205</localPort>
       *      <!-- 目标端口号-->
       *      <remotePort>2205</remotePort>
       *  </dev>
       * @endcode
      */
      class BROADCASTADAPTERSHARED_EXPORT BroadcastAdapter : public Adapter
      {
      public:
          BroadcastAdapter();
          ~BroadcastAdapter();
      public:
          void init(const TiXmlElement *ele) override;
          bool sendMsg(const char *msg, const int &msgSize) override;
          bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          std::string getClassName() override;
          std::string getId() override;
          int getAttribute(const std::string &attr, void *value) override;
          int setAttribute(const std::string &attr, const void *value) override;
          bool writeValue(const double *value, const int size) override;
          bool readValue(double *value, int &rSzie) override;
      private:
          std::shared_ptr<PfBus::BroadcastUdp> udp; ///< 广播实例
          std::mutex mMutex;   ///< 消息锁
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          BROADCASTADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // BROADCASTADAPTER_H
    
    
