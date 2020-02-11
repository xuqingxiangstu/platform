#include "datastorage.h"

#include <bitset>
#include <sstream>

namespace Pf
{
    namespace PfIcdWorkBench
    {
        dataStorage::dataStorage()
        {

        }
        unsigned __int64 dataStorage::getData(const unsigned char *u8Data, const unsigned int u32Size, const int byteStartPos, const int byteSize, const int bitStartPos, const int bitSize,const std::string dataType)
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

        void dataStorage::setData(unsigned char *u8Data, const unsigned int u32Size, const int byteStartPos, const int byteSize, const int bitStartPos, const int bitSize, const __int64 value, const std::string dataType)
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
