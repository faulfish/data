// PhoneCtrl.cpp: implementation of the CPhoneCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "PhoneCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhoneCtrl::CPhoneCtrl()
{

}

CPhoneCtrl::~CPhoneCtrl()
{
}

void CPhoneCtrl::SetLibraryMode(unsigned char Mode)
{
	QLIB_SetLibraryMode(Mode);
	return;
    /**********************************************
    Set the library mode--QPST or QPHONEMS. The default state of the library is to use QPHONEMS. In order to use QPST, this function must be called with bUseQPST set to TRUE.
	Parameters:
	 bUseQPST  = true to use QPST, false to use QPHONEMS  

	Returns:
	void  
	**********************************************/
}

void CPhoneCtrl::GetLibraryVersion(char *Version_Buffer)
{
	QLIB_GetLibraryVersion(Version_Buffer); 
	return;
	/**********************************************
	Returns a NULL terminated string with the version information in the format: "QLIB VXX.yy.xx, <MODE>" where: XX is the major version #, yy is the sub- minor version, xx is the minor version number and <MODE> is either QPST or QPHONEMS, depending upon the library mode
	Examples: "QLIB V04.0.11,QPHONEMS" "QLIB V04.0.11,QPST"
	Parameters:
	 psVersion  = Buffer to store the version number, 25 bytes should be reserved for this buffer.  
	Returns:
	void
	**********************************************/
}

void CPhoneCtrl::GetLibraryCapabilities(unsigned char *SupportsDiag, unsigned char *SupportsEFS, unsigned char *SupportsSWDownload, unsigned char *UsingQPST)
{
	QLIB_GetLibraryCapabilities(SupportsDiag,SupportsEFS,SupportsSWDownload,UsingQPST);
	return;
	/**********************************************
    Returns flags indicating the capabilities of the library
	Parameters:
	 pbSupportsDiag  = true if diag/ftm commands are supported  
	 pbSupportsEFS  = true if EFS commands are supported  
	 pbSupportsSwDownload  = true if software download commands are supported  
	 pbUsingQPST  = true if QPST is being used, false if QPHONEMS is used 
	Returns:
	void 
	**********************************************/
}

HANDLE CPhoneCtrl::ConnectServerWithWait(unsigned int ComPort, unsigned long Wait_MS)
{
	return QLIB_ConnectServerWithWait(ComPort,Wait_MS);   
    /**********************************************
    Connect the server to a certain COM port number
	Parameters:
	 iComPort  = COM port number to use for communication with phone. This does not open the COM port at the operating system level until a call is made to the embedded target. 
	For auto detection, the COM port passed in can be QLIB_COM_AUTO_DETECT, and the first attached phone will be used automatically.
	Parameters:
	 iWait_ms  = # of milliseconds to wait before an error is considered. 
	Returns:
	HANDLE to use for all subsequent QLIB calls that require a handle. If NULL, then no valid phone was found.  
    **********************************************/
}

unsigned char CPhoneCtrl::GetComPortNumber(HANDLE PhoneHandle, unsigned short *ComPort)
{
	return QLIB_GetComPortNumber(PhoneHandle,ComPort);
	/**********************************************
    Returns the COM port number associated with a specific resource context. This can be used to determine the COM port number for a phone that was connected to automatically.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 piPhysicalPort  = output, physical port number, such as a 6 for COM6 
	Returns:
	true if COM port could be determined successfully 
	**********************************************/ 
}

void CPhoneCtrl::DisconnectAllServers()
{
	 QLIB_DisconnectAllServers();
	 return;
	 /**********************************************
	 Disconnects all servers and closes the COM ports associated with those server.
	 Normally, this function does not need to be called, because a call to QLIB_DisconnectServer() can be done for each reasource that is opened.
	 This function is to be used for cases when multiple connections are made to different COM ports during the lifetime of the application, or in the case of emergency/exception shut down.
	 Returns:
	 void
	 Warning:
	 All handles will be invalid after this function is called. 
     **********************************************/
}

unsigned char CPhoneCtrl::GetAvailablePhonesPortList(unsigned short *PortNumber, unsigned short *PortList, unsigned short IgnorePortNumber, unsigned short *IgnorePortList)
{
	return QLIB_GetAvailablePhonesPortList(PortNumber,PortList,IgnorePortNumber,IgnorePortList);   
    /**********************************************
    Uses GetAllPortList() to determine which ports have phones available, then uses attempts a connection on each of the ports returned.
	Parameters:
	 iNumPorts  = input/output, The input value is the maximum number of entries that can be added to the pPortList array. The output value is updated to reflect how many valid ports were found on the system. 
	 pPortList  = output, unsigned char array. Each unsigned char indicates a COM port number that is available on the system. 
	 iNumIgnorePorts  = # of ports in the "ignore port list." Zero to check all ports  
	 pIgnorePortList  = a list of port numbers that are to be ignored. This can speed up the auto detection of ports. 

	Returns:
	true if operation was able to complete successfully and at least one valid port is found.
	Warning:
	1) this function will take about 1 second for each port that is on the system
	2) All connections will be closed before searching begins.
	**********************************************/  
}

void CPhoneCtrl::ConfigureCallBacks(HANDLE PhoneHandle, EfsDirCB EfsDirCallBackFunc, generalSWDownloadCB GeneralSwDownloadCallBackFunc, asyncMessageCB AsyncMessageCallBackFunc)
{
	QLIB_ConfigureCallBacks(PhoneHandle,EfsDirCallBackFunc,GeneralSwDownloadCallBackFunc,AsyncMessageCallBackFunc);
	return;
    /**********************************************
	Configures call back functions.
	If one of the pointers is NULL, then the call back will be disabled for that call back type.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 pEfsDirCallback  = Callback for the EfsDirectory() operation  
	 pGeneralSwDownloadCB  = Callback for download events  
	 pAsyncMessageCB  = Callback for filtering async messages 
	Returns:
	None
	**********************************************/ 
}

void CPhoneCtrl::ConfigureEFSCallBacks(HANDLE PhoneHandle, EfsFileTransferCB EFSCallBack, EfsDirCB EFSDirCallBackFunc)
{
	QLIB_ConfigureEfs2CallBacks(PhoneHandle,EFSCallBack,EFSDirCallBackFunc);   
    return;
	/**********************************************
    Configures EFS2 call back functions.
	If one of the pointers is NULL, then the call back will be disabled for that call back type.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 pEfsCallback  = Callback for general EFS2 operations  
	 EfsDirCB  = Callback for EFS Directory operations 
	Returns:
	None 
	**********************************************/ 
}

unsigned char CPhoneCtrl::ConfigureTimeOut(HANDLE PhoneHandle, unsigned long TimeOutId, unsigned long NewTimeOutValue_MS)
{
	return QLIB_ConfigureTimeOut(PhoneHandle,TimeOutId,NewTimeOutValue_MS);
    /**********************************************
    Configures a specific timeout value.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 eTimeOutId  = Identifier index of timeout to change, index defined by QMSL_TimeOutType_Enum in QLibDefines.h  
	 iNewValue_ms  = number of millseconds for timeout value 
	Returns:
	None
	**********************************************/  
}

unsigned long CPhoneCtrl::GetTimeOut(HANDLE PhoneHandle, unsigned long TimeOutID)
{
	return QLIB_GetTimeOut(PhoneHandle,TimeOutID);  
    /**********************************************
    Get a specific timeout value.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 eTimeOutId  = Identifier index of timeout to change, index defined by QMSL_TimeOutType_Enum 
	Returns:
	timeout value in milliseconds
	None 
	**********************************************/
}

unsigned char CPhoneCtrl::IsPhoneConnected(HANDLE PhoneHandle)
{
	return QLIB_IsPhoneConnected(PhoneHandle);
	/**********************************************
	CDMA ICD, 3.4.123 Diagnostic Protocol Loopback Request/Response
	Pings the phone using the diagnostic command DIAG_PROTOCOL_LOOPBACK_F.
	Additionally verifies the connection using the dignostic version command DIAG_VERNO_F.
	This command has a timeout value that can be checked or modified using the identifier QMSL_Timeout_IsPhoneConnected when calling QLIB_ConfigureTimeOut() or QLIB_GetTimeOut()
	The default timeout is 200ms, so that as little time as possible will be spent to wait for a phone request in the event that a phone is not present. Other diagnostic functions use a longer timeout, so this function is the only one that should be called until a phone is verified to be on a certain COM port.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	Returns:
	Returns true if the phone is connected, false if it is not connected
	**********************************************/ 
}

unsigned char CPhoneCtrl::IsFTMMode(HANDLE PhoneHandle, unsigned char *FTMMode)
{
	return QLIB_IsFTM_Mode(PhoneHandle,FTMMode);
	/**********************************************
    The function checks whether the phone is in FTM mode.
	Depending on the command code ( FTM_COMMAND_59 or FTM_COMMAND75 ), this function uses different methods to check for FTM mode.
	Method 1: If the command code is set to FTM_COMMAND_59, this function reads the NV_FTM_MODE item to check whether the phone is in FTM mode.
	Note that MSM6000/6025/6050 and older MSM targets use FTM_COMMAND_59 command code for FTM mode transcation.
	Method 2: If the command code is set to FTM_COMMAND_75, this function issues a CM state info request to query the phone state. The command is CMLOG_STATE_INFO_F (refer 80-V1294-7, 3.2).
	General comment: The command code should be set by QLIB_FTM_SET_COMMAND_CODE function before this function is called. By default, the command code is FTM_COMMAND_75.
	Parameters:
	 pbIsFTMMode  = output, true(1) if the phone is in FTM mode, false(0) if the phone is not in FTM mode 
	Returns:
	true if the function is successful 
	**********************************************/ 
}

unsigned char CPhoneCtrl::SetCommandCode(HANDLE PhoneHandle, short CommandCode)
{
	return QLIB_FTM_SET_COMMAND_CODE(PhoneHandle,CommandCode);
	/**********************************************
    This function sets the command code (59 or 75);
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 eCommandMode  = Command code (59 or 75) 
	Note: The library uses command code 75 by default. MSM6000/6025/6050 uses commmand code 59. For MSM6000/6025/6050, this function should be called prior to other FTM functions
	Returns:
	true if successful, false otherwise
	**********************************************/  
}

unsigned char CPhoneCtrl::ChangeFTMBootMode(HANDLE PhoneHandle, unsigned char Mode_Flag, unsigned char Reset_Flag)
{
	return QLIB_ChangeFTM_BootMode(PhoneHandle,Mode_Flag,Reset_Flag);  
    /**********************************************
	QLIB_ChangeFTM_BootMode() is used to change into the FTM mode by first checking the NV_FTM_MODE_I state, then change the phone into the desired state using NV_FTM_MODE_I.
    The function changes phone to FTM or ONLINE mode through NV item NV_FTM_MODE method.
	This function:
	1. Changes phone to offline mode (QLIB_DIAG_CONTROL_F(MODE_OFFLINE_F)) 2. Delays by the timeout value QMSL_Timeout_Delay_OFFLINE, which can be set wiht QLIB_ConfigureTimeout() 2. Writes bFTMmode value to NV item NV_FTM_MODE (453)
	If (bReset == 1), the function returns immediately after reset command is issued.
	Parameters:
	 bFTMMode  = (1 = FTM mode), (0 = ONLINE mode)  
	 bReset  = (1 = Reset after NV write), (0 = No reset after NV write) 
	Returns:
	true if the function is successful
	**********************************************/  
}

unsigned char CPhoneCtrl::ChangeFTMModeRuntime(HANDLE PhoneHandle, unsigned char ModeFlag)
{
	return QLIB_ChangeFTM_ModeRuntime(PhoneHandle,ModeFlag); 
    /**********************************************
    The function changes the mode to ONLINE or FTM mode using run time method
	The function calls IsFTM_Mode to determines the phone is in FTM mode or not.
	If current mode is not equal to bFTMmode, it will switch mode.
	General comment: This function can't be used in MSM6000/6025/6050 and older MSM targets. As they use FTM_COMMAND_59 command code for FTM and DO NOT support runtime swtiching to FTM
	The command code should be set to FTM_COMMAND_75 by QLIB_FTM_SET_COMMAND_CODE function before this function is called.
	By default, the command code is FTM_COMMAND_75.
	Parameters:
	 bFTMMode  = (1 = FTM mode), (0 = ONLINE mode) 
	Returns:
	true if the function is successful 
	**********************************************/ 
}

unsigned char CPhoneCtrl::GetVersionInfo(HANDLE PhoneHandle,
		                         char CompileDate[11],
								 char CompileTime[8],
								 char ReleaseData[11],
								 char ReleaseTime[8],
								 char VersionDirectory[8],
								 unsigned char *StationClassMark,
								 unsigned char *MobileCAIRevision,
								 unsigned char *MobileModel,
								 unsigned short *MobileFirmwareRevision,
								 unsigned char *SlotCycleIndex,
								 unsigned char *VocoderMajorVersion,
								 unsigned char *VocoderMinorVersion)
{
	return QLIB_DIAG_VERNO_F(PhoneHandle,
		                     CompileDate,
							 CompileTime,
							 ReleaseData,
							 ReleaseTime,
							 VersionDirectory,
							 StationClassMark,
							 MobileCAIRevision,
							 MobileModel,
							 MobileFirmwareRevision,
							 SlotCycleIndex,
							 VocoderMajorVersion,
							 VocoderMinorVersion);
	/**********************************************
    DMSS ICD, 3.4.1 Version Number Request/Response WCDMA Diagnostic ICD, 3.5.15 Diag version request/response
	Return software version information
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 comp_date[  11] = Compile date Jun 11 1991  
	 comp_time[  8 ] = Compile time hh:mm:ss  
	 rel_date  [ 11] = Release date  
	 rel_time  [ 8 ] = Release time  
	 ver_dir  [ 8 ] = Version directory  
	 scm  = Station Class Mark  
	 mob_cai_rev  = CAI rev  
	 mob_model  = Mobile Model  
	 mob_firm_rev  = Firmware Rev  
	 slot_cycle_index  = Slot Cycle Index  
	 voc_maj  = Vocoder major version  
	 voc_min  = Vocoder minor version 
	Returns:
	true if successful, false if fail.
	Warning:
	None.
	**********************************************/  
}

unsigned char CPhoneCtrl::GetVersionInfoEXT(HANDLE PhoneHandle, unsigned long *MSMHWVersion, unsigned long *MobileModel, char *MobileSWRevision, char *ModelString)
{
	return QLIB_DIAG_EXT_BUILD_ID_F(PhoneHandle,MSMHWVersion,MobileModel,MobileSWRevision,ModelString);  
    /**********************************************
    DMSS ICD, 3.4.124.2 Extended Build ID Response Message
	This packet is an extension to the Version Number Request/Response packet (CMD_CODE 0)
	Parameters:
	 piMSM_HW_Version  = MSM version. This is an extension of the MSM_VER field from the Version Number Request/Response packet (DIAG_VER_NO_F)  
	 piMobModel  = Manufacturer's mobile model number. This is an extension of MOB_MODEL field from the Version Number Request/Response packet 
	 sMobSwRev  = Mobile software revision string. This string is a NULL-terminated ASCII string. If string is nonexistent, a NULL char indicates an empty string. This field is an extension of the VER_DIR field from the Version Number Request/Response packet 
	NOTE: Enough memory must be available at this pointer location to store the entire string. Recommend a buffer of 512 bytes.
	Parameters:
	 sModelStr  = Mobile model string. This string is an ASCII NULL-terminated string. If string is nonexistent, a NULL char indicates an empty string. 
	NOTE: Enough memory must be available at this pointer location to store the entire string. Recommend a buffer of 512 bytes.
	Returns:
	true if successful, false if fail.
	Warning: 
	**********************************************/
}

unsigned char CPhoneCtrl::GetQpstPhoneStatus(HANDLE PhoneHandle, unsigned long *PhoneStatus)
{
	return QLIB_GetQpstPhoneStatus(PhoneHandle,PhoneStatus); 
	/**********************************************
    Return the phone status, as reported by QPST Server.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 piStatus  = value of the current status, as defined by the SWD_enPhoneMode enumeration in SoftwareDownload.h 
	Returns:
	true if successful, false if fail.
	Warning:
	---this function is only valid when the library is in QPST mode
	**********************************************/  
}

unsigned char CPhoneCtrl::ModeSwitch(HANDLE PhoneHandle, int Mode)
{
	return QLIB_DIAG_CONTROL_F(PhoneHandle,Mode);
    /**********************************************
    DMSS Diagnostic ICD, 3.4.38 Mode Change Request/Response WCDMA Diagnostic ICD, 3.5.23 Mode Change Request/Response
	Changes the current system state.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 eMode  = Defined by mode_enum_type in QLib_Defines.h 
					MODE_OFFLINE_A_F = 0,   
					MODE_OFFLINE_D_F = 1,   
					MODE_RESET_F = 2,               
					MODE_FTM_F = 3,                 
					MODE_ONLINE_F = 4,              
					MODE_MAX_F = 5                  //!<' Last (and invalid) mode enum value
	Returns:
	Returns true if the phone mode change executed correctly
	Warning:
	, If the new mode is MODE_FTM_F, a delay will be issued after the command is successfully sent to the mobile.
	The duration of the delay is based on the value of delay type "QMSL_Timeout_Delay_ONLINE_FTM" which can be controled by QLIB_ConfigureTimeOut() and read by QLIB_GetTimeOut()  

    **************
    There are two ways to put the device into FTM mode:

    The first is to call QLIB_DIAG_CONTROL_F(), to transfer immediately from ONLINE mode to FTM mode at run-time. 
	This is the preferred method because the transition can be done in less than a second and without rebooting. 
	This technique is not available on the MSM6000, MSM6050™, or MSM6025™, but it will be enabled for the SE-1X chipsets.
    QLIB_DIAG_CONTROL_F() is used to set the phone into FTM, Online, and Offline modes. The mode parameter is controlled.
	Once the phone is in Offline mode, it can only be changed to Reset mode, not to FTM
    or Online. 
	typedef enum 
	{
		MODE_OFFLINE_A_F = 0,  //!<' Go to offline analog
		MODE_OFFLINE_D_F = 1,  //!<' Go to offline digital 
		MODE_RESET_F = 2,      //!<' Reset. Only exit from offline 
		MODE_FTM_F = 3,        //!<' FTM mode
		MODE_ONLINE_F = 4,     //!<' Go to Online 
		MODE_LPM_F = 5,        //!<' Low Power Mode (if supported)
		MODE_POWER_OFF_F = 6,  //!<' Power off (if supported)
		MODE_MAX_F = 7         //!<' Last (and invalid) mode enum value

	} mode_enum_type;

    The second way to enter FTM mode is to set the NV value of item 453, NV_FTM_MODE_I to a value of 1 and reset the phone. 
	To boot into ONLINE mode, this NV item must be a value of zero.   This is also referred to as the "Boot Mode" of the phone.
	NV items cannot be written in Online mode, but they can be written in FTM mode. 
	**********************************************/
}

unsigned char CPhoneCtrl::NVItemWrite(HANDLE PhoneHandle, unsigned short ItemID, unsigned char *ItemData, int ItemLength, unsigned short *Status)
{
	return QLIB_DIAG_NV_WRITE_F(PhoneHandle,ItemID,ItemData,ItemLength,Status);   
    /**********************************************
	NV items cannot be written in Online mode, but they can be written in FTM mode. 
    DMSS ICD, 3.4.36 Nonvolatile Item Write Request/Response WCDMA Diagnostic ICD, 3.5.22 Nonvolatile Item Write Request/Response
	Write an NV Item
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iItemID  = ID of NV Item  
	 pItemData  = Reference to storage area for data  
	 iLength  = size of data, in unsigned chars  
	 iStatus  = Reference to storage area for status 
		 NV_DONE_S = 0,      // Request completed okay 
		 NV_BUSY_S = 1,      // Request is queued 
		 NV_BADCMD_S = 2,    // Unrecognizable command field 
		 NV_FULL_S = 3,      // The NVM is full 
		 NV_FAIL_S = 4,      // Command failed, reason other than NVM was full 
		 NV_NOTACTIVE_S = 5, // Variable was not active 
		 NV_BADPARM_S = 6,   // Bad parameter in command block 
		 NV_READONLY_S = 7,  // Parameter is write-protected and thus read only 
		 NV_BADTG_S = 8,     // Item not valid for Target 
		 NV_NOMEM_S = 9,     // free memory exhausted 
		 NV_NOTALLOC_S = 10  // address is not a valid allocation 
	Returns:
	true if successful, false if fail.
	Warning:
	None.  
	**********************************************/
}

unsigned char CPhoneCtrl::NVItemRead(HANDLE PhoneHandle, unsigned short ItemID, unsigned char *ItemData, int ItemLength, unsigned short *Status)
{
	return QLIB_DIAG_NV_READ_F(PhoneHandle,ItemID,ItemData,ItemLength,Status);
    /**********************************************
	DMSS ICD, 3.4.35 Nonvolatile Item Read Request/Response WCDMA Diagnostic ICD, 3.5.23 Mode Change Request/Response
	Read an NV Item
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 item_id  = Enumeration ID of NV Item  
	 pItemData  = Reference to storage area for data  
	 iLength  = size of data, in unsigned chars  
	 iStatus  = Reference to storage area for status 
		 NV_DONE_S = 0,      // Request completed okay 
		 NV_BUSY_S = 1,      // Request is queued 
		 NV_BADCMD_S = 2,    // Unrecognizable command field 
		 NV_FULL_S = 3,      // The NVM is full 
		 NV_FAIL_S = 4,      // Command failed, reason other than NVM was full 
		 NV_NOTACTIVE_S = 5, // Variable was not active 
		 NV_BADPARM_S = 6,   // Bad parameter in command block 
		 NV_READONLY_S = 7,  // Parameter is write-protected and thus read only 
		 NV_BADTG_S = 8,     // Item not valid for Target 
		 NV_NOMEM_S = 9,     // free memory exhausted 
		 NV_NOTALLOC_S = 10  // address is not a valid allocation 
	Returns:
	true if successful, false if fail.
	Warning:
	None.
	**********************************************/  
}

unsigned char CPhoneCtrl::SendSPC(HANDLE PhoneHandle, unsigned char SPC[], unsigned char *SPCResult)
{
	return QLIB_DIAG_SPC_F(PhoneHandle,SPC,SPCResult); 
    /**********************************************
    DMSS ICD, 3.4.63 Service Programming Code Request/Response WCDMA Diagnostic ICD, 3.5.29.3 Service Programming Code Request Message
	Sends the service programming code (SPC) to the phone.
	The SPC is a 6 digit number, which can enable certain operations, according to the DMSS Security Plan.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iSPC  = array of 6 bytes, which hold the service programming code 6 bytes of security code, in ASCII format (digits 0..9 only)  
	 piSPC_Result  = Result of the SPC 
							1 = code was correct and Service Programming (SP) is unlocked
							0 = code was incorrect and SP is still locked
	Returns:
	true if successful, false if fail.
	Warning:
	if the code was incorrect, the phone will time out for 10 sec before responding to any more requests through the serial interface.  
	**********************************************/
}

unsigned char CPhoneCtrl::QueryPhoneStatus(HANDLE PhoneHandle, unsigned char *State)
{
	return QLIB_DIAG_STATUS_F(PhoneHandle,State);
	/**********************************************
	WCDMA ICD, 3.4.9 Status request message (cmd_code 14 of the WCDMA subsystem)
	Returns the phone status--offline, online, other
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 piTMC_State  = Return value of TMC state: 
			0  = DIAG_STATUS_OFFLINE =  Offline
			1  = DIAG_STATUS_ONLINE  =  Online
			2  = DIAG_STATUS_LPM     =  Low power mode
			99 = DIAG_STATUS_UNKNOWN =  Unknown state
	Returns:
	true if successful, false if fail.
	Warning: 
	**********************************************/
}

unsigned char CPhoneCtrl::SecurityFreeze(HANDLE PhoneHandle)
{
	return QLIB_DIAG_SECURITY_FREEZE_F(PhoneHandle);
    /**********************************************
	Sirius custom diag command
	Sends the trigger for the security freeze step to be executed on the mobile
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer() 
	Returns:
	true if successful, false if fail.
	Warning:
	**********************************************/ 
}

unsigned char CPhoneCtrl::BackupNVToCNV(HANDLE PhoneHandle)
{
	return QLIB_DIAG_NV_CNV_DIAG_BACKUP_F(PhoneHandle);
    /**********************************************
	Start CNV backup
	If the CNV backup has been triggered by a DIAG command, the response to the DIAG command will tell the PC if the backup was successful or not (If it's SWUOTA triggered F3 messages are all that can give status of CNV backup). There are 2 responses to the DIAG command, first reports on the status if the command has been successfully queued in CNV, and the second response reports on the status of the backup.
	NV_CNV_DIAG_BACKUP: Command sent from the DM to the DMSS to backup the NV data to CNV.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer() 
	Returns:
	true if successful, false if fail.
	**********************************************/  
}

unsigned char CPhoneCtrl::ReadESN(HANDLE PhoneHandle, unsigned long *ESN)
{
	return QLIB_DIAG_READ_ESN_F(PhoneHandle,ESN);
	/**********************************************
	Read ESN, special case of DIAG_NV_READ_F
	Returns the value of the NV_ESN_I NV field, item #0.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 piESN  = Pointer to the location where the ESN will be stored 
	Returns:
	true if successful, false if fail.
	Warning:
	**********************************************/
}

unsigned char CPhoneCtrl::UploadQCNFile(HANDLE PhoneHandle,
										char* QCNFileName, 
										char* Service_Programming_Code,  
										unsigned char AutoRestore,  
										unsigned char SkipReset,  
										unsigned char  AllowEsnMismatch,  
										unsigned char  IgnorePhoneModel)
{
	return QLIB_UploadQcnFile(PhoneHandle,
							  QCNFileName, 
							  Service_Programming_Code,  
							  AutoRestore,  
							  SkipReset,  
							  AllowEsnMismatch,  
							  IgnorePhoneModel);  
    /**********************************************
    Upload a QCN (NV memory contents) file from the PC to the phone.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 sFileName  = Path and file name of QCN file  
	 sSPC  = Service provider code, usually "000000"  
	 bAutoRestore  = true always, when sending a QCN file to the phone  
	 bSkipReset  = true to not reset after QCN file uploaded, false to force reset after QCN file is uploaded  
	 bAllowEsnMismatch  = true to allow the ESN to be mismatched between the QCN file and the phone  
	 bIgnorePhoneModel  = true to allow the phone model number to be mismatched between the QCN file and the phone 
	Returns:
	true if successful, false if fail.
	Warning:
	**********************************************/ 
}

unsigned char CPhoneCtrl::DownloadQCNFile(HANDLE PhoneHandle, char *QCNFileName, char *SPC)
{
	return QLIB_DownloadQcnFile(PhoneHandle,QCNFileName,SPC);
    /**********************************************
	Download a QCN (NV memory contents) file from the phone to the PC.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 sFileName  = Path and file name of QCN file  
	 sSPC  = Service provider code, usually "000000" 

	Returns:
	true if successful, false if fail.
	Warning:
	**********************************************/
}

unsigned char CPhoneCtrl::UploadCEFSFile(HANDLE PhoneHandle, char *CEFSFileName, char *PartitionFileName)
{
	return QLIB_UploadCEFS_File(PhoneHandle,CEFSFileName,PartitionFileName);
    /**********************************************
	Upload a CEFS file from the PC to the phone.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 sFileName  = the local path and filename of the CEFS file to upload. 
	Returns:
	true if successful, false if fail.
	Warning:
	**********************************************/
}

unsigned char CPhoneCtrl::UploadCEFSFile2(HANDLE PhoneHandle, char *CEFSFileName, char *PartitionFileName, long ARMPRG_Type)
{
	return QLIB_UploadCEFS_File2(PhoneHandle,CEFSFileName,PartitionFileName,ARMPRG_Type);
    /**********************************************
	Upload a CEFS file from the PC to the phone, specifying the ARMPROG file, for use when the phone is already in download mode.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 sFileName  = the local path and filename of the CEFS file to upload.  
	 iDloadArmprg  = if bUseCustomArmprg = false, then ID of the ARMPRG to use refer to the enumeration, dloadArmprgType 
			From QPST code, SharedInc/ArmprgType.h:
			typedef enum            // Default armprg to use when mobile alread in download mode.
			{
			  apStandard    = 0,    // MSM3000, 3100, 3300, 5000, 5010, 5100, 5105
			  apMSM6000     = 1,    // MSM6000
			  apMSM6050     = 2,    // MSM6050
			  apMSM6200_1   = 3,    // MSM6200 Cheetah
			  apMSM6100     = 4,    // MSM6100 Cougar with NOR flash
			  apMSM6100NAND = 5,    // MSM6100 Cougar with NAND flash.
			  apMSM6250     = 6,    // MSM6250 (Saber) NOR flash
			  apMSM6250NAND = 7,    // MSM6250 (Saber) NAND flash
			  apMSM6500     = 8,    // MSM6500 (Jaguar) NOR flash
			  apMSM6500NAND = 9,    // MSM6500 (Jaguar) NAND flash
			  apMSM6100NORFFA=10,   // MSM6100 FFA with NOR flash
			  apMSM6550     = 11,   // MSM6550 (Eagle) NOR flash
			  apMSM6550NAND = 12,   // MSM6550 NAND (3/9/05)
			  apMSM6550PB   = 13,   // MSM6550 progressive boot
			  apEM6700      = 14,   // MSM6700 emulator (Quasar) 
			  apMSM6275NOR  = 15,   // MSM6275 (Raven) NOR
			  apMSM6275NAND = 16,   // MSM6275 (Raven) NAND
			  apMSM6800NOR  = 17,   // MSM6800 (DoRA) NOR
			  apMSM6800NAND = 18,   // MSM6800 (DoRA) NAND
			  apMSM6250Sec  = 19,   // MSM6250 secure (one-time programmable)
			  apMSM7500NAND = 20,   // MSM7500 (Phoenix)
			  apMSMSC1x     = 21,   // SC1x a.k.a. QSC, NOR progressive
			  amMSM6280NAND = 22,   // MSM6280 NAND multi-image

			  apMarkerForDialog // Don't use!
			} dloadArmprgType ;
	Returns:
	true if successful, false if fail.
	Warning:
	This function is grouped with the EFS functions, not the download functions, so it is important to check for errors in the EFS area, not Software download. 
	**********************************************/
}

void CPhoneCtrl::GetLastSoftwareDownloadErrorInfo(HANDLE  PhoneHandle,  
										          unsigned char* ErrorOccurred,  
												  char* ErrorMessage,  
										          int MaxStringLength)
{
	QLIB_GetLastSoftwareDownloadErrorInfo(PhoneHandle,  
										  ErrorOccurred,  
										  ErrorMessage,  
										  MaxStringLength);
	/**********************************************
    Get the error status and description of the last software download operation.
	Parameters:
	 bErrorOccurred  = true if an error occurred during the last operation. False if no error occurred during the last operation. 
	 sErrorMessage  = Filled filled in with a text description of the most recent error. Empty if no error has occurred. 
	 iMaxStringLength  = Maximum number of characters available in the sErrorMessage buffer 
	Returns:
	true if successful, false if failure
	Warning:
	The error information will be cleared after this is called.
	**********************************************/  
}

unsigned char CPhoneCtrl::UploadMultiImage_OBL(HANDLE PhoneHandle,  
											   char* PartitionFileName,  
											   char* OTBPL_FileName,  
											   char* PBL_FileName,  
											   char* QCSBL_FileName,  
											   char* QCSBL_HeaderName,  
											   char* OEMSBL_FileName,  
											   char* OEMSBL_HeaderName,  
											   char* ModemFileName,  
											   char* ModemHeaderName,  
											   char* AppsFileName,  
											   char* AppsHeaderName,  
											   char* AppsBlFileName,  
											   char* AppsBlHeaderFileName,  
											   unsigned char OverridePrtnTable,  
											   unsigned char UseTrustedMode,  
											   char*  Service_Programming_Code,  
											   unsigned char  ClearErrorLog,  
											   char*  StorageFile,  
											   unsigned char OverrideModelCheck,  
											   unsigned char SkipReset,  
											   unsigned char UseCustomArmprg,  
											   long  DloadArmprg,  
											   long  DownloadBitMask)
{
	return QLIB_UploadMultiImage(PhoneHandle,  
								PartitionFileName,  
								OTBPL_FileName,  
								PBL_FileName,  
								QCSBL_FileName,  
								QCSBL_HeaderName,  
								OEMSBL_FileName,  
								OEMSBL_HeaderName,  
								ModemFileName,  
								ModemHeaderName,  
								AppsFileName,  
								AppsHeaderName,  
								AppsBlFileName,  
								AppsBlHeaderFileName,  
								OverridePrtnTable,  
								UseTrustedMode,  
								Service_Programming_Code,  
								ClearErrorLog,  
								StorageFile,  
								OverrideModelCheck,  
								SkipReset,  
								UseCustomArmprg,  
								DloadArmprg,  
								DownloadBitMask);
   /**********************************************
   Upload a multi-image software image from the PC to the phone. This function performs an "OBL" download, which is equivalent to the QPST Software Download application.
	For the following string inputs DON'T PASS NULL's, use "". To control which files will be sent, use the paraemter, iDownloadBitMask.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 sPartitionFileName  = partition path and file name, usually "partition.mbn"  
	 sOTBPL_FileName  = OTBPL path and file name, usually "obl.mbn"  
	 sPBL_FileName  = Primary Boot Loader, usually "pbl.mbn"  
	 sQCSBL_FileName  = secure boot loader, usually "qcsblhd_cfgdata.mbn"  
	 sQCSBL_HeaderName  = secure boot loader header, usually "qcsbl.mbn"  
	 sOEMSBL_FileName  = OEM secure boot loader, usually "oemsbl.mbn"  
	 sOEMSBL_HeaderName  = OEM secure boot loader header, usually "oemsblhd.mbn"  
	 sModemFileName  = AMSS File, usually "amss.mbn"  
	 sModemHeaderName  = AMSS Header, usually "amsshd.mbn"  
	 sAppsFileName  = Application file  
	 sAppsHeaderName  = Application header file  
	 char  bOverridePrtnTable = TRUE to overwrite the stored partition table in the phone  
	 char  bUseTrustedMode = TRUE to use trusted mode and the AMSS has a built-in PBL. If FALSE, then PBL must be provided. 
	 sSPC  = Service Programming Code, usually "000000"  
	 bClearErrorLog  = TRUE to clear the QPST error log  
	 sStorageFile  = file name of temporary QCN file  
	 bOverrideModelCheck  = TRUE to override a model number check  
	 bSkipReset  = TRUEto skip the reset operation after the phone is programmed  
	 bUseCustomArmprg  = FALSE to let QPST server determine which ARMPRG to bet used. If TRUE, then the NPRG or ARMPRG will be searched for in the same folder as the modem file name.  
	 iDloadArmprg  = if bUseCustomArmprg = false, then ID of the ARMPRG to use refer to the enumeration, dloadArmprgType  
	 iDownloadBitMask  = bitmask of files to download, based on SWD_miFileTypeBitmask_enum 
	Returns:
	true if successful, false if fail.
	Warning:
	**********************************************/ 
}

unsigned char CPhoneCtrl::UploadMultiImage(HANDLE  PhoneHandle,  
										   char* PartitionFileName,  
										   char* PBL_FileName,  
										   char* QCSBL_FileName,  
										   char* QCSBL_HeaderName,  
										   char* OEMSBL_FileName,  
										   char* OEMSBL_HeaderName,  
										   char* ModemFileName,  
										   char* ModemHeaderName,  
										   char* AppsFileName,  
										   char* AppsHeaderName,  
										   char* AppsBlFileName,  
										   char* AppsBlHeaderFileName,  
										   unsigned char OverridePrtnTable,  
										   unsigned char UseTrustedMode,  
										   char* Service_Programming_Code,  
										   unsigned char ClearErrorLog,  
										   char* StorageFile,  
										   unsigned char OverrideModelCheck,  
										   unsigned char SkipReset,  
										   unsigned char UseCustomArmprg,  
										   long  DloadArmprg,  
										   long  DownloadBitMask)
{
	return QLIB_UploadMultiImage2(PhoneHandle,  
									PartitionFileName,  
									PBL_FileName,  
									QCSBL_FileName,  
									QCSBL_HeaderName,  
									OEMSBL_FileName,  
									OEMSBL_HeaderName,  
									ModemFileName,  
									ModemHeaderName,  
									AppsFileName,  
									AppsHeaderName,  
									AppsBlFileName,  
									AppsBlHeaderFileName,  
									OverridePrtnTable,  
									UseTrustedMode,  
									Service_Programming_Code,  
									ClearErrorLog,  
									StorageFile,  
									OverrideModelCheck,  
									SkipReset,  
									UseCustomArmprg,  
									DloadArmprg,  
									DownloadBitMask); 
   /**********************************************
   Upload a multi-image software image from the PC to the phone. This function is equivalent to the "Multi-image" tab in the QPST Software Download application.
	For the following string inputs DON'T PASS NULL's, use "". To control which files will be sent, use the paraemter, iDownloadBitMask.


	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 sPartitionFileName  = partition path and file name, usually "partition.mbn"  
	 sPBL_FileName  = Primary Boot Loader, usually "pbl.mbn"  
	 sQCSBL_FileName  = secure boot loader, usually "qcsblhd_cfgdata.mbn"  
	 sQCSBL_HeaderName  = secure boot loader header, usually "qcsbl.mbn"  
	 sOEMSBL_FileName  = OEM secure boot loader, usually "oemsbl.mbn"  
	 sOEMSBL_HeaderName  = OEM secure boot loader header, usually "oemsblhd.mbn"  
	 sModemFileName  = AMSS File, usually "amss.mbn"  
	 sModemHeaderName  = AMSS Header, usually "amsshd.mbn"  
	 sAppsFileName  = Application file  
	 sAppsHeaderName  = Application header file  
	 sAppsBlFileName  = Application boot loader file  
	 sAppsBlHeaderFileName  = Application boot loader header file  
	 char  bOverridePrtnTable = TRUE to overwrite the stored partition table in the phone  
	 char  bUseTrustedMode = TRUE to use trusted mode and the AMSS has a built-in PBL. If FALSE, then PBL must be provided. 
	 sSPC  = Service Programming Code, usually "000000"  
	 bClearErrorLog  = TRUE to clear the QPST error log  
	 sStorageFile  = file name of temporary QCN file  
	 bOverrideModelCheck  = TRUE to override a model number check  
	 bSkipReset  = TRUEto skip the reset operation after the phone is programmed  
	 bUseCustomArmprg  = FALSE to let QPST server determine which ARMPRG to bet used. If TRUE, then the NPRG or ARMPRG will be searched for in the same folder as the modem file name.  
	 iDloadArmprg  = if bUseCustomArmprg = false, then ID of the ARMPRG to use refer to the enumeration, dloadArmprgType  
	 iDownloadBitMask  = bitmask of files to download, based on SWD_miFileTypeBitmask_enum 

	Returns:
	true if successful, false if fail.
	Warning: 
	**********************************************/
}

unsigned char CPhoneCtrl::WriteSPCToPhone(HANDLE Phone_Handle, unsigned char SPC_Code[], unsigned char *SPC_Result)
{
	return QLIB_DIAG_SPC_F(Phone_Handle, SPC_Code, SPC_Result);
	/**********************************************
    DMSS ICD, 3.4.63 Service Programming Code Request/Response WCDMA Diagnostic ICD, 3.5.29.3 Service Programming Code Request Message
	Sends the service programming code (SPC) to the phone.
	The SPC is a 6 digit number, which can enable certain operations, according to the DMSS Security Plan.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iSPC  = array of 6 bytes, which hold the service programming code 6 bytes of security code, in ASCII format (digits 0..9 only)  
	 piSPC_Result  = Result of the SPC 
							1 = code was correct and Service Programming (SP) is unlocked
							0 = code was incorrect and SP is still locked
	Returns:
	true if successful, false if fail.
	Warning:
	if the code was incorrect, the phone will time out for 10 sec before responding to any more requests through the serial interface.  
    **********************************************/

}

unsigned char CPhoneCtrl::FactoryAreaReadWrite(HANDLE Phone_Handle,unsigned char Read_Write_Flag,unsigned short Address,unsigned char *Factory_Data)
{
	return QLIB_EXT_FTM_READ_WRITE_FACTORY_DATA(Phone_Handle,Read_Write_Flag,Address,Factory_Data);
	/******************************************************************************
	Factory Test Mode Production, 1.13 Read/Write Factory Data
	Read or write one unsigned char of factory data to a persistent area of memory. 
	The factory data will be a total of 512 unsigned chars in size and spread across 4 NV 
	items that are 128 unsigned chars each. It is possible to read and write the NV items 
	one at a time, but this FTM function provides two levels of optimization. 
	First, the packet sizes for the FTM command are significantly smaller than 
	those for an NV READ/WRITE operation. Secondly, this operation handles the 
	physical mapping of an factory data address (0-511) to an NV item and an offset 
	inside the NV item.
	param hResourceContext = Resource context that was returned from the call to ConnectServer()
	param iOperationType:
		code
				0 = read data, transfer unsigned char from phone to PC,
				1 = write, transfer unsigned char from PC to phone.
		endcode
	param iDataAddress = Address of unsigned char that was read. The valid range is 0 to 511.. 
	param piDataValue = Applicable only for a "write" operation. This is the data value that 
	 will be stored at the address indicated by iDataAddress
	return true if successful, false if fail.
	warning 
	*******************************************************************************/
}

unsigned char CPhoneCtrl::RootKeyWrite(HANDLE Phone_Handle, unsigned char SlotId, unsigned char SIM_AppIdLen, unsigned char *pSIM_AppId, unsigned long RootKeyLen, unsigned char *pRootKeyData, unsigned long *pCommandStatus, unsigned long *pStatus)
{
	return QLIB_GSDIDIAG_ROOT_KEY_WRITE_CMD(Phone_Handle,SlotId,SIM_AppIdLen,pSIM_AppId,RootKeyLen,pRootKeyData,pCommandStatus,pStatus);   
    /******************************************************************************
    GDSI ICD, 4.16 DELAYED RESPONSE: GSDIDIAG_ROOT_KEY_WRITE_CMD
	This function is used to write the root key to the FLASH.
	This function handles the delayed response message and the return values are taken from delayed asynchronous message, not the first response message.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iSlotID  = Input, SLOT_ID - Slot number for the SIM for which this information needs retrieved. Values applicable to dual slot targets are: 
				0x01 - SLOT 1
				0x02 - SLOT 2
	For single slot targets, slot number should be None. 
	 iSIM_AppIdLen  = Input, Length of the Application ID from which to retrieve information; set to Length = 0 when not used. Be sure to set this to 0 when not used. Optional and applicable only to USIM. For a USIM, this implies that the USIM ADF is to be selected by default as necessary. Note: When using GSM cards and RUIM cards, set this to 0. 
	 piSIM_AppId  = Input, Optional and applicable only to USIM. USIM application identifier (APP ID); should only be used when the Card is a UICC with application directory files (ADFs) The APP IDs are retrieved from the USIM file EF-DIR. Set bytes [1 to 16] to 0x00 for GSM cards and RUIM cards. 
	If this is not used, then set value to NULL, otherwise, must be 16 bytes long.
	Parameters:
	 iRootKeyLen  = Input, # of bytes used by the root key,  
	 piRootKeyData  = Input, storage area of the root key, must be a valid pointer to "iRootKeyLen" number of bytes. 
	 piCommandStatus  = Output, Command status is set to GSDI_SUCCESS if APS_AS_TRANSPORT is set to 1.Otherwise it is set to the status returned by GSDI 
	This value is not valid if the function returns a false.
	Parameters:
	 piStatus  = Output, status code returned by the delayed response GSDI. These are the codes returned in the CDM_STATUS fields of Events. 
	This value is not valid if the function returns a false. 
			 0      - COMMAND COMPLETED SUCCESSFULLY
			 1      - GENERAL ERROR:  No other information specified
			 2      - ACCESS DENIED:  Security Procedures performed thus far don't satisfy restrictions
			 3      - NOT FOUND:  SIM Elementary File or Directory File was not found.
			 4      - INCOMPATIBLE PIN STATUS:  Trying to perform a PIN Action for a PIN that is not in the correct state.  
						  For example, trying to Disable an already Disabled PIN.
			 5      - INCORRECT CODE:  The Code provided is incorrect for the PIN ID used.
			 6      - CODE BLOCKED:  The Number of retries has been exceeded and the PIN is now blocked.
			 7      - INCREASE IMPOSSIBLE:  The INCREASE Command has failed due to a Bad Value
			 8      - INCORRECT PARAMETERS:  The SIM detected an incorrect parameter for the action performed.
			 9      - NOT SUPPORTED:  Not allowed by GSDI due to Service Table Values
			 10     - NOT INIT:  Cache has not been INIT
			 11     - SUCCESS BUT ILLEGAL SIM:  The SIM has been marked as an illegal SIM for the network, 
						  but the actions are succeeding on the SIM.
			 12     - RESERVED
			 13     - RESERVED
			 14     - SIM TECHNICAL PROBLEMS:  The SIM malfunctioned.
			 15     - NO EF SELECTED:  Attempted to read a file without fist selecting it.
			 16     - EF INCONSISTENT:  Trying to Read a Record when the file is a Binary or similar type error.
			 17     - RESERVED
			 18     - UNKNOWN INST CLASS:  Trying to send a UICC Command to a GSM/RUIM Card or vice versa.
			 19     - RESERVED
			 20     - RESERVED
			 21     - INCORRECT LENGTH:  A Bad length was provided in the command.
			 22     - DRIVERS TIMED OUT:  The Drivers communicating with the Card Timed Out.
			 23     - CODE PERM BLOCKED:  The Unblock Code has been blocked.  Need Administrative assistance to unblock it.
			 24     - RESERVED
			 25     - RESERVED
			 26     - RESERVED
	Returns:
	true if successful, false if fail.
******************************************************************************/ 


}

unsigned char CPhoneCtrl::SaveIMEIFSNToFlash(HANDLE Phone_Handle, unsigned char SlotID, unsigned char SIM_AppIDLen, unsigned char *SIM_AppID, unsigned long *pCommandStatus, unsigned long *pStatus)
{
	return QLIB_GSDIDIAG_SECURE_DEVICE_INFO_CMD(Phone_Handle,SlotID,SIM_AppIDLen,SIM_AppID,pCommandStatus,pStatus);   
    /******************************************************************************
	GDSI ICD, 1.2.17 DELAYED RESPONSE: GSDIDIAG_SECURE_DEVICE_INFO_CMD
	This function is used to save IMEI, Serial Number to the Flash and perform the activation procedure.
	This function handles the delayed response message and the return values are taken from delayed asynchronous message, not the first response message.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iSlotID  = input,  
	 iSIM_AppIdLen  = input,  
	 piSIM_AppId  = input,  
	 piCommandStatus  = Output,  
	 piStatus  = Output,  
	Returns:
	true if successful, false if fail.
	Warning:
	******************************************************************************/ 
}

unsigned char CPhoneCtrl::WriteSIMDataToPhone(HANDLE Phone_Handle,
											  unsigned char Slot_ID,
											  unsigned char SIM_AppIdLength,
											  unsigned char *pSIM_AppId,
											  unsigned long SIM_SecureDataLength,
											  unsigned char *pSIM_Data,
											  unsigned long *pCommandStatus,
											  unsigned long *pStatus)
{
	return QLIB_GSDIDIAG_SIM_SECURE_STORE_CMD(Phone_Handle,
											  Slot_ID,
											  SIM_AppIdLength,
											  pSIM_AppId,
											  SIM_SecureDataLength,
											  pSIM_Data,
											  pCommandStatus,
											  pStatus);
	/******************************************************************************
	GDSI ICD, 4.15 DELAYED RESPONSE: GSDIDIAG_SIM_SECURE_STORE_CMD
	This function is used to store encrypted SIM lock data.
	This function handles the delayed response message and the return values are taken from delayed asynchronous message, not the first response message.
	Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iSlotID  = input,  
	 iSIM_AppIdLen  = input,  
	 piSIM_AppId  = input,  
	 iSIM_SecureDataLen  = input, size of sim secure buffer. At most 1360 bytes  
	 piSIM_SecureData  = input, buffer of SIM secure data, must be a minimum of "iSIM_SecureDataLen" bytes, at most 1360 bytes 
	 piCommandStatus  = Output,  
	 piStatus  = Output,  
	Returns:
	true if successful, false if fail.
	Warning: 
	******************************************************************************/
}

void CPhoneCtrl::DisConnect_Server(HANDLE PhoneHandle)
{
	QLIB_DisconnectServer(PhoneHandle);
	return;
    //************DESCRIPTION**********************///
	/*
	Disconnect the server and close the COM port associated with the server.
	Parameters:
	hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer() 

	Returns:
	void  
	*/
    //*********************************************//
}


unsigned char CPhoneCtrl::VibratorOnOff(HANDLE PhoneHandle,unsigned char OnOffFlag)
{
	if(OnOffFlag)//on
	{
		return QLIB_FTM_SET_PMIC_DRIVE_LEVEL(PhoneHandle,2,18);
	}
	else//off
		return QLIB_FTM_SET_PMIC_DRIVE_LEVEL(PhoneHandle,2,0);

	 //************DESCRIPTION**********************///
	 /*
	 Factory Test Mode PMIC Commands 8.1 Set PMIC Pin Drive Level

	 The PMIC has three high strength drive pins that are connected to the LCD backlight (LCD_DRV pin), the keypad LEDs (KPD_DRV pin), and the vibrator (VIB_DRV pin). Each pin has a programmable drive level as described below.
	 FTM_SET_PMIC_DRIVE_LEVEL allows selection of one of three high drive pins and selection of the drive level. A drive level selection of 0 will turn off the pin drive.

     Parameters:
	 hResourceContext  = Resource context that was returned from the call to QLIB_ConnectServer()  
	 iHighDrivePinID  = Selects pin for drive level programming, values for this field are: 
				  0 = LCD_DRV pin
				  1 = KPD_DRV pin
				  2 = VIB_DRV pin

	  iDriveLevel  = Programs drive level for selected pin: 
				  a) LCD_DRV pin current = drive_level * 10mA.  Valid range is 0-15
				  b) KPD_DRV pin current = drive_level * 10mA.  Valid range is 0-15
				  c) VIB_DRV pin voltage = drive_level  * 100mV.  Valid range is 0 (off) or 12-31.
	 Returns:
	 true if successful, false if fail. 
     */
     //*********************************************//
}
