#include <windows.h>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "stdio.h"
#include "ni488.h"
#include "GPIBControl.h"
#include <tchar.h>

using namespace std;

#define UNDEFINED							-1
#define MAX_STRING_LENGTH			1024
#define TIMEOUT							T10s
#define EOTMODE							1
#define EOSMODE							0
#define MAX_INSTRUMENT_COUNT	31
#define POWERSUPPLY_LISTFILE		"PowerSupply.list"
#define MAX_PS_COUNT					10
#define MAX_PS_NAME_LENGTH		256

#define CMD_SET_CURRENT 0
#define CMD_SET_VOLTAGE 1

static int iGpibIndex;
static int iInstrCount;
static int iPSListCount;
static Addr4882_t IniInstrumentAddr[MAX_INSTRUMENT_COUNT];
static Addr4882_t InstrumentAddr[MAX_INSTRUMENT_COUNT];
static char chPowerSupplyList[MAX_PS_COUNT][MAX_PS_NAME_LENGTH];

static char ErrorMnemonic[28][5] =
{"EDVR", "ECIC", "ENOL", "EADR", "EARG",
  "ESAC", "EABO", "ENEB", "EDMA", "", "EOIP",
  "ECAP", "EFSO", "", "EBUS", "ESTB",
  "ESRQ", "", "", "", "ETAB",
  "ELCK", "EARM", "EHDL", "", ""
  "EWIP", "ERST", "EPWR"};

extern int ibsta;
extern int iberr;
extern int ibcnt;
extern long ibcntl;

GPIBCTRL_API bool __cdecl GPIB_Initialization(void)
{
	bool	bResult = true;	
	ifstream		ifsPSList(POWERSUPPLY_LISTFILE);	

	for (int loop = 0; loop <= MAX_INSTRUMENT_COUNT; loop++)
	{
		IniInstrumentAddr[loop] = NOADDR;
	}
	
	iGpibIndex = UNDEFINED;
	iInstrCount = UNDEFINED;			

	if (ifsPSList.is_open())
	{
		char		chReadLine[MAX_PS_NAME_LENGTH];		
		int			iNameIndex = 0;

		while (! ifsPSList.eof())
		{
			memset(chReadLine,'\0',MAX_PS_NAME_LENGTH),
			ifsPSList.getline(chReadLine,MAX_PS_NAME_LENGTH);			
			if (chReadLine[0]=='\0')
			{
				continue;
			}
			strcpy_s(chPowerSupplyList[iNameIndex],MAX_PS_NAME_LENGTH,chReadLine);			
			++iNameIndex;
		}

		iPSListCount = iNameIndex;
	}
	else
	{
		MessageBox(NULL,_T("c:\\PowerSupply.list not found"),_T("Caution"),MB_OK);
		bResult = false;
		return bResult;
	}

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_SetControlIndex(char *pchIndex)
{
	bool	bResult = true;

	iGpibIndex = atoi(pchIndex);		

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_GetDeviceCount(char *pchCount)
{
	bool			bResult = true;
	errno_t		errResult;

	if (UNDEFINED == iGpibIndex)
	{
		bResult = false;
		return bResult;
	}

	SendIFC(iGpibIndex);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	for (int loop = 0; loop < MAX_INSTRUMENT_COUNT; loop++)
	{
		IniInstrumentAddr[loop] = (Addr4882_t)(loop + 1);
	}

	IniInstrumentAddr[MAX_INSTRUMENT_COUNT-1] = NOADDR;

	FindLstn(iGpibIndex, IniInstrumentAddr, InstrumentAddr, MAX_INSTRUMENT_COUNT);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	iInstrCount = ibcntl;	

	errResult = _itoa_s(iInstrCount,pchCount,MAX_STRING_LENGTH,10);	

	if (0!=errResult)
	{
		bResult = false;
	}

	ibonl (iGpibIndex, 0);		

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_QueryDeviceDescription(char *pchIndex,char *pchInstrName)
{
	bool	bResult = true;	
	int		iInstrIndex = atoi(pchIndex);
	int		iInstrHandle;

	if (UNDEFINED == iGpibIndex)
	{
		bResult = false;
		return bResult;
	}

	iInstrHandle = ibdev(iGpibIndex,GetPAD(InstrumentAddr[iInstrIndex]),GetSAD(InstrumentAddr[iInstrIndex]),
		TIMEOUT, EOTMODE, EOSMODE);	

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	ibclr(iInstrHandle);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	ibwrt(iInstrHandle,"*IDN?",5L);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}	

	ibrd(iInstrHandle,pchInstrName,MAX_STRING_LENGTH);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_IsPowerSupply(char *pchIndex, char *pchIsPowerSupply)
{
	bool	bResult = true;
	char	chInstrName[MAX_STRING_LENGTH];		

	GPIB_QueryDeviceDescription(pchIndex,chInstrName);

	for (int i=0;i<iPSListCount;++i)
	{
		if (strstr(chInstrName,chPowerSupplyList[i]))
		{			
			sprintf_s(pchIsPowerSupply,MAX_STRING_LENGTH,"TRUE");
				return bResult;			
		}
	}

	sprintf_s(pchIsPowerSupply,MAX_STRING_LENGTH,"FALSE");

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_SetCurrentVoltage(char *pchIndex, char *pchCurrent, char *pchVoltage, char *pchOutput)
{
	bool	bResult = true;
	char	chIsPowerSupply[MAX_STRING_LENGTH];	
	int		iInstrHandle;
	int		iIndex = atoi(pchIndex);
	char	chCommand[MAX_STRING_LENGTH];

	if (UNDEFINED == iGpibIndex)
	{
		bResult = false;
		return bResult;
	}

	GPIB_IsPowerSupply(pchIndex,chIsPowerSupply);

	if (strstr(chIsPowerSupply,"TRUE")==NULL)
	{
		bResult = false;
		return bResult;
	}

	iInstrHandle = ibdev(iGpibIndex,GetPAD(InstrumentAddr[iIndex]),GetSAD(InstrumentAddr[iIndex]),
		TIMEOUT, EOTMODE, EOSMODE);	

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	ibclr(iInstrHandle);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}	

	sprintf_s(chCommand,MAX_STRING_LENGTH,"CURR %s", pchCurrent);	

	ibwrt(iInstrHandle,chCommand,MultiByteToWideChar(CP_ACP, 0, chCommand, -1, NULL, 0));

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}	

	sprintf_s(chCommand,MAX_STRING_LENGTH,"VOLT %s", pchVoltage);

	ibwrt(iInstrHandle,chCommand,MultiByteToWideChar(CP_ACP, 0, chCommand, -1, NULL, 0));

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	if (strstr(pchOutput,"1")!=NULL)
	{
		ibwrt(iInstrHandle,"OUTP ON",7L);

		if (ibsta & ERR)
		{
			bResult = false;
			return bResult;
		}
	} 
	else if (strstr(pchOutput,"0")!=NULL)
	{
		ibwrt(iInstrHandle,"OUTP OFF",8L);

		if (ibsta & ERR)
		{
			bResult = false;
			return bResult;
		}
	}

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_SendCommmand(char *pchIndex, char *pchCmd)
{
	bool	bResult = true;			
	int		iInstrHandle;	
	int		iIndex = atoi(pchIndex);

	if (UNDEFINED == iGpibIndex)
	{
		bResult = false;
		return bResult;
	}	

	iInstrHandle = ibdev(iGpibIndex,GetPAD(InstrumentAddr[iIndex]),GetSAD(InstrumentAddr[iIndex]),
		TIMEOUT, EOTMODE, EOSMODE);	

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	ibclr(iInstrHandle);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}		

	ibwrt(iInstrHandle,pchCmd,MultiByteToWideChar(CP_ACP, 0, pchCmd, -1, NULL, 0));	

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}	

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_ReadCommandResult(char *pchIndex,char* pchCmdResult)
{
	bool	bResult = true;			
	int		iInstrHandle;	
	int		iIndex = atoi(pchIndex);

	if (UNDEFINED == iGpibIndex)
	{
		bResult = false;
		return bResult;
	}	

	iInstrHandle = ibdev(iGpibIndex,GetPAD(InstrumentAddr[iIndex]),GetSAD(InstrumentAddr[iIndex]),
		TIMEOUT, EOTMODE, EOSMODE);	

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}	

	ibrd(iInstrHandle,pchCmdResult,MAX_STRING_LENGTH);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_SendQueryCommmand(char *pchIndex, char *pchCmd,char *pchCmdResult)
{
	bool	bResult = true;			
	int		iInstrHandle;	
	int		iIndex = atoi(pchIndex);

	if (UNDEFINED == iGpibIndex)
	{
		bResult = false;
		return bResult;
	}	

	iInstrHandle = ibdev(iGpibIndex,GetPAD(InstrumentAddr[iIndex]),GetSAD(InstrumentAddr[iIndex]),
		TIMEOUT, EOTMODE, EOSMODE);	

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	ibclr(iInstrHandle);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}		

	ibwrt(iInstrHandle,pchCmd,MultiByteToWideChar(CP_ACP, 0, pchCmd, -1, NULL, 0));

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}	

	ibrd(iInstrHandle,pchCmdResult,MAX_STRING_LENGTH);

	if (ibsta & ERR)
	{
		bResult = false;
		return bResult;
	}

	return bResult;
}

GPIBCTRL_API bool __cdecl GPIB_ErrorMsg(char *pchErrMsg)
{
	bool	bResult = true;
		
	sprintf_s(pchErrMsg,MAX_STRING_LENGTH,("ibsta = 0x%x \niberr = %d (%s)"),ibsta,iberr,ErrorMnemonic[iberr]);
	
	return bResult;
}