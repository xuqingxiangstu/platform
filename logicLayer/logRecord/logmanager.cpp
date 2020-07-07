#include "logmanager.h"

std::shared_ptr<logManager> logManager::mInstance = nullptr;

logManager::logManager(QObject *parent)
    :QObject(parent)
{

}

logManager::~logManager()
{
    reset();
}

void logManager::reset()
{
    mMutex.lock();
    for(auto itor = mRecordsObj.begin(); itor != mRecordsObj.end(); )
    {
        (*itor).reset();

        mRecordsObj.erase(itor);
    }
    mMutex.unlock();
}

void logManager::record(QString uuid, QByteArray Msg)
{
    emit toLog(uuid, Msg);
}

void logManager::init(const std::vector<std::tuple<std::string, std::string, std::string, std::string>> &devs)
{
    mMutex.lock();
    for(auto dev : devs)
    {
        std::shared_ptr<Pf::PfCommon::RecordLog> rd = std::make_shared<Pf::PfCommon::RecordLog>(true);

        rd->setUuid(std::get<0>(dev).c_str());
        rd->setSource(std::get<1>(dev));

        connect(this, &logManager::toLog, rd.get(), &Pf::PfCommon::RecordLog::record);

        mRecordsObj.emplace_back(rd);
    }

    mMutex.unlock();
}
