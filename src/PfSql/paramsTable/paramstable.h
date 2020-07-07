#ifndef PARAMSTABLE_H
#define PARAMSTABLE_H

#include "paramstable_global.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <mutex>
#include <QMutex>
#include <memory>

#include "../../PfCommon/jsoncpp/json.h"

/****************表字段定义*************************/

#define PARAM_TABLE_TABLE_NUM "TABLE_NUM"
#define PARAM_TABLE_CODING_NUM	"CODING_NUM"
#define PARAM_TABLE_FRAME_TYPE "FRAME_TYPE"
#define PARAM_TABLE_INFO_WORD_TYPE "INFO_WORD_TYPE"
#define PARAM_TABLE_IS_ACK  "IS_ACK"
#define PARAM_TABLE_GROUP_NAME "GROUP_NAME"
#define PARAM_TABLE_CMD_TYPE "CMD_TYPE"
#define PARAM_TABLE_PARAM_NAME "PARAM_NAME"
#define PARAM_TABLE_PARAM_BYTE_START_POS "PARAM_BYTE_START_POS"
#define PARAM_TABLE_PARAM_BYTE_SIZE "PARAM_BYTE_SIZE"
#define PARAM_TABLE_PARAM_BIT_START_POS "PARAM_BIT_START_POS"
#define PARAM_TABLE_PARAM_BIT_SIZE "PARAM_BIT_SIZE"
#define PARAM_TABLE_L_B_ENDIAN	"L_B_ENDIAN"
#define PARAM_TABLE_INIT_VALUE "INIT_VALUE"
#define PARAM_TABLE_DATA_TYPE "DATA_TYPE"
#define PARAM_TABLE_SYSTEM "SYSTEM"
#define PARAM_TABLE_VALUE_MEAN "VALUE_MEAN"
#define PARAM_TABLE_MIN_VALUE   "MIN_VALUE"
#define PARAM_TABLE_MAX_VALUE   "MAX_VALUE"

/***************数据类型***************************/
#define DATA_TYPE_NRAW      "NRAW"
#define DATA_TYPE_NCHAR     "NCHAR"
#define DATA_TYPE_UINT8     "UINT8"
#define DATA_TYPE_UINT16    "UINT16"
#define DATA_TYPE_UINT32    "UINT32"
#define DATA_TYPE_IEEE32    "IEEE32"
#define DATA_TYPE_IEEE64    "IEEE64"


/****************指令类型定义********************/
#define CMD_TYPE_CMD    "指令"
#define CMD_TYPE_STATE  "状态量"
#define CMD_TYPE_DATA   "数值量"
#define CMD_CHAR_TYPE   "字符串"

/*****************是否应答定义*****************************/
#define IS_ACK_YES  "是"
#define IS_ACK_NO   "否"

class PARAMSTABLESHARED_EXPORT paramsTable
{
public:
    static paramsTable *getInstance(){
        if(mInstance == nullptr)
        {
            mInstance = new paramsTable();
        }
        return mInstance;
    }

    ~paramsTable();
public:

    bool getStateValues(Json::Value &value);

    bool getParamValues(Json::Value &value);

    bool getCmdValues(Json::Value &value);

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
    bool getValue(const QString &tableNum, int coding, Json::Value &value);

    bool getValue(const QString &tableNum, const QString &coding, Json::Value &value);

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

    bool getValues(unsigned int tableNum, Json::Value &value);

    bool getValues(const std::string &tableNum, Json::Value &value);

    /**
    * @brief getValueFrameParamSys 获取系统分类
    * @param value
    * @return
    */
    bool getValueFrameParamSys(Json::Value &value);

    /**
           * @brief getValueFrameParamCMD获取指令分类
           * @param system
           * @param value
           * @return
           */
          bool getValueFrameParamCMD(QString system, Json::Value &value);
          /**
           * @brief getValueFrameParamGroup 获取表分类
           * @param cmdType
           * @param value
           * @return
           */
          bool getValueFrameParamGroup(QString system, QString cmdType, Json::Value &value);
          /**
           * @brief getValueFrameParamPar
           * @param group
           * @param value
           * @return
           */
          bool getValueFrameParamPar(QString system,QString cmdType, QString group, Json::Value &value);
private:
    paramsTable();
private:
    const QString mTableName = "params_table";
    QSqlDatabase mDb;
    QMutex mMutex;
private:
    static paramsTable *mInstance;
};

#endif // PARAMSTABLE_H
