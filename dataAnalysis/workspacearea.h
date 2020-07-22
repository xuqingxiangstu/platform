#ifndef WORKSPACEAREA_H
#define WORKSPACEAREA_H

#include <QWidget>

#include "datashowtable.h"

#include "datashowwidget.h"

namespace Ui {
class workspaceArea;
}

class workspaceArea : public QWidget
{
    Q_OBJECT

public:
    explicit workspaceArea(QWidget *parent = 0);
    ~workspaceArea();
public slots:
    /**
     * @brief onShowTableWidget    显示表格窗体
     * @param uuid          工程uuid
     */
    void onShowTableWidget(QString uuid, QString dbname);

    /**
     * @brief onShowSingleImgWidget   显示单图
     * @param uuid                  工程uuid
     */
    void onShowSingleImgWidget(QString uuid, QString dbname);

    /**
     * @brief onShowMultImgWidget 显示全图
     * @param uuid
     */
    void onShowMultImgWidget(QString uuid, QString dbname);
signals:
    void showMessage(QString msg, bool state);
private:
    void loadDataShowWidget(QString uuid, QString dbPath);
private:
    //dataShowTable *mDataShowTable;
    QMap<QString, QWidget*> mDataShowWidgetManager;
private:
    Ui::workspaceArea *ui;
};

#endif // WORKSPACEAREA_H
