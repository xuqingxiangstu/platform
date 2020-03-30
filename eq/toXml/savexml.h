#ifndef SAVEXML_H
#define SAVEXML_H

#include <QThread>
#include <QString>
#include <QTreeWidget>

class saveXml : public QThread
{
    Q_OBJECT
public:
    saveXml(QObject *parent = 0);

    ~saveXml();

    /**
     * @brief setFilePath   设置文件路径
     * @param path  路径
     */
    void setFilePath(QString path){mCurPath = path;}

    /**
     * @brief setTreeWidget 设置待保存的树
     * @param widget    句柄
     */
    void setTreeWidget(QTreeWidget *widget);

    /**
     * @brief setSystemInfo 设置系统信息
     * @param item  系统item
     */
    void setSystemInfo(QTreeWidgetItem *item);

    /**
     * @brief startTask 启动任务
     */
    void startTask();
signals:
    /**
     * @brief saveOver  保存结束
     */
    void saveOver();

    /**
     * @brief saveProgress  保存进度
     * @param index 进度
     */
    void saveProgress(int index);
private:
    void run();
private:
    QString mCurPath;
    QTreeWidget *mCurTree;
    QTreeWidgetItem *mCurSystemItem;
};

#endif // SAVEXML_H
