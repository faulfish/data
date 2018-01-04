// SSSO2.cpp: implementation of the CSSSO2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "SSSO2.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
		return FALSE;
	}
	return TRUE;
}

unsigned char CSSSO2::SSSO2_QueryIMEI(char *StationtState, char *ErrCode, char *ErrMsg,char *Status_Output)
{
	BOOL bResult;	
	CComBSTR	_GSN(20), _BT(14),  _SWV(10), _HW(10), _MDate(20), _MTime(20), _Carton(32), _DBDate(20), _DBTime(20), _Phone(20), _Mode(2), _ErrCode(12), _ErrDes(104),_Wireless_MAC(50);
	_bstr_t		_temp;
	char		_szErrCode[10];
	bResult = ITS_O2->funQueryIMEI_Data(gIMEIrecord.IMEI,
										StationtState,
										&_GSN,
										&_BT,
										&_HW,
										&_SWV,
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
	_stprintf(gIMEIrecord.MSN, _T("%s"), (LPCTSTR)_temp);
	_GSN.Empty();

	_temp = _BT.Copy();
	_stprintf(gIMEIrecord.BTAddr, _T("%s"), (LPCTSTR)_temp);
	_BT.Empty();

    _temp = _SWV.Copy();//Lucky
	_stprintf(gIMEIrecord.SW_ver, _T("%s"), (LPCTSTR)_temp);
	_SWV.Empty();

	_temp = _HW.Copy();//Lucky
	_stprintf(gIMEIrecord.HW_ver, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();

	_temp = _Carton.Copy();
	_stprintf(Status_Output, _T("%s"), (LPCTSTR)_temp);
	_Carton.Empty();

	/*_temp = _SW.Copy();
	_stprintf(m_szSW, _T("%s"), (LPCTSTR)_temp);
	_SW.Empty();

	_temp = _HW.Copy();
	_stprintf(m_szHW, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();


	_temp = _Phone.Copy();
	_stprintf(m_szPhone, _T("%s"), (LPCTSTR)_temp);
	_Phone.Empty();*/

	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return TRUE;
	
	return FALSE;

}

unsigned char CSSSO2::SSSO2_UpdateIMEIStatus(char *ErrCode, char *ErrMsg)
{
	BOOL bResult;
	CComBSTR	_ErrCode(12), _ErrDes(104);
	_bstr_t		_temp;
	char		_szErrCode[10];
	short int	_iRow;
	char		_SWver[8];
	char        g_HW_ver[8];
	char        _NAL[16];//the length of NAL is 15
    ZeroMemory(_SWver,8);
    ZeroMemory(g_HW_ver,8);
	ZeroMemory(_NAL,16);

	strcpy(_SWver,Software_Version);
	strcpy(g_HW_ver,Hardware_Version);
	strcpy(_NAL,MAL_FROM_SCANNER);
	/*
	strcpy(_SWver,SW_Version_In_Mobile);
	strcpy(g_HW_ver,HW_Version_In_Mobile);
	*/
	bResult = ITS_O2->funUpdateIMEI_Data(gIMEIrecord.IMEI,_SWver,g_HW_ver,_NAL/*Added*/,&_iRow,&_ErrCode,&_ErrDes);
	//bResult = ITS_O2->funUpdateIMEI_Data(gIMEIrecord.IMEI,(LPCTSTR),(LPCTSTR),&_iRow,&_ErrCode,&_ErrDes);//Lucky

	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();
    
	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return true;
	
	return false;
}

unsigned char CSSSO2::SSSO2_CheckNAL_PN(char *ErrCode, char *ErrMsg)
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
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();
    
	if ( strcmp(_szErrCode, _T("400")) == 0 )
		return true;
	
	return false;

}

unsigned char CSSSO2::SSSO2_GetProcessResult(char *ErrCode, char *ErrMsg)
{
	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(12), _ErrDes(104),_GDNG(2);
	char        _szGDNG[2];
	char		_szErrCode[10];
//check MMI
	bResult = ITS_O2->funGetProcessResult(gIMEIrecord.Picasso,"MMI",gIMEIrecord.Model,&_GDNG,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	_temp = _GDNG.Copy();
	_stprintf(_szGDNG, _T("%s"), (LPCTSTR)_temp);
	_GDNG.Empty();
    
	if (strncmp(_szGDNG, _T("Y"),1) != 0 )
	   return false;

//080711/SUNLIGHT ADD FOR CHECK FAV
	bResult = ITS_O2->funGetProcessResult(gIMEIrecord.Picasso,"FA_VI",gIMEIrecord.Model,&_GDNG,&_ErrCode,&_ErrDes);
	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	_temp = _GDNG.Copy();
	_stprintf(_szGDNG, _T("%s"), (LPCTSTR)_temp);
	_GDNG.Empty();
    
	if (strncmp(_szGDNG, _T("Y"),1) != 0 )
		return false;
	else return true;
}
