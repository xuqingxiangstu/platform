#include "recordnavigation.h"
#include "ui_recordnavigation.h"

#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"

#include "newprojectdialog.h"
#include <QMessageBox>

recordNavigation::recordNavigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recordNavigation)
{
    ui->setupUi(this);

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionNewProject);
    mPopMenu->addAction(ui->actionDelete);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &recordNavigation::onItemClicked);

    connect(mPopMenu, &QMenu::triggered, [=](QAction *action){
       onMenuTrigger(action);
    });

    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,[=](const QPoint &point){
        //mRightMousePoint = point;
        QTreeWidgetItem *currentItem = ui->treeWidget->itemAt(point);

        mCurSelectItem = currentItem;

        if(currentItem)
        {
            recordRole role = currentItem->data(Name_Index, Qt::UserRole).value<recordRole>();
            if(System_Node == role.nodeType)
            {
                ui->actionDelete->setEnabled(false);
                ui->actionNewProject->setEnabled(true);
            }
            else if(Flow_Node == role.nodeType)
            {
                ui->actionDelete->setEnabled(true);
                ui->actionNewProject->setEnabled(false);
            }

            mPopMenu->exec(QCursor::pos());
        }

    });    
}

recordNavigation::~recordNavigation()
{
    delete ui;
}

void recordNavigation::buildTree()
{
    //获取系统表中的系统
    Json::Value sysNames = systemTable::getInstance()->getSysGroups();
    if(sysNames.isNull())
        return ;

    for(int sysIndex = 0; sysIndex < sysNames.size(); sysIndex++)
    {
        std::string sysUuid = sysNames[sysIndex][SYSTEM_TABLE_UUID].asString();
        std::string des = sysNames[sysIndex][SYSTEM_TABLE_SYSTEM_NAME].asString();

        //创建系统节点
        QTreeWidgetItem *sysNode = createNode(sysUuid, des, System_Node);

        //根据系统uuid获取响应记录
        Json::Value recordJs = flowRecordTable::getInstance()->getValueBySystemUuid(sysUuid);
        if(!recordJs.isNull())
        {
            for(int recordIndex = 0; recordIndex < recordJs.size(); recordIndex++)
            {
                std::string uuid = recordJs[recordIndex][FLOW_RECORD_TABLE_UUID].asString();
                std::string name = recordJs[recordIndex][FLOW_RECORD_TABLE_FLOW_NAME].asString();

                sysNode->addChild(createNode(uuid, name, Flow_Node));
            }
        }

        ui->treeWidget->addTopLevelItem(sysNode);
        ui->treeWidget->expandItem(sysNode);
    }

    //触发第一个flow
    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        recordRole role = (*Itor)->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(Flow_Node == role.nodeType)
        {

            mCurSelectUuid = role.uuid.c_str();
            emit flowChange((*Itor)->parent()->text(Name_Index),(*Itor)->text(Name_Index), mCurSelectUuid);
            ui->treeWidget->setCurrentItem(*Itor);
            break;
        }
        ++Itor;
    }
}

QTreeWidgetItem *recordNavigation::createNode(const std::string &uuid, const std::string &name, NodeType type)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    //item->
    item->setText(Name_Index, QString::fromStdString(name));

    //关联数据
    QVariant data;
    recordRole role;
    role.nodeType = type;
    role.uuid = uuid;
    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);

    return item;
}

void recordNavigation::deleteItem(QTreeWidgetItem *item)
{
    if(!item)
        return ;

    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem)
    {
        QTreeWidgetItem *del = parentItem->takeChild(parentItem->indexOfChild(item));
        if(del)
            delete del;
    }
    else
    {
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
    }
}

void recordNavigation::waring(QString text)
{
    QMessageBox::warning(this, "提示", text);
}

void recordNavigation::onMenuTrigger(QAction *action)
{
    std::string actionObjName = action->objectName().toStdString();
    if("actionNewProject" == actionObjName)
    {
        onActionNew();
    }
    else if("actionDelete" == actionObjName)
    {
        if(QMessageBox::Yes == QMessageBox::warning(this, "提示", "是否删除",QMessageBox::Yes | QMessageBox::No))
        {
            onActionDelete();
        }
    }
}

void recordNavigation::onItemClicked(QTreeWidgetItem * item, int column)
{
    if(!item)
        return ;

    recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();
    if(Flow_Node == role.nodeType)
    {
        if(mCurSelectUuid.toStdString() != role.uuid)
        {
            mCurSelectUuid = role.uuid.c_str();

            emit flowChange(item->parent()->text(Name_Index),item->text(Name_Index),mCurSelectUuid);
        }
    }
}


void recordNavigation::onActionNew()
{
    if(!mCurSelectItem)
        return ;

    recordRole role = mCurSelectItem->data(Name_Index, Qt::UserRole).value<recordRole>();

    newProjectDialog dlg;
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        dlg.name();
        dlg.describe();

        //增加记录
        std::string uuid;
        if(flowRecordTable::getInstance()->addRecord(role.uuid, dlg.name().toStdString(), dlg.describe().toStdString(), uuid))
        {
            QTreeWidgetItem *item = createNode(uuid, dlg.name().toStdString(), Flow_Node);
            //添加节点
            mCurSelectItem->addChild(item);

            ui->treeWidget->setCurrentItem(item);

            //更新主界面
            emit flowChange(item->parent()->text(Name_Index),item->text(Name_Index),uuid.c_str());
        }
        else
        {
            waring("insert record faild");
        }

    }
}

void recordNavigation::onActionDelete()
{
    //获取uuid
    if(!mCurSelectItem)
        return ;

    recordRole role = mCurSelectItem->data(Name_Index, Qt::UserRole).value<recordRole>();

    //删除界面
    emit deleteFlow(role.uuid.c_str());

    //删除item
    deleteItem(mCurSelectItem);
    mCurSelectItem = nullptr;

    //删除记录
    flowRecordTable::getInstance()->deleteRecordByUuid(role.uuid);

    //TODO：删除相应文件
}
