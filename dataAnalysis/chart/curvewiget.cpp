#include "curvewiget.h"
#include "ui_curvewiget.h"
#include <QGraphicsLayout>
#include <QDebug>
#include <QScatterSeries>

curveWiget::curveWiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::curveWiget),
    mResultTable(nullptr),
    mTooltip(nullptr),
    mCurSelectSeries(nullptr)
{
    ui->setupUi(this);    

    mSeriesPop = new QMenu(this);
    mSeriesPop->addAction(ui->actionDelete);
    mSeriesPop->addAction(ui->actionFilterline);

    ui->actionFilterline->setCheckable(true);

    connect(ui->actionFilterline, &QAction::triggered, this, [=](){
        ui->actionFilterline->setCheckable(!ui->actionFilterline->isCheckable());

        mChartView->setFilterLineSeries(mCurSelectSeries);
    });

    connect(ui->actionDelete, &QAction::triggered, this, [=](){
        if(mCurSelectSeries)
        {
            mDefaultPenWidht.remove(mCurSelectSeries);
            mPointsInfo.remove(mCurSelectSeries);
            int index = mLineSeries.values().indexOf(mCurSelectSeries);

            emit deleteCurve(mLineSeries.keys().at(index));

            mLineSeries.remove(mLineSeries.keys().at(index));

            mChart->removeSeries(mCurSelectSeries);
            delete mCurSelectSeries;
            mCurSelectSeries = nullptr;
        }
    });

    initCurve();
}

curveWiget::~curveWiget()
{
    delete ui;
}

void curveWiget::setDbInfo(QString dbPath, QString uuid)
{
    mResultTable = std::make_shared<resultTable>(dbPath, uuid);
}

void curveWiget::initCurve()
{
    mChart = new QChart;
   // mChart->layout()->setContentsMargins(0,0,0,0);
   // mChart->setMargins(QMargins(0, 0, 0, 0));
    mChart->setTheme(QChart::ChartThemeQt);//设置白色主题
    mChart->setDropShadowEnabled(true);//背景阴影
    mChart->setAutoFillBackground(true);  //设置背景自动填充
    mAxisYData = new QValueAxis;
    mAxisXData = new QValueAxis;
    mAxisXData->setLabelFormat("%i");
    mAxisYData->setLabelFormat("%f");

//    axisX->setLabelFormat("%u");   //设置刻度的格式
//     axisX->setGridLineVisible(true);   //网格线可见
//     axisX->setTickCount(10);       //设置多少格
//     axisX->setMinorTickCount(1);   //设置每格小刻度线的数目

    //mChart->setTitle("Time-Value");
    mChart->createDefaultAxes();
//    m_chart->setAxisX(axisX_Time);
    mChart->setAxisX(mAxisXData);
    mChart->setAxisY(mAxisYData);
    mChart->legend()->setAlignment(Qt::AlignTop);
    mChartView = new ChartView(mChart);//使用自定义的chartview
    mChartView->setRenderHint(QPainter::Antialiasing);//消除锯齿
    ui->verticalLayout->addWidget(mChartView);

    connect(mChartView, &ChartView::pointHovered, this, &curveWiget::tooltip);
}

QLineSeries *curveWiget::createSeries()
{
    QLineSeries *series = new QLineSeries();

    connect(series, &QLineSeries::pressed, this, [=](const QPointF &point){
       //高亮
       if(QGuiApplication::mouseButtons() & Qt::LeftButton)
       {
            qDebug() << "left";
       }
       else if(QGuiApplication::mouseButtons() & Qt::RightButton)
       {
           mCurSelectSeries = series;
            //qDebug() << "right";
           mSeriesPop->exec(QCursor::pos());
       }
    });

    connect(series, &QLineSeries::hovered, this, [=](const QPointF &point, bool state){
       //高亮
        if(state)
        {
            QPen tmp = series->pen();
            tmp.setWidth(3);
            series->setPen(tmp);
        }
        else
        {
            QPen tmp = series->pen();
            tmp.setWidth(mDefaultPenWidht[series]);
            series->setPen(tmp);
        }
    });

#if 1
    series->setVisible(true);
    //点标签是否可视
    series->setPointLabelsVisible(false);
//    //点标签颜色
//    series->setPointLabelsColor(QColor(0,0,0));
//    //点标签字体
//    series->setPointLabelsFont(QFont("微软雅黑"));
//    //设置点标签显示格式
//    series->setPointLabelsFormat("(@xPoint,@yPoint)");
//    //是否切割边缘点标签，默认为true
//    series->setPointLabelsClipping(false);
//    //设置点标签是否可视
//    series->setPointsVisible(true);
//    //series->setUseOpenGL(true);

    //connect(series, &QSplineSeries::hovered, this, &curveWiget::tooltip1);
#endif

    mDefaultPenWidht[series] = series->pen().width();

    if(mCurSelectSeries == nullptr)
    {
        mCurSelectSeries = series;
        mChartView->setFilterLineSeries(mCurSelectSeries);
    }

    return series;
}

void curveWiget::onAddCurve(QString uuid, QString name)
{
    QString tableNum;
    QString codingNum;

    //获取表号编码值
    QStringList result = name.split("_");
    if(result.size() > 2)
    {
        tableNum = result.at(0);
        codingNum = result.at(1);
    }   

    mLineSeries[uuid] = createSeries();

    //数据获取值
    if(mResultTable)
    {
        QJsonArray values;

        if(mResultTable->getTimeByTableCoding(tableNum, codingNum, values))
        {
            QList<QPointF> points;
            QVector<QString> info;

            for(int index = 0; index < values.size(); index++)
            {
                QString time = values.at(index).toObject().value(RESULT_TABLE_TIME).toString();

                QString parseValue = values.at(index).toObject().value(RESULT_TABLE_PARSE_VALUE).toString();
                QString hexValue = values.at(index).toObject().value(RESULT_TABLE_HEX_VALUE).toString();
                QString isOver = values.at(index).toObject().value(RESULT_TABLE_IS_OVER).toString();
                double tmp = parseValue.toDouble();

                mXMaxValue < index ? mXMaxValue = index : mXMaxValue = mXMaxValue;
                mYMaxValue < tmp ? mYMaxValue = tmp : mYMaxValue = mYMaxValue;

                points.append(QPointF(index, tmp));

                QString msg;
                msg += "时间:";
                msg += time;
                msg += "\n";
                msg += "源码:";
                msg += hexValue;
                msg += "\n";
                msg += "工程值:";
                msg += parseValue;

                info.append(msg);
            }

            mPointsInfo[mLineSeries[uuid]] = info;
            mLineSeries[uuid]->append(points);
        }
    }

    mLineSeries[uuid]->setName(name);

    mAxisYData->setRange(0, mYMaxValue * 1.3);
    mAxisXData->setRange(0, mXMaxValue * 1.3);

    mChart->addSeries(mLineSeries[uuid]);

    mChart->setAxisX(mAxisXData, mLineSeries[uuid]);    

    mChart->setAxisY(mAxisYData, mLineSeries[uuid]);
}


void curveWiget::tooltip(QAbstractSeries *series, QPointF point, bool state)
{    
    if(!mTooltip)
    {
        mTooltip = new Callout(mChart);
    }   

    if(state && series)
    {
        int index = point.toPoint().x();

        QString str = mPointsInfo[dynamic_cast<QLineSeries*>(series)].at(index);
        mTooltip->setText(str);
    //        m_tooltip->setText(QString("时间: %1 \n数值: %2 ").arg(point.x()).arg(point.y()));
        mTooltip->setAnchor(point);
        mTooltip->setZValue(11);
        mTooltip->updateGeometry();
        mTooltip->show();
    }
    else
    {
        mTooltip->hide();
    }
}
