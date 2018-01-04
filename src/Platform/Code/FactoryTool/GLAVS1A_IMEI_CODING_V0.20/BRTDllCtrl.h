// BRTDllCtrl.h: interface for the CBRTDllCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRTDLLCTRL_H__AC1DF715_4D6F_4112_AAAB_CA5D9C8AE577__INCLUDED_)
#define AFX_BRTDLLCTRL_H__AC1DF715_4D6F_4112_AAAB_CA5D9C8AE577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBRTDllCtrl  
{
public:
	unsigned char Initial_BRT_Dll(void);
	void BRTPassHandle(HANDLE Phone_Handle);
	int BRTBackUpAllNVItem(int MaxID, char *BRTFileName,char *SPC_Array, int Log_Flag, int *NVItem_Count, int *Completed);
	int BRTBackUpNVItemAccordingToCSV(char *CSVFileName, char *BRTFileName,char *SPC_Array,int Log_Flag,int *NVItem_Count,int *Completed);
	int BRTRestore(char *BRTFileName,char *SPC_Array,int Log_Flag, int *NVItem_Count,int *Completed);
	int BRTRestoreWithCheck(char *BRTFileName, char *SPC_Array,int Log_Flag,int *NVItem_Count,int *NVItem_Read);
	int BRTGetLastError(char *ErrorMessage);
	int BRTSetLogFile(char *LogFileName);
	CBRTDllCtrl();
	virtual ~CBRTDllCtrl();
	HMODULE  DllHandle;

};

#endif // !defined(AFX_BRTDLLCTRL_H__AC1DF715_4D6F_4112_AAAB_CA5D9C8AE577__INCLUDED_)
