#include "randamodel.h"

RandaModel::RandaModel():
    randm(nullptr)
{
}

RandaModel::~RandaModel()
{

}
void RandaModel::init(const TiXmlElement *xmlEle)
{
    std::lock_guard<std::mutex> lk(mMutex);

    const TiXmlElement *pTmpElem = NULL;

    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("min");
    if(pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
        {
            mMinValue = std::atof(point);
            mMinValue /= unitConver;
        }
    }
    pTmpElem = xmlEle->FirstChildElement("max");
    if(pTmpElem)
    {
        point = pTmpElem->GetText();
        if(point)
        {
            mMaxValue = std::atof(point);
            mMaxValue /= unitConver;
        }
    }
}

void RandaModel::init(std::initializer_list<double> params)
{
    auto pParam = params.begin();

    std::lock_guard<std::mutex> lk(mMutex);

    mMinValue = *pParam;
    mMinValue /= unitConver;
    pParam++;

    mMaxValue = *pParam;
    mMaxValue /= unitConver;
    pParam++;
}

valueType RandaModel::value()
{
    calculate();

    return mCurValue;
}

int RandaModel::setAttribute(const std::string &attr, const void *value)
{
    return 0;
}

void RandaModel::calculate()
{
    if (mMaxValue == mMinValue)
    {
        mCurValue = mMinValue;
    }
    else
    {
        double tmpMin = mMinValue * mDecimal;
        double tmpMax = mMaxValue * mDecimal;

        mMutex.lock();
        mCurValue = rand() % ((long long)(tmpMax - tmpMin + 1)) + tmpMin;
        mMutex.unlock();
        mCurValue = mCurValue/mDecimal*unitConver;
    }
}


std::string RandaModel::getClassName()
{
    return VAR_NAME(RandaModel);
}


extern "C"
{
    Algorithm *LoadClass()
    {
        return new RandaModel();
    }
}
