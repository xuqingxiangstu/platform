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

QMutex paramsTable::mInstanceMutex;
std::shared_ptr<paramsTable> paramsTable::mInstance = nullptr;

paramsTable::paramsTable()
{
    mDb = sqlTable::getInstance()->getDataBase();


#if USE_MEMORY_QUERY
    //初始化时读取所有数据
    readAll();
#endif
}

paramsTable::~paramsTable()
{
#if USE_MEMORY_QUERY
    for(auto itor = mParamsMem.begin(); itor != mParamsMem.end(); )
    {
        itor.value().reset();

        mParamsMem.erase(itor++);
    }
#endif
}

void paramsTable::readAll()
{
    mMutex.lock();

    QString sql = "SELECT * FROM params_table";

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        Json::Value item;
        rec = query.record();

        QString table = "";
        int coding = 0;

        for(int index =0; index<rec.count();index++)
        {
            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toInt();
            }
            else
            {
                item[rec.fieldName(index).toStdString()] = rec.value(index).toString().toStdString();
            }

            if(rec.fieldName(index).compare(PARAM_TABLE_TABLE_NUM) == 0)
            {
                table = rec.value(index).toString();
            }
            else if(rec.fieldName(index).compare(PARAM_TABLE_CODING_NUM) == 0)
            {
                coding = rec.value(index).toInt();
            }
        }

        if(mParamsMem.contains(table))
        {
            std::shared_ptr<QMap<int, Json::Value>> tmp = mParamsMem.value(table);
            tmp->insert(coding, item);

            //mParamsMem.insert(table, tmp);
        }
        else
        {
            std::shared_ptr<QMap<int, Json::Value>> tmp = std::make_shared<QMap<int, Json::Value>>();
            tmp->insert(coding, item);
            mParamsMem.insert(table, tmp);
        }
    }

    mMutex.unlock();
}

bool paramsTable::getValue(const QString &tableNum, const QString &coding, Json::Value &value)
{
    bool Ok;
    return getValue(tableNum, coding.toInt(&Ok, 10), value);
}

bool paramsTable::getValueFrameParamSys(Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

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

    mMutex.unlock();

    return value.isNull();
}

bool paramsTable::getValueFrameParamCMD(QString system,Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

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

    mMutex.unlock();

    return value.isNull();
}
bool paramsTable::getValueFrameParamGroup(QString system, QString cmdType, Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

    QString sql  = "SELECT * FROM params_table WHERE CMD_TYPE ="+QString("'%1'").arg(cmdType);
            sql += "and SYSTEM = " + QString("'%1'").arg(system);
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

    mMutex.unlock();

    return value.isNull();
}

bool paramsTable::getValueFrameParamPar(QString system, QString cmdType, QString group, Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

    QString sql  = "SELECT * FROM params_table WHERE GROUP_NAME ="+QString("'%1'").arg(group);
    sql += "and SYSTEM = " + QString("'%1'").arg(system);
    sql += "and CMD_TYPE = " + QString("'%1'").arg(cmdType);
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

    mMutex.unlock();

    return value.isNull();
}

bool paramsTable::getValue(const QString &tableNum, int coding, Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

#if USE_MEMORY_QUERY
    if(mParamsMem.contains(tableNum))
    {
        if(mParamsMem.value(tableNum)->contains(coding))
        {
            value = mParamsMem.value(tableNum)->value(coding);

            Ok = true;
        }
    }
#else
    QString sql = "SELECT * from params_table where params_table.TABLE_NUM='";
    sql += tableNum;
    sql += "' and params_table.CODING_NUM=";
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
#endif
    mMutex.unlock();

    return !value.isNull();
}

bool paramsTable::getStateValues(Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

    QString sql = "SELECT * from " + mTableName + " where ";
    sql += PARAM_TABLE_CMD_TYPE;
    sql += "='";
    sql += CMD_TYPE_STATE;
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

    mMutex.unlock();

    return !value.isNull();
}


bool paramsTable::getParamValues(Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

    QString sql = "SELECT * from " + mTableName + " where ";
    sql += PARAM_TABLE_CMD_TYPE;
    sql += "='";
    sql += CMD_TYPE_DATA;
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

    mMutex.unlock();

    return !value.isNull();
}

bool paramsTable::getCmdValues(Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

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

    mMutex.unlock();

    return !value.isNull();
}

bool paramsTable::getValues(const std::string &tableNum, Json::Value &value)
{
    return getValues(std::atoi(tableNum.c_str()), value);
}

bool paramsTable::getValues(int tableNum, Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();
#if USE_MEMORY_QUERY
    if(mParamsMem.contains(QString::number(tableNum, 10)))
    {
        auto map = mParamsMem.value(QString::number(tableNum, 10));

        for(int key : map->keys())
        {
            value.append(map->value(key));
        }

        Ok = true;
    }
#else
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
#endif
    mMutex.unlock();

    return !value.isNull();
}

bool paramsTable::getValues(unsigned int tableNum, Json::Value &value)
{
    bool Ok = false;

    mMutex.lock();

#if USE_MEMORY_QUERY
    if(mParamsMem.contains(QString::number(tableNum, 16)))
    {
        auto map = mParamsMem.value(QString::number(tableNum, 16));

        for(int key : map->keys())
        {
            value.append(map->value(key));
        }

        Ok = true;
    }
#else
    QString sql = "SELECT * from params_table where params_table.TABLE_NUM='";
    sql += QString::number(tableNum, 16);
    sql += "' ORDER BY params_table.\"INDEX\" ASC";

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
#endif
    mMutex.unlock();

    return !value.isNull();
}
