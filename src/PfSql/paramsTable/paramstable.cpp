#include "paramstable.h"


#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>

#include "sqltable.h"
#include "../../PfCommon/tools/ut_error.h"

paramsTable *paramsTable::mInstance = nullptr;

paramsTable::paramsTable()
{
    mDb = sqlTable::getInstance()->getDataBase();
}

paramsTable::~paramsTable()
{

}

bool paramsTable::getValue(std::string tableNum, int coding, Json::Value &value)
{
    return getValue(std::atoi(tableNum.c_str()), coding, value);
}
bool paramsTable::getValue(std::string tableNum, std::string coding, Json::Value &value)
{
    return getValue(std::atoi(tableNum.c_str()), std::atoi(coding.c_str()), value);
}

bool paramsTable::getValueFrameParamSys(Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT SYSTEM FROM params_table GROUP BY SYSTEM";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        value.append(item);
    }
    return value.isNull();
}

bool paramsTable::getValueFrameParamCMD(QString system,Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT CMD_TYPE FROM params_table WHERE SYSTEM="+QString("'%1'").arg(system);
            sql +="GROUP BY CMD_TYPE";
            sql += " ORDER BY params_table.\"INDEX\" ASC";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
        }
        value.append(item);
    }
    return value.isNull();
}
bool paramsTable::getValueFrameParamGroup(QString cmdType, Json::Value &value)
{
    bool Ok = false;
    QString sql  = "SELECT * FROM params_table WHERE CMD_TYPE ="+QString("'%1'").arg(cmdType);
            sql += "GROUP BY GROUP_NAME";
            sql += " ORDER BY params_table.\"INDEX\" ASC";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {

            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(item);
    }
    return value.isNull();
}

bool paramsTable::getValueFrameParamPar(QString group, Json::Value &value)
{
    bool Ok = false;
    QString sql  = "SELECT * FROM params_table WHERE GROUP_NAME ="+QString("'%1'").arg(group);
    sql += " ORDER BY params_table.\"INDEX\" ASC";
    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        Json::Value item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(item);
    }
    return value.isNull();
}

bool paramsTable::getValue(int tableNum, int coding, Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT * from params_table where params_table.TABLE_NUM=";
    sql += QString::number(tableNum, 10);
    sql += " and params_table.CODING_NUM=";
    sql += QString::number(coding, 10);
    sql += " ORDER BY params_table.\"INDEX\" ASC";

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

    return !value.isNull();
}

bool paramsTable::getCmdValues(Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where ";
    sql += PARAM_TABLE_CMD_TYPE;
    sql += "='";
    sql += CMD_TYPE_CMD;
    sql += "'";
    sql += " ORDER BY 'INDEX' ASC";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();
        Json::Value tmpV;
        for(int index = 0; index < rec.count(); index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                tmpV[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            else
            {
                tmpV[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(tmpV);
    }
    return !value.isNull();
}

bool paramsTable::getValues(const std::string &tableNum, Json::Value &value)
{
    return getValues(std::atoi(tableNum.c_str()), value);
}

bool paramsTable::getValues(int tableNum, Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT * from params_table where params_table.TABLE_NUM=";
    sql += QString::number(tableNum, 10);
    sql += " ORDER BY params_table.\"INDEX\" ASC";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();
        Json::Value tmpV;
        for(int index = 0; index < rec.count(); index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                tmpV[rec.fieldName(index).toStdString()] = rec.value(index).toInt(&Ok);
            }
            //else if(QVariant::String == type)
            else
            {
                tmpV[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }
        }
        value.append(tmpV);
    }

    return !value.isNull();
}
