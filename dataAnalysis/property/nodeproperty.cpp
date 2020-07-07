#include "nodeproperty.h"
#include <algorithm>
#include <QDebug>

nodeProperty::nodeProperty()
{

}

void nodeProperty::initProperty(Json::Value initV)
{

}

nodeProperty::nodeProperty(const Json::Value &v)
{
    mDefalultProperty = v;

    std::string key = v["key"].asString();
    Json::Value propertyJs = v["property"];

    for(int index = 0; index < propertyJs.size(); index++)
    {
        Json::Value groupJs = propertyJs[index]["group"];
        Json::Value readOnlyJs = propertyJs[index]["readOnly"];
        Json::Value isVisibleJs = propertyJs[index]["isVisible"];

        std::shared_ptr<property> groupObj = std::make_shared<property>();
        groupObj->setName(groupJs.asString());
        groupObj->setReadOnly(readOnlyJs.asBool());
        groupObj->setVisible(isVisibleJs.asBool());

        mProperty.emplace_back(groupObj);

        Json::Value nodeJs = propertyJs[index]["node"];
        for(int nodeIndex = 0; nodeIndex < nodeJs.size(); nodeIndex++)
        {
            Json::Value proJs = nodeJs[nodeIndex];

            std::shared_ptr<property> obj = std::make_shared<property>();

            obj->setAttr(proJs);
            obj->setType(proJs["type"].asString());
            obj->setName(proJs["attr"].asString());
            obj->setReadOnly(proJs["readOnly"].asBool());
            obj->setVisible(proJs["isVisible"].asBool());
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

Json::Value nodeProperty::curAttr(const std::string &name)
{
    Json::Value tmpV;

    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        tmpV = (*itor)->curAttr();
    }
    return tmpV;
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

void nodeProperty::setVisible(const std::string &name, bool isVisible)
{
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        (*itor)->setVisible(isVisible);
    }
}

bool nodeProperty::isVisible(const std::string &name)
{
    bool res = false;
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        res = (*itor)->isVisible();
    }

    return res;
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


Json::Value nodeProperty::initValue(const std::string &name)
{
    Json::Value tmpV;

    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        tmpV = (*itor)->initValue();
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

bool nodeProperty::isExist(const std::string &name)
{
    bool res = false;
    auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
        {
            return pro->name() == name;
        });

    if(itor != mProperty.end())
    {
        res = true;
    }

    return res;
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

    //tmp->mTableNum = mTableNum;
    //tmp->mCodingNum = mCodingNum;

    return std::shared_ptr<nodeProperty>(tmp);
}

void nodeProperty::setTableNum(std::string table)
{
    mDefalultProperty["table"] = table;
}

std::string nodeProperty::tableNum()
{
    Json::Value tmpJs = mDefalultProperty["table"];
    if(tmpJs.isNull())
        return "";

    return tmpJs.asString();
}

int nodeProperty::codingNum()
{
    Json::Value tmpJs = mDefalultProperty["coding"];
    if(tmpJs.isNull())
        return 0;

    return tmpJs.asInt();
}

void nodeProperty::setCodingNum(int coding)
{
    mDefalultProperty["coding"] = coding;
}

Json::Value nodeProperty::getSaveJson()
{
    //只更改默认属性中的curValue值即可

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
                //modify xqx 20200421 由于指令太多，保存时不保存初始化值（软件启动后会自动读取数据库进行更新）
                if( (PROPERTY_START_CMDORPARAM == name) || (PROPERTY_STOP_CMDORPARAM == name)
                        || (PROPERTY_FE_DATA_TYPE_SEND_SYS == name) || (PROPERTY_FE_DATA_TYPE_RCV_SYS == name)
                        || (PROPERTY_START_SYSTEM == name) || (PROPERTY_STOP_SYSTEM == name)
                        )
                {
                    proJs["initValue"] = Json::Value();
                }
                else
                {
                    proJs["initValue"] = (*itor)->initValue();
                }

                proJs["readOnly"] = (*itor)->isReadOnly();
                proJs["type"] = (*itor)->type();
                proJs["isVisible"] = (*itor)->isVisible();
                proJs["curValue"] = (*itor)->curValue();

            }
            nodeJs[nodeIndex] = proJs;
        }

        //modify xqx 更新gourp readOnly

        std::string name = propertyJs[index]["group"].asString();

        auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
            {
                return pro->name() == name;
            });

        if(itor != mProperty.end())
        {
            propertyJs[index]["readOnly"] = (*itor)->isReadOnly();
            propertyJs[index]["isVisible"] = (*itor)->isVisible();
        }

        propertyJs[index]["node"] = nodeJs;
    }

    mDefalultProperty["property"] = propertyJs;

    return mDefalultProperty;
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
                proJs["initValue"] = (*itor)->initValue();
                proJs["readOnly"] = (*itor)->isReadOnly();
                proJs["type"] = (*itor)->type();
                proJs["isVisible"] = (*itor)->isVisible();
                proJs["curValue"] = (*itor)->curValue();                

            }
            nodeJs[nodeIndex] = proJs;
        }

        //modify xqx 更新gourp readOnly

        std::string name = propertyJs[index]["group"].asString();

        auto itor = std::find_if(mProperty.begin(), mProperty.end(), [=](const std::shared_ptr<property> &pro)
            {
                return pro->name() == name;
            });

        if(itor != mProperty.end())
        {
            propertyJs[index]["readOnly"] = (*itor)->isReadOnly();
            propertyJs[index]["isVisible"] = (*itor)->isVisible();
        }

        propertyJs[index]["node"] = nodeJs;
    }

    mDefalultProperty["property"] = propertyJs;

    //qDebug() << mDefalultProperty.toStyledString().c_str();

    return mDefalultProperty;
}
