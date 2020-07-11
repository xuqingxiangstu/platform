#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "../src/PfSql/paramsTable/parseResult/resulttable.h"
#include "../src/PfSql/paramsTable/parseResult/createresultdb.h"

#include <memory>
#include <QMap>


/**
 * @brief The databaseManager class 数据库管理
 */
class databaseManager
{
public:
    static std::shared_ptr<databaseManager> getInstance()
    {
        if(mInstance == nullptr)
        {
            mInstance = std::make_shared<databaseManager>();
        }

        return mInstance;
    }

public:
    databaseManager();
    ~databaseManager();
public:
    std::shared_ptr<resultTable> getDataBase(const QString &uuid);
private:
    QMap<QString, std::shared_ptr<resultTable>> mDataBaseManager;
private:
    static std::shared_ptr<databaseManager> mInstance;
};

#endif // DATABASEMANAGER_H
