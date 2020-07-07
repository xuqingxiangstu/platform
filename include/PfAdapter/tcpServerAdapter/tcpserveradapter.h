#ifndef TCPSERVERADAPTER_H
#define TCPSERVERADAPTER_H

#include "tcpserveradapter_global.h"

#include "../adapter/Adapter.h"
#include "../../PfBus/tcpServer/tcpServer.h"
#include <memory>
#include <QObject>
#include <QByteArray>

/** 版本号 **/
#define VERSION "1.0.0"

namespace Pf
{
  namespace PfAdapter
  {
      class TCPSERVERADAPTERSHARED_EXPORT tcpServerAdapter : public QObject, public Adapter
      {
          Q_OBJECT
      public:
          tcpServerAdapter(QObject *parent = 0);
          ~tcpServerAdapter();
      public:
          /*
           *{
           *    "type":"local",
           *    "ip_addr":"192.168.34.2",
           *    "port":4001
           *}
           */
          void init(const std::string &json) override;
          void init(const TiXmlElement *ele) override;
          bool sendMsg(const char *msg, const int &msgSize) override;
          bool receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize = 1024, const unsigned int &timeOut = 0xFFFFFFFF) override;
          std::string getClassName() override;
          std::string getId() override;
          int getAttribute(const std::string &attr, void *value) override;
          int setAttribute(const std::string &attr, const void *value) override;
          bool writeValue(const double *value, const int size) override;
          bool readValue(double *value, int &rSzie) override;
          std::string version() override {return VERSION;}
      signals:
          void toMsg(QByteArray);

          void rcvMsg(QByteArray);
      private:
          std::mutex mMutex;   ///< 消息锁
          std::shared_ptr<PfBus::tcpServer> mTcpClientObj;
      };

      extern "C"
      {
          /**
          * @brief 加载类
          * @return 类指针
          */
          TCPSERVERADAPTERSHARED_EXPORT Adapter *LoadClass();
      }

  }
}

#endif // TCPSERVERADAPTER_H
