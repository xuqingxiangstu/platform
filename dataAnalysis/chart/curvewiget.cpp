#include "curvewiget.h"
#include "ui_curvewiget.h"
#include <QGraphicsLayout>
#include <QDebug>

curveWiget::curveWiget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::curveWiget),
    mResultTable(nullptr)
{
    ui->setupUi(this);    

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
#if 0
    mChart = new QChart();

    mChart->layout()->setContentsMargins(0, 0, 0, 0);//设置外边界全部为0
    mChart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0
    mChart->setBackgroundRoundness(0);//设置背景区域无圆角

    mChart->setTheme(QChart::ChartThemeLight);//设置白色主题
    mChart->setDropShadowEnabled(true);//背景阴影
    mChart->setAutoFillBackground(true);  //设置背景自动填充

    mChart->legend()->setBackgroundVisible(true);//设置背景是否可视
    mChart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    mChart->setLocalizeNumbers(true);//数字是否本地化
    mChart->createDefaultAxes();//创建默认轴

    mAxisYData = new QValueAxis;
    mAxisXData = new QValueAxis;
    mChart->setTitle("Time-Value");
    mChart->createDefaultAxes();

    mChart->setAxisX(mAxisXData);
    mChart->setAxisY(mAxisYData);

    mChartView = new QChartView(mChart);
    mChartView->setRubberBand(QChartView::RectangleRubberBand);//矩形缩放
    mChartView->setRenderHint(QPainter::Antialiasing);//消除锯齿
    ui->verticalLayout->addWidget(mChartView);
#else
    mChart = new Chart();

    mChart->setTheme(QChart::ChartThemeLight);//设置白色主题
    mChart->setDropShadowEnabled(true);//背景阴影
    mChart->setAutoFillBackground(true);  //设置背景自动填充
    //mChart->setTitleBrush(QBrush(QColor(0,0,255)));//设置标题Brush
    //mChart->setTitleFont(QFont("微软雅黑"));//设置标题字体
    //mChart->setTitle("曲线图");

    mAxisYData = new QValueAxis;
    mAxisXData = new QValueAxis;

    mYMaxValue = 10;
    mXMaxValue = 10;

    mChart->setAxisX(mAxisXData);
    mChart->setAxisY(mAxisYData);

    //mChart->createDefaultAxes();

    //m_chart->createDefaultAxes();             //或者创建默认轴

    //修改说明样式
    mChart->legend()->setVisible(true);
    mChart->legend()->setAlignment(Qt::AlignTop);//底部对齐
    //mChart->legend()->setBackgroundVisible(true);//设置背景是否可视
    //mChart->legend()->setAutoFillBackground(true);//设置背景自动填充
    //mChart->legend()->setColor(QColor(222,233,251));//设置颜色
    //mChart->legend()->setLabelColor(QColor(0,100,255));//设置标签颜色
    //mChart->legend()->setMaximumHeight(50);
    //mChartView = new QChartView(mChart);
    mChartView = new ChartView(mChart);
    //mChartView->setGeometry(0, 0, 400, 400);
    mChartView->setRenderHint(QPainter::Antialiasing);
    mChartView->setRubberBand(QChartView::RectangleRubberBand);//矩形缩放
    ui->verticalLayout->addWidget(mChartView);


#endif
}

LineSeries *curveWiget::createSeries()
{
    LineSeries *series = new LineSeries();

    series->setVisible(true);
    //点标签是否可视
    series->setPointLabelsVisible(false);
    //点标签颜色
    series->setPointLabelsColor(QColor(0,0,0));
    //点标签字体
    series->setPointLabelsFont(QFont("微软雅黑"));
    //设置点标签显示格式
    series->setPointLabelsFormat("(@xPoint,@yPoint)");
    //是否切割边缘点标签，默认为true
    series->setPointLabelsClipping(false);
    //设置点标签是否可视
    series->setPointsVisible(true);
    //series->setUseOpenGL(true);

    //connect(series, &QSplineSeries::hovered, this, &curveWiget::tooltip);

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
            //QList<QPointF> points;

            for(int index = 0; index < values.size(); index++)
            {
                QString time = values.at(index).toObject().value(RESULT_TABLE_TIME).toString();

                QString parseValue = values.at(index).toObject().value(RESULT_TABLE_PARSE_VALUE).toString();

                double tmp = parseValue.toDouble();

                mXMaxValue < index ? mXMaxValue = index : mXMaxValue = mXMaxValue;
                mYMaxValue < tmp ? mYMaxValue = tmp : mYMaxValue = mYMaxValue;

                //points.append(QPointF(index, tmp));
                QPointF point = QPointF(index, tmp);
                qDebug() << point;
                mLineSeries[uuid]->append(point);
            }

            //mLineSeries[uuid]->append(points);

        }
    }

    mLineSeries[uuid]->setName(name);

    mAxisYData->setRange(0, mYMaxValue);
    mAxisXData->setRange(0, mXMaxValue);


    mChart->addSeries(mLineSeries[uuid]);
    //mChart->createDefaultAxes();
    mChart->setAxisX(mAxisXData, mLineSeries[uuid]);
    mChart->setAxisY(mAxisYData, mLineSeries[uuid]);    
}


void curveWiget::tooltip(QPointF point, bool state)
{
#if 0
    if(!mTooltip)
    {
        mTooltip = new Callout(mChart);
    }

    //qDebug() << point << state;

    if (state) {
        int a = point.x();

        QPointF curVal = mChart->mapToValue(point);
        qDebug() << curVal;
        //QString m_time = tableTime.at(a);
        int c = point.y();


        QString m_data = QString::number(c);
        QString str = "时间：\r\n数值：" + m_data;
        mTooltip->setText(str);
//        m_tooltip->setText(QString("时间: %1 \n数值: %2 ").arg(point.x()).arg(point.y()));
        mTooltip->setAnchor(point);
        mTooltip->setZValue(11);
        mTooltip->updateGeometry();
        mTooltip->show();
    } else {
        mTooltip->hide();
    }
#endif
}
