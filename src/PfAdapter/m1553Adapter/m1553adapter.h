#ifndef M1553ADAPTER_H
#define M1553ADAPTER_H

#include "m1553adapter_global.h"

#include "../adapter/Adapter.h"
#include "../../PfBus/M1553B/m1553b.h"
#include <memory>

#include <QByteArray>

/** 版本号 **/
#define VERSION "1.0.0"

namespace Pf
{
  namespace PfAdapter
  {
      class M1553ADAPTERSHARED_EXPORT m1553Adapter : public Adapter
      {         
      public:
          m1553Adapter();
          ~m1553Adapter();
      public:
          /*
           *{
           *    "type":"local",
           *    "ip_addr":"192.168.34.2",
           *    "port":4001
           *}
           */
          void init(const std::string &json) override;
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
          void setSendRtAndSa(const std::string &rt, const std::string &sa);

          void getRcvRtAndSa(unsigned short &rt, unsigned short &sa){rt = mRcvRtAddr; sa = mRcvSaAddr;}
      private:
          void toMsg(QByteArray);
      private:
          std::mutex mMutex;   ///< 消息锁
          std::shared_ptr<Pf::PfBus::M1553B> mBusObj;
          int mRtAddr;
          int mSaAddr;
          unsigned short mRcvRtAddr;
          unsigned short mRcvSaAddr;
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          M1553ADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // M1553ADAPTER_H
