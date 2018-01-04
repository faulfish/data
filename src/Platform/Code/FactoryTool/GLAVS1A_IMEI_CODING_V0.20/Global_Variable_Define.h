//define for variable
#define SETREGISTRY   1
#define WM_RESETTESTCOUNT WM_USER+102
char Temp_Global[2048]="\0";
CString Current_Directory="";
CString Sim_Data_File_Name="";

//PPF file name
CString QSNV_File_Name="";
CString QVCEFS_File_Name="";
CString QZCS_File_Name="";
CString DUCK_File_Name="";

CString QSNV_File="";
CString QVCEFS_File="";
CString QZCS_File="";
CString DUCK_File="";

CString QVCEFS_File_Time="";
CString QSNV_File_Time="";
CString QZCS_File_Time="";
CString DUCK_File_Time="";

CString QVCEFS_File_Size="";
CString QSNV_File_Size="";
CString QZCS_File_Size="";
CString DUCK_File_Size="";

//QSWINFO file name--->to get the public key for FSN freez
CString QSWINFO_File_Name="";

CString CSV_File_Name="";

CString Model_Name="";
CString Mode_Name="";
CString Target_Type="";
CString Ini_File_Path="";
CString TRANS_NO="";
CString SO_Str="12345";
CString SO_Str_Old="12345";
CString Picasso_No="";

CString Message_String="";

CString Software_Version="";
CString Hardware_Version="";
CString Part_Number="";
CString Mechanical_Version="";

CString Load_Path="";
CString Tool_Version="";
CString Computer_Name="";
unsigned char BRT_QCN_FLAG=1;//1 for BRT, 0 for QCN
unsigned char Write_MAL_Flag=0;//1 to write MAL
unsigned char DuckDll_Flag=0;
unsigned char DUCK_Flag=0;//use fast mode to download PPF
unsigned char Key_Exist_Flag=0;
unsigned char Return_Flag=0;
unsigned char SetConfig=0;
unsigned char CheckDiff=0;

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
			 char NAL_String[50];//Lucky
}	gIMEIrecord;

CString Library_Version="";
CString Com_Port="";
HANDLE PHONE_HANDLE=NULL;
unsigned short Port_Number=0;
CString Error_Code_String="";
unsigned char Lock_Status=0;
unsigned char Unlock_Count=0;
long Status=0; 
char SPC_Code[7]="\0";
char IMEI_From_Phone[16]="\0";
char IMEI_For_Phone[16]="\0";
char BT_From_Phone[13]="\0";
unsigned char BT_For_Phone[7]="\0";
char IMEI_String[16]="\0";
char FSN_From_Phone_32[33]="\0" ;
unsigned char Status_SPC=0;
CString IMEI;
CString Mobile_ID="";
CString USBLogName;
unsigned char KLF_Byte=0;
CString HW_Version_In_Mobile="";
CString SW_Version_In_Mobile=""; 
CString ME_Version_In_Mobile="";
CString BT_Address="";
CString GSN_String="";
unsigned int Key_CheckSum=0;
unsigned char Device_Serial_Number[17]="";
unsigned char Device_Serial_Number_Digit[33]="";
unsigned char Public_Key[261]="\0";
unsigned int Public_Key_CheckSum=0;
unsigned char Slot_ID;
unsigned char SIM_AppIdLen=16;
unsigned char SIM_AppId[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
unsigned long CommandStatus=0;
unsigned char SIM_Data_For_Phone[577]={0};
unsigned char SIM_Data_From_PICS[4096]={0};

//FOR ATHB1A AND H21
char Network_Unlock_Code[9]="\0";
char Service_Provider_Unlock_Code[9]="\0";
char Coporate_Unlock_Code[9]="\0";
char Network_Subset_Unlock_Code[9]="\0";
char User_SIM_Unlock_Code[9]="\0";
char User_USIM_Unlock_Code[9]="\0";
char Serial_Code[9]="\0";
char Phone_Lock_Code[9]="\0";

//FOR ATHB3A
char BT_Network_Unlock_Code[17]="\0";
char BT_Service_Provider_Unlock_Code[17]="\0";
char BT_Coporate_Unlock_Code[17]="\0";
char BT_Network_Subset_Unlock_Code[17]="\0";
char BT_User_SIM_Unlock_Code[17]="\0";
char BT_User_USIM_Unlock_Code[17]="\0";
char BT_Serial_Code[17]="\0";
char BT_Phone_Lock_Code[17]="\0";

unsigned int AutoRestore=TRUE;
unsigned int AllowESN_Mismatch=FALSE;
unsigned int IgnorePhoneModel=TRUE;
unsigned char SkipReset=FALSE;//TRUE to skip the reset operation after the phone is programmed   


//Lock Indicator
unsigned char NetworkLock_Flag=0;
unsigned char CorporateLock_Flag=0;
unsigned char ServiceProviderLock_Flag=0;
unsigned char NetworkSubsetLock_Flag=0;
unsigned char SIMLock_Flag=0;
unsigned char UserSIMLock_Flag=0;
unsigned char ATHB1A_Flag=0;

CString IMEI_LAST_WRITE="";
CString IMEI_FROM_SCANNER="";
CString MAL_FROM_SCANNER="";
CString IMEI_FIRST_PART="";
CString MAL_FIRST_PART="";
unsigned char IMEI_Difference=1;
unsigned char ButtonChange_Flag=0;
unsigned char ButtonStart_Flag=1;

unsigned char SW_CHECK=1;
unsigned char HW_CHECK=1;
unsigned char ME_CHECK=1;
unsigned char PROCESS_CHECK=1;
CString Backup_Path="";
CString PICSFile_Path="";
int Connect_Server_Timeout=5000;
int WriteSIM_Delay=5000;
int Cycle_Count=5;
unsigned int TimeCount=0;
unsigned char IMEI_BT_GSN_Flag=0;
unsigned char DEBUG_ONOFF_FLAG=0;
CString Log_String="";
CString Duck_Log_String="";
unsigned char SECURITY_MECHANISM_FLAG=1;//to write publick key and SIM data
//check station
unsigned char BASEBAND_TEST_CHECK_FLAG=0;
unsigned char CAL_PRETEST_2G_CHECK_FLAG=1;
unsigned char CAL_PRETEST_3G_CHECK_FLAG=1;
unsigned char LEAKAGE_CHARGER_CHECK_FLAG=1;
unsigned char FUNCTION_WIRELESS_TEST_CHECK_FLAG=1;
unsigned char PPF_CHECK_FLAG=0;
unsigned char ACM_BOARD_CHECK_FLAG=0;
unsigned char SAVE_PICS_FILE_FLAG=1;
CString ServerDirectory="";
CString Config_Version="";
unsigned int TotalNo=0;
unsigned int PassNo=0;
unsigned int FailNo=0;
char Total_String[25]="";
char Pass_String[25]="";
char Fail_String[25]="";
HWND Global_WND=NULL;
unsigned char Start_Flag=0;
CString PNva[50];
 
// for script
bool errFlag = false;
char brt_file_name[512]="";
