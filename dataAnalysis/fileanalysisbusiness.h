#ifndef FILEANALYSISBUSINESS_H
#define FILEANALYSISBUSINESS_H

#include <QObject>
#include <QVector>
#include <QPair>
#include "fileAnalysis/analysis.h"

/**
 * @brief The fileAnalysisBusiness class    文件分析业务
 */
class fileAnalysisBusiness : public QObject
{
    Q_OBJECT
public:
    fileAnalysisBusiness();
signals:
    /**
     * @brief showMessage   显示消息提示
     * @param msg           消息
     * @param state         消息状态
     * -true:正常
     * -false:异常
     */
    void showMessage(QString msg, bool state);

    /**
     * @brief toAnalysis  交由数据分析
     * @param uuid          uuid
     * @param file          文件路径
     */
    void toAnalysis(QString uuid, QString file, analysisRule rule);
public slots:
    /**
     * @brief analysis  数据分析
     * @param fileInfo  文件信息（多个文件:uuid+filepath）
     */
    void onAnalysis(QVector<QPair<QString, QString>> fileInfo);
};

#endif // FILEANALYSISBUSINESS_H
