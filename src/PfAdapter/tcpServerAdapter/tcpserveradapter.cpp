#include "tcpserveradapter.h"

#include "../../PfCommon/jsoncpp/json.h"
#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    using namespace PfBus;

  namespace PfAdapter
  {
      tcpServerAdapter::tcpServerAdapter(QObject *parent)
          :QObject(parent)
      {
      }

      tcpServerAdapter::~tcpServerAdapter()
      {

      }

      void tcpServerAdapter::init(const std::string &json)
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


          mTcpClientObj = std::make_shared<PfBus::tcpServer>();

          mTcpClientObj->init(strRemoteIp, strRemotePort);

          connect(this, &tcpServerAdapter::toMsg, mTcpClientObj.get(), &tcpServer::onSendMsg);
      }

      void tcpServerAdapter::init(const TiXmlElement *xmlEle)
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

          mTcpClientObj = std::make_shared<PfBus::tcpServer>();

          mTcpClientObj->init(strRemoteIp, std::atoi(strRemotePort.c_str()));

      }

      bool tcpServerAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          emit toMsg(QByteArray(msg, msgSize));
          return true;
      }

      bool tcpServerAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          std::unique_lock<std::mutex> lk(mMutex);
          return mTcpClientObj->receiveMsg((unsigned char*)msg, (unsigned int*)&rcvSize, maxRcvSize, timeOut);
      }

      std::string tcpServerAdapter::getClassName()
      {
          return VAR_NAME(tcpAdapter);
      }

      std::string tcpServerAdapter::getId()
      {
          return "";
      }
      int tcpServerAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int tcpServerAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool tcpServerAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool tcpServerAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new tcpServerAdapter();
          }
      }
  }
}
