#include "paramstables.h"
#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>

#include "../../PfCommon/tools/ut_error.h"

paramsTables *paramsTables::mInstance = nullptr;

paramsTables::paramsTables()
{
    mDb = QSqlDatabase::addDatabase( "QSQLITE" );

    QString filePath = QDir::currentPath() + "/net_ptl_eq.db";

    QFileInfo info(filePath);

    if(!info.exists(filePath))
    {
        UT_THROW_EXCEPTION("open error" + filePath.toStdString());
    }

    mDb.setDatabaseName(filePath);

    if(!mDb.open() )
    {
        UT_THROW_EXCEPTION(mDb.lastError().text().toStdString());
    }
}

paramsTables::~paramsTables()
{

}

bool paramsTables::getValue(int tableNum, int coding, Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT * from params_table where params_table.TABLE_NUM=";
    sql += QString::number(tableNum, 16);
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

    return value.isNull();
}


bool paramsTables::getValues(int tableNum, Json::Value &value)
{
    bool Ok = false;
    QString sql = "SELECT * from params_table where params_table.TABLE_NUM=";
    sql += QString::number(tableNum, 16);
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

    return value.isNull();
}
