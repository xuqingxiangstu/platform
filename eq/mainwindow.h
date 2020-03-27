#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QKeyEvent>
#include "templatetree.h"
#include "flowtree.h"
#include "recordnavigation.h"
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
public:
    void keyPressEvent(QKeyEvent *e);
private:
    void onSave();

public slots:
    /**
     * @brief onFlowChange  流程变化
     * @param sysName       系统名称
     * @param testName      测试项
     * @param uuid
     */
    void onFlowChange(QString sysName, QString testName, QString uuid);

    void onDeleteFlow(QString uuid);


signals:
    void valueChange(QString attr, Json::Value val);

    void toShowProperty(Json::Value);
private:
    templateTree *mTemplateTreeObj;
    recordNavigation *mRecordNavigationObj;
    propertyWidget *mPropertyWidgetObj;
    QMap<QString, QWidget*> mFlowWidgetManager; //流程管理
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
