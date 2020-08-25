#include "recordnavigation.h"
#include "ui_recordnavigation.h"

#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/flowrecordtable.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"
#include "./property/templateproperty.h"
#include "newprojectdialog.h"

#include "./toXml/nullxml.h"
#include "./toXml/copyxml.h"

#include <iostream>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QUuid>

recordNavigation::recordNavigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recordNavigation)
{
    ui->setupUi(this);

    mUiUuid = QUuid::createUuid().toString();

    mPopMenu = new QMenu(this);
    mPopMenu->addAction(ui->actionNewProject);
    mPopMenu->addAction(ui->actionDelete);
    mPopMenu->addAction(ui->actionCopy);
    mPopMenu->addAction(ui->actionPaste);


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
                ui->actionCopy->setEnabled(false);
                ui->actionPaste->setEnabled(true);
            }
            else if(Flow_Node == role.nodeType)
            {
                ui->actionDelete->setEnabled(true);
                ui->actionNewProject->setEnabled(false);
                ui->actionCopy->setEnabled(true);
                ui->actionPaste->setEnabled(true);
            }

            mPopMenu->exec(QCursor::pos());
        }

    });    
}

recordNavigation::~recordNavigation()
{
    delete ui;
}

void recordNavigation::onSaveProject(QTreeWidgetItem *item)
{
    if(!item)
        return ;

    if(!mIsModify.contains(mCurSelectUuid))
    {
        mIsModify[mCurSelectUuid] = false;
    }

    if(mIsModify[mCurSelectUuid])
    {
        recordRole role = item->data(0, Qt::UserRole).value<recordRole>();

        if(Flow_Node == role.nodeType)
        {
            //记录中增加节点
            flowRecordTable::getInstance()->updateNodeInfo(role.uuid, role.mNodeProperty->getJson().toStyledString());
        }       
    }
}

QTreeWidgetItem *recordNavigation::findItem(QString uuid)
{
    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        recordRole role = (*Itor)->data(0, Qt::UserRole).value<recordRole>();

        if(uuid.toStdString() == role.uuid)
        {
            item = *Itor;
            break;
        }

        ++Itor;
    }

    return item;
}

QStringList recordNavigation::getModifyProject()
{
    QStringList modifyUuid;

    foreach(const QString &key, mIsModify.keys())
    {
        if(mIsModify[key])
            modifyUuid.append(key);
    }

    return modifyUuid;
}

bool recordNavigation::isModify(QString uuid)
{
    if(!mIsModify.contains(uuid))
    {
        mIsModify[uuid] = false;
    }

    return mIsModify[uuid];
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
        QTreeWidgetItem *sysNode = createNode("", sysUuid, des, System_Node);

        //根据系统uuid获取响应记录
        Json::Value recordJs = flowRecordTable::getInstance()->getValueBySystemUuid(sysUuid);
        if(!recordJs.isNull())
        {
            for(int recordIndex = 0; recordIndex < recordJs.size(); recordIndex++)
            {
                std::string uuid = recordJs[recordIndex][FLOW_RECORD_TABLE_UUID].asString();
                std::string name = recordJs[recordIndex][FLOW_RECORD_TABLE_FLOW_NAME].asString();

                sysNode->addChild(createNode(sysUuid, uuid, name, Flow_Node));
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

            //属性变化
            emit toShowProperty(mUiUuid, role.mNodeProperty->getJson());

            emit flowChange((*Itor)->parent()->text(Name_Index), role.sysType, (*Itor)->text(Name_Index), mCurSelectUuid, false);

            //通知流程树类型变化
            Json::Value tmJs;
            role.mNodeProperty->getProperty(PROPERTY_FRAME, tmJs);

            emit frameTypeChange(mCurSelectUuid, QString::fromStdString(tmJs.asString()));

            ui->treeWidget->setCurrentItem(*Itor);
            break;
        }
        ++Itor;
    }
}


void recordNavigation::onProjectAlreadySave(QString uuid)
{  
    if(!mIsModify.contains(uuid))
    {
        mIsModify[uuid] = false;
    }

    if(!mIsModify[uuid])
        return ;

    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        recordRole role = (*Itor)->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(uuid.toStdString() == role.uuid)
        {
            QString srcText = (*Itor)->text(Name_Index);

            //去除标志
            int index = srcText.lastIndexOf(mModifyFlag);
            if(index != -1)
            {
                srcText = srcText.mid(0, index);

                (*Itor)->setText(Name_Index, srcText);
            }
            break;
        }
        ++Itor;
    }

    mIsModify[uuid]  = false;
}

void recordNavigation::onProjectModify(QString uuid)
{    
    if(!mIsModify.contains(uuid))
    {
        mIsModify[uuid] = false;
    }

    if(mIsModify[uuid])
        return ;

    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        recordRole role = (*Itor)->data(Name_Index, Qt::UserRole).value<recordRole>();
        if(uuid.toStdString() == role.uuid)
        {
            QString srcText = (*Itor)->text(Name_Index);
            srcText += mModifyFlag;
            (*Itor)->setText(Name_Index, srcText);
            break;
        }
        ++Itor;
    }
    mIsModify[uuid] = true;
}

QTreeWidgetItem *recordNavigation::copyNode(const QTreeWidgetItem *srcNode, const std::string &uuid, const std::string &name)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem();

    recordRole srcRole = srcNode->data(Name_Index, Qt::UserRole).value<recordRole>();

    recordRole newRole;
    newRole.uuid = uuid;
    newRole.nodeType = Flow_Node;
    newRole.mNodeProperty = srcRole.mNodeProperty->clone();

    Json::Value recordJs = flowRecordTable::getInstance()->getValue(uuid);

    newRole.mNodeProperty->setProperty(PROPERTY_RECORD_NAME, recordJs[FLOW_RECORD_TABLE_FLOW_NAME]);
    newRole.mNodeProperty->setProperty(PROPERTY_RECORD_CREATE_TIME, recordJs[FLOW_RECORD_TABLE_CREATE_TIME]);
    newRole.mNodeProperty->setProperty(PROPERTY_RECORD__OPEN_TIME, recordJs[FLOW_RECORD_TABLE_RECENT_OPEN_TIME]);
    newRole.mNodeProperty->setProperty(PROPERTY_RECORD__NODE, recordJs[FLOW_RECORD_TABLE_NOTE]);

    QVariant data;

    newItem->setText(Name_Index, QString::fromStdString(name));

    data.setValue(newRole);

    newItem->setData(Name_Index,Qt::UserRole,data);

    return newItem;
}

QTreeWidgetItem *recordNavigation::createNode(const std::string &fatherUuid, const std::string &uuid, const std::string &name, NodeType type)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    //item->
    item->setText(Name_Index, QString::fromStdString(name));

    //关联数据
    QVariant data;
    recordRole role;
    role.nodeType = type;
    role.uuid = uuid;

    //更新属性
    if(fatherUuid != "")
    {
        Json::Value recordJs = flowRecordTable::getInstance()->getValue(uuid);

        Json::Value devCurInfo;
        Json::Value dstDevCurInfo;

        std::string nodeInfo = recordJs[FLOW_RECORD_TABLE_NODE_INFO].asString();
        if(nodeInfo != "")  //如果记录中有节点属性则按照记录中的，否则新建
        {
            Json::Value value;
            Json::Reader jsonReader;
            if (jsonReader.parse(nodeInfo, value))
            {
                role.mNodeProperty = std::make_shared<nodeProperty>(value);
                role.mNodeProperty->getProperty(PROPERTY_RECORD_LOCAL_DEVICE, devCurInfo);
                role.mNodeProperty->getProperty(PROPERTY_RECORD_DEST_DEVICE, dstDevCurInfo);        
            }
            else
            {
                role.mNodeProperty = std::make_shared<nodeProperty>(templateProperty::getInstance()->getProperty("record"));
            }
        }
        else
        {
            role.mNodeProperty = std::make_shared<nodeProperty>(templateProperty::getInstance()->getProperty("record"));
        }

        //modify xqx 2020-6-22 11:18:53 更新基本属性中的名称是否使能
        nodeProperty tmpProperty(templateProperty::getInstance()->getProperty("record"));
        Json::Value tmpV;
        tmpProperty.getProperty(PROPERTY_RECORD_NAME, tmpV);

        role.mNodeProperty->setReadOnly(PROPERTY_RECORD_NAME, tmpProperty.isReadOnly(PROPERTY_RECORD_NAME));
        //end

        role.mNodeProperty->setProperty(PROPERTY_RECORD_NAME, recordJs[FLOW_RECORD_TABLE_FLOW_NAME]);
        role.mNodeProperty->setProperty(PROPERTY_RECORD_CREATE_TIME, recordJs[FLOW_RECORD_TABLE_CREATE_TIME]);
        role.mNodeProperty->setProperty(PROPERTY_RECORD__OPEN_TIME, recordJs[FLOW_RECORD_TABLE_RECENT_OPEN_TIME]);
        role.mNodeProperty->setProperty(PROPERTY_RECORD__NODE, recordJs[FLOW_RECORD_TABLE_NOTE]);

        //role.mNodeProperty->setProperty(PROPERTY_SRC_SYS_TYPE, );

        if(mDestDevInitValue.contains(fatherUuid))
        {
            role.mNodeProperty->setInitValue(PROPERTY_RECORD_LOCAL_DEVICE, mDestDevInitValue[fatherUuid]);

            if( (devCurInfo.isNull()) && (mDestDevInitValue[fatherUuid].size() > 0))
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_LOCAL_DEVICE, mDestDevInitValue[fatherUuid][0]);
            else
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_LOCAL_DEVICE,devCurInfo);

            role.mNodeProperty->setInitValue(PROPERTY_RECORD_DEST_DEVICE, mDestDevInitValue[fatherUuid]);

            if( (dstDevCurInfo.isNull()) && (mDestDevInitValue[fatherUuid].size() > 0))
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_DEST_DEVICE, mDestDevInitValue[fatherUuid][0]);
            else
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_DEST_DEVICE,dstDevCurInfo);
        }
        else
        {
            //更新目标设备记录
            Json::Value interfaceJs = sysInterfaceTable::getInstance()->getValueBySysUuid(fatherUuid);

            Json::Value arrayJs;
            for(int tmpIndex = 0; tmpIndex < interfaceJs.size(); tmpIndex++)
            {
                Json::Value tmpJs;

                tmpJs[PROPERTY_DEV_VALUE_UUID] = interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_UUID].asString();
                tmpJs[PROPERTY_DEV_VALUE_NAME] = interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_DEV_NAME].asString();

                arrayJs.append(tmpJs);
            }

            mDestDevInitValue[fatherUuid] = arrayJs;

            role.mNodeProperty->setInitValue(PROPERTY_RECORD_LOCAL_DEVICE, mDestDevInitValue[fatherUuid]);

            if( (devCurInfo.isNull()) && (mDestDevInitValue[fatherUuid].size() > 0))
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_LOCAL_DEVICE, mDestDevInitValue[fatherUuid][0]);
            else
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_LOCAL_DEVICE,devCurInfo);

            role.mNodeProperty->setInitValue(PROPERTY_RECORD_DEST_DEVICE, mDestDevInitValue[fatherUuid]);

            if( (dstDevCurInfo.isNull()) && (mDestDevInitValue[fatherUuid].size() > 0))
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_DEST_DEVICE, mDestDevInitValue[fatherUuid][0]);
            else
                role.mNodeProperty->setCurValue(PROPERTY_RECORD_DEST_DEVICE,dstDevCurInfo);

            //更新协议选择
            //获取协议配置
            nodeProperty ptlPro(templateProperty::getInstance()->getProperty("record"));

            Json::Value ptlJs;
            ptlJs = ptlPro.initValue(PROPERTY_FRAME);
            role.mNodeProperty->setInitValue(PROPERTY_FRAME, ptlJs);

            //更新FE数据帧类型

            nodeProperty fePro(templateProperty::getInstance()->getProperty("cmd"));

            Json::Value feJs;
            feJs = fePro.initValue(PROPERTY_FE_DATA_TYPE_SEND_SYS);
            role.mNodeProperty->setInitValue(PROPERTY_FE_DATA_TYPE_SEND_SYS, feJs);
            role.mNodeProperty->setInitValue(PROPERTY_FE_DATA_TYPE_RCV_SYS, feJs);
        }
#if 0
        //modify xqx 20200509 更新中间件确认标志属性
        Json::Value frameJs;
        role.mNodeProperty->getProperty(PROPERTY_FRAME, frameJs);
        if(!frameJs.isNull() && (PROPERTY_FRAME_MIDDLE == frameJs.asString()))
        {
            Json::Value askJs;
            role.mNodeProperty->getProperty(PROPERTY_ASK_FLAG, askJs);
            if(askJs.isNull())
            {
                nodeProperty tmpProperty(templateProperty::getInstance()->getProperty("record"));

                role.mNodeProperty->setInitValue(PROPERTY_ASK_FLAG, tmpProperty.initValue(PROPERTY_ASK_FLAG));
                role.mNodeProperty->setCurValue(PROPERTY_ASK_FLAG, tmpProperty.curValue(PROPERTY_ASK_FLAG));
            }
        }
        //end
#endif
    }
    else
    {
        role.mNodeProperty = std::make_shared<nodeProperty>(templateProperty::getInstance()->getProperty("system"));
    }

    data.setValue(role);

    item->setData(Name_Index,Qt::UserRole,data);

    return item;
}

QTreeWidgetItem *recordNavigation::curItem(QString uuid)
{
    return findItem(uuid);
}

void recordNavigation::onPropertyValueChange(QString uuid, QString attr, Json::Value value)
{
    if(mUiUuid.compare(uuid) != 0)
        return ;

    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(!curItem)
        return ;

    recordRole role = curItem->data(0, Qt::UserRole).value<recordRole>();
    role.mNodeProperty->setProperty(attr.toStdString(), value);

    //modify xqx 2020-6-22 11:32:35 名称可重命名，需更改数据库

    if(attr.compare(PROPERTY_RECORD_NAME) == 0)
    {
        std::string name = value.asString();

        role.mNodeProperty->setProperty(PROPERTY_RECORD_NAME, value);

        QString tmpValue = QString::fromStdString(name);

        QString srcText = curItem->text(0);
        if(-1 != srcText.lastIndexOf(mModifyFlag))
        {
            tmpValue += mModifyFlag;
        }

        curItem->setText(0, tmpValue);

        //写入数据库
        flowRecordTable::getInstance()->updateNodeName(role.uuid, name);
    }

    //end

    if(attr.compare(PROPERTY_FRAME) == 0)
    {        
        std::string type = value.asString();

        if( (type == PROPERTY_FRAME_BE))
        {
            disableAskAttr(role);

            emit removeGroupProperty(PROPERTY_SRC);

            //创建组
            emit addGroupProperty(PROPERTY_SRC);

            //加入属性
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_SYS_TYPE));
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_SYS_CODING));
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_NODE_CODING));


            role.mNodeProperty->setVisible(PROPERTY_SRC_SYS_TYPE, true);
            role.mNodeProperty->setVisible(PROPERTY_SRC_SYS_CODING, true);
            role.mNodeProperty->setVisible(PROPERTY_SRC_NODE_CODING, true);

            role.mNodeProperty->setVisible(PROPERTY_USER, false);
            role.mNodeProperty->setVisible(PROPERTY_SOFT_VERSION, false);

            role.mNodeProperty->setReadOnly(PROPERTY_SRC, false);
            role.mNodeProperty->setVisible(PROPERTY_SRC, true);
        }
        else if( (PROPERTY_FRAME_FE == type ))
        {
            disableAskAttr(role);

            emit removeGroupProperty(PROPERTY_SRC);

            //创建组
            emit addGroupProperty(PROPERTY_SRC);

            //加入属性
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_SYS_TYPE));

            role.mNodeProperty->setVisible(PROPERTY_SRC_SYS_TYPE, true);
            role.mNodeProperty->setVisible(PROPERTY_SRC_SYS_CODING, false);
            role.mNodeProperty->setVisible(PROPERTY_SRC_NODE_CODING, false);

            role.mNodeProperty->setVisible(PROPERTY_USER, false);
            role.mNodeProperty->setVisible(PROPERTY_SOFT_VERSION, false);

            role.mNodeProperty->setReadOnly(PROPERTY_SRC, false);
            role.mNodeProperty->setVisible(PROPERTY_SRC, true);

        }
        else if( (PROPERTY_FRAME_93 == type ) || (PROPERTY_FRAME_1553B == type ))
        {
            disableAskAttr(role);

            //删除属性，并设置隐藏
            emit removeGroupProperty(PROPERTY_SRC);
            role.mNodeProperty->setVisible(PROPERTY_SRC, false);
        }
        else if(type == PROPERTY_FRAME_MIDDLE)
        {            
            enableAskAttr(role);

            emit removeGroupProperty(PROPERTY_SRC);

            //创建组
            emit addGroupProperty(PROPERTY_SRC);

            //加入属性
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_SYS_TYPE));
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_SYS_CODING));
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SRC_NODE_CODING));

            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_USER));
            emit addProperty(PROPERTY_SRC, role.mNodeProperty->curAttr(PROPERTY_SOFT_VERSION));

            role.mNodeProperty->setVisible(PROPERTY_SRC_SYS_TYPE, true);
            role.mNodeProperty->setVisible(PROPERTY_SRC_SYS_CODING, true);
            role.mNodeProperty->setVisible(PROPERTY_SRC_NODE_CODING, true);

            role.mNodeProperty->setVisible(PROPERTY_USER, true);
            role.mNodeProperty->setVisible(PROPERTY_SOFT_VERSION, true);

            role.mNodeProperty->setReadOnly(PROPERTY_SRC, false);
            role.mNodeProperty->setVisible(PROPERTY_SRC, true);
        }

        //通知流程树类型变化
        emit frameTypeChange(mCurSelectUuid, QString::fromStdString(type));

        //属性变化
        emit toShowProperty(mUiUuid, role.mNodeProperty->getJson());
    }

    onProjectModify(role.uuid.c_str());
}

void recordNavigation::enableAskAttr(recordRole role)
{
    emit removeGroupProperty(PROPERTY_FRAME_TYPE_ATTR);

    //创建组
    emit addGroupProperty(PROPERTY_FRAME_TYPE_ATTR);

    role.mNodeProperty->setReadOnly(PROPERTY_FRAME_TYPE_ATTR, false);
    role.mNodeProperty->setVisible(PROPERTY_FRAME_TYPE_ATTR, true);

    //加入属性
    emit addProperty(PROPERTY_FRAME_TYPE_ATTR, role.mNodeProperty->curAttr(PROPERTY_FRAME));
    emit addProperty(PROPERTY_FRAME_TYPE_ATTR, role.mNodeProperty->curAttr(PROPERTY_ASK_FLAG));

    role.mNodeProperty->setVisible(PROPERTY_FRAME, true);
    role.mNodeProperty->setVisible(PROPERTY_ASK_FLAG, true);
}

void recordNavigation::disableAskAttr(recordRole role)
{
    emit removeGroupProperty(PROPERTY_FRAME_TYPE_ATTR);

    //创建组
    emit addGroupProperty(PROPERTY_FRAME_TYPE_ATTR);

    role.mNodeProperty->setReadOnly(PROPERTY_FRAME_TYPE_ATTR, false);
    role.mNodeProperty->setVisible(PROPERTY_FRAME_TYPE_ATTR, true);

    //加入属性
    emit addProperty(PROPERTY_FRAME_TYPE_ATTR, role.mNodeProperty->curAttr(PROPERTY_FRAME));

    role.mNodeProperty->setVisible(PROPERTY_FRAME, true);
    role.mNodeProperty->setVisible(PROPERTY_ASK_FLAG, false);
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
    else if("actionCopy" == actionObjName)
    {
        onActionCopy();
    }
    else if("actionPaste" == actionObjName)
    {
        onActionPaste();
    }
}

void recordNavigation::onSwitchItem(QString uuid)
{
    QTreeWidgetItemIterator Itor(ui->treeWidget);

    while (*Itor)
    {
        recordRole role = (*Itor)->data(Name_Index, Qt::UserRole).value<recordRole>();

        if(Flow_Node == role.nodeType)
        {
            if(uuid.toStdString() == role.uuid)
            {
                mCurSelectUuid = role.uuid.c_str();

                //通知流程树类型变化
                Json::Value tmJs;
                role.mNodeProperty->getProperty(PROPERTY_FRAME, tmJs);

                emit frameTypeChange(mCurSelectUuid, QString::fromStdString(tmJs.asString()));

                //属性变化
                emit toShowProperty(mUiUuid, role.mNodeProperty->getJson());

                ui->treeWidget->setCurrentItem(*Itor);
                break;
            }
        }

        ++Itor;
    }

}

void recordNavigation::onItemClicked(QTreeWidgetItem * item, int column)
{
    if(!item)
        return ;

    recordRole role = item->data(Name_Index, Qt::UserRole).value<recordRole>();
    if(Flow_Node == role.nodeType)
    {
        //流程窗体
        if(mCurSelectUuid.toStdString() != role.uuid)
        {
            mCurSelectUuid = role.uuid.c_str();

            emit flowChange(item->parent()->text(Name_Index), role.sysType, item->text(Name_Index),mCurSelectUuid, mNewUuid.contains(mCurSelectUuid));

            //通知流程树类型变化
            Json::Value tmJs;
            role.mNodeProperty->getProperty(PROPERTY_FRAME, tmJs);

            emit frameTypeChange(mCurSelectUuid, QString::fromStdString(tmJs.asString()));
        }
    }
    else
    {
        //清空流程属性
        emit clearFlowProperty(mUiUuid, Json::Value());
    }

    mCurSelectUuid = role.uuid.c_str();

    //属性变化
    emit toShowProperty(mUiUuid, role.mNodeProperty->getJson());
}

void recordNavigation::onActionCopy()
{
    mIsCopy = false;

    mCopyItem = ui->treeWidget->currentItem();
    if(mCopyItem)
    {
        recordRole role = mCopyItem->data(0, Qt::UserRole).value<recordRole>();

        int type = role.nodeType;
        if( (Flow_Node == type))
        {
            mIsCopy = true;
        }
    }
}

void recordNavigation::onActionPaste()
{
    if(mIsCopy && mCopyItem)
    {
        mIsCopy = false;

        QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
        if(curItem)
        {
            std::string fatherUuid;

            recordRole role = curItem->data(0, Qt::UserRole).value<recordRole>();
            if(Flow_Node == role.nodeType)
            {
                QTreeWidgetItem *fatherItem = curItem->parent();
                if(fatherItem)
                {
                    fatherUuid = fatherItem->data(0, Qt::UserRole).value<recordRole>().uuid;
                }
            }
            else if(System_Node == role.nodeType)
            {
                fatherUuid = role.uuid;
            }

            //增加记录
            std::string uuid;
            std::string name = mCopyItem->text(0).toStdString();
            name += "-副本";

            if(flowRecordTable::getInstance()->addRecord(fatherUuid, name, "", uuid))
            {
                QTreeWidgetItem *item = copyNode(mCopyItem, uuid, name);

                //添加节点
                if(Flow_Node == role.nodeType)
                {
                    QTreeWidgetItem *fatherItem = curItem->parent();
                    if(fatherItem)
                    {
                        fatherItem->insertChild(fatherItem->indexOfChild(curItem) + 1, item);
                    }
                }
                else if(System_Node == role.nodeType)
                {
                    curItem->addChild(item);
                }

                ui->treeWidget->setCurrentItem(item);

                mNewUuid.append(uuid.c_str());

                //拷贝文件
                std::string dstPath = "./flowFile/" + uuid + ".xml";
                std::string srcPath = "./flowFile/" + mCopyItem->data(0, Qt::UserRole).value<recordRole>().uuid + ".xml";

                //创建空文件
                //nullXml::create(dstPath);

                copyXml::copy(QString::fromStdString(srcPath), QString::fromStdString(dstPath));

                //更新主界面
                emit flowChange(item->parent()->text(Name_Index), role.sysType, item->text(Name_Index),uuid.c_str(), false);
            }
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
            QTreeWidgetItem *item = createNode(role.uuid, uuid, dlg.name().toStdString(), Flow_Node);
            //添加节点
            mCurSelectItem->addChild(item);

            ui->treeWidget->setCurrentItem(item);

            mNewUuid.append(uuid.c_str());

            //生成空文件
            std::string path = "./flowFile/" + uuid + ".xml";
            nullXml::create(path);

            //更新主界面
            emit flowChange(item->parent()->text(Name_Index), role.sysType, item->text(Name_Index),uuid.c_str(), true);
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

    if(mNewUuid.contains(role.uuid.c_str()))
        mNewUuid.removeOne(role.uuid.c_str());
}
