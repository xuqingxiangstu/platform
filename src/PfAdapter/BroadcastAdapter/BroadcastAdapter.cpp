#include "BroadcastAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      BroadcastAdapter::BroadcastAdapter():
          udp(nullptr)
      {
      }

      BroadcastAdapter::~BroadcastAdapter()
      {

      }

      void BroadcastAdapter::init(const TiXmlElement *xmlEle)
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

          pTmpElem = xmlEle->FirstChildElement("remotePort");
          if (pTmpElem)
          {
              strRemotePort = pTmpElem->GetText();
          }

          udp = std::make_shared<PfBus::BroadcastUdp>();

          udp->init(strLocalIp, strLocalPort, strRemoteIp, strRemotePort);

      }

      bool BroadcastAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return udp->sendMsg((unsigned char*)msg, msgSize);
      }

      bool BroadcastAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return udp->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
      }

      bool BroadcastAdapter::atomicTrMsg(const char *sMsg, const int &sMsgSize, char *rMsg, int &rcvSize, const unsigned int &interval, const int &rMaxRcvSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);

          //发送
          udp->sendMsg((unsigned char*)sMsg, sMsgSize);

          //接收
          return udp->receiveMsg((unsigned char*)rMsg, (unsigned int*)&rcvSize, rMaxRcvSize, interval);
      }

      std::string BroadcastAdapter::getClassName()
      {
          return VAR_NAME(BroadcastAdapter);
      }

      std::string BroadcastAdapter::getId()
      {
          return "";
      }
      int BroadcastAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int BroadcastAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool BroadcastAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool BroadcastAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new BroadcastAdapter();
          }
      }
  }
}

    
    
