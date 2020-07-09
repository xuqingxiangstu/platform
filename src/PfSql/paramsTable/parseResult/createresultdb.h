#ifndef CREATERESULTDB_H
#define CREATERESULTDB_H

#include <QMutex>
#include <memory>
#include <QString>

/**
 * @brief The createResultDb class  创建解析结果db文件
 */

class createResultDb
{
public:
    static std::shared_ptr<createResultDb> getInstance(){
        if(mInstance == nullptr)
        {
            mInstanceMutex.lock();
            if(mInstance == nullptr)
            {
                createResultDb *obj = new createResultDb();

                mInstance = std::shared_ptr<createResultDb>(obj);
            }
            mInstanceMutex.unlock();
        }
        return mInstance;
    }
private:
    createResultDb();
public:
    ~createResultDb();
public:
    /**
     * @brief create    创建新数据库
     * @param uuid
     */
    void create(const QString &uuid);
private:
    const QString mTemplatePath = "./parseResult/template.db";
    static QMutex mInstanceMutex;
    static std::shared_ptr<createResultDb> mInstance;
};

#endif // CREATERESULTDB_H
