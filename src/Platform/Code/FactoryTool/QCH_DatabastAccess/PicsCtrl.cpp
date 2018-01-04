// PicsCtrl.cpp: implementation of the CPicsCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PicsCtrl.h"
#include <atlbase.h>
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\LogPolicy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace QSYNC_LIB;

CPicsCtrl::CPicsCtrl()
{
	::CoInitialize(NULL);
}

CPicsCtrl::~CPicsCtrl()
{
	::CoUninitialize();
}

unsigned char CPicsCtrl::Initial_PICSInterfaceDLL()
{
	CLSID clsid;
	unsigned char nRes = S_FALSE;
	HRESULT hresult=CLSIDFromProgID(OLESTR("PICSInterface.clsPICSInterface"), &clsid);
	hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_clsPICSInterface),(LPVOID *) &PICS_Interface);
	if(FAILED(hresult))
	{
		//Message_String="Register PICS Interface DLL fail!!";
		//AfxMessageBox(Message_String);
		//return S_FALSE;
	}
	else
	{
		CheckPlatformInPICS("Qualcomm");
		nRes = (unsigned char)ConnectPICS();
	}

	if (nRes != S_OK)
	{
		CString strFileName = _T("IMEIStationToolLog.txt");
		LogPolicy::Log("Connect PICS fail!", strFileName);
	}
	return nRes;
}

//HRESULT CPicsCtrl::CreateNewPICSObject(const char *Server, int SupportMultithread, LCID Local, int Reserved, CAObjHandle *ObjectHandle)
//{
	//Server="\0",supportMultithreading=0,locale=LOCALE_NEUTRAL,Reserved=0
	//return 1000;
	//return PICSInterface_New_clsPICSInterface(Server,SupportMultithread,Local,Reserved,ObjectHandle);
    /*************************************************************
	Use this function to create a new _clsPICSInterface object, 
	and obtain a handle to the object.

	If the server application is already running, this function may 
	or may not start another copy of the application.  
	This is determined by the server application.

	You must call CA_InitActiveXThreadStyleForCurrentThread with 
	COINIT_APARTMENTTHREADED if you register any ActiveX event 
	callbacks and want the callbacks to be called from the same 
	thread in which they were registered.  If you do not call 
	CA_InitActiveXThreadStyleForCurrentThread with 
	COINIT_APARTMENTTHREADED your callbacks will be called from a 
	system thread.
	*************************************************************/
//}

short CPicsCtrl::ConnectPICS()
{
	return PICS_Interface->Connection("HONGBO.ZHANG\0","pics\0");
	/*************************************************************
	1. PICS Server should confirm the authority before make the database connection
	2. PICS Interface DLL gets MAC address of local PC.
	3. If it succeeds, the client tool can continue to use the following functions.
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 =PICS Middleware exception.
	MID_RET_INVALID_MAC	' -18=The MAC address is not registered in PICS.
	MID_RET_NO_USER	' -19=User id not found.
	MID_RET_INVALID_USER	' -20=User id is invalid.
	MID_RET_INVALID_PASSWORD	' -21=Wrong password.
	MID_RET_NO_LOGIN_LONGTIME	' -22= You have not logined for a long time.
	INT_RET_EXCEPTION 	' -100 = PICS Interface exception.
	*************************************************************/
}


short CPicsCtrl::QuerySPCFromPICS(const char IMEI_Array[],char SPC_Array[])
{	
	CComBSTR	_SPC(20);
	_bstr_t		_temp;
    short result=PICS_Interface->QuerySpcCode(IMEI_Array,&_SPC);
	if(result)
		return result;
	_temp = _SPC.Copy();
	UnicodeToAnsiBuffer(_temp,SPC_Array,20);
	_SPC.Empty();
	return result;
	/*************************************************************
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NO_SPC	' -9 = SPC not found.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	Description: Query SPC Code. If it fails (except for the return value, 0), out_SpcCode is BLANK.
	*************************************************************/
}

short CPicsCtrl::CheckIMEI(const char IMEI_Array[], char *Part_Number_Array)
{
	return PICS_Interface->CheckImei(IMEI_Array,Part_Number_Array);
	/*************************************************************
	1. Check the IMEI format before starting any functions.
	2. Check the IMEI validation:
	- check if it exists in specific IMEI range (TACFAC and SSS booking range) for the part number.
	- check the SP (Spare, the 15th digit of IMEI) is valid.
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_INVALID_IMEI_LEN	' -2 = Invalid IMEI length.
	MID_RET_INVALID_IMEI_TACFAC	' -3 = The IMEI is invalid because the part number cannot use this TAC and FAC.
	MID_RET_INVALID_IMEI_RANGE	' -5 = Invalid IMEI range.
	MID_RET_INVALID_IMEI_CHECKSUM	' -6 = The IMEI is invalid according to the checksum.
	MID_RET_NO_BOOKING_PARTNO	' -14 = The part number is not found in SerialNumber storage maintenance (SSS_Booking System).
	MID_RET_INVALID_PARTNO	' -16 = The part number is not found in PICS model master.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	*************************************************************/
}

short CPicsCtrl::CreateIMEIRecord(const char IMEI_Array[])
{
	return PICS_Interface->WriteImei(IMEI_Array);
	/*************************************************************
	1. Create an IMEI record.
	2. Factory Tool will use CheckImei() to check if the IMEI is valid or not, so WriteImei() needn't to check again.
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_EXISTS_IMEI	' -7 = IMEI has already existed.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	*************************************************************/
}

short CPicsCtrl::GetFSNCode(const char IMEI_Array[], unsigned char FSN_Array[])
{
	CComBSTR	_FSN(33);
	_bstr_t		_temp;
    short result=PICS_Interface->GetFSNCode(IMEI_Array,&_FSN);
	if(result)
		return result;
	_temp = _FSN.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)FSN_Array,33);
	_FSN.Empty();
	return result;
	/*************************************************************
	1. CI use IMEI to query FSN Code from PICS and then write it into phone.
	   PICS should always generate a new FSN, store into PICS and return to CI.
	2. The FSN Code (16 bytes raw data) should be generated randomly and must be unique for all projects. It can't be overwritten FSN in the flash chip.
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	*************************************************************/
}

short CPicsCtrl::QueryFSNCode(const char IMEI_Array[], unsigned char FSN_Array[])
{
	CComBSTR	_FSN(33);
	_bstr_t		_temp;
    short result=PICS_Interface->QueryFSNCode(IMEI_Array,&_FSN);
	if(result)
		return result;
	_temp = _FSN.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)FSN_Array,33);
	_FSN.Empty();
	return result;
	/*************************************************************
	1. CI use IMEI to query FSN Code from PICS.
	2. QueryFsnCode doesn't generate FSN code just like GetFsnCode does.
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NO_FSN	' -10= FSN not found.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	Description: Query FSN Code. If it fails (except for the return value, 0), out_FSNCode is BLANK.
	*************************************************************/
}

short CPicsCtrl::GetSIMData(short SIMType, const char IMEI_Array[],const char FSN_Array[],const char *SIMFileName, unsigned char SIM_Data_Array[], short *SIM_Data_Length)
{
/*  CComBSTR	_SIMData(1025);
	_bstr_t		_temp;
	short result=PICS_Interface->GetSimData(SIMType, 
											IMEI_Array, 
											FSN_Array, 
											SIMFileName, 
											&_SIMData, 
											SIM_Data_Length);*/
	BSTR _SIMData = NULL;
	short result=PICS_Interface->GetSimData(SIMType, 
											IMEI_Array, 
											FSN_Array, 
											SIMFileName, 
											&_SIMData, 
											SIM_Data_Length);
	if(result != 0)
		return result;
	/*_temp = _SIMData.Copy();
	_stprintf((char *)SIM_Data_Array , _T("%s"), (LPCTSTR)_temp);
	_SIMData.Empty();*/

	//for(int i=0;i<1152;i++)
	//{
	//	SIM_Data_Array[i]=(unsigned char)*(_SIMData+i);
	//}

	UnicodeToAnsiBuffer(_SIMData, (char*)SIM_Data_Array, 1153);
	return result;
	/*************************************************************
	In_SimType: integer
	in_ImeiCode: 15 digit characters.
	in_FsnCode: 32 digit characters.
	in_SimFileName: character string with null-terminated characters.
	inout_pSimData: 1024 digit characters.
	inout_SimDataSize: indicate the size of buffer pointed by inout_pSimData.
	Then be updated as actual bytes of SIM data after function returns.	
	1. CI use IMEI, FSN and SIM lock file name to request a new SIM data (512 bytes raw data) and then write SIM data into phone.
	2. The SIM data is generated by SIM Encoder (e.g. SIM Lock Security DLL) and needs 4 inputs listed below.
	  - IMEI
	  - FSN
	  - SIM Lock File
	  - Unlock Code and Unblock Code (8 digits: 00000001~99999998)
	"	Unlock code for Network Lock
	"	Unlock code for Service Provider Lock
	"	Unlock code for Corporate Lock
	"	Unlock code for Network Subset Lock
	"	Unlock code for SIM Lock
	"	Unblock code for User SIM Lock
	"	Reserved code for future usage
	3. Middleware use IMEI to query FSN from PICS against the specific FSN from CI. ?QueryFsnCode()?
	4. Middleware use SIM lock file name to query SIM lock file from PICS.
	5. Middleware use IMEI to query 7 unlock codes/unblock codes from PICS.
	6. PICS should always randomly (use time as seed) generate new 7 unlock codes/unblock codes, store into PICS and return to Middleware. The 7 unlock codes/unblock codes use the same random pool.
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NOT_SAME_FSN	' -11 = The FSN is not the same as the FSN in PICS.
	MID_RET_NO_FILE	' -12 = SIM file not found.
	SIM_RET_NONE_SUPP_FUNC	' 1 = The specific function is not supported for specific SIM type.
	SIM_RET_NONE_ENOUGH_MEM	' 2 = No enough memory.
	SIM_RET_INVALID_PARAM	' 3 = The specific parameter is not valid.
	SIM_RET_INVALID_OBJECT	' 4 = The specific object handle is not valid.
	SIM_RET_NO_IMEI	' 5 = IMEI is not set yet.
	SIM_RET_NO_FSN	' 6 = FSN is not set yet.
	SIM_RET_NO_SIM_FILE	' 7 = SIM file is not set yet.
	SIM_RET_NO_UNLOCK_CODE	' 8 = Unlock code is not set yet.
	SIM_RET_FILE_PARSE_FAIL	' 9 = Parse SIM file fails.
	SIM_RET_BUF_NOT_ENOUGH	' 10 = The specific output buffer is too small.
	SIM_RET_RESULT_FAIL	' 11 = The specific SIM data is invalid.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	Description: Get SIM Lock Data. The output value of inout_pSimData is 512 sets of hexadecimal number (1024 bits long), 
	and the output value of inout_SimDataSize is 512. (Currently the length of SIM data is 512 bytes fixed, and in the future it may be changed. If the function fails (except for the return value, 0), inout_pSimData is BLANK.
	The sample of SIM data is "2B 97 BE 61 52 B4 90 ED 37 E1 F4 2C EB 
	40 C7 55 6C BE 9C 70 76 8E A0 00 51 03 1D E4 D0 EA 6C 1D 2B 97 BE 
	1 52 B4 90 ED CA 47 D0 50 8F 62 3E CF C8 89 0F 38 C1 E2 19 E7 6B 
	B3 4B 3C 44 93 64 90 E0 8B 6C 21 BD 10 1F 1C 52 0B 69 AB BC F0 F4 
	93 69 52 A7 48 03 48 93 88 BC 0C 88 80 71 1E 24 41 EA 4F D9 02 38 
	53 87 B9 ED 07 00 74 24 C2 B0 5F F3 E4 03 CC 90 E0 05 07 C7 F8 7A 
	64 13 97 BC 85 D5 25 64 66 A2 6D EA 04 F8 53 8E 6B 80 3B 34 5E 8E 
	52 A2 D8 A4 F8 0C 53 A8 74 F9 D7 69 4A 5F 90 D3 2C 53 F4 BA B3 CD 
	85 14 F4 4C B8 AB 56 A2 4B 66 60 07 F3 04 97 E0 D0 23 52 88 81 D7 E9 BA BC FE D6 C7 04 B4 64 69 25 A2 51 41 81 34 1B E2 92 83 E3 EA A2 1B 18 A2 56 4D F8 08 00 A8 10 20 02 2B 97 BE 61 52 B4 90 ED CA 47 D0 AF 46 E0 62 E5 A3 65 6B 96 69 31 46 3C CA BF AA B7 DF 28 E2 FC D5 E2 63 B2 0A 30 0A 07 24 35 17 1A 45 21 E2 54 D0 4A C0 38 F8 3F 2E 22 87 F4 97 8A F0 45 1C 59 0A 93 D3 C6 09 04 9B 14 C5 1C 85 8F A3 BF 05 88 75 DC D5 F8 1F 23 E0 B5 A7 8A 28 3F 0D 54 9C C5 53 B1 D0 2F 58 BD 00 76 57 DB F3 49 82 87 04 70 1B 16 2F 3D 3F 60 E7 DF 3C 3F E0 10 BA AD F2 C3 02 18 14 31 3E 5A 45 22 26 08 CC 62 6E BC CA F5 D7 4C 3D D2 64 F5 09 59 6C CF BF 8B 02 95 B2 3B 6A 48 64 07 56 51 7A 20 CC 67 FF 08 D8 DE 8E 2E 91 D0 59 90 DF 6F 7A 9B 9D F8 2D CE CC 95 77 60 68 1E A3 66 8F 83 0C 24 0E F5 B2 CF 28 6C 2F 02 E3 99 F5 11 02 6A 8B 54 A1 A0 99 00 6C D8 6A 1E 82 57 08 00 6A 4C 1B 79 95 F6 E6 A9 D8 EA 66 F7 DE B8 5B 74 34 C4 BC CE 58 66 F1 88 A5 DB 50 36 11 A4 C8 CC 1F 9F AA C8 0A 0D 42 3E 23 00 DE F7 19 34 02 DE 98 C1 07 22 95 BE 46 2E 0D 95 45 57", and the space is omitted.
	*************************************************************/
}

short CPicsCtrl::QueryUnlockUnblockCode(const char IMEI_Array[],
										 char Network_Unlock_Code_Array[],
										 char Service_Provider_Unlock_Code_Array[],
										 char Coporate_Unlock_Code_Array[],
										 char Network_Subset_Unlock_Code_Array[],
										 char User_SIM_Unlock_Code_Array[],
										 char User_USIM_Unlock_Code_Array[],
										 char Serial_Code_Array[],
										 char SPC_Array[],
										 char Phone_Lock_Code_Array[])
{
	CComBSTR	_Network_Unlock_Code(9),
				_Service_Provider_Unlock_Code(9),
				_Coporate_Unlock_Code(9),
				_Network_Subset_Unlock_Code(9),
				_User_SIM_Unlock_Code(9),
				_User_USIM_Unlock_Code(9),
				_Serial_Code(9),
				_SPC(7),
				_Phone_Lock_Code(9);
		        
	_bstr_t		_temp;
	short result=PICS_Interface->QueryUnlockCode(IMEI_Array, 
											&_Network_Unlock_Code,
											&_Service_Provider_Unlock_Code,
											&_Coporate_Unlock_Code,
											&_Network_Subset_Unlock_Code,
											&_User_SIM_Unlock_Code,
											&_User_USIM_Unlock_Code,
											&_Serial_Code,
											&_SPC,
											&_Phone_Lock_Code);						
	if(result)
		return result;
	_temp = _Network_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Network_Unlock_Code_Array,9);
	_Network_Unlock_Code.Empty();
	Network_Unlock_Code_Array[9]='\0';

	_temp = _Service_Provider_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Service_Provider_Unlock_Code_Array,9);
	_Service_Provider_Unlock_Code.Empty();
	Service_Provider_Unlock_Code_Array[9]='\0';

	_temp = _Coporate_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Coporate_Unlock_Code_Array,9);
	_Coporate_Unlock_Code.Empty();
	Coporate_Unlock_Code_Array[9]='\0';

	_temp = _Network_Subset_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Network_Subset_Unlock_Code_Array,9);
	_Network_Subset_Unlock_Code.Empty();
	Network_Subset_Unlock_Code_Array[9]='\0';

	_temp = _User_SIM_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)User_SIM_Unlock_Code_Array,9);
	_User_SIM_Unlock_Code.Empty();
	User_SIM_Unlock_Code_Array[9]='\0';

	_temp = _User_USIM_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)User_USIM_Unlock_Code_Array,9);
	_User_USIM_Unlock_Code.Empty();
	User_USIM_Unlock_Code_Array[9]='\0';

	_temp = _Serial_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Serial_Code_Array,9);
	_Serial_Code.Empty();
	Serial_Code_Array[9]='\0';

	_temp = _SPC.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)SPC_Array,7);
	_SPC.Empty();
	SPC_Array[7]='\0';

	_temp = _Phone_Lock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Phone_Lock_Code_Array,9);
	_Phone_Lock_Code.Empty();
	Phone_Lock_Code_Array[9]='\0';

	return result;
	/*************************************************************
	1. PC Tools use IMEI to query unlock code and unblock code from PICS.
	2. 
	"	out_unlock_N: Network Unlock Code
	"	out_unlock_S: Service Provider Unlock Code
	"	out_unlock_C: Coporate Unlock Code
	"	out_unlock_NS: Network Subset Unlock Code
	"	out_unlock_US: User SIM Unlock Code
	"	out_unblock_SIM: User SIM Unblock Code
	"	out_unlock_Res: Serial Code (Reserved Code)
	"	out_SPC: SPC code
	"	out_phone_lock: Phone Lock Code
	3. Return 0 if IMEI is found; return -8 if IMEI not found.
	Declaration?QueryUnlockCode(in_ImeiCode[in] String, out_unlock_N[out] String, out_unlock_S[out] String, out_unlock_C[out] String, out_unlock_NS[out] String, out_unlock_US[out] String, out_unblock_SIM[out] String, out_unlock_Res[out] String, out_SPC[out] String, out_phone_lock[out] String) as T_MID_RET_VALUE
	Return Value:
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	Description: If it fails (except for the return value, 0), all the lock codes are BLANK; If the IMEI exists, but some codes have not created yet, they are BLANK.
	*************************************************************/

}

short CPicsCtrl::QueryUnlockUnblockCode_BT(const char IMEI_Array[],
										 char Network_Unlock_Code_Array[],
										 char Service_Provider_Unlock_Code_Array[],
										 char Coporate_Unlock_Code_Array[],
										 char Network_Subset_Unlock_Code_Array[],
										 char User_SIM_Unlock_Code_Array[],
										 char User_USIM_Unlock_Code_Array[],
										 char Serial_Code_Array[],
										 char SPC_Array[],
										 char Phone_Lock_Code_Array[])
{
	CComBSTR	_Network_Unlock_Code(17),
				_Service_Provider_Unlock_Code(17),
				_Coporate_Unlock_Code(17),
				_Network_Subset_Unlock_Code(17),
				_User_SIM_Unlock_Code(17),
				_User_USIM_Unlock_Code(17),
				_Serial_Code(17),
				_SPC(7),
				_Phone_Lock_Code(17);
		        
	_bstr_t		_temp;
	short result=PICS_Interface->QueryUnlockCode(IMEI_Array, 
											&_Network_Unlock_Code,
											&_Service_Provider_Unlock_Code,
											&_Coporate_Unlock_Code,
											&_Network_Subset_Unlock_Code,
											&_User_SIM_Unlock_Code,
											&_User_USIM_Unlock_Code,
											&_Serial_Code,
											&_SPC,
											&_Phone_Lock_Code);						
	if(result)
		return result;
	_temp = _Network_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Network_Unlock_Code_Array,17);
	_Network_Unlock_Code.Empty();
	Network_Unlock_Code_Array[17]='\0';

	_temp = _Service_Provider_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Service_Provider_Unlock_Code_Array,17);
	_Service_Provider_Unlock_Code.Empty();
	Service_Provider_Unlock_Code_Array[17]='\0';

	_temp = _Coporate_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Coporate_Unlock_Code_Array,17);
	_Coporate_Unlock_Code.Empty();
	Coporate_Unlock_Code_Array[17]='\0';

	_temp = _Network_Subset_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Network_Subset_Unlock_Code_Array,17);
	_Network_Subset_Unlock_Code.Empty();
	Network_Subset_Unlock_Code_Array[17]='\0';

	_temp = _User_SIM_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)User_SIM_Unlock_Code_Array,17);
	_User_SIM_Unlock_Code.Empty();
	User_SIM_Unlock_Code_Array[17]='\0';

	_temp = _User_USIM_Unlock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)User_USIM_Unlock_Code_Array,17);
	_User_USIM_Unlock_Code.Empty();
	User_USIM_Unlock_Code_Array[17]='\0';

	_temp = _Serial_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Serial_Code_Array,17);
	_Serial_Code.Empty();
	Serial_Code_Array[17]='\0';

	_temp = _SPC.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)SPC_Array,7);
	_SPC.Empty();
	SPC_Array[7]='\0';

	_temp = _Phone_Lock_Code.Copy();
	UnicodeToAnsiBuffer(_temp,(char*)Phone_Lock_Code_Array,17);
	_Phone_Lock_Code.Empty();
	Phone_Lock_Code_Array[17]='\0';

	return result;
	/*************************************************************
	1. PC Tools use IMEI to query unlock code and unblock code from PICS.
	2. 
	"	out_unlock_N: Network Unlock Code
	"	out_unlock_S: Service Provider Unlock Code
	"	out_unlock_C: Coporate Unlock Code
	"	out_unlock_NS: Network Subset Unlock Code
	"	out_unlock_US: User SIM Unlock Code
	"	out_unblock_SIM: User SIM Unblock Code
	"	out_unlock_Res: Serial Code (Reserved Code)
	"	out_SPC: SPC code
	"	out_phone_lock: Phone Lock Code
	3. Return 0 if IMEI is found; return -8 if IMEI not found.
	Declaration?QueryUnlockCode(in_ImeiCode[in] String, out_unlock_N[out] String, out_unlock_S[out] String, out_unlock_C[out] String, out_unlock_NS[out] String, out_unlock_US[out] String, out_unblock_SIM[out] String, out_unlock_Res[out] String, out_SPC[out] String, out_phone_lock[out] String) as T_MID_RET_VALUE
	Return Value:
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	Description: If it fails (except for the return value, 0), all the lock codes are BLANK; If the IMEI exists, but some codes have not created yet, they are BLANK.
	*************************************************************/

}


short CPicsCtrl::GetSPCCodeFromPICS(const char IMEI_Array[], char SPC_Array[])
{
	CComBSTR	_SPC(20);
    short result=PICS_Interface->GetSpcCode(IMEI_Array,&_SPC);
	if(result == 0)
		UnicodeToAnsiBuffer(_SPC,(char*)SPC_Array,20);
	return result;
	/*************************************************************
	1. CI use IMEI to query SPC Code from PICS. Then CI could unlock SPC protection or write a new SPC code into phone.
	  PICS should always generate a new SPC, store into PICS and return to CI.
	2. The SPC Code (6 digits: 0~9) should be generated randomly excluding 000000 and 999999 every time; it's not unique.
	Declaration: GetSpcCode(in_ImeiCode[in] String, out_SpcCode[out] String) as T_MID_RET_VALUE
	Return Value:
	MID_RET_SUCCESS	' 0 = Success.
	MID_RET_EXCEPTION	' -1 = PICS Middleware exception.
	MID_RET_NO_IMEI	' -8= IMEI not found in SIM pool.
	MID_RET_NO_RESPONSE_LONGTIME	' -23= No response for a long time. Please connect PICS again.
	MID_RET_NO_AUTHORITY	' -24= You are not authorized to use this function.
	MID_RET_NO_PASSKEY	' -25= Connection failed. Please connect PICS again.
	MID_RET_INCONSISTENT_MAC	' -26= Connection failed due to the inconsistent MAC address. Please connect PICS again.
	INT_RET_EXCEPTION	' -100 = PICS Interface exception.
	Description: Get SPC Code. If it fails (except for the return value, 0), out_SpcCode is BLANK.
	*************************************************************/

}

short CPicsCtrl::EnableLog(bool LogFlag,char *LogFileName)
{
	return PICS_Interface->EnableTextLog(LogFlag,LogFileName);
}

void CPicsCtrl::CheckPlatformInPICS(char *Platform)
{
	PICS_Interface->CheckPlatform=Platform;
	return;
}
