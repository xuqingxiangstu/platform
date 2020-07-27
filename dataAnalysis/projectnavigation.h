#ifndef PROJECTNAVIGATION_H
#define PROJECTNAVIGATION_H

#include <QWidget>
#include <QVector>
#include <QMenu>
#include <QPair>
#include <tuple>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "./project/projectcfg.h"
#include "../src/PfCommon/jsoncpp/json.h"
#include "./property/nodeproperty.h"
#include "fileanalysisbusiness.h"

namespace Ui {
class projectNavigation;
}

struct recordRole
{
    int nodeType;       ///< 节点类型
    QString proUuid;    ///< proUuid：工程UUID
    QString nodeUuid;    ///< 节点UUID
    QString proPath;    ///< 工程路径
    QString filePath;   ///< 文件路径,当DataFile_Node有效
    int sysType;        ///< 系统类型
    std::shared_ptr<nodeProperty> mNodeProperty;
};

Q_DECLARE_METATYPE(recordRole)

class projectNavigation : public QWidget
{
    Q_OBJECT
    enum TreeItemIndex{
        Name_Index
    };

    enum NodeType{
        Prj_Node,
        DataFile_Group_Node,
        DataFile_Node,
        Table_Node,
        S_Img_Node,
        Time_Img_Node
    };
public:
    explicit projectNavigation(QWidget *parent = 0);
    ~projectNavigation();
signals:
    void showMessage(QString msg, bool state);
    /**
     * @brief analysis  数据分析
     * @param proUuid   工程UUID
     * @param fileInfo  文件信息（多个文件:uuid+filepath）
     */  
    void analysis(const QString &proUuid, const QString &dbPath, QVector<std::tuple<QString, std::shared_ptr<analysisRule>, std::shared_ptr<filterManager>>> fileInfo);

    void toShowProperty(QString uuid, Json::Value);

    /**
     * @brief showWidget    显示表格窗体
     * @param uuid          工程uuid
     */
    void showTableWidget(QString uuid, QString proPath);

    /**
     * @brief showSingleImgWidget   显示单图
     * @param uuid                  工程uuid
     */
    void showSingleImgWidget(QString uuid, QString proPath);

    /**
     * @brief showMultImgWidget 显示全图
     * @param uuid
     */
    void showMultImgWidget(QString uuid, QString proPath);

    /**
     * @brief saveProjectOver   保存结束
     */
    void saveProjectOver(QString uuid);
public slots:
    /**
     * @brief analysisResult    数据分析结果
     * @param result            文件信息（多个文件:uuid+filepath）
     */
    //void analysisResult(QVector<QPair<QString, QString>> result);

    /**
     * @brief createProject 创建工程
     * @param name          工程名称
     * @param proPath       工程路径
     * @param files         数据文件
     * @return uuid
     */
    QString createProject(const QString &name, QString proPath, const QStringList &files = {});

    /**
     * @brief loadProject   加载工程
     * @param proFile       工程文件
     */
    void loadProject(const QString &proFile);

    /**
     * @brief onAnalysis    解析
     */
    void onAnalysis();

    /**
     * @brief onDoubleClicked   双击槽函数
     * @param item
     * @param column
     */
    void onDoubleClicked(QTreeWidgetItem *item, int column);

    /**
     * @brief onClicked   单机击槽函数
     * @param item
     * @param column
     */
    void onClicked(QTreeWidgetItem *item, int column);

    /**
     * @brief onPropertyValueChange
     * @param uuid
     * @param attr
     * @param val
     */
    void onPropertyValueChange(QString uuid, QString attr, Json::Value val);

    void onProjectModify(QString uuid);

    void onSaveProject(QString uuid);

    /**
     * @brief onProjectAlreadySave  工程已保存
     * @param uuid
     */
    void onProjectAlreadySave(QString uuid);
public:
    bool isModify(QString uuid);
    /**
     * @brief getModifyProject  获取已修改的工程
     * @return
     */
    QStringList getModifyProject();
private:

    /**
     * @brief nodeUuidConvertProUuid    节点uuid转工程uuid
     * @param nodeUuid                  节点uuid
     * @return          工程uuid
     */
    QString nodeUuidConvertProUuid(const QString &nodeUuid);

    void expandNode(QTreeWidgetItem *item);
    /**
     * @brief findItemByProUuid 通过工程uuid查找
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findItemByProUuid(QString uuid);

    /**
     * @brief findItemByNodeUuid 通过节点uuid查找
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findItemByNodeUuid(QString uuid);
    /**
     * @brief createPrjNode 创建工程节点
     * @param name
     * @return
     */
    QTreeWidgetItem *createPrjNode(const QString &uuid, const QString &proPath, const QString &name);

    /**
     * @brief createFileNode    创建文件节点
     * @param files             文件列表
     * @return
     */
    QTreeWidgetItem *createFileGroupNode(const QString &uuid, const QString &proPath);

    void createFileNode(QTreeWidgetItem *fatherItem, const QString &uuid, const QString &proPath, const QVector<QPair<QString, QString>> &files, bool isInit = false);

    QTreeWidgetItem *createTableNode(const QString &uuid, const QString &proPath);

    QTreeWidgetItem *createSingleImgNode(const QString &uuid, const QString &proPath);

    QTreeWidgetItem *createTimeImgNode(const QString &uuid, const QString &proPath);

    void onAddFiles();

    void onDeleteFile();

    /**
     * @brief getDataFilesJson  获取数据文件路径及json
     * @return
     */
    QVector<QPair<QString, QString>> getDataFilesJson(const QString &uuid);

    /**
     * @brief getDataFiles  获取数据文件路径及uuid
     * @return
     */
    QVector<QPair<QString, QString>> getDataFilesUuid(const QString &uuid);

    /**
     * @brief getDbPathByUuid   通过uuid找到数据库文件路径
     * @param uuid              工程uuid
     * @return                  数据库路径
     */
    QString getDbPathByUuid(const QString &uuid);

    /**
     * @brief findPrjFromUuid   通过uuid找到工程item
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findPrjFromUuid(const QString &uuid);

    void deleteItem(QTreeWidgetItem *item);
private:
    QTreeWidgetItem *mCurSelectItem;    //当前选择Item
    QString mUiUuid;    //界面UUID
    QMenu *mPopMenu;
    QMap<QString, std::shared_ptr<projectCfg>> mProjcetCfgManager;  ///<系统配置管理
    std::shared_ptr<fileAnalysisBusiness> mFileAnalysisBusiness;    ///< 文件分析业务
    const QString mModifyFlag = "[* 已修改]";   //已修改标志
    QMap<QString, bool> mIsModify; //是否修改
private:
    Ui::projectNavigation *ui;
};

#endif // PROJECTNAVIGATION_H
