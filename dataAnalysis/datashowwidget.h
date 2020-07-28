#ifndef DATASHOWWIDGET_H
#define DATASHOWWIDGET_H

#include <QWidget>
#include <QMap>

#define TABLE_WIDGET    "表格显示"
#define S_IMG_WIDGET    "单图显示"
#define M_IMG_WIDGET    "全图显示"
#define FILE_WIDGET     "文件显示"

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
    void setProUuidAndPath(const QString &proUuid, const QString &proPath){mProUuid = proUuid;mProPath = proPath;}
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

    /**
     * @brief onShowFileWiget   显示文件内容
     * @param uuid              文件UUID
     * @param path              文件路径
     */
    void onShowFileWiget(const QString &uuid, const QString &path);
private: 
    QString mProUuid;
    QString mProPath;
    QMap<QString, QWidget*> mTabWidgetManager;
private:
    Ui::dataShowWidget *ui;    
};

#endif // DATASHOWWIDGET_H
