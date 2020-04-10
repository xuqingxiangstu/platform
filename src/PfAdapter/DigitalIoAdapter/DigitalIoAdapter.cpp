#include "DigitalIoAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      DigitalIoAdapter::DigitalIoAdapter():
          dgtIn(nullptr)
      {
      }

      DigitalIoAdapter::~DigitalIoAdapter()
      {

      }

      void DigitalIoAdapter::init(const TiXmlElement *xmlEle)
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

          dgtIn = std::make_shared<PfNi::DigitalIn>();
          dgtIn->initCard(strRName, channels);

      }

      bool DigitalIoAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool DigitalIoAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string DigitalIoAdapter::getClassName()
      {
          return VAR_NAME(DigitalIoAdapter);
      }

      std::string DigitalIoAdapter::getId()
      {
          return "";
      }
      int DigitalIoAdapter::getAttribute(const std::string &attr, void *value)
      {
          if( "PortSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);

              *((int*)value) = dgtIn->getPortSize();
          }
          else
          {
                return 0;
          }
          return -1;
      }

      int DigitalIoAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool DigitalIoAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool DigitalIoAdapter::readValue(double *rValue, int &rSzie)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          std::vector<unsigned char> values;
          bool res = dgtIn->readValue(values);
          int index = 0;
          for(auto v : values)
          {
            rValue[index++] = v;
          }

          rSzie = index;

          return res;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new DigitalIoAdapter();
          }
      }
  }
}

    
    
