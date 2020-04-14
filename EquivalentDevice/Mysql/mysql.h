#ifndef MYSQL_H
#define MYSQL_H

#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QUuid>
#include "mainwindow.h"
#include "../sysManage/addsysmanage.h"


class MySql
{

public:
    MySql();
    static void initsql();                     //初始化连接数据库
    static QSqlDatabase getDb(){return db;}
    //void createtable();            //建表 索引 触发器
    bool deletestu(QString man_sys);   //删除设备信息
    bool addsysManage(my_sys *stu);//查询故障信息

private:
    QSqlQuery *query;
    static QSqlDatabase db;
};

#endif // MYSQL_H
