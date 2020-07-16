#ifndef PROJECTNAVIGATION_H
#define PROJECTNAVIGATION_H

#include <QWidget>
#include <QVector>
#include <QMenu>
#include <QPair>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "../src/PfCommon/jsoncpp/json.h"
#include "./property/nodeproperty.h"

namespace Ui {
class projectNavigation;
}

struct recordRole
{
    int nodeType;       ///< 节点类型
    QString uuid;       ///< uuid
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
    /**
     * @brief analysis  数据分析
     * @param fileInfo  文件信息（多个文件:uuid+filepath）
     */
    //void analysis(QVector<QPair<QString, QString>> fileInfo);

    void toShowProperty(QString uuid, Json::Value);
public slots:
    /**
     * @brief analysisResult    数据分析结果
     * @param result            文件信息（多个文件:uuid+filepath）
     */
    //void analysisResult(QVector<QPair<QString, QString>> result);

    /**
     * @brief createProject 创建工程
     * @param name          工程名称
     * @param uuid          uuid
     * @param files         数据文件
     */
    void createProject(const QString &name, const QString &uuid, const QStringList &files = {});

    /**
     * @brief onAnalysis    解析
     */
    void onAnalysis();
private:
    /**
     * @brief createPrjNode 创建工程节点
     * @param name
     * @return
     */
    QTreeWidgetItem *createPrjNode(const QString &uuid, const QString &name);

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
     * @brief findPrjFromUuid   通过uuid找到工程item
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findPrjFromUuid(const QString &uuid);
private:
    QTreeWidgetItem *mCurSelectItem;    //当前选择Item
    QString mUiUuid;    //界面UUID
    QMenu *mPopMenu;
private:
    Ui::projectNavigation *ui;
};

#endif // PROJECTNAVIGATION_H
