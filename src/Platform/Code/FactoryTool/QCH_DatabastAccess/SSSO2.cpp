// SSSO2.cpp: implementation of the CSSSO2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSSO2.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\LogPolicy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace QSYNC_LIB;

CSSSO2::CSSSO2()
{

}

CSSSO2::~CSSSO2()
{

}

unsigned char CSSSO2::Initial_SSSO2Dll()
{
	//to regist the COM
	HRESULT hresult;
	CLSID clsid;
	/*UINT Result;
	CString ExecutableFilePath=Current_Directory+"\\UnRegO2.bat";

	Result=WinExec(ExecutableFilePath,SW_SHOW);
	if((Result==0)||
	   (Result==ERROR_BAD_FORMAT)||
	   (Result==ERROR_FILE_NOT_FOUND)||
	   (Result==ERROR_PATH_NOT_FOUND))
	{
	    Message_String="Unregister SSS_O2.dll fail!!";
		AfxMessageBox(Message_String);
		return FALSE;
	}

	ExecutableFilePath=Current_Directory+"\\RegO2.bat";
	Result=WinExec(ExecutableFilePath,SW_SHOW);
	if((Result==0)||
	   (Result==ERROR_BAD_FORMAT)||
	   (Result==ERROR_FILE_NOT_FOUND)||
	   (Result==ERROR_PATH_NOT_FOUND))
	{
	    Message_String="Register SSS_O2.dll fail!!";
		AfxMessageBox(Message_String);
		return FALSE;
	}*/

	CoInitialize(NULL);

	hresult=CLSIDFromProgID(OLESTR("SSS_O2.clsO2Check"), &clsid);
	hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_clsO2Check),(LPVOID *) &ITS_O2);
	if(FAILED(hresult))
	{
		WinExec("tregsvr -q SSS_O2.Dll",SW_SHOWDEFAULT);
		CString strFileName = _T("IMEIStationToolLog.txt");
		LogPolicy::Log("Initial SSSO2Dll fail!", strFileName);
		return S_FALSE;
	}
	return S_OK;
}

unsigned char CSSSO2::SSSO2_QueryIMEI(
									  char *szIMEI,char* szMSN,char* szBTAddr,char* szSW_ver,char* szHW_ver,
									  char *StationtState, char *ErrCode, char *ErrMsg,char *Status_Output)
{
	BOOL bResult;	
	CComBSTR	_GSN(20);
	CComBSTR	_BT(14);
	CComBSTR	 _SW_(10);
	CComBSTR	_HW(10);
	CComBSTR	_MDate(20);
	CComBSTR	_MTime(20);
	CComBSTR	_Carton(32);
	CComBSTR	_DBDate(20);
	CComBSTR	_DBTime(20);
	CComBSTR	_Phone(20);
	CComBSTR	_Mode(2);
	CComBSTR	_ErrCode(12);
	CComBSTR	_ErrDes(104);
	CComBSTR	_Wireless_MAC(50);
	_bstr_t		_temp;
	char		_szErrCode[10];
	bResult = ITS_O2->funQueryIMEI_Data(szIMEI,
										StationtState,
										&_GSN,
										&_BT,
										&_HW,
										&_SW_,
										&_MDate,
										&_MTime,
										&_Carton,
										&_DBDate,
										&_DBTime,
										&_Phone,
										&_Mode,
										&_ErrCode,
										&_ErrDes,
										&_Wireless_MAC);//Added

	_temp = _GSN.Copy();
	UnicodeToAnsiBuffer(_temp,szMSN,21);
	_GSN.Empty();

	_temp = _BT.Copy();
	UnicodeToAnsiBuffer(_temp,szBTAddr,15);
	_BT.Empty();

    _temp = _SW_.Copy();//Lucky
	UnicodeToAnsiBuffer(_temp,szSW_ver,11);
	_SW_.Empty();

	_temp = _HW.Copy();//Lucky
	UnicodeToAnsiBuffer(_temp,szHW_ver,11);
	_HW.Empty();

	_temp = _Carton.Copy();
	UnicodeToAnsiBuffer(_temp,Status_Output,33);
	_Carton.Empty();

	_temp = _ErrCode.Copy();
	UnicodeToAnsiBuffer(_temp,_szErrCode,13);
	UnicodeToAnsiBuffer(_temp,ErrCode,13);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	UnicodeToAnsiBuffer(_temp,ErrMsg,105);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, "0000") == 0 )
		return TRUE;
	
	return FALSE;

}

unsigned char CSSSO2::SSSO2_UpdateIMEIStatus(
	char* szIMEI,char* Software_Version,char* Hardware_Version,char* MAL_FROM_SCANNER,
	char *ErrCode, char *ErrMsg)
{
	BOOL bResult;
	CComBSTR	_ErrCode(12), _ErrDes(104);
	_bstr_t		_temp;
	char		_szErrCode[10] = {0};
	short int	_iRow;
	char		_SWver[8] = {0};
	char        g_HW_ver[8] = {0};
	char        _NAL[16] = {0};//the length of NAL is 15

	strcpy(_SWver,Software_Version);
	strcpy(g_HW_ver,Hardware_Version);
	strcpy(_NAL,MAL_FROM_SCANNER);
	/*
	strcpy(_SWver,SW_Version_In_Mobile);
	strcpy(g_HW_ver,HW_Version_In_Mobile);
	*/
	bResult = ITS_O2->funUpdateIMEI_Data(szIMEI,_SWver,g_HW_ver,_NAL/*Added*/,&_iRow,&_ErrCode,&_ErrDes);
	//bResult = ITS_O2->funUpdateIMEI_Data(gIMEIrecord.IMEI,(LPCTSTR),(LPCTSTR),&_iRow,&_ErrCode,&_ErrDes);//Lucky

	_temp = _ErrCode.Copy();
	UnicodeToAnsiBuffer(_temp,_szErrCode,12);
	UnicodeToAnsiBuffer(_temp,ErrCode,12);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	UnicodeToAnsiBuffer(_temp,ErrMsg,104);
	_ErrDes.Empty();
    
	if ( strcmp(_szErrCode, "0000") == 0 )
		return true;
	
	return false;
}

unsigned char CSSSO2::SSSO2_CheckNAL_PN(
										char* Part_Number,char* MAL_FROM_SCANNER,char* IMEI_FROM_SCANNER,
										char *ErrCode, char *ErrMsg)
{
	BOOL bResult;

	CComBSTR	_ErrCode(12), _ErrDes(104);
	_bstr_t		_temp;
	char		_szErrCode[10];
	char		_PartNumber[13];//the length of PN is 12
	char        _NAL[16];//the length of NAL is 15
	char        _IMEI[16];//the length of IMEI is 15
    ZeroMemory(_szErrCode,10);
    ZeroMemory(_PartNumber,13);
	ZeroMemory(_NAL,16);
	ZeroMemory(_IMEI,16);

	strcpy(_PartNumber,Part_Number);
	strcpy(_NAL,MAL_FROM_SCANNER);
	strcpy(_IMEI,IMEI_FROM_SCANNER);
	
	bResult = ITS_O2->funCheckNALPN_Data(_IMEI,_PartNumber,_NAL,&_ErrCode,&_ErrDes);

	_temp = _ErrCode.Copy();
	UnicodeToAnsiBuffer(_temp,_szErrCode,12);
	UnicodeToAnsiBuffer(_temp,ErrCode,12);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	UnicodeToAnsiBuffer(_temp,ErrMsg,104);
	_ErrDes.Empty();
    
	if ( strcmp(_szErrCode, "400") == 0 )
		return true;
	
	return false;

}

unsigned char CSSSO2::SSSO2_GetProcessResult(
	char* szPicasso,char* szModel,
	char *ErrCode, char *ErrMsg)
{
	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(12), _ErrDes(104),_GDNG(2);
	char        _szGDNG[2];
	char		_szErrCode[10];
//check MMI
	bResult = ITS_O2->funGetProcessResult(szPicasso,"MMI",szModel,&_GDNG,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	UnicodeToAnsiBuffer(_temp,_szErrCode,12);
	UnicodeToAnsiBuffer(_temp,ErrCode,12);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	UnicodeToAnsiBuffer(_temp,ErrMsg,104);
	_ErrDes.Empty();

	_temp = _GDNG.Copy();
	UnicodeToAnsiBuffer(_temp,_szGDNG,2);
	_GDNG.Empty();
    
	if (strncmp(_szGDNG, "Y",1) != 0 )
	   return false;

//080711/SUNLIGHT ADD FOR CHECK FAV
	bResult = ITS_O2->funGetProcessResult(szPicasso,"FA_VI",szModel,&_GDNG,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	UnicodeToAnsiBuffer(_temp,_szErrCode,12);
	UnicodeToAnsiBuffer(_temp,ErrCode,12);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	UnicodeToAnsiBuffer(_temp,ErrMsg,104);
	_ErrDes.Empty();

	_temp = _GDNG.Copy();
	UnicodeToAnsiBuffer(_temp,_szGDNG,2);
	_GDNG.Empty();
    
	if (strncmp(_szGDNG, "Y",1) != 0 )
		return false;
	else return true;
}
