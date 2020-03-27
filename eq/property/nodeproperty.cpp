#include "nodeproperty.h"
#include <algorithm>
#include <QDebug>

nodeProperty::nodeProperty()
{

}

nodeProperty::nodeProperty(const Json::Value &v)
{
    mDefalultProperty = v;

    std::string key = v["key"].asString();
    Json::Value propertyJs = v["property"];

    for(int index = 0; index < propertyJs.size(); index++)
    {
        Json::Value nodeJs = propertyJs[index]["node"];
        for(int nodeIndex = 0; nodeIndex < nodeJs.size(); nodeIndex++)
        {
            Json::Value proJs = nodeJs[nodeIndex];

            std::shared_ptr<property> obj = std::make_shared<property>();

            obj->setType(proJs["type"].asString());
            obj->setName(proJs["attr"].asString());
            obj->setReadOnly(proJs["readOnly"].asBool());
            obj->setInitValue(proJs["initValue"]);
            if(proJs["curValue"].isNull())
            {
                obj->setCurValue(proJs["initValue"]);
            }
            else
            {
                obj->setCurValue(proJs["curValue"]);
            }

            mProperty.emplace_back(obj);
        }
    }
}

void nodeProperty::setInitValue(const std::string &name, const Json::Value &initValue)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        (*itor)->setInitValue(initValue);
    }
}

void nodeProperty::setType(const std::string &name, const std::string &type)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        (*itor)->setType(type);
    }
}

std::string nodeProperty::type(const std::string &name)
{
    std::string tmp = "";
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        tmp = (*itor)->type();
    }

    return tmp;
}

void nodeProperty::setCurValue(const std::string &name, const Json::Value &curValue)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        (*itor)->setCurValue(curValue);
    }
}

void nodeProperty::setReadOnly(const std::string &name, bool isRead)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        (*itor)->setReadOnly(isRead);
    }
}

bool nodeProperty::isReadOnly(const std::string &name)
{
    bool res = false;
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        res = (*itor)->isReadOnly();
    }

    return res;
}

Json::Value nodeProperty::curValue(const std::string &name)
{
    Json::Value tmpV;

    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        tmpV = (*itor)->curValue();
    }
    return tmpV;
}

void nodeProperty::setProperty(const std::string &name, const Json::Value &value)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        (*itor)->setCurValue(value);
    }
}

std::string nodeProperty::getKey()
{
    std::string key = "";

    Json::Value keyJs = mDefalultProperty["key"];
    if(!keyJs.isNull())
        key = keyJs.asString();

    return key;
}

void nodeProperty::getProperty(const std::string &name, Json::Value &value)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        value = (*itor)->curValue();
    }
}

std::shared_ptr<nodeProperty> nodeProperty::clone()
{
    nodeProperty *tmp = new nodeProperty();

    tmp->mDefalultProperty = mDefalultProperty;

    for(auto pro : mProperty)
    {
        tmp->mProperty.push_back(pro->clone());
    }

    tmp->mTableNum = mTableNum;
    tmp->mCodingNum = mCodingNum;

    return std::shared_ptr<nodeProperty>(tmp);
}

Json::Value nodeProperty::getJson()
{
    //只更改默认属性中的curValue值即可

    //qDebug() << mDefalultProperty.toStyledString().c_str();

    std::string key = mDefalultProperty["key"].asString();
    Json::Value propertyJs = mDefalultProperty["property"];

    for(int index = 0; index < propertyJs.size(); index++)
    {
        Json::Value nodeJs = propertyJs[index]["node"];
        for(int nodeIndex = 0; nodeIndex < nodeJs.size(); nodeIndex++)
        {
            Json::Value proJs = nodeJs[nodeIndex];

            std::string name = proJs["attr"].asString();

            auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
                {
                    return pro->name() == name;
                });

            if(itor != mProperty.end())
            {
#if 0
                //更新初值
                if( (PROPERTY_START_CONDITION == name)
                        || (PROPERTY_STOP_CONDITION == name)
                        || (PROPERTY_DESTDEVICE == name)
                        )
                {
                    proJs["initValue"] = (*itor)->initValue();
                }
#endif

                proJs["initValue"] = (*itor)->initValue();
                proJs["readOnly"] = (*itor)->isReadOnly();
                proJs["type"] = (*itor)->type();

                proJs["curValue"] = (*itor)->curValue();                

            }
            nodeJs[nodeIndex] = proJs;
        }

        propertyJs[index]["node"] = nodeJs;
    }

    mDefalultProperty["property"] = propertyJs;

    //qDebug() << mDefalultProperty.toStyledString().c_str();

    return mDefalultProperty;
}
