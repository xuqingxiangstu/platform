#ifndef ANALOGOUTADAPTER_H
#define ANALOGOUTADAPTER_H

#include "AnalogOutAdapter_global.h"

#include "../adapter/Adapter.h"

#include "../../PfNi/analogOut/analogout.h"

#include <memory>

namespace Pf
{
  namespace PfAdapter
  {
      /**
       * @brief The AnalogOutAdapter class
       * 模拟量输出适配器
       * @note xml配置如下
       * @code
       * <!--
       * 模拟量输出适配器
       * id：全局唯一，需按实际更改
       * class：固定不变
       * -->
       * <dev id="PXI6704_1" class="AnalogOutAdapter">
       * <!-- 设备名称，Ni-Max中查看 -->
       * <rname>PXI6704_1</rname>
       * <!-- port号，按实际填写 Ni-Max中查看，可多个<port></port> -->
       * <port>16</port>
       * <port>17</port>
       * <!-- 采样率 -->
       * <sampleRate>10</sampleRate>
       * <!-- 采样点 -->
       * <sampleSize>1</sampleSize>
       * <!-- 输出类型 VoltsOut(电压输出) AmpsOut(电流输出)-->
       * <outType>VoltsOut</outType>
       * <!-- 最小值 -->
       * <minV>-0.02</minV>
       * <!-- 最大值 -->
       * <maxV>0.02</maxV>
       * </dev>
       * @endcode
       */

      class ANALOGOUTADAPTERSHARED_EXPORT AnalogOutAdapter : public Adapter
      {
      public:
          AnalogOutAdapter();
          ~AnalogOutAdapter();
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
           *- SampleRate 采样率
           *- SampleSize 采样点
           * @param value
           *- ChannelSize int
           *- SampleRate int
           *- SampleSize int
           * @return 属性获取结果
           *- -1 获取成功
           *- 0 不存在该属性
           */
          int getAttribute(const std::string &attr, void *value) override;
          int setAttribute(const std::string &attr, const void *value) override;
          /**
           * @brief writeValue 读取模拟量值
           * @param[int] value 模拟量输出值：和初始化时通道一一对应
           * @see init()
           * @return 是否成功
           * - true：成功
           * - false：失败
           */
          bool writeValue(const double *value, const int size) override;
          bool readValue(double *value, int &rSzie) override;
      private:
          std::mutex mMutex;   ///< 消息锁
          std::shared_ptr<PfNi::AnalogOut> anaOut; ///< 模拟量输出实例
          int mRate;   ///< 采样率
          int mSize;   ///< 采样点
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          ANALOGOUTADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // ANALOGOUTADAPTER_H
    
    
