#ifndef PARTPACKAGE_H
#define PARTPACKAGE_H

/**
  *帧拆包
  */

#include <QByteArray>
#include <QVector>

class partPackage
{
public:
    partPackage();
public:
    /**
     * @brief setHeadCode   设置帧头
     * @param head          帧头
     */
    void setHeadCode(const QByteArray &head){mHead = head;}

    /**
     * @brief setSrcPackage 设置原包数据
     * @param msg   数据
     */
    void setSrcPackage(const QByteArray &msg);

    /**
     * @brief getVaildPkSize    获取有效包数
     * @return  包数
     */
    int getVaildPkSize();

    /**
     * @brief getVaildMsg   获取有效数据
     * @param pkIndex   包索引
     * @return
     */
    QByteArray getVaildMsg(int pkIndex);
private:
    QByteArray mHead;
    QByteArray mSrcMsg;
    QVector<QByteArray> mVaildMsg;
};

#endif // PARTPACKAGE_H
