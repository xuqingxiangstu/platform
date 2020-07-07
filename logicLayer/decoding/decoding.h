#ifndef DECODING_H
#define DECODING_H

#include <QThreadPool>
#include <QRunnable>
#include <memory>
#include <QObject>
#include <QMap>

#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../src/PfCommon/jsoncpp/json.h"

/**
 * @brief The decodingPool class    数据解析池
 */


class decoding : public QObject, public QRunnable
{
    Q_OBJECT
public:
    decoding();
    ~decoding();

    void exe_decoding(Json::Value param, std::string recordUuid, std::string uuid, std::string ptl, Pf::PfAdapter::Adapter *adapterObj, const std::string &ipAddr, const int &port, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> frameObj, QByteArray msg);
signals:
    /**
     * @brief result    解析结果
     * @param v     结果
     */
    void result(Json::Value v);
public:
    void run();
private:
    bool package(QByteArray &srcMsg, QByteArray &curMsg);
    void toUi(const std::string &msg, bool state = true);
private:
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    Pf::PfAdapter::Adapter *mBusObj;    ///< 总线句柄
    QByteArray mCurMsg; ///< 当前消息
    std::string mDstIp;
    int mDstPort;
    std::string mCurPtrl;
    std::string mUuid;
    std::string mRecordUuid;
    Pf::PfAdapter::Adapter *mUiBus;
    Json::Value mParam;
};

class decodingPool : public QObject
{
    Q_OBJECT
public:
    decodingPool(QObject *parent = 0);
public:
    /**
     * @brief setIcdFrameAdpter 设置ICD管理句柄
     * @param obj   句柄
     */
    void setIcdFrameAdpter(std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> obj);

    void setPfAdapterManager(std::shared_ptr<Pf::PfAdapter::PfAdapterManager> manager){mPfAdapterManager = manager;}
public slots:
    void decode(Json::Value param, QString recordUuid, QString uuid, QString ptl, QByteArray msg, QString rcvIp, int rcvPort);
signals:
    /**
     * @brief result    解析结果
     * @param v     结果
     */
    void result(Json::Value v);
private:
    std::shared_ptr<QThreadPool> mThreadPool;
    std::shared_ptr<Pf::PfAdapter::PfAdapterManager> mPfAdapterManager;
    std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> mIcdWorkBench; ///< ICD解析器
    QMap<QString, std::shared_ptr<Pf::PfIcdWorkBench::frameObj>> mParseObj; ///< 解析器
    std::shared_ptr<decoding> mDecoding;
};

Q_DECLARE_METATYPE(Json::Value)

#endif // DECODING_H
