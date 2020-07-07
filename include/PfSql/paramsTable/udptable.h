#ifndef UDPTABLE_H
#define UDPTABLE_H

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

class PARAMSTABLESHARED_EXPORT udpTable
{
public:
    static udpTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new udpTable();
        }
        return mInstance;
    }

    ~udpTable();
public:
    Json::Value getValue(std::string uuid);
private:
    udpTable();
private:
    const QString mTableName = "udp_table";
    QSqlDatabase mDb;
private:
    static udpTable *mInstance;
};

#endif // UDPTABLE_H
