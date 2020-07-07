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
    node->getProperty(PROPERTY_RECORD_LOCAL_DEVICE, devJs);
    if(!devJs.isNull())
    {
        if(!devJs[PROPERTY_DEV_VALUE_UUID].isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_SYSTEM_UUID_ELEMENT, devJs[PROPERTY_DEV_VALUE_UUID].asString()));
        }
    }

    Json::Value dstdevJs;
    node->getProperty(PROPERTY_RECORD_DEST_DEVICE, dstdevJs);
    if(!dstdevJs.isNull())
    {
        if(!dstdevJs[PROPERTY_DEV_VALUE_UUID].isNull())
        {
            eqEle->LinkEndChild(createTextElement(EQ_DST_SYSTEM_UUID_ELEMENT, dstdevJs[PROPERTY_DEV_VALUE_UUID].asString()));
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


    XmlEle->LinkEndChild(createTextElement(JSON_ELEMENT, node->getSaveJson().toStyledString()));

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

            //接口uuid
            Json::Value devJs;
            headNode->getProperty(PROPERTY_RECORD_LOCAL_DEVICE, devJs);
            if(!devJs.isNull())
            {
                if(!devJs[PROPERTY_DEV_VALUE_UUID].isNull())
                {
                    startCondiEle->LinkEndChild(createTextElement(SYSTEM_UUID_ELEMENT, devJs[PROPERTY_DEV_VALUE_UUID].asString()));
                }
            }

            startCondiEle->LinkEndChild(createTextElement(TABLE_ELEMENT, startCondJs[PROPERTY_CONDITION_VALUE_TABLE_NUM].asString()));
            startCondiEle->LinkEndChild(createTextElement(CODING_ELEMENT, startCondJs[PROPERTY_CONDITION_VALUE_CODING_NUM].asString()));

            TiXmlElement *valueEle = new TiXmlElement(PROPERTY_CONDITION_VALUE_VALUE);

            if(startCondJs[PROPERTY_CONDITION_VALUE_VALUE].isDouble())
            {
                valueEle->LinkEndChild(createTextElement(VALUE_TYPE_ELEMENT, "double"));
            }
            else if(startCondJs[PROPERTY_CONDITION_VALUE_VALUE].isString())
            {
                valueEle->LinkEndChild(createTextElement(VALUE_TYPE_ELEMENT, "string"));
            }
            else
            {
                valueEle->LinkEndChild(createTextElement(VALUE_TYPE_ELEMENT, "int"));
            }

            valueEle->LinkEndChild(createTextElement(VALUE_VALUE_CONTEXT_ELEMENT, startCondJs[PROPERTY_CONDITION_VALUE_VALUE].asString()));

            startCondiEle->LinkEndChild(valueEle);

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

            //接口uuid
            Json::Value devJs;
            headNode->getProperty(PROPERTY_RECORD_LOCAL_DEVICE, devJs);
            if(!devJs.isNull())
            {
                if(!devJs[PROPERTY_DEV_VALUE_UUID].isNull())
                {
                    stopCondiEle->LinkEndChild(createTextElement(SYSTEM_UUID_ELEMENT, devJs[PROPERTY_DEV_VALUE_UUID].asString()));
                }
            }

            stopCondiEle->LinkEndChild(createTextElement(TABLE_ELEMENT, stopCondJs[PROPERTY_CONDITION_VALUE_TABLE_NUM].asString()));
            stopCondiEle->LinkEndChild(createTextElement(CODING_ELEMENT, stopCondJs[PROPERTY_CONDITION_VALUE_CODING_NUM].asString()));

            TiXmlElement *valueEle = new TiXmlElement(PROPERTY_CONDITION_VALUE_VALUE);

            if(startCondJs[PROPERTY_CONDITION_VALUE_VALUE].isDouble())
            {
                valueEle->LinkEndChild(createTextElement(VALUE_TYPE_ELEMENT, "double"));
            }
            else if(startCondJs[PROPERTY_CONDITION_VALUE_VALUE].isString())
            {
                valueEle->LinkEndChild(createTextElement(VALUE_TYPE_ELEMENT, "string"));
            }
            else
            {
                valueEle->LinkEndChild(createTextElement(VALUE_TYPE_ELEMENT, "int"));
            }

            valueEle->LinkEndChild(createTextElement(VALUE_VALUE_CONTEXT_ELEMENT, stopCondJs[PROPERTY_CONDITION_VALUE_VALUE].asString()));

            stopCondiEle->LinkEndChild(valueEle);

            XmlEle->LinkEndChild(stopCondiEle);
        }
    }

    //action
    TiXmlElement *actionEle = new TiXmlElement(ACTION_ELEMENT);

    //dest_system

    Json::Value destSysJs;
    headNode->getProperty(PROPERTY_LOCAL_DEVICE, destSysJs);

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

        cmdEle->LinkEndChild(createTextElement(TABLE_ELEMENT, node->tableNum()));

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

            paramEle->LinkEndChild(createTextElement(TABLE_ELEMENT, subNode->tableNum()));

            paramEle->LinkEndChild(createTextElement(CODING_ELEMENT,std::to_string(subNode->codingNum())));

            //获取仿真模型
            Json::Value simJs;
            subNode->getProperty(PROPERTY_SIM_MODEL, simJs);

            TiXmlElement *valueEle = new TiXmlElement(VALUE_ELEMENT);

            if(PROPERTY_MODEL_FIX == simJs.asString())
            {
                Json::Value tmpJs;
                subNode->getProperty(PROPERTY_FIX_VALUE, tmpJs);

                //modify xqx 20200419 固定值时用户选择有含义，需去除含义
                QString tmp = tmpJs.asString().c_str();

                int pos = tmp.indexOf(":");
                if(-1 != pos)
                {
                    tmp = tmp.mid(0, pos);
                }

                if(!tmpJs.isNull())
                    valueEle->LinkEndChild(createTextElement(INIT_ELEMENT, tmp.toStdString().c_str()));
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

        //modify xqx 2020-5-19 11:13:12  车辆类型 车辆编号合成为弹编号

        Json::Value clTypeJs, clNumJs;
        node->getProperty(PROPERTY_OTHER_CL_TYPE, clTypeJs);
        node->getProperty(PROPERTY_OTHER_CL_NUM, clNumJs);
        if(!clTypeJs.isNull() && !clNumJs.isNull())
        {
            std::string tmp = clTypeJs.asString();
            tmp = clNumJs.asString();
            unsigned char type = (unsigned char)clTypeJs.asInt();
            unsigned char num = (unsigned char)clNumJs.asInt();

            int dNum = type + (num << 8);

            frameEle->LinkEndChild(createTextElement(EQ_D_NUM_ELEMENT, std::to_string(dNum)));
        }

        //end

        //数据
        Json::Value dataJs;
        node->getProperty(PROPERTY_OTHER_DATA, dataJs);
        if(!dataJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_DATA_ELEMENT, dataJs.asString()));
        }
    }
    else if( (PROPERTY_FRAME_FE == type) || (PROPERTY_FRAME_JG == type) || (PROPERTY_FRAME_CZXK == type))
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
        node->getProperty(PROPERTY_SRC_SYS_TYPE, dstTypeJs);
        if(!dstTypeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_FE_HEAD_DST_NODE, dstTypeJs.asString()));
        }

        //发起系统
        Json::Value sendSysJs;
        node->getProperty(PROPERTY_FE_DATA_TYPE_SEND_SYS, sendSysJs);
        if(!sendSysJs.isNull())
        {
            QString tmp = sendSysJs.asString().c_str();
            int index = tmp.indexOf(":");
            if(index != -1)
                tmp = tmp.left(index);

            frameEle->LinkEndChild(createTextElement(EQ_FE_HEAD_SEND_SYS_NODE, tmp.toStdString().c_str()));
        }

        //接收系统
        Json::Value rcvSysJs;
        node->getProperty(PROPERTY_FE_DATA_TYPE_RCV_SYS, rcvSysJs);
        if(!rcvSysJs.isNull())
        {
            QString tmp = rcvSysJs.asString().c_str();
            int index = tmp.indexOf(":");
            if(index != -1)
                tmp = tmp.left(index);
            frameEle->LinkEndChild(createTextElement(EQ_FE_HEAD_RCV_SYS_NODE, tmp.toStdString().c_str()));
        }
    }
    else if(PROPERTY_FRAME_1553B == type)
    {
        //总线
        Json::Value busJs;
        node->getProperty(PROPERTY_1553B_BUS, busJs);
        if(!busJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(BUS_ELEMENT, busJs.asString()));
        }

        //模式
        Json::Value modelJs;
        node->getProperty(PROPERTY_1553B_MODE, modelJs);
        if(!modelJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(MODEL_ELEMENT, modelJs.asString()));

            std::string model = modelJs.asString();
            if(PROPERTY_1553B_MODE_BC_RT == model)
            {
                //RT地址
                Json::Value rtJs;
                node->getProperty(PROPERTY_1553B_BC_RT_RT_ADDR, rtJs);
                if(!rtJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(RT_ELEMENT, rtJs.asString()));
                }

                //SA地址
                Json::Value saJs;
                node->getProperty(PROPERTY_1553B_BC_RT_SA_ADDR, saJs);
                if(!saJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(SA_ELEMENT, saJs.asString()));
                }
            }
            else if(PROPERTY_1553B_MODE_RT_RT == model)
            {
                //sRT地址
                Json::Value sRtJs;
                node->getProperty(PROPERTY_1553B_RT_RT_S_RT_ADDR, sRtJs);
                if(!sRtJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(S_RT_ELEMENT, sRtJs.asString()));
                }

                //sSA地址
                Json::Value sSaJs;
                node->getProperty(PROPERTY_1553B_RT_RT_S_SA_ADDR, sSaJs);
                if(!sSaJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(S_SA_ELEMENT, sSaJs.asString()));
                }

                //rRT地址
                Json::Value rRtJs;
                node->getProperty(PROPERTY_1553B_RT_RT_R_RT_ADDR, rRtJs);
                if(!rRtJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(R_RT_ELEMENT, rRtJs.asString()));
                }

                //rSA地址
                Json::Value rSaJs;
                node->getProperty(PROPERTY_1553B_RT_RT_R_SA_ADDR, rSaJs);
                if(!rSaJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(R_SA_ELEMENT, rSaJs.asString()));
                }

                //长度
                Json::Value sizeJs;
                node->getProperty(PROPERTY_1553B_RT_RT_DATA_SIZE, sizeJs);
                if(!sizeJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(DATA_SIZE_ELEMENT, sizeJs.asString()));
                }
            }
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

        //帧类型
        Json::Value frameTypeJs;
        headNode->getProperty(PROPERTY_FRAME, frameTypeJs);
        if(!frameTypeJs.isNull())
        {
            if(PROPERTY_FRAME_MIDDLE == frameTypeJs.asString())
            {
                Json::Value askJs;
                headNode->getProperty(PROPERTY_ASK_FLAG, askJs);
                if(!askJs.isNull())
                {
                    frameEle->LinkEndChild(createTextElement(EQ_ASK_FLAG_ELEMENT, askJs.asString()));
                }
            }
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

        //信息字类型
        Json::Value infoWordTypeJs;
        node->getProperty(PROPERTY_BASE_INFO_WORD, infoWordTypeJs);

        if(!infoWordTypeJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_INFO_WORD_TYPE_ELEMENT, infoWordTypeJs.asString()));
        }

        //弹编号
        Json::Value dNumJs;
        node->getProperty(PROPERTY_OTHER_D_NUM, dNumJs);
        if(!dNumJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_D_NUM_ELEMENT, dNumJs.asString()));
        }

        //设备编号
        Json::Value devNumJs;
        node->getProperty(PROPERTY_OTHER_DEV_INDEX, devNumJs);
        if(!devNumJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_DEV_NUM_ELEMENT, devNumJs.asString()));
        }

        //模块编号
        Json::Value modelNumJs;
        node->getProperty(PROPERTY_OTHER_MODLE_INDEX, modelNumJs);
        if(!modelNumJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_MODLE_NUM_ELEMENT, modelNumJs.asString()));
        }

        //预留字符串
        Json::Value reserveJs;
        node->getProperty(PROPERTY_OTHER_RESERVE, reserveJs);
        if(!reserveJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_RESERVE_ELEMENT, reserveJs.asString()));
        }
        else
        {
            frameEle->LinkEndChild(createTextElement(EQ_RESERVE_ELEMENT, ""));
        }

        //数据
        Json::Value dataJs;
        node->getProperty(PROPERTY_OTHER_DATA, dataJs);
        if(!dataJs.isNull())
        {
            frameEle->LinkEndChild(createTextElement(EQ_DATA_ELEMENT, dataJs.asString()));
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

