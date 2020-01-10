#include "visaOpt.h"

#include <sstream>

#include <Windows.h>

Implement_DynCreate(visaOpt, JhDevObj)

/*
*	功能描述 :	CRs422Opt:构造函数
*	输入参数 :	无
*	输出参数 :	无
*	返 回 值 :	无
*	作    者 :	xqx
*/


visaOpt::visaOpt(void)
{
    m422Handle = new visaCtrl();
	i32Channel = 0;
}

/*
*	功能描述 :	~CSyn422Opt:析构函数
*	输入参数 :	无
*	输出参数 :	无
*	返 回 值 :	无
*	作    者 :	xqx
*/

visaOpt::~visaOpt(void)
{
	if (m422Handle != NULL)
	{
		delete m422Handle;
		m422Handle = NULL;
		i32Channel = 0;
	}
}

void visaOpt::unInit()
{
	delete this;
}

/*
*	功能描述 :	
*	输入参数 :	xmlEle:节点
*	输出参数 :	无
*	返 回 值 :	无
*	作    者 :	xqx
*/

void visaOpt::init(TiXmlElement *xmlEle)
{
	std::ostringstream strErr;
	int i32Baud = 0;
	int i32DataBits = 0;

	std::string rname = "";
	std::string stopbits = "";
	std::string parity = "";

	std::string tmp = "";

	strDevId = xmlEle->Attribute("id");

	TiXmlElement *pTmpElem = NULL;
	
	pTmpElem = xmlEle->FirstChildElement("rname");
	if (pTmpElem)
	{
		rname = pTmpElem->GetText();
	}

	if (rname == "")
	{
		strErr.str("");
        strErr << "visaOpt rname 输入错误";

		throw std::runtime_error(strErr.str());
	}

    m422Handle->Init(rname);
}
	//发送消息
bool visaOpt::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
{
	std::lock_guard<std::mutex> lk(mMutex);
		
	return m422Handle->sendMsg(u8Msg, u32MsgLen);
}
bool visaOpt::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
{
	std::lock_guard<std::mutex> lk(mMutex);
	
	return m422Handle->receiveMsg(u8Msg, u32MsgLen);
}
