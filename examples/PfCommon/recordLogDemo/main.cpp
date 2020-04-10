#include <QCoreApplication>

#include "recordlogdemo.h"

/**
 * @brief 日志记录示例
 * @example PfCommon/recordLogDemo/main.cpp
 */

// 0:非线程
// 1:线程
#define THREAD_DEMO 1

//1 按照大小
//0 一个文件
#define RECORD_TYPE 1


workThread::workThread(QObject *parent):
    QThread(parent),
    isStop(false)
{
    //step1 实例化
#if THREAD_DEMO
    mRecord = std::make_shared<PfCommon::RecordLog>(true);
#else
    mRecord = std::make_shared<PfCommon::RecordLog>(false);
#endif
    //step2 连接信号
    connect(this, &workThread::recordSig, mRecord.get(), &PfCommon::RecordLog::record);
#if RECORD_TYPE
    //step3 设置存储方式及大小 10K = 10 * 1024
    mRecord->setRecordType(PfCommon::RecordLog::RECORD_FILE_SIZE);
    mRecord->setMaxSize(10*1024);
#else
    //step3 一个文件
    mRecord->setRecordType(PfCommon::RecordLog::RECORD_NORMAL);
#endif
    //step4 设置文件夹名称
    mRecord->setSource("测试");

    //启动存储线程
    start();
}

//析构
workThread::~workThread()
{
    if(isRunning())
    {
        isStop = true;
        wait();
    }
}

//线程执行体
void workThread::run()
{
    while(!isStop)
    {
        QByteArray msg;
        msg.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
        msg.append("\n");
        //存储
        emit recordSig(msg);

        msleep(10);
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::shared_ptr<workThread> work = std::make_shared<workThread>();

    int res = a.exec();

    return res;
}
