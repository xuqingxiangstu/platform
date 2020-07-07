#ifndef COPYPASTE_H
#define COPYPASTE_H

#include <QThread>
#include <memory>

#include <QTreeWidget>

#include "./dragTree/dragrole.h"

class copyPaste : public QThread
{
    Q_OBJECT
public:
    static std::shared_ptr<copyPaste> getInstance()
    {
        if(mInstance)
        {
            mInstance = std::make_shared<copyPaste>();
        }

        return mInstance;
    }
public:
    copyPaste(QObject *parent = nullptr);
    ~copyPaste();

    /**
     * @brief setCopy   设置拷贝内容
     * @param copyItem  item
     */
    void setCopy(const QTreeWidgetItem *&copyItem);

    /**
     * @brief isPaste   是否可以粘贴
     */
    bool isPaste();

    /**
     * @brief startPaste    启动粘贴
     */
    void startPaste();
signals:
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
    void subFlowItemValue(QString flowUuid, const std::shared_ptr<dragRole> &role);

    /**
     * @brief setCmdItemValue   设置指令ITEM值
     * @param subFlowUuid       子流程UUID
     * @param role          值
     */
    void cmdItemValue(QString subFlowUuid, const std::shared_ptr<dragRole> &role);

    /**
     * @brief setParamItemValue 设置参数ITEM值
     * @param subFlowUuid       子流程UUID
     * @param role          值
     * @param subRole       子值
     */
    void paramItemValue(QString subFlowUuid, const std::shared_ptr<dragRole> &role, const std::vector<std::shared_ptr<dragRole>> &subRole);
private:
    void run();
private:
    QTreeWidgetItem *mCopyItem;
    bool mIsFirst;
    static std::shared_ptr<copyPaste> mInstance;

};

#endif // COPYPASTE_H
