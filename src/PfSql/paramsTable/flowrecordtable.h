#ifndef FLOWRECORDTABLE_H
#define FLOWRECORDTABLE_H

#include "paramstable_global.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/
#define FLOW_RECORD_TABLE_UUID              "UUID"
#define FLOW_RECORD_TABLE_SYSTEM_UUID       "SYSTEM_UUID"
#define FLOW_RECORD_TABLE_FLOW_NAME         "FLOW_NAME"
#define FLOW_RECORD_TABLE_CREATE_TIME       "CREATE_TIME"
#define FLOW_RECORD_TABLE_RECENT_OPEN_TIME  "RECENT_OPEN_TIME"
#define FLOW_RECORD_TABLE_NOTE              "NOTE"

class PARAMSTABLESHARED_EXPORT flowRecordTable
{
public:
    static flowRecordTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new flowRecordTable();
        }
        return mInstance;
    }

    ~flowRecordTable();
public:
    Json::Value getValue(std::string uuid);

    Json::Value getValueBySystemUuid(const std::string &sysUuid);

    bool deleteRecordByUuid(const std::string &uuid);

    bool addRecord(const std::string &sysUuid, const std::string &flowName, const std::string &note, std::string &uuid);
private:
    flowRecordTable();
private:
    const QString mTableName = "flow_record_table";
    QSqlDatabase mDb;
private:
    static flowRecordTable *mInstance;
};

#endif // FLOWRECORDTABLE_H
