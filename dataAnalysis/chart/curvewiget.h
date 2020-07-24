#ifndef CURVEWIGET_H
#define CURVEWIGET_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>
#include "chartView.h"
#include "chart.h"
#include "lineSeries.h"
//#include "callout.h"
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
    void tooltip(QPointF point, bool state);
private:
    void initCurve();

    LineSeries *createSeries();
private:
    std::shared_ptr<resultTable> mResultTable;

    double mXMaxValue;
    double mYMaxValue;

    //Callout *mTooltip;

    QValueAxis *mAxisYData;
    QValueAxis *mAxisXData;
    //QChartView *mChartView;
    ChartView *mChartView;
    Chart *mChart;

    QMap<QString, LineSeries*> mLineSeries;
private:
    Ui::curveWiget *ui;
};

#endif // CURVEWIGET_H
