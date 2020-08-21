#ifndef CREATERESULTDB_H
#define CREATERESULTDB_H

#include "../paramstable_global.h"

#include <QMutex>
#include <memory>
#include <QString>

/**
 * @brief The createResultDb class  创建解析结果db文件
 */

class PARAMSTABLESHARED_EXPORT createResultDb
{
public:
    static std::shared_ptr<createResultDb> getInstance(){
        if(mInstance == nullptr)
        {
            mInstanceMutex.lock();
            if(mInstance == nullptr)
            {
                mInstance = std::make_shared<createResultDb>();
            }
            mInstanceMutex.unlock();
        }
        return mInstance;
    }
public:
    createResultDb();
    ~createResultDb();
public:
    /**
     * @brief create    创建新数据库
     * @param uuid
     */
    QString create(const QString &uuid);
private:
    const QString mTemplatePath = "./parseResult/template.db";
    static QMutex mInstanceMutex;
    static std::shared_ptr<createResultDb> mInstance;
};

#endif // CREATERESULTDB_H
