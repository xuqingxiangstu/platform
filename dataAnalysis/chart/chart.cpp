#include "chart.h"
#include "lineSeries.h"

#include <QValueAxis>
#include <QDebug>

// --------------------------------------------------------------------------------
// 	FUNCTION: Chart (public )
// --------------------------------------------------------------------------------
Chart::Chart() : QChart(),
	m_bounds(0,0,0,0)
{

}

// --------------------------------------------------------------------------------
// 	FUNCTION: bounds (public )
// --------------------------------------------------------------------------------
QRectF Chart::bounds() const
{
	return m_bounds;
}

// --------------------------------------------------------------------------------
// 	FUNCTION: createDefaultAxes (public )
// --------------------------------------------------------------------------------
void Chart::createDefaultAxes()
{
	QChart::createDefaultAxes();
	enableBoundChanges(true);
	connect(dynamic_cast<QValueAxis*>(axisX()), &QValueAxis::rangeChanged, this, &Chart::changeZoomLevel, Qt::UniqueConnection);
	connect(dynamic_cast<QValueAxis*>(axisY()), &QValueAxis::rangeChanged, this, &Chart::changeZoomLevel, Qt::UniqueConnection);
}

// --------------------------------------------------------------------------------
// 	FUNCTION: addSeries (public )
// --------------------------------------------------------------------------------
void Chart::addSeries(LineSeries* series)
{
	QChart::addSeries(series);
	emit seriesAdded(series);

	if (m_bounds == QRectF(0, 0, 0, 0))
		m_bounds = series->bounds();
	else
		m_bounds.united(series->bounds());
	emit boundsChanged(m_bounds);
}

// --------------------------------------------------------------------------------
// 	FUNCTION: zoomIn ()
// --------------------------------------------------------------------------------
void Chart::zoom(const QRectF &rect)
{
	enableBoundChanges(false);
	
	dynamic_cast<QValueAxis*>(axisX())->setRange(rect.left(), rect.width() + rect.left());
	dynamic_cast<QValueAxis*>(axisY())->setRange(rect.top(), rect.height() + rect.top());
	
	QRectF realRect = changeZoomLevel(false);
	if (m_bounds == realRect)
	{
		emit zoomed(realRect);
	}
	else if (realRect.left() < m_bounds.left() || realRect.right() > m_bounds.right())
	{
		this->zoomReset();
	}
	else
	{
		emit zoomed(realRect);
	}	
	enableBoundChanges(true);

	return;
}

// --------------------------------------------------------------------------------
// 	FUNCTION: zoomReset ()
// --------------------------------------------------------------------------------
void Chart::zoomReset()
{
	enableBoundChanges(false);
	dynamic_cast<QValueAxis*>(axisX())->setRange(m_bounds.left(), m_bounds.width() + m_bounds.left());
	dynamic_cast<QValueAxis*>(axisY())->setRange(m_bounds.top(), m_bounds.height() + m_bounds.top());
	enableBoundChanges(true);
	changeZoomLevel();
	return;
}

// --------------------------------------------------------------------------------
// 	FUNCTION: changeZoomLevel (public )
// --------------------------------------------------------------------------------
QRectF Chart::changeZoomLevel(bool emitSignal /*= true*/)
{
	auto xAxis = dynamic_cast<QValueAxis*>(this->axisX());
	auto yAxis = dynamic_cast<QValueAxis*>(this->axisY());
	auto rect = QRectF(xAxis->min(), yAxis->min(), xAxis->max() - xAxis->min(), yAxis->max() - yAxis->min());
	if (emitSignal){ zoomed(rect); }
	return rect;
}

// --------------------------------------------------------------------------------
// 	FUNCTION: enableBoundChanges (protected )
// --------------------------------------------------------------------------------
void Chart::enableBoundChanges(bool val)
{
	if (val)
	{
		if (!m_xBoundsConnection)
		{
			auto xAxis = dynamic_cast<QValueAxis*>(this->axisX());
            if(xAxis)
            {
                m_xBoundsConnection = connect(xAxis, &QValueAxis::rangeChanged, this, [this](qreal min, qreal max)
                {
                    if (min < m_bounds.left()) { m_bounds.setLeft(min); }
                    if (max > m_bounds.right()) { m_bounds.setRight(max); }
                    emit boundsChanged(m_bounds);
                }, Qt::UniqueConnection);
            }
		}

		if (!m_yBoundsConnection)
		{
			auto yAxis = dynamic_cast<QValueAxis*>(this->axisY());
            if(yAxis)
            {
                m_yBoundsConnection = connect(yAxis, &QValueAxis::rangeChanged, this, [this](qreal min, qreal max)
                {
                    // remember, for QRect the 'top' is the lower value.
                    if (min < m_bounds.top()) { m_bounds.setTop(min); }
                    if (max > m_bounds.bottom()) { m_bounds.setBottom(max); }
                    emit boundsChanged(m_bounds);
                }, Qt::UniqueConnection);
            }
		}
	}
	else
	{
        bool res = disconnect(m_xBoundsConnection);
        res = disconnect(m_yBoundsConnection);
	}
}
