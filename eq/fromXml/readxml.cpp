#include "readxml.h"
#include <QMetaType>
#include <QDebug>
#include <QFile>
#include <iostream>
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

    try
    {
        if(!mXmlDoc->LoadFile())
        {
            strErr << "[ERROR](xml)Open File Error" << mCurPath.toStdString() << "\n";
            emit errInfo(QString(strErr.str().c_str()));
        }
        else
        {
            TiXmlElement *mRoot = mXmlDoc->RootElement();

            initDestDevInitValue();
            initFeInitValue();

            readFlow(mRoot);
        }
    }
    catch(std::runtime_error err)
    {
        qDebug() << "[ERROR]readXml" << err.what();
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

QStringList readXml::getValueMeans(QString value)
{
    QStringList meansList;

    if(value.compare("") != 0)
    {
        meansList = value.split(";");

        //qDebug() << meansList;
    }

    return meansList;
}


nodeProperty *readXml::getNewAttr(Json::Value saveJs)
{    
    nodeProperty saveProperty(saveJs);

    nodeProperty *newProperty = new nodeProperty(templateProperty::getInstance()->getProperty(saveProperty.getKey()));

    for(auto pro : newProperty->mProperty)
    {
        if(saveProperty.isExist(pro->name()))
        {
            //modify xqx 2020-5-26 10:58:20 当类型不一致时默认值按照存储（固定值时软件启动后会更改相应类型及值）
            if(pro->name() == "fixValue")
            {
                pro->setInitValue(saveProperty.initValue(pro->name()));
            }
            //end

            pro->setType(saveProperty.type(pro->name()));

            pro->setCurValue(saveProperty.curValue(pro->name()));
            pro->setReadOnly(saveProperty.isReadOnly(pro->name()));
            pro->setVisible(saveProperty.isVisible(pro->name()));
        }
    }

    newProperty->setTableNum(saveProperty.tableNum());
    newProperty->setCodingNum(saveProperty.codingNum());

    //modify xqx 2020-5-23 16:25:34,增加基本属性中表号编码
    newProperty->setProperty(PROPERTY_BASE_TABLE_NUM, Json::Value(saveProperty.tableNum()));
    newProperty->setProperty(PROPERTY_BASE_CODING_NUM, Json::Value(saveProperty.codingNum()));
    //end

    return newProperty;
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

        nodeProperty *pro = getNewAttr(jsValue);

        std::shared_ptr<dragRole> testItemRole = std::make_shared<dragRole>();

        testItemRole->setProperty(pro);
        testItemRole->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(des));
        testItemRole->setUuid(testItemUuid);

        if(testItemRole->mNodeKey[dragRole::Node_Cmd] == pro->getKey())
        {
            testItemRole->setNodeType(dragRole::Node_Cmd);

            updateDestDevInitValue(testItemRole.get());
            updateFeSysInitValue(testItemRole.get());

            emit cmdItemValue(QString(subFlowUuid.c_str()), testItemRole);
        }
        else if(testItemRole->mNodeKey[dragRole::Node_Param_Group] == pro->getKey())
        {
            testItemRole->setNodeType(dragRole::Node_Param_Group);

            updateDestDevInitValue(testItemRole.get());
            updateFeSysInitValue(testItemRole.get());

            std::string tableString = "";

            //modify xqx 2020-5-26 13:06:58 获取表号
            if(
                    testItemEle->FirstChildElement("action") &&
                    testItemEle->FirstChildElement("action")->FirstChildElement("frame") &&
                    testItemEle->FirstChildElement("action")->FirstChildElement("frame")->FirstChildElement("datafields")
                    )
            {
                TiXmlElement *datafielsEle = testItemEle->FirstChildElement("action")->FirstChildElement("frame")->FirstChildElement("datafields");
                if(datafielsEle->FirstChildElement("cmd") && datafielsEle->FirstChildElement("cmd")->FirstChildElement("table"))
                {
                    tableString = datafielsEle->FirstChildElement("cmd")->FirstChildElement("table")->GetText();
                }
                else if(datafielsEle->FirstChildElement("param") && datafielsEle->FirstChildElement("param")->FirstChildElement("table"))
                {
                    tableString = datafielsEle->FirstChildElement("param")->FirstChildElement("table")->GetText();
                }
            }
            if(tableString != "")
            {
                pro->setTableNum(tableString);
                pro->setProperty(PROPERTY_BASE_TABLE_NUM, Json::Value(tableString));
            }
            //end


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

//                nodeProperty *paramPro = new nodeProperty(paramJs);

//                //modify xqx 2020-5-23 16:25:34,增加基本属性中表号编码
//                paramPro->setProperty(PROPERTY_BASE_TABLE_NUM, Json::Value(paramPro->tableNum()));
//                paramPro->setProperty(PROPERTY_BASE_CODING_NUM, Json::Value(paramPro->codingNum()));
//                //end

                nodeProperty *paramPro = getNewAttr(paramJs);

                std::shared_ptr<dragRole> paramRole = std::make_shared<dragRole>();

                paramRole->setProperty(paramPro);
                paramRole->setNodeType(dragRole::Node_Param);

                subRoles.push_back(paramRole);
            }

            emit paramItemValue(QString(subFlowUuid.c_str()), testItemRole, subRoles);
        }
    }
}
