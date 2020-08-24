#include "searchform.h"
#include "ui_searchform.h"

#include <qvalidator.h>

#include <QMessageBox>

searchForm::searchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchForm)
{
    ui->setupUi(this);

    ui->treeWidget->header()->hide();

    QValidator *validator = new QIntValidator(0,9999,this);

    ui->tableLineEdit->setValidator(validator);
    ui->codingLineEdit->setValidator(validator);

    connect(ui->clearBtn, &QPushButton::clicked, this, [=](){

        if(ui->searchParamPage != ui->stackedWidget->currentWidget())
            ui->stackedWidget->setCurrentWidget(ui->searchParamPage);
    });

    connect(ui->searchBtn, &QPushButton::clicked, this, [=](){
        QString table = ui->tableLineEdit->text();
        QString coding = ui->codingLineEdit->text();
        if( (table.isEmpty() && coding.isEmpty()) )
        {
            QMessageBox::information(this, "提示", "请输入搜索信息!");
        }
        else
        {

            deleteAllItem();

            Json::Value condition;

            QString text = ui->comboBox->currentText();
            if(text.compare("所有项目") == 0)
                condition["scope"] = "all";
            else
                condition["scope"] = "current";

            if(!table.isEmpty())
                condition["table"] = table.toStdString();

            if(!coding.isEmpty())
                condition["coding"] = coding.toStdString();

            emit toSearch(condition);
        }
    });

    connect(ui->closeBtn, &QPushButton::clicked, this, [=](){
       emit closeSearch();
    });
}

searchForm::~searchForm()
{
    delete ui;
}

void searchForm::onChanageSearch()
{
    if(ui->searchParamPage != ui->stackedWidget->currentWidget())
        ui->stackedWidget->setCurrentWidget(ui->searchParamPage);
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

    variant.setValue(data);

    item->setData(0, Qt::UserRole, variant);

    item->setText(0, result["item_name"].asString().c_str());

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
