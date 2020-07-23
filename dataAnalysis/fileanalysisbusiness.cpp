#include "fileanalysisbusiness.h"

#include "./argSave/saveDataBaseTask.h"
#include <QEventLoop>
#include <QFile>
#include <QUuid>
#include <QThread>
#include <QDebug>

fileAnalysisBusiness::fileAnalysisBusiness()
{

}

void fileAnalysisBusiness::onAnalysis(const QString &proUuid, const QString &dbPath, QVector<QPair<QString, std::shared_ptr<analysisRule>> > fileInfo)
{
    //step1：显示进度条窗体

    mCurAnalysisStep = 1;
    mAnalysisUuids = QStringList();

    analysisProgress progress;
    connect(this, &fileAnalysisBusiness::curAnalysisStep, &progress, &analysisProgress::setCurV);

    emit showMessage("正在解析，请稍等...");

    //启动解析线程
    QThread *thread = new QThread;

    connect(thread, &QThread::started, [&](){

        QEventLoop loop;

        mAnalysisObj = std::make_shared<analysis>();
        connect(mAnalysisObj.get(), &analysis::showMessage, this, &fileAnalysisBusiness::showMessage);
        connect(this, SIGNAL(toAnalysis(QString, QString, std::shared_ptr<analysisRule>, std::shared_ptr<filterManager>)), mAnalysisObj.get(), SLOT(onAnalysis(QString, QString, std::shared_ptr<analysisRule>, std::shared_ptr<filterManager>)));        
        connect(mAnalysisObj.get(), SIGNAL(analysisOver(QString)), this, SLOT(onAnalysisOver(QString)));
        connect(mAnalysisObj.get(), SIGNAL(step(int)), this, SLOT(onAnalysisStep(int)));

        mSaveDbTask = std::make_shared<saveDataBaseTask>();
        connect(mAnalysisObj.get(), &analysis::toDataBase, mSaveDbTask.get(), &saveDataBaseTask::onWrite);

        //启动存储线程
        mSaveDbTask->startTask(dbPath, proUuid);
        connect(mSaveDbTask.get(), &saveDataBaseTask::noMsg, &loop, &QEventLoop::quit);

        //获取最大值
        int maxCount = getAnalysisMaxSize(fileInfo);
        progress.setMinMaxV(1, maxCount);

        //解析
        for(QPair<QString, std::shared_ptr<analysisRule>> info : fileInfo)
        {
            QString uuid = QUuid::createUuid().toString();

            mAnalysisUuids.append(uuid);

            emit toAnalysis(uuid, info.first, info.second, nullptr);
        }

        mSaveDbTask->onOver();

        loop.exec();

        mAnalysisObj.reset();
        mSaveDbTask.reset();


        thread->quit();
    });

    //connect(this, &fileAnalysisBusiness::allAnalysisOver, &progress, &analysisProgress::onClose);
    connect(thread, &QThread::finished, &progress, &analysisProgress::onClose);

    thread->start();

    progress.exec();

    emit showMessage("解析完成");

    delete thread;
    thread = nullptr;
}

void fileAnalysisBusiness::onAnalysisOver(QString uuid)
{
    if(mAnalysisUuids.contains(uuid))
    {
        mAnalysisUuids.removeOne(uuid);
    }

    if(mAnalysisUuids.size() == 0)
    {
        emit allAnalysisOver();
    }
}

int fileAnalysisBusiness::getAnalysisMaxSize(const QVector<QPair<QString, std::shared_ptr<analysisRule>>> &fileInfo)
{
    int maxCount = 0;

    for(QPair<QString, std::shared_ptr<analysisRule>> info : fileInfo)
    {
        QFile fileObj(info.first);
        if(!fileObj.open(QIODevice::ReadOnly))
        {
            continue;
        }

        QByteArray allData = fileObj.readAll();

        //step1：分割帧
        QString tmpAllData = allData;

        QStringList frames = tmpAllData.split(info.second->getSegmentationMark());

        maxCount += frames.size();

        fileObj.close();
    }

    return maxCount;
}

void fileAnalysisBusiness::onAnalysisStep(int step)
{
    mCurAnalysisStep += step;

    emit curAnalysisStep(mCurAnalysisStep);
}
