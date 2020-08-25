#include "mainwindow.h"
#include <QApplication>
#include <memory>
#include <QMetaType>
#include <QDebug>
#include <QMessageBox>

#include "./qtsingleapplication/QtSingleApplication"

using namespace std;


int main(int argc, char *argv[])
{    
    QtSingleApplication singleApp(argc, argv);

    if (singleApp.isRunning())
    {
        QMessageBox::information(0, "提示", "程序已运行，请退出后重新打开！");

        return EXIT_SUCCESS;
    }

    qRegisterMetaType<shared_ptr<dragRole>>("shared_ptr<dragRole>");

    MainWindow w;
    w.show();

    return singleApp.exec();
}
