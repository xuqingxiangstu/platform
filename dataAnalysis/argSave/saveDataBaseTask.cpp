#include "saveDataBaseTask.h"

#include <QJsonArray>

saveDataBaseTask::saveDataBaseTask(QObject *parent)
    : QThread(parent),
      mIsStop(false),
      mResultTable(nullptr),
      mIsOver(false)
{

}

saveDataBaseTask::~saveDataBaseTask()
{
    if(isRunning())
    {
        mIsStop = true;

        quit();
        wait();
    }
}

void saveDataBaseTask::startTask(const QString &uuid)
{
    if(!isRunning())
    {
        mResultTable = databaseManager::getInstance()->getDataBase(uuid);
        mResultTable->clearTable();
        mIsStop = false;
        start();
    }
}

void saveDataBaseTask::onWrite(QJsonObject value)
{
    mMsgMutex.lock();

    mDataQueue.enqueue(value);

    mMsgMutex.unlock();
}

void saveDataBaseTask::onOver()
{
    mIsOver = true;
}

void saveDataBaseTask::run()
{
    while(!mIsStop)
    {
        QJsonArray values;

        mMsgMutex.lock();

        if(!mDataQueue.isEmpty())
        {
            int size = mDataQueue.size();

            size > QUEUE_SIZE ? size = QUEUE_SIZE : size = size;

            for(int index = 0; index < size; index++)
                values.append(mDataQueue.dequeue());
        }
        else
        {
            if(mIsOver)
                emit noMsg();
        }

        mMsgMutex.unlock();


        //存数据库
        if(values.size() > 0 && mResultTable != nullptr)
        {
            mResultTable->insertMultiValue(values);
        }
    }

}
