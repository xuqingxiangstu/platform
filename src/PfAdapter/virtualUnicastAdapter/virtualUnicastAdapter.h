#ifndef VIRTUALUNICASTADAPTER_H
#define VIRTUALUNICASTADAPTER_H

#include "virtualUnicastAdapter_global.h"

#include "../adapter/Adapter.h"
#include "../../PfCommon/jsoncpp/json.h"
#include "../../PfBus/udp/UnicastUdp.h"

#include <memory>

/** 版本号 **/
#define VERSION "1.0.0"

namespace Pf
{
  namespace PfAdapter
  {
      class VIRTUALUNICASTADAPTERSHARED_EXPORT virtualUnicastAdapter : public Adapter
      {
      public:
          virtualUnicastAdapter();
          ~virtualUnicastAdapter();
      public:          
          /*
           *{
           *    "type":"local",
           *    "ip_addr":"192.168.34.2",
           *    "port":4001
           *}
           */
          void init(const std::string &json);          
          bool sendMsg(const char *msg, const int &msgSize, const std::string &ipAddr, const int &port) override;
          bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          bool receiveMsg(char *msg, int &rcvSize, std::string &rcvIp, unsigned short &rcvPort, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          bool atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, const int &rMaxRcvSize = 1024) override;
          std::string getClassName() override;
          std::string getId() override;         
          std::string version() override {return VERSION;}

          std::shared_ptr<PfBus::UnicastUdp> getUdpOpt();
          void setUdpOpt(std::shared_ptr<PfBus::UnicastUdp> obj);

      private:
          std::shared_ptr<PfBus::UnicastUdp> udp; ///< 单播实例
          std::string mIpAddr;
          std::string mPort;
          std::mutex mMutex;   ///< 消息锁
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          VIRTUALUNICASTADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // VIRTUALUNICASTADAPTER_H
    
    
