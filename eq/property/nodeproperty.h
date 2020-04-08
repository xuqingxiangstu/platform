#ifndef NODEPROPERTY_H
#define NODEPROPERTY_H

#include <memory>

#include "../src/PfCommon/jsoncpp/json.h"

/*************帧类型属性********************/
#define PROPERTY_FRAME  "帧类型"
#define PROPERTY_FRAME_BE   "BE"
#define PROPERTY_FRAME_FE   "FE"
#define PROPERTY_FRAME_93   "93"
#define PROPERTY_FRAME_MIDDLE   "中间件"

/*************记录属性********************/
#define PROPERTY_RECORD_NAME    "名称"
#define PROPERTY_RECORD_CREATE_TIME "创建时间"
#define PROPERTY_RECORD__OPEN_TIME  "最近打开时间"
#define PROPERTY_RECORD__NODE       "备注"
#define PROPERTY_RECORD_DEV_SEL     "接口选择"

/*************系统属性********************/
#define PROPERTY_SYS_NAME   "名称"
#define PROPERTY_SYS_TYPE   "系统类型"

/*************信源属性********************/
#define PROPERTY_SRC    "信源属性"
#define PROPERTY_DST    "信宿属性"

#define PROPERTY_USER   "用户"
#define PROPERTY_SOFT_VERSION   "软件标识"

#define PROPERTY_SRC_SYS_TYPE   "系统类型"
#define PROPERTY_SRC_SYS_CODING "系统编码"
#define PROPERTY_SRC_NODE_CODING    "节点编码"


/*************属性宏定义**********************/
#define PROPERTY_DESCRIBE           "describe"  //描述
#define PROPERTY_START_CONDITION    "startCondition"
#define PROPERTY_DELAY              "delay"
#define PROPERTY_TIMING             "timing"
#define PROPERTY_STOP_CONDITION     "stopCondition"
#define PROPERTY_DESTDEVICE         "接口选择"
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

    void setAttr(const Json::Value &value){mCurAttr = value;}
    Json::Value curAttr(){return mCurAttr;}

    void setType(const std::string &type){mType = type;}
    std::string type(){return mType;}

    void setReadOnly(bool isRead){mIsRead = isRead;}
    bool isReadOnly(){return mIsRead;}

    void setVisible(bool isRead){mIsVisible = isRead;}
    bool isVisible(){return mIsVisible;}

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
        obj->mIsVisible = mIsVisible;
        obj->mType = mType;
        return std::shared_ptr<property>(obj);
    }
private:
    std::string mName;
    std::string mType;
    bool mIsRead;
    bool mIsVisible;
    Json::Value mInitValue;
    Json::Value mCurValue;
    Json::Value mCurAttr;
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

    Json::Value curAttr(const std::string &name);

    void setReadOnly(const std::string &name, bool isRead);
    bool isReadOnly(const std::string &name);

    void setVisible(const std::string &name, bool isVisible);
    bool isVisible(const std::string &name);

    void setType(const std::string &name, const std::string &type);
    std::string type(const std::string &name);

    void setProperty(const std::string &name, const Json::Value &value);
    void getProperty(const std::string &name, Json::Value &value);

    void setTableNum(int table);
    int tableNum();

    void setCodingNum(int coding);
    int codingNum();

    /**
     * @brief getKey    获取关键字（cmd 、param、param_group）
     * @return
     */
    std::string getKey();
    Json::Value getJson();
private:
    Json::Value mDefalultProperty;    
    std::vector<std::shared_ptr<property>> mProperty;
};

#endif // NODEPROPERTY_H
