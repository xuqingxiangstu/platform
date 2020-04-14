#include "mysql.h"

QSqlDatabase MySql::db;
MySql::MySql()
{

}

void MySql::initsql()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./mydb.db");
    if(db.open())
        {
            qDebug()<<"Database connected successfully!";
            //createtable();
            return;
        }
    else
        {
            qDebug()<<"Database connected failed!";
            return;
        }
}


bool MySql::deletestu(QString man_sys)
{
    query=new QSqlQuery;
    QString str=QString("delete from sysManage where UUid='%1'").arg(man_sys);
    bool ret=query->exec(str);
    return ret;
}

bool MySql::addsysManage(my_sys *stu)
{
    query=new QSqlQuery(db);

    QString mansys = stu->uuid;

    mansys.remove(0,1);
    mansys.remove(mansys.length() - 1, 1);

    //QString str=QString("IF NOT EXISTS(SELECT * FROM sysManage WHERE ip = %3 AND port = %4 ) THEN INSERT INTO sysManage VALUES (\"%1\",\"%2\",\"%3\",\"%4\")").arg(mansys).arg(stu->sys).arg(stu->ip).arg(stu->port);
    QString str=QString("INSERT INTO sysManage VALUES (\"%1\",\"%2\",\"%3\",\"%4\")").arg(mansys).arg(stu->sys).arg(stu->ip).arg(stu->port);
    bool ret=query->exec(str);
    return ret;

}
