#include "datashowtable.h"
#include "ui_datashowtable.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QUuid>

dataShowTable::dataShowTable(QString proUuid, QString proPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataShowTable),
    mCurCondition("")
{
    ui->setupUi(this);

    for(int index = 0; index < mTableFiledName.size(); index++)
    {
        mSelectField += mTableFiledName.at(index);
        if(index != (mTableFiledName.size() - 1))
            mSelectField += ",";
    }

    if(QSqlDatabase::contains(proUuid))
    {
        mDb = QSqlDatabase::database(proUuid);
    }
    else
    {
        mDb = QSqlDatabase::addDatabase("QSQLITE", proUuid);

        QString dbPath = proPath + "/result/" + proUuid + ".db";

        mDb.setDatabaseName(dbPath);

        if(!mDb.open() )
        {
            throw std::runtime_error(mDb.lastError().text().toStdString());
        }
    }

    initTable();

    connect(ui->queryBtn, &QPushButton::clicked, [=](){
        QString queryCon = ui->lineEdit->text();

        for(QString name : mTableTitle)
        {
            queryCon.contains(name);
            queryCon.replace(name, mTableFiledName.at(mTableTitle.indexOf(name)));
        }

        mCurCondition = queryCon;

        updateTable(queryCon);
    });

    connect(ui->refreshBtn, &QPushButton::clicked, [=](){
        updateTable("");
    });

    connect(ui->lineEdit, &QLineEdit::returnPressed, [=](){

        QString queryCon = ui->lineEdit->text();

        for(QString name : mTableTitle)
        {
            queryCon.contains(name);
            queryCon.replace(name, mTableFiledName.at(mTableTitle.indexOf(name)));
        }

        mCurCondition = queryCon;

        updateTable(queryCon);
    });

    updateTable("");

    connect(ui->firstPageBtn, &QPushButton::clicked, this, [=](){
        mCurPageIndex = 1;

        query(pageSql(mCurPageIndex, mCurCondition));

        updateButtonAndInfo();
    });

    connect(ui->upPageBtn, &QPushButton::clicked, this, [=](){

        mCurPageIndex--;

        query(pageSql(mCurPageIndex, mCurCondition));

        updateButtonAndInfo();
    });

    connect(ui->downPageBtn, &QPushButton::clicked, this, [=](){

        mCurPageIndex++;

        query(pageSql(mCurPageIndex, mCurCondition));

        updateButtonAndInfo();
    });

    connect(ui->lastPageBtn, &QPushButton::clicked, this, [=](){

        mCurPageIndex = mMaxPageIndex;        

        query(pageSql(mCurPageIndex, mCurCondition));

        updateButtonAndInfo();
    });

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinValueChange(int)));
}

dataShowTable::~dataShowTable()
{
    delete ui;
}

void dataShowTable::updateTable(QString cond)
{
    mMaxRecordSize = getRecordSize(cond);

    if(mMaxRecordSize % MAX_RECORD_SIZE)
        mMaxPageIndex = mMaxRecordSize / MAX_RECORD_SIZE + 1;
    else
        mMaxPageIndex = mMaxRecordSize / MAX_RECORD_SIZE;

    mCurPageIndex = 1;

    query(pageSql(mCurPageIndex, cond));

    updateButtonAndInfo();

    QString maxPageText = "共 ";
    maxPageText += QString::number(mMaxPageIndex, 10);
    maxPageText += " 页";

    ui->maxPageLabel->setText(maxPageText);

    ui->spinBox->setMinimum(1);
    ui->spinBox->setMaximum(mMaxPageIndex);
}

void dataShowTable::onSpinValueChange(int value)
{
    if(mCurPageIndex == value)
        return ;

    mCurPageIndex = value;

    query(pageSql(mCurPageIndex));

    updateButtonAndInfo();
}

void dataShowTable::updateButtonAndInfo()
{
    if(mMaxPageIndex == mCurPageIndex)
    {
        if(mCurPageIndex == 1)
        {
            ui->firstPageBtn->setEnabled(false);
            ui->upPageBtn->setEnabled(false);
            ui->downPageBtn->setEnabled(false);
            ui->lastPageBtn->setEnabled(false);
        }
        else
        {
            ui->firstPageBtn->setEnabled(true);
            ui->upPageBtn->setEnabled(true);
            ui->downPageBtn->setEnabled(false);
            ui->lastPageBtn->setEnabled(false);
        }
    }
    else if(mMaxPageIndex != mCurPageIndex)
    {
        if(mCurPageIndex == 1)
        {
            ui->firstPageBtn->setEnabled(false);
            ui->upPageBtn->setEnabled(false);
            ui->downPageBtn->setEnabled(true);
            ui->lastPageBtn->setEnabled(true);
        }
        else
        {
            ui->firstPageBtn->setEnabled(true);
            ui->upPageBtn->setEnabled(true);
            ui->downPageBtn->setEnabled(true);
            ui->lastPageBtn->setEnabled(true);
        }
    }

    ui->spinBox->setValue(mCurPageIndex);

    QString curPageText = "共 ";

    if(mCurPageIndex != mMaxPageIndex)
        curPageText += QString::number(MAX_RECORD_SIZE, 10);
    else
        curPageText += QString::number(mMaxRecordSize % MAX_RECORD_SIZE, 10);

    curPageText += " 条第";
    curPageText += QString::number(mCurPageIndex, 10);
    curPageText += "页";

    ui->curPageLabel->setText(curPageText);
}

QString dataShowTable::pageSql(int pageIndex, QString cond)
{
    QString sql = "";

    sql = "select " + mSelectField +  " from " + mTableName;

    if(cond.compare("") != 0)
    {
        sql += " where ";
        sql += cond;
    }

    sql += " limit ";
    sql += QString::number(MAX_RECORD_SIZE, 10);
    sql += " offset ";
    sql += QString::number((pageIndex - 1) * MAX_RECORD_SIZE);

    return sql;
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

int dataShowTable::getRecordSize(QString cond)
{
    int size = -1;    

    QString sql = "select count (*) as num from " + mTableName;

    if(cond.compare("") != 0)
    {
        sql += " where ";
        sql += cond;
    }

    QSqlQuery query(sql, mDb);
    QSqlRecord rec = query.record();

    while(query.next())
    {
        rec = query.record();
        for(int index =0; index<rec.count();index++)
        {
            size = rec.value(index).toInt();
        }
    }

    return size;
}
