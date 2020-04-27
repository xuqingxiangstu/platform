#ifndef M1553BTABLE_H
#define M1553BTABLE_H

#include "paramstable_global.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/
#define M1553B_TABLE_UUID       "UUID"
#define M1553B_TABLE_CARD_NUM   "CARD_NUM"
#define M1553B_TABLE_MODE       "MODE"
#define M1553B_TABLE_RESERVE    "RESERVE"

class PARAMSTABLESHARED_EXPORT m1553bTable
{
public:
    static m1553bTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new m1553bTable();
        }
        return mInstance;
    }

    ~m1553bTable();
public:
    Json::Value getValue(std::string uuid);
private:
    m1553bTable();
private:
    const QString mTableName = "m1553B_table";
    QSqlDatabase mDb;
private:
    static m1553bTable *mInstance;
};


#endif // M1553BTABLE_H
