#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "projectnavigation.h"
#include "workspacearea.h"
#include "messagearea.h"

#include "./propertyui/propertywidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void test();
private:
    projectNavigation *mProjectNavigationWidget;    ///< 导航区
    propertyWidget *mNavigationPropertyObj; //导航属性
    workspaceArea *mWorkSpaceAreaWidget;            ///< 工作区
    messageArea *mMessageAreaWidget;                ///< 消息区
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
