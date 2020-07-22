#ifndef PROJECTTEMPLATE_H
#define PROJECTTEMPLATE_H

#include <QString>

/**
 * @brief The projectTemplate class 工程模板，创建新工程
 */

class projectTemplate
{
public:   
    /**
     * @brief create    创建工程
     * @param name      名称
     * @param path      路径
     */
    static void create(const QString &name, const QString &path, const QString &dbName);
};

#endif // PROJECTTEMPLATE_H
