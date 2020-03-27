#ifndef NODEPROPERTY_H
#define NODEPROPERTY_H

#include <memory>

#include "../src/PfCommon/jsoncpp/json.h"

/*************属性宏定义**********************/
#define PROPERTY_DESCRIBE           "describe"  //描述
#define PROPERTY_START_CONDITION    "startCondition"
#define PROPERTY_DELAY              "delay"
#define PROPERTY_TIMING             "timing"
#define PROPERTY_STOP_CONDITION     "stopCondition"
#define PROPERTY_DESTDEVICE         "destDevice"
#define PROPERTY_SIM_MODEL          "simulationModel"
#define PROPERTY_FIX_VALUE          "fixValue"
#define PROPERTY_RAND_MIN           "rand_min"
#define PROPERTY_RAND_MAX           "rand_max"
#define PROPERTY_LINE_MIN           "line_min"
#define PROPERTY_LINE_MAX           "line_max"
#define PROPERTY_LINE_STEP          "line_step"

/****************参数变化宏*******************/
#define PROPERTY_MODEL_FIX  "固定值"
#define PROPERTY_MODEL_RAND "随机数"
#define PROPERTY_MODEL_LINE "直线"

/****************条件属性宏*******************/
#define PROPERTY_CONDITION_NAME     "name"
#define PROPERTY_CONDITION_TABLE    "table"
#define PROPERTY_CONDITION_CODING   "coding"
#define PROPERTY_CONDITION_NO       "无"

#define PROPERTY_CONDITION_VALUE_NAME   "name"
#define PROPERTY_CONDITION_VALUE_TABLE_NUM    "table"
#define PROPERTY_CONDITION_VALUE_CODING_NUM   "coding"

/*****************设备属性**************************/

#define PROPERTY_DEV_VALUE_NAME "name"
#define PROPERTY_DEV_VALUE_UUID "uuid"

/*****************属性类型******************/
#define PROPERTY_STRING_TYPE    "string"
#define PROPERTY_INT_TYPE       "int"
#define PROPERTY_DOUBLE_TYPE    "double"
#define PROPERTY_ENUM_TYPE      "enum"
#define PROPERTY_TRIGGER_TYPE   "trigger"


class property
{
public:
    void setType(const std::string &type){mType = type;}
    std::string type(){return mType;}

    void setReadOnly(bool isRead){mIsRead = isRead;}
    bool isReadOnly(){return mIsRead;}

    void setName(const std::string &name){mName = name;}
    std::string name(){return mName;}

    void setInitValue(const Json::Value &v){mInitValue = v;}
    Json::Value initValue(){return mInitValue;}

    void setCurValue(const Json::Value &v)
    {
        mCurValue = v;
    }
    Json::Value curValue(){return mCurValue;}

    std::shared_ptr<property> clone()
    {
        property *obj = new property();
        obj->mCurValue = this->mCurValue;
        obj->mInitValue = this->mInitValue;
        obj->mName = mName;
        obj->mIsRead = mIsRead;
        obj->mType = mType;
        return std::shared_ptr<property>(obj);
    }
private:
    std::string mName;
    std::string mType;
    bool mIsRead;
    Json::Value mInitValue;
    Json::Value mCurValue;
};

class nodeProperty
{
public:   
    nodeProperty();

    nodeProperty(const Json::Value &v);

    nodeProperty(const nodeProperty &other) = delete;
    nodeProperty &operator =(const nodeProperty &other) = delete;
    nodeProperty(const nodeProperty &&other) = delete;
    nodeProperty & operator =(nodeProperty &&other) = delete;
public:
    std::shared_ptr<nodeProperty> clone();
public:
    void setInitValue(const std::string &name, const Json::Value &initValue);

    void setCurValue(const std::string &name, const Json::Value &curValue);
    Json::Value curValue(const std::string &name);

    void setReadOnly(const std::string &name, bool isRead);
    bool isReadOnly(const std::string &name);

    void setType(const std::string &name, const std::string &type);
    std::string type(const std::string &name);

    void setProperty(const std::string &name, const Json::Value &value);
    void getProperty(const std::string &name, Json::Value &value);

    void setTableNum(int table){mTableNum = table;}
    int tableNum(){return mTableNum;}

    void setCodingNum(int coding){mCodingNum = coding;}
    int codingNum(){return mCodingNum;}

    /**
     * @brief getKey    获取关键字（cmd 、param、param_group）
     * @return
     */
    std::string getKey();
    Json::Value getJson();
private:
    Json::Value mDefalultProperty;
    int mTableNum;
    int mCodingNum;
    std::vector<std::shared_ptr<property>> mProperty;
};

#endif // NODEPROPERTY_H
