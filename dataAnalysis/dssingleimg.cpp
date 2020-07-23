#include "dssingleimg.h"
#include "ui_dssingleimg.h"

dsSingleImg::dsSingleImg(QString proUuid, QString proPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dsSingleImg)
{
    ui->setupUi(this);
}

dsSingleImg::~dsSingleImg()
{
    delete ui;
}
