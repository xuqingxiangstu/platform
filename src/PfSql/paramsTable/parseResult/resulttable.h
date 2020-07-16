#ifndef RESULTTABLE_H
#define RESULTTABLE_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <mutex>
#include <QMutex>
#include <memory>
#include <QReadWriteLock>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "../paramstable_global.h"

/****************表字段定义*************************/

#define RESULT_TABLE_TABLE_NUM      "TABLE_NUM"
#define RESULT_TABLE_CODING_NUM     "CODING_NUM"
#define RESULT_TABLE_TIME           "TIME"
#define RESULT_TABLE_HEX_VALUE      "HEX_VALUE"
#define RESULT_TABLE_PARSE_VALUE    "PARSE_VALUE"
#define RESULT_TABLE_MEAN           "MEAN"
#define RESULT_TABLE_IS_OVER        "IS_OVER"
#define RESULT_TABLE_NAME           "NAME"

/**
 * @brief The resultTable class 解析结果表
 */
class PARAMSTABLESHARED_EXPORT resultTable
{
public:
    resultTable(QString dbPath, QString uuid);
    ~resultTable();
public:
    /**
     * @brief insertMultiValue  插入多组数据(采用事务方式)
     * @param values            数据
     * @par 示例
     * @code
     * QJsonArray values;
     *   for(int i = 0; i < 500; i++)
     *   {
     *       QJsonObject obj;
     *       obj.insert(RESULT_TABLE_TABLE_NUM, "30000");
     *       obj.insert(RESULT_TABLE_CODING_NUM, i);
     *       obj.insert(RESULT_TABLE_HEX_VALUE, "0001");
     *       obj.insert(RESULT_TABLE_MEAN, "正常");
     *       obj.insert(RESULT_TABLE_PARSE_VALUE, "1");
     *       obj.insert(RESULT_TABLE_TIME, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z"));
     *       obj.insert(RESULT_TABLE_IS_OVER, 0);

     *       values.append(obj);
     *   }

     *   table->insertMultiValue(values);
     * @endcode
     */
    void insertMultiValue(const QJsonArray &values);

    /**
     * @brief clearTable    清空表数据
     */
    void clearTable();

    QSqlDatabase getDataBase(){return mDb;}
public:
    //bool queryAfterContextFromTime(const QString &user, const QDateTime &time, const int &count, QJsonArray &values);
private:
    bool query(const QString &sql, QJsonArray &values);
private:
    const QString mTableName = "result";
    QReadWriteLock mDataMutex;
    QSqlDatabase mDb;
};

#endif // RESULTTABLE_H
