#pragma once


#include <string>

#include "../equipment_global.h"

#include "./Visa/visa.h"
//#include "./Visa/ivi.h"

#include <vector>

//#define DEBUG_RS422	0

#define SERIALPORT_SIZE_MAX	1024

#define TIMOUT_MS 2000

class visaCtrl
{
public:
    visaCtrl(){}
    visaCtrl(std::string);
    ~visaCtrl(void);
public:
    void Init(std::string);
	
	bool receiveMsg(unsigned char *rData, unsigned int *rSize);
	bool sendMsg(unsigned char *tData, unsigned int tSize);
	
	bool Write(std::vector<unsigned char>);
	bool Read(std::vector<unsigned char> &);

private:
	void Init();
	ViAttrState GetParityV(std::string Parity);		
	ViAttrState GetStopBitsV(std::string StopBits);
private:
	std::string mRname;
	std::string mParity;
	int mBaud;
	int mDatabits; 
	std::string mStopbits;

	ViSession rmSession;
	ViSession pnInstrHandle;	
};
