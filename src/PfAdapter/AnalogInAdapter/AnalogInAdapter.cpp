#include "AnalogInAdapter.h"

namespace Pf
{
  namespace PfAdapter
  {
      AnalogInAdapter::AnalogInAdapter()
      {
      }

      AnalogInAdapter::~AnalogInAdapter()
      {

      }

      void AnalogInAdapter::init(const TiXmlElement *ele)
      {

      }

      bool AnalogInAdapter::sendMsg(const char *msg, const int &msgSize)
      {
          return false;
      }

      bool AnalogInAdapter::receiveMsg(char *msg, int &rcvSize, const int &maxRcvSize, const unsigned int &timeOut)
      {
          return false;
      }

      std::string AnalogInAdapter::getClassName()
      {
          return VAR_NAME(AnalogInAdapter);
      }

      std::string AnalogInAdapter::getId()
      {
          return "";
      }
      int AnalogInAdapter::getAttribute(const std::string &attr, void *value)
      {
          return 0;
      }

      int AnalogInAdapter::setAttribute(const std::string &attr, const void *value)
      {
          return 0;
      }

      bool AnalogInAdapter::writeValue(const double *value, const int size)
      {
          return false;
      }

      bool AnalogInAdapter::readValue(double *value, int &rSzie)
      {
          return false;
      }

      extern "C"
      {
          Adapter *LoadClass()
          {
              return new AnalogInAdapter();
          }
      }
  }
}

    
    