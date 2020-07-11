#ifndef FEARGEXTRACT_H
#define FEARGEXTRACT_H

#include "argextract.h"

/**
 * @brief The feArgExtract class    FE参数提取
 */
class feArgExtract : public argExtract
{
    Q_OBJECT
public:
    feArgExtract(QObject *parent = 0);
    ~feArgExtract();
signals:

public:
    void extract(const QString &uuid, const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result) override;
};

#endif // FEARGEXTRACT_H
