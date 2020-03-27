#ifndef RECORDNAVIGATION_H
#define RECORDNAVIGATION_H

#include <QWidget>

#include <QMenu>
#include <QTreeWidget>

namespace Ui {
class recordNavigation;
}

struct recordRole
{
    int nodeType;       //节点类型
    std::string uuid;   //uuid
};

Q_DECLARE_METATYPE(recordRole)

class recordNavigation : public QWidget
{
    Q_OBJECT
    enum TreeItemIndex{
        Name_Index
    };

    enum NodeType{
        System_Node,
        Flow_Node
    };
public:
    explicit recordNavigation(QWidget *parent = 0);
    ~recordNavigation();
public slots:
    void onMenuTrigger(QAction * action);
    void onItemClicked(QTreeWidgetItem * item, int column);
signals:
    /**
     * @brief flowChange    点击流程变化槽
     * @param sysName   系统名称
     * @param testName  测试项目
     * @param uuid      记录UUID
     */
    void flowChange(QString sysName, QString testName, QString uuid);

    void deleteFlow(QString uuid);
public:
    void buildTree();
private:    
    QTreeWidgetItem *createNode(const std::string &uuid, const std::string &name, NodeType type);
    void deleteItem(QTreeWidgetItem *item);
private:
    void waring(QString text);
private:
    void onActionNew();
    void onActionDelete();
private:
    QMenu *mPopMenu;
    QTreeWidgetItem *mCurSelectItem;    //当前选择Item
    QString mCurSelectUuid;             //当前选择的uuid
private:
    Ui::recordNavigation *ui;
};



#endif // RECORDNAVIGATION_H
