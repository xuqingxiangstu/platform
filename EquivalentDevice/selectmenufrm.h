#ifndef SELECTMENUFRM_H
#define SELECTMENUFRM_H

#include <QWidget>
#include <QTreeWidget>
#include <QWidgetAction>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QToolButton>
#include <QListWidgetItem>


namespace Ui {
class selectmenufrm;
}

class selectmenufrm : public QWidget
{
    Q_OBJECT

public:
    explicit selectmenufrm(QWidget *parent = 0);
    ~selectmenufrm();
    bool getCFTreeTableNum(QString &tableNum, QString &name);
    QString startFlag;
    QString startName;
public slots:
    void SeleItem(QTreeWidgetItem *item,int column);
private:
    void init();
    void getTree();
    void selectItem();
private:
    Ui::selectmenufrm *ui;
    QToolButton *m_toolButton;
    QTreeWidget *m_treeWidget;
    QWidgetAction *m_widgetaction;
    QMenu *m_menu;
};

#endif // SELECTMENUFRM_H
