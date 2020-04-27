#ifndef TCPSERVERTABLE_H
#define TCPSERVERTABLE_H

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

class PARAMSTABLESHARED_EXPORT tcpServerTable
{
public:
    static tcpServerTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new tcpServerTable();
        }
        return mInstance;
    }

    ~tcpServerTable();
public:
    Json::Value getValue(std::string uuid);
private:
    tcpServerTable();
private:
    const QString mTableName = "tcp_server_table";
    QSqlDatabase mDb;
private:
    static tcpServerTable *mInstance;
};

#endif // TCPSERVERTABLE_H
