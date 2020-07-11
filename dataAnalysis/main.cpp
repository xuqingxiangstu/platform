#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <thread>
#include "../src/PfSql/paramsTable/parseResult/resulttable.h"
#include "../src/PfSql/paramsTable/parseResult/createresultdb.h"

#include "./argSave/databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 0
   // QString dbPath = createResultDb::getInstance()->create("test");


    //resultTable *mResultTable = new resultTable(dbPath);
    std::shared_ptr<resultTable> mResultTable = databaseManager::getInstance()->getDataBase("test");

    std::thread thread([=](){
    QJsonArray values;

    for(int i = 0; i < 500; i++)
    {
        QJsonObject obj;
        obj.insert(RESULT_TABLE_TABLE_NUM, "30000");
        obj.insert(RESULT_TABLE_CODING_NUM, i);
        obj.insert(RESULT_TABLE_HEX_VALUE, "0001");
        obj.insert(RESULT_TABLE_MEAN, "正常");
        obj.insert(RESULT_TABLE_PARSE_VALUE, "1");
        obj.insert(RESULT_TABLE_TIME, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.z"));
        obj.insert(RESULT_TABLE_IS_OVER, 0);

        values.append(obj);
    }

    mResultTable->insertMultiValue(values);
});
    thread.join();
    return 0;
#endif
    MainWindow w;
    w.show();

    return a.exec();
}
