#ifndef BUSINESS_H
#define BUSINESS_H

#include "../src/PfCommon/jsoncpp/json.h"

#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"

#include "../flow/frame.h"

class business
{
public:
    business(){}
    virtual ~business(){}
public:
    /**
     * @brief deal  业务处理逻辑
     * @param msg
     */
    virtual void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg){}

    /**
     * @brief setBusObj 设置总线句柄
     * @param obj
     */
    virtual void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort){}

    /**
     * @brief setFrameObj   设置帧类型
     * @param obj
     */
    virtual void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj){}

    /**
     * @brief setRecordUuid 设置记录UUID
     * @param uuid
     */
    virtual void setRecordUuid(const std::string &uuid){}

    /**
     * @brief setRecordUuid 设置设备UUID
     * @param uuid
     */
    virtual void setDevUuid(const std::string &uuid){}

    virtual void setParam(const Json::Value &param){}

    /**
     * @brief clone 类克隆（深拷贝）
     * @return 类句柄
     */
    virtual std::shared_ptr<business> clone(){return nullptr;}
};

#endif // BUSINESS_H
