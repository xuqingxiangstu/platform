#include "createsubflow.h"
#include "../dragTree/dragrole.h"
#include <QUuid>
#include <QDebug>

//#define DEBUG_CLASS     0

/*****************config****************************/

TiXmlElement *createRoot::element()
{
    return  new TiXmlElement(CONFIG_ELEMENT);
}

/*****************equivalent****************************/

TiXmlElement *createEquivalent::element(nodeProperty *node)
{
    TiXmlElement *eqEle = new TiXmlElement(EQ_ELEMENT);

    //接口uuid
    Json::Value devJs;
    node->getProperty(PROPERTY_RECORD_DEV_SEL, devJs);
    if(!devJs.isNull())
    {
        if(!devJs[PROPERTY_DEV_VALUE_UUID].isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_SYSTEM_UUID_ELEMENT, devJs[PROPERTY_DEV_VALUE_UUID].asString()));
        }
    }

    //帧类型
    Json::Value frameTypeJs;    
    node->getProperty(PROPERTY_FRAME, frameTypeJs);
    if(!frameTypeJs.isNull())
    {        
        eqEle->LinkEndChild(createTextElement(EQ_PROTOCOL_ELEMENT, frameTypeJs.asString()));
    }
#if 0
    if(PROPERTY_FRAME_BE == type)
    {
        //系统类型
        Json::Value sysTypeJs;
        node->getProperty(PROPERTY_SRC_SYS_TYPE, sysTypeJs);
        if(!sysTypeJs.isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_SRC_SYS_TYPE_ELEMENT, sysTypeJs.asString()));
        }


        //系统编码
        Json::Value sysCodeJs;
        node->getProperty(PROPERTY_SRC_SYS_CODING, sysCodeJs);
        if(!sysCodeJs.isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_SRC_SYS_CODE_ELEMENT, sysCodeJs.asString()));
        }


        //节点编码
        Json::Value nodeCodeJs;
        node->getProperty(PROPERTY_SRC_NODE_CODING, nodeCodeJs);
        if(!nodeCodeJs.isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_SRC_SYS_NODE_CODE_ELEMENT, nodeCodeJs.asString()));
        }
    }
    else if(PROPERTY_FRAME_FE == type)
    {
        //源节点
        Json::Value sysTypeJs;
        node->getProperty(PROPERTY_SRC_SYS_TYPE, sysTypeJs);
        if(!sysTypeJs.isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_FE_HEAD_SRC_NODE, sysTypeJs.asString()));
        }
    }
#endif

    return  eqEle;
}

TiXmlElement *createEquivalent::createTextElement(const std::string &eleName, const std::string &text)
{
    TiXmlElement *tixmlgEle = new TiXmlElement(eleName.c_str());

    TiXmlText *tixmlText = new TiXmlText(text.c_str());

    tixmlgEle->LinkEndChild(tixmlText);

    return tixmlgEle;
}


/*****************createFlow****************************/


TiXmlElement *createFlow::element(nodeProperty *node)
{
    TiXmlElement *mFlowXmlEle = new TiXmlElement(FLOW_ELEMENT);

    if(!node)
        return mFlowXmlEle;

    //描述
    Json::Value desJs;
    node->getProperty(PROPERTY_DESCRIBE, desJs);
    if(!desJs.isNull())
        mFlowXmlEle->SetAttribute(FLOW_ATTR_DESCRIBE, desJs.asString().c_str());
    else
        mFlowXmlEle->SetAttribute(FLOW_ATTR_DESCRIBE, "");

    //uuid
    std::string flowUuid = QUuid::createUuid().toString().toStdString();
    mFlowXmlEle->SetAttribute(FLOW_ATTR_UUID, flowUuid.c_str());

    return mFlowXmlEle;
}

/*****************createSubFlow****************************/

TiXmlElement *createSubFlow::element(nodeProperty *headNode, nodeProperty *node, std::vector<nodeProperty*> subNode)
{
    TiXmlElement *XmlEle = new TiXmlElement(SUB_FLOW_ELEMENT);

    //存储Json subJson
    //TiXmlElement *jsonEle = new TiXmlElement(JSON_ELEMENT);
    XmlEle->LinkEndChild(createTextElement(JSON_ELEMENT, node->getJson().toStyledString()));

    if(subNode.size() > 0)
    {
        Json::Value arrayJs;
        for(nodeProperty *pro : subNode)
        {
            arrayJs.append(pro->getJson());
        }

        XmlEle->LinkEndChild(createTextElement(SUB_JSON_ELEMENT, arrayJs.toStyledString()));
    }

    //从节点中获取 时机、启动条件、停止条件、目的设备、延时等参数

    std::string text;
    TiXmlElement *tixmlgEle = nullptr;

    //描述
    Json::Value desJs;
    node->getProperty(PROPERTY_DESCRIBE, desJs);
    if(!desJs.isNull())
        XmlEle->SetAttribute(TEST_ITEM_ATTR_DESCRIBE, desJs.asString().c_str());
    else
        XmlEle->SetAttribute(TEST_ITEM_ATTR_DESCRIBE, "");

    //uuid
    std::string flowUuid = QUuid::createUuid().toString().toStdString();
    XmlEle->SetAttribute(TEST_ITEM_ATTR_UUID, flowUuid.c_str());

    //timing
    Json::Value timingJs;
    node->getProperty(PROPERTY_TIMING, timingJs);

    if(!timingJs.isNull())
        text = timingJs.asString().c_str();
    else
        text = "";

    XmlEle->LinkEndChild(createTextElement(TIMING_ELEMENT, text));

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

            XmlEle->LinkEndChild(startCondiEle);
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

            XmlEle->LinkEndChild(stopCondiEle);
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
    frameEle->LinkEndChild(createHeadElement(headNode, node, subNode));

    //frame-datafields
    frameEle->LinkEndChild(createDataFieldsElement(node, subNode));

    actionEle->LinkEndChild(frameEle);

    XmlEle->LinkEndChild(actionEle);

    //从字节点中获取 action 中的datafields信息
#ifdef DEBUG_CLASS

    TiXmlDocument *myDocument = new TiXmlDocument();

    myDocument->LinkEndChild(XmlEle);

    //myDocument->Print();
    myDocument->SaveFile("E:/test.xml");
#endif

    return XmlEle;
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

TiXmlElement *createSubFlow::createHeadElement(nodeProperty *headNode, nodeProperty *node, std::vector<nodeProperty *>subNode)
{
    TiXmlElement *headEle = new TiXmlElement(HEAD_ELEMENT);

    //帧类型
    Json::Value frameTypeJs;
    std::string type;
    headNode->getProperty(PROPERTY_FRAME, frameTypeJs);
    if(!frameTypeJs.isNull())
    {
        type = frameTypeJs.asString();
    }

    //创建类型节点

    TiXmlElement *frameEle = new TiXmlElement(type.c_str());

    if(PROPERTY_FRAME_BE == type)
    {
        //系统类型
        Json::Value sysTypeJs;
        headNode->getProperty(PROPERTY_SRC_SYS_TYPE, sysTypeJs);
        if(!sysTypeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_SYS_TYPE_ELEMENT, sysTypeJs.asString()));
        }


        //系统编码
        Json::Value sysCodeJs;
        headNode->getProperty(PROPERTY_SRC_SYS_CODING, sysCodeJs);
        if(!sysCodeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_SYS_CODE_ELEMENT, sysCodeJs.asString()));
        }


        //节点编码
        Json::Value nodeCodeJs;
        headNode->getProperty(PROPERTY_SRC_NODE_CODING, nodeCodeJs);
        if(!nodeCodeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_SYS_NODE_CODE_ELEMENT, nodeCodeJs.asString()));
        }

        //信宿信息
        Json::Value dstSysType;
        node->getProperty(PROPERTY_SRC_SYS_TYPE, dstSysType);
        if(!dstSysType.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_DST_TYPE_ELEMENT, dstSysType.asString()));
        }

        Json::Value dstSysCode;
        node->getProperty(PROPERTY_SRC_SYS_CODING, dstSysCode);
        if(!dstSysCode.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_DST_CODE_ELEMENT, dstSysCode.asString()));
        }

        Json::Value dstNodeCode;
        node->getProperty(PROPERTY_SRC_NODE_CODING, dstNodeCode);
        if(!dstNodeCode.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_DST_NODE_CODE_ELEMENT, dstNodeCode.asString()));
        }
    }
    else if(PROPERTY_FRAME_FE == type)
    {
        //源节点
        Json::Value srcTypeJs;
        headNode->getProperty(PROPERTY_SRC_SYS_TYPE, srcTypeJs);
        if(!srcTypeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_FE_HEAD_SRC_NODE, srcTypeJs.asString()));
        }

        //目标节点
        Json::Value dstTypeJs;
        headNode->getProperty(PROPERTY_SRC_SYS_TYPE, dstTypeJs);
        if(!dstTypeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_FE_HEAD_DST_NODE, dstTypeJs.asString()));
        }
    }
    else if(PROPERTY_FRAME_MIDDLE == type)
    {
        //系统类型
        Json::Value sysTypeJs;
        headNode->getProperty(PROPERTY_SRC_SYS_TYPE, sysTypeJs);
        if(!sysTypeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_SYS_TYPE_ELEMENT, sysTypeJs.asString()));
        }


        //系统编码
        Json::Value sysCodeJs;
        headNode->getProperty(PROPERTY_SRC_SYS_CODING, sysCodeJs);
        if(!sysCodeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_SYS_CODE_ELEMENT, sysCodeJs.asString()));
        }


        //节点编码
        Json::Value nodeCodeJs;
        headNode->getProperty(PROPERTY_SRC_NODE_CODING, nodeCodeJs);
        if(!nodeCodeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_SYS_NODE_CODE_ELEMENT, nodeCodeJs.asString()));
        }

        //信宿信息
        Json::Value dstSysType;
        node->getProperty(PROPERTY_SRC_SYS_TYPE, dstSysType);
        if(!dstSysType.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_DST_TYPE_ELEMENT, dstSysType.asString()));
        }

        Json::Value dstSysCode;
        node->getProperty(PROPERTY_SRC_SYS_CODING, dstSysCode);
        if(!dstSysCode.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_DST_CODE_ELEMENT, dstSysCode.asString()));
        }

        Json::Value dstNodeCode;
        node->getProperty(PROPERTY_SRC_NODE_CODING, dstNodeCode);
        if(!dstNodeCode.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_DST_NODE_CODE_ELEMENT, dstNodeCode.asString()));
        }

        //中间件增加 用户和软件标识

        //源用户及软件标识
        Json::Value srcUserJs;
        headNode->getProperty(PROPERTY_USER, srcUserJs);
        if(!srcUserJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_USER_ELEMENT, srcUserJs.asString()));
        }

        Json::Value srcVersionJs;
        headNode->getProperty(PROPERTY_SOFT_VERSION, srcVersionJs);
        if(!srcVersionJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_SRC_VERSION_ELEMENT, srcVersionJs.asString()));
        }
        //目的用户及标识
        Json::Value dstUserJs;
        node->getProperty(PROPERTY_USER, dstUserJs);
        if(!dstUserJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_DST_USER_ELEMENT, dstUserJs.asString()));
        }

        Json::Value dstVersionJs;
        node->getProperty(PROPERTY_SOFT_VERSION, dstVersionJs);
        if(!dstVersionJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_DST_VERSION_ELEMENT, dstVersionJs.asString()));
        }
    }

    headEle->LinkEndChild(frameEle);

    return headEle;
}

TiXmlElement *createSubFlow::createTextElement(const std::string &eleName, const std::string &text)
{
    TiXmlElement *tixmlgEle = new TiXmlElement(eleName.c_str());

    TiXmlText *tixmlText = new TiXmlText(text.c_str());

    tixmlgEle->LinkEndChild(tixmlText);

    return tixmlgEle;
}

