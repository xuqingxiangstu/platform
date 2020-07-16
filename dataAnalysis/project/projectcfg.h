#ifndef PROJECTCFG_H
#define PROJECTCFG_H

#include <QObject>

/**
 * @brief The projectCfg class  工程配置
 */

class projectCfg : public QObject
{
    Q_OBJECT
public:
    explicit projectCfg(QObject *parent = 0);
public:
    /**
     * @brief loadCfg   加载配置文件
     * @param path
     */
    void loadCfg(const QString &path);
signals:

public slots:
};

#endif // PROJECTCFG_H
