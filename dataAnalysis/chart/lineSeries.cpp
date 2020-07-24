#include "lineSeries.h"

#include <QGuiApplication>
#include <QDebug>

// --------------------------------------------------------------------------------
// 	FUNCTION: LineSeries (public )
// --------------------------------------------------------------------------------
LineSeries::LineSeries() : QLineSeries(),
	m_xMin(std::numeric_limits<qreal>::max()),
	m_xMax(std::numeric_limits<qreal>::min()),
	m_yMin(std::numeric_limits<qreal>::max()),
	m_yMax(std::numeric_limits<qreal>::min())
{
	connect(this, &LineSeries::pressed, this, [this]()
	{
		if (QGuiApplication::mouseButtons() & Qt::LeftButton)
		{
			emit leftClicked(this);
		}
		else if (QGuiApplication::mouseButtons() & Qt::RightButton)
		{
			emit rightClicked(this);
		}
	});
}

// --------------------------------------------------------------------------------
// 	FUNCTION: append ()
// --------------------------------------------------------------------------------
void LineSeries::append(qreal x, qreal y)
{
	if (x > m_xMax){ m_xMax = x;}
	if (x < m_xMin){ m_xMin = x;}
	if (y > m_yMax){ m_yMax = y;}
	if (y < m_yMin){ m_yMin = y;}
	QLineSeries::append(x, y);
}

// --------------------------------------------------------------------------------
// 	FUNCTION: bounds ()
// --------------------------------------------------------------------------------
QRectF LineSeries::bounds() const
{
	return QRectF(m_xMin, m_yMin, m_xMax - m_xMin, m_yMax - m_yMin);
}