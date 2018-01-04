#ifndef GPIB_DEVICE_H
#define GPIB_DEVICE_H

#include "StdAfx.h"
//#include "QLib_Defines.h"
#include "DECL-32.H"
//#include "ErrorCode.h"

// ---------------   Error Code ---------------------------
// DEVICE Error 20001~20100
#define		ERR_DEVICE_POW						0xf0020001
#define		ERR_DEVICE_CAL						0xf0020002
#define		ERR_DEVICE_NOTFOUND					0xf0020003
#define		ERR_DEVICE_POWCOMMAND				0xf0020004
#define		ERR_DEVICE_CAL_INIT					0xf0020005
#define		ERR_DEVICE_POW_INIT					0xf0020006
#define		ERR_DEVICE_MISMATCH_CONFIG_ADDRESS	0xf0020007
#define		ERR_DEVICE_CMU200_SEC_ADDR			0xf0020008
#define		ERR_DEVICE_SETTING					0xf0020009
#define		ERR_DEVICE_DATA_READ				0xf0020010
#define		ERR_DEVICE_CAL_CLEAR				0xf0020011
#define		ERR_DEVICE_POW_CLEAR				0xf0020012
#define		ERR_PHONE_MODE_NOTFOUND				0xf0020013
#define		ERR_CHIP_TYPE						0xf0020014
#define		ERR_SYS_SETTING_FOR_POLAR			0xf0020015
#define		ERR_SYS_SETTING_FOR_WCDMA_IM2		0xf0020016

// GPIB ERROR 20101~20150
#define		ERR_GPIB_WRITE						0xf0020101
#define		ERR_GPIB_READ						0xf0020102
#define		ERR_GPIB_SEDIFC						0xf0020103
#define		ERR_GPIB_FINDLSTN					0xf0020104
#define		ERR_GPIB_SENDLIST					0xf0020105
#define		ERR_GPIB_INTERFACE					0xf0020106
#define		ERR_GPIB_DEVCLEARLIST				0xf0020107
#define		ERR_GPIB_DEVICE_EXIT				0xf0020108
#define		ERR_GPIB_DEVICE_NO_SELECT			0xf0020109
#define		ERR_GPIB_POW_QUERY     				0xf0020110
#define		ERR_GPIB_QUERY						0xf0020111
#define		ERR_GPIB_INVALID_ADDRESS			0xf0020112
#define		ERR_GPIB_NO_Instrument				0xf0020113
#define		ERR_GPIB_SENDCMDS					0xf0020114


#define READ_BUFF_SIZE 8196
#define MAX_DEVICE_NUM 16
#define CHANNEL_POWER 0
#define AVERAGE_POWER 1
#define THERMAL_POWER AVERAGE_POWER

#define	SUCCESS		  0
#define	WAIT_TIME	  0.02
#define	ARRAY_SIZE	  32767
#define	OFF			  0			
#define	ON			  1			
#define	NOTFIND		  999			
#define	TRUE		  1	
#define UnknownDevice 255
#define Unknown		  255
#define UnknownPow	  255

#define CMD_ARRAY_SIZE      1024

#define	CALLTESTFETCHBUFSIZE	4096
#define	TEST_GSM_PVT_LEAD_OFFSET_ARRAY_SIZE  6
#define	TEST_GSM_PVT_TRAIL_OFFSET_ARRAY_SIZE 6
#define	TEST_GSM_ORFS_MOD_OFFSET_ARRAY_SIZE	8
#define	TEST_GSM_ORFS_SW_OFFSET_ARRAY_SIZE	8


#ifndef ENUM_EGPIBDEVICETYPE
#define ENUM_EGPIBDEVICETYPE
enum eGPIBDeviceType {  HpE3631 = 0, 
						Hp66311, 
						Hp66332A,
						RsCmu200,
                        Hp8960,
						WillTek,
						DeviceMaxNum,
						GPIB_DEVICE_UNKNOWN=255};
enum ePow_Device_Port{PS_P6V=0, PS_P25V, PS_MAX=0xFF};
enum eDevice_Kind{PowerSupply=1, GSMFunction, WCDMAFunction};

#endif


typedef struct	DeviceInfoTag
{
	int iType;
	int iPad;
	int iAddress;

}DeviceInfo; 



class CGPIBDevice
{
public:
	
	CGPIBDevice(void);
	virtual ~CGPIBDevice(void);

	int IdAndInitDevice(int &iCalDevicePad, int &iCalDevicePad2, int &iCalDevicePad3
						,int &iDeviceAddress1, int &iDeviceAddress2, int &iDeviceAddress3);

public:
	int m_iCMUMode;
	eGPIBDeviceType	m_enumCalDevice,m_enumCalSG1Device,m_enumCalSG2Device,m_enumCalSP1Device,m_enumCal2Device;
	char* m_chCMUModeStr;

	int m_enumPowDevice;
	int m_iCalDeviceID;
	int m_iPowDeviceID;
protected:
	int m_iDeviceID;
	eGPIBDeviceType m_enumDevice; 
};

#endif
