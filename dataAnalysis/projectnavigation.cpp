#include "projectnavigation.h"
#include "ui_projectnavigation.h"
#include "./property/templateproperty.h"

#include <QUuid>

projectNavigation::projectNavigation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::projectNavigation)
{
    ui->setupUi(this);

    mUiUuid = QUuid::createUuid().toString();


}

projectNavigation::~projectNavigation()
{
    delete ui;
}
