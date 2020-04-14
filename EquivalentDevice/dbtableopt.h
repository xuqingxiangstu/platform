#ifndef DBTABLEOPT_H
#define DBTABLEOPT_H
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QUuid>
#include <QString>
#include "./include/PfCommon/jsoncpp/json.h"
class DBTableOpt
{
public:
    DBTableOpt();
    static DBTableOpt *getInstance(){
        if(mInstance==nullptr)
        {
            mInstance = new DBTableOpt();
        }
        return mInstance;
    }
    ~DBTableOpt();
    /**
     * @brief 获取所有分系统的信息
     * @param[out] value Json结果
     * {
     *  [
     *      {
     *      "UUID":xxx,
     *      "SYSTEM_NAME":xxx},
     *      {
     *          //同上
     *      },
     *      ...
     *   ]
     * }
     * @return
     */
    bool getValueAllSubSystemInfo(Json::Value &value);
    /**
     * @brief getSystemDev获取该系的设备
     * @param uuid 该系的uuid
     * @param value
     * {
     *      [
     *          {
     *          "UUID":
     *          "DEV_NAME":
     *          "DEV_TYPE":
     *          "DESCRIBE":
     *          },
     *          {
     *          "UUID":
     *          "DEV_NAME":
     *          "DEV_TYPE":
     *          "DESCRIBE":
     *          },
     *      ]
     * }
     * @return
     */
    bool getSystemDev(QString systemUuid, Json::Value &value);
    /**
     * @brief deleteSubSystem 删除某个子系统
     * @return
     */
    bool deleteSubSystem(QString SystemUuid);
    /**
      *@brief addSubSystem 增加一个分系统
      *@QString SysteName 分系统名
      *@QString Note 备注
      */
      bool addSubSystem(std::vector<QString> &sysArr, QString &sysUuid, QString Note="");
      /**
        * @brief addSystemDev 给系统增加一个硬件设备
        * @param value 设备的描述Json文件
        *
        * @brief addSubSystem增加子系统
        * @param[in] value
        * {
        *    "SYSTEM_NAME":xxx
        *    "NOTE":xxx
        *    "DEV_TYPE":xxx //UART,UDP,1553B
        *    "DEVINFO":{
        *
        *    }
        * DEVINFO目前有三中结构
        * 1.UART
        *   "DEVINFO":{
        *        "DEV_NAME":xxx,
        *        "PORT_NUM":xxx,
        *      "BAUD_RATE":xxx,
        *        "DATA_BIT":xxx,
        *        "STOP_BIT":xxx,
        *        "PARITY_BIT":xxx,
        *         "RESERVE":xxx
        *    }
        * }
        * 2.UDP
        *   "DEVINFO":{
        *        "DEV_NAME":xxx,
        *        "IP_ADDR":xxx,
        *      "PORT":xxx,
        *        "RESERVE":xxx,
        *    }
        * 3.1553B
        * "DEVINFO":{
        *    "DEV_NAME":xxx,
        *    "CARD_NUM":xxx
        *    "MODE":xxx
        *    "RT_ADDR":xxx
        *   }
        * }
       * }
       * @param uuid设备的uuid
       * @return
       */
      bool addSystemDev(Json::Value value,QString &uuid);
      /**
       * @brief deleteSystemDev
       * @param uuid设备的uuid删除系统的某个设备
       * @return
       */
      bool deleteSystemDev(QString uuid);
      /**
       * @brief getValueAllFlowRecord 获取SystemUUID主键的所有流程记录
       * @param[in] SystemUUid 分系统表的UUID
       * @param[out] value
       * {
       *    [
       *        {"FLOW_NAME":xxx,
       *         "CREAT_OPEN_TIME":xxx,
       *         "RECENT_OPEN_TIME"xxx,
       *         "NOTE"xxx,
       *        },
       *        {
       *            ...同上
       *        },...
       *    ]
       * }
       * @return
       */
      bool getValueAllFlowRecord(QString SystemUUID, Json::Value &value);
      /**
       * @brief addSubSystemFlow
       * @param[in] value
       * {
       *    "SYSTEM_UUID":xxx,
       *    "FLOW_NAME":xxx,
       *    "CREATE_TIME"xxx,
       *    "RECENT_OPEN_TIME":xxx,
       *    "NOTE"
       * }
       * @return
       */
      bool addSubSystemFlow(Json::Value &value,QString &uuid);
      /**
       * @brief alterFlow更改流程表
       * @param uuid 流程的uuid
       * @param Name 要修改的字段名称
       * @param Content 要修改的内容
       * @return
       */
      bool alterFlow(QString uuid,QString Name,QString Content);
      /**
       * @brief alterSubSystem 更改分系统名字
       * @param uuid
       * @param Name
       * @return
       */
      bool alterSubSystem(QString uuid, QString Name);
      /**
       * @brief deleteSubSystem 删除分系统的某个流程
       * @param flowName 流程名
       * @param CreateTime 创建时间
       * @return
       */
      bool deleteFlow(QString uuid);
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
      bool getValueFrameParamGroup(QString cmdType, Json::Value &value);
      /**
       * @brief getValueFrameParamPar
       * @param group
       * @param value
       * @return
       */
      bool getValueFrameParamPar(QString group, Json::Value &value);

      /**
       * @brief getValues 通过表号获取相应帧参数信息多组(已排序)
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
      bool getValuesFrameParam(int tableNum, Json::Value &value);
      bool    CreateXmlFile(QString Name,QString CreatTime);
      /**
       * @brief getRecTestNotes 获取最新的测试记录
       * @param[out] value Json结果
       * @return
       */
      bool getRecTestNotes(Json::Value &value);
      /**
       * @brief setRecTestNotes 保存最新的测试记录
       * @param[in] value Json结果
       * {
       *    "sysName":[
       *                xxx,
       *                xxx
       *               ],
       *    "recond_uuid":[
       *                xxx,
       *                xxx
       *                   ]
       * }
       * @return
       */
      bool setRecTestNotes(Json::Value &value);
      /**
       * @brief excelToDb 导入excel表到数据库
       * @param[in] msgBox excel一行的数据
       * @return
       */
      bool excelToDb(const std::vector<QString> &msgBox);
      /**
       * @brief dbToExcel 从数据库导出的excel
       * @param[out] msgBox
       * @return
       */
      bool dbToExcelTableNum(std::vector<QString> &msgBox);

      bool dbToExcel(QString tableNum,Json::Value &msgBox);
private:
      bool getDevDescribe(QSqlRecord rec,QString &Describe,QString &DevName);
      QString getDevTableName(QString TableTyep);
      bool    CheckDevTableRepeat(Json::Value &value);
private:
    QSqlDatabase mDb;
    static DBTableOpt *mInstance;
};

#endif // DBTABLEOPT_H
