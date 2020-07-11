#ifndef DATASHOWTABLE_H
#define DATASHOWTABLE_H

#include <QWidget>
#include <QSqlQueryModel>

#include "./argSave/databasemanager.h"

namespace Ui {
class dataShowTable;
}

class dataShowTable : public QWidget
{
    Q_OBJECT

/** 表头标题 **/
const QStringList mTableTitle = {
    "时间",
    "表号",
    "编码",
    "源码",
    "工程值",
    "含义",
    "判读"
};

/** 表头域名 SQL **/
const QStringList mTableFiledName = {
    RESULT_TABLE_TIME,
    RESULT_TABLE_TABLE_NUM,
    RESULT_TABLE_CODING_NUM,
    RESULT_TABLE_HEX_VALUE,
    RESULT_TABLE_PARSE_VALUE,
    RESULT_TABLE_MEAN,
    RESULT_TABLE_IS_OVER
};

public:
    explicit dataShowTable(QWidget *parent = 0);
    ~dataShowTable();
private:
    void initTable();
private:

    void conditionQuery(QString cond);

    void query(const QString &sql);
private:
    QSqlQueryModel *mDataModel;
    QSqlDatabase db;
    QString mSelectField;
    const QString mTableName = "result";
private:
    Ui::dataShowTable *ui;
};

#endif // DATASHOWTABLE_H
