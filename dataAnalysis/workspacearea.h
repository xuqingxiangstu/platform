#ifndef WORKSPACEAREA_H
#define WORKSPACEAREA_H

#include <QWidget>

#include "datashowtable.h"

namespace Ui {
class workspaceArea;
}

class workspaceArea : public QWidget
{
    Q_OBJECT

public:
    explicit workspaceArea(QWidget *parent = 0);
    ~workspaceArea();
private:
    dataShowTable *mDataShowTable;
private:
    Ui::workspaceArea *ui;
};

#endif // WORKSPACEAREA_H
