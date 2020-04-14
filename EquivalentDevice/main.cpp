//#include "mainwindow.h"
#include <QApplication>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <regex>
#include <iostream>
#include <QEventLoop>
#include <QThread>
#include <QProcess>
#include <QSharedMemory>
#include "mainwindow.h"
#include "login.h"

#include "./XML/xmlconfig.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.unLoading();
    return a.exec();

    /*
     *  m_simTestArr.toStyledString();
        std::string out = Json::FastWriter().write(m_simTestArr);
        qDebug()<<QString::fromStdString(out);
        */
}
