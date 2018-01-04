// DuckCtrl.h: interface for the CDuckCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUCKCTRL_H__71F2835C_EB79_4DF5_8266_845DC4CADBA5__INCLUDED_)
#define AFX_DUCKCTRL_H__71F2835C_EB79_4DF5_8266_845DC4CADBA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDuckCtrl  
{
public:
	void SetCallBackFunction( void (*CallBackFunction_Progress)(int Range,int Position), void (*CallBackFunction_Message)(char* Message));
	unsigned int GetLastError(char ** Error);
	unsigned int RestoreBRT(char *BRT_FileName, char *SPCCode_Array);
	unsigned int BackupBRT(char *BRT_FileName, char *CSV_FileName, char *SPCCode_Array);
	void FreeDuckLib(void);
	void SetLibMode(unsigned char Lib_Mode);
	unsigned int InitialDuckLibUsingPort(char *PORT);
	unsigned int InitialDuckLibUsingHandle(HANDLE hResourceContext);
	CDuckCtrl();
	virtual ~CDuckCtrl();

};

#endif // !defined(AFX_DUCKCTRL_H__71F2835C_EB79_4DF5_8266_845DC4CADBA5__INCLUDED_)
