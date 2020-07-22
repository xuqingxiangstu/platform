#include "dssingleimg.h"
#include "ui_dssingleimg.h"

dsSingleImg::dsSingleImg(QString dbPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dsSingleImg)
{
    ui->setupUi(this);
}

dsSingleImg::~dsSingleImg()
{
    delete ui;
}
