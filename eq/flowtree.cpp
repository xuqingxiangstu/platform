#include "flowtree.h"
#include "ui_flowtree.h"

#include <iostream>

#include "property/templateproperty.h"
#include "dragTree/dragrole.h"
#include "property/nodeproperty.h"
#include "toXml/createsubflow.h"
#include "newdialog.h"
#include <QDebug>

flowTree::flowTree(QString uuid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flowTree),
    mCurProjectUuid(uuid)
{
    ui->setupUi(this);

    //ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->treeWidget, &PfTreeWidget::dropDrag, this, &flowTree::onDropDrag);
    connect(ui->treeWidget, &PfTreeWidget::itemClicked, this, &flowTree::onItemClicked);
   // ui->treeWidget->setex;
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionNewFlow);
    mPopMenu->addAction(ui->actionNewSubFlow);
    mPopMenu->addAction(ui->actionCopy);
    mPopMenu->addAction(ui->actionPase);
    mPopMenu->addAction(ui->actionDelete);
    mPopMenu->addAction(ui->actionTestSend);

    connect(mPopMenu, &QMenu::triggered, [=](QAction *action){
       onMenuTrigger(action);
    });

    connect(ui->treeWidget,&PfTreeWidget::customContextMenuRequested,[=](const QPoint &point){
        mRightMousePoint = point;
        QTreeWidgetItem *currentItem = ui->treeWidget->itemAt(mRightMousePoint);

        mCurItem = currentItem;

        if(currentItem)
        {
            std::shared_ptr<dragRole> data = currentItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

            if(dragRole::Node_Flow == data->getNodeType())
            {
                ui->actionCopy->setEnabled(false);
                ui->actionDelete->setEnabled(false);
                ui->actionPase->setEnabled(false);
                ui->actionNewSubFlow->setEnabled(true);
                ui->actionNewFlow->setEnabled(true);
                ui->actionTestSend->setEnabled(false);
            }
            else if(dragRole::Node_SubFlow == data->getNodeType())
            {
                ui->actionCopy->setEnabled(false);
                ui->actionDelete->setEnabled(false);
                ui->actionPase->setEnabled(false);
                ui->actionNewSubFlow->setEnabled(true);
                ui->actionNewFlow->setEnabled(false);
                ui->actionTestSend->setEnabled(false);
            }
            else if( (dragRole::Node_Cmd == data->getNodeType()) || (dragRole::Node_Param_Group == data->getNodeType()))
            {
                ui->actionCopy->setEnabled(false);
                ui->actionDelete->setEnabled(false);
                ui->actionPase->setEnabled(false);
                ui->actionNewSubFlow->setEnabled(false);
                ui->actionNewFlow->setEnabled(true);
                ui->actionTestSend->setEnabled(true);
            }
            else if((dragRole::Node_Param == data->getNodeType()))
            {
                ui->actionCopy->setEnabled(false);
                ui->actionDelete->setEnabled(false);
                ui->actionPase->setEnabled(false);
                ui->actionNewSubFlow->setEnabled(false);
                ui->actionNewFlow->setEnabled(false);
                ui->actionTestSend->setEnabled(false);
            }
        }
        else
        {
            ui->actionCopy->setEnabled(false);
            ui->actionDelete->setEnabled(false);
            ui->actionPase->setEnabled(false);
            ui->actionNewSubFlow->setEnabled(false);
            ui->actionNewFlow->setEnabled(true);
            ui->actionTestSend->setEnabled(false);
        }

        mPopMenu->exec(QCursor::pos());
    });
#if 1
    QTreeWidgetItem *item = new QTreeWidgetItem();

    QString showText = "发射测试";

    item->setText(0, showText);
    //关联数据
    QVariant variant;
    std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();
    dragData->setNodeType(dragRole::Node_Flow);
    dragData->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_Flow])));
    dragData->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(showText.toStdString()));
    variant.setValue(dragData);
    item->setData(0,Qt::UserRole,variant);

    for(int index = 0; index < 1; index++)
    {
        QTreeWidgetItem *subitem = new QTreeWidgetItem();

        showText = "-14h检查";

        subitem->setText(0, showText);
        //关联数据
        QVariant variant1;
        std::shared_ptr<dragRole> dragData1 = std::make_shared<dragRole>();
        dragData1->setNodeType(dragRole::Node_SubFlow);
        dragData1->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_SubFlow])));
        dragData1->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(showText.toStdString()));
        variant1.setValue(dragData1);
        subitem->setData(0,Qt::UserRole,variant1);

        item->addChild(subitem);

        ui->treeWidget->addTopLevelItem(item);
    }
#endif
    ui->treeWidget->expandAll();
}

flowTree::~flowTree()
{
    delete ui;
}

void flowTree::setSystemAndProductName(QString sysName, QString produceName)
{
    ui->systemNameLab->setText(sysName);
    ui->testProductLab->setText(produceName);
}



QTreeWidgetItem *flowTree::newItem(QTreeWidgetItem *dropItem)
{
    if(!dropItem)
        return nullptr;

    std::shared_ptr<dragRole> drapData = dropItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();
    if( (dragRole::Node_Cmd == drapData->getNodeType()))
    {
        return newCmdItem(dropItem);
    }
    else if(dragRole::Node_Param_Group == drapData->getNodeType())
    {
        return newParamItem(dropItem);
    }

    return dropItem;
}

QTreeWidgetItem *flowTree::newCmdItem(QTreeWidgetItem *dropItem)
{
    std::shared_ptr<dragRole> drapData = dropItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

    QTreeWidgetItem *item = new QTreeWidgetItem();

    QVariant variant;
    std::shared_ptr<dragRole> newData = drapData->clone();
    variant.setValue(newData);

    item->setData(0, Qt::UserRole, variant);

    Json::Value desJs;
    drapData->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);
    if(!desJs.isNull())
        item->setText(Name_Index, desJs.asString().c_str());

    Json::Value startCondJs;
    drapData->getProperty()->getProperty(PROPERTY_START_CONDITION, startCondJs);
    if(!startCondJs.isNull())
    {
        item->setText(Start_Cond_Index, startCondJs[PROPERTY_CONDITION_NAME].asString().c_str());
    }
    else
    {
        item->setText(Start_Cond_Index, PROPERTY_CONDITION_NO);
    }

    Json::Value delayJs;
    drapData->getProperty()->getProperty(PROPERTY_DELAY, delayJs);
    if(!delayJs.isNull())
        item->setText(Delay_Index, delayJs.asString().c_str());

    Json::Value stopCondJs;
    drapData->getProperty()->getProperty(PROPERTY_STOP_CONDITION, stopCondJs);
    if(!stopCondJs.isNull())
    {
        item->setText(Stop_Cond_Index, stopCondJs[PROPERTY_CONDITION_NAME].asString().c_str());
    }
    else
    {
        item->setText(Stop_Cond_Index, PROPERTY_CONDITION_NO);
    }

    Json::Value timingJs;
    drapData->getProperty()->getProperty(PROPERTY_TIMING, timingJs);
    if(!timingJs.isNull())
        item->setText(Timming_Index, timingJs.asString().c_str());

    Json::Value desDevJs;
    drapData->getProperty()->getProperty(PROPERTY_DESTDEVICE, desDevJs);
    if(!desDevJs.isNull())
    {
        item->setText(Dest_Dev_Index, desDevJs[PROPERTY_DEV_VALUE_NAME].asString().c_str());
    }

    item->setExpanded(true);

    return item;
}

QTreeWidgetItem *flowTree::newParamItem(QTreeWidgetItem *dropItem)
{
    //创建父节点
    QTreeWidgetItem *parentItem = newCmdItem(dropItem);

    //创建子节点
    for(int index = 0; index < dropItem->childCount(); index++)
    {
        QTreeWidgetItem *dropChildItem = dropItem->child(index);

        QTreeWidgetItem *newChildItem = new QTreeWidgetItem();

        std::shared_ptr<dragRole> drapData = dropChildItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

        //qDebug() << drapData->getProperty()->getJson().toStyledString().c_str();

        QVariant variant;
        std::shared_ptr<dragRole> newData = drapData->clone();
        variant.setValue(newData);

        newChildItem->setData(0, Qt::UserRole, variant);

        Json::Value desJs;
        drapData->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);
        if(!desJs.isNull())
        {
            newChildItem->setText(Name_Index, desJs.asString().c_str());
        }

        Json::Value changeJs;
        drapData->getProperty()->getProperty(PROPERTY_SIM_MODEL, changeJs);
        if(!changeJs.isNull())
        {
            newChildItem->setText(Param_Change_Index, changeJs.asString().c_str());

            std::string strV;

            if(PROPERTY_MODEL_FIX == changeJs.asString())
            {
                Json::Value tmpJs;
                drapData->getProperty()->getProperty(PROPERTY_FIX_VALUE, tmpJs);
                if(!tmpJs.isNull())
                {
                    strV = tmpJs.asString();
                }
            }
            else if(PROPERTY_MODEL_RAND == changeJs.asString())
            {
                Json::Value minJs;
                std::string min, max;
                drapData->getProperty()->getProperty(PROPERTY_RAND_MIN, minJs);
                if(!minJs.isNull())
                {
                    min = minJs.asString();
                }

                Json::Value maxJs;
                drapData->getProperty()->getProperty(PROPERTY_RAND_MAX, maxJs);
                if(!maxJs.isNull())
                {
                    max = maxJs.asString();
                }
                strV = "[" + min + "," + max + "]";
            }
            else if(PROPERTY_MODEL_LINE == changeJs.asString())
            {
                Json::Value minJs;
                std::string min, max, step;
                drapData->getProperty()->getProperty(PROPERTY_LINE_MIN, minJs);
                if(!minJs.isNull())
                {
                    min = minJs.asString();
                }

                Json::Value maxJs;
                drapData->getProperty()->getProperty(PROPERTY_LINE_MAX, maxJs);
                if(!maxJs.isNull())
                {
                    max = maxJs.asString();
                }

                Json::Value stepJs;
                drapData->getProperty()->getProperty(PROPERTY_LINE_STEP, stepJs);
                if(!stepJs.isNull())
                {
                    step = stepJs.asString();
                }

                strV = "[" + min + "," + max + "]:" + step;
            }
            newChildItem->setText(Value_Index, strV.c_str());
            newChildItem->setExpanded(true);
        }

        parentItem->addChild(newChildItem);        
    }

    return parentItem;
}

void flowTree::onDropDrag(QTreeWidgetItem *parentItem, QTreeWidgetItem *curItem, QTreeWidgetItem *dropItem, bool isFrontInsert)
{
    if(!parentItem)
        return ;

    QTreeWidgetItem *insertItem = newItem(dropItem);

    if(isFrontInsert)
    {
        if(curItem == nullptr)
            parentItem->addChild(insertItem);
        else
            parentItem->insertChild(parentItem->indexOfChild(curItem), insertItem);
    }
    else
    {
        if(curItem == nullptr)
            parentItem->addChild(insertItem);
        else
            parentItem->insertChild(parentItem->indexOfChild(curItem) + 1, insertItem);
    }

    ui->treeWidget->expandItem(insertItem);
    ui->treeWidget->expandItem(parentItem);
    ui->treeWidget->update();

    mIsModify = true;
    emit projectModify(mCurProjectUuid);
}

void flowTree::onItemClicked(QTreeWidgetItem * item, int column)
{
    if(!item)
        return ;

    std::shared_ptr<dragRole> drapData = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();
    //qDebug() << drapData.getProperty()->getJson().toStyledString().c_str();
    emit toShowProperty(drapData->getProperty()->getJson());
}

void flowTree::onPropertyValueChange(QString attr, Json::Value value)
{
    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(!curItem)
        return ;

    std::shared_ptr<dragRole> drapData = curItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

    std::string propertyName = attr.toStdString();

    //更新节点关联属性
    drapData->getProperty()->setProperty(propertyName, value);

     //更改树节点显示
    if( (dragRole::Node_Flow == drapData->getNodeType()) || (dragRole::Node_SubFlow == drapData->getNodeType()))
    {
        if(PROPERTY_DESCRIBE == propertyName)
        {
            curItem->setText(Name_Index, value.asString().c_str());
        }
    }
    else if( (dragRole::Node_Cmd == drapData->getNodeType()) || (dragRole::Node_Param_Group == drapData->getNodeType()))
    {
        if(PROPERTY_START_CONDITION == propertyName)
        {
            curItem->setText(Start_Cond_Index, value[PROPERTY_CONDITION_VALUE_NAME].asString().c_str());
        }
        else if(PROPERTY_DELAY == propertyName)
        {
            curItem->setText(Delay_Index, value.asString().c_str());
        }
        else if(PROPERTY_STOP_CONDITION == propertyName)
        {
            curItem->setText(Stop_Cond_Index, value[PROPERTY_CONDITION_VALUE_NAME].asString().c_str());
        }
        else if(PROPERTY_TIMING == propertyName)
        {
            curItem->setText(Timming_Index, value.asString().c_str());
        }
        else if(PROPERTY_DESTDEVICE == propertyName)
        {
            curItem->setText(Dest_Dev_Index, value[PROPERTY_DEV_VALUE_NAME].asString().c_str());
        }
    }
    else if(dragRole::Node_Param == drapData->getNodeType())
    {
        if(PROPERTY_SIM_MODEL == propertyName)
        {
            curItem->setText(Param_Change_Index, value.asString().c_str());
        }
        else if(PROPERTY_FIX_VALUE == propertyName)
        {
            curItem->setText(Value_Index, value.asString().c_str());
        }
        else if( (PROPERTY_RAND_MIN == propertyName) || (PROPERTY_RAND_MAX == propertyName))
        {
            std::string strV;
            std::string min, max;

            Json::Value minJs;
            drapData->getProperty()->getProperty(PROPERTY_RAND_MIN, minJs);
            if(!minJs.isNull())
            {
                min = minJs.asString();
            }

            Json::Value maxJs;
            drapData->getProperty()->getProperty(PROPERTY_RAND_MAX, maxJs);
            if(!maxJs.isNull())
            {
                max = maxJs.asString();
            }
            strV = "[" + min + "," + max + "]";

            curItem->setText(Value_Index, strV.c_str());
        }
        else if( (PROPERTY_LINE_MIN == propertyName) || (PROPERTY_LINE_MAX == propertyName) || (PROPERTY_LINE_STEP == propertyName))
        {
            std::string strV;
            Json::Value minJs;
            std::string min, max, step;
            drapData->getProperty()->getProperty(PROPERTY_LINE_MIN, minJs);
            if(!minJs.isNull())
            {
                min = minJs.asString();
            }

            Json::Value maxJs;
            drapData->getProperty()->getProperty(PROPERTY_LINE_MAX, maxJs);
            if(!maxJs.isNull())
            {
                max = maxJs.asString();
            }

            Json::Value stepJs;
            drapData->getProperty()->getProperty(PROPERTY_LINE_STEP, stepJs);
            if(!stepJs.isNull())
            {
                step = stepJs.asString();
            }

            strV = "[" + min + "," + max + "]:" + step;

            curItem->setText(Value_Index, strV.c_str());
        }
    }

    mIsModify = true;
    emit projectModify(mCurProjectUuid);
}

void flowTree::updateProject()
{
    //TODO:获取数据库，获取文件解析显示

}

void flowTree::onActionTestSend(QTreeWidgetItem *item)
{
    dragRole *role = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();

   auto type = role->getNodeType();
   if(dragRole::Node_Cmd == type)
   {
        createSubFlow subFlow;
        subFlow.setProperty(role->getProperty());
   }
   else if(dragRole::Node_Param_Group == type)
   {
       std::vector<nodeProperty*> subRoles;
       //获取子信息
       for(int index = 0; index < item->childCount(); index++)
       {
           dragRole *tmpRole = (item->child(index))->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();
           subRoles.push_back(tmpRole->getProperty());
       }
       createSubFlow subFlow;
       subFlow.setProperty(role->getProperty(), subRoles);
   }
}

void flowTree::onMenuTrigger(QAction *action)
{
    std::string actionObjName = action->objectName().toStdString();
    if("actionNewFlow" == actionObjName)
    {
        newDialog dlg;
        dlg.setTitleText("新建测试流程");
        dlg.exec();
        if(QDialog::Accepted == dlg.result())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();

            item->setText(0, dlg.getDes());
            //关联数据
            QVariant variant;
            std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();
            dragData->setNodeType(dragRole::Node_Flow);
            dragData->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_Flow])));
            dragData->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(dlg.getDes().toStdString()));
            variant.setValue(dragData);
            item->setData(0,Qt::UserRole,variant);
            if(mCurItem)
            {
                ui->treeWidget->insertTopLevelItem(ui->treeWidget->indexOfTopLevelItem(mCurItem) + 1, item);
            }
            else
            {
                ui->treeWidget->addTopLevelItem(item);
            }
        }
    }
    else if("actionNewSubFlow" == actionObjName)
    {
        newDialog dlg;
        dlg.setTitleText("新建子流程");
        dlg.exec();
        if(QDialog::Accepted == dlg.result())
        {
            QTreeWidgetItem *subitem = new QTreeWidgetItem();

            subitem->setText(0, dlg.getDes());
            //关联数据
            QVariant variant;
            std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();
            dragData->setNodeType(dragRole::Node_SubFlow);
            dragData->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_SubFlow])));
            dragData->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(dlg.getDes().toStdString()));
            variant.setValue(dragData);
            subitem->setData(0, Qt::UserRole, variant);
            if(mCurItem)
            {
                if(mCurItem->parent() == nullptr)
                {
                    mCurItem->addChild(subitem);
                    ui->treeWidget->expandItem(mCurItem);
                }
                else
                {
                    mCurItem->parent()->insertChild(mCurItem->parent()->indexOfChild(mCurItem) + 1, subitem);
                    ui->treeWidget->expandItem(mCurItem->parent());
                }
            }
         }
    }
    else if("actionTestSend" == actionObjName)
    {
        if(mCurItem)
        {
            onActionTestSend(mCurItem);
        }
    }
}

void flowTree::saveProject()
{
    qDebug() << "save project";
    if(!mIsModify)
        return ;

    mIsModify = false;
}
