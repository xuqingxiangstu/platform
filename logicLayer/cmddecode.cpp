#include "cmddecode.h"
#include <sstream>
#include <QDebug>
#include <string>
#include <vector>
#include "../../src/PfCommon/tools/ut_error.h"
#include "../src/PfCommon/cmdToJson/cmdtojson.h"
cmdDecode::cmdDecode(PfAdapter::Adapter  *obj, QObject *parent )
    :QObject(parent),
      mCmdSendObj(obj),
      //mAnalogSendObj(nullptr),
      mAdpterManagerObj(nullptr)
{
    mAdpterManagerObj = new PfAdapter::PfAdapterManager();
}
cmdDecode::~cmdDecode()
{
    ///设备对象析构
    delete mAdpterManagerObj;

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
    qDebug() << srcJson;
#endif
    Json::Value root;
    Json::Reader reader;
    if(reader.parse(srcJson.toStdString(),root))
    {
        std::string msgType = root[MSG_TYPE].asString();
        ///类型检查
       if((msgType.compare(CMD_TYPE) == 0))
       {
           cmdMsg(root[CMD_MSG]);
       }
       else if((msgType.compare(SWITCH_TYPE) == 0))
       {
           switchOutMsg(root[CMD_MSG]);
       }
       else if((msgType.compare(ANALOG_TYPE) == 0))
       {
           analogOutMsg(root[CMD_MSG]);
       }
       else if((msgType.compare(RS422_TYPE) == 0))
       {
           rs422OutMsg(root[CMD_MSG]);
       }
       else
       {
           std::string strErrInfo= "命令类型错误，类型(" + msgType + " )不存在";
           std::string strTemp = PfCommon::cmdToJson::replyErrorCmd((const std::string&)strErrInfo);
           respond(strTemp);
       }
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
    std::string type = jsValue[TYPE].asString();

#ifndef QT_NO_DEBUG
    UT_SHOW(type);
#endif

    ///程序初始化
    if(type == INIT_TEST)
    {
        initPrograme();
    }
    else if(type == START_TEST)
    {
        startTest(jsValue["flow"].asString(), jsValue["items"].asString());
    }
    else if(type == STOP_TEST)
    {
        stopTest(jsValue["flow"].asString());
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
    else if(type == GET_RUNITES)
    {
        getRunItems(jsValue["flow"].asString());
    }
    else if(type == LOAD)
    {
        load(jsValue["flow"].asString(), jsValue["path"].asString());
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

void cmdDecode::stopTest(std::string flowType)
{
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
    vecMsg.push_back("stop");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
}

void cmdDecode::getRunItems(std::string flowType)
{
    std::vector<std::string> items;
    std::string errCode;
    std::ostringstream strItems;
    strItems.str("");
    std::map<std::string, RunFlow*>::iterator it;
    it = mFLows.find(flowType);
    if(it!=mFLows.end())
    {
        mFLows[flowType]->getRunItems(items);

        for (unsigned int i = 0; i < items.size(); i++)
        {
            strItems << items.at(i) << ";";
        }
        errCode = OK;
    }
    else
    {
        errCode = NO_FLOW;
    }

    ///返回结果
    std::vector<std::string> vecMsg;
    vecMsg.clear();
    vecMsg.push_back("get_runitems");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    vecMsg.push_back(strItems.str());
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
}


void cmdDecode::exitTest(std::string flowType)
{
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
}

void cmdDecode::continueTest(std::string flowType)
{
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
}

void cmdDecode::suspendTest(std::string flowType)
{
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
}

void cmdDecode::startTest(std::string flowType, std::string items)
{
    std::string errCode;
    try
    {
        std::map<std::string, RunFlow*>::iterator it;
        it = mFLows.find(flowType);
        if(it!=mFLows.end())
        {
            if(!mFLows[flowType]->isRunning())
            {
                if(items.compare("all") == 0)
                    mFLows[flowType]->runAllFlow();
                else
                    mFLows[flowType]->runFlow(items);

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
    vecMsg.push_back("start");
    vecMsg.push_back(flowType);
    vecMsg.push_back(errCode);
    std::string strTemp=PfCommon::cmdToJson::replyCtrlCmd(vecMsg);
    respond(strTemp);
}

void cmdDecode::load(std::string flowType, std::string path)
{
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
}
void cmdDecode::initPrograme()
{
    std::string errCode = "ok";
    static bool isInit = false;

    if(!isInit)
    {
        try
        {
            ///初始化仿真模型
            ///simulationModel::getInstance()->init("./cfgfile/simulationModel.xml");

            ///初始化硬件设备配置
            mAdpterManagerObj->init("./cfgfile/logic_devcfg.xml");

            RunFlow *tmpFlow = nullptr;

            ///初始化正常流程
            tmpFlow = new RunFlow();
            tmpFlow->init("./cfgfile/flow.xml");
            tmpFlow->setFlowType(NORMAL_FLOW);
           //fjt
            tmpFlow->setMsgObj(mCmdSendObj);

            mFLows[NORMAL_FLOW] = tmpFlow;

            ///初始化故障流程
            //tmpFlow = new RunFlow();
            //tmpFlow->init("./cfgfile/fault_flow.xml");
            //tmpFlow->setFlowType(FAULT_FLOW);
           //fjt tmpFlow->setMsgObj(mCmdSendObj);

            //mFLows[FAULT_FLOW] = tmpFlow;

#if 0
            ///获取开关量/模拟量输入通信句柄
            if (!equipment::getInstance()->getDevObj(QUERY_ID, &mAnalogSendObj))
            {
                strErr.str("");
                strErr << "获取设备句柄失败 id=" << QUERY_ID;
                UT_THROW_EXCEPTION(strErr.str());
            }

            ///启动开关量输入采集任务
            mSwitchQueryTask = new switchQueryTask();
            mSwitchQueryTask->setSendId(QUERY_ID);

            std::vector<std::string> queryIds;
            //queryIds.push_back("ff");

            mSwitchQueryTask->setQueryIds(queryIds);

            mSwitchQueryTask->startTask();


            ///启动模拟量更新任务向UI更新数据
            mUpdateAnalogTask = new updateAnalogTask();
            mUpdateAnalogTask->setUpdateObj(mAnalogSendObj);

            ///启动模拟量采集任务

            std::vector<std::string> analogInIds;
            ///TODO:设置模拟量采集ID
            analogInIds.push_back("PXI6375_1");
            analogInIds.push_back("PXI6375_2");
            analogInIds.push_back("PXI6375_3");
            analogInIds.push_back("PXI6375_4");

            for(auto id : analogInIds)
            {
                analogInTask *analogTask = nullptr;

                analogTask = new analogInTask();
                ///connect(analogTask, &analogInTask::upDate, this, &cmdDecode::upDateAnalogInData);
                connect(analogTask, &analogInTask::upDate, mUpdateAnalogTask, &updateAnalogTask::onUpdate);
                analogTask->setDevId(id);
                analogTask->startTask();
                mAnalogInTasks.push_back(analogTask);
            }

            mUpdateAnalogTask->startTask();

            ///启动模拟量输出任务
            std::vector<std::vector<std::string>> analogOutIds;
            ///TODO:设置输出ID,二维，线程达到最大值时分多个线程，需测试20191021

            std::vector<std::string> id;
            id.push_back("PXI6704_1");
            id.push_back("PXI6704_2");
            id.push_back("PXI6704_3");
            id.push_back("PXI6704_4");
            id.push_back("PXI6704_5");
            id.push_back("PXI6704_6");
            id.push_back("PXI6704_7");

            std::vector<std::string> id1;
            id1.push_back("PXI6704_8");
            id1.push_back("PXI6704_9");
            id1.push_back("PXI6704_10");
            id1.push_back("PXI6704_11");
            id1.push_back("PXI6704_12");
            id1.push_back("PXI6704_13");
            id1.push_back("PXI6704_14");

            std::vector<std::string> id2;
            id2.push_back("PXI6704_15");
            id2.push_back("PXI6704_16");
            id2.push_back("PXI6704_17");
            id2.push_back("PXI6704_18");
            id2.push_back("PXI6704_19");
            id2.push_back("PXI6704_20");
            id2.push_back("PXI6704_21");

            analogOutIds.push_back(id);
            analogOutIds.push_back(id1);
            analogOutIds.push_back(id2);

            for(auto ids : analogOutIds)
            {
                analogOutTask *task = new analogOutTask();
                QObject::connect(this, &cmdDecode::setAnalogOut, task, &analogOutTask::setOutData);
                task->setCardIds(ids);
                task->startTask();
                mAnalogOutTasks.push_back(task);
            }
#endif
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
