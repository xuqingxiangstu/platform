#include "ZmqRcvAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      ZmqRcvAdapter::ZmqRcvAdapter():
          mRcvObj(nullptr)
      {
      }

      ZmqRcvAdapter::~ZmqRcvAdapter()
      {

      }

      void ZmqRcvAdapter::init(const std::string &remoteIp, const std::string &remotePort)
      {
          std::lock_guard<std::mutex> lk(mMutex);
          if(!mRcvObj)
          {
              mRcvObj = std::make_shared<PfBus::zmqReceive>();
              mRcvObj->init(remoteIp, remotePort);
          }
      }
      void ZmqRcvAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          std::string strRemoteIp = "";
          std::string strRemotePort = "";

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

          std::lock_guard<std::mutex> lk(mMutex);

          mRcvObj = std::make_shared<PfBus::zmqReceive>();
          mRcvObj->init(strRemoteIp, strRemotePort);
      }

      bool ZmqRcvAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool ZmqRcvAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::lock_guard<std::mutex> lk(mMutex);

          return mRcvObj->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
      }

      std::string ZmqRcvAdapter::getClassName()
      {
          return VAR_NAME(ZmqRcvAdapter);
      }

      std::string ZmqRcvAdapter::getId()
      {
          return "";
      }
      int ZmqRcvAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int ZmqRcvAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool ZmqRcvAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool ZmqRcvAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new ZmqRcvAdapter();
          }
      }
  }
}

    
    
