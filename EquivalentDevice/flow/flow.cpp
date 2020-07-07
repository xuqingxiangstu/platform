#include "flow.h"
#include "ui_flow.h"
//#include <windows.h>
#include <stdio.h>
#include <QMessageBox>
#include <QDebug>
#include "../dbtableopt.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include <QDateTime>
#include <QInputDialog>

flow::flow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flow)
{
    ui->setupUi(this);
    ui->flowTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->flowTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_flowTreeWidget_customContextMenuRequested(const QPoint &pos)));
    ui->flowTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->flowTableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_flowTableWidget_customContextMenuRequested(const QPoint &pos)));
    initSystemTree();
    initFlowTable();
    initFlowTree();
}

flow::~flow()
{
    delete ui;
}

void flow::initSystemTree()
{
    Json::Value value;
    if(DBTableOpt::getInstance()->getValueAllSubSystemInfo(value)){
        //QMessageBox::about(NULL,"","数据库连接错误！");
    }
    for(int i = 0;i<value.size();i++){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(value[i]["SYSTEM_NAME"].asString()));
        item->setText(1, QString::fromStdString(value[i]["UUID"].asString()));
        ui->systemTree->addTopLevelItem(item);
    }
}
void flow::initFlowTable()
{
    ui->flowTableWidget->clearContents();
    ui->flowTableWidget->setStyleSheet("selection-background-color:lightblue;");
    ui->flowTableWidget->verticalHeader()->setVisible(false);//去掉默认行号
    ui->flowTableWidget->setColumnHidden(4,true);
    ui->flowTableWidget->setShowGrid(true);
    ui->flowTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->flowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

}
void flow::initFlowTree()
{
    ui->flowTreeWidget->setDragEnabled(true);
    //设置拖放
    ui->flowTreeWidget->setAcceptDrops(true);

    //设置显示将要被放置的位置
    ui->flowTreeWidget->setDropIndicatorShown(true);

    //设置拖放模式为移动项目，否则为复制项目
    ui->flowTreeWidget->setDragDropMode(QAbstractItemView::InternalMove);

    //设置item进入编辑的方式
    ui->flowTreeWidget->setEditTriggers(QAbstractItemView::SelectedClicked);

}
void flow::on_flowTableWidget_customContextMenuRequested(const QPoint &pos)
{
    flowTable = new QMenu(ui->flowTableWidget);
    newBulit = flowTable->addAction("新建");
    deleteBulit = flowTable->addAction("删除");
    connect(newBulit, SIGNAL(triggered(bool)), this, SLOT(newBulitFlowDialog()));
    connect(deleteBulit, SIGNAL(triggered(bool)), this, SLOT(deleFlow()));
    flowTable->move(cursor().pos());
    flowTable->show();
}

void flow::on_flowTreeWidget_customContextMenuRequested(const QPoint &pos)
{

    flowTree = new QMenu(ui->flowTreeWidget);
    newFlow = flowTree->addAction("新建测试项");
    newData = flowTree->addAction("新建数据");
    copy = flowTree->addAction("复制");
    paste = flowTree->addAction("粘贴");
    del = flowTree->addAction("删除");
    connect(newFlow, SIGNAL(triggered(bool)), this, SLOT(newBulitTest()));
    connect(newData, SIGNAL(triggered(bool)), this, SLOT(newDataFunc()));
    connect(copy, SIGNAL(triggered(bool)), this, SLOT(newDataFunc()));
    connect(paste, SIGNAL(triggered(bool)), this, SLOT(newDataFunc()));
    connect(del, SIGNAL(triggered(bool)), this, SLOT(delTest()));
    flowTree->move(cursor().pos());
    flowTree->show();
}
void flow::message(QTableWidgetItem *item){
    connect(ui->flowTableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotItemChanged(QTableWidgetItem*)));
}

void flow::deleFlow()
{
    QList<QTableWidgetItem*> items = ui->flowTableWidget->selectedItems();

    QMessageBox::StandardButton reply;
    if(items.size()){
        bool ok = false;
        reply  =  QMessageBox::information(NULL, "Title", "确认删除？",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if( reply == QMessageBox::Yes ){

            int row = ui->flowTableWidget->row(items.at(0));
            QString uuid = ui->flowTableWidget->item(row,4)->text();
            ok = DBTableOpt::getInstance()->deleteFlow(uuid);
            if(!ok){
                QMessageBox::information(this, tr("消息"), tr("删除失败!"), QMessageBox::Ok);
                return;
            }
            ui->flowTableWidget->removeRow(row);
        }
    }else{
        QMessageBox::about(NULL,"","选择内容为空！");
    }
}

void flow::newBulitFlowDialog()
{
    std::vector<QString> flowArr;
    QDialog dialog(this);
    QFormLayout form(&dialog);
    QString arr[2]={"测试流程","备注"};
    QList<QLineEdit *> fields;
    for(int i = 0; i < 2; ++i) {
        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString(arr[i]).arg(i + 1);
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
        newBulitFlow(flowArr);
    }

}
void flow::newBulitFlow(std::vector<QString> &flowArr)
{
    sys_flow flow;
    QString flowUuid = "";
    Json::Value value;

//    SYSTEMTIME sys;
//    GetLocalTime( &sys );
//    QString sysTime = QString("%1-%2-%3 %4-%5-%6").arg(sys.wYear).arg(sys.wMonth).arg(sys.wDay).arg(sys.wHour).arg(sys.wMinute).arg(sys.wSecond);

    QString sysTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");


    value["SYSTEM_UUID"] = m_addFlowSysName.toStdString();
    value["FLOW_NAME"] = flowArr[0].toStdString();
    value["CREATE_TIME"] = sysTime.toStdString();
    value["RECENT_OPEN_TIME"] = sysTime.toStdString();
    value["NOTE"] = flowArr[1].toStdString();

    if(!DBTableOpt::getInstance()->addSubSystemFlow(value,flowUuid)){
        QMessageBox::information(this, tr("消息"), tr("添加失败!"), QMessageBox::Ok);
        return;
    }
    flow.uuid = flowUuid;
    flow.flowName = flowArr[0];
    flow.flowCreaTime = sysTime;
    flow.flowOpenTime = sysTime;
    flow.flowNote = flowArr[1];

    update(&flow);
}

void flow::newDataFunc(){
    dataSelect *data = new dataSelect(this);
    data->setWindowModality(Qt::ApplicationModal);
    data->show();
}

void flow::on_systemTree_itemClicked(QTreeWidgetItem *item, int column)
{
    disconnect(ui->flowTableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotItemChanged(QTableWidgetItem*)));
    if(qApp->mouseButtons() != Qt::RightButton)
    {
        Json::Value value;
        QString SystemUUID = item->text(1);
        m_addFlowSysName = SystemUUID;
        DBTableOpt::getInstance()->getValueAllFlowRecord(SystemUUID,value);
        int rowNum =  ui->flowTableWidget->rowCount();
        for(int i = 0;i < rowNum;i++)
        {
            ui->flowTableWidget->removeRow(0);

        }
        for(int i =0;i<value.size();i++){
            ui->flowTableWidget->insertRow(i);
            ui->flowTableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(value[i]["FLOW_NAME"].asString())));
            ui->flowTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromStdString(value[i]["CREATE_TIME"].asString())));
            ui->flowTableWidget->setItem(i,2,new QTableWidgetItem(QString::fromStdString(value[i]["RECENT_OPEN_TIME"].asString())));
            ui->flowTableWidget->setItem(i,3,new QTableWidgetItem(QString::fromStdString(value[i]["NOTE"].asString())));
            ui->flowTableWidget->setItem(i,4,new QTableWidgetItem(QString::fromStdString(value[i]["UUID"].asString())));
            ui->flowTableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->flowTableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->flowTableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->flowTableWidget->item(i, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->flowTableWidget->item(i, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->flowTableWidget->item(i, 1)->setFlags(Qt::NoItemFlags);
            ui->flowTableWidget->item(i, 2)->setFlags(Qt::NoItemFlags);
        }
        connect(ui->flowTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(message(QTableWidgetItem*)));
    }
}

void flow::update(sys_flow *flow)
{
    int rowIndex = ui->flowTableWidget->rowCount();
    ui->flowTableWidget->setRowCount(rowIndex + 1);
    ui->flowTableWidget->setItem(rowIndex,0,new QTableWidgetItem(flow->flowName));
    ui->flowTableWidget->setItem(rowIndex,1,new QTableWidgetItem(flow->flowCreaTime));
    ui->flowTableWidget->setItem(rowIndex,2,new QTableWidgetItem(flow->flowOpenTime));
    ui->flowTableWidget->setItem(rowIndex,3,new QTableWidgetItem(flow->flowNote));
    ui->flowTableWidget->setItem(rowIndex,4,new QTableWidgetItem(flow->uuid));
    for(int i = 0;i<5;i++){
        ui->flowTableWidget->item(rowIndex, i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->flowTableWidget->item(rowIndex, 1)->setFlags(Qt::NoItemFlags);
        ui->flowTableWidget->item(rowIndex, 2)->setFlags(Qt::NoItemFlags);
    }
}

void flow::slotItemChanged(QTableWidgetItem *item)
{

    bool ok = false;
    int countRow = item->row();
    int countCol = item->column();
    QString Name;
    QString uuid = ui->flowTableWidget->item(countRow,4)->text();
    if(countCol == 0){
        Name = "FLOW_NAME";
    }else {
        Name = "NOTE";
    }
    QString Content = item->data(Qt::DisplayRole).toString();

    ok = DBTableOpt::getInstance()->alterFlow(uuid,Name,Content);
    if(!ok){
        QMessageBox::information(this, tr("消息"), tr("修改失败!"), QMessageBox::Ok);
        return;
    }

}

void flow::newBulitTest(){

    bool isOK;
    QString systemName = QInputDialog::getText(NULL, "新增测试项","请输入测试项名称！", QLineEdit::Normal,"",&isOK);
    if(isOK) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, systemName);
        ui->flowTreeWidget->addTopLevelItem(item);
    }
}

void flow::delTest()
{
    QTreeWidgetItem *curritem = ui->flowTreeWidget->currentItem();
    if (curritem != NULL)
    {
        QTreeWidgetItem *curritemparent = curritem->parent();
        if (curritemparent == NULL)
        {
            int index = ui->flowTreeWidget->indexOfTopLevelItem(curritem);
            if (index != -1)
            {
                ui->flowTreeWidget->takeTopLevelItem(index);
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



