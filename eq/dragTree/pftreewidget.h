#ifndef PFTREEWIDGET_H
#define PFTREEWIDGET_H

#include <QTreeWidget>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

#include <QAbstractItemView>

#define APP_String "PFTREE"

class TreeItemMimeData:public QMimeData
{
    Q_OBJECT
public:
    TreeItemMimeData():
        QMimeData()
    {
        m_pDragItem = NULL;
    }

    ~TreeItemMimeData()
    {

    }

    void SetDragData(QString mimeType , QTreeWidgetItem *pItem)
    {
        m_format<<mimeType;
        m_pDragItem = pItem;
    }

    QStringList formats() const
    {
        return m_format;
    }

    const QTreeWidgetItem* DragItemData() const
    {
        return m_pDragItem;
    }


protected:

    QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const
    {
        if (mimetype == APP_String)
        {
            return m_pDragItem;
        }
        else
        {
             return QMimeData::retrieveData(mimetype, preferredType);
        }
    }
private:
    const QTreeWidgetItem   *m_pDragItem;
    QStringList              m_format;
};

class PfTreeWidget : public QTreeWidget
{
    Q_OBJECT

    enum TreeWidgetColumn {
        SYSTEM_ROLE,
        TYPE_ROLE,
        INFO_ROLE
    };
    enum DragLineType
    {
        Line_Forbit,    //禁止
        Line_Permit     //允许
    };

public:
    PfTreeWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
signals:
    void dropDrag(QTreeWidgetItem *parentItem, QTreeWidgetItem *curItem, QTreeWidgetItem *dropItem, bool isFrontInsert);
private:
    void performDrag();
    QTreeWidgetItem *getLastSubFlowItem();
    QTreeWidgetItem *mStartDragItem;
    void setItemDes(QTreeWidgetItem *item, int index);
    void deleteItem(QTreeWidgetItem *item);
    bool drawLine(QRect rect, QPainter &painter);
private:
    QPoint m_MousePoint;            //拖动时红线开始点
    bool m_bFrontInsert;            //前/后插入
    QPoint     m_startDragPoint;
    bool       m_CtrlPressed;
    bool mIsMeetCondition;
    DragDropMode mDragMode;
    DragLineType mLineType;
};

#endif // PFTREEWIDGET_H
