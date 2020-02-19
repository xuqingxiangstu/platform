#include "infowordregion.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../common/type.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        void infoWordRegionManager::init(QXlsx::Workbook *workBook)
        {
            std::ostringstream strErr;
            QVariant tmp;
            QString value;
            bool Ok;

            if(workBook == nullptr)
            {
                strErr.str("");
                strErr << "workBook == nullptr";

                UT_THROW_EXCEPTION(strErr.str());
            }

            if(workBook->sheetCount() == 0)
            {
                strErr.str("");
                strErr << "sheet == 0";

                UT_THROW_EXCEPTION(strErr.str());
            }

            if(workBook->sheetCount() == 1)
            {
                strErr.str("");
                strErr << "sheet == 1, no sub frame sheet";

                UT_THROW_EXCEPTION(strErr.str());
            }

            for(int index = 0; index < workBook->sheetCount(); index++)
            {
                QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(index));

                tmp = workSheet->read(1, 2);
                value = tmp.toString();
                int code = value.toInt(&Ok, 16);

                auto itor = mManager.find(code);
                if(itor != mManager.end())
                {
                    strErr.str("");
                    strErr << "exist code(" << std::hex << code << ")";

                    UT_THROW_EXCEPTION(strErr.str());
                }

                auto obj = std::make_shared<infoWordRegion>();
                obj->init(workSheet);

                mManager[code] = obj;
            }
        }

        std::shared_ptr<infoWordRegion> infoWordRegionManager::getRegion(const unsigned int &code)
        {
            auto itor = mManager.find(code);
            if(itor != mManager.end())
            {
                return (itor->second)->clone();
            }

            return nullptr;
        }

        std::shared_ptr<infoWordRegionManager> infoWordRegionManager::clone()
        {
            infoWordRegionManager *obj = new infoWordRegionManager();

            for(auto itor = mManager.begin() ; itor != mManager.end(); itor++)
            {
                obj->mManager[itor->first] = (itor->second)->clone();
            }

            return std::shared_ptr<infoWordRegionManager>(obj);
        }

        infoWordRegion::infoWordRegion()
        {

        }

        void infoWordRegion::init(QXlsx::Worksheet *workSheet)
        {
            int i32Rows = workSheet->dimension().lastRow();
            QVariant tmp;
            QString value;
            bool Ok;
            for(int rowIndex = 3; rowIndex <= i32Rows; rowIndex++)
            {
                int columnIndex = 1;
                std::string pName = "";
                int byteStart = 0, byteSize = 0;
                int bitStart = 0, bitSize = 0;
                std::string bigSmal;
                double meanMin = 0, meanMax = 0;
                std::string pCategory = "";
                unsigned int initData = 0;

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
#ifdef Utf8_Coding
                pName = value.toUtf8().data();
#else
                pName = value.toLocal8Bit().data();
#endif

                // 遇到空内容则停止
                if( pName == "")
                {
                    break;
                }

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
                byteStart = value.toInt(&Ok, 10);

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
                byteSize = value.toInt(&Ok, 10);

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
                bitStart = value.toInt(&Ok, 10);

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
                bitSize = value.toInt(&Ok, 10);

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();

                if(value.compare(QStringLiteral("是")) == 0)
                {
                    bigSmal = BIGENDIAN;
                }
                else
                {
                    bigSmal = SAMLLENDIAN;
                }

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
                if( value.contains("0x") || value.contains("0X"))
                {
                    initData = value.toInt(&Ok, 16);
                }
                else
                {
                    initData = value.toInt(&Ok, 10);
                }

                tmp = workSheet->read(rowIndex, columnIndex++);
                value = tmp.toString();
    #ifdef Utf8_Coding
                pCategory = value.toUtf8().data();
    #else
                pCategory = value.toLocal8Bit().data();
    #endif
                mStorages.push_back(std::make_shared<storageType>( pName, byteStart, byteSize, bitStart, bitSize, bigSmal,
                                                                   initData, pCategory));

            }
        }

        std::shared_ptr<infoWordRegion> infoWordRegion::clone()
        {

        }
    }
}
