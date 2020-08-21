#ifndef SYSINTERFACETABLE_H
#define SYSINTERFACETABLE_H

#include "paramstable_global.h"


#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/
#define SYSTEM_INTERFACE_TABLE_UUID           "UUID"
#define SYSTEM_INTERFACE_TABLE_SYSTEM_UUID    "SYSTEM_UUID"
#define SYSTEM_INTERFACE_TABLE_DEV_NAME       "DEV_NAME"
#define SYSTEM_INTERFACE_TABLE_DEV_TYPE       "DEV_TYPE"
#define SYSTEM_INTERFACE_TABLE_DEV_UUID        "DEV_UUID"

/****************设备类型定义*************************/
#define SYSTEM_INTERFACE_TABLE_UDP    "UDP"
#define SYSTEM_INTERFACE_TABLE_TCP    "TCP"
#define SYSTEM_INTERFACE_TABLE_TCP_SERVER    "TCP_SERVER"
#define SYSTEM_INTERFACE_TABLE_1553B  "1553B"
#define SYSTEM_INTERFACE_TABLE_UART   "UART"

class PARAMSTABLESHARED_EXPORT sysInterfaceTable
{
public:
    static sysInterfaceTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new sysInterfaceTable();
        }
        return mInstance;
    }

    ~sysInterfaceTable();
public:
    Json::Value getValue(std::string uuid);    

    /**
     * @brief getValueBySysUuid 通过系统的UUID获取值
     * @param uuid  系统UUID
     * @return
     */
    Json::Value getValueBySysUuid(const std::string &sysUuid);
private:
    sysInterfaceTable();
private:
    const QString mTableName = "system_interface_table";
    QSqlDatabase mDb;
private:
    static sysInterfaceTable *mInstance;
};

#endif // SYSTEMTABLE_H
