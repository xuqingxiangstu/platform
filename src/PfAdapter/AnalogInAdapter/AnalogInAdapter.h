#ifndef ANALOGINADAPTER_H
#define ANALOGINADAPTER_H

#include "AnalogInAdapter_global.h"

#include "../adapter/Adapter.h"

#include <memory>

namespace Pf
{
  namespace PfAdapter
  {
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
      private:
          std::mutex mMutex;   ///< 消息锁
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
    
    