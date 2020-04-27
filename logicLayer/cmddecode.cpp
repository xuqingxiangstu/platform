#include "cmddecode.h"
#include <sstream>
#include <QDebug>
#include <string>
#include <vector>
#include <QDir>
#include <tuple>
#include <set>

#include "../../src/PfCommon/tools/ut_error.h"
#include "../src/PfCommon/cmdToJson/cmdtojson.h"
#include "../src/PfSql/paramsTable/paramstable.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"
#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"
#include "../src/PfSql/paramsTable/udptable.h"
#include "../src/PfSql/paramsTable/tcptable.h"
#include "../src/PfSql/paramsTable/m1553btable.h"
#include "../src/PfSql/paramsTable/tcpservertable.h"
#include "../src/PfAdapter/UnicastAdapter/UnicastAdapter.h"
#include "../src/PfAdapter/tcpAdapter/tcpAdapter.h"
#include "../src/PfAdapter/tcpServerAdapter/tcpServerAdapter.h"
#include "../src/PfAdapter/m1553Adapter/m1553adapter.h"
#include "uibus.h"
#include "virtualParams/virtualparams.h"
#include "./custom/custommsg.h"

cmdDecode::cmdDecode(PfAdapter::Adapter  *obj, QObject *parent )
    :QObject(parent),
      mCmdSendObj(obj),
      mIcdFrameAdpter(nullptr),
      mAdpterManagerObj(nullptr),
      mIsInitSuccessful(false)

{
    mRcvMsgTask = std::make_shared<rcvTask>();

    connect(customMsg::getInstance().get(), &customMsg::switchPermiss, mRcvMsgTask.get(), &rcvTask::onSwitchPermiss);
}
cmdDecode::~cmdDecode()
{
    qDebug() << "~cmdDecode";
}
void cmdDecode::respond(const std::string &code)
{
    if(mCmdSendObj)
        mCmdSendObj->sendMsg(code.c_str(), code.size());

#ifndef QT_NO_DEBUG
    SHOW(code);
#endif
}
void cmdDecode::parse(QString srcJson)
{
    ///将接收到的消息转化为Json对象
#ifndef QT_NO_DEBUG
    //qDebug() << srcJson;
#endif
    Json::Value root;
    Json::Reader reader;
    if(reader.parse(srcJson.toStdString(),root))
    {        
        cmdMsg(root);
    }
    else
    {
       std::string strErrInfo= "json语句解析失败，错误的json语句为"+ srcJson.toStdString();
       std::string strTemp = PfCommon::cmdToJson::replyErrorCmd(strErrInfo);
       respond(strTemp);
    }
}
void cmdDecode::cmdMsg(Json::Value jsValue)
{
    std::string type = jsValue[MSG_TYPE].asString();

    //初始化失败则不进行命令交互
    if(!mIsInitSuccessful)
    {
        std::string strErrInfo= "初始化失败，不进行交互！";
        std::string strTemp = PfCommon::cmdToJson::replyErrorCmd(strErrInfo);
        respond(strTemp);
        return ;
    }

#ifndef QT_NO_DEBUG
    UT_SHOW(type);
#endif

    ///程序初始化
    if(type == INIT_TEST)
    {
        initPrograme();
    }
    else if(type == INIT_FLOW)
    {
        initFlow(jsValue["msg"]);
    }
    else if(type == START_TEST)
    {
        startTest(jsValue["msg"]);
    }
    else if(type == MANUALTRIGGER)
    {
        manualTrigger(jsValue["msg"]);
    }
    else if(type == STOP_TEST)
    {
        stopTest(jsValue["msg"]);
    }
    else if(type == SUSPEND_TEST)
    {
        suspendTest(jsValue["flow"].asString());
    }
    else if(type == CONTINUE_TEST)
    {
        continueTest(jsValue["flow"].asString());
    }
    else if(type == EXIT_TEST)
    {
        exitTest(jsValue["flow"].asString());
    }
    else if(type == SINGLE_SEND_TEST)
    {
        singleSendTest(jsValue["msg"]);
    }
    else if(type == GET_RUNITES)
    {
        getRunItems();
    }
    else if(type == LOAD)
    {
        load(jsValue["flow"].asString(), jsValue["path"].asString());
    }
    else
    {
        std::string strErrInfo= "命令类型错误，类型(" + type + " )不存在";
        std::string strTemp = PfCommon::cmdToJson::replyErrorCmd(strErrInfo);
        respond(strTemp);
    }
}
void cmdDecode::rs422OutMsg(Json::Value rs422Obj)
{
    std::ostringstream errCode;

    std::string devId = rs422Obj["id"].asString();

    std::vector<unsigned char> msg;
    Json::Value array = rs422Obj["data"];

    for(unsigned int index = 0; index <  array.size(); index++)
    {
        msg.push_back(array[index].asInt());
    }

    PfAdapter::Adapter *devObj = nullptr;
    if(mAdpterManagerObj->getAdapter(devId, &devObj))
    {
        devObj->sendMsg((const char*)&msg.at(0), msg.size());
    }
    else
    {
        errCode << "get obj faild id = " << devId;
    }

    if(errCode.str() == "")
        errCode << OK;

    ///返回结果
    std::string strCode=PfCommon::cmdToJson::replyRs422ResCmd(errCode.str());
    respond(strCode);
}

void cmdDecode::switchOutMsg(Json::Value switchArray)
{
    std::ostringstream errCode;

    for(unsigned int index = 0; index < switchArray.size(); index++)
    {
        Json::Value obj = switchArray[index];
        std::string id = obj["id"].asString();
        int channel = obj["channel"].asInt();
        bool status = obj["status"].asBool();

#ifndef QT_NO_DEBUG
        UT_SHOW(id + " " + std::to_string(channel) + " " + (status == true ? "open" : "close"));
#endif

        PfAdapter::Adapter *devObj = nullptr;
        if(mAdpterManagerObj->getAdapter(id, &devObj))
        {
           // dynamic_cast<switchOpt*>(devObj)->setValue(channel, status); fjt,目前先注释调
        }
        else
        {
            errCode << "get obj faild id = " << id;
        }
    }

    if(errCode.str() == "")
        errCode << OK;

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("switch_out");
    vecMsg.push_back(errCode.str());
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
}

void cmdDecode::analogOutMsg(Json::Value analogArray)
{
    std::ostringstream errCode;

    for(unsigned int index = 0; index < analogArray.size(); index++)
    {
        std::string id = analogArray[index]["id"].asString();
        int channel = analogArray[index]["channel"].asInt();
        double min = analogArray[index]["min"].asDouble() / 1000.0;    ///界面输入为mA、实际输出为A
        double max = analogArray[index]["max"].asDouble() / 1000.0;

#ifndef QT_NO_DEBUG
        UT_SHOW(id + " " + std::to_string(channel) + " " + std::to_string(min)+ " " + std::to_string(max));
#endif

        emit setAnalogOut(id, channel, min, max);
    }

    if(errCode.str() == "")
        errCode << OK;

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("analog_out");
    vecMsg.push_back(errCode.str());
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
}


void cmdDecode::getRunItems()
{
    Json::Value items;

    items["msgType"] = GET_RUNITES;

    Json::Value msg;

    for(auto itor = mFLowsObj.begin(); itor != mFLowsObj.end(); itor++)
    {
        Json::Value flow;
        flow["record_uuid"] = itor->first;        
        flow["flow"] = (itor->second)->getRunItems();

        //msg.append(flow);

        items["msg"] = flow;

        respond(items.toStyledString());
    }
}

void cmdDecode::singleSendTest(Json::Value value)
{
    emit singleTestSig(value);
}

void cmdDecode::exitTest(std::string flowType)
{
#if 0
    std::string errCode;
    try
    {
        std::map<std::string, RunFlow*>::iterator it;
        it = mFLows.find(flowType);
        if(it!=mFLows.end())
        {
            mFLows[flowType]->exitAllTest();

            errCode = OK;
        }
        else
        {
            errCode = NO_FLOW;
        }
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("exit");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
#endif
}

void cmdDecode::continueTest(std::string flowType)
{
#if 0
    std::string errCode;
    try
    {
        std::map<std::string, RunFlow*>::iterator it;
        it = mFLows.find(flowType);
        if(it!=mFLows.end())
        {
            mFLows[flowType]->continueTest();

            errCode = OK;
        }
        else
        {
            errCode = NO_FLOW;
        }
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("continue");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
#endif
}

void cmdDecode::suspendTest(std::string flowType)
{
#if 0
    std::string errCode;
    try
    {
        std::map<std::string, RunFlow*>::iterator it;
        it = mFLows.find(flowType);
        if(it!=mFLows.end())
        {
            mFLows[flowType]->suspendTest();

            errCode = OK;
        }
        else
        {
            errCode = NO_FLOW;
        }
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("suspend");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
#endif
}
void cmdDecode::stopTest(const Json::Value &msg)
{
    std::string errCode;

    std::string uuid = "";
    try
    {
        uuid = msg["record_uuid"].asString();

        if(!mIsInitFlow.contains(uuid))
        {
            mIsInitFlow[uuid] = false;
        }

        if(!mIsInitFlow[uuid])
        {
            respond(resultMsg(START_TEST, "[ERROR] 初始化流程失败，禁止停止测试!", uuid));
            return;
        }

        auto it = mFLowsObj.find(uuid);

        if(it != mFLowsObj.end())
        {
            if(mFLowsObj[uuid]->isRunning())
            {
                mFLowsObj[uuid]->exit();
                errCode = OK;
            }
            else
            {
                errCode = NOT_RUNNING;
            }
        }
        else
        {
            errCode = NO_FLOW;
        }
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果
    respond(resultMsg(STOP_TEST, errCode, uuid));
}

void cmdDecode::manualTrigger(const Json::Value &msg)
{
    std::string errCode;
    std::string uuid = "";
    try
    {
        uuid = msg["record_uuid"].asString();

        if(!mIsInitFlow.contains(uuid))
        {
            mIsInitFlow[uuid] = false;
        }

        if(!mIsInitFlow[uuid])
        {
            respond(resultMsg(START_TEST, "[ERROR] 初始化流程失败，禁止启动测试!", uuid));
            return;
        }

        virtualParams::getInstance()->setValue({uuid, "UI", ""}, mapValue());
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果

    respond(resultMsg(MANUALTRIGGER, errCode, uuid));
}

void cmdDecode::startTest(const Json::Value &msg)
{
    std::string errCode;

    std::string uuid = "";

    try
    {

        uuid = msg["record_uuid"].asString();

        if(!mIsInitFlow.contains(uuid))
        {
            mIsInitFlow[uuid] = false;
        }

        if(!mIsInitFlow[uuid])
        {
            respond(resultMsg(START_TEST, "[ERROR] 初始化流程失败，禁止启动测试!", uuid));
            return;
        }

        auto it = mFLowsObj.find(uuid);

        if(it != mFLowsObj.end())
        {
            if(!mFLowsObj[uuid]->isRunning())
            {
                std::string flowUuid = "";
                std::string subFlowUuid = "";

                if(!msg["flow_uuid"].isNull())
                    flowUuid = msg["flow_uuid"].asString();

                if(!msg["sub_flow_uuid"].isNull())
                    subFlowUuid = msg["sub_flow_uuid"].asString();

                mFLowsObj[uuid]->exe(flowUuid, subFlowUuid);

                errCode = OK;
            }
            else
            {
                errCode = RUNNING;
            }
        }
        else
        {
            errCode = NO_FLOW;
        }
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果

    respond(resultMsg(START_TEST, errCode, uuid));
}

void cmdDecode::load(std::string flowType, std::string path)
{
#if 0
    std::string errCode;
    try
    {
        std::map<std::string, RunFlow*>::iterator it;
        it = mFLows.find(flowType);
        if(it!=mFLows.end())
        {
            if(!mFLows[flowType]->isRunning())
            {
                mFLows[flowType]->init(path.c_str());
                mFLows[flowType]->setFlowType(flowType);
                mFLows[flowType]->setMsgObj(mCmdSendObj);

                errCode = OK;
            }
            else
            {
                errCode = RUNNING;
            }
        }
        else
        {
            errCode = NO_FLOW;
        }
    }
    catch(std::runtime_error err)
    {
        errCode = err.what();
    }

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("load");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
#endif
}
void cmdDecode::initPrograme()
{
    std::string errCode = "ok";
    static bool isInit = false;

    if(!isInit)
    {
        try
        {
            paramsTable::getInstance();
            virtualParams::getInstance();

            mAdpterManagerObj = std::make_shared<PfAdapter::PfAdapterManager>();

            mRcvMsgTask->setPfAdapterManager(mAdpterManagerObj);
            //mAdpterManagerObj->init("./cfgfile/logic_devcfg.xml");

            //将发送UI适配器加入管理
            mAdpterManagerObj->setAdapter("UI", mCmdSendObj);

            uiBus::getInstance()->setUiAdapter(mCmdSendObj);

            mIcdFrameAdpter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();
            mIcdFrameAdpter->init("./cfgfile/icd.xml");
            mRcvMsgTask->setIcdFrameAdpter(mIcdFrameAdpter);

            //mSingleTestObj = std::make_shared<singleTest>();
            //connect(this, &cmdDecode::singleTestSig, mSingleTestObj, &singleTest::onTest);

            mIsInitSuccessful = true;
        }
        catch(std::runtime_error err)
        {
            errCode = err.what();
        }

        isInit = true;
    }

    ///返回初始化结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("init");
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
}

void cmdDecode::initFlow(const Json::Value &msg)
{
    Json::Value flows = msg["flows"];
    std::ostringstream errorInfo;
    errorInfo.str("");

    //停止所有接收线程
    mRcvMsgTask->stopTask();

    //清空之前流程

    for(auto itor = mFLowsObj.begin(); itor != mFLowsObj.end(); )
    {
        (itor->second).reset();
        mFLowsObj.erase(itor++);
    }

    //获取文件信息，并判断文件是否存在
    std::vector<std::tuple<std::string, std::string>> flowFileInfo;

    for(int index = 0; index < flows.size(); index++)
    {
        errorInfo.str("");

        std::string uuid = flows[index].asString();
        //从数据库中获取xml文件信息
        Json::Value js = flowRecordTable::getInstance()->getValue(uuid);
        if(!js.isNull())
        {
            std::string flowName = js[FLOW_RECORD_TABLE_FLOW_NAME].asString();
            std::string time = js[FLOW_RECORD_TABLE_CREATE_TIME].asString();
            std::string dbUuid = js[FLOW_RECORD_TABLE_UUID].asString();

            std::string file = dbUuid + ".xml";

            std::string path = "/flowFile/" + file;

            //校验文件是否存在

            QString filePath = QDir::currentPath() + QString::fromStdString(path);

            QFileInfo info(filePath);

            if(!info.exists(filePath))
            {
                errorInfo << "[ERROR] file not exist : " << filePath.toStdString();
                respond(resultMsg(INIT_FLOW, errorInfo.str(), uuid));
                mIsInitFlow[uuid] = false;
                continue;
            }

            flowFileInfo.push_back(std::make_tuple(uuid, filePath.toStdString()));
        }
        else
        {
            errorInfo << "[ERROR] get uuid(" << uuid << ")record faild";
            mIsInitFlow[uuid] = false;
            respond(resultMsg(INIT_FLOW, errorInfo.str(), uuid));
        }
    }

    //初始化各流程部

    for(auto info : flowFileInfo)
    {
        errorInfo.str("");

        try
        {
            std::shared_ptr<flowManager> tmpFlow = std::make_shared<flowManager>();
            tmpFlow->init(std::get<1>(info));
            tmpFlow->setAdapterManager(mAdpterManagerObj.get());
            tmpFlow->setIcdAdapter(mIcdFrameAdpter.get());
            tmpFlow->setRecordUuid(std::get<0>(info));

            mFLowsObj[std::get<0>(info)] = tmpFlow;
        }
        catch(std::runtime_error err)
        {
            errorInfo << "[ERROR]" << err.what();
        }
        catch(Json::LogicError err)
        {
            errorInfo << err.what();
        }
        catch(...)
        {
            errorInfo << "[ERROR] ...";
        }

        if(errorInfo.str() != "")
        {
            respond(resultMsg(INIT_FLOW, errorInfo.str(), std::get<0>(info)));
            mIsInitFlow[std::get<0>(info)] = false;
        }
    }

    //初始化之后，初始化各设备适配器
    if(errorInfo.str() == "")
    {
        try
        {
            resetAdapter();
        }
        catch(std::runtime_error err)
        {
            errorInfo << err.what();            
        }
        catch(Json::LogicError err)
        {
            errorInfo << err.what();
        }
        catch(...)
        {
            errorInfo << "[ERROR] ...";
        }
    }
}

bool cmdDecode::resetAdapter()
{
    bool res = true;

    std::ostringstream errorInfo;

    std::map<std::string, std::string> adapters;
    QMap<std::string, std::string> recordUuid;
    QMap<std::string, std::string> protocols;
    std::vector<std::tuple<std::string, std::string>> mAdapterUuids;

    for(auto itor = mFLowsObj.begin(); itor != mFLowsObj.end(); itor++)
    {      
        errorInfo.str("");
#if 0
        //获取各目的设备信息
        std::set<std::string> tmpV;

        Json::Value info = (itor->second)->getRunItems();

        for(int index = 0; index < info.size(); index++)
        {
            Json::Value subFlow = info[index]["sub_flow"];

            for(int subIndex = 0; subIndex < subFlow.size(); subIndex++)
            {
                tmpV.insert(subFlow[subIndex]["dest_system_uuid"].asString());
            }
        }
#endif
        std::string eqUuid = (itor->second)->eqSystemUuid();

        if(eqUuid == "")
        {
            errorInfo << "[ERROR] 等效设备 uuid == null ";
            respond(resultMsg(INIT_FLOW, errorInfo.str(), itor->first));
            mIsInitFlow[itor->first] = false;
            continue;
        }
        protocols[eqUuid] = (itor->second)->eqProtocol();
        recordUuid[eqUuid] = itor->first;
        mAdapterUuids.push_back(std::make_tuple(eqUuid, itor->first));
        adapters[eqUuid] = (itor->second)->dstSystemUuid();
    }


    //初始化各系统

    //step2:清空之前所有
    mAdpterManagerObj->deleteAll();

    //step3:重新初始化各系统

    std::vector<std::tuple<std::string, std::string, std::string, std::string>> eqUuids;

    for(auto itor = adapters.begin(); itor != adapters.end(); itor++)
    {
        errorInfo.str("");

        try
        {
            std::string eqSysUuid = itor->first;

            Json::Value devInfo;
            getDevInfo(eqSysUuid, devInfo);

            if(SYSTEM_INTERFACE_TABLE_UDP == devInfo["type"].asString())
            {
                if(mAdpterManagerObj->isExist(eqSysUuid))
                    continue;

                //获取目标IP+PORT
                Json::Value destDevInfo;
                getDestDevIpPort(itor->second, destDevInfo);


                //等效设备,源IP+PORT
                Pf::PfAdapter::Adapter *eqObj = new Pf::PfAdapter::UnicastAdapter();
                Json::Value paramJs;

                paramJs["local_ip"] = devInfo["ip_addr"];
                paramJs["local_port"] = devInfo["port"];
                paramJs["remote_ip"] = destDevInfo["ip_addr"];
                paramJs["remote_port"] = destDevInfo["port"];

                eqObj->init(paramJs.toStyledString());

                mAdpterManagerObj->setAdapter(eqSysUuid, eqObj);

                eqUuids.push_back(std::make_tuple(eqSysUuid, devInfo["name"].asString(), protocols[eqSysUuid], recordUuid[eqSysUuid]));

#if 0
                if(mAdpterManagerObj->isExist(eqSysUuid))
                    continue;

                //等效设备,源IP+PORT
                Pf::PfAdapter::Adapter *eqObj = new Pf::PfAdapter::virtualUnicastAdapter();
                Json::Value paramJs;

                paramJs["type"] = "local";
                paramJs["ip_addr"] = devInfo["ip_addr"];
                paramJs["port"] = devInfo["port"];

                dynamic_cast<Pf::PfAdapter::virtualUnicastAdapter*>(eqObj)->init(paramJs.toStyledString());

                mAdpterManagerObj->setAdapter(eqSysUuid, eqObj);

                eqUuids.push_back(std::make_tuple(eqSysUuid, devInfo["name"].asString(), protocols[eqSysUuid], recordUuid[eqSysUuid]));

                //初始化各发送系统
                auto sendSyss = itor->second;
                for(auto send_uuid : sendSyss)
                {
                    Json::Value sendInfo;
                    getDevInfo(send_uuid, sendInfo);

                    if(SYSTEM_INTERFACE_TABLE_UDP == sendInfo["type"].asString())
                    {
                        if(mAdpterManagerObj->isExist(send_uuid))
                            continue;

                        Pf::PfAdapter::Adapter *tmpObj = new Pf::PfAdapter::virtualUnicastAdapter();
                        Json::Value paramJs;

                        paramJs["type"] = "dst";
                        paramJs["ip_addr"] = sendInfo["ip_addr"];
                        paramJs["port"] = sendInfo["port"];

                        dynamic_cast<Pf::PfAdapter::virtualUnicastAdapter*>(tmpObj)->init(paramJs.toStyledString());
                        std::shared_ptr<PfBus::UnicastUdp> obj = dynamic_cast<Pf::PfAdapter::virtualUnicastAdapter*>(eqObj)->getUdpOpt();
                        dynamic_cast<Pf::PfAdapter::virtualUnicastAdapter*>(tmpObj)->setUdpOpt(obj);

                        mAdpterManagerObj->setAdapter(send_uuid, tmpObj);
                    }
                }
#endif
            }
            else if(SYSTEM_INTERFACE_TABLE_TCP == devInfo["type"].asString())
            {
                if(mAdpterManagerObj->isExist(eqSysUuid))
                    continue;

                Json::Value paramJs;

                paramJs["ip_addr"] = devInfo["ip_addr"];
                paramJs["port"] = devInfo["port"];

                Pf::PfAdapter::Adapter *eqObj = new Pf::PfAdapter::tcpAdapter();

                eqObj->init(paramJs.toStyledString());
                mAdpterManagerObj->setAdapter(eqSysUuid, eqObj);

                eqUuids.push_back(std::make_tuple(eqSysUuid, devInfo["name"].asString(), protocols[eqSysUuid], recordUuid[eqSysUuid]));
            }
            else if(SYSTEM_INTERFACE_TABLE_TCP_SERVER == devInfo["type"].asString())
            {
                if(mAdpterManagerObj->isExist(eqSysUuid))
                    continue;

                Json::Value paramJs;

                paramJs["ip_addr"] = devInfo["ip_addr"];
                paramJs["port"] = devInfo["port"];

                Pf::PfAdapter::Adapter *eqObj = new Pf::PfAdapter::tcpServerAdapter();

                eqObj->init(paramJs.toStyledString());
                mAdpterManagerObj->setAdapter(eqSysUuid, eqObj);

                eqUuids.push_back(std::make_tuple(eqSysUuid, devInfo["name"].asString(), protocols[eqSysUuid], recordUuid[eqSysUuid]));
            }
            else if(SYSTEM_INTERFACE_TABLE_1553B == devInfo["type"].asString())
            {
                if(mAdpterManagerObj->isExist(eqSysUuid))
                    continue;

                Json::Value paramJs;

                paramJs["card_num"] = devInfo["card_num"];
                paramJs["mode"] = devInfo["mode"];

                Pf::PfAdapter::Adapter *eqObj = new Pf::PfAdapter::m1553Adapter();

                eqObj->init(paramJs.toStyledString());
                mAdpterManagerObj->setAdapter(eqSysUuid, eqObj);

                eqUuids.push_back(std::make_tuple(eqSysUuid, devInfo["name"].asString(), protocols[eqSysUuid], recordUuid[eqSysUuid]));
            }
        }
        catch(std::runtime_error err)
        {
            errorInfo << err.what();
        }
        catch(Json::LogicError err)
        {
            errorInfo << err.what();
        }
        catch(...)
        {
            errorInfo << "[ERROR] ...";
        }

        if(errorInfo.str() != "")
        {
            for(auto tmpItor = mAdapterUuids.begin(); tmpItor != mAdapterUuids.end(); )
            {
                if(std::get<0>((*tmpItor)) == itor->first)
                {
                    respond(resultMsg(INIT_FLOW, errorInfo.str(), std::get<1>(*tmpItor)));
                    mIsInitFlow[std::get<1>(*tmpItor)] = false;
                    mAdapterUuids.erase(tmpItor);
                }
                else
                {
                    tmpItor++;
                }
            }
        }
    }

    //向界面发送初始化成功的uuid
    for(auto uuid : mAdapterUuids)
    {
        mIsInitFlow[std::get<1>(uuid)] = true;
        respond(resultMsg(INIT_FLOW, "", std::get<1>(uuid)));
    }

    //step4：启动接收线程
    mRcvMsgTask->setRcvUuid(eqUuids);

    mRcvMsgTask->startTask();

    return res;
}

void cmdDecode::getDestDevIpPort(const std::string &uuid, Json::Value &info)
{
    std::ostringstream errInfo;

    //获取系统表中的DEV_TYPE、DEV_UUID字段
    Json::Value tmpV = sysInterfaceTable::getInstance()->getValue(uuid);
    if(!tmpV.isNull())
    {
        std::string type = tmpV[SYSTEM_INTERFACE_TABLE_DEV_TYPE].asString();
        std::string devUuid = tmpV[SYSTEM_INTERFACE_TABLE_DEV_UUID].asString();

        info["type"] = type;

        if(SYSTEM_INTERFACE_TABLE_UDP == type)//UDP网络
        {
            //获取网络IP、PORT
            Json::Value udpJs = udpTable::getInstance()->getValue(devUuid);
            if(!udpJs.isNull())
            {
                info["ip_addr"] = udpJs[UDP_TABLE_IP_ADDR].asString();
                info["port"] = udpJs[UDP_TABLE_PORT].asInt();
                info["name"] = udpJs[UDP_TABLE_DEV_NAME].asString();
            }
            else
            {
                errInfo << "[ERROR] system_interface_table:" << uuid << "，DEV_UUID:" << devUuid
                        << ", udp_table not find ";

                THROW_EXCEPTION(errInfo.str());
            }
        }
    }
}

void cmdDecode::getDevInfo(const std::string &sys_uuid, Json::Value &info)
{
    std::ostringstream errInfo;

    //获取系统表中的DEV_TYPE、DEV_UUID字段
    Json::Value tmpV = sysInterfaceTable::getInstance()->getValue(sys_uuid);
    if(!tmpV.isNull())
    {
        std::string type = tmpV[SYSTEM_INTERFACE_TABLE_DEV_TYPE].asString();
        std::string devUuid = tmpV[SYSTEM_INTERFACE_TABLE_DEV_UUID].asString();

        info["type"] = type;

        if(SYSTEM_INTERFACE_TABLE_UDP == type)//UDP网络
        {
            //获取网络IP、PORT
            Json::Value udpJs = udpTable::getInstance()->getValue(devUuid);
            if(!udpJs.isNull())
            {
                info["ip_addr"] = udpJs[UDP_TABLE_IP_ADDR].asString();
                info["port"] = udpJs[UDP_TABLE_PORT].asInt();
                info["name"] = udpJs[UDP_TABLE_DEV_NAME].asString();
            }
            else
            {
                errInfo << "[ERROR] system_interface_table:" << sys_uuid << "，DEV_UUID:" << devUuid
                        << ", udp_table not find ";

                THROW_EXCEPTION(errInfo.str());
            }
        }
        else if(SYSTEM_INTERFACE_TABLE_TCP == type)//tcp网络
        {
            //获取网络IP、PORT
            Json::Value udpJs = tcpTable::getInstance()->getValue(devUuid);
            if(!udpJs.isNull())
            {
                info["ip_addr"] = udpJs[UDP_TABLE_IP_ADDR].asString();
                info["port"] = udpJs[UDP_TABLE_PORT].asInt();
                info["name"] = udpJs[UDP_TABLE_DEV_NAME].asString();
            }
            else
            {
                errInfo << "[ERROR] system_interface_table:" << sys_uuid << "，DEV_UUID:" << devUuid
                        << ", udp_table not find ";

                THROW_EXCEPTION(errInfo.str());
            }
        }
        else if(SYSTEM_INTERFACE_TABLE_TCP_SERVER == type)//tcp网络
        {
            //获取网络IP、PORT
            Json::Value udpJs = tcpServerTable::getInstance()->getValue(devUuid);
            if(!udpJs.isNull())
            {
                info["ip_addr"] = udpJs[UDP_TABLE_IP_ADDR].asString();
                info["port"] = udpJs[UDP_TABLE_PORT].asInt();
                info["name"] = udpJs[UDP_TABLE_DEV_NAME].asString();
            }
            else
            {
                errInfo << "[ERROR] system_interface_table:" << sys_uuid << "，DEV_UUID:" << devUuid
                        << ", udp_table not find ";

                THROW_EXCEPTION(errInfo.str());
            }
        }
        else if(SYSTEM_INTERFACE_TABLE_1553B == type)//1553B等效
        {
            //获取网络IP、PORT
            Json::Value m1553Js = m1553bTable::getInstance()->getValue(devUuid);
            if(!m1553Js.isNull())
            {
                info["card_num"] = m1553Js[M1553B_TABLE_CARD_NUM].asString();
                info["mode"] = m1553Js[M1553B_TABLE_MODE].asString();
            }
            else
            {
                errInfo << "[ERROR] system_interface_table:" << sys_uuid << "，DEV_UUID:" << devUuid
                        << ", 1553b_table not find ";

                THROW_EXCEPTION(errInfo.str());
            }
        }
        else
        {
            errInfo << "[ERROR] system_interface_table:" << sys_uuid << "，unknown type:" << type;

            THROW_EXCEPTION(errInfo.str());
        }
    }
    else
    {
        errInfo << "[ERROR] not find system_interface_table:" << sys_uuid << "，record!";

        THROW_EXCEPTION(errInfo.str());
    }
}

std::string cmdDecode::resultMsg(std::string type, const std::string &msg, std::string uuid)
{
    Json::Value objJs;
    objJs["msgType"] = type;

    Json::Value msgJs;
    msgJs["result"] = msg;

    if(uuid != "")
        msgJs["record_uuid"] = uuid;
    objJs["msg"] = msgJs;

    return objJs.toStyledString();
}
