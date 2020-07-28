#include "dssingleimg.h"
#include "ui_dssingleimg.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QUuid>

#include <QThread>
#include <QEventLoop>

#include "dssingleupdatethread.h"
#include "./fileAnalysis/analysisprogress.h"

dsSingleImg::dsSingleImg(QString proUuid, QString proPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dsSingleImg),
    mProUuid(proUuid),
    mProPath(proPath)
{
    ui->setupUi(this);

    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    mCurveWidget = new curveWiget(this);

    updateTree();

    connect(this->ui->lineEdit_tree, &QLineEdit::textChanged,[=](QString text)
    {
            if(text.compare("") == 0)
            {
                //遍历所有设置可见并展开
                QTreeWidgetItemIterator Itor(this->ui->treeWidget);
                while(*Itor)
                {
                    (*Itor)->setExpanded(true);
                    (*Itor)->setHidden(false);
                    ++Itor;
                }
            }
            else
            {
                searchChild(text);
            }
    });

    connect(this, &dsSingleImg::addCurve, mCurveWidget, &curveWiget::onAddCurve);
    connect(mCurveWidget, &curveWiget::deleteCurve, this, &dsSingleImg::onDeleteCurve);

    ui->verticalLayout_chart->addWidget(mCurveWidget);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,[=](const QPoint &point){

        QTreeWidgetItem *currentItem = ui->treeWidget->itemAt(point);
        mCurSelectItem = currentItem;
        if(currentItem)
        {
            nodeRole role = currentItem->data(Table_Index, Qt::UserRole).value<nodeRole>();

            if(Table_Node == role.nodeType)
            {
                ui->actionAdd->setEnabled(false);
            }
            else if(Coding_Node == role.nodeType)
            {
                ui->actionAdd->setEnabled(true);
            }

            mPopMenu->exec(QCursor::pos());
        }

    });

    mPopMenu = new QMenu();
    mPopMenu->addAction(ui->actionAdd);

    connect(ui->actionAdd, &QAction::triggered, [=](){
        addCurveShow();
    });
}

dsSingleImg::~dsSingleImg()
{
    delete ui;
}

void dsSingleImg::addCurveShow()
{
    QTreeWidgetItem *curItem = ui->treeWidget->currentItem();
    if(!curItem)
    {
        return ;
    }

    //通过uuid找到工程item
    nodeRole role = curItem->data(Table_Index, Qt::UserRole).value<nodeRole>();
    if(Coding_Node == role.nodeType)
    {
        if(mHasCurves.size() > CURVE_MAX_SIZE)
        {
            QString info;
            info += "最多添加";
            info += QString::number(CURVE_MAX_SIZE);
            info += "曲线!";
            QMessageBox::information(this, "提示", info);
            return ;
        }
        else
        {
            if(mHasCurves.contains(role.nodeUuid))
            {
                QString info;
                info += "画布已有此参数，请重新选择";

                QMessageBox::information(this, "提示", info);
                return ;
            }

            QString name = "";

            if(curItem->parent())
            {
                name += curItem->parent()->text(Table_Index);
                name += "_";
            }

            name += curItem->text(Table_Index);
            name += "_";
            name += curItem->text(Name_Index);

            emit addCurve(role.nodeUuid, name);

            mHasCurves.append(role.nodeUuid);
        }
    }
}

void dsSingleImg::onDeleteCurve(QString uuid)
{
    if(mHasCurves.contains(uuid))
    {
        mHasCurves.removeOne(uuid);
    }
}

void dsSingleImg::searchChild(QString strKey)
{
    //遍历寻找
    QTreeWidgetItemIterator Itor(this->ui->treeWidget);
    while(*Itor)
    {
        //只查第二级，第一级不查询
        if((*Itor)->childCount() == 0)
        {
            QString TmpText = (*Itor)->text(1);
            if(TmpText.contains(strKey, Qt::CaseInsensitive))
            {
                (*Itor)->setHidden(false);
                //查找父节点并展开
                parentExpand(*Itor);
            }
        }
        else
        {
            (*Itor)->setHidden(true);
        }
        ++Itor;
    }
}

void dsSingleImg::parentExpand(QTreeWidgetItem *item)
{
    if(!item)
        return;
    if(item->parent())
    {
        item->parent()->setHidden(false);
        item->parent()->setExpanded(true);
        parentExpand(item->parent());
    }
}

void dsSingleImg::updateTree()
{
    QString dbPath = mProPath + "/result/" + mProUuid + ".db";

    mCurveWidget->setDbInfo(dbPath, mProUuid);

    analysisProgress progress;
    dssingleUpdateThread *thread  = new dssingleUpdateThread(dbPath, mProUuid);
    connect(thread, &dssingleUpdateThread::updateTableNode, this, &dsSingleImg::createTableNode);
    connect(thread, &dssingleUpdateThread::updateCodingNode, this, &dsSingleImg::createCodingNode);

    connect(thread, &dssingleUpdateThread::finished, &progress, &analysisProgress::onClose);

    progress.show();

    thread->startTask();

    progress.exec();

    delete thread;
    thread = nullptr;
}
void dsSingleImg::createTableNode(QString uuid, QString table)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    nodeRole role;
    role.nodeType = Table_Node;
    role.nodeUuid = uuid;

    data.setValue(role);

    item->setData(Table_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Table_Index, table);

    ui->treeWidget->addTopLevelItem(item);
}

void dsSingleImg::createCodingNode(QString fatherUuid, QString coding, QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //关联数据
    QVariant data;
    nodeRole role;
    role.nodeType = Coding_Node;
    role.nodeUuid = QUuid::createUuid().toString();

    data.setValue(role);

    item->setData(Table_Index,Qt::UserRole,data);
    item->setExpanded(true);
    item->setText(Table_Index, coding);
    item->setText(Name_Index, name);

    auto fatherItem = findItemFromUuid(fatherUuid);

    if(fatherItem)
    {
        fatherItem->addChild(item);
    }
}

QTreeWidgetItem *dsSingleImg::findItemFromUuid(const QString &uuid)
{
    QTreeWidgetItem *item = nullptr;

    QTreeWidgetItemIterator Itor(ui->treeWidget);
    while (*Itor)
    {
        nodeRole role = (*Itor)->data(0, Qt::UserRole).value<nodeRole>();

        if(role.nodeUuid.compare(uuid) == 0)
        {
            item = *Itor;
            break;
        }

        ++Itor;
    }

    return item;
}
