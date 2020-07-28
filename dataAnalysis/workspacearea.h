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
    void onShowTableWidget(QString uuid, QString proPath);

    /**
     * @brief onShowSingleImgWidget   显示单图
     * @param uuid                  工程uuid
     */
    void onShowSingleImgWidget(QString uuid, QString proPath);

    /**
     * @brief onShowMultImgWidget 显示全图
     * @param uuid
     */
    void onShowMultImgWidget(QString uuid, QString proPath);

    /**
     * @brief onShowFileWidget  显示文件窗体
     * @param proUuid           工程UUID
     * @param fileUuid          文件UUID
     * @param filePath          文件路径
     */
    void onShowFileWidget(QString proUuid, QString fileUuid, QString proPath, QString filePath);

    /**
     * @brief onCloseProject    关闭工程
     * @param uuid
     */
    void onCloseProject(QString uuid);
signals:
    void showMessage(QString msg, bool state);
private:
    void loadDataShowWidget(QString uuid, QString proPath);
private:
    //dataShowTable *mDataShowTable;
    QMap<QString, QWidget*> mDataShowWidgetManager;
private:
    Ui::workspaceArea *ui;
};

#endif // WORKSPACEAREA_H
