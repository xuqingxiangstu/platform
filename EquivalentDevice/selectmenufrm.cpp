#include "selectmenufrm.h"
#include "ui_selectmenufrm.h"
#include "dbtableopt.h"
#include "flow/dataselect.h"
#include <QMessageBox>

Qt::ItemFlags targetFlags;

selectmenufrm::selectmenufrm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectmenufrm)
{
    ui->setupUi(this);
    init();
    m_treeWidget->header()->hide();
    m_treeWidget->hideColumn(1);
}

selectmenufrm::~selectmenufrm()
{
    delete ui;
}

void selectmenufrm::init()
{
    m_toolButton  = new QToolButton(this);
    m_toolButton->setGeometry(0,0,this->width(),this->height());
    m_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    m_menu = new QMenu(this);

    m_treeWidget = new QTreeWidget(m_menu);
    m_widgetaction = new QWidgetAction(m_treeWidget);
    m_treeWidget->setFrameShape(QFrame::NoFrame);

    m_treeWidget->setMinimumWidth(this->width()-8);
    m_treeWidget->setMinimumHeight(this->height());
    m_treeWidget->setGeometry(0,0,this->width(),this->height());


    getTree();

    m_widgetaction->setDefaultWidget(m_treeWidget);
    m_menu->addAction(m_widgetaction);

    m_toolButton->setMenu(m_menu);

    connect(m_treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(SeleItem(QTreeWidgetItem*,int)));

}

void selectmenufrm::SeleItem(QTreeWidgetItem *item,int column)
{
    m_menu->close();
    QString qstr = item->text(0);
    m_toolButton->setText(qstr);

    selectItem();
    item->setSelected(true);
    if(item->checkState(0)==Qt::Checked){
        item->setCheckState(0,Qt::Unchecked);
    }else if(item->checkState(0)==Qt::Unchecked &&item->flags() == targetFlags){
        item->setCheckState(0,Qt::Checked);
    }
    startName = item->text(0);
    startFlag = item->text(1);
}

void selectmenufrm::getTree()
{
    Json::Value systemN;
    if(DBTableOpt::getInstance()->getValueFrameParamSys(systemN)){
        //QMessageBox::about(NULL,"","数据库连接错误！");
    }

    for(int i = 0;i<systemN.size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(systemN[i]["SYSTEM"].asString()));


        QTreeWidgetItem *itemEmpty = new QTreeWidgetItem();
        itemEmpty->setText(0,"无");
        m_treeWidget->addTopLevelItem(itemEmpty);
        m_treeWidget->addTopLevelItem(item);

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
                    itemSubSub->setText(1, QString::fromStdString(groupN[h]["TABLE_NUM"].asString()+"_"+groupN[h]["CODING_NUM"].asString()));
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
                        itemSubSubSub->setText(0, QString::fromStdString(paramN[g]["PARAM_NAME"].asString()));

                    }else{
                        itemSubSubSub->setText(0, QString::fromStdString(paramN[g]["PARAM_NAME"].asString()));
                        itemSubSubSub->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                        itemSubSubSub->setCheckState (0, Qt::Unchecked);//复选框
                        targetFlags = itemSubSubSub->flags();
                        itemSubSubSub->setText(1, QString::fromStdString(paramN[g]["TABLE_NUM"].asString()+"_"+paramN[g]["CODING_NUM"].asString()));
                    }
                    itemSubSub->addChild(itemSubSubSub);
                }
            }
        }
    }
}

void selectmenufrm::selectItem()
{
    auto leftTree = this->m_treeWidget;
    QTreeWidgetItemIterator it(leftTree,QTreeWidgetItemIterator::Checked);

    while (*it) {
       if((*it)->checkState(0)==Qt::Checked){
           (*it)->setSelected(false);
           (*it)->setCheckState(0,Qt::Unchecked);
       }
       ++it;
     }
}

bool selectmenufrm::getCFTreeTableNum(QString &tableNum,QString &name)
{
    bool result =true;
    tableNum = startFlag;
    name = startName;
    return result;
}
