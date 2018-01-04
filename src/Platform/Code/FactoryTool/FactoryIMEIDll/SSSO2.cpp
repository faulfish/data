// SSSO2.cpp: implementation of the CSSSO2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "QUALCOMM_IMEI_CODING.h"
#include "SSSO2.h"

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

unsigned char CSSSO2::SSSO2_QueryIMEI(char *StationtState,
							  char *pIMEI,
							  char *szMSN,
							  char *szBTAddr,
							  char *szSWver,
							  char *szHWver,
							  char *ErrCode, 
							  char *ErrMsg,
							  char *Status_Output)
{
	//return true;

	if(!Initial_SSSO2Dll())
	{
		return false;
	}
	char        _IMEI[16];//the length of IMEI is 15
	ZeroMemory(_IMEI,16);
	strcpy(_IMEI,pIMEI);
	BOOL bResult;	
	CComBSTR	_GSN(20), _BT(14),  _SWV(10), _HW(10), _MDate(20), _MTime(20), _Carton(32), _DBDate(20), _DBTime(20), _Phone(20), _Mode(2), _ErrCode(12), _ErrDes(104),_Wireless_MAC(50);
	_bstr_t		_temp;
	char		_szErrCode[10];

	bResult = ITS_O2->funQueryIMEI_Data(_IMEI,
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

	//_temp = _T("123456");
	//_stprintf(szMSN,_T("%s"), (LPCTSTR)_temp);
	//_GSN.Empty();

	_temp = _GSN.Copy();
	_stprintf(szMSN, _T("%s"), (LPCTSTR)_temp);
	_GSN.Empty();

	_temp = _BT.Copy();
	_stprintf(szBTAddr, _T("%s"), (LPCTSTR)_temp);
	_BT.Empty();

    _temp = _SWV.Copy();//Lucky
	_stprintf(szSWver, _T("%s"), (LPCTSTR)_temp);
	_SWV.Empty();

	_temp = _HW.Copy();//Lucky
	_stprintf(szHWver, _T("%s"), (LPCTSTR)_temp);
	_HW.Empty();

	_temp = _Carton.Copy();
	_stprintf(Status_Output, _T("%s"), (LPCTSTR)_temp);
	_Carton.Empty();


	_temp = _ErrCode.Copy();
	_stprintf(_szErrCode, _T("%s"), (LPCTSTR)_temp);
	_stprintf(ErrCode, _T("%s"), (LPCTSTR)_temp);
	_ErrCode.Empty();

	_temp = _ErrDes.Copy();
	_stprintf(ErrMsg, _T("%s"), (LPCTSTR)_temp);
	_ErrDes.Empty();

	if ( strcmp(_szErrCode, _T("0000")) == 0 )
		return TRUE;
	
	return false;

}

unsigned char CSSSO2::SSSO2_UpdateIMEIStatus(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *ErrCode, char *ErrMsg)
{
	if(!Initial_SSSO2Dll())
	{
		return false;
	}
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
	
	char       _IMEI[16];//the length of IMEI is 15
	ZeroMemory(_IMEI,16);
	strcpy(_IMEI,pIMEI);
	strcpy(_SWver,szSWver);
	strcpy(g_HW_ver,szHWver);
	strcpy(_NAL,szMal);

	bResult = ITS_O2->funUpdateIMEI_Data(_IMEI,_SWver,g_HW_ver,_NAL/*Added*/,&_iRow,&_ErrCode,&_ErrDes);


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
//
unsigned char CSSSO2::SSSO2_CheckNAL_PN(char *inIMEI,char *nPort,char *inMal,char *ErrCode, char *ErrMsg)
{
	BOOL bResult;
	if(!Initial_SSSO2Dll())
	{
		return false;
	}
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

	strcpy(_PartNumber,nPort);
	strcpy(_NAL,inMal);
	strcpy(_IMEI,inIMEI);
	
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

unsigned char CSSSO2::SSSO2_GetProcessResult(char* Picasso, char* Model, char *ErrCode, char *ErrMsg)
{

	if(!Initial_SSSO2Dll())
	{
		return false;
	}
	BOOL bResult;
	_bstr_t		_temp;
	CComBSTR	_ErrCode(12), _ErrDes(104),_GDNG(2);
	char        _szGDNG[2];
	char		_szErrCode[10];
//check MMI
	bResult = ITS_O2->funGetProcessResult(Picasso,"MMI",Model,&_GDNG,&_ErrCode,&_ErrDes);
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
	bResult = ITS_O2->funGetProcessResult(Picasso,"FA_VI",Model,&_GDNG,&_ErrCode,&_ErrDes);
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

unsigned char CSSSO2::SSS02_CheckPicassoInSMT(char* Picasso, char* szProcess)
{
	if(!Initial_SSSO2Dll())
	{
		return false;
	}

	bool bResult =false;
	bResult = ITS_O2->funCheckPicassoInSMT(Picasso, szProcess);

	return bResult;

}
unsigned char CSSSO2::SSS02_CheckPiacssoInPICS(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError)
{
	if(!Initial_SSSO2Dll())
	{
		return false;
	}

	BSTR sBSIMEI[100];
	BSTR sBSMobileID[100];

	ZeroMemory(sBSMobileID,100);
	ZeroMemory(sBSIMEI,100);

	int nResult = ITS_O2->funCheckPicassoInPICS(szModel, Picasso, sBSIMEI, sBSMobileID, szProcess);

	memcpy(szIMEI, sBSIMEI, 16);
	memcpy(szMobileID, sBSMobileID, 32);

	itoa(nResult, szError, 0);
	if(nResult == 0)
		return true;
	else
		return false;
}

