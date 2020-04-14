#ifndef FLOW_H
#define FLOW_H

#include <QDialog>
#include <QMenu>
#include "dataselect.h"
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

//extern QString addFlowSysName;
//extern QString addDataSysID;

typedef struct sysflow
{
    QString uuid;
    QString flowName;
    QString flowCreaTime;
    QString flowOpenTime;
    QString flowNote;
}sys_flow;

namespace Ui {
class flow;
}

class flow : public QDialog
{
    Q_OBJECT

public:
    explicit flow(QWidget *parent = 0);
    ~flow();
    /**
     * @brief initSystemTree 初始化分系统的树形结构
     */
    void initSystemTree();
    /**
     * @brief initFlowTable 初始化测试流程的表格
     */
    void initFlowTable();
    /**
     * @brief update 更新测试流程的表格
     * @param[in] flow
     */
    void update(sys_flow *flow);
    /**
     * @brief newBulitFlow 创建新的测试流程
     * @param[out] flowArr
     */
    void newBulitFlow(std::vector<QString> &flowArr);
    void initFlowTree();

private slots:

    void message(QTableWidgetItem *item);

    void newDataFunc();

    void newBulitFlowDialog();

    void deleFlow();

    void delTest();

    void on_flowTableWidget_customContextMenuRequested(const QPoint &pos);

    void on_flowTreeWidget_customContextMenuRequested(const QPoint &pos);

    void on_systemTree_itemClicked(QTreeWidgetItem *item, int column);

    void slotItemChanged(QTableWidgetItem *item);

    void newBulitTest();

private:
    Ui::flow *ui;
    QMenu *flowTable;
    QAction *newBulit;
    QAction *deleteBulit;

    QMenu *flowTree;
    QAction *newFlow;
    QAction *newData;
    QAction *copy;
    QAction *paste;
    QAction *del;

    QString m_addFlowSysName;

};

#endif // FLOW_H
