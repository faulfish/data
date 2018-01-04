#if !defined(GPIBCTRL_H)
#define GPIBCTRL_H

#ifdef GPIBCONTROL_EXPORT
	#define	GPIBCTRL_API extern "C" __declspec(dllexport)
#else
	#define GPIBCTRL_API extern "C" __declspec(dllimport)
#endif

typedef bool (*pfnGPIB_SetControlIndex)(char *pchIndex);
GPIBCTRL_API bool __cdecl GPIB_SetControlIndex(char *pchIndex);

typedef bool (*pfnGPIB_GetDeviceCount)(char *pchCount);
GPIBCTRL_API bool __cdecl GPIB_GetDeviceCount(char *pchCount);

typedef bool (*pfnGPIB_QueryDeviceDescription)(char *pchIndex,char *pchInstrName);
GPIBCTRL_API bool __cdecl GPIB_QueryDeviceDescription(char *pchIndex,char *pchInstrName);

typedef bool (*pfnGPIB_SetCurrentVoltage)(char *pchIndex,char *pchCurrent, char *pchVoltage,char *pchOutput);
GPIBCTRL_API bool __cdecl GPIB_SetCurrentVoltage(char *pchIndex,char *pchCurrent, char *pchVoltage,char *pchOutput= "0");

typedef bool (*pfnGPIB_IsPowerSupply)(char *pchIndex, char *pchIsPowerSupply);
GPIBCTRL_API bool __cdecl GPIB_IsPowerSupply(char *pchIndex, char *pchIsPowerSupply);

typedef bool (*pfnGPIB_Initialization)(void);
GPIBCTRL_API bool __cdecl GPIB_Initialization(void);

typedef bool (*pfnGPIB_SendCommmand)(char *pchIndex, char *pchCmd);
GPIBCTRL_API bool __cdecl GPIB_SendCommmand(char *pchIndex, char *pchCmd);

typedef bool (*pfnGPIB_SendQueryCommmand)(char *pchIndex, char *pchCmd,char *pchCmdResult);
GPIBCTRL_API bool __cdecl GPIB_SendQueryCommmand(char *pchIndex, char *pchCmd,char *pchCmdResult);

typedef bool (*pfnGPIB_ReadCommandResult)(char *pchIdnex,char *pchCmdResult);
GPIBCTRL_API bool __cdecl GPIB_ReadCommandResult(char *pchIndex,char*pchCmdResult);

typedef bool (*pfnGPIB_ErrorMsg)(char *pchErrMsg);
GPIBCTRL_API bool __cdecl GPIB_ErrorMsg(char *pchErrMsg);

#endif	//	defined GPIBCTRL_H