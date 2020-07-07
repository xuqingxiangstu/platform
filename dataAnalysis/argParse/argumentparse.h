#ifndef ARGUMENTPARSE_H
#define ARGUMENTPARSE_H

#include <QObject>
#include <memory>
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

/**
 * @brief The argumentParse class   参数解析类
 */

class argumentParse : public QObject
{
    Q_OBJECT
public:
    explicit argumentParse(QObject *parent = 0);
public:
    /**
     * @brief init  初始化参数解析相关类
     */
    void init();
signals:

public slots:
private:
    std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> mIcdFrameAdpter; ///<组帧、解帧协议适配对象
};

#endif // ARGUMENTPARSE_H
