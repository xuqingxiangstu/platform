/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "callout.h"
#include <QApplication>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QValueAxis>
#include <QtCharts/QChart>
#include <QtGui/QFontMetrics>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>

Callout::Callout(QChart *chart) :
    QGraphicsItem(chart),
	m_seriesName(""),
    m_chart(chart)
{
	this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsScenePositionChanges| QGraphicsItem::ItemIsFocusable);
	setZValue(11);
	setBrush(QColor(QRgb(0x3d3d3d)));
}

QRectF Callout::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect;
    rect.setLeft(qMin(m_rect.left(), anchor.x()));
    rect.setRight(qMax(m_rect.right(), anchor.x()));
    rect.setTop(qMin(m_rect.top(), anchor.y()));
    rect.setBottom(qMax(m_rect.bottom(), anchor.y()));
    return rect;
}


// --------------------------------------------------------------------------------
// 	FUNCTION: setBrush (public )
// --------------------------------------------------------------------------------
void Callout::setBrush(QBrush brush)
{
	m_brush = brush;

	// determine text color: http://stackoverflow.com/questions/1855884/determine-font-color-based-on-background-color
	double a = 1 - (0.299 * m_brush.color().red() + 0.587 * m_brush.color().green() + 0.114 * m_brush.color().blue()) / 255;

	if (a < 0.5)
		m_textColor = Qt::black; // bright colors - black font
	else
		m_textColor = Qt::white; // dark colors - white font

	this->update();
}

void Callout::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    QPainterPath path;
    path.addRoundedRect(m_rect, 5, 5);

    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    if (!m_rect.contains(anchor)) 
	{
		QRectF plotRect = mapFromParent(m_chart->plotArea()).boundingRect();
		if (!plotRect.contains(anchor))
		{
			anchor.rx() = anchor.x() < plotRect.left() ? plotRect.left() : plotRect.right();
		}

		QPointF point1, point2;
	
		// establish the position of the anchor point in relation to m_rect
		bool above = anchor.y() <= m_rect.top();
		bool aboveCenter = anchor.y() > m_rect.top() && anchor.y() <= m_rect.center().y();
		bool belowCenter = anchor.y() > m_rect.center().y() && anchor.y() <= m_rect.bottom();
		bool below = anchor.y() > m_rect.bottom();
	
		bool onLeft = anchor.x() <= m_rect.left();
		bool leftOfCenter = anchor.x() > m_rect.left() && anchor.x() <= m_rect.center().x();
		bool rightOfCenter = anchor.x() > m_rect.center().x() && anchor.x() <= m_rect.right();
		bool onRight = anchor.x() > m_rect.right();
	
		// get the nearest m_rect corner.
		qreal x = m_rect.left() + m_rect.width() / 2;
		qreal y = below ? m_rect.bottom() : m_rect.top();
		bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
		bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);
	
		point1.setX(x - 5);
		point1.setY(y);
	
		point2.setX(x + 5);
		point2.setY(y);
	
		path.moveTo(point1);
		path.lineTo(anchor);
		path.lineTo(point2);
		path = path.simplified();
    }

	if(option->state & QStyle::State_Selected)
		painter->setBrush(m_brush.color().lighter());
	else
		painter->setBrush(m_brush);
    painter->drawPath(path);
	painter->setPen(m_textColor);
    painter->drawText(m_textRect, m_text);
}


QPainterPath Callout::shape() const
{
	QPainterPath path;
	path.addRect(m_rect);
	return path;
}

void Callout::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
	{
        setPos(mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)));
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

QVariant Callout::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange)
	{
		return clampY(value.toPointF());
	}

	return QGraphicsItem::itemChange(change, value);
}

QPointF Callout::clampY(QPointF point) const
{
	QRectF plotRect = m_chart->plotArea();

	if (point.y() < plotRect.top() + plotRect.height() / 2)
		point.ry() = m_chart->plotArea().top() - m_rect.height();
	else
		point.ry() = m_chart->plotArea().bottom();

	return point;
}

void Callout::setText(const QString &text)
{
    m_text = text;
    QFontMetrics metrics(m_font);
    m_textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
    m_textRect.translate(5, 5);
    prepareGeometryChange();
    m_rect = m_textRect.adjusted(-5, -5, 5, 5);
}

void Callout::setAnchor(QPointF point)
{
    m_anchor = point;
}

void Callout::updateGeometry()
{
	prepareGeometryChange();

	QPointF topLeftCorner = m_chart->mapToPosition(m_anchor);
	topLeftCorner.rx() -= m_rect.width() / 2;
	setPos(clampY(topLeftCorner));
}

// --------------------------------------------------------------------------------
// 	FUNCTION: anchor (public )
// --------------------------------------------------------------------------------
QPointF Callout::anchor() const
{
	return m_anchor;
}

// --------------------------------------------------------------------------------
// 	FUNCTION: setSeriesName (public )
// --------------------------------------------------------------------------------
void Callout::setSeriesName(QString name)
{
	m_seriesName = name;
}

// --------------------------------------------------------------------------------
// 	FUNCTION: seriesName (public )
// --------------------------------------------------------------------------------
QString Callout::seriesName() const
{
	return m_seriesName;
}