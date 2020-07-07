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
    explicit flowTree(QString uuid, int sysType = 0, QWidget *parent = 0);
    ~flowTree();
public:
    void keyPressEvent(QKeyEvent *event);
public slots:
    void onDropDrag(QTreeWidgetItem *parentItem, QTreeWidgetItem *curItem, QTreeWidgetItem *dropItem, bool isFrontInsert);

    void onItemClicked(QTreeWidgetItem * item, int column);

    void onPropertyValueChange(QString uuid, QString attr, Json::Value value);

    /**
     * @brief onUpdateProject 根据UUID读取文件并显示
     * @param uuid
     */
    void onUpdateProject(QString uuid);

    /**
     * @brief setSystemAndProductName   设置系统及测试项目名称
     * @param sysName       系统名称
     * @param produceName   测试项目
     */
    void setSystemAndProductName(QString sysName, QString produceName);

    /**
     * @brief saveProject   保存工程
     * @param uuid
     */
    void onSaveProject(QTreeWidgetItem *item);

    /**
     * @brief setFlowItemValue  设置流程ITEM值
     * @param role  值
     */
    void setFlowItemValue(const std::shared_ptr<dragRole> &role);

    /**
     * @brief setSubFlowItemValue   设置子流程ITEM值
     * @param flowUuid  流程UUID
     * @param role  值
     */
    void setSubFlowItemValue(QString flowUuid, const std::shared_ptr<dragRole> &role);

    /**
     * @brief setCmdItemValue   设置指令ITEM值
     * @param flowUuid          流程UUID
     * @param subFlowUuid       子流程UUID
     * @param role          值
     */
    void setCmdItemValue(QString subFlowUuid, const std::shared_ptr<dragRole> &role);

    /**
     * @brief setParamItemValue 设置参数ITEM值
     * @param flowUuid          流程UUID
     * @param subFlowUuid       子流程UUID
     * @param role          值
     * @param subRole       子值
     */
    void setParamItemValue(QString subFlowUuid, const std::shared_ptr<dragRole> &role, const std::vector<std::shared_ptr<dragRole>> &subRole);

    /**
     * @brief onShowCurItemProperty 显示当前选择节点属性
     */
    void onShowCurItemProperty(QString uuid);

    /**
     * @brief onFrameTypeChange 帧类型改变
     * @param type 类型
     */
    void onFrameTypeChange(QString uuid, QString type);

    void onPasteCmdItem(std::shared_ptr<dragRole> role);
private:
    void onNewFlowNode();
    void onCopyNode();
    void onParseNode();
    void onDeleteNode();

    void disable1553BAttr(std::shared_ptr<dragRole> role);
    void enable1553BAttr(std::shared_ptr<dragRole> role, bool isNew = true);

    QTreeWidgetItem *getPasteBrother(const dragRole::nodeType &type);
signals:
    void toShowProperty(QString uuid, Json::Value);

    void updateProperty(QString propertyName, Json::Value value);

    void setGroupPropertyEnable(QString propertyName, bool isEnable);

    void removeProperty(QString propertyName);
    void removeGroupProperty(QString propertyName);

    void addProperty(QString propertyName, Json::Value v);
    void addGroupProperty(QString propertyName);

    /**
     * @brief projectModify
     * @param 工程修改触发
     */
    void projectModify(QString uuid);

    /**
     * @brief saveProjectOver   保存结束
     */
    void saveProjectOver(QString uuid);

    /**
     * @brief singleTeset   单项测试
     * @param value 测试项
     */
    void singleTeset(Json::Value value);
private:
    /**
     * @brief newItem   新创建Item
     * @param dropItem  拖拽的模板item
     * @return
     */
    QTreeWidgetItem *newItem(QTreeWidgetItem *dropItem);

    QTreeWidgetItem *newCmdItem(QTreeWidgetItem *dropItem);
    QTreeWidgetItem *newParamItem(QTreeWidgetItem *dropItem);

    void deleteItem(QTreeWidgetItem *item);

    void onMenuTrigger(QAction * action);

    void onActionTestSend(QTreeWidgetItem *);

    void onActionDelete(QTreeWidgetItem *);

    /**
     * @brief getItemSize   获取item总数
     * @param curWidget     树句柄
     * @return  总数
     */
    int getItemSize(QTreeWidget *curWidget);

    /**
     * @brief findItem  根据Uuid查找item
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findItem(QString uuid);

    /**
     * @brief updateCmdOrParamGroupItemValue   更指令或参数组节点相关值
     * @param role  数据
     */
    void updateCmdOrParamGroupItemValue(QTreeWidgetItem *item, std::shared_ptr<dragRole> role);

    /**
     * @brief updateParamItemValue   更参数节点相关值
     * @param role  数据
     */
    void updateParamItemValue(QTreeWidgetItem *item, std::shared_ptr<dragRole> role);

    void updateFrameAttr(std::shared_ptr<dragRole> role);

    /**
     * @brief updateInfoWordAttr    更新信息字相关属性
     * @param role
     */
    void updateInfoWordAttr(std::shared_ptr<dragRole> role);

    /**
     * @brief updateParamSelAttr    更新参数选择相关属性
     * @param role
     */
    void updateParamSelAttr(std::shared_ptr<dragRole> role);
private:
    QMenu *mPopMenu;
    QPoint mRightMousePoint;
    QTreeWidgetItem *mCurItem;    
    QTreeWidgetItem *mCopyItem;
    bool mIsCopy;
    QString mCurProjectUuid;    ///< 当前工程uuid
    int mCurSystemType;         ///< 当前系统类型
    bool mIsUpdateTree;         ///< 是否更新树
    QString mUiUuid;    ///< 界面UUID(唯一标识)
    std::string mCurFrameType;  ///< 当前帧类型
    QTreeWidgetItem *mCurSelectItem;    ///< 当前选择item
private:
    Ui::flowTree *ui;
};

#endif // FLOWTREE_H
