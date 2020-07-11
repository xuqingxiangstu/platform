#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QObject>

#include <QVector>
#include <QPair>
#include <memory>
#include "analysisrule.h"
#include "../argParse/argumentparse.h"
#include "filter.h"

/**
 * @brief The analysis class    文件分析
 */
class analysis : public QObject
{
    Q_OBJECT
public:
    explicit analysis(QObject *parent = 0);

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
     * @brief analysisOver  解析结束
     */
    void analysisOver();

    /**
     * @brief toParse   解析
     * @param frameType 帧类型
     * @param validMsg  有效数据
     */
    void toParse(QString uuid, Json::Value param, QByteArray validMsg);

    /**
     * @brief toDataBase    存储数据库
     * @param value         数据
     */
    void toDataBase(QJsonObject value);
public slots:
    /**
     * @brief onAnalysis    文件分析
     * @param uuid          uuid
     * @param file          文件路径
     * @param filter        过滤器
     * @param rule          解析规则
     */
    void onAnalysis(QString uuid, QString file, std::shared_ptr<analysisRule> rule, std::shared_ptr<filterManager> filterManager);
private:
    QString mCurUuid;
    QString mCurFile;
    std::shared_ptr<argumentParse> mArgParse;   ///< 参数解析
};

#endif // ANALYSIS_H
