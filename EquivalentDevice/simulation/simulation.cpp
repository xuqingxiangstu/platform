#include "simulation.h"
#include "ui_simulation.h"
#include <QMessageBox>
#include "../dbtableopt.h"
#include <QDebug>

simulation::simulation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::simulation)
{
    ui->setupUi(this);
    initFlowTable();
    initSystemTree();
    initFlowTree();
    ui->simTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget_3->setColumnHidden(2,true);

    connect(ui->simTableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_simTableWidget_customContextMenuRequested(const QPoint &pos)));
    connect(ui->treeWidget_3, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_treeWidget_3_customContextMenuRequested(const QPoint &pos)));

    m_simTestflags = false;

}

simulation::~simulation()
{
    delete ui;
}
void simulation::initSystemTree()
{
    Json::Value value;
    if(DBTableOpt::getInstance()->getValueAllSubSystemInfo(value)){
        //QMessageBox::about(NULL,"","数据库连接错误！");
    }
    for(int i = 0;i<value.size();i++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(value[i]["SYSTEM_NAME"].asString()));
        item->setText(1, QString::fromStdString(value[i]["UUID"].asString()));
        ui->simTreeWidget->addTopLevelItem(item);
    }

    QTreeWidgetItemIterator it(ui->simTreeWidget);
    if(*it){
        ui->simTreeWidget->setFocus();
        ui->simTreeWidget->setCurrentItem(*it);
    }
    if(value.size()>0){
        Json::Value init;
        QString systemUuid = QString::fromStdString(value[0]["UUID"].asString());
        DBTableOpt::getInstance()->getValueAllFlowRecord(systemUuid,init);
        int rowNum =  ui->simTableWidget->rowCount();
        for(int i = 0;i < rowNum;i++)
        {
            ui->simTableWidget->removeRow(0);
        }
        for(int i =0;i<init.size();i++){
            ui->simTableWidget->insertRow(i);
            ui->simTableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(init[i]["FLOW_NAME"].asString())));
            ui->simTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(init[i]["CREATE_TIME"].asString())));
            ui->simTableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(init[i]["RECENT_OPEN_TIME"].asString())));
            ui->simTableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(init[i]["NOTE"].asString())));
            ui->simTableWidget->setItem(i,4,new QTableWidgetItem(QString::fromStdString(init[i]["UUID"].asString())));
            ui->simTableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}
void simulation::initFlowTable()
{
    ui->simTableWidget->clearContents();
    ui->simTableWidget->setStyleSheet("selection-background-color:lightblue;");
    ui->simTableWidget->verticalHeader()->setVisible(false);//去掉默认行号
    ui->simTableWidget->setColumnHidden(4,true);
    ui->simTableWidget->setShowGrid(true);
    ui->simTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->simTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->simTableWidget->setShowGrid(true);
    ui->simTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->simTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->simTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    ui->simTableWidget->horizontalHeader()->setStretchLastSection(true);//最后一个单元格扩展
}

void simulation::initFlowTree()
{
    Json::Value m_value;
    ui->treeWidget_3->clear();
    DBTableOpt::getInstance()->getRecTestNotes(m_value);

    for(int i = 0;i<m_value["SYSTEM_NAME"].size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,QString::fromStdString(m_value["SYSTEM_NAME"][i].asString()));
        item->setText(1,QString::fromStdString(m_value["FLOW_NAME"][i].asString()));
        item->setText(2,QString::fromStdString(m_value["RECORD_UUID"][i].asString()));
        ui->treeWidget_3->addTopLevelItem(item);
    }

}
void simulation::on_simTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

    if(qApp->mouseButtons() != Qt::RightButton)
    {
        Json::Value value;
        QString systemUuid = item->text(1);
        QString systemName = item->text(0);
        m_sysName = systemName;
        DBTableOpt::getInstance()->getValueAllFlowRecord(systemUuid,value);
        int rowNum =  ui->simTableWidget->rowCount();
        for(int i = 0;i < rowNum;i++)
        {
            ui->simTableWidget->removeRow(0);
        }
        for(int i =0;i<value.size();i++){
            ui->simTableWidget->insertRow(i);
            ui->simTableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(value[i]["FLOW_NAME"].asString())));
            ui->simTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(value[i]["CREATE_TIME"].asString())));
            ui->simTableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(value[i]["RECENT_OPEN_TIME"].asString())));
            ui->simTableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(value[i]["NOTE"].asString())));
            ui->simTableWidget->setItem(i,4,new QTableWidgetItem(QString::fromStdString(value[i]["UUID"].asString())));
            ui->simTableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->simTableWidget->item(i, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void simulation::on_simTableWidget_customContextMenuRequested(const QPoint &pos)
{
    simTable = new QMenu(ui->simTableWidget);
    m_add = simTable->addAction("添加到仿真表");
    connect(m_add, SIGNAL(triggered(bool)), this, SLOT(addSimTree()));
    simTable->move(cursor().pos());
    simTable->show();
}

void simulation::on_treeWidget_3_customContextMenuRequested(const QPoint &pos)
{
    simTree = new QMenu(ui->treeWidget_3);
    m_del = simTree->addAction("删除");
    connect(m_del, SIGNAL(triggered(bool)), this, SLOT(delTest()));
    simTree->move(cursor().pos());
    simTree->show();
}

void simulation::delTest()
{
    QTreeWidgetItem *curritem = ui->treeWidget_3->currentItem();
    if (curritem != NULL)
    {
        QTreeWidgetItem *curritemparent = curritem->parent();
        if (curritemparent == NULL)
        {
            int index = ui->treeWidget_3->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->treeWidget_3->takeTopLevelItem(index);
            }
        }
        else
        {
            int countChild = curritemparent->childCount();
            for (int i = 0; i < countChild; i++)
            {
                if (curritemparent->child(i) == curritem)
                {
                    curritemparent->takeChild(i);
                    break;
                }
            }
        }
    }
}

void simulation::addSimTree()
{

    QString record_uuid,flowName;
    QList<QTableWidgetItem*> items = ui->simTableWidget->selectedItems();

    if(items.size()>0){

        int row = ui->simTableWidget->row(items.at(0));
        flowName= ui->simTableWidget->item(row,0)->text();
        record_uuid= ui->simTableWidget->item(row,4)->text();
        if(checkSimTree(record_uuid)){
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0,m_sysName);
            item->setText(1,flowName);
            item->setText(2,record_uuid);
            ui->treeWidget_3->addTopLevelItem(item);
        }else{
            QMessageBox::about(NULL,"","测试任务已存在！");
        }

    }else{

       QMessageBox::about(NULL,"","选择内容为空！");

    }

}

void simulation::on_submitSim_clicked()
{
    m_simTestflags = true;
    Json::Value sysName;
    Json::Value flowName;
    Json::Value recordUuid;
    QTreeWidgetItemIterator it(ui->treeWidget_3);
    if((*it)){
        while (*it) {
            sysName.append((*it)->text(0).toStdString());
            flowName.append((*it)->text(1).toStdString());
            recordUuid.append((*it)->text(2).toStdString());
            ++it;
        }
        m_simTestArr["SYSTEM_NAME"] = sysName;
        m_simTestArr["FLOW_NAME"] = flowName;
        m_simTestArr["RECORD_UUID"] = recordUuid;
        DBTableOpt::getInstance()->setRecTestNotes(m_simTestArr);
        this->hide();

    }else{
        QMessageBox::about(NULL,"","请选择测试流程！");
    }

}
void simulation::getSimTest(Json::Value &value)
{
    value = m_simTestArr;
}

void simulation::closeEvent(QCloseEvent *)
{
    m_simTestflags = false;
}

void simulation::on_pushButton_clicked()
{
    m_simTestflags = false;
    this->hide();
}

bool simulation::checkSimTree(QString record_uuid)
{
    QTreeWidgetItemIterator it(ui->treeWidget_3);
    bool flag = true;
    if((*it)){
        while (*it) {
            if(record_uuid == (*it)->text(2))
            {
                flag = false;
                break;
            }
            ++it;
        }
    }
    return flag;
}
