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
    void stopRecid(QString recid,int cnt);
    void initFlowErr(QString recid,bool flag);

    void sendErrRecid(QString recid);
    void stopErrRecid(QString recid);
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
    void setBtnAble(int index);
    void resetFlowState(QString flow_uuid);
public slots:
    void testTreeShow(QJsonObject,QString);
    void rcvAllStart(QVector<QString> &msg);
    void setStartBtn(QString record_uuid,QString msg);
private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void startflowTest();
    void slot_onClick(bool);
    void on_pushButton_clicked();

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

    QTreeWidget *m_treeWidget;

    bool isnotAllStart;
};

#endif // TESTFLOW_H
