#include <QCoreApplication>
#include "rcvcmdtask.h"
#include "cmddecode.h"
#include "../src/PfBus/zeroMq/zmqsend.h"
#include "../src/PfBus/zeroMq/zmqreceive.h"
#include "../../src/PfCommon/tools/ut_error.h"
#include "../src/PfAdapter/ZmqRcvAdapter/ZmqRcvAdapter.h"
#include "../src/PfAdapter/ZmqSendAdapter/ZmqSendAdapter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    /*argc=5;
    std::string strtemp="127.0.0.1";
    memcpy(argv[1],strtemp.c_str(),20);
    strtemp="2202";
    memcpy(argv[2],strtemp.c_str(),20);
    strtemp="";
    memcpy(argv[3],strtemp.c_str(),20);
    strtemp="2203";
    memcpy(argv[4],strtemp.c_str(),20);*/
    if(argc != 5)
    {
        throw std::runtime_error(std::to_string(argc));
        return -1;
    }

    ///根据传入的参数初始化交互类
    int index = 1;
    PfAdapter::Adapter *zmqSend = nullptr;
    PfAdapter::Adapter *zmqRcv = nullptr;

    try
    {
        std::string sendIp = argv[index++];
        std::string sendPort = argv[index++];

        std::string rcvIp = argv[index++];
        std::string rcvPort = argv[index++];

#ifndef QT_NO_DEBUG
        UT_SHOW(sendIp + " " + sendPort + " " + rcvIp + " " + rcvPort);
#endif

        zmqSend = new PfAdapter::ZmqSendAdapter();
        dynamic_cast<PfAdapter::ZmqSendAdapter*>(zmqSend)->init(sendIp, sendPort);

        zmqRcv = new PfAdapter::ZmqRcvAdapter();
        dynamic_cast<PfAdapter::ZmqRcvAdapter*>(zmqRcv)->init(rcvIp, rcvPort);
    }
    catch(std::runtime_error err)
    {
        UT_SHOW(std::string(err.what()));
    }
    ///启动接收命令
    rcvCmdTask *rcvTask = new rcvCmdTask();
    rcvTask->setRcvAdapter(zmqRcv);



    cmdDecode decode(zmqSend);
    QObject::connect(rcvTask, &rcvCmdTask::toParse, &decode, &cmdDecode::parse);

    try
    {
        //decode.load("E:\\work\\[43]地面测发控系统仿真系统\\flow.xml");
        decode.initPrograme();
    }
    catch(std::runtime_error err)
    {
        ///上报初始化状态

        std::ostringstream strCode;
        strCode.str("");
        strCode << "{\"msgType\":\"init\",\"result\":\"" << err.what();
        strCode << "\"}}";

        zmqSend->sendMsg((const char*)strCode.str().c_str(), strCode.str().size());
    }

    rcvTask->startTask();
    return a.exec();
}
