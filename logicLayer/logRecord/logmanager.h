#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <memory>
#include <vector>
#include <tuple>
#include <mutex>

#include "../src/PfCommon/recordLog/recordlog.h"

class logManager : public QObject
{
    Q_OBJECT
public:
    static std::shared_ptr<logManager> getInstance()
    {
        if(mInstance == nullptr)
        {
            mInstance = std::make_shared<logManager>();
        }

        return mInstance;
    }

public:
    logManager(QObject *parent = 0);
    ~logManager();
public:
    /**
     * @brief reset 重置，关闭文件夹及删除相应句柄
     */
    void reset();

    /**
     * @brief init  初始化各设备
     * @param devs  设备
     */
    void init(const std::vector<std::tuple<std::string, std::string, std::string, std::string>> &devs);
public slots:
    /**
     * @brief record    记录日志
     * @param uuid      uuid
     * @param Msg       消息
     */
    void record(QString uuid, QByteArray Msg);
signals:
    void toLog(QString uuid, QByteArray msg);
private:
    std::vector<std::shared_ptr<Pf::PfCommon::RecordLog>> mRecordsObj; //日志记录
    std::mutex mMutex;
private:
    static std::shared_ptr<logManager> mInstance;
};

#endif // LOGMANAGER_H
