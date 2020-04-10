#include "virtualUnicastAdapter.h"

#include "../../PfCommon/tools/ut_error.h"
#include <QDebug>
namespace Pf
{
  namespace PfAdapter
  {
      virtualUnicastAdapter::virtualUnicastAdapter():
          udp(nullptr)
      {
      }

      virtualUnicastAdapter::~virtualUnicastAdapter()
      {            
      }

    void virtualUnicastAdapter::init(const std::string &json)
    {
        std::ostringstream strErr;
        Json::Reader reader;
        Json::Value root;
        if(!reader.parse(json, root))
        {
          strErr.str("");
          strErr << "json格式错误(" << json << ")";

          UT_THROW_EXCEPTION(strErr.str());
        }
        std::string type = root["type"].asString();

        mIpAddr = root["ip_addr"].asString();

        mPort = std::to_string(root["port"].asInt());

        if(type == "local")
        {
            udp = std::make_shared<PfBus::UnicastUdp>();

            udp->init(mIpAddr, mPort);
        }
    }

      bool virtualUnicastAdapter::sendMsg(const char *msg, const int &msgSize, const std::string &ipAddr, const int &port)
      {
          bool res = false;

          if(udp)
              res = udp->sendMsg((unsigned char*)msg, msgSize, ipAddr, std::to_string(port));

          return res;
      }


      bool virtualUnicastAdapter::atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, const int &rMaxRcvSize)
      {
          bool res = false;

          if(udp)
            res = udp->atomicTrMsg(sMsg, sMsgSize, rMsg, rcvSize, interval, mIpAddr, mPort);
          return res;
      }

      bool virtualUnicastAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          bool res = false;

          if(udp)
              res = udp->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);

          return res;
      }

      bool virtualUnicastAdapter::receiveMsg(char *msg, int &rcvSize, std::string &rcvIp, unsigned short &rcvPort, const int &maxRcvSize, const unsigned int &timeOut)
      {
          bool res = false;

          if(udp)
              res = udp->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, rcvIp, rcvPort, timeOut);

          return res;
      }

      std::string virtualUnicastAdapter::getClassName()
      {
          return VAR_NAME(virtualUnicastAdapter);
      }

      std::shared_ptr<PfBus::UnicastUdp> virtualUnicastAdapter::getUdpOpt()
      {
          if(udp != nullptr)
              return udp;

          return nullptr;
      }

      void virtualUnicastAdapter::setUdpOpt(std::shared_ptr<PfBus::UnicastUdp> obj)
      {
        if(udp == nullptr)
            udp = std::shared_ptr<PfBus::UnicastUdp>(obj);
      }

      std::string virtualUnicastAdapter::getId()
      {
          return "";
      }     

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new virtualUnicastAdapter();
          }
      }
  }
}

    
    
