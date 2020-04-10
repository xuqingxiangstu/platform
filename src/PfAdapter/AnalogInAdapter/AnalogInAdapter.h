#ifndef ANALOGINADAPTER_H
#define ANALOGINADAPTER_H

#include "AnalogInAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfNi/analogIn/analogin.h"

#include <memory>

/** 版本号 **/
#define VERSION "1.0.0"

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The AnalogInAdapter class
       * 模拟量输入适配器
       * @note xml配置如下
       * @code
       * 	<!--
       * 模拟量输入适配器
       * id：全局唯一，需按实际更改
       * class：固定不变
       * -->
       * <dev id="PXI6375_1" class="AnalogInAdapter">
       * <!-- 设备名称，Ni-Max中查看 -->
       * <rname>PXI6375_1</rname>
       * <!-- port号，按实际填写 Ni-Max中查看，可多个<port></port> -->
       * <port>1</port>
       * <port>2</port>
       * <!-- 采样率 -->
       * <sampleRate>10</sampleRate>
       * <!-- 采样点 -->
       * <sampleSize>1</sampleSize>
       *  </dev>
       * @endcode
       */
      class ANALOGINADAPTERSHARED_EXPORT AnalogInAdapter : public Adapter
      {
      public:
          AnalogInAdapter();
          ~AnalogInAdapter();
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

          std::shared_ptr<PfNi::AnalogIn> anaIn; ///< 模拟量输入实例
          int mRate;   ///< 采样率
          int mSize;   ///< 采样点

      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          ANALOGINADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // ANALOGINADAPTER_H
    
    
