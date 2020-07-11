#ifndef BEARGEXTRACT_H
#define BEARGEXTRACT_H


#include "argextract.h"

/**
 * @brief The beArgExtract class    BE参数提取
 */
class beArgExtract : public argExtract
{
    Q_OBJECT
public:
    beArgExtract(QObject *parent = 0);
    ~beArgExtract();
signals:

public:
    void extract(const QString &uuid, const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result) override;
};


#endif // BEARGEXTRACT_H
