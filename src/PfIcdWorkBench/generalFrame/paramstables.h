#ifndef PARAMSTABLES_H
#define PARAMSTABLES_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

#define TABLE_NUM "TABLE_NUM"
#define CODING_NUM	"CODING_NUM"
#define FRAME_TYPE "FRAME_TYPE"
#define INFO_WORD_TYPE "INFO_WORD_TYPE"
#define PARAM_TYPE "PARAM_TYPE"
#define PARAM_NAME "PARAM_NAME"
#define PARAM_BYTE_START_POS "PARAM_BYTE_START_POS"
#define PARAM_BYTE_SIZE "PARAM_BYTE_SIZE"
#define PARAM_BIT_START_POS "PARAM_BIT_START_POS"
#define PARAM_BIT_SIZE "PARAM_BIT_SIZE"
#define L_B_ENDIAN	"L_B_ENDIAN"
#define INIT_VALUE "INIT_VALUE"
#define DATA_TYPE "DATA_TYPE"
#define SYSTEM "SYSTEM"
#define VALUE_MEAN "VALUE_MEAN"

class paramsTables
{
public:
    static paramsTables *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new paramsTables();
        }
        return mInstance;
    }

    ~paramsTables();
public:
    /**
     * @brief getValues 通过表号编码获取相应参数信息
     * @param[in] tableNum 表号
     * @param[in] coding 编码
     * @param[out] value Json结果
     * {
     *  "TABLE_NUM":xxx,
     *  "CODING":xxx,
     *      ...
     * }
     * @return
     */
    bool getValue(int tableNum, int coding, Json::Value &value);

    /**
     * @brief getValues 通过表号获取相应参数信息多组(已排序)
     * @param[in] tableNum 表号
     * @param[out] value Json结果
     * {
     *      [
     *          {
     *              "TABLE_NUM":xxx,
     *              "CODING":xxx,
     *              ...
     *          },
     *          {
     *              "TABLE_NUM":xxx,
     *              "CODING":xxx,
     *              ...
     *          }
     *      ]
     * }
     * @return
     */
    bool getValues(int tableNum, Json::Value &value);
private:
    paramsTables();
private:
    const QString mTableName = "params_table";
    QSqlDatabase mDb;
private:
    static paramsTables *mInstance;
};

#endif // PARAMSTABLES_H
