#include "datastorage.h"
#include "datatype.h"

#include <bitset>
#include <sstream>
#include <QDebug>
#include <atomic>

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataStorage::dataStorage()
        {

        }

        unDataConvert dataStorage::getAutoData(const unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const std::string &dataType)
        {
            std::atomic_int i32Pos(0);
            int i32BitStartPos = bitStartPos;
            int i32BitStopPos = i32BitStartPos + bitSize;
            unsigned __int64 TmpData = 0;
            int i32ByteStartPos = byteStartPos;
            int i32ByteStopPos = i32ByteStartPos + byteSize;
            unDataConvert mData;
            mData.u64Value = 0;

            std::ostringstream strErr;

            if( (i32ByteStartPos < 0) || (i32ByteStopPos < 0))
            {
                strErr << "起始字节终止位异常 i32ByteStartPos(" << i32ByteStartPos << "), i32ByteStopPos(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (i32BitStartPos < 0) || (i32BitStopPos < 0))
            {
                strErr << "起始位终止位异常 i32BitStartPos(" << i32BitStartPos << "), i32BitStopPos(" << i32BitStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (bitSize > 64) || (bitSize < 0))
            {
                strErr << "起始位终止位差值异常 bitsize = (" << bitSize << ") => (0 < bitsize < 64)";
                throw std::runtime_error(strErr.str());
            }

            //校验
            if (i32ByteStopPos > u32Size)
            {
                strErr << " 总长度(" << std::dec << u32Size << ") < 待提取长度(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }            

            /*
            step1:先取整字节
            step1:再按位取
            */

            //先接收高字节，再接收低字节
            if (dataType == BIGENDIAN)
            {
                for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                {
                    mData.i8Buf[i32Pos++] = u8Data[i];
                }
            }
            else if (dataType == SAMLLENDIAN)
            {
                i32Pos = 0;
                for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                {
                    mData.u8Buf[i32Pos++] = u8Data[i];
                }
            }

            return mData;
        }

        unsigned __int64 dataStorage::getData(const unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize,const std::string &dataType)
        {
            int i32Pos = 0;
            int i32BitStartPos = bitStartPos;
            int i32BitStopPos = i32BitStartPos + bitSize;
            unsigned __int64 TmpData = 0;
            int i32ByteStartPos = byteStartPos;
            int i32ByteStopPos = i32ByteStartPos + byteSize;
            unDataConvert mData;
            mData.u64Value = 0;
            std::ostringstream strErr;

            if( (i32ByteStartPos < 0) || (i32ByteStopPos < 0))
            {
                strErr << "起始字节终止位异常 i32ByteStartPos(" << i32ByteStartPos << "), i32ByteStopPos(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (i32BitStartPos < 0) || (i32BitStopPos < 0))
            {
                strErr << "起始位终止位异常 i32BitStartPos(" << i32BitStartPos << "), i32BitStopPos(" << i32BitStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (bitSize > 64) || (bitSize < 0))
            {
                strErr << "起始位终止位差值异常 bitsize = (" << bitSize << ") => (0 < bitsize < 64)";
                throw std::runtime_error(strErr.str());
            }

            //校验
            if (i32ByteStopPos > u32Size)
            {
                strErr << " 总长度(" << std::dec << u32Size << ") < 待提取长度(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            /*
            step1:先取整字节
            step1:再按位取
            */

            //先接收高字节，再接收低字节
            if (dataType == BIGENDIAN)
            {
                for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                {
                    mData.i8Buf[i32Pos++] = u8Data[i];
                }
            }
            else if (dataType == SAMLLENDIAN)
            {
                for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                {
                    mData.i8Buf[i32Pos++] = u8Data[i];
                }
            }

            if (0 == bitSize)
            {
                TmpData = mData.u64Value;
            }
            else
            {
                std::bitset<64> BitMask;

                for (int j = i32BitStartPos; j < i32BitStopPos; j++)
                {
                    BitMask.set(j);
                }

                TmpData = mData.u64Value & (BitMask.to_ulong());

                TmpData = TmpData >> i32BitStartPos;
            }

            return TmpData;
        }


        void dataStorage::setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const double &value, const std::string &dataType)
        {
            int i32Pos = 0;
            int i32BitStartPos = bitStartPos;
            int i32BitStopPos = i32BitStartPos + bitSize;
            int i32ByteStartPos = byteStartPos;
            int i32ByteStopPos = i32ByteStartPos + byteSize;
            std::ostringstream strErr;
            unDataConvert mData;
            mData.d64Value = 0;

            if( (i32ByteStartPos < 0) || (i32ByteStopPos < 0))
            {
                strErr << "起始字节终止位异常 i32ByteStartPos(" << i32ByteStartPos << "), i32ByteStopPos(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (i32BitStartPos < 0) || (i32BitStopPos < 0))
            {
                strErr << "起始位终止位异常 i32BitStartPos(" << i32BitStartPos << "), i32BitStopPos(" << i32BitStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (bitSize > 64) || (bitSize < 0))
            {
                strErr << "起始位终止位差值异常 bitsize = (" << bitSize << ") => (0 < bitsize < 64)";
                throw std::runtime_error(strErr.str());
            }

            //校验
            if (i32ByteStopPos > u32Size)
            {
                strErr << " 总长度(" << std::dec << u32Size << ") < 待提取长度(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if (bitSize == 0)
            {
                if(byteSize == 8)
                {
                    mData.d64Value = value;
                }
                else
                {
                    mData.f32Value = value;
                }

                i32Pos = 0;
                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
            }
        }
        void dataStorage::setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const float &value, const std::string &dataType)
        {
            int i32Pos = 0;
            int i32BitStartPos = bitStartPos;
            int i32BitStopPos = i32BitStartPos + bitSize;
            int i32ByteStartPos = byteStartPos;
            int i32ByteStopPos = i32ByteStartPos + byteSize;
            std::ostringstream strErr;
            unDataConvert mData;
            mData.d64Value = 0;
            std::string convertDataType = dataType;

            //modify xqx 2020 0424 用户没有填充此字段时按照默认小端处理
            if("" == convertDataType)
            {
                convertDataType = SAMLLENDIAN;
            }
            //end

            if( (i32ByteStartPos < 0) || (i32ByteStopPos < 0))
            {
                strErr << "起始字节终止位异常 i32ByteStartPos(" << i32ByteStartPos << "), i32ByteStopPos(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (i32BitStartPos < 0) || (i32BitStopPos < 0))
            {
                strErr << "起始位终止位异常 i32BitStartPos(" << i32BitStartPos << "), i32BitStopPos(" << i32BitStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (bitSize > 64) || (bitSize < 0))
            {
                strErr << "起始位终止位差值异常 bitsize = (" << bitSize << ") => (0 < bitsize < 64)";
                throw std::runtime_error(strErr.str());
            }

            //校验
            if (i32ByteStopPos > u32Size)
            {
                strErr << " 总长度(" << std::dec << u32Size << ") < 待提取长度(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if (bitSize == 0)
            {
                mData.f32Value = value;

                i32Pos = 0;
                if (convertDataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
                else if (convertDataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
            }
        }

        void dataStorage::setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const unsigned int &value, const std::string &dataType)
        {
            int i32Pos = 0;
            int i32BitStartPos = bitStartPos;
            int i32BitStopPos = i32BitStartPos + bitSize;
            int i32ByteStartPos = byteStartPos;
            int i32ByteStopPos = i32ByteStartPos + byteSize;
            std::ostringstream strErr;
            unDataConvert mData;
            mData.i64Value = 0;

            if( (i32ByteStartPos < 0) || (i32ByteStopPos < 0))
            {
                strErr << "起始字节终止位异常 i32ByteStartPos(" << i32ByteStartPos << "), i32ByteStopPos(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (i32BitStartPos < 0) || (i32BitStopPos < 0))
            {
                strErr << "起始位终止位异常 i32BitStartPos(" << i32BitStartPos << "), i32BitStopPos(" << i32BitStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (bitSize > 64) || (bitSize < 0))
            {
                strErr << "起始位终止位差值异常 bitsize = (" << bitSize << ") => (0 < bitsize < 64)";
                throw std::runtime_error(strErr.str());
            }

            //校验
            if (i32ByteStopPos > u32Size)
            {
                strErr << " 总长度(" << std::dec << u32Size << ") < 待提取长度(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if (bitSize == 0)
            {
                mData.i64Value = value;

                i32Pos = 0;
                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
            }
            else
            {
                i32Pos = 0;
                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        mData.u8Buf[i32Pos++] = u8Data[i];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        mData.u8Buf[i32Pos++] = u8Data[i];
                    }
                }

                std::bitset<64> BitMask;
                for (int j = i32BitStartPos; j < i32BitStopPos; j++)
                {
                    BitMask.set(j);
                }

                mData.u64Value &= (~BitMask.to_ulong());

                mData.u64Value |= (value << i32BitStartPos);

                i32Pos = 0;

                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
            }
        }

        void dataStorage::setData(unsigned char *u8Data, const unsigned int &u32Size, const int &byteStartPos, const int &byteSize, const int &bitStartPos, const int &bitSize, const int &value, const std::string &dataType)
        {
            int i32Pos = 0;
            int i32BitStartPos = bitStartPos;
            int i32BitStopPos = i32BitStartPos + bitSize;
            int i32ByteStartPos = byteStartPos;
            int i32ByteStopPos = i32ByteStartPos + byteSize;
            std::ostringstream strErr;
            unDataConvert mData;
            mData.i64Value = 0;

            if( (i32ByteStartPos < 0) || (i32ByteStopPos < 0))
            {
                strErr << "起始字节终止位异常 i32ByteStartPos(" << i32ByteStartPos << "), i32ByteStopPos(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (i32BitStartPos < 0) || (i32BitStopPos < 0))
            {
                strErr << "起始位终止位异常 i32BitStartPos(" << i32BitStartPos << "), i32BitStopPos(" << i32BitStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if( (bitSize > 64) || (bitSize < 0))
            {
                strErr << "起始位终止位差值异常 bitsize = (" << bitSize << ") => (0 < bitsize < 64)";
                throw std::runtime_error(strErr.str());
            }

            //校验
            if (i32ByteStopPos > u32Size)
            {
                strErr << " 总长度(" << std::dec << u32Size << ") < 待提取长度(" << i32ByteStopPos << ")";
                throw std::runtime_error(strErr.str());
            }

            if (bitSize == 0)
            {
                mData.i64Value = value;

                i32Pos = 0;
                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
            }
            else
            {
                i32Pos = 0;
                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        mData.u8Buf[i32Pos++] = u8Data[i];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        mData.u8Buf[i32Pos++] = u8Data[i];
                    }
                }

                std::bitset<64> BitMask;
                for (int j = i32BitStartPos; j < i32BitStopPos; j++)
                {
                    BitMask.set(j);
                }

                mData.u64Value &= (~BitMask.to_ulong());

                mData.u64Value |= (value << i32BitStartPos);

                i32Pos = 0;

                if (dataType == BIGENDIAN)
                {
                    for (int i = i32ByteStopPos - 1; i >= i32ByteStartPos; i--)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
                else if (dataType == SAMLLENDIAN)
                {
                    for (int i = i32ByteStartPos; i < i32ByteStopPos; i++)
                    {
                        u8Data[i] = mData.u8Buf[i32Pos++];
                    }
                }
            }
        }
    }
}
