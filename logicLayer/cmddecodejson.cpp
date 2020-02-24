#include "cmddecodejson.h"
#include <sstream>
#include <QDebug>
#include "../../src/PfCommon/tools/ut_error.h"
cmdDecode::cmdDecode(PfAdapter::Adapter  *obj, QObject *parent )
    :QObject(parent),
      mCmdSendObj(obj),
      mAnalogSendObj(nullptr),
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
        mCmdSendObj->sendMsg((char*)code.c_str(), code.size());

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
    //QJsonDocument document = QJsonDocument::fromJson(QByteArray(srcJson.data(), srcJson.size()));
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(srcJson.toUtf8(), &err);
    if( (err.error == QJsonParseError::NoError) && !document.isNull())
    {
        QJsonObject jsonObject = document.object();

        ///消息类型
        QString msgType = jsonObject[MSG_TYPE].toString();

         ///类型检查
        if((msgType.compare(CMD_TYPE) == 0))
        {
            cmdMsg(jsonObject[CMD_MSG].toObject());
        }
        else if((msgType.compare(SWITCH_TYPE) == 0))
        {
            switchOutMsg(jsonObject[CMD_MSG].toArray());
        }
        else if((msgType.compare(ANALOG_TYPE) == 0))
        {
            analogOutMsg(jsonObject[CMD_MSG].toArray());
        }
        else if((msgType.compare(RS422_TYPE) == 0))
        {
            rs422OutMsg(jsonObject[CMD_MSG].toObject());
        }
        else
        {
            respond("{\"msgType\":\"error\",\"errmsg\":\"命令类型错误，类型(" + msgType.toStdString() + ")不存在\"}");
        }
    }
    else
    {
        respond("{\"msgType\":\"error\",\"errmsg\":\"json error (" + err.errorString().toStdString() + ")\"}");
    }
}
void cmdDecode::cmdMsg(QJsonObject cmdObj)
{
    QString type = cmdObj[TYPE].toString();

#ifndef QT_NO_DEBUG
    UT_SHOW(type.toStdString());
#endif

    ///程序初始化
    if(type == INIT_TEST)
    {
        initPrograme();
    }
    else if(type == START_TEST)
    {
        startTest(cmdObj["flow"].toString(), cmdObj["items"].toString());
    }
    else if(type == STOP_TEST)
    {
        stopTest(cmdObj["flow"].toString());
    }
    else if(type == SUSPEND_TEST)
    {
        suspendTest(cmdObj["flow"].toString());
    }
    else if(type == CONTINUE_TEST)
    {
        continueTest(cmdObj["flow"].toString());
    }
    else if(type == EXIT_TEST)
    {
        exitTest(cmdObj["flow"].toString());
    }
    else if(type == GET_RUNITES)
    {
        getRunItems(cmdObj["flow"].toString());
    }
    else if(type == LOAD)
    {
        load(cmdObj["flow"].toString(), cmdObj["path"].toString());
    }
}
void cmdDecode::rs422OutMsg(QJsonObject rs422Obj)
{
    std::ostringstream errCode;

    std::string devId = rs422Obj["id"].toString().toStdString();

    std::vector<unsigned char> msg;
    QJsonArray array = rs422Obj["data"].toArray();

    for(int index = 0; index <  array.size(); index++)
    {
        msg.push_back(array[index].toInt());
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
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"RS422\",\"msg\":{\"result\":\"";
    strCode << errCode.str() << "\"}}";

    respond(strCode.str());
}

void cmdDecode::switchOutMsg(QJsonArray switchArray)
{
    std::ostringstream errCode;

    for(int index = 0; index < switchArray.size(); index++)
    {
        QJsonObject obj = switchArray[index].toObject();
        QString id = obj["id"].toString();
        int channel = obj["channel"].toInt();
        bool status = obj["status"].toBool();

#ifndef QT_NO_DEBUG
        UT_SHOW(id.toStdString() + " " + std::to_string(channel) + " " + (status == true ? "open" : "close"));
#endif

        PfAdapter::Adapter *devObj = nullptr;
        if(mAdpterManagerObj->getAdapter(id.toStdString(), &devObj))
        {
           // dynamic_cast<switchOpt*>(devObj)->setValue(channel, status); fjt,目前先注释调
        }
        else
        {
            errCode << "get obj faild id = " << id.toStdString();
        }
    }

    if(errCode.str() == "")
        errCode << OK;

    ///返回结果
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"switch_out\",\"result\":\"";
    strCode << errCode.str() << "\"}}";

    respond(strCode.str());
}

void cmdDecode::analogOutMsg(QJsonArray analogArray)
{
    std::ostringstream errCode;

    for(int index = 0; index < analogArray.size(); index++)
    {
        QJsonObject obj = analogArray[index].toObject();
        QString id = obj["id"].toString();
        int channel = obj["channel"].toInt();
        double min = obj["min"].toDouble() / 1000.0;    ///界面输入为mA、实际输出为A
        double max = obj["max"].toDouble() / 1000.0;

#ifndef QT_NO_DEBUG
        UT_SHOW(id.toStdString() + " " + std::to_string(channel) + " " + std::to_string(min)+ " " + std::to_string(max));
#endif

        emit setAnalogOut(id, channel, min, max);
    }

    if(errCode.str() == "")
        errCode << OK;

    ///返回结果
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"analog_out\",\"result\":\"";
    strCode << errCode.str() << "\"}}";

    respond(strCode.str());
}

void cmdDecode::stopTest(QString flowType)
{
    std::string errCode;
    try
    {
        if(mFLows.contains(flowType))
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
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"stop\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() << "\",";
    strCode << "\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}

void cmdDecode::getRunItems(QString flowType)
{
    std::vector<std::string> items;
    std::string errCode;
    std::ostringstream strItems;
    strItems.str("");
    if(mFLows.contains(flowType))
    {
        mFLows[flowType]->getRunItems(items);

        for (int i = 0; i < items.size(); i++)
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
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"get_runitems\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() << "\",";
    strCode << "\"result\":\"";
    strCode << errCode;
    strCode << "\",";
    strCode << "\"items\":\"";
    strCode << strItems.str() << "\"}}";

    respond(strCode.str());
}


void cmdDecode::exitTest(QString flowType)
{
    std::string errCode;
    try
    {
        if(mFLows.contains(flowType))
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
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"exit\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() <<"\",";
    strCode << "\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}

void cmdDecode::continueTest(QString flowType)
{
    std::string errCode;
    try
    {
        if(mFLows.contains(flowType))
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
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"continue\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() <<"\",";
    strCode << "\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}

void cmdDecode::suspendTest(QString flowType)
{
    std::string errCode;
    try
    {
        if(mFLows.contains(flowType))
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
    std::ostringstream strCode;
    strCode.str("");
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"suspend\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() <<"\",";
    strCode << "\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}

void cmdDecode::startTest(QString flowType, QString items)
{
    std::string errCode;
    try
    {
        if(mFLows.contains(flowType))
        {
            if(!mFLows[flowType]->isRunning())
            {
                if(items.compare("all") == 0)
                    mFLows[flowType]->runAllFlow();
                else
                    mFLows[flowType]->runFlow(items.toStdString());

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
    std::ostringstream strCode;
    strCode.str("");
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"start\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() <<"\",";
    strCode << "\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}

void cmdDecode::load(QString flowType, QString path)
{
    std::string errCode;
    try
    {
        if(mFLows.contains(flowType))
        {
            if(!mFLows[flowType]->isRunning())
            {
                mFLows[flowType]->init(path.toLocal8Bit().data());
                mFLows[flowType]->setFlowType(flowType.toStdString());
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
    std::ostringstream strCode;
    strCode.str("");
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"load\",";
    strCode << "\"flow\":\"";
    strCode << flowType.toStdString() <<"\",";
    strCode << "\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}
void cmdDecode::initPrograme()
{
    std::string errCode = "ok";
    static bool isInit = false;
    std::ostringstream strErr;

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
            tmpFlow->setMsgObj(mCmdSendObj);

            mFLows[NORMAL_FLOW] = tmpFlow;

            ///初始化故障流程
            tmpFlow = new RunFlow();
            tmpFlow->init("./cfgfile/fault_flow.xml");
            tmpFlow->setFlowType(FAULT_FLOW);
            tmpFlow->setMsgObj(mCmdSendObj);

            mFLows[FAULT_FLOW] = tmpFlow;

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
    std::ostringstream strCode;
    strCode << "{\"msgType\": \"cmd\",\"msg\":{\"type\":\"init\",\"result\":\"";
    strCode << errCode << "\"}}";

    respond(strCode.str());
}
