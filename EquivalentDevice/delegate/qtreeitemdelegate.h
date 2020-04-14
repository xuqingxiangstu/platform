#ifndef QTREEITEMDELEGATE_H
#define QTREEITEMDELEGATE_H

#include<QItemDelegate>
class QtreeItemDelegate:public QItemDelegate
{
    Q_OBJECT
public:
    QtreeItemDelegate(QObject * parent=0);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)const;
};
#endif // QTREEITEMDELEGATE_H
