#ifndef DIGITALIOADAPTER_H
#define DIGITALIOADAPTER_H

#include "DigitalIoAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfNi/digitalIo/digitalio.h"

#include <memory>

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The DigitalIoAdapter class
       * 数字量输入适配器
       * @note xml配置如下
       * @code
       * <!--
       * 数字量输入适配器
       * id：全局唯一，需按实际更改
       * class：固定不变
       * -->
       * <dev id="PXI6375_1" class="DigitalIoAdapter">
       * <!-- 设备名称，Ni-Max中查看 -->
       * <rname>PXI6375_1</rname>
       * <!-- port号，按实际填写 Ni-Max中查看，可多个<port></port> -->
       * <port>0</port>
       * <port>1</port>
       * </dev>
       * @endcode
       */
      class DIGITALIOADAPTERSHARED_EXPORT DigitalIoAdapter : public Adapter
      {
      public:
          DigitalIoAdapter();
          ~DigitalIoAdapter();
      public:
          void init(const TiXmlElement *ele) override;
          bool sendMsg(const char *msg, const int &msgSize) override;
          bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          std::string getClassName() override;
          std::string getId() override;
          /**
           * @brief getAttribute 获取属性
           * @param attr
           *- PortSize 通道数
           * @param value
           *- ChannelSize int
           * @return 属性获取结果
           *- -1 获取成功
           *- 0 不存在该属性
           */
          int getAttribute(const std::string &attr, void *value) override;
          int setAttribute(const std::string &attr, const void *value) override;
          bool writeValue(const double *value, const int size) override;
          /**
           * @brief readValue 读取输入值
           * @param[out] value    与初始化端口一一对应（一个端口为8个输入信号）
           * @note 调用之前必须初始化板卡
           * @see init()
           * @return 读取状态
           * - true：成功
           * - false：失败
           */
          bool readValue(double *value, int &rSzie) override;
      private:
          std::mutex mMutex;   ///< 消息锁
          std::shared_ptr<PfNi::DigitalIn> dgtIn; ///< 模拟量输入实例
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          DIGITALIOADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // DIGITALIOADAPTER_H
    
    
