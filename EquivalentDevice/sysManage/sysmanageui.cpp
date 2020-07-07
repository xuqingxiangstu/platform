#include "sysmanageui.h"
#include "ui_sysmanageui.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QInputDialog>
#include <QTableWidgetItem>
#include "addsysmanage.h"
#include <QScrollBar>
#include "../dbtableopt.h"
#include <QDebug>
#include <algorithm>
#include <QMessageBox>
#include <QList>
#include "mainwindow.h"
#include <QFormLayout>
#include <QDialogButtonBox>

using namespace std;

QString addDevSysName = "";

sysManageUi::sysManageUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sysManageUi)
{
    ui->setupUi(this);
    ui->information_tableWidget->setAlternatingRowColors(true);
    systemInit();
    initSystemTree();

    ui->systemTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->systemTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_systemTree_customContextMenuRequested(const QPoint &pos)));

    ui->information_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->information_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_information_tableWidget_customContextMenuRequested(const QPoint &pos)));

    connect(ui->systemTree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(changeSysName(QTreeWidgetItem*,int)));

}

sysManageUi::~sysManageUi()
{
    delete ui;
}

void sysManageUi::systemInit()
{
    ui->information_tableWidget->clearContents();
    ui->information_tableWidget->setStyleSheet("selection-background-color:lightblue;");
    ui->information_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->information_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->information_tableWidget->setColumnHidden(3,true);
    ui->information_tableWidget->setShowGrid(true);
    ui->information_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->information_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->information_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    ui->information_tableWidget->horizontalHeader()->setStretchLastSection(true);//最后一个单元格扩展
    ui->information_tableWidget->setFocusPolicy(Qt::NoFocus);//解决选中虚框问题
    ui->information_tableWidget->setFrameShape(QFrame::NoFrame);//去除边框
    ui->information_tableWidget->setColumnWidth(0,150);
    ui->information_tableWidget->setColumnWidth(1,100);
    ui->information_tableWidget->setColumnWidth(2,300);
    ui->information_tableWidget->verticalHeader()->setVisible(false);//去掉默认行号
}

void sysManageUi::update(my_sys *stu)
{
    int rowIndex = ui->information_tableWidget->rowCount();
    ui->information_tableWidget->setRowCount(rowIndex + 1);
    ui->information_tableWidget->setItem(rowIndex,0,new QTableWidgetItem(stu->sysName));
    ui->information_tableWidget->setItem(rowIndex,1,new QTableWidgetItem(stu->dev_type));
    ui->information_tableWidget->setItem(rowIndex,2,new QTableWidgetItem(stu->describe));
    ui->information_tableWidget->setItem(rowIndex,3,new QTableWidgetItem(stu->uuid));
    for(int i = 0;i<4;i++){
        ui->information_tableWidget->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void sysManageUi::changeUpdate(my_sys *stu,int col)
{
    ui->information_tableWidget->removeRow(col);

    int rowIndex = ui->information_tableWidget->rowCount();
    ui->information_tableWidget->setRowCount(rowIndex + 1);
    ui->information_tableWidget->setItem(rowIndex,0,new QTableWidgetItem(stu->sysName));
    ui->information_tableWidget->setItem(rowIndex,1,new QTableWidgetItem(stu->dev_type));
    ui->information_tableWidget->setItem(rowIndex,2,new QTableWidgetItem(stu->describe));
    ui->information_tableWidget->setItem(rowIndex,3,new QTableWidgetItem(stu->uuid));
    for(int i = 0;i<4;i++){
        ui->information_tableWidget->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}
void sysManageUi::on_systemTree_customContextMenuRequested(const QPoint &pos)
{
    sysNameMenu = new QMenu(ui->systemTree);
    newBulit = sysNameMenu->addAction("新建");
    deleteBulit = sysNameMenu->addAction("删除");
    connect(newBulit, SIGNAL(triggered(bool)), this, SLOT(newBulitD()));
    connect(deleteBulit, SIGNAL(triggered(bool)), this, SLOT(deleD()));
    sysNameMenu->move(cursor().pos());
    sysNameMenu->show();
}
void sysManageUi::newBulitD()
{
    QString sysUuid = "";
    std::vector<QString> flowArr;
    QDialog dialog(this);
    QFormLayout form(&dialog);
    QString arr[2]={"分系统名称","分系统类型"};
    QList<QLineEdit *> fields;

    QRegExp regx("^(25[0-6]|2[0-4][0-9]|[1]?[0-9]?[0-9])$");

    for(int i = 0; i < 2; ++i) {
        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString(arr[i]).arg(i + 1);
        if(label == "分系统类型"){
            lineEdit->setValidator(new QRegExpValidator(regx));
        }
        form.addRow(label, lineEdit);
        fields << lineEdit;
    }
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        foreach(QLineEdit * lineEdit, fields) {
            flowArr.push_back(lineEdit->text());
        }
        if(!DBTableOpt::getInstance()->addSubSystem(flowArr,sysUuid)){
            QMessageBox::information(this, tr("消息"), tr("添加失败!"), QMessageBox::Ok);
            return;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, flowArr[0]);
        item->setText(1, sysUuid);
        item->setFlags(Qt::ItemIsEditable| Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->systemTree->addTopLevelItem(item);
    }
}
void sysManageUi::on_information_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    deviceTable = new QMenu(ui->information_tableWidget);
    newDevice = deviceTable->addAction("新增设备");
    delDevice = deviceTable->addAction("删除设备");
    connect(newDevice, SIGNAL(triggered(bool)), this, SLOT(newDeviceFunc()));
    connect(delDevice, SIGNAL(triggered(bool)), this, SLOT(deleDev()));
    deviceTable->move(cursor().pos());
    deviceTable->show();
}

void sysManageUi::deleDev()
{
    QList<QTableWidgetItem*> items = ui->information_tableWidget->selectedItems();

    QMessageBox::StandardButton reply;
    if(items.size()){
        bool ok = false;
        reply  =  QMessageBox::information(NULL, "Title", "确认删除？",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if( reply == QMessageBox::Yes ){

            int row = ui->information_tableWidget->row(items.at(0));
            QString uuid = ui->information_tableWidget->item(row,3)->text();
            ok = DBTableOpt::getInstance()->deleteSystemDev(uuid);
            if(!ok){
                QMessageBox::information(this, tr("消息"), tr("删除失败!"), QMessageBox::Ok);
                return;
            }
            ui->information_tableWidget->removeRow(row);
        }
    }else{
        QMessageBox::about(NULL,"","选择内容为空！");
    }

}

void sysManageUi::newDeviceFunc(){
    addSysManage *AI=new addSysManage(this);
    AI->show();
}

void sysManageUi::deleD()
{
    QMessageBox::StandardButton reply;
    bool ok = false;
    reply  =  QMessageBox::information(NULL, "Title", "确认删除？",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(reply == QMessageBox::Yes ){
        QTreeWidgetItem* curItem=ui->systemTree->currentItem();
        QString systemName = curItem->text(0);
        QString uuid = curItem->text(1);
        if(DBTableOpt::getInstance()->deleteSubSystem(uuid)){

        }
        int rowNum =  ui->information_tableWidget->rowCount();
        for(int i = 0;i < rowNum;i++)
        {
            ui->information_tableWidget->removeRow(0);

        }
        delete ui->systemTree->takeTopLevelItem(ui->systemTree->currentIndex().row());
    }

}
void sysManageUi::initSystemTree()
{
    Json::Value value;
    if(DBTableOpt::getInstance()->getValueAllSubSystemInfo(value)){
        //QMessageBox::about(NULL,"","数据库连接错误！");
    }
    for(int i = 0;i<value.size();i++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(value[i]["SYSTEM_NAME"].asString()));
        item->setText(1, QString::fromStdString(value[i]["UUID"].asString()));
        item->setFlags(Qt::ItemIsEditable| Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->systemTree->addTopLevelItem(item);
    }

    QTreeWidgetItemIterator it(ui->systemTree);
    if(*it){
        ui->systemTree->setFocus();
        ui->systemTree->setCurrentItem(*it);
    }
    if(value.size()>0){
        Json::Value init;
        m_sysUuid = QString::fromStdString(value[0]["UUID"].asString());
        addDevSysName = m_sysUuid;
        DBTableOpt::getInstance()->getSystemDev(m_sysUuid,init);
        int rowNum =  ui->information_tableWidget->rowCount();
        for(int i = 0;i < rowNum;i++)
        {
            ui->information_tableWidget->removeRow(0);
        }
        for(int i =0;i<init.size();i++){
            ui->information_tableWidget->insertRow(i);
            ui->information_tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(init[i]["DEV_NAME"].asString())));
            ui->information_tableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(init[i]["DEV_TYPE"].asString())));
            ui->information_tableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(init[i]["DESCRIBE"].asString())));
            ui->information_tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(init[i]["UUID"].asString())));
            ui->information_tableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->information_tableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->information_tableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->information_tableWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }


}

void sysManageUi::on_systemTree_itemClicked(QTreeWidgetItem *item, int column)
{
    if(qApp->mouseButtons() != Qt::RightButton)
    {
        Json::Value value;
        QString systemUuid = item->text(1);
        addDevSysName = systemUuid;
        DBTableOpt::getInstance()->getSystemDev(systemUuid,value);
        int rowNum =  ui->information_tableWidget->rowCount();
        for(int i = 0;i < rowNum;i++)
        {
            ui->information_tableWidget->removeRow(0);

        }
        for(int i =0;i<value.size();i++){
            ui->information_tableWidget->insertRow(i);
            ui->information_tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(value[i]["DEV_NAME"].asString())));
            ui->information_tableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(value[i]["DEV_TYPE"].asString())));
            ui->information_tableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(value[i]["DESCRIBE"].asString())));
            ui->information_tableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(value[i]["UUID"].asString())));
            ui->information_tableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->information_tableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->information_tableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->information_tableWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}
void sysManageUi::changeSysName(QTreeWidgetItem* item,int column)
{
    QString uuid = item->text(1);
    QString Name = item->text(0);
    DBTableOpt::getInstance()->alterSubSystem(uuid,Name);
}

void sysManageUi::on_information_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    QString mouseActive = "update";

    int countRow = item->row();

    QString devUuid = ui->information_tableWidget->item(countRow,3)->text();
    QString sysName = ui->information_tableWidget->item(countRow,0)->text();

    addSysManage *AI=new addSysManage(this,mouseActive,devUuid,sysName,countRow);
    AI->show();
}
