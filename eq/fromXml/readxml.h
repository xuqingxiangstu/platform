#ifndef READXML_H
#define READXML_H

#include <QThread>
#include <QString>

#include "../src/PfCommon/TinyXml/tinyxml.h"

#include "../dragTree/dragrole.h"

class readXml : public QThread
{
    Q_OBJECT
public:
    readXml(QObject *parent = 0);

    ~readXml();

    /**
     * @brief setFilePath   设置文件路径
     * @param path  路径
     */
    void setFilePath(QString path);

    /**
     * @brief startTask 启动任务
     */
    void startTask();
signals:
    /**
     * @brief saveOver  read结束
     */
    void readOver();

    /**
     * @brief saveProgress  保存进度
     * @param index 进度
     */
    void readProgress(int index);

    /**
     * @brief itemValue 树节点值
     * @param role
     */
    void itemValue(std::shared_ptr<dragRole> role);

    /**
     * @brief errInfo   异常信息
     * @param info  信息
     */
    void errInfo(QString info);

    /**
     * @brief setFlowItemValue  设置流程ITEM值
     * @param role  值
     */
    void flowItemValue(std::shared_ptr<dragRole> role);

    /**
     * @brief setSubFlowItemValue   设置子流程ITEM值
     * @param flowUuid  流程UUID
     * @param role  值
     */
    void subFlowItemValue(QString flowUuid, std::shared_ptr<dragRole> role);

    /**
     * @brief setCmdItemValue   设置指令ITEM值
     * @param subFlowUuid       子流程UUID
     * @param role          值
     */
    void cmdItemValue(QString subFlowUuid, std::shared_ptr<dragRole> role);

    /**
     * @brief setParamItemValue 设置参数ITEM值
     * @param subFlowUuid       子流程UUID
     * @param role          值
     * @param subRole       子值
     */
    void paramItemValue(QString subFlowUuid, std::shared_ptr<dragRole> role, std::vector<std::shared_ptr<dragRole>> subRole);
private:
    void readFlow(TiXmlElement *ele);
    void readSubFlow(std::string subFlowUuid, TiXmlElement *ele);
private:
    void run();
private:
    QString mCurPath;
};

//Q_DECLARE_METATYPE(std::vector<std::shared_ptr<dragRole>>)

#endif // READXML_H
