#ifndef SIMULATION_H
#define SIMULATION_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QTableWidgetItem>
#include "../include/PfCommon/jsoncpp/json.h"
#include "../mainwindow.h"

namespace Ui {
class simulation;
}

class simulation : public QDialog
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event);
public:
    explicit simulation(QWidget *parent = 0);
    ~simulation();
    void initSystemTree();
    void initFlowTable();
    void getSimTest(Json::Value &value);

private slots:
    void delTest();
    void addSimTree();
    void on_simTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_simTableWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_3_customContextMenuRequested(const QPoint &pos);
    void on_submitSim_clicked();
    void on_pushButton_clicked();
signals:
    void sendSimTest();
private:
    Ui::simulation *ui;
    QString m_sysName;

    QMenu *simTable;
    QAction *m_add;

    QMenu *simTree;
    QAction *m_del;   
public:
    Json::Value m_simTestArr;
    bool m_simTestflags;
};

#endif // SIMULATION_H
