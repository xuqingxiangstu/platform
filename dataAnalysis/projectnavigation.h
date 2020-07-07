#ifndef PROJECTNAVIGATION_H
#define PROJECTNAVIGATION_H

#include <QWidget>
#include <QVector>
#include <QPair>

#include "../src/PfCommon/jsoncpp/json.h"

namespace Ui {
class projectNavigation;
}

class projectNavigation : public QWidget
{
    Q_OBJECT

public:
    explicit projectNavigation(QWidget *parent = 0);
    ~projectNavigation();
signals:
    /**
     * @brief analysis  数据分析
     * @param fileInfo  文件信息（多个文件:uuid+filepath）
     */
    //void analysis(QVector<QPair<QString, QString>> fileInfo);

    void toShowProperty(QString uuid, Json::Value);

public slots:
    /**
     * @brief analysisResult    数据分析结果
     * @param result            文件信息（多个文件:uuid+filepath）
     */
    //void analysisResult(QVector<QPair<QString, QString>> result);

private:
    QString mUiUuid;    //界面UUID
private:
    Ui::projectNavigation *ui;
};

#endif // PROJECTNAVIGATION_H
