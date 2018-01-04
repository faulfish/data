// PicsCtrl.h: interface for the CPicsCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICSCTRL_H__69C03FA6_4722_42A7_A4DF_339566AD8453__INCLUDED_)
#define AFX_PICSCTRL_H__69C03FA6_4722_42A7_A4DF_339566AD8453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>

class CPicsCtrl  
{
public:
	void CheckPlatformInPICS(char *Platform);
	short EnableLog(bool LogFlag,char *LogFileName);
	short GetSPCCodeFromPICS(const char IMEI_Array[16],char SPC_Array[7]);

	short QueryUnlockUnblockCode(const char IMEI_Array[16],
								 char Network_Unlock_Code_Array[9],
								 char Service_Provider_Unlock_Code_Array[9],
								 char Coporate_Unlock_Code_Array[9],
								 char Network_Subset_Unlock_Code_Array[9],
								 char User_SIM_Unlock_Code_Array[9],
								 char User_USIM_Unlock_Code_Array[9],
								 char Serial_Code_Array[9],
								 char SPC_Array[7],
								 char Phone_Lock_Code_Array[9]);

	short QueryUnlockUnblockCode_BT(const char IMEI_Array[16],
								 char Network_Unlock_Code_Array[17],
								 char Service_Provider_Unlock_Code_Array[17],
								 char Coporate_Unlock_Code_Array[17],
								 char Network_Subset_Unlock_Code_Array[17],
								 char User_SIM_Unlock_Code_Array[17],
								 char User_USIM_Unlock_Code_Array[17],
								 char Serial_Code_Array[17],
								 char SPC_Array[7],
								 char Phone_Lock_Code_Array[17]);

	short GetSIMData(short SIMType, const char IMEI_Array[16],const char FSN_Array[33],const char *SIMFileName,unsigned char SIM_Data_Array[1153], short *SIM_Data_Length);
	short QueryFSNCode(const char IMEI_Array[16],unsigned char FSN_Array[33]);
	short GetFSNCode(const char IMEI_Array[16], unsigned char FSN_Array[33]);
	short CreateIMEIRecord(const char IMEI_Array[16]);
	short CheckIMEI(const char IMEI_Array[16],char *Part_Number_Array);
	short QuerySPCFromPICS(const char IMEI_Array[16],char SPC_Array[7]);
	short ConnectPICS(void);
	HRESULT CreateNewPICSObject(const char *Server,int SupportMultithread,LCID Local,int Reserved,CAObjHandle *ObjectHandle);
	unsigned char Initial_PICSInterfaceDLL(void);
	CPicsCtrl();
	_clsPICSInterface *PICS_Interface;
	virtual ~CPicsCtrl();
};

#endif // !defined(AFX_PICSCTRL_H__69C03FA6_4722_42A7_A4DF_339566AD8453__INCLUDED_)
