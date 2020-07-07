#include "tcpAdapter.h"

#include "../../PfCommon/jsoncpp/json.h"
#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    using namespace PfBus;

  namespace PfAdapter
  {
      tcpAdapter::tcpAdapter(QObject *parent)
          :QObject(parent)
      {
      }

      tcpAdapter::~tcpAdapter()
      {

      }

      void tcpAdapter::init(const std::string &json)
      {
          std::ostringstream strErr;
          Json::Reader reader;
          Json::Value root;
          std::string strRemoteIp = "";
          int strRemotePort;

          if(!reader.parse(json, root))
          {
            strErr.str("");
            strErr << "json格式错误(" << json << ")";

            UT_THROW_EXCEPTION(strErr.str());
          }

          strRemoteIp = root["ip_addr"].asString();

          strRemotePort = root["port"].asInt();


          mTcpClientObj = std::make_shared<PfBus::tcpClient>();

          mTcpClientObj->init(strRemoteIp, strRemotePort);

          connect(this, &tcpAdapter::toMsg, mTcpClientObj.get(), &tcpClient::onSendMsg);

          connect(mTcpClientObj.get(), &tcpClient::rcvMsg, this, &tcpAdapter::rcvMsg);
      }      

      void tcpAdapter::init(const TiXmlElement *xmlEle)
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

          mTcpClientObj = std::make_shared<PfBus::tcpClient>();

          mTcpClientObj->init(strRemoteIp, std::atoi(strRemotePort.c_str()));

      }

      void tcpAdapter::setTrigger(bool state)
      {
           mTcpClientObj->setTrigger(state);
      }

      bool tcpAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          emit toMsg(QByteArray(msg, msgSize));
          return true;
      }

      bool tcpAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return mTcpClientObj->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
      }

      std::string tcpAdapter::getClassName()
      {
          return VAR_NAME(tcpAdapter);
      }

      std::string tcpAdapter::getId()
      {
          return "";
      }
      int tcpAdapter::getAttribute(const std::string &attr, void *value)
      {
          int result=0;
          if(mTcpClientObj->getAttribute(attr,value))
          {
              result= 1;
          }
          return 0;
      }

      int tcpAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool tcpAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool tcpAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new tcpAdapter();
          }
      }
  }
}

    
    
