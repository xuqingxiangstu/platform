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
#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../src/PfCommon/jsoncpp/json.h"
#include "simulation/simulation.h"
#include "delegate/qtreeitemdelegate.h"
#include <QProcess>
#include <QVector>

extern int startAllCnt;
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
    void setBtnAble(bool flag);
    void sendAll(QString record_uuid, QString msgType);
signals:
    void sendCmd(QString json);
    void cmdMsg(QJsonObject,QString);
    void showCurrent(QString id);
    void sendAllStart(QString json);
    void sendErrId(QVector<QString> &msg);
    void sendAllStartId(QVector<QString> &msg);
    void sendStartSignal(QString record_uuid,QString msg);

private slots:

    void switchPage(QListWidgetItem * );
    void reset();
    void setFlag(bool flag);
    void saveRecid(QString recid);
    void deleRecid(QString recid, int cnt);
    void onLogicOutPut(); 
    void on_simBtn_clicked();
    void on_flowBtn_clicked();
    void on_sysBtn_clicked();
    void on_impBtn_clicked();
    void on_expBtn_clicked();
    void on_resetBtn_clicked();
    void on_changeBtn_clicked();
    void timeUpdate(void);
    void revRecid(QString,bool);
    void on_pushButtonStartAll_clicked();
    void on_pushButtonStopAll_clicked();

    void saveErrRecid(QString recid);
    void deleErrRecid(QString recid);

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
	qtreeitemdelegate *m_sysTreeItem;
    bool m_changeFlag;
    std::vector<QString> m_saveAllStartId;
    QVector<QString> m_errRecoid;
    std::vector<QString> m_saveErrRecid;
    QVector<QString> m_AllStartRecoid;
};

#endif // MAINWINDOW_H
