#include "SerialPortAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      SerialPortAdapter::SerialPortAdapter():
          sep(nullptr)
      {
      }

      SerialPortAdapter::~SerialPortAdapter()
      {

      }

      void SerialPortAdapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          //串口名
          std::string strComName = "";
          //波特率,
          unsigned long u32BandRate = 0;
          //数据位  4-8
          unsigned char u8Bytesize = 0;
          //停止位  0，1，2 = 1bit, 1.5bit, 2bit
          unsigned char u8StopBit = 0;
          //奇偶校验位 0-4=None,Odd,Even,Mark,Space
          unsigned char u8Parity = 0;

          pTmpElem = xmlEle->FirstChildElement("rname");
          if (pTmpElem)
          {
              strComName = pTmpElem->GetText();
          }

          pTmpElem = xmlEle->FirstChildElement("baud");
          if (pTmpElem)
          {
              std::string strBandRate = pTmpElem->GetText();
              u32BandRate = atoi(strBandRate.c_str());

          }

          pTmpElem = xmlEle->FirstChildElement("dataBits");
          if (pTmpElem)
          {
              std::string strByteSize = pTmpElem->GetText();
              u8Bytesize = std::atoi(strByteSize.c_str());
          }

          pTmpElem = xmlEle->FirstChildElement("stopBits");
          if (pTmpElem)
          {
              std::string strStopBit = pTmpElem->GetText();
              u8StopBit = std::atoi(strStopBit.c_str());
          }

          pTmpElem = xmlEle->FirstChildElement("parity");
          if (pTmpElem)
          {
              std::string strParity = pTmpElem->GetText();
              u8Parity = std::atoi(strParity.c_str());
          }

          sep = std::make_shared<PfBus::SerialPort>();

          sep->Init(strComName, u32BandRate, u8Bytesize, u8StopBit, u8Parity);
      }

      bool SerialPortAdapter::sendMsg(const char *msg, const int &msgSize, const std::string &ipAddr, const int &port)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return sep->SndUartData((unsigned char*)msg, msgSize);
      }

      bool SerialPortAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return sep->RcvUartData((unsigned char*)msg, DWORD(maxRcvSize), (DWORD*)&rcvSize, timeOut);
      }

      std::string SerialPortAdapter::getClassName()
      {
          return VAR_NAME(SerialPortAdapter);
      }

      std::string SerialPortAdapter::getId()
      {
          return "";
      }
      int SerialPortAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int SerialPortAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool SerialPortAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool SerialPortAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new SerialPortAdapter();
          }
      }
  }
}

    
    
