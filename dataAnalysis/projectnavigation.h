#ifndef PROJECTNAVIGATION_H
#define PROJECTNAVIGATION_H

#include <QWidget>
#include <QVector>
#include <QMenu>
#include <QPair>
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
    QString uuid;       ///< uuid
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
    void analysis(const QString &proUuid, const QString &dbPath, QVector<QPair<QString, std::shared_ptr<analysisRule>>> fileInfo);

    void toShowProperty(QString uuid, Json::Value);

    /**
     * @brief showWidget    显示表格窗体
     * @param uuid          工程uuid
     */
    void showTableWidget(QString uuid, QString dbname);

    /**
     * @brief showSingleImgWidget   显示单图
     * @param uuid                  工程uuid
     */
    void showSingleImgWidget(QString uuid, QString dbname);

    /**
     * @brief showMultImgWidget 显示全图
     * @param uuid
     */
    void showMultImgWidget(QString uuid, QString dbname);
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
    QString createProject(const QString &name, const QString &proPath, const QStringList &files = {});

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
private:
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
    QTreeWidgetItem *createFileGroupNode(const QString &uuid);

    void createFileNode(QTreeWidgetItem *fatherItem, const QString &uuid, const QStringList &files);

    QTreeWidgetItem *createTableNode(const QString &uuid);

    QTreeWidgetItem *createSingleImgNode(const QString &uuid);

    QTreeWidgetItem *createTimeImgNode(const QString &uuid);

    void onAddFiles();

    /**
     * @brief getDataFiles  获取数据文件路径
     * @return
     */
    QStringList getDataFiles(const QString &uuid);

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
private:
    QTreeWidgetItem *mCurSelectItem;    //当前选择Item
    QString mUiUuid;    //界面UUID
    QMenu *mPopMenu;
    QMap<QString, std::shared_ptr<projectCfg>> mProjcetCfgManager;  ///<系统配置管理
    std::shared_ptr<fileAnalysisBusiness> mFileAnalysisBusiness;    ///< 文件分析业务
private:
    Ui::projectNavigation *ui;
};

#endif // PROJECTNAVIGATION_H
