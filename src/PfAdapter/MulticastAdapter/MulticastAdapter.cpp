#include "MulticastAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      MulticastAdapter::MulticastAdapter():
          udp(nullptr)
      {
      }

      MulticastAdapter::~MulticastAdapter()
      {

      }

      void MulticastAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          std::string strLocalIp = "";
          std::string strLocalPort = "";
          std::string strRemoteIp = "";
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

          udp = std::make_shared<PfBus::MulticastUdp>();

          udp->init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);

      }

      bool MulticastAdapter::sendMsg(const char *msg, const int &msgSize, const std::string &ipAddr, const int &port)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return udp->sendMsg((unsigned char*)msg, msgSize);
      }

      bool MulticastAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return udp->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
      }

      bool MulticastAdapter::atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, const int &rMaxRcvSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);

          //发送
          udp->sendMsg((unsigned char*)sMsg, sMsgSize);

          //接收
          return udp->receiveMsg((unsigned char*)rMsg, (unsigned int*)&rcvSize, rMaxRcvSize, interval);
      }
      std::string MulticastAdapter::getClassName()
      {
          return VAR_NAME(MulticastAdapter);
      }

      std::string MulticastAdapter::getId()
      {
          return "";
      }
      int MulticastAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int MulticastAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool MulticastAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool MulticastAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new MulticastAdapter();
          }
      }
  }
}

    
    
