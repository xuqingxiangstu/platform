#include "systemtable.h"

#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>

#include "../../PfCommon/tools/ut_error.h"
#include "sqltable.h"

systemTable *systemTable::mInstance = nullptr;

systemTable::systemTable()
{
    mDb = sqlTable::getInstance()->getDataBase();
}

bool systemTable::getSystemDev(QString systemName,Json::Value &value)
{
    bool result =true;
    QString sql=QString("SELECT * from system_table WHERE SYSTEM_NAME='%1'").arg(systemName);
    QSqlQuery query(sql,mDb);
    while(query.next())
    {
       QSqlRecord rec = query.record();
       QString Describe,DevName;
       Json::Value Item;
       Item.clear();
       rec = query.record();
       if(QString::compare(rec.value("DEV_TYPE").toString(),"nothing"))
       {
           Item["DEV_TYPE"]=rec.value("DEV_TYPE").toString().toStdString();
           result &= getDevDescribe(rec,Describe,DevName);
           Item["UUID"]=rec.value("DEV_UUID").toString().toStdString();
           Item["DEV_NAME"]=DevName.toStdString();
           Item["DESCRIBE"]=Describe.toStdString();
           value.append(Item);
       }
    }
    return result;
}

QString systemTable::getDevTableName(QString Tabletype)
{
    QString TableName="";
    if(!QString::compare(Tabletype, "UDP"))
    {
        TableName="udp_table";
    }
    else if(!QString::compare(Tabletype, "UART"))
    {
        TableName="uart_table";
    }
    else if(!QString::compare(Tabletype, "1553B"))
    {
        TableName="table_1553B";
    }
    return TableName;
}

bool systemTable::getDevDescribe(QSqlRecord systemRec,QString &Describe,QString &DevName)
 {
    bool result=false;
     QString sql = "SELECT * from ";
     QString qstTableName=getDevTableName(systemRec.value("DEV_TYPE").toString());
     sql +=qstTableName+" WHERE UUID="+QString("'%1'").arg(systemRec.value("DEV_UUID").toString());
     QSqlQuery query(sql, mDb);
     if(query.next())
     {
         QSqlRecord rec = query.record();
         DevName = rec.value("DEV_NAME").toString();
         if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "UDP"))
         {
           rec = query.record();
           Describe = rec.value("IP_ADDR").toString()+" : "+rec.value("PORT").toString();
         }
         else if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "UART"))
         {
             rec = query.record();
             Describe = rec.value("PORT_NUM").toString();
             Describe += " : "+rec.value("DATA_BIT").toString();
             Describe += " : "+rec.value("STOP_BIT").toString();
             Describe += " : "+rec.value("PARITY_BIT").toString();
         }
         else if(!QString::compare(systemRec.value("DEV_TYPE").toString(), "1553B"))
         {
              Describe = "Card"+rec.value("CARD_NUM").toString();
              Describe += " : "+rec.value("MODE").toString();
              if(!QString::compare(rec.value("MODE").toString(), "RT"))
              {
                  Describe += rec.value("RT_ADDR").toString();
              }
         }
         result=true;
     }
     return result;
 }

Json::Value systemTable::getSysInfoByUuid(const std::string &uuid)
{
    Json::Value value;
    bool Ok = false;
    QString sql = "SELECT * from " + mTableName + " where " + SYSTEM_TABLE_UUID + "=\"";
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

Json::Value systemTable::getSysGroups()
{
    Json::Value value;
    bool Ok = false;

    QString sql = "SELECT * from " + mTableName + " WHERE " + QString(SYSTEM_TABLE_UUID);
    sql += " in( SELECT min(" + QString(SYSTEM_TABLE_UUID) + ") from " + mTableName + " WHERE 1=1 group by " + QString(SYSTEM_TABLE_SYSTEM_NAME) + ")";

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
