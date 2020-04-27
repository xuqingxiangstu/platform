#include "templatetree.h"
#include "ui_templatetree.h"

#include <QDebug>
#include "../src/PfCommon/jsoncpp/json.h"
#include "../src/PfSql/paramsTable/paramstable.h"
#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/sysInterfacetable.h"

#include <QMessageBox>
#include "dragTree/dragrole.h"
#include "./property/templateproperty.h"
#include "./property/nodeproperty.h"

templateTree::templateTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::templateTree)
{
    ui->setupUi(this);

    ui->paramTreeWidget->header()->setStretchLastSection(true);
    ui->paramTreeWidget->setDragDropMode(QAbstractItemView::DragOnly);

    initDestDevInitValue();
    initConditionValue();

    connect(ui->serchLineEdit, &QLineEdit::textChanged, [=](const QString &txt){

        if (txt.compare("") == 0)
        {
            showAllItem();
        }
        else
        {
            searchTreeAndHighlight(txt);
        }
    });

    buildTree();
}

templateTree::~templateTree()
{
    delete ui;
}

void templateTree::showAllItem()
{
    QTreeWidget *CurTree = ui->paramTreeWidget;

    //遍历查找
    QTreeWidgetItemIterator Itor(CurTree);

    while (*Itor)
    {
        (*Itor)->setHidden(false);
        (*Itor)->setExpanded(true);
        ++Itor;
    }
}

void templateTree::expandNode(QTreeWidgetItem *item)
{
    if(!item)
        return ;

    if(item->parent())
    {
        item->parent()->setHidden(false);
        item->parent()->setExpanded(true);
        expandNode(item->parent());
    }
}

void templateTree::searchTreeAndHighlight(QString strKey)
{
    //遍历查找
    QTreeWidgetItemIterator Itor(ui->paramTreeWidget);

    while (*Itor)
    {

        std::shared_ptr<dragRole> role = (*Itor)->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();
        if( (dragRole::Node_Cmd == role->getNodeType())
                || (dragRole::Node_Cmd_Group == role->getNodeType())
                || (dragRole::Node_Param_Group == role->getNodeType())
                || (dragRole::Node_Param == role->getNodeType())
                )
        {
            QString TmpText1 = (*Itor)->text(0);
            QString TmpText2 = (*Itor)->text(1);

            if( TmpText1.contains(strKey, Qt::CaseInsensitive) || TmpText2.contains(strKey, Qt::CaseInsensitive))
            //if( TmpText2.contains(strKey, Qt::CaseInsensitive) )
            {
                (*Itor)->setHidden(false);
                //查找父节点并展开
                expandNode(*Itor);
            }
        }
        else
        {
            (*Itor)->setHidden(true);
        }
        ++Itor;
    }
}

void templateTree::updateDestDevInitValue(dragRole *role)
{
    return ;
    if(!mDestDevInitValue.isNull())
    {
        role->getProperty()->setInitValue(PROPERTY_LOCAL_DEVICE, mDestDevInitValue);
        role->getProperty()->setInitValue(PROPERTY_DEST_DEVICE, mDestDevInitValue);
    }

    if(!mDestDevCurValue.isNull())
    {
        if(role->getProperty()->curValue(PROPERTY_LOCAL_DEVICE).isNull())
            role->getProperty()->setCurValue(PROPERTY_LOCAL_DEVICE, mDestDevCurValue);

        if(role->getProperty()->curValue(PROPERTY_DEST_DEVICE).isNull())
            role->getProperty()->setCurValue(PROPERTY_DEST_DEVICE, mDestDevCurValue);
    }
}

QStringList templateTree::getValueMeans(QString value)
{
    QStringList meansList;

    if(value.compare("") != 0)
    {
        meansList = value.split(";");

        //qDebug() << meansList;
    }

    return meansList;
}

void templateTree::updateParamType(dragRole *role)
{
    std::string tableNum = role->getProperty()->tableNum();
    int codingNum = role->getProperty()->codingNum();

    Json::Value infoJs;
    if(paramsTable::getInstance()->getValue(tableNum, codingNum, infoJs))
    {
        std::string dataType = infoJs[PARAM_TABLE_DATA_TYPE].asString();
        QString initValue = QString::fromStdString(infoJs[PARAM_TABLE_INIT_VALUE].asString());
        QString meanValue = QString::fromStdString(infoJs[PARAM_TABLE_VALUE_MEAN].asString());

        QStringList means = getValueMeans(meanValue);

        bool isOk;

        if( (DATA_TYPE_NCHAR == dataType) || (DATA_TYPE_NRAW == dataType))
        {
            role->getProperty()->setType(PROPERTY_FIX_VALUE, PROPERTY_STRING_TYPE);
            role->getProperty()->setReadOnly(PROPERTY_SIM_MODEL, true);

            if(!(means.size() > 0))   //单值
            {
                role->getProperty()->setType(PROPERTY_FIX_VALUE, PROPERTY_STRING_TYPE);
                role->getProperty()->setInitValue(PROPERTY_FIX_VALUE, Json::Value(initValue.toStdString()));
                role->getProperty()->setCurValue(PROPERTY_FIX_VALUE, Json::Value(initValue.toStdString()));
            }
            else    //下拉框选择
            {
                Json::Value arrayJs;
                foreach (QString m, means)
                {
                    arrayJs.append(m.toStdString());
                }
                role->getProperty()->setType(PROPERTY_FIX_VALUE, PROPERTY_ENUM_TYPE);
                role->getProperty()->setInitValue(PROPERTY_FIX_VALUE, arrayJs);
                role->getProperty()->setCurValue(PROPERTY_FIX_VALUE, Json::Value(means.at(0).toStdString()));
            }
        }
        else if( (DATA_TYPE_IEEE32 == dataType) || (DATA_TYPE_IEEE64 == dataType))
        {
            role->getProperty()->setType(PROPERTY_FIX_VALUE, PROPERTY_DOUBLE_TYPE);
            role->getProperty()->setReadOnly(PROPERTY_SIM_MODEL, false);

            double tmpV = initValue.toDouble(&isOk);
            if(!isOk)
                tmpV = 0.0;

            role->getProperty()->setInitValue(PROPERTY_FIX_VALUE, Json::Value(tmpV));
            role->getProperty()->setCurValue(PROPERTY_FIX_VALUE, Json::Value(tmpV));
        }
        else
        {
            role->getProperty()->setReadOnly(PROPERTY_SIM_MODEL, false);

            unsigned int tmpV = initValue.toInt(&isOk, 10);

            if(!isOk)
                tmpV = initValue.toUInt(&isOk, 16);

            if(!isOk)
                tmpV = 0;

            if(!(means.size() > 0))   //单值
            {
                role->getProperty()->setType(PROPERTY_FIX_VALUE, PROPERTY_INT_TYPE);
                role->getProperty()->setInitValue(PROPERTY_FIX_VALUE, Json::Value(tmpV));
                role->getProperty()->setCurValue(PROPERTY_FIX_VALUE, Json::Value(tmpV));
            }
            else    //下拉框选择
            {
                Json::Value arrayJs;
                foreach (QString m, means)
                {
                    arrayJs.append(m.toStdString());
                }
                role->getProperty()->setType(PROPERTY_FIX_VALUE, PROPERTY_ENUM_TYPE);
                role->getProperty()->setInitValue(PROPERTY_FIX_VALUE, arrayJs);
                role->getProperty()->setCurValue(PROPERTY_FIX_VALUE, Json::Value(means.at(0).toStdString()));
            }
        }
    }
}

void templateTree::initConditionValue()
{
    Json::Value cmdJs;

    if(paramsTable::getInstance()->getCmdValues(cmdJs))
    {
        Json::Value noJs;
        noJs[PROPERTY_CONDITION_VALUE_NAME] = PROPERTY_CONDITION_NO;
        noJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = -1;
        noJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = -1;

        mConditionCurValue = noJs;

        mConditionInitValue.append(noJs);

        for(int index = 0; index < cmdJs.size(); index++)
        {
            Json::Value tmpJs;

            tmpJs[PROPERTY_CONDITION_VALUE_NAME] = cmdJs[index][PARAM_TABLE_PARAM_NAME];
            tmpJs[PROPERTY_CONDITION_VALUE_TABLE_NUM] = cmdJs[index][PARAM_TABLE_TABLE_NUM];
            tmpJs[PROPERTY_CONDITION_VALUE_CODING_NUM] = cmdJs[index][PARAM_TABLE_CODING_NUM];

            mConditionInitValue.append(tmpJs);
        }
    }
}

void templateTree::initDestDevInitValue()
{
    //获取系统
    Json::Value sysGroup = systemTable::getInstance()->getSysGroups();

    if(sysGroup.isNull())
        return ;

    for(int index = 0; index < sysGroup.size(); index++)
    {
        std::string uuid = sysGroup[index][SYSTEM_TABLE_UUID].asString();
        std::string name = sysGroup[index][SYSTEM_TABLE_SYSTEM_NAME].asString();

        //获取系统下的接口
        Json::Value interfaceJs = sysInterfaceTable::getInstance()->getValueBySysUuid(uuid);

        for(int tmpIndex = 0; tmpIndex < interfaceJs.size(); tmpIndex++)
        {
            Json::Value tmpJs;

            tmpJs[PROPERTY_DEV_VALUE_UUID] = interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_UUID].asString();
            tmpJs[PROPERTY_DEV_VALUE_NAME] = "[" + name + "] " + interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_DEV_NAME].asString();

            mDestDevInitValue.append(tmpJs);

            if(mDestDevCurValue.isNull())
                mDestDevCurValue = tmpJs;
        }
    } 
}

void templateTree::updateConditionInit(dragRole *role)
{
    if(!mConditionInitValue.isNull())
    {
        role->getProperty()->setInitValue(PROPERTY_START_CONDITION, mConditionInitValue);
        role->getProperty()->setInitValue(PROPERTY_STOP_CONDITION, mConditionInitValue);
    }

    if(!mConditionCurValue.isNull())
    {
        if(role->getProperty()->curValue(PROPERTY_START_CONDITION).isNull())
            role->getProperty()->setCurValue(PROPERTY_START_CONDITION, mConditionCurValue);

        if(role->getProperty()->curValue(PROPERTY_STOP_CONDITION).isNull())
            role->getProperty()->setCurValue(PROPERTY_STOP_CONDITION, mConditionCurValue);
    }
}


void templateTree::buildTree()
{
    Json::Value systemN;
    if(paramsTable::getInstance()->getValueFrameParamSys(systemN)){
        QMessageBox::about(NULL,"","数据库连接错误！");
    }

    for(int i = 0;i<systemN.size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        //item->
        item->setText(Table_Or_Coding_Index, QString::fromStdString(systemN[i][PARAM_TABLE_SYSTEM].asString()));
        //关联数据
        QVariant variant;
        std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();
        dragData->setNodeType(dragRole::Node_Root);
        variant.setValue(dragData);
        item->setData(Table_Or_Coding_Index,Qt::UserRole,variant);

        Json::Value cmdType;
        paramsTable::getInstance()->getValueFrameParamCMD(QString::fromStdString(systemN[i][PARAM_TABLE_SYSTEM].asString()),cmdType);

        for(int j = 0;j<cmdType.size();j++)
        {
            QTreeWidgetItem *itemSub = new QTreeWidgetItem(item);
            itemSub->setText(Table_Or_Coding_Index, QString::fromStdString(cmdType[j][PARAM_TABLE_CMD_TYPE].asString()));

            //关联数据
            QVariant variant;            
            std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();
            dragData->setNodeType(dragRole::Node_Cmd_Or_Param_Group);
            variant.setValue(dragData);
            itemSub->setData(Table_Or_Coding_Index,Qt::UserRole,variant);

            item->addChild(itemSub);

            Json::Value groupN;
            paramsTable::getInstance()->getValueFrameParamGroup(QString::fromStdString(systemN[i][PARAM_TABLE_SYSTEM].asString()), QString::fromStdString(cmdType[j][PARAM_TABLE_CMD_TYPE].asString()),groupN);

            for(int h = 0;h<groupN.size();h++)
            {
                QTreeWidgetItem *itemSubSub = new QTreeWidgetItem(itemSub);
                bool ifHaveCheckbox1 = false;
                if(groupN[h][PARAM_TABLE_CMD_TYPE].asString() == CMD_TYPE_CMD){
                    ifHaveCheckbox1 = true;
                }else{
                    ifHaveCheckbox1 = false;
                }

                //关联数据
                QVariant variant;                
                std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();

                if(ifHaveCheckbox1){
                    dragData->setNodeType(dragRole::Node_Cmd_Group);
                    itemSubSub->setText(Table_Or_Coding_Index, QString::fromStdString(groupN[h][PARAM_TABLE_TABLE_NUM].asString()));
                    itemSubSub->setText(Name_Index, QString::fromStdString(groupN[h][PARAM_TABLE_GROUP_NAME].asString()));
                    //itemSubSub->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                }else{
                    dragData->setNodeType(dragRole::Node_Param_Group);
                    dragData->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_Param_Group])));                    

                    dragData->getProperty()->setProperty(PROPERTY_BASE_INFO_WORD, groupN[h][PARAM_TABLE_INFO_WORD_TYPE]);
                    updateConditionInit(dragData.get());
                    updateDestDevInitValue(dragData.get());

                    itemSubSub->setText(Table_Or_Coding_Index, QString::fromStdString(groupN[h][PARAM_TABLE_TABLE_NUM].asString()));
                    itemSubSub->setText(Name_Index, QString::fromStdString(groupN[h][PARAM_TABLE_GROUP_NAME].asString()));
                    //itemSubSub->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                    //itemSubSub->setCheckState (0, Qt::Unchecked);//复选框


                }
                variant.setValue(dragData);
                itemSubSub->setData(Table_Or_Coding_Index,Qt::UserRole,variant);

                itemSub->addChild(itemSubSub);

                Json::Value paramN;
                paramsTable::getInstance()->getValueFrameParamPar(QString::fromStdString(systemN[i][PARAM_TABLE_SYSTEM].asString()),QString::fromStdString(cmdType[j][PARAM_TABLE_CMD_TYPE].asString()),QString::fromStdString(groupN[h][PARAM_TABLE_GROUP_NAME].asString()),paramN);

                for(int g = 0;g<paramN.size();g++)
                {
                    QTreeWidgetItem *itemSubSubSub = new QTreeWidgetItem(itemSubSub);

                    bool ifHaveCheckbox2 = false;
                    if(paramN[g][PARAM_TABLE_CMD_TYPE].asString() == CMD_TYPE_CMD){
                        ifHaveCheckbox2 = true;
                    }else{
                        ifHaveCheckbox2 = false;
                    }

                    //关联数据
                    QVariant variant;                    
                    std::shared_ptr<dragRole> dragData = std::make_shared<dragRole>();                   

                    if(!ifHaveCheckbox2){
                        dragData->setNodeType(dragRole::Node_Param);
                        dragData->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_Param])));
                        itemSubSubSub->setText(Table_Or_Coding_Index, QString::fromStdString(paramN[g][PARAM_TABLE_CODING_NUM].asString()));
                        itemSubSubSub->setText(Name_Index, QString::fromStdString(paramN[g][PARAM_TABLE_PARAM_NAME].asString()));                        

                        dragData->setTableAndCoding(paramN[g][PARAM_TABLE_TABLE_NUM].asString(), paramN[g][PARAM_TABLE_CODING_NUM].asInt());

                        updateParamType(dragData.get());
                    }else{
                        dragData->setNodeType(dragRole::Node_Cmd);
                        dragData->setProperty(new nodeProperty(templateProperty::getInstance()->getProperty(dragData->mNodeKey[dragRole::Node_Cmd])));

                        dragData->getProperty()->setProperty(PROPERTY_BASE_INFO_WORD, paramN[g][PARAM_TABLE_INFO_WORD_TYPE]);
                        dragData->setTableAndCoding(paramN[g][PARAM_TABLE_TABLE_NUM].asString(), paramN[g][PARAM_TABLE_CODING_NUM].asInt());
                        updateConditionInit(dragData.get());
                        updateDestDevInitValue(dragData.get());

                        itemSubSubSub->setText(Table_Or_Coding_Index, QString::fromStdString(paramN[g][PARAM_TABLE_CODING_NUM].asString()));
                        itemSubSubSub->setText(Name_Index, QString::fromStdString(paramN[g][PARAM_TABLE_PARAM_NAME].asString()));                        

                        ui->paramTreeWidget->expandItem(itemSubSubSub);
                    }                    

                    variant.setValue(dragData);
                    itemSubSubSub->setData(Table_Or_Coding_Index,Qt::UserRole,variant);

                    itemSubSub->addChild(itemSubSubSub);
                }

                ui->paramTreeWidget->collapseItem(itemSubSub);
            }

            ui->paramTreeWidget->expandItem(itemSub);
        }

        ui->paramTreeWidget->expandItem(item);

        ui->paramTreeWidget->addTopLevelItem(item);
        ui->paramTreeWidget->expandAll();
    }

    //重新展开与关闭
    QTreeWidgetItemIterator Itor(ui->paramTreeWidget);
    while (*Itor)
    {
        std::shared_ptr<dragRole> role = (*Itor)->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

        if(dragRole::Node_Param_Group == role->getNodeType())
        {
            ui->paramTreeWidget->collapseItem(*Itor);
        }
        else
        {
            ui->paramTreeWidget->expandItem(*Itor);
        }

        ++Itor;
    }
}
