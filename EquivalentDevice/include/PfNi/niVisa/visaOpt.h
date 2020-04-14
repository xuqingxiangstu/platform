#pragma once

#include "../JhDevObj/jhdevobj.h"
#include "visaCtrl.h"
#include <string>
#include <map>

#include <mutex>

using namespace std;

class EQUIPMENTSHARED_EXPORT visaOpt :	public JhDevObj
{
    Declare_DynCreate(visaOpt)
public:
    virtual ~visaOpt(void);
    visaOpt(void);
public:
	void init(TiXmlElement *);
	void unInit();
	bool sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen);
    bool receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax = 1024, const unsigned int u32TimeOut = 1);
	std::string getInfo() { return strDevId; }
private:
	int i32Channel;
    visaCtrl *m422Handle;
	std::string strDevId;
	std::mutex mMutex;
};
