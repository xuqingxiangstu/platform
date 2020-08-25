#include "searchform.h"
#include "ui_searchform.h"

#include <qvalidator.h>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

searchForm::searchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchForm)
{
    ui->setupUi(this);

    ui->treeWidget->header()->hide();
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QRegExp regExp("[a-fA-F0-9]*");

    ui->tableLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->codingLineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(ui->clearBtn, &QToolButton::clicked, this, [=](){

        if(ui->searchParamPage != ui->stackedWidget->currentWidget())
            ui->stackedWidget->setCurrentWidget(ui->searchParamPage);

        ui->leftBtn->setEnabled(false);
        ui->rightBtn->setEnabled(false);
    });

    connect(ui->searchBtn, &QPushButton::clicked, this, [=](){

        QString text = ui->comboBox->currentText();

        Json::Value condition;

        if(text.compare("已打开项目") == 0)
            condition["scope"] = "all";
        else
            condition["scope"] = "current";

        if(ui->tableCodingGroupBox->isChecked())
        {
            QString table = ui->tableLineEdit->text();
            QString coding = ui->codingLineEdit->text();
            if( (table.isEmpty() && coding.isEmpty()) )
            {
                QMessageBox::information(this, "提示", "请输入搜索信息!");
            }
            else
            {
                deleteAllItem();

                condition["type"] = "tableCodingType";

                if(!table.isEmpty())
                    condition["table"] = table.toStdString();

                if(!coding.isEmpty())
                    condition["coding"] = coding.toStdString();

                emit toSearch(condition);
            }
        }

        if(ui->nameGroupBox->isChecked())
        {
            QString name = ui->nameLineEdit->text();
            if(name.isEmpty())
            {
                QMessageBox::information(this, "提示", "请输入搜索信息!");

                return ;
            }

            deleteAllItem();

            condition["type"] = "nameType";
            condition["name"] = name.toStdString();

            condition["wholeWords"] = ui->wholeWordsCheck->isChecked();

            emit toSearch(condition);
        }
    });

    connect(ui->closeBtn, &QPushButton::clicked, this, [=](){
       emit closeSearch();
    });

    connect(ui->treeWidget, &QTreeWidget::doubleClicked, this, [=](const QModelIndex &index){
        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        if(item)
        {
            searchRole data = item->data(0, Qt::UserRole).value<searchRole>();
            if(data.node == Search_Node)
            {
                emit positionResult(QString::fromStdString(data.projectUuid), QString::fromStdString(data.itemUuid));
                leftRightBtnCheck();
            }
        }
    });

    ui->leftBtn->setEnabled(false);
    ui->rightBtn->setEnabled(false);

    connect(ui->leftBtn, &QToolButton::clicked, this, [=](){
        QTreeWidgetItem *item = getPreItem(ui->treeWidget->currentItem());
        if(item)
        {
            searchRole data = item->data(0, Qt::UserRole).value<searchRole>();
            if(data.node == Search_Node)
            {
                emit positionResult(QString::fromStdString(data.projectUuid), QString::fromStdString(data.itemUuid));
                ui->treeWidget->setCurrentItem(item);

                leftRightBtnCheck();
            }
        }
    });

    connect(ui->rightBtn, &QToolButton::clicked, this, [=](){

        QTreeWidgetItem *item = getNextItem(ui->treeWidget->currentItem());
        if(item)
        {
            searchRole data = item->data(0, Qt::UserRole).value<searchRole>();
            if(data.node == Search_Node)
            {
                emit positionResult(QString::fromStdString(data.projectUuid), QString::fromStdString(data.itemUuid));
                ui->treeWidget->setCurrentItem(item);
                leftRightBtnCheck();
            }
        }
    });

    connect(ui->tableCodingGroupBox, &QGroupBox::clicked, this, [=](bool isCheck){
        ui->nameGroupBox->setChecked(!isCheck);
    });

    connect(ui->nameGroupBox, &QGroupBox::clicked, this, [=](bool isCheck){
        ui->tableCodingGroupBox->setChecked(!isCheck);
    });

}

searchForm::~searchForm()
{
    delete ui;
}

QTreeWidgetItem *searchForm::preItem(QTreeWidgetItemIterator &itor)
{
    QTreeWidgetItem *item = nullptr;

    itor--;

    QTreeWidgetItem *tmpItem = *itor;
    if(tmpItem)
    {
        searchRole data = tmpItem->data(0, Qt::UserRole).value<searchRole>();
        if(data.node == Search_Node)
        {
            item = tmpItem;
        }
        else
        {
            item = preItem(itor);
        }
    }

    return item;
}

QTreeWidgetItem *searchForm::nextItem(QTreeWidgetItemIterator &itor)
{
    QTreeWidgetItem *item = nullptr;

    itor++;

    QTreeWidgetItem *tmpItem = *itor;
    if(tmpItem)
    {
        searchRole data = tmpItem->data(0, Qt::UserRole).value<searchRole>();
        if(data.node == Search_Node)
        {
            item = tmpItem;
        }
        else
        {
            item = nextItem(itor);
        }
    }

    return item;
}

QTreeWidgetItem *searchForm::getPreItem(QTreeWidgetItem *curItem)
{
    QTreeWidgetItem *item = nullptr;

    if(curItem)
    {
        QTreeWidgetItemIterator itor(ui->treeWidget);

        while (*itor)
        {
            if(curItem == *itor)
            {
                item = preItem(itor);
                break;
            }
            itor++;
        }
    }

    return item;
}

QTreeWidgetItem *searchForm::getNextItem(QTreeWidgetItem *curItem)
{
    QTreeWidgetItem *item = nullptr;

    if(curItem)
    {
        QTreeWidgetItemIterator itor(ui->treeWidget);

        while (*itor)
        {
            if(curItem == *itor)
            {
                item = nextItem(itor);
                break;
            }
            itor++;
        }
    }

    return item;
}

void searchForm::onChanageSearch()
{
    if(ui->searchParamPage != ui->stackedWidget->currentWidget())
        ui->stackedWidget->setCurrentWidget(ui->searchParamPage);

    if(ui->leftBtn->isEnabled())
        ui->leftBtn->setEnabled(false);

    if(ui->rightBtn->isEnabled())
        ui->rightBtn->setEnabled(false);
}

void searchForm::updateCount(QTreeWidgetItem *item, QString text)
{
    if(!item)
        return ;

    int count = item->childCount();

    text += "(";
    text += QString::number(count, 10);
    text += ")";
    item->setText(0, text);
}

void searchForm::deleteAllItem()
{
    QList<QTreeWidgetItem*> delItems;
    for(int index = 0; index < ui->treeWidget->topLevelItemCount(); index++)
    {
        delItems.append(ui->treeWidget->topLevelItem(index));
    }

    for(auto item : delItems)
    {
        QTreeWidgetItem *del = ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        if(del)
            delete del;
    }
}

void searchForm::deleteItem(QTreeWidgetItem *item)
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

void searchForm::onSearchResult(Json::Value result)
{
    //qDebug() << result.toStyledString().c_str();
    //切换显示
    if(ui->searchResultPage != ui->stackedWidget->currentWidget())
        ui->stackedWidget->setCurrentWidget(ui->searchResultPage);


    //更新数据
    updateTree(result);
}

void searchForm::leftRightBtnCheck()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(getPreItem(item))
        ui->leftBtn->setEnabled(true);
    else
        ui->leftBtn->setEnabled(false);

    if(getNextItem(item))
        ui->rightBtn->setEnabled(true);
    else
        ui->rightBtn->setEnabled(false);
}

void searchForm::updateTree(Json::Value result)
{
    std::string projectUuid = result["project_uuid"].asString();
    QString projectText = result["project_name"].asString().c_str();

    QTreeWidgetItem *fatherItem = findItemPassPrjUuid(projectUuid);
    if(!fatherItem)
    {
        fatherItem = newProjectItem(result);

        ui->treeWidget->addTopLevelItem(fatherItem);
    }
    else
    {

    }

    fatherItem->addChild(newItem(result));

    updateCount(fatherItem, projectText);
}

QTreeWidgetItem *searchForm::newProjectItem(const Json::Value &result)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    QVariant variant;
    searchRole data;

    data.projectUuid = result["project_uuid"].asString();
    data.itemUuid = result["item_uuid"].asString();
    data.node = Root_Node;
    variant.setValue(data);

    item->setData(0, Qt::UserRole, variant);

    item->setText(0, result["project_name"].asString().c_str());

    return item;
}

QTreeWidgetItem *searchForm::newItem(const Json::Value &result)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    QVariant variant;
    searchRole data;

    data.projectUuid = result["project_uuid"].asString();
    data.itemUuid = result["item_uuid"].asString();
    data.node = Search_Node;
    variant.setValue(data);

    item->setData(0, Qt::UserRole, variant);

    item->setText(0, result["item_name"].asString().c_str());

    item->setText(1, std::string(result["table"].asString() + "_" + result["coding"].asString()).c_str());

    return item;
}

QTreeWidgetItem *searchForm::findItemPassPrjUuid(const std::string &uuid)
{
    QTreeWidgetItem *findItem = nullptr;

    for(int index = 0; index < ui->treeWidget->topLevelItemCount(); index++)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(index);
        searchRole role = item->data(0, Qt::UserRole).value<searchRole>();
        if(role.projectUuid == uuid)
        {
            findItem = item;
            break;
        }
    }

    return findItem;
}
