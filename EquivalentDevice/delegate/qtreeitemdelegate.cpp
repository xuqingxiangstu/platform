#include "qtreeitemdelegate.h"
#include <QProgressBar>
#include <QDateTimeEdit>

QtreeItemDelegate::QtreeItemDelegate(QObject *parent):QItemDelegate(parent)

{

}
QWidget *QtreeItemDelegate::createEditor(QWidget *parent,
const QStyleOptionViewItem &/*option*/,
const QModelIndex &/*index*/)const
{
    QProgressBar *progress = new QProgressBar(parent);
            progress->setValue(20);
        return progress;
}

