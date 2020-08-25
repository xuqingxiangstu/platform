#ifndef RECORDNAVIGATION_H
#define RECORDNAVIGATION_H

#include <QWidget>
#include <QMap>
#include <QMenu>
#include <QTreeWidget>
#include "./property/nodeproperty.h"
#include "../src/PfCommon/jsoncpp/json.h"

namespace Ui {
class recordNavigation;
}

struct recordRole
{
    int nodeType;       //节点类型
    std::string uuid;   //uuid   
    int sysType;        //系统类型
    std::shared_ptr<nodeProperty> mNodeProperty;
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

    /**
     * @brief onProjectModify
     * @param 工程修改触发
     */
    void onProjectModify(QString uuid);

    /**
     * @brief onProjectAlreadySave  工程已保存
     * @param uuid
     */
    void onProjectAlreadySave(QString uuid);

    void onPropertyValueChange(QString uuid, QString attr, Json::Value value);

    void onSaveProject(QTreeWidgetItem *item);

    void onSwitchItem(QString uuid);
signals:
    /**
     * @brief flowChange    点击流程变化槽
     * @param sysName   系统名称
     * @param sysType   系统类型
     * @param testName  测试项目
     * @param uuid      记录UUID
     */
    void flowChange(QString sysName, int sysType, QString testName, QString uuid, bool isNew);

    void deleteFlow(QString uuid);


    void toShowProperty(QString uuid, Json::Value);

    void clearFlowProperty(QString uuid, Json::Value);

    void setGroupPropertyEnable(QString propertyName, bool isEnable);

    void frameTypeChange(QString uuid, QString type);

    void removeGroupProperty(QString propertyName);
    void removeProperty(QString propertyName);

    void addProperty(QString propertyName, Json::Value v);
    void addGroupProperty(QString propertyName);
public:
    void buildTree();

    bool isModify(QString uuid);
    /**
     * @brief getModifyProject  获取已修改的工程
     * @return
     */
    QStringList getModifyProject();
    QTreeWidgetItem *curItem(QString uuid);
private:    
    /**
     * @brief findItem  根据Uuid查找item
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findItem(QString uuid);

    QTreeWidgetItem *createNode(const std::string &fatherUuid, const std::string &uuid, const std::string &name, NodeType type);

    QTreeWidgetItem *copyNode(const QTreeWidgetItem *srcNode, const std::string &uuid, const std::string &name);

    void deleteItem(QTreeWidgetItem *item);

    void enableAskAttr(recordRole);
    void disableAskAttr(recordRole);
private:
    void waring(QString text);
private:
    void onActionNew();
    void onActionDelete();
    void onActionCopy();
    void onActionPaste();
private:
    QTreeWidgetItem *mCopyItem;
    bool mIsCopy;
    QMenu *mPopMenu;
    QTreeWidgetItem *mCurSelectItem;    //当前选择Item
    QString mCurSelectUuid;             //当前选择的uuid
    QVector<QString> mNewUuid;          //新创建的uuid
    QMap<std::string, Json::Value> mDestDevInitValue;   //设备初始表属性
    const QString mModifyFlag = "[* 已修改]";   //已修改标志
    QMap<QString, bool> mIsModify; //是否修改
    QString mUiUuid;    //界面UUID
private:
    Ui::recordNavigation *ui;
};



#endif // RECORDNAVIGATION_H
