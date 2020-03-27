#ifndef FLOWTREE_H
#define FLOWTREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QMenu>

#include "./dragTree/dragrole.h"

#include "../src/PfCommon/jsoncpp/json.h"

namespace Ui {
class flowTree;
}

class flowTree : public QWidget
{
    Q_OBJECT
    enum TreeWidgetItemIndex{
        Name_Index,
        Value_Index,
        Param_Change_Index,
        Start_Cond_Index,
        Delay_Index,
        Stop_Cond_Index,
        Timming_Index,
        Dest_Dev_Index
    };
public:
    explicit flowTree(QString uuid, QWidget *parent = 0);
    ~flowTree();
public slots:
    void onDropDrag(QTreeWidgetItem *parentItem, QTreeWidgetItem *curItem, QTreeWidgetItem *dropItem, bool isFrontInsert);

    void onItemClicked(QTreeWidgetItem * item, int column);

    void onPropertyValueChange(QString attr, Json::Value value);

    /**
     * @brief updateProject 根据UUID读取文件并显示
     * @param uuid
     */
    void updateProject();

    /**
     * @brief setSystemAndProductName   设置系统及测试项目名称
     * @param sysName       系统名称
     * @param produceName   测试项目
     */
    void setSystemAndProductName(QString sysName, QString produceName);

    /**
     * @brief saveProject   保存工程
     */
    void saveProject();
signals:
    void toShowProperty(Json::Value);

    /**
     * @brief projectModify
     * @param 工程修改触发
     */
    void projectModify(QString uuid);
private:
    /**
     * @brief newItem   新创建Item
     * @param dropItem  拖拽的模板item
     * @return
     */
    QTreeWidgetItem *newItem(QTreeWidgetItem *dropItem);

    QTreeWidgetItem *newCmdItem(QTreeWidgetItem *dropItem);
    QTreeWidgetItem *newParamItem(QTreeWidgetItem *dropItem);

    void onMenuTrigger(QAction * action);

    void onActionTestSend(QTreeWidgetItem *);
private:
    QMenu *mPopMenu;
    QPoint mRightMousePoint;
    QTreeWidgetItem *mCurItem;
    bool mIsModify; //是否修改
    QString mCurProjectUuid;    //当前工程uuid
private:
    Ui::flowTree *ui;
};

#endif // FLOWTREE_H
