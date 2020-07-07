#include "flowrecordtable.h"

#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>
#include <QDateTime>

#include <QUuid>
#include "../../PfCommon/tools/ut_error.h"
#include "sqltable.h"

flowRecordTable *flowRecordTable::mInstance = nullptr;

flowRecordTable::flowRecordTable()
{
    mDb = sqlTable::getInstance()->getDataBase();
}

bool flowRecordTable::addRecord(const std::string &sysUuid, const std::string &flowName, const std::string &note, std::string &recordUuid)
{
    QString uuid = QUuid::createUuid().toString();

    recordUuid = uuid.toStdString();

    QString value;

    value += "'";
    value += uuid;
    value += "'";

    value += ",";
    value += "'";
    value += QString::fromStdString(sysUuid);
    value += "'";

    value += ",";
    value += "'";
    value += QString::fromStdString(flowName);
    value += "'";

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");

    value += ",";
    value += "'";
    value += time;
    value += "'";

    value += ",";
    value += "'";
    value += time;
    value += "'";

    value += ",";
    value += "'";
    value += QString::fromStdString(note);
    value += "'";

    QString sql = "insert into " + mTableName + "("
            + FLOW_RECORD_TABLE_UUID +
            ", " + FLOW_RECORD_TABLE_SYSTEM_UUID
            + ", " + FLOW_RECORD_TABLE_FLOW_NAME
            + "," + FLOW_RECORD_TABLE_CREATE_TIME
            + "," + FLOW_RECORD_TABLE_RECENT_OPEN_TIME
            + "," + FLOW_RECORD_TABLE_NOTE
            + ") values("+value+ ")";

    QSqlQuery query(mDb);
    bool result = query.exec(sql);
    if(!result)
    {
        qDebug() << mDb.lastError();
    }
    return result;
}

bool flowRecordTable::deleteRecordByUuid(const std::string &uuid)
{
    QString sql = "DELETE from " + mTableName + " WHERE " + FLOW_RECORD_TABLE_UUID + "=";
    sql += "'";
    sql += QString::fromStdString(uuid);
    sql += "'";

    QSqlQuery query(mDb);
    bool result = query.exec(sql);
    if(!result)
    {
        qDebug() << mDb.lastError();
    }
    return result;
}

Json::Value flowRecordTable::getValueBySystemUuid(const std::string &sysUuid)
{
    Json::Value value;
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where " + FLOW_RECORD_TABLE_SYSTEM_UUID + "=";
    sql += "'";
    sql += QString::fromStdString(sysUuid);
    sql += "'";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        Json::Value tmpJs;
        rec = query.record();
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

void flowRecordTable::updateNodeName(const std::string &uuid, const std::string &name)
{
    QString sql = "update " + mTableName + " set " + FLOW_RECORD_TABLE_FLOW_NAME + " = '" + name.c_str() + "' "
        + " where " + FLOW_RECORD_TABLE_UUID + " = ";
    sql += "'";
    sql += QString::fromStdString(uuid);
    sql += "'";
    QSqlQuery query(mDb);
    bool result = query.exec(sql);
    if(!result)
    {
        qDebug() << mDb.lastError();
    }
}

void flowRecordTable::updateNodeInfo(const std::string &uuid, const std::string &value)
{
    QString sql = "update " + mTableName + " set " + FLOW_RECORD_TABLE_NODE_INFO + " = '" + value.c_str() + "' "
        + " where " + FLOW_RECORD_TABLE_UUID + " = ";
    sql += "'";
    sql += QString::fromStdString(uuid);
    sql += "'";
    QSqlQuery query(mDb);
    bool result = query.exec(sql);
    if(!result)
    {
        qDebug() << mDb.lastError();
    }
}

Json::Value flowRecordTable::getValue(std::string uuid)
{
    Json::Value value;
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where " + FLOW_RECORD_TABLE_UUID + "=";
    sql += "'";
    sql += QString::fromStdString(uuid);
    sql += "'";

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
