#include "createsubflow.h"

#include "../dragTree/dragrole.h"
#include <QUuid>
#include <QDebug>

#define DEBUG_CLASS     0

createSubFlow::createSubFlow()
{
    mSubFlowXmlEle = new TiXmlElement(FLOW_ELEMENT);
}

void createSubFlow::setProperty(nodeProperty *node, std::vector<nodeProperty *> subNode)
{
    //从节点中获取 时机、启动条件、停止条件、目的设备、延时等参数

    std::string text;
    TiXmlElement *tixmlgEle = nullptr;

    //描述
    Json::Value desJs;
    node->getProperty(PROPERTY_DESCRIBE, desJs);
    if(!desJs.isNull())
        mSubFlowXmlEle->SetAttribute(FLOW_ATTR_DESCRIBE, desJs.asString().c_str());
    else
        mSubFlowXmlEle->SetAttribute(FLOW_ATTR_DESCRIBE, "");

    //uuid
    std::string flowUuid = QUuid::createUuid().toString().toStdString();
    mSubFlowXmlEle->SetAttribute(FLOW_ATTR_UUID, flowUuid.c_str());

    //timing
    Json::Value timingJs;
    node->getProperty(PROPERTY_TIMING, timingJs);

    if(!timingJs.isNull())
        text = timingJs.asString().c_str();
    else
        text = "";

    mSubFlowXmlEle->LinkEndChild(createTextElement(TIMING_ELEMENT, text));

    //startCondition

    Json::Value startCondJs;
    node->getProperty(PROPERTY_START_CONDITION, startCondJs);

    if(!startCondJs.isNull())
    {
        std::string conName = startCondJs[PROPERTY_CONDITION_VALUE_NAME].asString();
        if(PROPERTY_CONDITION_NO != conName)
        {
            TiXmlElement *startCondiEle = new TiXmlElement(START_CONDITION_ELEMENT);

            startCondiEle->LinkEndChild(createTextElement(TABLE_ELEMENT, startCondJs[PROPERTY_CONDITION_VALUE_TABLE_NUM].asString()));
            startCondiEle->LinkEndChild(createTextElement(CODING_ELEMENT, startCondJs[PROPERTY_CONDITION_VALUE_CODING_NUM].asString()));

            mSubFlowXmlEle->LinkEndChild(startCondiEle);
        }
    }

    //stopCondition

    Json::Value stopCondJs;
    node->getProperty(PROPERTY_STOP_CONDITION, stopCondJs);

    if(!stopCondJs.isNull())
    {
        std::string conName = stopCondJs[PROPERTY_CONDITION_VALUE_NAME].asString();
        if(PROPERTY_CONDITION_NO != conName)
        {
            TiXmlElement *stopCondiEle = new TiXmlElement(STOP_CONDITION_ELEMENT);

            stopCondiEle->LinkEndChild(createTextElement(TABLE_ELEMENT, stopCondJs[PROPERTY_CONDITION_VALUE_TABLE_NUM].asString()));
            stopCondiEle->LinkEndChild(createTextElement(CODING_ELEMENT, stopCondJs[PROPERTY_CONDITION_VALUE_CODING_NUM].asString()));

            mSubFlowXmlEle->LinkEndChild(stopCondiEle);
        }
    }

    //action
    TiXmlElement *actionEle = new TiXmlElement(ACTION_ELEMENT);

    //dest_system
    Json::Value destSysJs;
    node->getProperty(PROPERTY_DESTDEVICE, destSysJs);

    if(!destSysJs.isNull())
        text = destSysJs[PROPERTY_DEV_VALUE_UUID].asString().c_str();
    else
        text = "";
    actionEle->LinkEndChild(createTextElement(DEST_SYSTEM_ELEMENT, text));

    //sleep
    Json::Value sleepJs;
    node->getProperty(PROPERTY_DELAY, sleepJs);
    if(!sleepJs.isNull())
        text = sleepJs.asString().c_str();
    else
        text = "";
    actionEle->LinkEndChild(createTextElement(SLEEP_ELEMENT, text));

    //frame
    TiXmlElement *frameEle = new TiXmlElement(FRAME_ELEMENT);

    //frame-head
    frameEle->LinkEndChild(createHeadElement(node, subNode));

    //frame-datafields
    frameEle->LinkEndChild(createDataFieldsElement(node, subNode));

    actionEle->LinkEndChild(frameEle);

    mSubFlowXmlEle->LinkEndChild(actionEle);

    //从字节点中获取 action 中的datafields信息
#ifdef DEBUG_CLASS

    TiXmlDocument *myDocument = new TiXmlDocument();

    myDocument->LinkEndChild(mSubFlowXmlEle);

    //myDocument->Print();
    myDocument->SaveFile("E:/test.xml");
#endif
}

TiXmlElement *createSubFlow::createDataFieldsElement(nodeProperty *node, std::vector<nodeProperty *>subNodes)
{
    TiXmlElement *dataFileldEle = new TiXmlElement(DATAFIELDS_ELEMENT);

    dragRole role;
    //判断指令还是参数
    if(role.mNodeKey[dragRole::Node_Cmd] == node->getKey())
    {
        TiXmlElement *cmdEle = nullptr;

        //指令
        cmdEle = new TiXmlElement(CMD_ELEMENT);

        cmdEle->LinkEndChild(createTextElement(TABLE_ELEMENT, std::to_string(node->tableNum())));

        cmdEle->LinkEndChild(createTextElement(CODING_ELEMENT,std::to_string(node->codingNum())));

        dataFileldEle->LinkEndChild(cmdEle);
    }
    else if(role.mNodeKey[dragRole::Node_Param_Group] == node->getKey())
    {
        //参数

        //遍历各子节点信息
        for(nodeProperty *subNode : subNodes)
        {
            TiXmlElement *paramEle = new TiXmlElement(PARAM_ELEMENT);

            paramEle->LinkEndChild(createTextElement(TABLE_ELEMENT, std::to_string(subNode->tableNum())));

            paramEle->LinkEndChild(createTextElement(CODING_ELEMENT,std::to_string(subNode->codingNum())));

            //获取仿真模型
            Json::Value simJs;
            subNode->getProperty(PROPERTY_SIM_MODEL, simJs);

            TiXmlElement *valueEle = new TiXmlElement(VALUE_ELEMENT);

            if(PROPERTY_MODEL_FIX == simJs.asString())
            {
                Json::Value tmpJs;
                subNode->getProperty(PROPERTY_FIX_VALUE, tmpJs);

                if(!tmpJs.isNull())
                    valueEle->LinkEndChild(createTextElement(INIT_ELEMENT, tmpJs.asString().c_str()));
                else
                    valueEle->LinkEndChild(createTextElement(INIT_ELEMENT, ""));

            }
            else if(PROPERTY_MODEL_LINE == simJs.asString())
            {
                TiXmlElement *lineEle = new TiXmlElement(LINE_ELEMENT);

                Json::Value minJs;
                subNode->getProperty(PROPERTY_LINE_MIN, minJs);

                if(!minJs.isNull())
                    lineEle->LinkEndChild(createTextElement(MIN_ELEMENT, minJs.asString().c_str()));
                else
                    lineEle->LinkEndChild(createTextElement(MIN_ELEMENT, ""));

                Json::Value maxJs;
                subNode->getProperty(PROPERTY_LINE_MAX, maxJs);

                if(!maxJs.isNull())
                    lineEle->LinkEndChild(createTextElement(MAX_ELEMENT, maxJs.asString().c_str()));
                else
                    lineEle->LinkEndChild(createTextElement(MAX_ELEMENT, ""));

                Json::Value stepJs;
                subNode->getProperty(PROPERTY_LINE_STEP, stepJs);

                if(!stepJs.isNull())
                    lineEle->LinkEndChild(createTextElement(STEP_ELEMENT, stepJs.asString().c_str()));
                else
                    lineEle->LinkEndChild(createTextElement(STEP_ELEMENT, ""));

                valueEle->LinkEndChild(lineEle);
            }
            else if(PROPERTY_MODEL_RAND == simJs.asString())
            {
                TiXmlElement *randEle = new TiXmlElement(RAND_ELEMENT);

                Json::Value minJs;
                subNode->getProperty(PROPERTY_RAND_MIN, minJs);

                if(!minJs.isNull())
                    randEle->LinkEndChild(createTextElement(MIN_ELEMENT, minJs.asString().c_str()));
                else
                    randEle->LinkEndChild(createTextElement(MIN_ELEMENT, ""));

                Json::Value maxJs;
                subNode->getProperty(PROPERTY_RAND_MAX, maxJs);

                if(!maxJs.isNull())
                    randEle->LinkEndChild(createTextElement(MAX_ELEMENT, maxJs.asString().c_str()));
                else
                    randEle->LinkEndChild(createTextElement(MAX_ELEMENT, ""));


                valueEle->LinkEndChild(randEle);
            }

            paramEle->LinkEndChild(valueEle);

            dataFileldEle->LinkEndChild(paramEle);
        }
    }



    return dataFileldEle;
}

TiXmlElement *createSubFlow::createHeadElement(nodeProperty *node, std::vector<nodeProperty *>subNode)
{
    TiXmlElement *headEle = new TiXmlElement(HEAD_ELEMENT);
    return headEle;
}

TiXmlElement *createSubFlow::createTextElement(const std::string &eleName, const std::string &text)
{
    TiXmlElement *tixmlgEle = new TiXmlElement(eleName.c_str());

    TiXmlText *tixmlText = new TiXmlText(text.c_str());

    tixmlgEle->LinkEndChild(tixmlText);

    return tixmlgEle;
}

TiXmlElement *createSubFlow::toXml()
{

    return nullptr;
}
