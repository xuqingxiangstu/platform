#include "tcpservertable.h"

#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>

#include "../../PfCommon/tools/ut_error.h"
#include "sqltable.h"

tcpServerTable *tcpServerTable::mInstance = nullptr;

tcpServerTable::tcpServerTable()
{
    mDb = sqlTable::getInstance()->getDataBase();
}

Json::Value tcpServerTable::getValue(std::string uuid)
{
    Json::Value value;
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where " + UDP_TABLE_UUID + "=\"";
    sql += QString::fromStdString(uuid);
    sql += "\"";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();
        //Json::Value tmpV;
        for(int index = 0; index < rec.count(); index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                value[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            //else if(QVariant::String == type)
            else
            {
                value[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        //value.append(tmpV);
    }

    return value;
}
