#include "UnicastAdapter.h"

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
          return 0;
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

    
    
