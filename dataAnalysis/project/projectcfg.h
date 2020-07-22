#ifndef PROJECTCFG_H
#define PROJECTCFG_H

#include <QObject>

#define PRJ_VERSION "V1.0.0"

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
     * @brief loadCfg   加载工程
     * @param path      工程路径
     */
    void load(const QString &path, const QString &uuid = "");

    /**
     * @brief save  保存工程
     */
    void save();

    /**
     * @brief copy  将源文件拷贝到工程目录下
     * @param srcFiles  源文件
     * @return 拷贝后的结果
     */
    QStringList copy(const QStringList &srcFiles);

    QString getUuid(){return mUuid;}

    /**
     * @brief getDbPath 获取数据库路径
     * @return
     */
    QString getDbPath(){return mDbPath;}

    /**
     * @brief setDbPath 设置数据库路径
     * @param path      路径
     */
    void setDbPath(const QString &path){mDbPath = path;}

    /**
     * @brief getDataFiles
     * @return
     */
    QStringList getDataFiles(){return mDataFiles;}

    /**
     * @brief setDataFiles  设置文件路径
     * @param files         文件路径
     */
    void setDataFiles(const QStringList &files){mDataFiles = files;}

    /**
     * @brief getPrjName    获取工程名称
     * @return  名称
     */
    QString getPrjName(){return mPrjName;}

    /**
     * @brief setPrjName    设置工程名称
     * @param name          名称
     */
    void setPrjName(const QString &name){mPrjName = name;}

    /**
     * @brief getPrjVersion 获取工程版本号
     * @return  版本号
     */
    QString getPrjVersion(){return mPrjVersion;}

    /**
     * @brief setPrjVersion 设置工程版本号
     * @param ver   版本号
     */
    void setPrjVersion(const QString &ver){mPrjVersion = ver;}
private:
    QString mPrjPath;       ///< 工程路径
    QString mPrjFile;       ///< 工程文件
    QString mDbPath;        ///<
    QString mProperty;      ///< 属性
    QString mPrjName;
    QString mUuid;
    QString mPrjVersion;
    QStringList mDataFiles;
};

#endif // PROJECTCFG_H
