#include "sysinterfacetable.h"

#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>

#include "../../PfCommon/tools/ut_error.h"
#include "sqltable.h"

sysInterfaceTable *sysInterfaceTable::mInstance = nullptr;

sysInterfaceTable::sysInterfaceTable()
{
    mDb = sqlTable::getInstance()->getDataBase();
}

Json::Value sysInterfaceTable::getValueBySysUuid(const std::string &sysUuid)
{
    Json::Value value;
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where " + SYSTEM_INTERFACE_TABLE_SYSTEM_UUID + "=\"";
    sql += QString::fromStdString(sysUuid);
    sql += "\"";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();
        Json::Value tmpJs;

        for(int index = 0; index < rec.count(); index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                tmpJs[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                tmpJs[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }

        value.append(tmpJs);
    }

    return value;
}

Json::Value sysInterfaceTable::getValue(std::string uuid)
{
    Json::Value value;
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where " + SYSTEM_INTERFACE_TABLE_UUID + "=\"";
    sql += QString::fromStdString(uuid);
    sql += "\"";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();        
        for(int index = 0; index < rec.count(); index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                value[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }            
            else
            {
                value[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }        
    }

    return value;
}
