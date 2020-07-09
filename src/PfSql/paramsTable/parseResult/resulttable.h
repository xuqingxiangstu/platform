#ifndef RESULTTABLE_H
#define RESULTTABLE_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <mutex>
#include <QMutex>
#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/

#define RESULT_TABLE_TABLE_NUM      "TABLE_NUM"
#define RESULT_TABLE_CODING_NUM     "CODING_NUM"
#define RESULT_TABLE_TIME           "TIME"
#define RESULT_TABLE_HEX_VALUE      "HEX_VALUE"
#define RESULT_TABLE_PARSE_VALUE    "PARSE_VALUE"
#define RESULT_TABLE_MEAN           "MEAN"
#define RESULT_TABLE_IS_OVER        "IS_OVER"

/**
 * @brief The resultTable class 解析结果表
 */
class resultTable
{
public:
    resultTable();
};

#endif // RESULTTABLE_H
