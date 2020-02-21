#include "infowordconf.h"

#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        /***********帧头***************/

        frameHeadConf::frameHeadConf():
            mInfoLen(0),
            mInfoType(-1)
        {
            mInfoLen = 34;
            mInfoType = 0xFF;
        }

        std::shared_ptr<infoConf> frameHeadConf::clone()
        {
            infoConf *obj = new frameHeadConf();

            std::shared_ptr<infoConf> tmp(obj);

            return tmp;
        }


        bool frameHeadConf::getInfoLen(const unsigned char *msg, const int &msgSize, int &len)
        {
            bool res = false;

            len = mInfoLen;
            res = true;

            return res;
        }

        /***********信息字1***************/

        infoWord1Conf::infoWord1Conf():
            mInfoLen(0),
            mInfoType(-1)
        {
            mInfoLen = 12;
            mInfoType = 0x1;
        }

        std::shared_ptr<infoConf> infoWord1Conf::clone()
        {
            infoWord1Conf *obj = new infoWord1Conf();

            std::shared_ptr<infoConf> tmp(obj);

            return tmp;
        }

        void infoWord1Conf::init(QXlsx::Worksheet *sheet)
        {
            //TODO Excel配置
        }

        bool infoWord1Conf::getInfoLen(const unsigned char *msg, const int &msgSize, int &len)
        {
            bool res = false;

            len = mInfoLen;
            res = true;

            return res;
        }


        /***********信息字2***************/

        infoWord2Conf::infoWord2Conf():
            mInfoLen(0),
            mInfoType(-1)
        {
            mInfoLen = 12;
            mInfoType = 0x3;
        }

        std::shared_ptr<infoConf> infoWord2Conf::clone()
        {
            infoWord2Conf *obj = new infoWord2Conf();

            std::shared_ptr<infoConf> tmp(obj);

            return tmp;
        }

        void infoWord2Conf::init(QXlsx::Worksheet *sheet)
        {

        }

        bool infoWord2Conf::getInfoLen(const unsigned char *msg, const int &msgSize, int &len)
        {
            bool res = false;
            int typeIndex = 6;
            int lenIndex = 14;
            int lenByteSize = 1;

            dataStorage data;
            try
            {
                //先查看类型
                int type = data.getData(msg, msgSize, typeIndex, 1, 0, 0);
                if(type == 3)
                {
                    //获取帧长度值
                    mInfoLen = data.getData(msg, msgSize, lenIndex, lenByteSize, 0, 0);
                    //加上未计算长度
                    mInfoLen += lenIndex;
                    mInfoLen += lenByteSize;
                }
                else
                {
                    mInfoLen = 14;
                }

                len = mInfoLen;

                res = true;
            }
            catch(std::runtime_error err)
            {
#ifdef DEBUG_ICD
                UT_SHOW(err.what());
#endif
                res = false;
            }

            return res;
        }
    }
}
