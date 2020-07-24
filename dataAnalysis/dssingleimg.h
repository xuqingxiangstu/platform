#ifndef DSSINGLEIMG_H
#define DSSINGLEIMG_H

#include <QWidget>
#include <QMenu>
#include <QSqlDatabase>
#include <QTreeWidgetItem>
#include "./chart/curvewiget.h"
#include "../src/PfSql/paramsTable/parseResult/resulttable.h"

namespace Ui {
class dsSingleImg;
}

#define CURVE_MAX_SIZE  5   ///< 最大曲线数

struct nodeRole
{
    int nodeType;       ///< 节点类型
    QString nodeUuid;   ///< 节点uuid;
};

Q_DECLARE_METATYPE(nodeRole)


class dsSingleImg : public QWidget
{
    Q_OBJECT
private:
    enum TreeItemIndex{
        Table_Index,
        Name_Index
    };

    enum NodeType{
        Table_Node,
        Coding_Node
    };
public:
    explicit dsSingleImg(QString proUuid, QString proPath, QWidget *parent = 0);
    ~dsSingleImg();
public slots:
    /**
     * @brief createTableNode 创建表号节点
     * @param uuid
     * @param table 表号
     * @return
     */
    void createTableNode(QString uuid, QString table);

    /**
     * @brief createCodingNode  创建编码节点
     * @param fatherUuid        父节点uuid
     * @param coding            编码
     * @param name              名称
     * @return
     */
    void createCodingNode(QString fatherUuid, QString coding, QString name);

    /**
     * @brief onDeleteCurve 删除曲线
     */
    void onDeleteCurve(QString uuid);
signals:
    /**
     * @brief addCurve  添加曲线
     * @param uuid
     * @param name
     */
    void addCurve(QString uuid, QString name);
private:
    /**
     * @brief updateTree    更新树
     */
    void updateTree();

    /**
     * @brief findItemFromUuid   通过uuid找到工程item
     * @param uuid
     * @return
     */
    QTreeWidgetItem *findItemFromUuid(const QString &uuid);

    void searchChild(QString strKey);

    void parentExpand(QTreeWidgetItem *item);

    void addCurveShow();
private:
    QStringList mHasCurves;
    QMenu *mPopMenu;
    QTreeWidgetItem *mCurSelectItem;    //当前选择Item
    curveWiget *mCurveWidget;
    QSqlDatabase mDb;
    QString mProPath;   ///< 工程路径
    QString mProUuid;   ///< 工程uuid
    QString mSelectField;
    const QString mTableName = "result";
private:
    Ui::dsSingleImg *ui;
};

#endif // DSSINGLEIMG_H
