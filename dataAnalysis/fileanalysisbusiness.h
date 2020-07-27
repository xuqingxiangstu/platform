#ifndef FILEANALYSISBUSINESS_H
#define FILEANALYSISBUSINESS_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <atomic>
#include "fileAnalysis/analysis.h"
#include <tuple>
#include "fileAnalysis/filter.h"
#include "./argSave/saveDataBaseTask.h"
#include "fileAnalysis/analysisprogress.h"

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
    void showMessage(QString msg, bool state = true);

    /**
     * @brief toAnalysis  交由数据分析
     * @param uuid          uuid
     * @param file          文件路径
     */
    void toAnalysis(QString uuid, QString file, std::shared_ptr<analysisRule> rule, std::shared_ptr<filterManager> filter);

    /**
     * @brief curAnalysisStep   当前解析步骤
     */
    void curAnalysisStep(int);

    void allAnalysisOver();
public slots:
    /**
     * @brief analysis  数据分析
     * @param proUuid   工程UUID
     * @param fileInfo  文件信息（多个文件:uuid+filepath）
     */
    void onAnalysis(const QString &proUuid, const QString &dbPath, QVector<std::tuple<QString, std::shared_ptr<analysisRule>, std::shared_ptr<filterManager>>> fileInfo);

    /**
     * @brief onAnalysisOver    解析结束
     * @param uuid
     */
    void onAnalysisOver(QString uuid);

    /**
     * @brief onAnalysisStep    当前解析步骤
     * @param step
     */
    void onAnalysisStep(int step);
private:
    /**
     * @brief getAnalysisMaxSize    获取待解析总个数
     * @return
     */
    int getAnalysisMaxSize(const QVector<std::tuple<QString, std::shared_ptr<analysisRule>, std::shared_ptr<filterManager>>> &fileInfo);
private:

    std::shared_ptr<analysis> mAnalysisObj; ///<数据解析实体
    std::atomic_int mCurAnalysisStep;       ///< 当前解析步骤
    QStringList mAnalysisUuids;             ///<当前解析的uuid
    std::shared_ptr<saveDataBaseTask> mSaveDbTask;
};

#endif // FILEANALYSISBUSINESS_H
