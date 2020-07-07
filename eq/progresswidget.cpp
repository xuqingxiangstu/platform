#include "progresswidget.h"
#include "ui_progresswidget.h"
#include <QMovie>

progressWidget::progressWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);    // 设置窗口标志
    setWindowOpacity(0.8);
    mMove = new QMovie(":/icon/loading.gif");
    ui->label->setAttribute(Qt::WA_TranslucentBackground, false);
    ui->label->setMovie(mMove);
    mMove->start();
}

progressWidget::~progressWidget()
{
    delete ui;
}

void progressWidget::onClose()
{
    mMove->stop();
    close();
}
