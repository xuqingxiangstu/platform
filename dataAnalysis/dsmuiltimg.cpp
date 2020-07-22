#include "dsmuiltimg.h"
#include "ui_dsmuiltimg.h"

dsMuiltImg::dsMuiltImg(QString dbPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dsMuiltImg)
{
    ui->setupUi(this);
}

dsMuiltImg::~dsMuiltImg()
{
    delete ui;
}
