#ifndef DATASHOWWIDGET_H
#define DATASHOWWIDGET_H

#include <QWidget>
#include <QMap>

#define TABLE_WIDGET    "表格显示"
#define S_IMG_WIDGET    "单图显示"
#define M_IMG_WIDGET    "全图显示"

namespace Ui {
class dataShowWidget;
}

class dataShowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dataShowWidget(QWidget *parent = 0);
    ~dataShowWidget();
public:
    void setDbName(const QString &dbname){mDbName = dbname;}
public slots:
    /**
     * @brief onShowTableWidget    显示表格窗体
     */
    void onShowTableWidget();

    /**
     * @brief onShowSingleImgWidget   显示单图
     */
    void onShowSingleImgWidget();

    /**
     * @brief onShowMultImgWidget 显示全图
     * @param uuid
     */
    void onShowMultImgWidget();
private:
    QString mDbName;
    QMap<QString, QWidget*> mTabWidgetManager;
private:
    Ui::dataShowWidget *ui;    
};

#endif // DATASHOWWIDGET_H
