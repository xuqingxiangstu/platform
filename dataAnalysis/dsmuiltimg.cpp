#include "dsmuiltimg.h"
#include "ui_dsmuiltimg.h"

dsMuiltImg::dsMuiltImg(QString proUuid, QString proPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dsMuiltImg)
{
    ui->setupUi(this);
}

dsMuiltImg::~dsMuiltImg()
{
    delete ui;
}
