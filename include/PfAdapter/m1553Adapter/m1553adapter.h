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
          enum MsgFormat{
              BC_2_RT = 0,
              RT_2_RT
          };

          enum BusType{
              Bus_B,
              Bus_A
          };

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

          void set1553BBus(BusType bus){m1553BBusType = bus;}
          void set1553BModel(MsgFormat format);
          void setBc2RtInfo(const unsigned short &rt, const unsigned short &sa);
          void setRt2RtInfo(const unsigned short &sRt, const unsigned short &sSa, const unsigned short &rRt, const unsigned short &rSa, const unsigned short &size);

          void getRcvRtAndSa(unsigned short &rt, unsigned short &sa){rt = mRcvRtAddr; sa = mRcvSaAddr;}
      private:
          void toMsg(QByteArray);
      private:
          std::mutex mMutex;   ///< 消息锁
          std::shared_ptr<Pf::PfBus::M1553B> mBusObj;

          unsigned short mBc2RtRtAddr;
          unsigned short mBc2RtSaAddr;

          MsgFormat m1553BMsgFormat;
          BusType m1553BBusType;
          unsigned short mRt2RtSRtAddr;
          unsigned short mRt2RtSSaAddr;
          unsigned short mRt2RtRRtAddr;
          unsigned short mRt2RtRSaAddr;
          unsigned short mRt2RtDataSize;

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
