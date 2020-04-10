#include "AnalogInAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      AnalogInAdapter::AnalogInAdapter()
      {
      }

      AnalogInAdapter::~AnalogInAdapter()
      {

      }

      void AnalogInAdapter::init(const TiXmlElement *xmlEle)
      {
          //设备名称
          std::string strRName = "";
          std::string strPort = "";
          //port号
          std::vector<int> channels;
          const TiXmlElement *pTmpElem = nullptr;

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

          pTmpElem = xmlEle->FirstChildElement("sampleRate");
          if (pTmpElem)
          {
              std::string strRate = pTmpElem->GetText();
              mRate = atoi(strRate.c_str());
          }
          pTmpElem = xmlEle->FirstChildElement("sampleSize");
          if (pTmpElem)
          {
              std::string strSize = pTmpElem->GetText();
              mSize = atoi(strSize.c_str());
          }

          anaIn = std::make_shared<PfNi::AnalogIn>();

          anaIn->initCard(strRName, channels);

          anaIn->setSamplingRateAndSize(mRate, mSize);
      }

      bool AnalogInAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool AnalogInAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string AnalogInAdapter::getClassName()
      {
          return VAR_NAME(AnalogInAdapter);
      }

      std::string AnalogInAdapter::getId()
      {
          return "";
      }
      int AnalogInAdapter::getAttribute(const std::string &attr, void *value)
      {   
          if( "ChannelSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);
              *((int*)value) = anaIn->getChannelSize();
          }
          else if("SampleRate" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);
              *((int*)value) = anaIn->getSamplingRate();
          }
          else if("SampleSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);
              *((int*)value) = anaIn->getSamplingSize();
          }else
          {
                return 0;
          }
          return -1;
      }

      int AnalogInAdapter::setAttribute(const std::string &attr, const void *value)
      {
          if("SampleRate" == attr)
          {
              mRate = *((int*)value);

              std::unique_lock<std::mutex> lk(mMutex);
              return anaIn->setSamplingRateAndSize(mRate, mSize);
          }
          else if("SampleSize" == attr)
          {
              mSize = *((int*)value);

              std::unique_lock<std::mutex> lk(mMutex);
              return anaIn->setSamplingRateAndSize(mRate, mSize);
          }else
          {
              return 0;
          }

          return -1;

          /*if("SampleRate" == attr)
          {
              mRate = *((int*)value);

              std::unique_lock<std::mutex> lk(mMutex);
              return anaIn->setSamplingRateAndSize(mRate, mSize);
          }
          else if("SampleSize" == attr)
          {
              mSize = *((int*)value);

              std::unique_lock<std::mutex> lk(mMutex);
              return anaIn->setSamplingRateAndSize(mRate, mSize);
          }else
          {
              return 0;
          }
          */
          return -1;
      }

      bool AnalogInAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool AnalogInAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new AnalogInAdapter();
          }
      }
  }
}

    
    
