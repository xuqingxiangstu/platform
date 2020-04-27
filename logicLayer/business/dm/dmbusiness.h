#ifndef DMBUSINESS_H
#define DMBUSINESS_H

/**
  *定苗系统业务相关
  */

#include "../business.h"

class dmBusiness : public business
{
public:
    dmBusiness();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
private:
    void toUi(const std::string &msg, bool state = true);
    std::string getCmdName(unsigned int type);
    bool getTableAndCoding(int type, std::string &table, std::string &coding);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
};

#endif // DMBUSINESS_H
