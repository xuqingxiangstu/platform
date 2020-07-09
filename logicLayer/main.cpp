#include <QCoreApplication>
#include "rcvcmdtask.h"
#include "cmddecode.h"
#include "../src/PfBus/zeroMq/zmqsend.h"
#include "../src/PfBus/zeroMq/zmqreceive.h"
#include "../src/PfCommon/tools/ut_error.h"
#include "../src/PfAdapter/ZmqRcvAdapter/ZmqRcvAdapter.h"
#include "../src/PfAdapter/ZmqSendAdapter/ZmqSendAdapter.h"
#include "../src/PfSql/paramsTable/paramstable.h"

#include "virtualParams/virtualparams.h"

#include <QDateTime>
#include <QDebug>
#include "flow/flow.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include "./custom/zy.h"

#define DEBUG_ZMQ   0

using namespace Json;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<Value>("Value");

    //qDebug() << QDateTime::currentDateTime().toString("hh:mm.ss.zzz");
    paramsTable::getInstance();
    //qDebug() << QDateTime::currentDateTime().toString("hh:mm.ss.zzz");
    virtualParams::getInstance();

#if 0
    try
    {
        paramsTable::getInstance();
        virtualParams::getInstance();

        PfAdapter::PfAdapterManager *mAdpterManagerObj = new PfAdapter::PfAdapterManager();
        mAdpterManagerObj->init("./cfgfile/logic_devcfg.xml");

        PfIcdWorkBench::icdFrameAdapter *mIcdFrameAdpter = new PfIcdWorkBench::icdFrameAdapter();
        mIcdFrameAdpter->init("./cfgfile/icd.xml");

        flowManager mang;
        mang.init("./cfgfile/flow_test.xml");

        SHOW(mang.getRunItems().toStyledString());

        mang.setAdapterManager(mAdpterManagerObj);
        mang.setIcdAdapter(mIcdFrameAdpter);
        mang.exe();

        qDebug() << "exit";
    }
    catch(std::runtime_error err)
    {
        qDebug() << err.what();
    }

    return a.exec();
#endif

#if 0
    argc=5;
    std::string strtemp="127.0.0.1";
    memcpy(argv[1],strtemp.c_str(),strtemp.size());
    strtemp="2202";
    memcpy(argv[2],strtemp.c_str(),strtemp.size());
    strtemp="";
    memcpy(argv[3],strtemp.c_str(),strtemp.size());
    strtemp="2203";
    memcpy(argv[4],strtemp.c_str(),strtemp.size());
#endif

#ifndef DEBUG_ZMQ
    if(argc != 5)
    {
        throw std::runtime_error(std::to_string(argc));
        return -1;
    }
#endif
    ///根据传入的参数初始化交互类
    int index = 1;
    PfAdapter::Adapter *zmqSend = nullptr;
    PfAdapter::Adapter *zmqRcv = nullptr;
    //std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> frameAdapter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();
    try
    {
#ifndef DEBUG_ZMQ
        std::string sendIp = argv[index++];
        std::string sendPort = argv[index++];

        std::string rcvIp = argv[index++];
        std::string rcvPort = argv[index++];
#else
        std::string sendIp = "127.0.0.1";
        std::string sendPort = "2202";

        std::string rcvIp = "";
        std::string rcvPort = "2203";
#endif


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

    std::shared_ptr<cmdDecode> decode = std::make_shared<cmdDecode>(zmqSend);
    QObject::connect(rcvTask, &rcvCmdTask::toParse, decode.get(),&cmdDecode::parse);

    try
    {
        //decode.load("E:\\work\\[43]地面测发控系统仿真系统\\flow.xml");
        decode->initPrograme();
//#ifndef QT_NO_DEBUG
#if 0
        std::string uuid = "{9df9f5d7-81fe-4a4e-851c-44021d307aec}";

        Json::Value tmp;
        tmp.append(uuid);
        //tmp.append("{f7b24908-b0d0-4c30-b92f-c228fddc536a}");
        Json::Value fJs;
        fJs["flows"] = tmp;       
        decode->initFlow(fJs);

        Json::Value value;
        value["record_uuid"] = uuid;
        decode->startTest(value);
#endif
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

    a.exec();

    return true;
}
