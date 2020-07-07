#include "UnicastAdapter.h"

#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
  namespace PfAdapter
  {
      UnicastAdapter::UnicastAdapter():
          udp(nullptr)
      {
      }

      UnicastAdapter::~UnicastAdapter()
      {

      }

      void UnicastAdapter::init(const std::string &json)
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

          udp = std::make_shared<PfBus::UnicastUdp>();

          udp->init(root["local_ip"].asString(), root["local_port"].asString(), root["remote_ip"].asString(), root["remote_port"].asString());
      }

      void UnicastAdapter::init(const Json::Value &json)
      {
          std::string strLocalIp = "";
          std::string strLocalPort = "";
          std::string strRemoteIp = "255.255.255.255";
          std::string strRemotePort = "";

          if(!json["localIp"].isNull() && json["localIp"].isString())
          {
                strLocalIp = json["localIp"].asString();
          }

          if(!json["localPort"].isNull() && json["localPort"].isInt())
          {
                strLocalPort = json["localPort"].asString();
          }

          if(!json["remoteIp"].isNull() && json["remoteIp"].isString())
          {
                strRemoteIp = json["remoteIp"].asString();
          }

          if(!json["remotePort"].isNull() && json["remotePort"].isInt())
          {
                strRemotePort = json["remotePort"].asString();
          }

          udp = std::make_shared<PfBus::UnicastUdp>();

          udp->init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);
      }

      void UnicastAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          std::string strLocalIp = "";
          std::string strLocalPort = "";
          std::string strRemoteIp = "255.255.255.255";
          std::string strRemotePort = "";

          pTmpElem = xmlEle->FirstChildElement("localIp");
          if (pTmpElem)
          {
              strLocalIp = pTmpElem->GetText();
          }

          pTmpElem = xmlEle->FirstChildElement("localPort");
          if (pTmpElem)
          {
              strLocalPort = pTmpElem->GetText();
          }

          pTmpElem = xmlEle->FirstChildElement("remoteIp");
          if (pTmpElem)
          {
              strRemoteIp = pTmpElem->GetText();
          }

          pTmpElem = xmlEle->FirstChildElement("remotePort");
          if (pTmpElem)
          {
              strRemotePort = pTmpElem->GetText();
          }

          udp = std::make_shared<PfBus::UnicastUdp>();

          udp->init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);

      }

      bool UnicastAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return udp->sendMsg((unsigned char*)msg, msgSize);
      }

      bool UnicastAdapter::atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, const int &rMaxRcvSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);

          //发送
          udp->sendMsg((unsigned char*)sMsg, sMsgSize);

          //接收
          return udp->receiveMsg((unsigned char*)rMsg, (unsigned int*)&rcvSize, rMaxRcvSize, interval);
      }

      bool UnicastAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return udp->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
      }

      std::string UnicastAdapter::getClassName()
      {
          return VAR_NAME(UnicastAdapter);
      }

      std::string UnicastAdapter::getId()
      {
          return "";
      }
      int UnicastAdapter::getAttribute(const std::string &attr, void *value)
      {
          int result=0;
          if(udp->getAttribute(attr,value))
          {
              result= 1;
          }
          return result;
      }

      int UnicastAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool UnicastAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool UnicastAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new UnicastAdapter();
          }
      }
  }
}

    
    
