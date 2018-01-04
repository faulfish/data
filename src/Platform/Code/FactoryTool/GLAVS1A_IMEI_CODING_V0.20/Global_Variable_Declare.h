//define for variable
#define VARIFY(VALUE) ASSERT(VALUE!=NULL)
#define SETREGISTRY   1
#define WM_RESETTESTCOUNT WM_USER+102
extern char Temp_Global[2048];
extern CString Current_Directory;
extern CString Sim_Data_File_Name;

//PPF file name
extern CString QSNV_File_Name;
extern CString QVCEFS_File_Name;
extern CString QZCS_File_Name;
extern CString DUCK_File_Name;

extern CString QSNV_File;
extern CString QVCEFS_File;
extern CString QZCS_File;
extern CString DUCK_File;

extern CString QVCEFS_File_Time;
extern CString QSNV_File_Time;
extern CString QZCS_File_Time;
extern CString DUCK_File_Time;

extern CString QVCEFS_File_Size;
extern CString QSNV_File_Size;
extern CString QZCS_File_Size;
extern CString DUCK_File_Size;

//QSWINFO file name--->to get the public key for FSN freez
extern CString QSWINFO_File_Name;
extern CString CSV_File_Name;

extern CString Model_Name;
extern CString Mode_Name;
extern CString Target_Type;
extern CString Ini_File_Path;
extern CString Message_String;
extern CString TRANS_NO;
extern CString SO_Str;
extern CString SO_Str_Old;
extern CString Picasso_No;

extern CString Software_Version;
extern CString Hardware_Version;
extern CString Part_Number;
extern CString Mechanical_Version;

extern CString Load_Path;
extern CString Tool_Version; 
extern CString Computer_Name;
extern unsigned char BRT_QCN_FLAG;//1 for BRT, 0 for QCN
extern unsigned char Write_MAL_Flag;//1 to write MAL
extern unsigned char DuckDll_Flag;
extern unsigned char DUCK_Flag;
extern unsigned char Key_Exist_Flag;
extern unsigned char Return_Flag;
extern unsigned char SetConfig;
extern unsigned char CheckDiff;

struct gIMEI{
			 char SO[50];
			 char MFGID[50];
			 char Model[50];
			 char Line[50];
			 char Station[50];
			 char Slot[50];
			 char ErrorCode[50];
			 char Result[50];
			 char IMEI[50];
			 char n_lock[50];
			 char ns_lock[50];
			 char sp_lock[50];
			 char c_lock[50];
			 char s_lock[50];
			 char MSN[100];
			 char IMSI[50];
			 char Phone[50];
			 char Level99[50];
			 char HW_ver[50];
			 char SW_ver[50];
			 char PCBA[50];
			 char SUG[50];
			 char Picasso[50];
			 char PASSWORD[50];
			 char BTAddr[13];	// Fox add
			 char SWLoad_Date[25];	// Fox add
			 char Wireless_MAC[50];//Lucky
};
extern struct gIMEI gIMEIrecord;
extern CString Library_Version;
extern CString Com_Port;
extern HANDLE PHONE_HANDLE;
extern unsigned short Port_Number;
extern CString Error_Code_String;
extern unsigned char Lock_Status;
extern unsigned char Unlock_Count;
extern long Status; 
extern char SPC_Code[7];
extern char IMEI_From_Phone[16];
extern char IMEI_For_Phone[16];
extern char BT_From_Phone[13];
extern unsigned char BT_For_Phone[7];
extern char IMEI_String[16];
extern char FSN_From_Phone_32[33];
extern unsigned char Status_SPC;
extern unsigned char ATHB1A_Flag;
extern CString IMEI;
extern CString Mobile_ID;
extern unsigned char KLF_Byte;
extern CString HW_Version_In_Mobile;
extern CString SW_Version_In_Mobile;
extern CString BT_Address;
extern CString GSN_String;
extern unsigned int Key_CheckSum;
extern unsigned char Device_Serial_Number[17];
extern unsigned char Device_Serial_Number_Digit[33];
extern unsigned char Public_Key[261];
extern unsigned int Public_Key_CheckSum;
extern unsigned char Slot_ID;
extern unsigned char SIM_AppIdLen;
extern unsigned char SIM_AppId[16]; 
extern unsigned long CommandStatus;
extern unsigned char SIM_Data_For_Phone[577];
extern unsigned char SIM_Data_From_PICS[4096];
//for ATHB1A and H21
extern char Network_Unlock_Code[9];
extern char Service_Provider_Unlock_Code[9];
extern char Coporate_Unlock_Code[9];
extern char Network_Subset_Unlock_Code[9];
extern char User_SIM_Unlock_Code[9];
extern char User_USIM_Unlock_Code[9];
extern char Serial_Code[9];
extern char Phone_Lock_Code[9];
//For ATHB3A
extern char BT_Network_Unlock_Code[17];
extern char BT_Service_Provider_Unlock_Code[17];
extern char BT_Coporate_Unlock_Code[17];
extern char BT_Network_Subset_Unlock_Code[17];
extern char BT_User_SIM_Unlock_Code[17];
extern char BT_User_USIM_Unlock_Code[17];
extern char BT_Serial_Code[17];
extern char BT_Phone_Lock_Code[17];

extern unsigned int AutoRestore;
extern unsigned int AllowESN_Mismatch;
extern unsigned int IgnorePhoneModel;
extern unsigned char SkipReset;

//Lock Indicator
extern unsigned char NetworkLock_Flag;
extern unsigned char CorporateLock_Flag;
extern unsigned char ServiceProviderLock_Flag;
extern unsigned char NetworkSubsetLock_Flag;
extern unsigned char SIMLock_Flag;
extern unsigned char UserSIMLock_Flag;

extern CString IMEI_LAST_WRITE;
extern CString IMEI_FROM_SCANNER;
extern CString MAL_FROM_SCANNER;
extern CString IMEI_FIRST_PART;
extern CString MAL_FIRST_PART;
extern unsigned char IMEI_Difference;
extern unsigned char ButtonChange_Flag;
extern unsigned char ButtonStart_Flag;

extern unsigned char SW_CHECK;
extern unsigned char HW_CHECK;
extern unsigned char ME_CHECK;
extern unsigned char PROCESS_CHECK;
extern CString Backup_Path;
extern CString PICSFile_Path;
extern CString ME_Version_In_Mobile;
extern int Connect_Server_Timeout;
extern int WriteSIM_Delay;
extern int Cycle_Count;
extern unsigned int TimeCount;
extern unsigned char IMEI_BT_GSN_Flag;
extern unsigned char DEBUG_ONOFF_FLAG;
extern CString Log_String;
extern CString Duck_Log_String;
extern unsigned char SECURITY_MECHANISM_FLAG;//to write publick key and SIM data or not

//station check
extern unsigned char BASEBAND_TEST_CHECK_FLAG;
extern unsigned char CAL_PRETEST_2G_CHECK_FLAG;
extern unsigned char CAL_PRETEST_3G_CHECK_FLAG;
extern unsigned char LEAKAGE_CHARGER_CHECK_FLAG;
extern unsigned char FUNCTION_WIRELESS_TEST_CHECK_FLAG;
extern unsigned char PPF_CHECK_FLAG;
extern unsigned char ACM_BOARD_CHECK_FLAG;
extern unsigned char SAVE_PICS_FILE_FLAG;
extern CString ServerDirectory;
extern CString Config_Version;
extern unsigned int TotalNo;
extern unsigned int PassNo;
extern unsigned int FailNo;
extern char Total_String[25];
extern char Pass_String[25];
extern char Fail_String[25];
extern HWND Global_WND;
extern unsigned char Start_Flag;
extern CString PNva[50];

//for script
extern char brt_file_name[512];
