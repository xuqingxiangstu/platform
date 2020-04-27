#include "readxml.h"
#include <QMetaType>
#include <QDebug>
#include "../property/templateproperty.h"
#include "../src/PfSql/paramsTable/paramstable.h"
#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"

readXml::readXml(QObject *parent):
    QThread(parent),
    mCurPath("")
{
    qRegisterMetaType<std::vector<std::shared_ptr<dragRole>>>("dragRoles");
}

readXml::~readXml()
{
    if(isRunning())
    {
        quit();

        wait();
    }
}

void readXml::startTask()
{
    if(!isRunning())
        start();
}

void readXml::setFilePath(QString path)
{
    mCurPath = path;
}

void readXml::run()
{
    std::ostringstream strErr;

    TiXmlDocument *mXmlDoc = new TiXmlDocument(mCurPath.toStdString().c_str());

    if(!mXmlDoc->LoadFile())
    {
        strErr << "[ERROR](xml)Open File Error" << mCurPath.toStdString() << "\n";
        emit errInfo(QString(strErr.str().c_str()));
    }
    else
    {
        TiXmlElement *mRoot = mXmlDoc->RootElement();

        initConditionValue();
        initDestDevInitValue();
        initFeInitValue();
        readFlow(mRoot);
    }

    emit readOver();
}

void readXml::readFlow(TiXmlElement *ele)
{
    TiXmlElement *mChildEle = ele->FirstChildElement("flow");

    for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement("flow"))
    {
        const char *point = nullptr;
        std::string des = "";
        std::string flowUuid = "";

        point = mChildEle->Attribute("uuid");
        if(point)
        {
            flowUuid = std::string(point);
        }

        point = mChildEle->Attribute("describe");
        if(point)
        {
            des = std::string(point);
        }

        std::shared_ptr<dragRole> flowRole = std::make_shared<dragRole>();
        flowRole->setNodeType(dragRole::Node_Flow);

        flowRole->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(flowRole->mNodeKey[dragRole::Node_Flow])));
        flowRole->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(des));
        flowRole->setUuid(flowUuid);

        emit flowItemValue(flowRole);

        readSubFlow(flowUuid, mChildEle);
    }
}

void readXml::initDestDevInitValue()
{
    //获取系统
    Json::Value sysGroup = systemTable::getInstance()->getSysGroups();

    if(sysGroup.isNull())
        return ;

    for(int index = 0; index < sysGroup.size(); index++)
    {
        std::string uuid = sysGroup[index][SYSTEM_TABLE_UUID].asString();
        std::string name = sysGroup[index][SYSTEM_TABLE_SYSTEM_NAME].asString();

        //获取系统下的接口
        Json::Value interfaceJs = sysInterfaceTable::getInstance()->getValueBySysUuid(uuid);

        for(int tmpIndex = 0; tmpIndex < interfaceJs.size(); tmpIndex++)
        {
            Json::Value tmpJs;

            tmpJs[PROPERTY_DEV_VALUE_UUID] = interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_UUID].asString();
            tmpJs[PROPERTY_DEV_VALUE_NAME] = "[" + name + "] " + interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_DEV_NAME].asString();

            mDestDevInitValue.append(tmpJs);
        }
    }
}

void readXml::updateConditionInit(dragRole *role)
{
    if(!mConditionInitValue.isNull())
    {
        role->getProperty()->setInitValue(PROPERTY_START_CONDITION, mConditionInitValue);
        role->getProperty()->setInitValue(PROPERTY_STOP_CONDITION, mConditionInitValue);
    }
}

void readXml::updateFeSysInitValue(dragRole *role)
{
    if(!mFeInitValue.isNull())
    {
        role->getProperty()->setInitValue(PROPERTY_FE_DATA_TYPE_SEND_SYS, mFeInitValue);
        role->getProperty()->setInitValue(PROPERTY_FE_DATA_TYPE_RCV_SYS, mFeInitValue);
    }
}

void readXml::updateDestDevInitValue(dragRole *role)
{
    if(!mDestDevInitValue.isNull())
    {
        role->getProperty()->setInitValue(PROPERTY_LOCAL_DEVICE, mDestDevInitValue);
        role->getProperty()->setInitValue(PROPERTY_DEST_DEVICE, mDestDevInitValue);
    }
}

void readXml::initFeInitValue()
{
    if(mFeInitValue.isNull())
    {
        //获取协议配置
        nodeProperty ptlPro(templateProperty::getInstance()->getProperty("cmd"));

        mFeInitValue = ptlPro.initValue(PROPERTY_FE_DATA_TYPE_SEND_SYS);
    }
}

void readXml::initConditionValue()
{
    Json::Value cmdJs;

    if(paramsTable::getInstance()->getCmdValues(cmdJs))
    {
        Json::Value noJs;
        noJs[PROPERTY_CONDITION_VALUE_NAME] = PROPERTY_CONDITION_NO;
        noJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = -1;
        noJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = -1;

        mConditionInitValue.append(noJs);

        for(int index = 0; index < cmdJs.size(); index++)
        {
            Json::Value tmpJs;

            tmpJs[PROPERTY_CONDITION_VALUE_NAME] = cmdJs[index][PARAM_TABLE_PARAM_NAME];
            tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = cmdJs[index][PARAM_TABLE_TABLE_NUM];
            tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = cmdJs[index][PARAM_TABLE_CODING_NUM];

            mConditionInitValue.append(tmpJs);
        }
    }
}


void readXml::readSubFlow(std::string subFlowUuid, TiXmlElement *ele)
{
    TiXmlElement *testItemEle = ele->FirstChildElement("subFlow");

    const char *point = nullptr;
    std::string des = "";

    for (testItemEle; testItemEle; testItemEle = testItemEle->NextSiblingElement("subFlow"))
    {
        std::string testItemUuid;
        point = testItemEle->Attribute("uuid");
        if(point)
        {
            testItemUuid = std::string(point);
        }

        point = testItemEle->Attribute("describe");
        if(point)
        {
            des = std::string(point);
        }

        //获取Json节点
        TiXmlElement *jsonEle = testItemEle->FirstChildElement("json");
        if(!jsonEle)
            continue;

        QString jsStr = jsonEle->GetText();
        Json::Value jsValue;
        Json::Reader jsonReader;
        if (!jsonReader.parse(jsStr.toStdString(), jsValue))
            continue;

        nodeProperty *pro = new nodeProperty(jsValue);

        std::shared_ptr<dragRole> testItemRole = std::make_shared<dragRole>();

        testItemRole->setProperty(pro);
        testItemRole->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(des));
        testItemRole->setUuid(testItemUuid);

        if(testItemRole->mNodeKey[dragRole::Node_Cmd] == pro->getKey())
        {
            testItemRole->setNodeType(dragRole::Node_Cmd);

            updateConditionInit(testItemRole.get());
            updateDestDevInitValue(testItemRole.get());
            updateFeSysInitValue(testItemRole.get());

            emit cmdItemValue(QString(subFlowUuid.c_str()), testItemRole);
        }
        else if(testItemRole->mNodeKey[dragRole::Node_Param_Group] == pro->getKey())
        {
            testItemRole->setNodeType(dragRole::Node_Param_Group);

            updateConditionInit(testItemRole.get());
            updateDestDevInitValue(testItemRole.get());
            updateFeSysInitValue(testItemRole.get());

            //获取subJson节点
            TiXmlElement *subJsonEle = testItemEle->FirstChildElement("subJson");
            if(!subJsonEle)
                continue;

            QString subJsStr = subJsonEle->GetText();
            Json::Value subJsValue;
            Json::Reader subJsonReader;
            if (!subJsonReader.parse(subJsStr.toStdString(), subJsValue))
                continue;

            std::vector<std::shared_ptr<dragRole>> subRoles;

            if(!subJsValue.isArray())
                continue;

            for(int index = 0; index < subJsValue.size(); index++)
            {
                Json::Value paramJs = subJsValue[index];

                nodeProperty *paramPro = new nodeProperty(paramJs);

                std::shared_ptr<dragRole> paramRole = std::make_shared<dragRole>();

                paramRole->setProperty(paramPro);
                paramRole->setNodeType(dragRole::Node_Param);

                subRoles.push_back(paramRole);
            }

            emit paramItemValue(QString(subFlowUuid.c_str()), testItemRole, subRoles);
        }
    }
}
