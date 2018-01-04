
#ifndef _BENQ_MOBILEPHONE_API_H_
#define _BENQ_MOBILEPHONE_API_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////
//	Interface function prototypes
 
#ifdef _MOBILE_INTERFACE_DLL	
		#define	MOBILE_INTERFACE	__declspec(dllexport)	
#else
		#define MOBILE_INTERFACE	__declspec(dllimport)
#endif

#include "IEFS.h"
#include "ISMS.h"
#include "ISYNCML.h"
#include "IPhonebook.h"
#include "ICalendar.h"
#include "IPIMCONFIG.h"

typedef enum CommInterface
{
  eIEFS = 0,
  eISyncML,
  eIIrMC,
  eIPIMConfigure,
  eIPhonebook,
  eICalendar,
  eISMS,
  eIMMS,
  eIMediaCenter,
  eISetting
} CommInterface;

typedef enum CommRetCode
{
   eCommErrInterfaceLocked = -1,
   eCommRetSuccess = 0, //Is the same S_OK 0
   eCommErrInterfaceAlreayOpen,
   eCommErrInterfaceNotExist,
   eCommErrInterfaceNotSupport,
   eCommErrDeviceAlreadyConnect,
   eCommErrDeviceNotInit,
   eCommErrDevModelNameNotMatch,
   eCommErrParamEmpty,
   eCommErrAttachDevice,
   eCommErrDeviceNotConnect,
   eCommErrParamNull,
   eCommErrWithoutIMEI
} CommRetCode; 

typedef enum CommTransDevice
{
    eCommSerial,
    eCommBlueTooth,
    eCommMassStorage,
    eCommEmptyDev
} CommTransDevice;

typedef enum CommDetectMode
{
    eCommDetectAuto,
    eCommDetectManual,
    eCommNoDetect   
} CommDetectMode;

typedef enum CommConnectionState
{
    eCommNoConnection,
    eCommConnecting,
    eCommConnected,
    eCommDisConnected
} CommConnectionState;

interface ICommDevice
{
	virtual BOOL	DeviceGetInterface( CommInterface eInterface, LPVOID *ppInterface ) =0 ; 
	virtual BOOL	DetachAllInterface () =0 ;
	virtual int		ConnectDevice( LPCTSTR strModel,  LPCTSTR port) =0 ;
	virtual int		DisconnectDevice( LPCTSTR strModel,  LPCTSTR port) =0 ;
	virtual BOOL	IsInterfaceAttached( CommInterface eInterface ) =0 ;
	virtual TSTRING	GetModelName() const = 0;
	virtual TSTRING	GetIMEI() const = 0;
	virtual TSTRING	GetSoftwareVer() const = 0;
	virtual TSTRING GetPort() const = 0;
	virtual CommTransDevice GetTransDevice() const = 0;
	virtual BOOL GetFDNStatus(bool& bIsFDN) = 0;
    virtual BOOL GetConnectivity(bool& bIsOnline) = 0;
	virtual ~ICommDevice(){}
};

typedef ICommDevice* ( * LPDeviceFunc)(void);

#define MAX_RET_STRING_LENGTH 256

MOBILE_INTERFACE int    CommOpenMobileInterface( LPCTSTR strApp ,int nIndex = 0);
MOBILE_INTERFACE int    CommSetDetectMode(  LPCTSTR strApp, CommDetectMode  eConnectMode ,int nIndex = 0); 
MOBILE_INTERFACE int    CommGetDetectMode(  LPCTSTR strApp, CommDetectMode &eConnectMode ,int nIndex = 0); 
MOBILE_INTERFACE int    CommSetDetectPorts( LPCTSTR strApp, CommTransDevice eTransDevice, LPCTSTR strPorts ,int nIndex = 0);
MOBILE_INTERFACE int    CommGetDetectPorts( LPCTSTR strApp, CommTransDevice eTransDevice, LPTSTR strPorts ,int nIndex = 0);
MOBILE_INTERFACE int    CommConnectMobile(  LPCTSTR strApp, CommTransDevice eTransDevice, LPCTSTR strPort, LPCTSTR strMobileModel ,int& nID);
MOBILE_INTERFACE int    CommDisConnectMobile( LPCTSTR strApp, CommTransDevice eTransDevice, LPCTSTR strPort, LPCTSTR strMobileModel ,int nIndex = 0);
MOBILE_INTERFACE int    CommGetConnectionState( CommConnectionState &eConnectionState ,int nIndex = 0);
MOBILE_INTERFACE int    CommGetConnectPort( CommTransDevice &eTransDevice, LPTSTR strPort ,int nID = 0);
MOBILE_INTERFACE int    CommGetMobileInfo(  LPCTSTR strApp, LPTSTR strMobileModel, LPTSTR strIMEI, LPTSTR strDisplayName,int nIndex = 0);
MOBILE_INTERFACE int    CommSetDisplayName( LPCTSTR strApp, LPTSTR strIMEI, LPTSTR strDisplayName,int nIndex = 0);
MOBILE_INTERFACE int    CommGetMobileSoftwareVer( LPTSTR strSoftwareVer ,int nIndex = 0);
MOBILE_INTERFACE int    CommIsConnectionAlive( int nTimeOut, LPCTSTR strIMEI ,int nIndex = 0);
MOBILE_INTERFACE BOOL   CommGetMobileCapability( CommInterface eInterface ,int nIndex = 0);
MOBILE_INTERFACE LPVOID CommCreateInterface( LPCTSTR strApp, LPCTSTR strClient, CommInterface eInterface ,int nIndex = 0);
MOBILE_INTERFACE void   CommDeleteInterface( LPVOID pInterface ,int nIndex = 0);
MOBILE_INTERFACE int    CommCloseMobileInterface( LPCTSTR strApp ,int nIndex = 0);
MOBILE_INTERFACE int    CommGetMobileInterfaceCount();
MOBILE_INTERFACE void CommGetPIMDevice(LPCTSTR strApp,LPCTSTR strType,/*Out*/LPTSTR strPIMDEviceName,int nIndex);
MOBILE_INTERFACE int    CommGetFDNStatus(bool& bIsFDN,int nIndex = 0);
MOBILE_INTERFACE int    CommGetConnectivity(bool& bIsOnline,int nIndex = 0);
#endif