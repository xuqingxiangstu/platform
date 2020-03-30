#include "pftreewidget.h"

#include <QApplication>
#include <QDrag>
#include <QDebug>
#include <QHeaderView>
#include <memory>
#include "dragrole.h"

PfTreeWidget::PfTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    mStartDragItem = nullptr;
    m_CtrlPressed = false;
    mIsMeetCondition = false;

    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setUniformRowHeights(true); //所有行等高（为例刷新的行号位置）

    this->header()->sectionResizeMode(QHeaderView::Stretch);
}



void PfTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_startDragPoint = event->pos();
    }
    QTreeWidget::mousePressEvent(event);
}


void PfTreeWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        m_CtrlPressed = true;
    }    
}

void PfTreeWidget::performDrag()
{
    QTreeWidgetItem *item = currentItem();
    if (item)
    {       
        QTreeWidgetItem *newItem = nullptr;

#if 0
        if(dragRole::Node_Cmd == stData.getNodeType())//如果为指令则重新构建，父节点+此节点
        {
            QTreeWidgetItem *parent = item->parent();
            newItem = new QTreeWidgetItem();
            for(int index = 0; index < parent->columnCount(); index++)
            {
                newItem->setText(index, parent->text(index));
                newItem->setData(index, Qt::UserRole, parent->data(index, Qt::UserRole));
            }
            newItem->addChild(item->clone());
        }
        else
        {
            newItem = item->clone();
        }
#endif

        newItem = item->clone();

        TreeItemMimeData *mimeData = new TreeItemMimeData;
        mimeData->SetDragData(APP_String,newItem);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/icon/new.png"));
        //drag->exec(Qt::CopyAction);
        drag->exec(Qt::MoveAction);


        //delete item;
    }
}

void PfTreeWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
    {
        m_CtrlPressed = false;
    }
}

void PfTreeWidget::mouseReleaseEvent(QMouseEvent *event)
{    
    QTreeWidget::mouseReleaseEvent(event);
}

void PfTreeWidget::startDrag(Qt::DropActions supportedActions)
{    
    Q_UNUSED(supportedActions);

    setSelectionMode(QAbstractItemView::SingleSelection);

    //判断条件
    QTreeWidgetItem *pMoveItem = this->currentItem(); //当前拖拽的Item
    if(!pMoveItem)
        return ;

    mStartDragItem = pMoveItem;
    std::shared_ptr<dragRole> stData = pMoveItem->data(0,Qt::UserRole).value<std::shared_ptr<dragRole>>();

    if(!( (dragRole::Node_Cmd == stData->getNodeType()) || (dragRole::Node_Param_Group == stData->getNodeType()) ) )
    {
        return ;
    }
    performDrag();
}



void PfTreeWidget::paintEvent(QPaintEvent *event)
{
    QTreeWidget::paintEvent(event);

    if(!m_MousePoint.isNull())
    {
        QTreeWidgetItem* phover = itemAt(m_MousePoint);
        QRect rect;

        QPainter painter(viewport());

        //3像素画笔
        QPen pen = painter.pen();
        pen.setWidth(3);
        painter.setPen(pen);

        if(phover)
        {
            rect = visualItemRect(phover);

            if(Line_Forbit == mLineType)
            {
                pen.setColor(Qt::red);
                painter.setPen(pen);
                drawLine(rect,painter);
            }
            else
            {
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                drawLine(rect,painter);
            }
        }
        else//空白处
        {
            if(Line_Forbit == mLineType)
            {
                pen.setColor(Qt::red);
                painter.setPen(pen);
                painter.drawLine(QPoint(10,m_MousePoint.y()),QPoint(200,m_MousePoint.y()));
            }
            else
            {
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                painter.drawLine(QPoint(10,m_MousePoint.y()),QPoint(200,m_MousePoint.y()));
            }
        }
    }
    else
    {

    }
}

bool PfTreeWidget::drawLine(QRect rect, QPainter &painter)
{
    bool bRet = false;
    if(rect.contains(m_MousePoint))
    {
        QPoint ptStart,ptEnd;
        if(m_MousePoint.y() - rect.topLeft().y() <=rect.height()/2.0) //上半部分
        {
            ptStart = rect.topLeft();
            ptEnd = QPoint(rect.topLeft().x()+200,rect.topLeft().y());
            m_bFrontInsert = true;
        }
        else
        {
            ptStart = rect.bottomLeft();
            ptEnd = QPoint(rect.bottomLeft().x()+200,rect.bottomLeft().y());
            m_bFrontInsert = false;
        }

        painter.drawLine(ptStart,ptEnd);

        bRet = true;
    }   

    return bRet;
}

void PfTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();

    QTreeWidget::dragLeaveEvent(event);
}

void PfTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasFormat(APP_String))
        return ;

    event->accept();
}

void PfTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{       
    if (!event->mimeData()->hasFormat(APP_String))
        return ;

#if 0
    if(this == event->source())
    {
        event->ignore();
        return ;
    }
#endif
    //****画红线
    m_MousePoint =  event->pos();
    viewport()->repaint();

    const TreeItemMimeData *pMimeData = (const TreeItemMimeData *)(event->mimeData());
    const QTreeWidgetItem *item = pMimeData->DragItemData();

    QTreeWidgetItem *currentItem = this->itemAt(event->pos());

    if(currentItem != nullptr)
    {
        std::shared_ptr<dragRole> stData = currentItem->data(0,Qt::UserRole).value<std::shared_ptr<dragRole>>();
        if(
                //(dragRole::Node_Flow == stData->getNodeType())
               // || (dragRole::Node_Cmd == stData.getNodeType())
                (dragRole::Node_Param == stData->getNodeType())
                )
        {
            event->ignore();
        }
        else
        {
            if( (dragRole::Node_Cmd_Group == stData->getNodeType())
                    || (dragRole::Node_Param_Group == stData->getNodeType())
                    )
            {
                //组节点下面不允许
                QRect rect;
                rect = visualItemRect(currentItem);
                if(rect.contains(m_MousePoint))
                {
                    if(m_MousePoint.y() - rect.topLeft().y() <=rect.height()/2.0) //上半部分
                    {

                    }
                    else
                    {
                        event->ignore();
                        mLineType = Line_Forbit;
                        //红线点至为空
                        m_MousePoint = QPoint();
                        return ;
                    }
                }
            }

            if( (dragRole::Node_Flow == stData->getNodeType())
                     )
             {
                 //子流程节点如果没有前兄弟节点则上面不允许，否则允许
                 QRect rect;
                 rect = visualItemRect(currentItem);
                 if(rect.contains(m_MousePoint))
                 {
                     if( (m_MousePoint.y() - rect.topLeft().y() <=rect.height()/2.0) //上半部分
                             && (currentItem->parent()->indexOfChild(currentItem) == 0))    //并且没有前兄弟节点
                     {
                         event->ignore();
                         mLineType = Line_Forbit;
                         //红线点至为空
                         m_MousePoint = QPoint();
                         return ;
                     }
                 }
             }
            if (currentItem == item)           //不允许拖回到原来的item
            {
                event->ignore();
            }
            else
            {
                setCurrentItem(currentItem);

                mLineType = Line_Permit;

                if(event->source() == this)
                   event->setDropAction(Qt::MoveAction);
                else
                   event->setDropAction(Qt::CopyAction);

                event->accept();

                return;
            }
        }
    }
    else//空白处
    {

        //如果没有节点则不允许
        if(this->topLevelItemCount() != 0)
        {
            //if(this->topLevelItem(this->topLevelItemCount() - 1)->childCount() > 0)
            {
                mLineType = Line_Permit;

                if(event->source() == this)
                   event->setDropAction(Qt::MoveAction);
                else
                   event->setDropAction(Qt::CopyAction);

                event->accept();
                mLineType = Line_Permit;
                m_MousePoint = QPoint();
                return;
            }
        }

        mLineType = Line_Forbit;
        event->ignore();
    }
    mLineType = Line_Forbit;
    m_MousePoint = QPoint();
}



QTreeWidgetItem *PfTreeWidget::getLastSubFlowItem()
{
    QTreeWidgetItem *lastItem = nullptr;

    int count = this->topLevelItemCount();
    if(count > 0)
    {
        QTreeWidgetItem *lastFlowItem = this->topLevelItem(count - 1);

        return lastFlowItem;
    }

    return lastItem;
}

void PfTreeWidget::setItemDes(QTreeWidgetItem *item, int clumIndex)
{
    if(!item)
        return ;

    item->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>()->getProperty()->setProperty(PROPERTY_DESCRIBE, Json::Value(item->text(clumIndex).toStdString()));

    for(int index = 0; index < item->childCount(); index++)
    {
        setItemDes(item->child(index), clumIndex);
    }

    return ;
}

void PfTreeWidget::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat(APP_String))
        return ;

    QTreeWidget *source =  dynamic_cast<QTreeWidget *>(event->source());
    const TreeItemMimeData *pMimeData = (const TreeItemMimeData *)(event->mimeData());
    if (source)
    {        
        const QTreeWidgetItem *item = pMimeData->DragItemData();
        QTreeWidgetItem *pItem = item->clone();
        QTreeWidgetItem *currentItem = this->itemAt(event->pos());

        if(source == this)
        {
            setItemDes(pItem, 0);
        }
        else
        {
            setItemDes(pItem, 1);
        }

        if (currentItem && (currentItem != item))
        {
            std::shared_ptr<dragRole> role = currentItem->data(0, Qt::UserRole).value<std::shared_ptr<dragRole>>();

            if( dragRole::Node_Flow == role->getNodeType())
            {
                if(m_bFrontInsert)//前面插入属于上个兄弟节点的最后一个
                {
                    emit dropDrag(currentItem->parent()->child(currentItem->parent()->indexOfChild(currentItem) - 1), nullptr, pItem, m_bFrontInsert);
                }
                else    //后面插入属于此节点的第一个
                {
                    if(currentItem->childCount() == 0)
                        emit dropDrag(currentItem, nullptr, pItem, false);
                    else
                        emit dropDrag(currentItem, currentItem->child(0), pItem, true);
                }
            }
            else if( dragRole::Node_Cmd == role->getNodeType()
                    || dragRole::Node_Param_Group == role->getNodeType()
                    )
            {
                emit dropDrag(currentItem->parent(), currentItem, pItem, m_bFrontInsert);
            }           
        }
        else    //空白处，添加最后一个节点
        {
            emit dropDrag(getLastSubFlowItem(), nullptr, pItem, false);
        }

        this->update();
        this->expandItem(pItem);

        if(event->source() == this)
           event->setDropAction(Qt::MoveAction);
        else
           event->setDropAction(Qt::CopyAction);

        event->accept();

        if(source == this)
        {
            deleteItem(mStartDragItem);
        }

    }

    m_MousePoint = QPoint();
}
void PfTreeWidget::deleteItem(QTreeWidgetItem *item)
{
    if(!item)
        return ;

    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem)
    {
        QTreeWidgetItem *del = parentItem->takeChild(parentItem->indexOfChild(item));
        if(del)
            delete del;
    }
    else
    {
        //this->takeTopLevelItem()
    }
}
