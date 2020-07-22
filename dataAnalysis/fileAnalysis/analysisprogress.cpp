#include "analysisprogress.h"
#include "ui_analysisprogress.h"
#include <QDebug>

analysisProgress::analysisProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::analysisProgress)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);    // 设置窗口标志
    setWindowOpacity(0.8);
    mMove = new QMovie(":/img/icon/loading.gif");
    ui->label->setAttribute(Qt::WA_TranslucentBackground, false);
    ui->label->setMovie(mMove);
    mMove->start();
}

analysisProgress::~analysisProgress()
{
    delete ui;
}

void analysisProgress::onClose()
{
    mMove->stop();
    close();
}

void analysisProgress::setMinMaxV(int min, int max)
{
//    ui->progressBar->setMinimum(min);
//    ui->progressBar->setMaximum(max);
}

void analysisProgress::setCurV(int cur)
{
   // ui->progressBar->setValue(cur);
}
