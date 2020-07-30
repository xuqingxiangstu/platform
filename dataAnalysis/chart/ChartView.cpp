#include "ChartView.h"
#include <QApplication>
#include <QValueAxis>
#include <QGraphicsLayout>
#include <QDebug>
#include <QLineSeries>
#include <algorithm>

ChartView::ChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
	m_isPress = false;
	m_alreadySaveRange = false;
    this->setMouseTracking(false);
}

ChartView::~ChartView()
{
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
	{
        m_lastPoint = event->pos();
        m_isPress = true;
        this->setDragMode(QGraphicsView::NoDrag);
	}
    else if(event->button() == Qt::LeftButton)
    {
        beginPoint = event->pos();
        this->setDragMode(QGraphicsView::RubberBandDrag);
    }
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
	const QPoint curPos = event->pos();

    if ((event->buttons() == Qt::MiddleButton) && event->type() == QEvent::MouseMove)
//    if (m_isPress)
	{
        this->setDragMode(QGraphicsView::NoDrag);
        QPoint offset = curPos - m_lastPoint;
		m_lastPoint = curPos;
		if (!m_alreadySaveRange)
		{
			this->saveAxisRange();
			m_alreadySaveRange = true;
		}
		this->chart()->scroll(-offset.x(), offset.y());

        setCursor(Qt::ClosedHandCursor);
	}

    else if(event->type() == QEvent::MouseMove && (event->buttons() == Qt::LeftButton))
    {
        emit mouseMovePoint(curPos);
        if (!m_alreadySaveRange)
        {
            this->saveAxisRange();
            m_alreadySaveRange = true;
        }
        this->setDragMode(QGraphicsView::RubberBandDrag);
    }      

   findNearlyPoint(this->chart()->mapToValue(event->pos()));

   QChartView::mouseMoveEvent(event);
}

void ChartView::findNearlyPoint(QPointF curPoint)
{     //axisX
    QList<QAbstractSeries *> series = this->chart()->series();
    for(QAbstractSeries *serie : series)
    {
        if(mShowSeries != serie)
        {
            continue ;
        }

        QLineSeries *lineS = dynamic_cast<QLineSeries*>(serie);
        QVector<QPointF> points = lineS->pointsVector();

        mMousePointColor = lineS->color();

        //第一个大于的点
        auto maxItor = std::find_if(points.begin(), points.end(), [=](QPointF pf){
            return pf.x() >= curPoint.x();
        });

        if(maxItor != points.end())
        {
            //qDebug() << *maxItor;
            if(maxItor == points.begin())   //第一个点
            {
                qDebug() << points.begin();
                //画红线
               m_MousePoint = this->chart()->mapToPosition(*(points.begin())).toPoint();
            }
            else
            {
                //和前一个点比较谁近
                QPointF maxF = *maxItor;
                maxItor--;
                QPointF minF = *maxItor;

                if( (maxF.x() - curPoint.x()) > (curPoint.x() - minF.x()))
                {
                    //qDebug() << minF;
                    m_MousePoint = this->chart()->mapToPosition(minF).toPoint();
                }
                else
                {
                    //qDebug() << maxF;
                    m_MousePoint = this->chart()->mapToPosition(maxF).toPoint();
                }
            }
        }
        else
        {
            //最后一个点
            //qDebug() << points.end();
            //m_MousePoint = this->chart()->mapToPosition(*(points.end())).toPoint();
            m_MousePoint = QPoint();
        }

        if(!m_MousePoint.isNull())
            emit pointHovered(serie, this->chart()->mapToValue(m_MousePoint), true);
        else
            emit pointHovered(serie, this->chart()->mapToValue(m_MousePoint), false);

        viewport()->repaint();
        break;
    }
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPress = false;

    if (event->button() == Qt::RightButton)
    {
        if (m_alreadySaveRange)
        {
            this->chart()->axisX()->setRange(m_xMin,m_xMax);
            this->chart()->axisY()->setRange(m_yMin,m_yMax);
            this->chart()->zoomReset();
            m_alreadySaveRange = false;
        }
    }

    else if(event->button() == Qt::LeftButton)
    {
        //鼠标左键释放，获取矩形框的endPoint,进行缩放
        endPoint = event->pos();
        QRectF rectF;
        rectF.setTopLeft(this->beginPoint);
        rectF.setBottomRight(this->endPoint);
        this->chart()->zoomIn(rectF);
        this->setDragMode(QGraphicsView::RubberBandDrag);
    }
    else if(event->button() == Qt::MidButton)
    {
        setCursor(Qt::ArrowCursor);
    }
    QChartView::mouseReleaseEvent(event);

    findNearlyPoint(this->chart()->mapToValue(event->pos()));
}

void ChartView::enterEvent(QEvent *event)
{
    QChartView::enterEvent(event);
}

void ChartView::leaveEvent(QEvent *event)
{
    QChartView::leaveEvent(event);

    m_MousePoint = QPoint();

    viewport()->repaint();

    emit pointHovered(nullptr, this->chart()->mapToValue(m_MousePoint), false);

    setCursor(Qt::ArrowCursor);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if (!m_alreadySaveRange)
    {
        this->saveAxisRange();
        m_alreadySaveRange = true;
    }

    if(event->delta() > 0 )
    {
        this->chart()->zoom(1.2);
    }
    else
    {
        this->chart()->zoom(10.0/12);
    }

    QChartView::wheelEvent(event);

    findNearlyPoint(this->chart()->mapToValue(event->pos()));
}


void ChartView::saveAxisRange()
{
    QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
	m_xMin = axisX->min();
	m_xMax = axisX->max();
    QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
	m_yMin = axisY->min();
	m_yMax = axisY->max();
}

void ChartView::paintEvent(QPaintEvent *event)
{
    QChartView::paintEvent(event);
#if 1
    if(!m_MousePoint.isNull())
    {
        QRect rect;

        QPainter painter(viewport());

        //3像素画笔
        QPen pen = painter.pen();
        pen.setWidth(1);
        pen.setColor(Qt::darkGray);
        painter.setPen(pen);
        //this->chart()->axisY()->
        //painter.drawLine(QPoint(m_MousePoint.x(), this->chart()->size().height()),QPoint(m_MousePoint.x(), 0));
        //this->chart()->layout()->contentsRect().height();

        painter.drawLine(QPoint(m_MousePoint.x(), this->chart()->layout()->contentsRect().height()),QPoint(m_MousePoint.x(), 0));
        //painter.drawLine(QPoint(this->chart()->size().width(), m_MousePoint.y()),QPoint(0, m_MousePoint.y()));

        //QRectF rectangle1(10.0, 100.0, 80.0, 80.0);
           //QPainter painter(this);

        pen.setWidth(4);
        pen.setColor(mMousePointColor);
        painter.setPen(pen);
        painter.drawEllipse(m_MousePoint.x() - 2, m_MousePoint.y() - 2, 4,4);
    }
#endif


}
