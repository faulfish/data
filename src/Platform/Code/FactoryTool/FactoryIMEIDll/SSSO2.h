// SSSO2.h: interface for the CSSSO2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSSO2_H__60904F7E_C68F_430C_B8E7_42946D09FA8B__INCLUDED_)
#define AFX_SSSO2_H__60904F7E_C68F_430C_B8E7_42946D09FA8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>

class CSSSO2  
{
public:
	unsigned char SSSO2_CheckNAL_PN(char *inIMEI,char *nPort,char *inMal,char *ErrCode, char *ErrMsg);
	unsigned char SSSO2_UpdateIMEIStatus(char *pIMEI,char *szSWver,char *szHWver,char *szMal,char *ErrCode, char *ErrMsg);
	unsigned char SSSO2_QueryIMEI(char *StationtState,
									char *pIMEIStuct,
									char *szMSN,
									char *szBTAddr,
									char *szSWver,
									char *szHWver,
									char *ErrCode, 
									char *ErrMsg,
									char *Status_Output);
	unsigned char Initial_SSSO2Dll();
	unsigned char SSSO2_GetProcessResult (char* Picasso, char* Model, char *ErrCode, char *ErrMsg);
	unsigned char SSS02_CheckPicassoInSMT(char* Picasso, char* szProcess);
	unsigned char SSS02_CheckPiacssoInPICS(char* szModel, char* Picasso, char* szIMEI, char* szMobileID, char* szProcess, char* szError);
	CSSSO2();
	_clsO2Check *ITS_O2;
	virtual ~CSSSO2();

};

#endif // !defined(AFX_SSSO2_H__60904F7E_C68F_430C_B8E7_42946D09FA8B__INCLUDED_)
