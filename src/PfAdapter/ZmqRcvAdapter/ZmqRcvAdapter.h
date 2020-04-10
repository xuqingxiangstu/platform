#ifndef ZMQRCVADAPTER_H
#define ZMQRCVADAPTER_H

/**
 * @file ZmqRcvAdapter.h
 * @brief zmq中间件接收适配器
 * @author xqx
 * @date 20200217
 * @version v1.0.0
 */


#include "ZmqRcvAdapter_global.h"

#include "../../PfBus/zeroMq/zmqreceive.h"
#include "../adapter/Adapter.h"

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
       * <dev id="cmdRcv" class="ZmqRcvAdapter">
       *    <!--
       *        *:接收任意IP
       *        IP地址
       *    -->
       *    <remoteIp>*</remoteIp>
       *    <!-- 接收端口号 -->
       *	<remotePort>2202</remotePort>
       * </dev>
       * @endcode
       */
      class ZMQRCVADAPTERSHARED_EXPORT ZmqRcvAdapter : public Adapter
      {
      public:
          ZmqRcvAdapter();
          ~ZmqRcvAdapter();
      public:
          /**
           * @brief init  初始化zmq接收
           * @param remoteIp[in]  远程IP地址
           * - *：接收任意IP
           * - ip地址
           * @param[in] remotePort    远程端口
           */
          void init(const std::string &remoteIp, const std::string &remotePort);
          void init(const TiXmlElement *ele) override;          
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
          std::shared_ptr<PfBus::zmqReceive> mRcvObj; ///< 接收句柄
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          ZMQRCVADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // ZMQRCVADAPTER_H
    
    
