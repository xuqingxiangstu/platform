#ifndef RECORDNAVIGATION_H
#define RECORDNAVIGATION_H

#include <QWidget>

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

    void onPropertyValueChange(QString attr, Json::Value value);

    void onSaveProject(QTreeWidgetItem *item);
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


    void toShowProperty(Json::Value);

    void setSelfGroupPropertyEnable(QString propertyName, bool isEnable);

    void setGroupPropertyEnable(QString propertyName, bool isEnable);
public:
    void buildTree();

    bool isModify(){return mIsModify;}


    QTreeWidgetItem *curItem();
private:    
    /**
     * @brief findItem  根据Uuid查找item
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findItem(QString uuid);
    QTreeWidgetItem *createNode(const std::string &fatherUuid, const std::string &uuid, const std::string &name, NodeType type);
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
    QVector<QString> mNewUuid;          //新创建的uuid
    QMap<std::string, Json::Value> mDestDevInitValue;   //设备初始表属性
    const QString mModifyFlag = "[* 已修改]";   //已修改标志
    bool mIsModify; //是否修改
private:
    Ui::recordNavigation *ui;
};



#endif // RECORDNAVIGATION_H
