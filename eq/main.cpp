#include "mainwindow.h"
#include <QApplication>
#include <memory>
#include <QMetaType>
#include <QDebug>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "----=======";

    qRegisterMetaType<shared_ptr<dragRole>>("shared_ptr<dragRole>");

    MainWindow w;
    w.show();

    return a.exec();
}
