#include "readxml.h"
#include <QMetaType>
#include <QDebug>
#include "../property/templateproperty.h"

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
        //testItemRole->setTableAndCoding(testItemRole->getProperty()->getProperty(PROPERTY_TIMING););

        if(testItemRole->mNodeKey[dragRole::Node_Cmd] == pro->getKey())
        {
            testItemRole->setNodeType(dragRole::Node_Cmd);

            emit cmdItemValue(QString(subFlowUuid.c_str()), testItemRole);
        }
        else if(testItemRole->mNodeKey[dragRole::Node_Param_Group] == pro->getKey())
        {
            testItemRole->setNodeType(dragRole::Node_Param_Group);


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
