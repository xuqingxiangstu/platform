#include "variableframe.h"

#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        variableFrame::variableFrame()
            :mFrameCfgPath(""),
              mDataRegionCfgPath("")
        {

        }

        variableFrame::~variableFrame()
        {

        }

        void variableFrame::init(const TiXmlElement *xmlEle)
        {
            const TiXmlElement *pTmpElem = NULL;

            pTmpElem = xmlEle->FirstChildElement("frameconfig");
            if (pTmpElem)
            {
                mFrameCfgPath = pTmpElem->GetText();
            }

            pTmpElem = xmlEle->FirstChildElement("dataRegionConfig");
            if (pTmpElem)
            {
                mDataRegionCfgPath = pTmpElem->GetText();
            }

            if( (mFrameCfgPath == "") || (mDataRegionCfgPath == ""))
            {
                UT_THROW_EXCEPTION( "frameconfig || dataRegionConfig == null " );
            }

            //初始化帧配置
            initFrameCfg(ROOT_PATH + mFrameCfgPath);

            //初始化子帧配置
            initSubFrameCfg(ROOT_PATH + mDataRegionCfgPath);
        }

        void variableFrame::simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode, const std::vector<icdInValueType> inValue)
        {

        }

        void variableFrame::parse(const unsigned char *inBuf, const unsigned int inSize, std::vector<icdOutSrcValueType> &outValue)
        {

        }

        void variableFrame::initFrameCfg(const std::string &path)
        {
            std::ostringstream strErr;

    #ifdef Utf8_Coding
            QXlsx::Document xlsx(QString::fromUtf8(path.c_str()));
    #else
            QXlsx::Document xlsx(QString::fromLocal8Bit(path.c_str()));
    #endif
            QXlsx::Workbook *workBook = xlsx.workbook();

            QStringList sheetNames = xlsx.sheetNames();

            if(sheetNames.size() == 0)
            {
                strErr.str("");
                strErr << "file path = (" << path << " ) sheet == 0";
                UT_THROW_EXCEPTION(strErr.str());
            }

            mProtocolCfg = std::make_shared<protocolConfigure>();
            mProtocolCfg->init(static_cast<QXlsx::Worksheet*>(workBook->sheet(0)));
        }

        void variableFrame::initSubFrameCfg(const std::string &path)
        {

        }

        extern "C"
        {
            frameObj *LoadClass()
            {
                return new variableFrame();
            }
        }

    }
}
