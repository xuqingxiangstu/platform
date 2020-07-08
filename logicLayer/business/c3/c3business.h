#ifndef C3BUSINESS_H
#define C3BUSINESS_H


/**
  *C3业务相关
  */

#include <QStringList>

#include "../business.h"

class c3Business : public business
{
    QStringList testName = {
        "",
        "进入发射流程",
        "弹上设备加电",
        "上传单机、点位诸元",
        "基准图传递",
        "上传被动雷达库信息",
        "上传弹道诸元",
        "上传引控诸元",
        "瞄准结束",
        "导弹准备出发",
        "开栓",
        "电池激活",
        "转电",
        "允许伺服电池激活",
        "允许发射"
    };

    QStringList table3VoltageName = {
        "",
        "HGP母线电压",
        "控制母线电压",
        "惯组I/F电路温度",
        "惯组加表表头温度",
        "惯组X陀螺监视电压",
        "惯组Y陀螺监视电压",
        "惯组Z陀螺监视电压"
    };

    QStringList table4VoltageName = {
        "",
        "一级自毁安全保险机构保险状态",
        "二级自毁安全保险机构保险状态"
    };
public:
    c3Business();
public:
    void deal(const Pf::PfIcdWorkBench::byteArray &inData, Json::Value &msg) override;
    void setBusObj(Pf::PfAdapter::Adapter *obj, const std::string &dstIp, const int &dstPort) override;
    void setFrameObj(std::shared_ptr<Pf::PfIcdWorkBench::frameObj> obj) override;
    void setRecordUuid(const std::string &uuid) override {mRecordUuid = uuid;}
    void setDevUuid(const std::string &uuid) override {mDevUuid = uuid;}
    std::shared_ptr<business> clone() override;
private:
    void respondCmd(int frameType, int code, Json::Value array);
    void toUi(const std::string &msg, bool state = true);
    std::string getCmdName(int type);
    bool getTableAndCoding(int type, std::string &table, std::string &coding);
private:
    void exeCmd(Json::Value regionJs);

    void testInfo(Json::Value regionJs);

    void queryVersion(Json::Value regionJs);

    void checkCmd(Json::Value regionJs);

    void sendFsCmd(Json::Value regionJs);
private:
    Pf::PfAdapter::Adapter *mBusObj;
    Pf::PfAdapter::Adapter *mUiBus;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    std::string mDstIp;
    std::string mRecordUuid;
    std::string mDevUuid;
    int mDstPort;
};



#endif // C3BUSINESS_H
