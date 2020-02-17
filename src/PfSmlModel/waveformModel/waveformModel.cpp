#include "waveformModel.h"

      waveformModel::waveformModel()
      {
      }

      waveformModel::~waveformModel()
      {

      }

      void waveformModel::init(const TiXmlElement *xmlEle)
      {
          std::lock_guard<std::mutex> lk(mMutex);

          const TiXmlElement *pTmpElem = NULL;

          const char *point = nullptr;

          pTmpElem = xmlEle->FirstChildElement("a");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                  a = std::atof(point);
              }
          }
          pTmpElem = xmlEle->FirstChildElement("w");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                  w = std::atof(point);
              }
          }
          pTmpElem = xmlEle->FirstChildElement("f");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                  f = std::atof(point);
              }
          }
          pTmpElem = xmlEle->FirstChildElement("y");
          if(pTmpElem)
          {
              point = pTmpElem->GetText();
              if(point)
              {
                  y = std::atof(point);
              }
          }
      }

      void waveformModel::init(std::initializer_list<double> params)
      {

          auto pParam = params.begin();

          std::lock_guard<std::mutex> lk(mMutex);

          a = *pParam;
          pParam++;

          w = *pParam;
          pParam++;

          f = *pParam;
          pParam++;

          y = *pParam;
      }

      int waveformModel::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      std::string waveformModel::getClassName()
      {
          return VAR_NAME(waveformModel);
      }

      void waveformModel::calculate()
      {
          mMutex.lock();
          mCurValue = a * sin(2 * 3.14 * w * mXValue + f) + y;
          mMutex.unlock();

          mXValue += 0.1;
      }

      valueType waveformModel::value()
      {
          calculate();

          return mCurValue;
      }


      extern "C"
      {
          Algorithm *LoadClass()
          {
              return new waveformModel();
          }
      }

    
    
