#ifndef MIDDLEBUSINESS_H
#define MIDDLEBUSINESS_H


/**
  *中间件业务相关
  */

#include "../business.h"

class middleBusiness : public business
{
public:
    middleBusiness();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
    std::shared_ptr<business> clone() override;
private:
    void toUi(const std::string &msg, bool state = true);
    void exe4307(const int &coding);

    void zy(Json::Value info);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
    const std::string mVirtualTable = "800003";
};


#endif // MIDDLEBUSINESS_H
