#include "AnalogOutAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      AnalogOutAdapter::AnalogOutAdapter():
          anaOut(nullptr),
          mRate(5),
          mSize(1)
      {
      }

      AnalogOutAdapter::~AnalogOutAdapter()
      {

      }

      void AnalogOutAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          //设备名称
          std::string strRName = "";
          std::string strPort = "";
          //port号
          std::vector<int> channels;
          //输出类型
          std::string strOutType = "";
          //最小值
          double minV = 0;
          //最大值
          double maxV = 0;

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

          pTmpElem = xmlEle->FirstChildElement("outType");
          if (pTmpElem)
          {
              strOutType = pTmpElem->GetText();
          }
          pTmpElem = xmlEle->FirstChildElement("minV");
          if (pTmpElem)
          {
              std::string tmpMinV = pTmpElem->GetText();
              minV = stod(tmpMinV);
          }
          pTmpElem = xmlEle->FirstChildElement("maxV");
          if (pTmpElem)
          {
              std::string tmpMaxV = pTmpElem->GetText();
              maxV = stod(tmpMaxV);
          }

          anaOut = std::make_shared<PfNi::AnalogOut>();

          anaOut->initCard(strRName, channels);

          anaOut->setSamplingRateAndSize(mRate, mSize);

          PfNi::AnalogOut::OutType outType = PfNi::AnalogOut::AmpsOut;
          if("AmpsOut" == strOutType)
          {
              outType = PfNi::AnalogOut::AmpsOut;
          }
          else if("VoltsOut" == strOutType)
          {
              outType = PfNi::AnalogOut::VoltsOut;
          }
          anaOut->setOutParam(outType, minV, maxV);

      }

      bool AnalogOutAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool AnalogOutAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string AnalogOutAdapter::getClassName()
      {
          return VAR_NAME(AnalogOutAdapter);
      }

      std::string AnalogOutAdapter::getId()
      {
          return "";
      }
      int AnalogOutAdapter::getAttribute(const std::string &attr, void *value)
      {
          if( "ChannelSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);
              *((int*)value) = anaOut->getChannelSize();
          }
          else if("SampleRate" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);
              *((int*)value) = anaOut->getSamplingRate();
          }
          else if("SampleSize" == attr)
          {
              std::unique_lock<std::mutex> lk(mMutex);
              *((int*)value) = anaOut->getSamplingSize();
          }else
          {
                return 0;
          }
          return -1;
      }

      int AnalogOutAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool AnalogOutAdapter::writeValue(const double *value, const int size)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          bool res = anaOut->writeValue(value);
          return res;
      }

      bool AnalogOutAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new AnalogOutAdapter();
          }
      }
  }
}

    
    
