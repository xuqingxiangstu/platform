#include "RelayCtrlAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      RelayCtrlAdapter::RelayCtrlAdapter():
          rlyCtl(nullptr)
      {
      }

      RelayCtrlAdapter::~RelayCtrlAdapter()
      {

      }

      void RelayCtrlAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          //设备名称
          std::string strRName = "";
          std::string strPort = "";
          //通道数
          int channelSize = 0;

          pTmpElem = xmlEle->FirstChildElement("rname");
          if (pTmpElem)
          {
              strRName = pTmpElem->GetText();
          }

          pTmpElem = xmlEle->FirstChildElement("channelSize");
          if (pTmpElem)
          {
              std::string strCs = pTmpElem->GetText();
              channelSize = atoi(strCs.c_str());
          }

          rlyCtl = std::make_shared<PfNi::RelayCtrl>();

          rlyCtl->initCard(strRName, channelSize);

      }

      bool RelayCtrlAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool RelayCtrlAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string RelayCtrlAdapter::getClassName()
      {
          return VAR_NAME(RelayCtrlAdapter);
      }

      std::string RelayCtrlAdapter::getId()
      {
          return "";
      }
      int RelayCtrlAdapter::getAttribute(const std::string &attr, void *value)
      {
          if( "ChannelSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);

              *((int*)value) = rlyCtl->getChannelSize();
          }
          else
          {
                return 0;
          }
          return -1;
      }

      int RelayCtrlAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool RelayCtrlAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool RelayCtrlAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new RelayCtrlAdapter();
          }
      }
  }
}

    
    
