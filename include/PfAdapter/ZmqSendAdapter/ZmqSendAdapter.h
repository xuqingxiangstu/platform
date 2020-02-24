#ifndef ZMQSENDADAPTER_H
#define ZMQSENDADAPTER_H

/**
 * @file zmqsendadapter.h
 * @brief zmq中间件发送适配器
 * @author xqx
 * @date 20200217
 * @version v1.0.0
 */

#include "ZmqSendAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfBus/zeroMq/zmqsend.h"

#include <memory>

/** 版本号 **/
#define VERSION "1.0.0"

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The ZmqSendAdapter class
       * zeromq中间件发送适配器
       * @note xml配置如下
       * @code
       *  <!--
       *      zeromq中间件发送配置
       *      id：全局唯一
       *      class：固定不变
       *  -->
       * <dev id="cmdSend" class="ZmqSendAdapter">
       *    <remoteIp>127.0.0.1</remoteIp>
       *	<remotePort>2203</remotePort>
       * </dev>
       * @endcode
       */
      class ZMQSENDADAPTERSHARED_EXPORT ZmqSendAdapter : public Adapter
      {
      public:
          ZmqSendAdapter();
          ~ZmqSendAdapter();
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
          std::string version() override {return VERSION;}
      private:
          std::mutex mMutex;   ///< 消息锁
          std::shared_ptr<PfBus::zmqSend> mSendObj; ///< 发送句柄
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          ZMQSENDADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // ZMQSENDADAPTER_H
    
    
