// DuckCtrl.cpp: implementation of the CDuckCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "DuckCtrl.h"
#include "Duck.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuckCtrl::CDuckCtrl()
{

}

CDuckCtrl::~CDuckCtrl()
{

}

unsigned int CDuckCtrl::InitialDuckLibUsingPort(char *PORT)
{
	return RunDuckLib( PORT, 0);
	 /*
	 you must use this function on your aplication start-up before another DUCK DLL's function, set iEmerg to 1 if you want connect with phone without asking if phone is connected, use it for CEFSEmerg
	 returning values:
	 0 - if ok
	 1 - if fail
	 */
}

unsigned int CDuckCtrl::InitialDuckLibUsingHandle(HANDLE hResourceContext)
{
	return RunDuckLibUsingHandler(hResourceContext, 0);
	/*
	you must use this function on your aplication start-up before another DUCK DLL's function, set iEmerg to 1 if you want connect with phone without asking if phone is connected, use it for CEFSEmerg
	returning values:
	0 - if ok
	1 - if fail
	*/
}

void CDuckCtrl::SetLibMode(unsigned char Lib_Mode)
{
	 //SetDuckLibMode(Lib_Mode);
	 /*
	 you must use this function in your aplication before RunDuckLib() function
	 bUseQPST = true to use QPST, false to use QPHONEMS
	 */

}

void CDuckCtrl::FreeDuckLib()
{
	ShutdownDuckLib();
	/*you must use this function on your aplication exit*/
}



unsigned int CDuckCtrl::BackupBRT(char *BRT_FileName, char *CSV_FileName, char *SPCCode_Array)
{
	
	return Duck_BackupBRT(BRT_FileName, CSV_FileName, SPCCode_Array); 
	/*
	Backup NV Items with BRT file 
	returning values:
	0 - if ok
	1 - if fail
	*/
}

unsigned int CDuckCtrl::RestoreBRT(char *BRT_FileName, char *SPCCode_Array)
{
	
	return Duck_RestoreBRT(BRT_FileName, SPCCode_Array); 
	/*
	restore NV Items with BRT file 
	returning values:
	0 - if ok
	1 - if fail
	*/
}

unsigned int CDuckCtrl::GetLastError(char ** Error)
{
	return Duck_GetLastError(Error);
	/*
	return last error number
	returning values:
	error number
	strLastError - error message
	*/
}

void CDuckCtrl::SetCallBackFunction( void (*CallBackFunction_Progress)(int Range/*ProgressBar range:0---100*/, int Position/*ProgressBar position*/ ), void (*CallBackFunction_Message)( char* Message ) )
{
	Duck_SetCallBack(CallBackFunction_Progress, CallBackFunction_Message);
	/*set call back functions*/
}
