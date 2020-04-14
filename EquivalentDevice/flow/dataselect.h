#ifndef DATASELECT_H
#define DATASELECT_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "../selectmenufrm.h"

namespace Ui {
class dataSelect;
}

class dataSelect : public QDialog
{
    Q_OBJECT

public:
    explicit dataSelect(QWidget *parent = 0);
    ~dataSelect();

    void initParamTree();
    void initTargetDev();
    void bulidTree();
    void selectItem(QTreeWidget *VIEW);
    void initSelectedDataTree();
    void deleSelectedDataTree();
private slots:
    void on_pushButton_clicked();
    void on_paramTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_2_clicked();
    void on_targetTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
private:
    Ui::dataSelect *ui;
};

#endif // DATASELECT_H
