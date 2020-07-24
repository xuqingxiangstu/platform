#include "resulttable.h"

#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QVariant>
#include <QSqlField>
#include <QDateTime>

resultTable::resultTable(QString dbPath, QString uuid)
{
    if(QSqlDatabase::contains(uuid))
    {
        mDb = QSqlDatabase::database(uuid);
    }
    else
    {
        mDb = QSqlDatabase::addDatabase("QSQLITE", uuid);

        mUuid = uuid;

        QString filePath = dbPath;

        QFileInfo info(filePath);

        if(!info.exists(filePath))
        {
            throw std::runtime_error("open error" + filePath.toStdString());
        }

        mDb.setDatabaseName(filePath);

        if(!mDb.open() )
        {
            throw std::runtime_error(mDb.lastError().text().toStdString());
        }
    }

}

resultTable::~resultTable()
{
#if 0
    if(mDb.isOpen())
    {
        mDb.close();
    }

    if(QSqlDatabase::contains(mUuid))
        QSqlDatabase::removeDatabase(mUuid);
#endif
}

void resultTable::clearTable()
{
    QString sql = "delete from " + mTableName;

    mDataMutex.lockForWrite();

    QSqlQuery query(sql, mDb);

    mDataMutex.unlock();
}

void resultTable::insertMultiValue(const QJsonArray &values)
{
    mDataMutex.lockForWrite();

    //开启事务
    mDb.transaction();

    for(int index = 0; index < values.size(); index++)
    {
        QJsonObject obj = values.at(index).toObject();

        QString value = "";

        value += "'";
        value += obj.value(RESULT_TABLE_TABLE_NUM).toString();
        value += "'";
        value += ",";

        value += QString::number(obj.value(RESULT_TABLE_CODING_NUM).toInt());
        value += ",";

        value += "'";
        value += obj.value(RESULT_TABLE_NAME).toString();
        value += "'";
        value += ",";

        value += "'";
        value += obj.value(RESULT_TABLE_TIME).toString();
        value += "'";
        value += ",";

        value += "'";
        value += obj.value(RESULT_TABLE_HEX_VALUE).toString();
        value += "'";
        value += ",";

        value += "'";
        value += obj.value(RESULT_TABLE_PARSE_VALUE).toString();
        value += "'";
        value += ",";

        value += "'";
        value += obj.value(RESULT_TABLE_MEAN).toString();
        value += "'";
        value += ",";

        value += QString::number(obj.value(RESULT_TABLE_IS_OVER).toInt());

        QString sql = "insert into " + mTableName + "("
                    + RESULT_TABLE_TABLE_NUM +
                    ", " + RESULT_TABLE_CODING_NUM
                    + ", " + RESULT_TABLE_NAME
                    + ", " + RESULT_TABLE_TIME
                    + "," + RESULT_TABLE_HEX_VALUE
                    + "," + RESULT_TABLE_PARSE_VALUE
                    + "," + RESULT_TABLE_MEAN
                    + "," + RESULT_TABLE_IS_OVER
                    + ") values("+value+ ")";

        QSqlQuery query(mDb);
        bool result = query.exec(sql);
        if(!result)
        {
            qDebug() << mDb.lastError();
        }
    }

    //提交
    mDb.commit();    

    mDataMutex.unlock();
}

bool resultTable::query(const QString &sql, QJsonArray &values)
{
    mDataMutex.lockForRead();

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        QJsonObject item;
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {

            QVariant::Type type = query.record().field(index).type();
            if(QVariant::Int == type)
            {
                item[rec.fieldName(index)] = rec.value(index).toInt();
            }
            else
            {
                item[rec.fieldName(index)] = rec.value(index).toString();
            }
        }
        values.append(item);
    }

    return !values.isEmpty();
}

bool resultTable::getTableGroupName(QJsonArray &values)
{
    QString sql  = "SELECT ";
            sql += RESULT_TABLE_TABLE_NUM;
            sql += " FROM ";
            sql += mTableName;
            sql += " GROUP BY ";
            sql += RESULT_TABLE_TABLE_NUM;

    return query(sql, values);
}

bool resultTable::getCodingByTableName(const QString &table, QJsonArray &values)
{
    QString sql  = "SELECT * ";
            sql += " FROM ";
            sql += mTableName;
            sql += " WHERE ";
            sql += RESULT_TABLE_TABLE_NUM;
            sql += " = ";
            sql += table;
            sql += "";
            sql += " GROUP BY ";
            sql += RESULT_TABLE_CODING_NUM;
            sql += " ORDER BY ";
            sql += RESULT_TABLE_CODING_NUM;

    return query(sql, values);
}

bool resultTable::getTimeByTableCoding(const QString &name, const QString &coding, QJsonArray &values)
{
    QString sql  = "SELECT * ";
            sql += " FROM ";
            sql += mTableName;
            sql += " WHERE ";
            sql += RESULT_TABLE_TABLE_NUM;
            sql += " = ";
            sql += name;
            sql += " AND ";
            sql += RESULT_TABLE_CODING_NUM;
            sql += " = ";
            sql += coding;
            sql += "";
            sql += " ORDER BY ";
            sql += RESULT_TABLE_TIME;

    return query(sql, values);
}

bool resultTable::getTimeList(QJsonArray &values)
{
    QString sql  = "SELECT ";
            sql += RESULT_TABLE_TIME;
            sql += " FROM ";
            sql += mTableName;
            sql += " GROUP BY ";
            sql += RESULT_TABLE_TIME;
            sql += " ORDER BY ";
            sql += RESULT_TABLE_TIME;

    return query(sql, values);
}

bool resultTable::getValueList(QJsonArray &values)
{
    QString sql  = "SELECT ";
            sql += RESULT_TABLE_PARSE_VALUE;
            sql += " FROM ";
            sql += mTableName;
            sql += " ORDER BY ";
            sql += RESULT_TABLE_PARSE_VALUE;

    return query(sql, values);
}

bool resultTable::getTableNameByTime(const QString &time, const QString &name, QJsonArray &values)
{
    QString sql  = "SELECT * ";
            sql += " FROM ";
            sql += mTableName;
            sql += " WHERE ";
            sql += RESULT_TABLE_TIME;
            sql += " = \"" ;
            sql += time;
            sql += "\" AND ";
            sql += RESULT_TABLE_TABLE_NUM;
            sql += " = ";
            sql += name;
            sql += "";
            sql += " ORDER BY ";
            sql += RESULT_TABLE_TABLE_NUM;

    return query(sql, values);
}

bool resultTable::getCodingByNameAndTime(const QString &time, const QString &name, QJsonArray &values)
{
    QString sql  = "SELECT * ";
            sql += " FROM ";
            sql += mTableName;
            sql += " WHERE ";
            sql += RESULT_TABLE_TIME;
            sql += " = \"" ;
            sql += time;
            sql += "\" AND ";
            sql += RESULT_TABLE_TABLE_NUM;
            sql += " = ";
            sql += name;
            sql += "";
            sql += " ORDER BY ";
            sql += RESULT_TABLE_CODING_NUM;

    return query(sql, values);
}
