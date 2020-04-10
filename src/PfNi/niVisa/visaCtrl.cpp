#include "visaCtrl.h"

#include <sstream>

/*
*	�������� :	CSerialPort:���캯��
*	������� :	��
*	������� :	��
*	�� �� ֵ :	��
*	��    �� :	xqx
*/

visaCtrl::visaCtrl(std::string rname)
:mRname(rname)
{
	rmSession = 0;
	pnInstrHandle = 0;
	
	Init();	
}

void visaCtrl::Init(std::string rname)
{
	mRname = rname;

	Init();
}

/*
*	�������� :	~CSerialPort:��������
*	������� :	��
*	������� :	��
*	�� �� ֵ :	��
*	��    �� :	xqx
*/

visaCtrl::~visaCtrl(void)
{
#ifndef DEBUG_RS422
	viClose(pnInstrHandle);
	viClose(rmSession);
#endif
	rmSession = 0;
	pnInstrHandle = 0;

}


void visaCtrl::Init()
{
	std::ostringstream strErr;
	ViStatus nReturnS = 0;
	
#ifndef DEBUG_RS422
	nReturnS = viOpenDefaultRM(&rmSession);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mRname << ")viOpenDefaultRM ";
		throw std::runtime_error(strErr.str());
	}
	nReturnS = viOpen(rmSession,ViRsrc(mRname.c_str()),VI_NULL,VI_NULL,&pnInstrHandle);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mRname << ")viOpen ";
		throw std::runtime_error(strErr.str());
	}

    nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_TERMCHAR_EN, VI_FALSE);
    if(nReturnS != VI_SUCCESS)
    {
        strErr << "[ERROR](" << mBaud << ")viSetAttribute(VI_ATTR_TERMCHAR_EN) ";
        throw std::runtime_error(strErr.str());
    }

#if 0
	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_ASRL_BAUD, mBaud);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mBaud << ")viSetAttribute(VI_ATTR_ASRL_BAUD) ";
		throw std::runtime_error(strErr.str());
	}
	
	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_ASRL_DATA_BITS, mDatabits);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mDatabits << ")viSetAttribute(VI_ATTR_ASRL_DATA_BITS) ";
		throw std::runtime_error(strErr.str());
	}

	ViAttrState Value = GetParityV(mParity);
	
	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_ASRL_PARITY, Value);

	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << Value << ")viSetAttribute(VI_ATTR_ASRL_PARITY) ";
		throw std::runtime_error(strErr.str());
	}

	Value = GetStopBitsV(mStopbits);

	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_ASRL_STOP_BITS, Value);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mStopbits << ")viSetAttribute(VI_ATTR_ASRL_STOP_BITS) ";
		throw std::runtime_error(strErr.str());
	}

	//���ó�ʱʱ��
	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_TMO_VALUE, TIMOUT_MS);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << TIMOUT_MS << ")viSetAttribute(VI_ATTR_TMO_VALUE) ";
		throw std::runtime_error(strErr.str());
	}

	//��ֹ��
	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_ASRL_END_OUT, VI_ASRL_END_NONE);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR] viSetAttribute(VI_ATTR_SEND_END_EN) ";
		throw std::runtime_error(strErr.str());
	}

	nReturnS = viSetAttribute(pnInstrHandle, VI_ATTR_ASRL_END_IN, VI_ASRL_END_NONE);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR]viSetAttribute(VI_ATTR_SEND_END_EN) ";
		throw std::runtime_error(strErr.str());
	}

	//�����շ���������С
	// ����
	nReturnS = viSetBuf(pnInstrHandle, VI_IO_OUT_BUF, SERIALPORT_SIZE_MAX);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mRname << ")viSetBuf(VI_IO_OUT_BUF) ";
		throw std::runtime_error(strErr.str());
	}
	//����
	nReturnS = viSetBuf(pnInstrHandle, VI_IO_IN_BUF, SERIALPORT_SIZE_MAX);
	if(nReturnS != VI_SUCCESS)
	{
		strErr << "[ERROR](" << mRname << ")viSetBuf(VI_ATTR_ASRL_STOP_BITS) ";
		throw std::runtime_error(strErr.str());
	}
#endif
#endif
}

ViAttrState visaCtrl::GetStopBitsV(std::string StopBits)
{
	ViAttrState Value = 0;

	if (StopBits == "1")
	{
		Value = VI_ASRL_STOP_ONE;
	}
	else if (StopBits == "1.5")
	{
		Value = VI_ASRL_STOP_ONE5;
	}
	else if (StopBits == "2")
	{
		Value = VI_ASRL_STOP_TWO;
	}

	return Value;
}

/*
*	�������� :	GetParityV:У��λת��
*	������� :	Parity:У���ַ���
*	������� :	��
*	�� �� ֵ :	ViAttrState ��ӦУ��ö��
*	��    �� :	xqx
*/

ViAttrState visaCtrl::GetParityV(std::string Parity)
{
	ViAttrState Value;

	if(mParity == "NONE")
	{
		Value = VI_ASRL_PAR_NONE;
	}
	else if(mParity == "EVEN")
	{
		Value = VI_ASRL_PAR_EVEN;
	}
	else if(mParity == "MARK")
	{
		Value = VI_ASRL_PAR_MARK;
	}
	else if(mParity == "SPACE")
	{
		Value = VI_ASRL_PAR_SPACE;
	}
	else if(mParity == "ODD")
	{
		Value = VI_ASRL_PAR_ODD;
	}
	else 
	{
		Value = VI_ASRL_PAR_NONE;
	}

	return Value;
}

/*
*	�������� :	Write:���ڷ���
*	������� :	Data:д������
*	������� :	��
*	�� �� ֵ :	bool �Ƿ�д�ɹ�
*	��    �� :	xqx
*/

bool visaCtrl::Write(std::vector<unsigned char> Data)
{
	bool Res = false;
#ifndef DEBUG_RS422
	ViUInt32 retCount = VI_NULL;
	std::string temp = mRname;
	if(VI_SUCCESS == viWrite(pnInstrHandle, ViBuf(&Data.at(0)), ViUInt32(Data.size()), &retCount))
	{	
		if((retCount) == Data.size())
		{
			Res = true;
		}
		
	}
#endif
	return Res;
}

bool visaCtrl::receiveMsg(unsigned char *rData, unsigned int *rSize)
{
	char buf[SERIALPORT_SIZE_MAX] = { 0 };
	ViUInt32 retCount = VI_NULL;
	bool Res = false;
#ifndef DEBUG_RS422
	ViStatus State = 0;
    ViUInt32 count = 1024;
    //viGetAttribute(pnInstrHandle, VI_ATTR_ASRL_AVAIL_NUM, &count);
	State = viRead(pnInstrHandle, ViPBuf(buf), count, &retCount);

	if (retCount > 0)
	{
		for (int i = 0; i < retCount; i++)
		{
			rData[i] = buf[i];
		}

		*rSize = retCount;

		Res = true;
	}
	else
	{
		*rSize = 0;
	}
#endif
	return Res;
}
bool visaCtrl::sendMsg(unsigned char *tData, unsigned int tSize)
{
	bool Res = false;
#ifndef DEBUG_RS422
	ViUInt32 retCount = VI_NULL;
	std::string temp = mRname;
	if (VI_SUCCESS == viWrite(pnInstrHandle, ViBuf(tData), ViUInt32(tSize), &retCount))
	{
		if ((retCount) == tSize)
		{
			Res = true;
		}

	}
#endif
	return Res;
}

/*
*	�������� :	Read:���ڶ�
*	������� :	Cnt:������
*	������� :	Data:��������
*	�� �� ֵ :	bool �Ƿ���ɹ�
*	��    �� :	xqx
*/

bool visaCtrl::Read(std::vector<unsigned char> &RBuf)
{
	char buf[SERIALPORT_SIZE_MAX] = {0};
	ViUInt32 retCount = VI_NULL;
	bool Res = false;
#ifndef DEBUG_RS422
	ViStatus State = 0;
    ViUInt32 count = 1024;
	State = viRead(pnInstrHandle, ViPBuf(buf), count, &retCount);

	if(retCount > 0)
	{
		for(int i = 0; i < retCount; i++)
		{
			RBuf.push_back(buf[i]);
		}

		Res = true;
	}	
#endif
	return Res;
}
