#ifndef __I_RF_CAL_TEST_DLL_H__
#define __I_RF_CAL_TEST_DLL_H__
#include "CommonUtil.h"


#define RF_CALLING_CONVENSION       __stdcall

//  Summary
//    This is the interface for a simple RF Cal and Test system.                       
class IRFCalTestDLL {
public:
	virtual bool RF_CALLING_CONVENSION SetLogger(
		FPFactoryLog fpFactoryLog, 
		FPTraceLog fpTraceLog,
		FPCommandLog fpCommandLog
		) = 0;

	virtual bool RF_CALLING_CONVENSION InitData(
		const PCConfigSetting	pConfigSetting
		) = 0;

	virtual bool RF_CALLING_CONVENSION SetHandlerList(
		const _TCHAR* pszHandlerList
		) = 0;

	virtual bool RF_CALLING_CONVENSION StartProcess()= 0;

	virtual void RF_CALLING_CONVENSION AbortProcess()=0;

	virtual void RF_CALLING_CONVENSION Free() = 0;
};
typedef IRFCalTestDLL* PIRFCalTestDLL;
typedef PIRFCalTestDLL* PPIRFCalTestDLL;

typedef bool (RF_CALLING_CONVENSION *LPFN_CREATE_RFCALPROCESSOR)(PPIRFCalTestDLL);


#endif  //  __I_RF_CAL_TEST_DLL_H__