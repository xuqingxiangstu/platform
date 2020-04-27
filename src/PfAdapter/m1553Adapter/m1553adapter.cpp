#include "m1553adapter.h"

#include "../../PfCommon/jsoncpp/json.h"
#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    using namespace PfBus;

  namespace PfAdapter
  {
      m1553Adapter::m1553Adapter():
          mRtAddr(1),
          mSaAddr(1)
      {
      }

      m1553Adapter::~m1553Adapter()
      {

      }

      void m1553Adapter::init(const std::string &json)
      {
          std::ostringstream strErr;
          Json::Reader reader;
          Json::Value root;          

          if(!reader.parse(json, root))
          {
            strErr.str("");
            strErr << "json格式错误(" << json << ")";

            UT_THROW_EXCEPTION(strErr.str());
          }

          std::string cardNum = root["card_num"].asString();

          std::string mode = root["mode"].asString();

          mBusObj = std::make_shared<Pf::PfBus::M1553B>();
          mBusObj->init(cardNum, mode);
      }

      void m1553Adapter::init(const TiXmlElement *xmlEle)
      {
          const TiXmlElement *pTmpElem = NULL;

          std::string strRemoteIp = "";
          std::string strRemotePort = "";

      }

      bool m1553Adapter::sendMsg(const char *msg, const int &msgSize)
      {
          std::vector<unsigned short> sendMsg;
          int convertSize = msgSize;

          if(convertSize % 2)
              convertSize -= 1;

          int index = 0;

          for (index = 0; index < convertSize; index += 2)
          {
              sendMsg.push_back(msg[index]);
              sendMsg.push_back(msg[index+1]);
          }

          if(convertSize % 2)
          {
              sendMsg.push_back(msg[index]);
              sendMsg.push_back(0);
          }

          std::unique_lock<std::mutex> lk(mMutex);

          return mBusObj->sendMsg(&sendMsg.at(0), sendMsg.size(), mRtAddr, mSaAddr);
      }

      bool m1553Adapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);

          bool res = mBusObj->receiveMsg(mRcvRtAddr, mRcvSaAddr, (unsigned short*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
          rcvSize *= 2;
          return res;
      }

      void m1553Adapter::setSendRtAndSa(const std::string &rt, const std::string &sa)
      {
          mRtAddr = std::atoi(rt.c_str());
          mSaAddr = std::atoi(sa.c_str());
      }

      std::string m1553Adapter::getClassName()
      {
          return VAR_NAME(m1553Adapter);
      }

      std::string m1553Adapter::getId()
      {
          return "";
      }
      int m1553Adapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int m1553Adapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool m1553Adapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool m1553Adapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new m1553Adapter();
          }
      }
  }
}
