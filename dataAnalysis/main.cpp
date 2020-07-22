#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <thread>
#include <memory>
#include "../src/PfSql/paramsTable/parseResult/resulttable.h"
#include "../src/PfSql/paramsTable/parseResult/createresultdb.h"

#include "./argSave/databasemanager.h"
#include "./project/projectcfg.h"

#include <QSettings>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    

    qRegisterMetaType<shared_ptr<analysisRule>>("shared_ptr<analysisRule>");
    qRegisterMetaType<shared_ptr<filterManager>>("shared_ptr<filterManager>");

    MainWindow w;
    w.show();

    return a.exec();
}
