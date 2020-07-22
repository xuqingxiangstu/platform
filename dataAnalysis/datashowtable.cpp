#include "datashowtable.h"
#include "ui_datashowtable.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QUuid>

dataShowTable::dataShowTable(QString dbName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataShowTable)
{
    ui->setupUi(this);

    for(int index = 0; index < mTableFiledName.size(); index++)
    {
        mSelectField += mTableFiledName.at(index);
        if(index != (mTableFiledName.size() - 1))
            mSelectField += ",";
    }
#if 1
    if(QSqlDatabase::contains(dbName))
    {
        mDb = QSqlDatabase::database(dbName);
    }
#else
    QString uuid = QUuid::createUuid().toString();

    mDb = QSqlDatabase::addDatabase("QSQLITE", uuid);

    mDb.setDatabaseName(dbName);

    if(!mDb.open() )
    {
        throw std::runtime_error(mDb.lastError().text().toStdString());
    }
#endif

    initTable();

    connect(ui->queryBtn, &QPushButton::clicked, [=](){
        QString queryCon = ui->lineEdit->text();
        if(queryCon.compare("") == 0)
        {
            query("select " + mSelectField +  " from " + mTableName);
        }

        conditionQuery(queryCon);
    });

    query("select " + mSelectField +  " from " + mTableName);

    connect(ui->lineEdit, &QLineEdit::returnPressed, [=](){

        QString queryCon = ui->lineEdit->text();
        if(queryCon.compare("") == 0)
        {
            query("select " + mSelectField +  " from " + mTableName);
        }

        conditionQuery(queryCon);
    });
}

dataShowTable::~dataShowTable()
{
    delete ui;
}

void dataShowTable::conditionQuery(QString cond)
{
    //中文名称替换为域名称
    for(QString name : mTableTitle)
    {
        cond.contains(name);
        cond.replace(name, mTableFiledName.at(mTableTitle.indexOf(name)));
    }

    QString sql = "select " + mSelectField;
    sql += " from " + mTableName;
    sql += " where ";
    sql += cond;

    query(sql);
}

void dataShowTable::initTable()
{
    mDataModel = new QSqlQueryModel();

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->setModel(mDataModel);
}

void dataShowTable::query(const QString &sql)
{
    QSqlQuery query(mDb);
    query.prepare(sql);
    if(query.exec())
    {
       mDataModel->setQuery(query);
    }
    else
    {
        qDebug() << mDb.lastError();
    }

    for(int index = 0; index < mTableTitle.size(); index++)
    {
        mDataModel->setHeaderData(index, Qt::Horizontal, mTableTitle.at(index));
    }
}
