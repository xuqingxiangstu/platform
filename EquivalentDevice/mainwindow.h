#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QString>
#include <QMap>
#include <QJsonObject>
#include <memory>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QListWidgetItem>

#include "./test/testflow.h"
#include "../include/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../include/PfCommon/jsoncpp/json.h"
#include "simulation/simulation.h"
#include <QProcess>
namespace Ui {
class MainWindow;
}
class cmdDecode;
class simulation;

#define DEBUG_LOGIC 0

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void unLoading();

private:
    void initSystemTree();
    void initFlowTree();
    void loading();
    void initFlowCom();
    void loadSimTest();
    void clearSysTree();
signals:
    void sendCmd(QString json);
    void cmdMsg(QJsonObject,QString);
private slots:
    void on_TbSysManage_triggered();
    void on_TbFlow_triggered();
    void on_TbSimulation_triggered();
    void on_TbExport_triggered();
    void switchPage(QListWidgetItem * );
    void on_ExcelToDb_triggered();
    void reset();
    void on_resetSys_triggered();
    void setFlag(bool flag);
    void saveRecid(QString recid);
    void deleRecid(QString recid);

    void onLogicOutPut();
private:
    Ui::MainWindow *ui;
    cmdDecode *mCmdDecode;
    simulation *mSimulation;
    testflow *m_testFlow;
    Pf::PfAdapter::PfAdapterManager *mPfAdaterObjs;
    Json::Value m_value;
    QString m_resetRecord;
    std::vector<QString> m_saveRecid;
    bool m_flag;
    int *taskCount;
    QProcess *mLogicLayerProcess;
};

#endif // MAINWINDOW_H
