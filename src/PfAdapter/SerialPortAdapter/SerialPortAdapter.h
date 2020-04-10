#ifndef SERIALPORTADAPTER_H
#define SERIALPORTADAPTER_H

#include "SerialPortAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfBus/serialPort/serialport.h"

#include <memory>

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The SerialPortAdapter class
       * 串口适配器
       * @note xml配置如下
       * @code
       *  <!--
       *      串口配置
       *      id：全局唯一
       *      class：固定不变
       *  -->
       *  <dev id="send" class="SerialPortAdapter">
       *      <!-- 本地COM口名称 -->
       *      <rname>COM1</rname>
       *      <!-- 波特率 -->
       *      <baud>9600</baud>
       *      <!-- 数据位 4-8 -->
       *      <dataBits>8</dataBits>
       *      <!-- 停止位 0，1，2 = 1bit, 1.5bit, 2bit -->
       *      <stopBits>0</stopBits>
       *      <!-- 奇偶校验位 0-4=None,Odd,Even,Mark,Space -->
       *      <parity>0</parity>
       *  </dev>
       * @endcode
       */
      class SERIALPORTADAPTERSHARED_EXPORT SerialPortAdapter : public Adapter
      {
      public:
          SerialPortAdapter();
          ~SerialPortAdapter();
      public:
          void init(const TiXmlElement *ele) override;
          bool sendMsg(const char *msg, const int &msgSize, const std::string &ipAddr = "", const int &port = 0) override;
          bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          std::string getClassName() override;
          std::string getId() override;
          int getAttribute(const std::string &attr, void *value) override;
          int setAttribute(const std::string &attr, const void *value) override;
          bool writeValue(const double *value, const int size) override;
          bool readValue(double *value, int &rSzie) override;
      private:
          std::shared_ptr<PfBus::SerialPort> sep; ///< 串口实例
          std::mutex mMutex;   ///< 消息锁
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          SERIALPORTADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // SERIALPORTADAPTER_H
    
    
