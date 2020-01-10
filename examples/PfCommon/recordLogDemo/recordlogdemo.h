#ifndef RECORDLOGDEMO
#define RECORDLOGDEMO

#include <QThread>
#include <QDateTime>
#include <QByteArray>
#include <QObject>

#include "../../../src/PfCommon/recordLog/recordlog.h"

using namespace Pf;

class workThread  : public QThread
{
    Q_OBJECT
public:
    workThread(QObject *parent = 0);

    ~workThread();
signals:
    //记录日志
    void recordSig(QByteArray msg);
public:
    //线程执行体
    void run() override;
private:
    std::shared_ptr<PfCommon::RecordLog> mRecord;
    bool isStop;
};

#endif // RECORDLOGDEMO

