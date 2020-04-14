#include "dataselect.h"
#include "ui_dataselect.h"
#include "../dbtableopt.h"
#include <QMessageBox>
#include <QDebug>
#include <algorithm>
#include "../selectmenufrm.h"

using namespace std;

Qt::ItemFlags flags;

dataSelect::dataSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataSelect)
{
    ui->setupUi(this);
    initParamTree();
    initTargetDev();
    ui->paramTreeWidget->hideColumn(1);
    ui->targetTreeWidget->hideColumn(3);
    ui->selectedDataTree->hideColumn(8);
    ui->selectedDataTree->hideColumn(9);
    ui->selectedDataTree->hideColumn(10);
    ui->selectedDataTree->hideColumn(11);
    initSelectedDataTree();
}

dataSelect::~dataSelect()
{
    delete ui;
}

void dataSelect::initParamTree()
{
    bulidTree();
}
void dataSelect::initSelectedDataTree()
{
    ui->selectedDataTree->setDragEnabled(true);
    //设置拖放
    ui->selectedDataTree->setAcceptDrops(true);
    //设置显示将要被放置的位置
    ui->selectedDataTree->setDropIndicatorShown(true);
    //设置拖放模式为移动项目，否则为复制项目
    ui->selectedDataTree->setDragDropMode(QAbstractItemView::InternalMove);
    //设置item进入编辑的方式
    ui->selectedDataTree->setEditTriggers(QAbstractItemView::SelectedClicked);

}
void dataSelect::initTargetDev()
{
    Json::Value systemN;
    DBTableOpt::getInstance()->getValueAllSubSystemInfo(systemN);

    for(int i = 0;i<systemN.size();i++){
        QTreeWidgetItem *itemSys = new QTreeWidgetItem();
        itemSys->setText(0, QString::fromStdString(systemN[i]["SYSTEM_NAME"].asString()));

        ui->targetTreeWidget->addTopLevelItem(itemSys);

        Json::Value devName;
        DBTableOpt::getInstance()->getSystemDev(QString::fromStdString(systemN[i]["SYSTEM_NAME"].asString()),devName);

        for(int j = 0;j<devName.size();j++)
        {
            QTreeWidgetItem *itemDev = new QTreeWidgetItem();
            itemDev->setText(0, QString::fromStdString(devName[j]["DEV_NAME"].asString()));
            itemDev->setText(1, QString::fromStdString(devName[j]["DEV_TYPE"].asString()));
            itemDev->setText(2, QString::fromStdString(devName[j]["DESCRIBE"].asString()));
            itemDev->setText(3, QString::fromStdString(devName[j]["UUID"].asString()));
            itemDev->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            itemDev->setCheckState(0, Qt::Unchecked);
            itemSys->addChild(itemDev);
        }
    }

}
void dataSelect::bulidTree()
{
    Json::Value systemN;
    if(DBTableOpt::getInstance()->getValueFrameParamSys(systemN)){
        //QMessageBox::about(NULL,"","数据库连接错误！");
    }

    for(int i = 0;i<systemN.size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(systemN[i]["SYSTEM"].asString()));
        ui->paramTreeWidget->addTopLevelItem(item);
        Json::Value cmdType;
        DBTableOpt::getInstance()->getValueFrameParamCMD(QString::fromStdString(systemN[i]["SYSTEM"].asString()),cmdType);

        for(int j = 0;j<cmdType.size();j++)
        {
            QTreeWidgetItem *itemSub = new QTreeWidgetItem(item);
            itemSub->setText(0, QString::fromStdString(cmdType[j]["CMD_TYPE"].asString()));
            item->addChild(itemSub);

            Json::Value groupN;
            DBTableOpt::getInstance()->getValueFrameParamGroup(QString::fromStdString(cmdType[j]["CMD_TYPE"].asString()),groupN);

            for(int h = 0;h<groupN.size();h++)
            {
                QTreeWidgetItem *itemSubSub = new QTreeWidgetItem(itemSub);
                bool ifHaveCheckbox1 = false;
                if(groupN[h]["CMD_TYPE"].asString() == "指令"){
                    ifHaveCheckbox1 = true;
                }else{
                    ifHaveCheckbox1 = false;
                }
                if(ifHaveCheckbox1){
                    itemSubSub->setText(0, QString::fromStdString(groupN[h]["TABLE_NUM"].asString()+"_"+groupN[h]["GROUP_NAME"].asString()));
                }else{
                    itemSubSub->setText(0, QString::fromStdString(groupN[h]["TABLE_NUM"].asString()+"_"+groupN[h]["GROUP_NAME"].asString()));
                    itemSubSub->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                    itemSubSub->setCheckState (0, Qt::Unchecked);//复选框
                }
                itemSub->addChild(itemSubSub);

                Json::Value paramN;
                DBTableOpt::getInstance()->getValueFrameParamPar(QString::fromStdString(groupN[h]["GROUP_NAME"].asString()),paramN);

                for(int g = 0;g<paramN.size();g++)
                {
                    QTreeWidgetItem *itemSubSubSub = new QTreeWidgetItem(itemSubSub);

                    bool ifHaveCheckbox2 = false;
                    if(paramN[g]["CMD_TYPE"].asString() == "指令"){
                        ifHaveCheckbox2 = true;
                    }else{
                        ifHaveCheckbox2 = false;
                    }
                    if(!ifHaveCheckbox2){
                        itemSubSubSub->setText(0, QString::fromStdString(paramN[g]["CODING_NUM"].asString()+"_"+paramN[g]["PARAM_NAME"].asString()));
                        itemSubSubSub->setText(1, QString::fromStdString(paramN[g]["INIT_VALUE"].asString()));
                    }else{
                        itemSubSubSub->setText(0, QString::fromStdString(paramN[g]["CODING_NUM"].asString()+"_"+paramN[g]["PARAM_NAME"].asString()));
                        itemSubSubSub->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                        itemSubSubSub->setCheckState (0, Qt::Unchecked);//复选框
                        itemSubSubSub->setText(1, QString::fromStdString(""));
                        flags = itemSubSubSub->flags();
                    }
                    itemSubSub->addChild(itemSubSubSub);
                }
            }
        }
    }
}

void dataSelect::on_pushButton_clicked()
{
    auto leftTree = this->ui->paramTreeWidget;
    QTreeWidgetItemIterator it(leftTree,QTreeWidgetItemIterator::Checked);

    QTreeWidgetItem *item = new QTreeWidgetItem;
    QString startTableNum;
    QString endTableNum;
    QString startName;
    QString endName;
    ui->widget->getCFTreeTableNum(startTableNum,startName);
    ui->widget_2->getCFTreeTableNum(endTableNum,endName);
    if (*it) {
        item->setText(0,(*it)->text(0));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState (0, Qt::Unchecked);//复选框

        item->setText(2,"");
        item->setText(3,startName);
        item->setText(4,endName);
        item->setText(5,ui->delayBox->text());
        item->setText(6,ui->sendType->currentText());
        item->setText(7,ui->targetTreeWidget->currentItem()->parent()->text(0)+"/"+ui->targetTreeWidget->currentItem()->text(0));

        if((*it)->childCount()>0){
            for(int i = 0;i<(*it)->childCount();i++){
                QTreeWidgetItem *itemSub = new QTreeWidgetItem;
                itemSub->setText(0,(*it)->child(i)->text(0));
                itemSub->setText(1,(*it)->child(i)->text(1));
                item->addChild(itemSub);
            }
        }else{
            item->setText(1,(*it)->text(1));
        }
        if(startName == "无"||startName == ""){
            item->setText(10,"");
        }else{
            item->setText(10,startTableNum);
        }
        if(endName == "无"||endName == ""){
            item->setText(11,"");
        }else{
            item->setText(11,endTableNum);
        }

        ui->selectedDataTree->addTopLevelItem(item);
    };
}
void dataSelect::on_paramTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidget *view = ui->paramTreeWidget;
    selectItem(view);
    item->setSelected(true);
    if(item->checkState(0)==Qt::Checked){
        item->setCheckState(0,Qt::Unchecked);
    }else if(item->checkState(0)==Qt::Unchecked &&item->flags() == flags){
        item->setCheckState(0,Qt::Checked);
    }
}
void dataSelect::selectItem(QTreeWidget *VIEW)
{
    QTreeWidgetItemIterator it(VIEW,QTreeWidgetItemIterator::Checked);

    while (*it) {
       if((*it)->checkState(0)==Qt::Checked){
           (*it)->setSelected(false);
           (*it)->setCheckState(0,Qt::Unchecked);
       }
       ++it;
     }
}
void dataSelect::deleSelectedDataTree(){
    std::vector<int> idArray;
    QTreeWidgetItemIterator it(ui->selectedDataTree,QTreeWidgetItemIterator::Checked);
    while (*it) {
        ui->selectedDataTree->setCurrentItem(*it);
        //ui->selectedDataTree->takeTopLevelItem(ui->selectedDataTree->currentIndex().row());
        QTreeWidgetItem *curritem = ui->selectedDataTree->currentItem();
        int index = ui->selectedDataTree->indexOfTopLevelItem(curritem);
        idArray.push_back(index);
        ++it;
     }
    sort(idArray.begin(), idArray.end(), less<int>());
    for(int i=idArray.size()-1;i>=0;i--){
       delete ui->selectedDataTree->takeTopLevelItem(idArray[i]);
    }
}


void dataSelect::on_pushButton_2_clicked()
{
    deleSelectedDataTree();
}

void dataSelect::on_targetTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidget *view = ui->targetTreeWidget;
    selectItem(view);

    item->setSelected(true);
    if(item->checkState(0)==Qt::Checked){
        item->setCheckState(0,Qt::Unchecked);
    }else if(item->checkState(0)==Qt::Unchecked &&item->flags() == flags){
        item->setCheckState(0,Qt::Checked);
    }
}

