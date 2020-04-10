#include "DigitalOutAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      DigitalOutAdapter::DigitalOutAdapter():
          dgtOut(nullptr)
      {
      }

      DigitalOutAdapter::~DigitalOutAdapter()
      {

      }

      void DigitalOutAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          //设备名称
          std::string strRName = "";
          std::string strPort = "";
          //port号
          std::vector<int> channels;

          pTmpElem = xmlEle->FirstChildElement("rname");
          if (pTmpElem)
          {
              strRName = pTmpElem->GetText();
          }

          pTmpElem = xmlEle->FirstChildElement("port");
          while(pTmpElem)
          {
              strPort = pTmpElem->GetText();

              channels.push_back(atoi(strPort.c_str()));
              pTmpElem = pTmpElem->NextSiblingElement("port");
          }

          dgtOut = std::make_shared<PfNi::DigitalOut>();
          dgtOut->initCard(strRName, channels);

      }

      bool DigitalOutAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool DigitalOutAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string DigitalOutAdapter::getClassName()
      {
          return VAR_NAME(DigitalOutAdapter);
      }

      std::string DigitalOutAdapter::getId()
      {
          return "";
      }
      int DigitalOutAdapter::getAttribute(const std::string &attr, void *value)
      {
          if( "PortSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);

              *((int*)value) = dgtOut->getPortSize();
          }
          else
          {
                return 0;
          }
          return -1;
      }

      int DigitalOutAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool DigitalOutAdapter::writeValue(const double *value, const int size)
      {


          return false;
      }

      bool DigitalOutAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new DigitalOutAdapter();
          }
      }
  }
}

    
    
