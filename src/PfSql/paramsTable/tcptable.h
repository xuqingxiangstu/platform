#ifndef TCPTABLE_H
#define TCPTABLE_H

#include "paramstable_global.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/
#define UDP_TABLE_UUID      "UUID"
#define UDP_TABLE_IP_ADDR   "IP_ADDR"
#define UDP_TABLE_PORT      "PORT"
#define UDP_TABLE_RESERVE   "RESERVE"
#define UDP_TABLE_DEV_NAME   "DEV_NAME"

class PARAMSTABLESHARED_EXPORT tcpTable
{
public:
    static tcpTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new tcpTable();
        }
        return mInstance;
    }

    ~tcpTable();
public:
    Json::Value getValue(std::string uuid);
private:
    tcpTable();
private:
    const QString mTableName = "tcp_table";
    QSqlDatabase mDb;
private:
    static tcpTable *mInstance;
};

#endif // TCPTABLE_H
