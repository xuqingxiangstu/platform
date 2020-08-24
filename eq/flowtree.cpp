#include "flowtree.h"
#include "ui_flowtree.h"

#include <iostream>
#include "./toXml/savexml.h"
#include "property/templateproperty.h"
#include "dragTree/dragrole.h"
#include "property/nodeproperty.h"
#include "toXml/createsubflow.h"
#include "newdialog.h"
#include "progresswidget.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"
#include "recordnavigation.h"
#include "progresswidget.h"
#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"
#include "./fromXml/readxml.h"
#include "triggercondition.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QUuid>
#include <QAction>

flowTree::flowTree(QString uuid, int sysType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flowTree),
    mCurProjectUuid(uuid),
    mIsUpdateTree(false),
    mCurSystemType(sysType),
    mCurSelectItem(nullptr),
    mIsCopy(false),
    mCopyItem(nullptr)
{
    ui->setupUi(this);

    mUiUuid = QUuid::createUuid().toString();

    //ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->treeWidget, &PfTreeWidget::dropDrag, this, &flowTree::onDropDrag);
    connect(ui->treeWidget, &PfTreeWidget::itemClicked, this, &flowTree::onItemClicked);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionNewFlow);
    mPopMenu->addAction(ui->actionDelete);
    mPopMenu->addAction(ui->actionCopy);
    mPopMenu->addAction(ui->actionPase);

    connect(mPopMenu, &QMenu::triggered, [=](QAction *action){
        //onMenuTrigger(action);
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
                    ui->actionDelete->setEnabled(false);
                    ui->actionNewFlow->setEnabled(true);

                    ui->actionCopy->setEnabled(false);
                    ui->actionPase->setEnabled(false);

                }
                else if( (dragRole::Node_Cmd == data->getNodeType()) || (dragRole::Node_Param_Group == data->getNodeType()))
                {
                    ui->actionDelete->setEnabled(true);
                    ui->actionNewFlow->setEnabled(true);

                    ui->actionCopy->setEnabled(true);

                    ui->actionPase->setEnabled(true);
                }
                else if((dragRole::Node_Param == data->getNodeType()))
                {
                    ui->actionDelete->setEnabled(false);
                    ui->actionNewFlow->setEnabled(false);

                    ui->actionCopy->setEnabled(false);
                    ui->actionPase->setEnabled(false);
                }
            }
            else
            {
                ui->actionDelete->setEnabled(false);
                ui->actionNewFlow->setEnabled(true);
            }

            mPopMenu->exec(QCursor::pos());
        }
    );

    this->addAction(ui->actionCopy);
    this->addAction(ui->actionPase);
    this->addAction(ui->actionDelete);
    this->addAction(ui->actionNewFlow);

    connect(ui->actionNewFlow, &QAction::triggered,  [=](){
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
    );

    connect(ui->actionCopy, &QAction::triggered, [=](){
        onCopyNode();
    });

    connect(ui->actionDelete, &QAction::triggered, [=](){
        onDeleteNode();
    });

    connect(ui->actionPase, &QAction::triggered, [=](){
        onParseNode();
    });

    ui->treeWidget->expandAll();

    setFocusPolicy(Qt::StrongFocus);

    //初始化启动终止条件初始值
    triggerCondition::getInstance();
}

flowTree::~flowTree()
{
    delete ui;
}

void flowTree::onSearch(QString uuid, Json::Value condition)
{
    if(mCurProjectUuid.compare(uuid) != 0)
        return ;

    //遍历查找
    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        std::shared_ptr<dragRole> drapData = (*Itor)->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

        if( (dragRole::Node_Cmd == drapData->getNodeType())
                || (dragRole::Node_Param_Group == drapData->getNodeType())
                )
        {
            QString tmpText = (*Itor)->text(Table_Coding_Index);
            QStringList splitResult = tmpText.split("_");
            QString table = splitResult.at(0);
            QString coding = splitResult.at(1);

            bool isFind = false;

            if(!condition["table"].isNull())
            {
                if(table.toStdString() == condition["table"].asString())
                    isFind = true;
            }

            //当为参数时不判断编码
            if(dragRole::Node_Param_Group != drapData->getNodeType())
            {
                if(!condition["coding"].isNull())
                {
                    if(isFind)
                    {
                        if(coding.toStdString() == condition["coding"].asString())
                            isFind = true;
                        else
                            isFind = false;
                    }
                }
            }
            if(isFind)
            {
                //找到信息

                Json::Value resultJs;

                //项目名称
                resultJs["project_name"] = ui->testProductLab->text().toStdString();

                //项目uuid
                resultJs["project_uuid"] = mCurProjectUuid.toStdString();

                //流程名称
                resultJs["item_name"] = (*Itor)->text(Name_Index).toStdString();

                //流程uuid
                resultJs["item_uuid"] = drapData->itemUuid();

                emit searchResult(resultJs);
            }
        }

        ++Itor;
    }
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

void flowTree::updateCmdOrParamGroupItemValue(QTreeWidgetItem *item, std::shared_ptr<dragRole> drapData)
{
    Json::Value desJs;
    drapData->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);
    if(!desJs.isNull())
        item->setText(Name_Index, desJs.asString().c_str());

    //modify xqx 2020-8-24 增加表号编码

    Json::Value tableJs;
    drapData->getProperty()->getProperty(PROPERTY_BASE_TABLE_NUM, tableJs);

    Json::Value codingJs;
    drapData->getProperty()->getProperty(PROPERTY_BASE_CODING_NUM, codingJs);
    if(!codingJs.isNull() && !tableJs.isNull())
        item->setText(Table_Coding_Index, std::string(tableJs.asString() + "_" + codingJs.asString()).c_str());

    //end

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
#if 0 //modify xqx 2020-4-26
    Json::Value desDevJs;
    drapData->getProperty()->getProperty(PROPERTY_DESTDEVICE, desDevJs);
    if(!desDevJs.isNull())
    {
        item->setText(Dest_Dev_Index, desDevJs[PROPERTY_DEV_VALUE_NAME].asString().c_str());
    }
#endif
}

QTreeWidgetItem *flowTree::newCmdItem(QTreeWidgetItem *dropItem)
{
    std::shared_ptr<dragRole> drapData = dropItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

    QTreeWidgetItem *item = new QTreeWidgetItem();

    QVariant variant;
    std::shared_ptr<dragRole> newData = drapData->clone();
    variant.setValue(newData);

    newData->setItemUuid(QUuid::createUuid().toString().toStdString());

    item->setData(0, Qt::UserRole, variant);

    updateCmdOrParamGroupItemValue(item, newData);

    item->setExpanded(true);

    return item;
}

void flowTree::updateParamItemValue(QTreeWidgetItem *newChildItem, std::shared_ptr<dragRole> drapData)
{
    Json::Value desJs;
    drapData->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);
    if(!desJs.isNull())
    {
        newChildItem->setText(Name_Index, desJs.asString().c_str());
    }

    //modify xqx 2020-8-24 增加表号编码

    Json::Value tableJs;
    drapData->getProperty()->getProperty(PROPERTY_BASE_TABLE_NUM, tableJs);

    Json::Value codingJs;
    drapData->getProperty()->getProperty(PROPERTY_BASE_CODING_NUM, codingJs);
    if(!codingJs.isNull() && !tableJs.isNull())
        newChildItem->setText(Table_Coding_Index, std::string(tableJs.asString() + "_" + codingJs.asString()).c_str());

    //end

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

        newData->setItemUuid(QUuid::createUuid().toString().toStdString());

        newChildItem->setData(0, Qt::UserRole, variant);

        updateParamItemValue(newChildItem, newData);

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

    //ui->treeWidget->expandItem(insertItem);
    //ui->treeWidget->expandItem(parentItem);
    ui->treeWidget->update();

    emit projectModify(mCurProjectUuid);
}

void flowTree::onItemClicked(QTreeWidgetItem * item, int column)
{
    if(!item || mCurSelectItem == item)
        return ;   

    std::shared_ptr<dragRole> drapData = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();


    if( ( (dragRole::Node_Cmd == drapData->getNodeType()) || (dragRole::Node_Param_Group == drapData->getNodeType())))
    {
        updateFrameAttr(drapData);

        //更新信息字相关属性
        updateInfoWordAttr(drapData);
    }
    else if(dragRole::Node_Param == drapData->getNodeType())
    {
        //更新参数变化属性
        updateParamSelAttr(drapData);
    }

    Json::Value startJs;
    drapData->getProperty()->getProperty(PROPERTY_START_CONDITION, startJs);
    if(!startJs.isNull())
    {
        qDebug() << startJs.toStyledString().c_str();
    }


    //modify xqx 2020-6-20 09:17:05 在此处增加启动条件初始值，在模板中加载时内存太大

    nodeProperty tmpProperty(drapData->getProperty()->getJson());

    Json::Value initValue;
    triggerCondition::getInstance()->getInitValue(initValue);

    if(!initValue.isNull())
    {
        tmpProperty.setInitValue(PROPERTY_START_CONDITION, initValue);
        tmpProperty.setInitValue(PROPERTY_STOP_CONDITION, initValue);
    }

    //end

    emit toShowProperty(mUiUuid, tmpProperty.getJson());

    mCurSelectItem = item;
}

void flowTree::onPropertyValueChange(QString uuid, QString attr, Json::Value value)
{
    if(mUiUuid.compare(uuid) != 0)
        return ;

    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(!curItem)
        return ;

    std::shared_ptr<dragRole> drapData = curItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

    std::string propertyName = attr.toStdString();

    //更新节点关联属性
    drapData->getProperty()->setProperty(propertyName, value);
#if 1 //modify xqx 2020-4-26
    //接口选择后需要更新系统类型,从数据库中获取
    if((dragRole::Node_Cmd == drapData->getNodeType()) || (dragRole::Node_Param_Group == drapData->getNodeType()))
    {
        if( (propertyName == PROPERTY_LOCAL_DEVICE) || (propertyName == PROPERTY_DEST_DEVICE) )
        {
            Json::Value devJs;
            drapData->getProperty()->getProperty(PROPERTY_LOCAL_DEVICE, devJs);
            if(devJs.isObject())
            {
                std::string devUuid = devJs[PROPERTY_DEV_VALUE_UUID].asString();
                Json::Value devJs = sysInterfaceTable::getInstance()->getValue(devUuid);

                Json::Value sysInfoJs = systemTable::getInstance()->getSysInfoByUuid(devJs[SYSTEM_INTERFACE_TABLE_SYSTEM_UUID].asString());

                if(!sysInfoJs.isNull())
                {
                    emit updateProperty(PROPERTY_SRC_SYS_TYPE, sysInfoJs[SYSTEM_TABLE_SYSTEM_TYPE]);

                    drapData->getProperty()->setProperty(PROPERTY_SRC_SYS_TYPE, sysInfoJs[SYSTEM_TABLE_SYSTEM_TYPE]);
                }
            }
        }
    }
#endif
     //更改树节点显示
    if( (dragRole::Node_Flow == drapData->getNodeType()))
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
        else if(PROPERTY_1553B_MODE == propertyName)
        {
            enable1553BAttr(drapData, false);
        }
#if 0//modify xqx 2020-4-26
        else if(PROPERTY_DESTDEVICE == propertyName)
        {
            curItem->setText(Dest_Dev_Index, value[PROPERTY_DEV_VALUE_NAME].asString().c_str());
        }
#endif
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

    emit projectModify(mCurProjectUuid);
}

void flowTree::onShowCurItemProperty(QString uuid)
{
    if(mCurProjectUuid.compare(uuid) != 0)
        return ;

    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(item)
    {
        std::shared_ptr<dragRole> drapData = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();
        //qDebug() << drapData->getProperty()->getJson().toStyledString().c_str();

        //modify xqx 2020-6-20 09:17:05 在此处增加启动条件初始值，在模板中加载时内存太大

        nodeProperty tmpProperty(drapData->getProperty()->getJson());

        Json::Value initValue;
        triggerCondition::getInstance()->getInitValue(initValue);

        if(!initValue.isNull())
        {
            tmpProperty.setInitValue(PROPERTY_START_CONDITION, initValue);
            tmpProperty.setInitValue(PROPERTY_STOP_CONDITION, initValue);
        }

        //end

        emit toShowProperty(mUiUuid, tmpProperty.getJson());
    }
    else
    {
        //清空
        emit toShowProperty(mUiUuid, Json::Value());
    }
}

void flowTree::onActionDelete(QTreeWidgetItem *item)
{   
    if(QMessageBox::Yes == QMessageBox::warning(this, "提示", "是否删除",QMessageBox::Yes | QMessageBox::No))
    {
        //删除item
        deleteItem(item);

        //清空属性窗体
        emit toShowProperty(mUiUuid, Json::Value());

        //增加修改提示
        emit projectModify(mCurProjectUuid);
    }
}

void flowTree::deleteItem(QTreeWidgetItem *item)
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
        QTreeWidgetItem *del = ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        if(del)
            delete del;
    }
}


void flowTree::onActionTestSend(QTreeWidgetItem *item)
{
    dragRole *role = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();

    //发给后台逻辑进行测试，接受测试结果

    Json::Value testJson;

    testJson["node"] = role->getProperty()->getJson();

    auto type = role->getNodeType();
    if(dragRole::Node_Cmd == type)
    {
    }
    else if(dragRole::Node_Param_Group == type)
    {
        Json::Value tmpJson;

       //获取子信息
       for(int index = 0; index < item->childCount(); index++)
       {
           dragRole *tmpRole = (item->child(index))->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();

           tmpJson.append(tmpRole->getProperty()->getJson());
       }

       testJson["subNode"] = tmpJson;
    }

    //由后台逻辑进行处理
    //emit singleTeset(testJson);
}

void flowTree::onCopyNode()
{
    mIsCopy = false;

    mCopyItem = ui->treeWidget->currentItem();
    if(mCopyItem)
    {
        dragRole *role = mCopyItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();

        auto type = role->getNodeType();
        if( (dragRole::Node_Cmd == type) || (dragRole::Node_Param_Group == type))
        {
            mIsCopy = true;
        }
    }
}

void flowTree::onParseNode()
{
    if(mIsCopy && mCopyItem)
    {
        mIsCopy = false;

        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        if(item)
        {
            QTreeWidgetItem *insertItem = newItem(mCopyItem);

            dragRole *role = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();
            auto type = role->getNodeType();
            if( (dragRole::Node_Cmd == type) || (dragRole::Node_Param_Group == type))
            {
                QTreeWidgetItem *parentItem = item->parent();
                if(parentItem)
                {
                    parentItem->insertChild(parentItem->indexOfChild(item) + 1, insertItem);

                    //增加修改提示
                    emit projectModify(mCurProjectUuid);
                }
            }
            else if(dragRole::Node_Flow == type)
            {
                item->addChild(insertItem);

                //增加修改提示
                emit projectModify(mCurProjectUuid);
            }
        }
    }
}

void flowTree::onDeleteNode()
{
    mCurItem = ui->treeWidget->currentItem();

    if(mCurItem)
    {
        dragRole *role = mCurItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();

        auto type = role->getNodeType();
        if( (dragRole::Node_Cmd == type) || (dragRole::Node_Param_Group == type))
        {
            onActionDelete(mCurItem);
        }
    }
}

void flowTree::onNewFlowNode()
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
#if 0
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
#endif
    else if("actionTestSend" == actionObjName)
    {
        if(mCurItem)
        {
            onActionTestSend(mCurItem);
        }
    }
    else if("actionDelete" == actionObjName)
    {
        if(mCurItem)
        {
            onActionDelete(mCurItem);
        }
    }
}


int flowTree::getItemSize(QTreeWidget *curWidget)
{
    if(!curWidget)
        return 0;

    int size = 0;

    QTreeWidgetItemIterator Itor(curWidget);

    while (*Itor)
    {
        size++;
        ++Itor;
    }

    return size;
}

void flowTree::onSaveProject(QTreeWidgetItem *item)
{
    recordRole role = item->data(0, Qt::UserRole).value<recordRole>();

    if(mCurProjectUuid.toStdString() != role.uuid)
        return ;

    //获取item总数
    int itemSize = getItemSize(ui->treeWidget);

    QString filePath = "./flowFile/" + QString::fromStdString(role.uuid) + ".xml";

    saveXml xmlObj;
    xmlObj.setFilePath(filePath);
    xmlObj.setTreeWidget(ui->treeWidget);
    xmlObj.setSystemInfo(item);
    progressWidget dlg;

    connect(&xmlObj, &saveXml::saveOver, &dlg, &progressWidget::onClose);

    xmlObj.startTask();

    dlg.exec();

    emit saveProjectOver(mCurProjectUuid);    
}

void flowTree::onUpdateProject(QString uuid)
{
    if(mIsUpdateTree)
        return ;

    if(mCurProjectUuid.compare(uuid) != 0)
        return ;

    QString filePath = "./flowFile/" + mCurProjectUuid + ".xml";
    QFileInfo fileObj(filePath);

    if(!fileObj.exists())
        QMessageBox::information(this, "提示", "文件不存在：" + filePath);

    readXml readObj;
    readObj.setFilePath(filePath);

    progressWidget dlg;
    //dlg.onMinMaxValue(0, itemSize);

    connect(&readObj, &readXml::readOver, &dlg, &progressWidget::onClose);

    connect(&readObj, &readXml::flowItemValue, this, &flowTree::setFlowItemValue);
    connect(&readObj, &readXml::subFlowItemValue, this, &flowTree::setSubFlowItemValue);    
    connect(&readObj, SIGNAL(cmdItemValue(QString, const std::shared_ptr<dragRole>&)), this, SLOT(setCmdItemValue(QString,const std::shared_ptr<dragRole>&)));
    connect(&readObj, SIGNAL(paramItemValue(QString,const std::shared_ptr<dragRole> &, const std::vector<std::shared_ptr<dragRole>> &)), this, SLOT(setParamItemValue(QString, const std::shared_ptr<dragRole> &, const std::vector<std::shared_ptr<dragRole>> &)));

    readObj.startTask();

    dlg.exec();

    mIsUpdateTree = true;
}

QTreeWidgetItem *flowTree::findItem(QString uuid)
{
    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        std::shared_ptr<dragRole> role = (*Itor)->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

        if(uuid.toStdString() == role->uuid())
        {
            item = *Itor;
            break;
        }

        ++Itor;
    }

    return item;
}

void flowTree::setFlowItemValue(const std::shared_ptr<dragRole> &role)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    Json::Value desJs;
    role->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);

    if(!desJs.isNull())
        item->setText(0, desJs.asString().c_str());

    //关联数据
    QVariant variant;
    variant.setValue(role);
    item->setData(0,Qt::UserRole,variant);

    ui->treeWidget->addTopLevelItem(item);
}

void flowTree::setSubFlowItemValue(QString flowUuid, const std::shared_ptr<dragRole> &role)
{
    //查找UUID
    QTreeWidgetItem *flowItem = findItem(flowUuid);
    if(flowItem)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();

        Json::Value desJs;
        role->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);

        if(!desJs.isNull())
            item->setText(0, desJs.asString().c_str());

        //关联数据
        QVariant variant;
        variant.setValue(role);
        item->setData(0,Qt::UserRole,variant);

        flowItem->addChild(item);
    }

    ui->treeWidget->expandItem(flowItem);

}

QTreeWidgetItem *flowTree::getPasteBrother(const dragRole::nodeType &parseType)
{
    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();

    if(curItem)
    {
        dragRole *role = curItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>().get();

        auto type = role->getNodeType();

        if(dragRole::Node_Flow == type)
        {

        }
        else if(dragRole::Node_Param_Group == type)
        {

        }
        else if(dragRole::Node_Cmd == type)
        {

        }
    }

}

void flowTree::onPasteCmdItem(std::shared_ptr<dragRole> role)
{

}

void flowTree::setCmdItemValue(QString subFlowUuid, const std::shared_ptr<dragRole> &role)
{
    QTreeWidgetItem *subFlowItem = findItem(subFlowUuid);
    if(subFlowItem)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();

        Json::Value desJs;
        role->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);

        if(!desJs.isNull())
            item->setText(0, desJs.asString().c_str());

        //关联数据
        QVariant variant;
        variant.setValue(role);
        item->setData(0,Qt::UserRole,variant);        

        updateCmdOrParamGroupItemValue(item, role);

        subFlowItem->addChild(item);
    }

    ui->treeWidget->expandItem(subFlowItem);
}

void flowTree::setParamItemValue(QString subFlowUuid, const std::shared_ptr<dragRole> &role, const std::vector<std::shared_ptr<dragRole>> &subRoles)
{
    QTreeWidgetItem *subFlowItem = findItem(subFlowUuid);
    if(subFlowItem)
    {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem();

        Json::Value desJs;
        role->getProperty()->getProperty(PROPERTY_DESCRIBE, desJs);

        if(!desJs.isNull())
            groupItem->setText(0, desJs.asString().c_str());

        //关联数据
        QVariant variant;
        variant.setValue(role);
        groupItem->setData(0,Qt::UserRole,variant);

        role->setItemUuid(QUuid::createUuid().toString().toStdString());

        updateCmdOrParamGroupItemValue(groupItem, role);

        for(std::shared_ptr<dragRole> subRole : subRoles)
        {
            QTreeWidgetItem *paramItem = new QTreeWidgetItem();

            Json::Value paramDesJs;
            subRole->getProperty()->getProperty(PROPERTY_DESCRIBE, paramDesJs);

            if(!paramDesJs.isNull())
                paramItem->setText(0, paramDesJs.asString().c_str());

            //关联数据
            QVariant variant1;
            variant1.setValue(subRole);
            paramItem->setData(0,Qt::UserRole,variant1);

            updateParamItemValue(paramItem, subRole);

            groupItem->addChild(paramItem);
        }

        subFlowItem->addChild(groupItem);
    }

    ui->treeWidget->expandItem(subFlowItem);
}

void flowTree::updateParamSelAttr(std::shared_ptr<dragRole> role)
{
    Json::Value infoJs;
    role->getProperty()->getProperty(PROPERTY_SIM_MODEL, infoJs);
    if(infoJs.isNull() || !infoJs.isString())
        return ;

    std::string changeType = infoJs.asString();

    if(PROPERTY_MODEL_FIX == changeType)
    {
        emit setGroupPropertyEnable(PROPERTY_MODEL_FIX, true);
        emit setGroupPropertyEnable(PROPERTY_MODEL_RAND, false);
        emit setGroupPropertyEnable(PROPERTY_MODEL_LINE, false);

        //role->getProperty()->setVisible(PROPERTY_MODEL_FIX, true);
        //role->getProperty()->setVisible(PROPERTY_MODEL_RAND, false);
        //role->getProperty()->setVisible(PROPERTY_MODEL_LINE, false);
    }
    else if(PROPERTY_MODEL_RAND == changeType)
    {
        emit setGroupPropertyEnable(PROPERTY_MODEL_FIX, false);
        emit setGroupPropertyEnable(PROPERTY_MODEL_RAND, true);
        emit setGroupPropertyEnable(PROPERTY_MODEL_LINE, false);

       // role->getProperty()->setVisible(PROPERTY_MODEL_FIX, false);
       // role->getProperty()->setVisible(PROPERTY_MODEL_RAND, true);
        //role->getProperty()->setVisible(PROPERTY_MODEL_LINE, false);
    }
    else if(PROPERTY_MODEL_LINE == changeType)
    {
        emit setGroupPropertyEnable(PROPERTY_MODEL_FIX, false);
        emit setGroupPropertyEnable(PROPERTY_MODEL_RAND, false);
        emit setGroupPropertyEnable(PROPERTY_MODEL_LINE, true);

        //role->getProperty()->setVisible(PROPERTY_MODEL_FIX, false);
        //role->getProperty()->setVisible(PROPERTY_MODEL_RAND, false);
        //role->getProperty()->setVisible(PROPERTY_MODEL_LINE, true);
    }
}

void flowTree::updateInfoWordAttr(std::shared_ptr<dragRole> role)
{
    if( (mCurFrameType == PROPERTY_FRAME_BE))
    {
        //删除其它属性
        emit removeGroupProperty(PROPERTY_OTHER);
        emit addGroupProperty(PROPERTY_OTHER);

        emit addProperty(PROPERTY_OTHER, role->getProperty()->curAttr(PROPERTY_OTHER_CL_TYPE));
        emit addProperty(PROPERTY_OTHER, role->getProperty()->curAttr(PROPERTY_OTHER_CL_NUM));

        role->getProperty()->setVisible(PROPERTY_OTHER, true);
        role->getProperty()->setVisible(PROPERTY_OTHER_D_NUM, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_TYPE, true);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_NUM, true);
        role->getProperty()->setVisible(PROPERTY_OTHER_DEV_INDEX, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_MODLE_INDEX, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_RESERVE, false);
    }
    else if( (PROPERTY_FRAME_FE == mCurFrameType ))
    {
        //删除其它属性
        emit removeGroupProperty(PROPERTY_OTHER);

        role->getProperty()->setVisible(PROPERTY_OTHER, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_D_NUM, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_TYPE, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_NUM, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_DEV_INDEX, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_MODLE_INDEX, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_RESERVE, false);
    }
    else if( (PROPERTY_FRAME_93 == mCurFrameType ))
    {
        //删除其它属性
        emit removeGroupProperty(PROPERTY_OTHER);

        role->getProperty()->setVisible(PROPERTY_OTHER, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_D_NUM, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_TYPE, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_NUM, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_DEV_INDEX, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_MODLE_INDEX, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_RESERVE, false);
    }
    else if( (PROPERTY_FRAME_MIDDLE == mCurFrameType ))
    {
        //删除其它属性
        emit removeGroupProperty(PROPERTY_OTHER);
        emit addGroupProperty(PROPERTY_OTHER);        

        emit addProperty(PROPERTY_OTHER, role->getProperty()->curAttr(PROPERTY_OTHER_D_NUM));

        int infoType = 0;
        Json::Value infoJs;
        role->getProperty()->getProperty(PROPERTY_BASE_INFO_WORD, infoJs);
        if(!infoJs.isNull() && infoJs.isInt())
        {
            infoType = infoJs.asInt();
        }

        if(2 == infoType)
        {
            emit addProperty(PROPERTY_OTHER, role->getProperty()->curAttr(PROPERTY_OTHER_DEV_INDEX));
            emit addProperty(PROPERTY_OTHER, role->getProperty()->curAttr(PROPERTY_OTHER_MODLE_INDEX));
            role->getProperty()->setVisible(PROPERTY_OTHER_DEV_INDEX, true);
            role->getProperty()->setVisible(PROPERTY_OTHER_MODLE_INDEX, true);
        }
        else
        {
            role->getProperty()->setVisible(PROPERTY_OTHER_DEV_INDEX, false);
            role->getProperty()->setVisible(PROPERTY_OTHER_MODLE_INDEX, false);
        }
        emit addProperty(PROPERTY_OTHER, role->getProperty()->curAttr(PROPERTY_OTHER_RESERVE));

        role->getProperty()->setVisible(PROPERTY_OTHER_D_NUM, true);
        role->getProperty()->setVisible(PROPERTY_OTHER_RESERVE, true);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_TYPE, false);
        role->getProperty()->setVisible(PROPERTY_OTHER_CL_NUM, false);
        role->getProperty()->setVisible(PROPERTY_OTHER, true);
    }
}

void flowTree::enable1553BAttr(std::shared_ptr<dragRole> role, bool isNew)
{
    if(isNew)
    {
        emit addGroupProperty(PROPERTY_1553B);
        emit addProperty(PROPERTY_1553B, role->getProperty()->curAttr(PROPERTY_1553B_MODE));
        emit addProperty(PROPERTY_1553B, role->getProperty()->curAttr(PROPERTY_1553B_BUS));

        role->getProperty()->setVisible(PROPERTY_1553B, true);
        role->getProperty()->setVisible(PROPERTY_1553B_MODE, true);
        role->getProperty()->setVisible(PROPERTY_1553B_BUS, true);
    }

    Json::Value modeJs;
    role->getProperty()->getProperty(PROPERTY_1553B_MODE, modeJs);
    if(!modeJs.isNull() && modeJs.isString())
    {
        std::string m1553BMode = modeJs.asString();

        if(PROPERTY_1553B_MODE_BC_RT == m1553BMode)
        {
            emit removeGroupProperty(PROPERTY_1553B_RT_RT);
            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT, false);

            emit addGroupProperty(PROPERTY_1553B_BC_RT);
            emit addProperty(PROPERTY_1553B_BC_RT, role->getProperty()->curAttr(PROPERTY_1553B_BC_RT_RT_ADDR));
            emit addProperty(PROPERTY_1553B_BC_RT, role->getProperty()->curAttr(PROPERTY_1553B_BC_RT_SA_ADDR));

            role->getProperty()->setVisible(PROPERTY_1553B_BC_RT, true);
            role->getProperty()->setVisible(PROPERTY_1553B_BC_RT_RT_ADDR, true);
            role->getProperty()->setVisible(PROPERTY_1553B_BC_RT_SA_ADDR, true);

        }
        else if(PROPERTY_1553B_MODE_RT_RT == m1553BMode)
        {
            emit removeGroupProperty(PROPERTY_1553B_BC_RT);
            role->getProperty()->setVisible(PROPERTY_1553B_BC_RT, false);

            emit addGroupProperty(PROPERTY_1553B_RT_RT);
            emit addProperty(PROPERTY_1553B_RT_RT, role->getProperty()->curAttr(PROPERTY_1553B_RT_RT_S_RT_ADDR));
            emit addProperty(PROPERTY_1553B_RT_RT, role->getProperty()->curAttr(PROPERTY_1553B_RT_RT_S_SA_ADDR));
            emit addProperty(PROPERTY_1553B_RT_RT, role->getProperty()->curAttr(PROPERTY_1553B_RT_RT_R_RT_ADDR));
            emit addProperty(PROPERTY_1553B_RT_RT, role->getProperty()->curAttr(PROPERTY_1553B_RT_RT_R_SA_ADDR));
            emit addProperty(PROPERTY_1553B_RT_RT, role->getProperty()->curAttr(PROPERTY_1553B_RT_RT_DATA_SIZE));

            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT, true);
            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT_S_RT_ADDR, true);
            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT_S_SA_ADDR, true);
            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT_R_RT_ADDR, true);
            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT_R_SA_ADDR, true);
            role->getProperty()->setVisible(PROPERTY_1553B_RT_RT_DATA_SIZE, true);
        }
    }
}

void flowTree::disable1553BAttr(std::shared_ptr<dragRole> role)
{
    emit removeGroupProperty(PROPERTY_1553B);
    role->getProperty()->setVisible(PROPERTY_1553B, false);

    emit removeGroupProperty(PROPERTY_1553B_BC_RT);
    role->getProperty()->setVisible(PROPERTY_1553B_BC_RT, false);

    emit removeGroupProperty(PROPERTY_1553B_RT_RT);
    role->getProperty()->setVisible(PROPERTY_1553B_RT_RT, false);
}

void flowTree::updateFrameAttr(std::shared_ptr<dragRole> role)
{
    //更新当前节点变化

    if( (mCurFrameType == PROPERTY_FRAME_BE))
    {
        disable1553BAttr(role);

        //删除信宿属性
        emit removeGroupProperty(PROPERTY_DST);
        //创建组
        emit addGroupProperty(PROPERTY_DST);

        //加入属性
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_SYS_TYPE));
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_SYS_CODING));
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_NODE_CODING));


        role->getProperty()->setVisible(PROPERTY_SRC_SYS_TYPE, true);
        role->getProperty()->setVisible(PROPERTY_SRC_SYS_CODING, true);
        role->getProperty()->setVisible(PROPERTY_SRC_NODE_CODING, true);

        role->getProperty()->setVisible(PROPERTY_USER, false);
        role->getProperty()->setVisible(PROPERTY_SOFT_VERSION, false);

        role->getProperty()->setReadOnly(PROPERTY_DST, false);
        role->getProperty()->setVisible(PROPERTY_DST, true);

        //删除数据帧类型
        emit removeGroupProperty(PROPERTY_FE_DATA_TYPE);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE, false);
    }
    else if((PROPERTY_FRAME_JG == mCurFrameType ) || (PROPERTY_FRAME_CZXK == mCurFrameType ))
    {
        disable1553BAttr(role);

        emit removeGroupProperty(PROPERTY_DST);

        //创建组
        emit addGroupProperty(PROPERTY_DST);

        //加入属性
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_SYS_TYPE));

        role->getProperty()->setVisible(PROPERTY_SRC_SYS_TYPE, true);
        role->getProperty()->setVisible(PROPERTY_SRC_SYS_CODING, false);
        role->getProperty()->setVisible(PROPERTY_SRC_NODE_CODING, false);

        role->getProperty()->setVisible(PROPERTY_USER, false);
        role->getProperty()->setVisible(PROPERTY_SOFT_VERSION, false);

        role->getProperty()->setReadOnly(PROPERTY_DST, false);
        role->getProperty()->setVisible(PROPERTY_DST, true);

        //删除数据帧类型
        emit removeGroupProperty(PROPERTY_FE_DATA_TYPE);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE, false);
    }
    else if( (PROPERTY_FRAME_FE == mCurFrameType ) )
    {
        disable1553BAttr(role);

        emit removeGroupProperty(PROPERTY_DST);

        //创建组
        emit addGroupProperty(PROPERTY_DST);

        //加入属性
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_SYS_TYPE));

        role->getProperty()->setVisible(PROPERTY_SRC_SYS_TYPE, true);
        role->getProperty()->setVisible(PROPERTY_SRC_SYS_CODING, false);
        role->getProperty()->setVisible(PROPERTY_SRC_NODE_CODING, false);

        role->getProperty()->setVisible(PROPERTY_USER, false);
        role->getProperty()->setVisible(PROPERTY_SOFT_VERSION, false);

        role->getProperty()->setReadOnly(PROPERTY_DST, false);
        role->getProperty()->setVisible(PROPERTY_DST, true);

        emit removeGroupProperty(PROPERTY_FE_DATA_TYPE);

        //创建组
        emit addGroupProperty(PROPERTY_FE_DATA_TYPE);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE, true);        
        //加入属性        
        emit addProperty(PROPERTY_FE_DATA_TYPE, role->getProperty()->curAttr(PROPERTY_FE_DATA_TYPE_SEND_SYS));
        emit addProperty(PROPERTY_FE_DATA_TYPE, role->getProperty()->curAttr(PROPERTY_FE_DATA_TYPE_RCV_SYS));
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE_SEND_SYS, true);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE_RCV_SYS, true);
    }
    else if( (PROPERTY_FRAME_93 == mCurFrameType ))
    {
        disable1553BAttr(role);

        //删除属性，并设置隐藏
        emit removeGroupProperty(PROPERTY_DST);
        role->getProperty()->setVisible(PROPERTY_DST, false);

        //删除数据帧类型
        emit removeGroupProperty(PROPERTY_FE_DATA_TYPE);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE, false);
    }
    else if( PROPERTY_FRAME_1553B == mCurFrameType)
    {
        enable1553BAttr(role);

        //删除其它属性
        emit removeGroupProperty(PROPERTY_OTHER);

        role->getProperty()->setVisible(PROPERTY_OTHER, false);

        emit removeGroupProperty(PROPERTY_DST);
        role->getProperty()->setVisible(PROPERTY_DST, false);        

        //删除数据帧类型
        emit removeGroupProperty(PROPERTY_FE_DATA_TYPE);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE, false);
    }
    else if(mCurFrameType == PROPERTY_FRAME_MIDDLE)
    {
        disable1553BAttr(role);

        emit removeGroupProperty(PROPERTY_DST);

        //创建组
        emit addGroupProperty(PROPERTY_DST);

        //加入属性
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_SYS_TYPE));
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_SYS_CODING));
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SRC_NODE_CODING));

        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_USER));
        emit addProperty(PROPERTY_DST, role->getProperty()->curAttr(PROPERTY_SOFT_VERSION));

        role->getProperty()->setVisible(PROPERTY_SRC_SYS_TYPE, true);
        role->getProperty()->setVisible(PROPERTY_SRC_SYS_CODING, true);
        role->getProperty()->setVisible(PROPERTY_SRC_NODE_CODING, true);

        role->getProperty()->setVisible(PROPERTY_USER, true);
        role->getProperty()->setVisible(PROPERTY_SOFT_VERSION, true);

        role->getProperty()->setReadOnly(PROPERTY_DST, false);
        role->getProperty()->setVisible(PROPERTY_DST, true);

        //删除数据帧类型
        emit removeGroupProperty(PROPERTY_FE_DATA_TYPE);
        role->getProperty()->setVisible(PROPERTY_FE_DATA_TYPE, false);
    }
}

void flowTree::onFrameTypeChange(QString uuid, QString type)
{
    if(mCurProjectUuid.compare(uuid) != 0)
        return ;

    mCurFrameType = type.toStdString();

    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(!item)
        return ;

    std::shared_ptr<dragRole> role = item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

    if( !( (dragRole::Node_Cmd == role->getNodeType()) || (dragRole::Node_Param_Group == role->getNodeType())))
        return ;

    //更新帧类型相关属性
    updateFrameAttr(role);

    //更新信息字相关属性
    updateInfoWordAttr(role);

    //更新参数变化属性
    //updateParamSelAttr(role);

    //modify xqx 2020-6-20 09:17:05 在此处增加启动条件初始值，在模板中加载时内存太大

    nodeProperty tmpProperty(role->getProperty()->getJson());

    Json::Value initValue;
    triggerCondition::getInstance()->getInitValue(initValue);

    if(!initValue.isNull())
    {
        tmpProperty.setInitValue(PROPERTY_START_CONDITION, initValue);
        tmpProperty.setInitValue(PROPERTY_STOP_CONDITION, initValue);
    }

    //end

    emit toShowProperty(mUiUuid, tmpProperty.getJson());
}

void flowTree::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::CTRL) && (event->key() == Qt::Key_N))//组合键触发
    {
        //onNewFlow();
    }
    qDebug() << event->key();
}
