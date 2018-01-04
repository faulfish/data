#ifndef _NV_FACTORY_H_
#define _NV_FACTORY_H_

#ifdef FEATURE_PC_TOOLS
#ifndef PACKED
#define  PACKED
#endif
#endif

/* Factory data */
/* The old struct type */
typedef PACKED struct 
{
    byte MFG_ID[16];
    byte Download_Date[2];
    byte PCBA_Date[2];
    byte Func_Date[2];
    byte Pack_Date[2];
    byte Download;
    byte Trigger;
    byte Calibration;
    byte Pre_test;
    byte LCD;
    byte MMI;
    byte Function;
    byte Antenna;
    byte Acoustic;
    byte Wireless;
    byte QA;
    byte ESN;
    byte PCBA_Error;
    byte FA_Error;
    byte PACK_Error;
    byte PCBA_Process;
    byte FA_Process;
    byte PACK_Process;
} nv_FactoryDataS;


typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    byte    MFG_ID[16];
    byte    Download_Date[2];
    byte    PCBA_Date[2];
    byte    Func_Date[2];
    byte    Pack_Date[2];
    byte    Download;
    byte    Trigger;
    byte    Calibration;
    byte    Pre_test;
    byte    LCD;
    byte    MMI;
    byte    Function;
    byte    Antenna;
    byte    Acoustic;
    byte    Wireless;
    byte    QA;
    byte    ESN;
    byte    PCBA_Error;
    byte    FA_Error;
    byte    PACK_Error;
    byte    PCBA_Process;
    byte    FA_Process;
    byte    PACK_Process;
} nvi_FactoryDataS;

/* New factory struct follow GSM*/
typedef PACKED struct
{
	byte MFG_ID[18];
    byte Factory_ID[4];
    byte PCBA_Ver[5];
    byte HW_Ver[5];
    byte SW_Ver[5];
    byte LPR_PR;
} nv_Factory_PCBA;

typedef PACKED struct
{
	byte Process_ID;
    byte Process_Flag;
    byte Process_Date[2];
    byte Process_Test_1_Type;
    byte Test_1_Num[6];
    byte Process_Test_2_Type;
    byte Test_2_Num[6];
    byte Process_Prog_Ver[5];
    byte Process_Conf_File_Ver[5];
    byte Process_Line_Stat[2];
    byte Process_Count;
    byte Process_Inf_Check[6]; 
} nv_Factory_Manufacturing;

typedef PACKED struct
{
	byte Redown_Count;
    byte Reserve_1[5];
    byte Reserve_2[5];
    byte Reserve_3[5];
	byte Reserve_4[5];
	byte Reserve_5[5];
}nv_Factory_Other;

typedef PACKED struct
{
	byte PPF_File[32];
	byte PPF_Time_Stamp[5];
}nv_Factory_WAP_Set;

typedef PACKED struct
{
	byte ODM_OEM_ID;
	byte ODM_E_Label_Reserved1[10];
	byte ODM_E_Label_Reserved2[10];
	byte ODM_E_Label_Reserved3[10];
}nv_Factory_ODM_OEM;

typedef PACKED struct 
{
	nv_Factory_PCBA				Factory_PCBA;
	byte						FA_Ver;
	nv_Factory_Manufacturing	Factory_Manufacture_Proc[10];
	nv_Factory_Other			Factory_Other;
	nv_Factory_WAP_Set			Factory_WAP_Set;
	nv_Factory_ODM_OEM			Factory_ODM_OEM;
} nv_Ext_FactoryDataS;

typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
	nv_Factory_PCBA				Factory_PCBA;
	byte						FA_Ver;
	nv_Factory_Manufacturing	Factory_Manufacture_Proc[10];
	nv_Factory_Other			Factory_Other;
	nv_Factory_WAP_Set			Factory_WAP_Set;
	nv_Factory_ODM_OEM			Factory_ODM_OEM;
} nvi_Ext_FactoryDataS; 

typedef PACKED struct nv_engr_ppf_filename_s
{
	unsigned char ppf_filename[64];
} nv_engr_ppf_filename_s;

typedef PACKED struct nv_engr_lock_s
{
	byte is_engr_locked;
} nv_engr_lock_s;


#endif