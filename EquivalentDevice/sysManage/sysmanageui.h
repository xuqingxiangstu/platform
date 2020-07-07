#ifndef SYSMANAGEUI_H
#define SYSMANAGEUI_H

#include <QDialog>
#include <QUuid>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

typedef struct sysInfo
{
    QString uuid;
    QString sysName;
    QString dev_type;
    QString describe;
}my_sys;

extern QString addDevSysName;
namespace Ui {
class sysManageUi;
}

class sysManageUi : public QDialog
{
    Q_OBJECT

public:
    explicit sysManageUi(QWidget *parent = 0);
    ~sysManageUi();
    void systemInit();
    void initSystemTree();
    void update(my_sys *stu);
    void changeUpdate(my_sys *stu, int col);

private slots:

    void deleDev();

    void newBulitD();

    void newDeviceFunc();

    void deleD();

    void on_systemTree_customContextMenuRequested(const QPoint &pos);

    void on_information_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_systemTree_itemClicked(QTreeWidgetItem *item, int column);

    void changeSysName(QTreeWidgetItem*,int);

    void on_information_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::sysManageUi *ui;

    QMenu *sysNameMenu;
    QAction *newBulit;
    QAction *deleteBulit;

    QMenu *deviceTable;
    QAction *newDevice;
    QAction *delDevice;

    QString m_sysNameBox;

    QString m_sysUuid;

};



#endif // SYSMANAGEUI_H
