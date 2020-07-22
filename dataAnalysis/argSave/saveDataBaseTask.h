#ifndef SAVEDATABASETASK_H
#define SAVEDATABASETASK_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <atomic>
#include <QThread>
#include <QWaitCondition>
#include <QJsonObject>

#include "databasemanager.h"

#define QUEUE_SIZE  500

/**
 * @brief The saveDataBaseTask class  数据存储任务，采用队列方式，每500个数据存储一次
 */

class saveDataBaseTask : public QThread
{
    Q_OBJECT
public:
    explicit saveDataBaseTask(QObject *parent = 0);
    ~saveDataBaseTask();
public:
    void startTask(const QString &dbPath, const QString &uuid);
private:
    void run();
signals:
    void noMsg();
public slots:
    void onWrite(QJsonObject value);
    void onOver();
private:
    std::shared_ptr<resultTable> mResultTable;
    QQueue<QJsonObject> mDataQueue;
    QMutex mMsgMutex;
    QString mDbPath;
    QString mUuid;
    std::atomic_bool mIsStop;
    std::atomic_bool mIsOver;
};

#endif // TODATABASE_H
