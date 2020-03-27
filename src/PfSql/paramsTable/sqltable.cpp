#include "sqltable.h"

#include <QDir>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlRecord>
#include <QtSql/QSqlError>
#include <QSqlField>
#include <QDebug>

#include "../../PfCommon/tools/ut_error.h"

sqlTable *sqlTable::mInstance = nullptr;

sqlTable::sqlTable()
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

