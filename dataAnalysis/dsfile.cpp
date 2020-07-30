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

    plainTextEdit = new TextEdit();
    plainTextEdit->setReadOnly(true);
    ui->verticalLayout->addWidget(plainTextEdit);
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
    plainTextEdit->insertPlainText(context);
    plainTextEdit->moveCursor(QTextCursor::End);
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

    plainTextEdit->moveCursor(QTextCursor::Start);
}
