#include "databasemanager.h"

std::shared_ptr<databaseManager> databaseManager::mInstance = nullptr;

databaseManager::databaseManager()
{

}

databaseManager::~databaseManager()
{
    for(auto itor = mDataBaseManager.begin(); itor != mDataBaseManager.end(); )
    {
        (itor.value()).reset();
        mDataBaseManager.erase(itor++);
    }
}

std::shared_ptr<resultTable> databaseManager::getDataBase(const QString &uuid)
{
    std::shared_ptr<resultTable> db;
    if(mDataBaseManager.contains(uuid))
    {
        db = mDataBaseManager.value(uuid);
    }
    else
    {
        QString path = createResultDb::getInstance()->create(uuid);
        db = std::make_shared<resultTable>(path, uuid);
        mDataBaseManager[uuid] = db;
    }

    return db;
}
