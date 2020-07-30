#ifndef DSFILE_H
#define DSFILE_H

#include <QWidget>
#include <QStandardItemModel>

#include "textedit.h"

namespace Ui {
class dsFile;
}

class dsFile : public QWidget
{
    Q_OBJECT

public:
    explicit dsFile(QWidget *parent = 0);
    ~dsFile();
public:
    /**
     * @brief updateContext 更新内容
     * @param filePath      文件路径
     */
    void updateContext(const QString &filePath);
public slots:
    /**
     * @brief appendContext 添加数据
     * @param context
     */
    void appendContext(QByteArray context);
private:
    QStandardItemModel *mDataModel;
    TextEdit *plainTextEdit;
private:
    Ui::dsFile *ui;
};

#endif // DSFILE_H
