#include "temperatureModel.h"

#include <iostream>
#include <QDebug>

      temperatureModel::temperatureModel():
          mTpType(Rising)
      {
      }

      temperatureModel::~temperatureModel()
      {

      }

      void temperatureModel::init(const TiXmlElement *xmlEle)
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

          pTmpElem = xmlEle->FirstChildElement("initValue");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                  mCurValue = std::atof(point);
                  mCurValue /= unitConver;
              }
          }
          pTmpElem = xmlEle->FirstChildElement("changeStep");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                  mChangeStep = std::atof(point);
                  mChangeStep /= unitConver;
              }
          }

          pTmpElem = xmlEle->FirstChildElement("changeType");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                 std::string type = std::string(point);
                 if(type == RISING)
                     mTpType = Rising;
                 else if(type == FALLING)
                     mTpType = Falling;
                 else if(type == KEEP)
                     mTpType = Keep;
              }
          }

          if(mTpType == Rising)
          {
              mCurValue = mMinValue;
              mCurValue /= unitConver;
          }
          else if(mTpType == Falling)
          {
              mCurValue = mMaxValue;
              mCurValue /= unitConver;
          }
      }

      void temperatureModel::init(std::initializer_list<double> params)
      {
          auto pParam = params.begin();

          std::lock_guard<std::mutex> lk(mMutex);

          mMinValue = *pParam;
          mMinValue /= unitConver;
          pParam++;

          mMaxValue = *pParam;
          mMaxValue /= unitConver;
          pParam++;

          mCurValue = *pParam;
          mCurValue /= unitConver;
          pParam++;

          mChangeStep = *pParam;
          mChangeStep /= unitConver;
          pParam++;

          valueType tmp = *pParam;
          pParam++;

          mTpType = (tpType)tmp;

      }

      int temperatureModel::setAttribute(const std::string &attr, const void *value)
      {
          if(attr == "changeType")
          {
              valueType tmp = *((valueType*)value);
              setChangeType((tpType)tmp);
          }
          else if(attr == "initValue")
          {
              valueType tmp = *((valueType*)value);
              setInitValue(tmp);
          }
          else
          {

          }
          return 0;
      }

      std::string temperatureModel::getClassName()
      {
          return VAR_NAME(temperatureModel);
      }

      void temperatureModel::calculate()
      {
          if(mTpType == Rising)
          {
              std::lock_guard<std::mutex> lk(mMutex);

              mCurValue += mChangeStep;
              if(mCurValue > mMaxValue)
                  mCurValue = mMaxValue;
          }
          else if(mTpType == Falling)
          {
              std::lock_guard<std::mutex> lk(mMutex);

              mCurValue -= mChangeStep;
              if(mCurValue < mMinValue)
                  mCurValue = mMinValue;
          }
          else if(mTpType == Keep)
          {

          }
      }

      valueType temperatureModel::value()
      {          
          valueType tempV;
          calculate();
          tempV = mCurValue * unitConver;
          return tempV;
      }

      void temperatureModel::setInitValue(valueType v)
      {
          std::lock_guard<std::mutex> lk(mMutex);

          mCurValue = v;
          mCurValue /= unitConver;
      }

      void temperatureModel::setChangeType(tpType type)
      {
          std::lock_guard<std::mutex> lk(mMutex);

          mTpType = type;
      }

      extern "C"
      {
          Algorithm *LoadClass()
          {
              return new temperatureModel();
          }
      }

    
    
