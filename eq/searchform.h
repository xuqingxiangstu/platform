#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "../../src/platform.h"

namespace Ui {
class searchForm;
}

enum searchNode{
    Root_Node,
    Search_Node
};

struct searchRole
{
    searchNode node;
    std::string projectUuid;
    std::string itemUuid;
};

Q_DECLARE_METATYPE(searchRole)

class searchForm : public QWidget
{
    Q_OBJECT

public:
    explicit searchForm(QWidget *parent = 0);
    ~searchForm();
signals:
    void toSearch(Json::Value condition);

    void closeSearch();

    void positionResult(QString projectUuid, QString itemUuid);
public slots:
    void onSearchResult(Json::Value result);

    void onChanageSearch();
private:
    void leftRightBtnCheck();
    QTreeWidgetItem * getPreItem(QTreeWidgetItem *curItem);
    QTreeWidgetItem * getNextItem(QTreeWidgetItem *curItem);

    QTreeWidgetItem *preItem(QTreeWidgetItemIterator &itor);
    QTreeWidgetItem *nextItem(QTreeWidgetItemIterator &itor);

    void deleteItem(QTreeWidgetItem *item);
    void deleteAllItem();
    void updateTree(Json::Value result);
    void updateCount(QTreeWidgetItem *item, QString text);

    QTreeWidgetItem *findItemPassPrjUuid(const std::string &uuid);

    QTreeWidgetItem *newProjectItem(const Json::Value &result);

    QTreeWidgetItem *newItem(const Json::Value &result);
private:
    Ui::searchForm *ui;
};

#endif // SEARCHFORM_H
