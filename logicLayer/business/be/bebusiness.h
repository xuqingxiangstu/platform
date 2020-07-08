#ifndef BEBUSINESS_H
#define BEBUSINESS_H


/**
  *BE帧业务相关
  */

#include "../business.h"

class beBusiness : public business
{
public:
    beBusiness();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
    std::shared_ptr<business> clone() override;
private:
    void toUi(const std::string &msg, bool state = true);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
};


#endif // MIDDLEBUSINESS_H
