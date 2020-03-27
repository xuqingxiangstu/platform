#ifndef TEMPLATETREE_H
#define TEMPLATETREE_H

#include <QWidget>

#include "../dragTree/dragrole.h"

#include <QTreeWidget>
namespace Ui {
class templateTree;
}

class templateTree : public QWidget
{
    Q_OBJECT
private:
    enum TreeItemIndex
    {
        Table_Or_Coding_Index,
        Name_Index
    };

public:
    explicit templateTree(QWidget *parent = 0);
    ~templateTree();
private:
    void showAllItem();
    void searchTreeAndHighlight(QString strKey);
    void expandNode(QTreeWidgetItem *);
private:
    void buildTree();

    void updateConditionInit(dragRole *role);
    void updateDestDevInitValue(dragRole *role);

    /**
     * @brief updateParamType   更新参数属性类型，根据数据类型进行重新组合
     * @param role
     */
    void updateParamType(dragRole *role);

    QStringList getValueMeans(QString value);

    void initConditionValue();
    void initDestDevInitValue();
private:
    Ui::templateTree *ui;
    Json::Value mConditionInitValue;
    Json::Value mDestDevInitValue;

    Json::Value mConditionCurValue;
    Json::Value mDestDevCurValue;
};

#endif // TEMPLATETREE_H
