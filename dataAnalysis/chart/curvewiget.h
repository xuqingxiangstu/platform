#ifndef CURVEWIGET_H
#define CURVEWIGET_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QAbstractSeries>
#include <QValueAxis>
#include <QMenu>
#include "ChartView.h"
#include "callout.h"
#include "../src/PfSql/paramsTable/parseResult/resulttable.h"

namespace Ui {
class curveWiget;
}

QT_CHARTS_USE_NAMESPACE

class curveWiget : public QWidget
{
    Q_OBJECT

public:
    explicit curveWiget(QWidget *parent = 0);
    ~curveWiget();
public:
    /**
     * @brief setDbInfo 设置数据库信息
     * @param dbPath    数据库路径
     * @param uuid      工程uuid
     */
    void setDbInfo(QString dbPath, QString uuid);
signals:
    void deleteCurve(QString uuid);
public slots:
    void onAddCurve(QString uuid, QString name);
    void tooltip(QAbstractSeries *series, QPointF point, bool state);
private:
    void initCurve();

    QLineSeries *createSeries();
private:
    std::shared_ptr<resultTable> mResultTable;

    double mXMaxValue;
    double mYMaxValue;

    QMap<QLineSeries*, int> mDefaultPenWidht;
    Callout *mTooltip;

    QValueAxis *mAxisYData;
    QValueAxis *mAxisXData;
    //QChartView *mChartView;
    ChartView *mChartView;
    QChart *mChart;

    QMap<QString, QLineSeries*> mLineSeries;
    QMenu *mSeriesPop;
    QMap<QLineSeries*, QVector<QString>> mPointsInfo;

    QLineSeries *mCurSelectSeries;
private:
    Ui::curveWiget *ui;
};

#endif // CURVEWIGET_H
