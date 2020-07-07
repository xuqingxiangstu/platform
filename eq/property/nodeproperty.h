#ifndef NODEPROPERTY_H
#define NODEPROPERTY_H

#include <memory>

#include "../src/PfCommon/jsoncpp/json.h"

/*************基本属性********************/
#define PROPERTY_BASE_INFO_WORD "信息字格式"
#define PROPERTY_BASE_TABLE_NUM    "表号"
#define PROPERTY_BASE_CODING_NUM       "编码"

#define PROPERTY_FRAME_TYPE_ATTR    "帧类型属性"
/*************帧类型属性********************/
#define PROPERTY_FRAME  "帧类型"
#define PROPERTY_FRAME_BE   "BE"
#define PROPERTY_FRAME_FE   "FE"
#define PROPERTY_FRAME_93   "密码集控装置协议"
#define PROPERTY_FRAME_MIDDLE   "中间件"
#define PROPERTY_FRAME_1553B   "M1553B协议"
#define PROPERTY_FRAME_JG   "机柜协议"
#define PROPERTY_FRAME_CZXK   "车长显控通讯协议"

#define PROPERTY_ASK_FLAG  "确认标志"

/*****************1553B RT属性**********************/
#define PROPERTY_1553B           "1553B属性"
#define PROPERTY_1553B_MODE     "模式"
#define PROPERTY_1553B_BUS      "总线"

#define PROPERTY_1553B_MODE_BC_RT   "BC->RT"
#define PROPERTY_1553B_MODE_RT_RT   "RT->RT"

#define PROPERTY_1553B_BC_RT    "BC->RT属性"
#define PROPERTY_1553B_BC_RT_RT_ADDR "RT地址"
#define PROPERTY_1553B_BC_RT_SA_ADDR "SA地址"

#define PROPERTY_1553B_RT_RT    "RT->RT属性"
#define PROPERTY_1553B_RT_RT_S_RT_ADDR  "发送RT地址"
#define PROPERTY_1553B_RT_RT_S_SA_ADDR  "发送SA地址"
#define PROPERTY_1553B_RT_RT_R_RT_ADDR  "接收RT地址"
#define PROPERTY_1553B_RT_RT_R_SA_ADDR  "接收SA地址"
#define PROPERTY_1553B_RT_RT_DATA_SIZE  "数据长度"

/*************记录属性********************/
#define PROPERTY_RECORD_NAME    "名称"
#define PROPERTY_RECORD_CREATE_TIME "创建时间"
#define PROPERTY_RECORD__OPEN_TIME  "最近打开时间"
#define PROPERTY_RECORD__NODE       "备注"
//#define PROPERTY_RECORD_DEV_SEL     "接口选择"
#define PROPERTY_RECORD_LOCAL_DEVICE  "等效接口选择"
#define PROPERTY_RECORD_DEST_DEVICE   "目标接口选择"

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

/*************其它属性********************/
#define PROPERTY_OTHER  "其它属性"
#define PROPERTY_OTHER_D_NUM    "弹编号"
#define PROPERTY_OTHER_CL_TYPE  "车辆类型"
#define PROPERTY_OTHER_CL_NUM   "车辆编号"
#define PROPERTY_OTHER_DEV_INDEX    "设备序号"
#define PROPERTY_OTHER_MODLE_INDEX  "模块序号"
#define PROPERTY_OTHER_RESERVE      "备用字符串"
#define PROPERTY_OTHER_DATA         "数据"

/*******************FE帧数据类型属性*********************/
#define PROPERTY_FE_DATA_TYPE   "数据帧类型"
#define PROPERTY_FE_DATA_TYPE_SEND_SYS  "发起系统"
#define PROPERTY_FE_DATA_TYPE_RCV_SYS   "接收系统"


/*************属性宏定义**********************/
#define PROPERTY_DESCRIBE           "describe"  //描述
#define PROPERTY_START_CONDITION    "startCondition"
#define PROPERTY_DELAY              "delay"
#define PROPERTY_TIMING             "timing"
#define PROPERTY_STOP_CONDITION     "stopCondition"
#define PROPERTY_LOCAL_DEVICE       "等效接口选择"
#define PROPERTY_DEST_DEVICE        "目标接口选择"
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

#define PROPERTY_CONDITION_VALUE_NAME           "name"
#define PROPERTY_CONDITION_VALUE_TABLE_NUM      "table"
#define PROPERTY_CONDITION_VALUE_CODING_NUM     "coding"
#define PROPERTY_CONDITION_VALUE_VALUE          "value"

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
        mCurAttr["curValue"] = v;
    }
    Json::Value curValue(){return mCurValue;}

    std::shared_ptr<property> clone()
    {
        property *obj = new property();
        obj->mCurValue = this->mCurValue;
        obj->mInitValue = this->mInitValue;
        obj->mCurAttr = this->mCurAttr;
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
    Json::Value initValue(const std::string &name);

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

    void setTableNum(std::string table);
    std::string tableNum();

    void setCodingNum(int coding);
    int codingNum();

    bool isExist(const std::string &name);

    /**
     * @brief getKey    获取关键字（cmd 、param、param_group）
     * @return
     */
    std::string getKey();
    Json::Value getJson();
    Json::Value getSaveJson();
private:
    void initProperty(Json::Value initV);
private:
    Json::Value mDefalultProperty;    
public:
    std::vector<std::shared_ptr<property>> mProperty;
};

#endif // NODEPROPERTY_H
