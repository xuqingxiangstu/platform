#include "ZmqSendAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      ZmqSendAdapter::ZmqSendAdapter():
          mSendObj(nullptr)
      {

      }

      ZmqSendAdapter::~ZmqSendAdapter()
      {

      }

      void  ZmqSendAdapter::init(const std::string &remoteIp, const std::string &remotePort)
      {
          std::lock_guard<std::mutex> lk(mMutex);
          if(!mSendObj)
          {
              mSendObj = std::make_shared<PfBus::zmqSend>();
              mSendObj->init(remoteIp, remotePort);
          }
      }

      void ZmqSendAdapter::init(const TiXmlElement *xmlEle)
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

          mSendObj = std::make_shared<PfBus::zmqSend>();
          mSendObj->init(strRemoteIp, strRemotePort);
      }

      bool ZmqSendAdapter::sendMsg(const char *msg, const int &msgSize, const std::string &ipAddr, const int &port)
      {          
          std::unique_lock<std::mutex> lk(mMutex);
          return mSendObj->sendMsg((unsigned char*)msg, msgSize);
      }

      bool ZmqSendAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string ZmqSendAdapter::getClassName()
      {
          return VAR_NAME(ZmqSendAdapter);
      }

      std::string ZmqSendAdapter::getId()
      {
          return "";
      }
      int ZmqSendAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int ZmqSendAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool ZmqSendAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool ZmqSendAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new ZmqSendAdapter();
          }
      }
  }
}

    
    
