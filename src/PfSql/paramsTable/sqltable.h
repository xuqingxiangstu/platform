#ifndef SQLTABLE_H
#define SQLTABLE_H

#include "paramstable_global.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

class PARAMSTABLESHARED_EXPORT sqlTable
{
public:
    static sqlTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new sqlTable();
        }
        return mInstance;
    }

    ~sqlTable();
public:
    QSqlDatabase getDataBase(){return mDb;}
private:
    sqlTable();
private:
    QSqlDatabase mDb;
private:
    static sqlTable *mInstance;
};

#endif // SQLTABLE_H
