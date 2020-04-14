#ifndef TESTFLOW_H
#define TESTFLOW_H

#include <QWidget>
#include <QStackedWidget>
#include <string>
#include <vector>
#include <QJsonArray>
#include <QJsonObject>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMovie>
#include <QBrush>
#include <QPushButton>

#define NORMAL  "NORMAL"
#define ERROR   "ERROR"
#define CORRECT "CORRECT"

#define ALL_TEST    "all"

namespace Ui {
class testflow;
}

class testflow : public QWidget
{
    Q_OBJECT
public:
    enum tableClumType
    {
        Flow_Name_Index,
        Flow_Value_Index
    };

    enum flowType
    {
        NORMAL_FLOW,    ///< 正常流程
        FAULT_FLOW      ///< 故障流程
    };
private:
    enum TreeWidgetColumn {
            flowName,           //流程名称
            flowValue,          //值
            triggerCon ,	    //触发条件
            flowDelay,          //延时
            sendTime,           //发送时机
            targetDev,          //目标设备
            testStatus,         //测试状态
        };

    enum ItemType {
        groupItem,
        flowItem,
    };

public:
    explicit testflow(QWidget *parent = 0, QString sysName = "", QString recUuid = "", QString flowName = "", bool flag=true, QStackedWidget *item = 0);
    ~testflow();
    static std::string initRunitems(const std::vector<std::string> &record_uuid);
    static std::string getRunitems(const std::string &runmsg);
signals:
    void sendCmd(QString json);
    void sendRecid(QString recid);
    void stopRecid(QString recid);
private:
    void setTreeItemsFirst(QJsonArray);
    void setTreeItemsSecond(QJsonArray, QTreeWidgetItem *item);
    void setTreeItemsThird(QJsonArray,QTreeWidgetItem *itemSub);
    void onShowMsg(std::string status, QString msg);
    void onTestFlow(QString subFlowUuid, QString status);
    void onInitResultItem();
    void initBtn();
    void onTestStart();
    void onTestStop();
    QString converJson(QString msgType);
    void onTestOver(bool);
public slots:
    void testTreeShow(QJsonObject,QString);
private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void startflowTest();
    void slot_onClick();
private:
    Ui::testflow *ui;

    QMenu *testTree;
    QAction *startTest;
    QString m_sysName;
    QString m_recUuid;
    QString m_flowName;
    bool m_flag;
    QMovie *mRuningMovie;
    QBrush mTreeItemSrcColor;
    QString mRunType;
    QPushButton *m_manualTrigger;
    QStackedWidget *m_stackItem;
    QWidget *m_coverWidget;
};

#endif // TESTFLOW_H
