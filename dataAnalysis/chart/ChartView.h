#pragma once

#include <QChartView>
#include <QMouseEvent>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
	Q_OBJECT

public:
    ChartView(QChart *chart, QWidget *parent = nullptr);
	~ChartView();
	// 保存坐标区域，用于复位
	void saveAxisRange();

    void setFilterLineSeries(QAbstractSeries *series){mShowSeries = series;}
signals:
    //鼠标放上触发
    void pointHovered(QAbstractSeries *series, QPointF point, bool state);
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    void findNearlyPoint(QPointF curPoint);
private:
    QAbstractSeries *mShowSeries;
    QColor mMousePointColor;
	QPoint m_lastPoint;
    QPoint m_MousePoint;
    bool m_isPress;
	bool m_alreadySaveRange;
	double m_xMin, m_xMax, m_yMin, m_yMax;
    QPoint  beginPoint; //选择矩形区的起点
    QPoint  endPoint;  //选择矩形区的终点

signals:
    void mouseMovePoint(QPoint point); //鼠标移动信号，在mouseMoveEvent()事件中触发
};
