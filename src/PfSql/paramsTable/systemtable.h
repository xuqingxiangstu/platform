#ifndef SYSTEMTABLE_H
#define SYSTEMTABLE_H

#include "paramstable_global.h"


#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/
#define SYSTEM_TABLE_UUID           "UUID"
#define SYSTEM_TABLE_SYSTEM_NAME    "SYSTEM_NAME"
#define SYSTEM_TABLE_SYSTEM_TYPE    "SYSTEM_TYPE"
#define SYSTEM_TABLE_RESERVE        "RESERVE"

class PARAMSTABLESHARED_EXPORT systemTable
{
public:
    static systemTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new systemTable();
        }
        return mInstance;
    }

    ~systemTable();
public:
    Json::Value getSysGroups();
    Json::Value getSysInfoByUuid(const std::string &uuid);
    bool getSystemDev(QString systemName,Json::Value &value);
    bool getDevDescribe(QSqlRecord systemRec,QString &Describe,QString &DevName);
    QString getDevTableName(QString Tabletype);
private:
    systemTable();
private:
    const QString mTableName = "system_table";
    QSqlDatabase mDb;
private:
    static systemTable *mInstance;
};

#endif // SYSTEMTABLE_H
