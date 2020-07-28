#include "dsfile.h"
#include "ui_dsfile.h"
#include <QEventLoop>
#include "readfilethread.h"
#include "./fileAnalysis/analysisprogress.h"

dsFile::dsFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dsFile)
{
    ui->setupUi(this);

    mDataModel = new QStandardItemModel(this);
    //ui->listView->setModel(mDataModel);
}

dsFile::~dsFile()
{
    delete ui;
}

void dsFile::appendContext(QByteArray context)
{
//    ui->textEdit->insertPlainText(context);
//    ui->textEdit->moveCursor(QTextCursor::End);
    //ui->listWidget->sett
//    QString text = context;
//    QStandardItem *tmp = new QStandardItem();
//    tmp->setText(text);
//    mDataModel->appendRow(tmp);
    ui->plainTextEdit->insertPlainText(context);
    ui->plainTextEdit->moveCursor(QTextCursor::End);
}

void dsFile::updateContext(const QString &filePath)
{
    readFileThread *thread  = new readFileThread(filePath);
    analysisProgress progress;

    connect(thread, &readFileThread::updateContext, this, &dsFile::appendContext);

    connect(thread, &readFileThread::finished, &progress, &analysisProgress::onClose);

    thread->startTask();

    progress.exec();

    delete thread;
    thread = nullptr;

    ui->plainTextEdit->moveCursor(QTextCursor::Start);
}
