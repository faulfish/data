#include "StdAfx.h"
#include "GPIB_Device.h"
//#include "GPIB_Factory.h"



//constructor
CGPIBDevice::CGPIBDevice(void)
{
	m_enumDevice = GPIB_DEVICE_UNKNOWN;
	m_iDeviceID	= -1;
	m_iCMUMode = -1;
	m_chCMUModeStr = "";
}

//destructor
CGPIBDevice::~CGPIBDevice(void)
{
}

int CGPIBDevice::IdAndInitDevice(int &iCalDevicePad, int &iCalDevicePad2, int &iCalDevicePad3
								 ,int &iDeviceAddress, int &iDeviceAddress2, int &iDeviceAddress3)
{

	char            buffer[200];       
	int             i,                            
					num_listeners;            
	unsigned short  pad = 0;              
	Addr4882_t      instruments[32],   
					result[32],          
					DeviceResult[32]; 
	const char		HP_Device[] = "HEWLETT-PACKARD";
	const char		AGILENT_Device[] = "Agilent Technologies";
	const char		RSCMU200_Device[] = "Rohde&Schwarz";
	const char		WILLTEK_Device[] = "\"WILLTEK";
	const char		*CalDevice[]= {"8960","CMU 200","NULL"};
	const char		*PowerName[]= {"E3631","66311","66332","NULL" };
	int				iNameIndex, iPowerIndex;
	int				iStrLength = 0;
	BOOL			bCalDeviceExist = FALSE;
	BOOL			bGpibOK = TRUE;
	BOOL			bPowerExist;
	BOOL			bFindBoradAddress = FALSE;
	char			*pChar,  cReading[50] = {0};
	int				iNumOfDevice=0;	
	DeviceInfo		deviceInfo[31];

	ibfind("GPIB0");
	ThreadIbsta();
	SendIFC(0);				
	if (ibsta & ERR){
		return ERR_GPIB_SEDIFC;
	}

	for (i = 0; i <= 30; i++){
		instruments[i] = i;
	}

	instruments[31] = NOADDR;
	FindLstn(0, instruments, result, 31);
	if (ibsta & ERR){
		return ERR_GPIB_FINDLSTN;
	}

	int j=0;
	for(i=0,j=0;i<ibcnt;i++){				
		if((result[i]&0xFF) != 0){
			if(j>0 && ((DeviceResult[j-1]&0x00FF) != (result[i]&0x00FF))){
				DeviceResult[j]=result[i];
				j++;
			}
			else if(j==0){
				DeviceResult[j]=result[i];
				j++;
			}
		}
	}	
	num_listeners = j;
	DeviceResult[j] = NOADDR;
	for(i=0;i<=j;i++){				
		result[i]=DeviceResult[i];
	}

	if(num_listeners>0){
		EnableRemote(0, result);
		if(ibsta & ERR){
			return ERR_GPIB_SENDLIST;
		}
		SendList(0, result, "*IDN?", 5L, NLend);
		if (ibsta & ERR){
			return ERR_GPIB_SENDLIST;
		}
	}
	if(bGpibOK){
		i=0;
		do{
			bPowerExist = FALSE;
			Receive(0, result[i], buffer, 200L, STOPend);
			if (ibsta & ERR){
				return ERR_DEVICE_NOTFOUND;
			}

			if(0 ==_strnicmp(buffer,HP_Device,15)){
				CString cstrTemp = buffer + 16;
				cstrTemp.TrimLeft();
				pChar = buffer+16;
				strcpy(pChar, 	cstrTemp.GetBuffer(cstrTemp.GetLength()));
				iPowerIndex = 0;
				while((strcmp(PowerName[iPowerIndex],"NULL") !=0)  )
				{
					iStrLength = strlen(PowerName[iPowerIndex]);
					if( 0 ==_strnicmp(pChar, PowerName[iPowerIndex],iStrLength) )
					{
						deviceInfo[i].iType = iPowerIndex;
						deviceInfo[i].iPad= pad;
						deviceInfo[i].iAddress = result[i];
						break;
					}
					iPowerIndex++;
				}
			}else if(0 ==_strnicmp(buffer,AGILENT_Device,20)){
				iNameIndex = 0;
				pChar = buffer+21;
				while( strcmp(CalDevice[iNameIndex],"NULL") != 0){
					iStrLength = strlen(CalDevice[iNameIndex]);
					if( 0 ==_strnicmp(pChar,CalDevice[iNameIndex],iStrLength) ){
						bCalDeviceExist = TRUE;
						break;
					}
					iNameIndex ++;
				}
				if(bCalDeviceExist){
					switch(iNameIndex)
					{
						case 0:
							deviceInfo[i].iType = Hp8960;
							deviceInfo[i].iPad= pad;
							deviceInfo[i].iAddress = result[i];
							break;
						case 1:
							deviceInfo[i].iType = RsCmu200;
							deviceInfo[i].iPad= pad;
							if (result[i]== -1){
								deviceInfo[i].iAddress = 0;
							}else
								deviceInfo[i].iAddress = result[i]-24576;
							break;
						default :
							deviceInfo[i].iType = Unknown;
					}
				}
			}else if(0 ==_strnicmp(buffer,RSCMU200_Device,sizeof(RSCMU200_Device)-1)){
					iNameIndex = 0;
				pChar = buffer+14;
					while( strcmp(CalDevice[iNameIndex],"NULL") != 0){
						iStrLength = strlen(CalDevice[iNameIndex]);
						if( 0 ==_strnicmp(pChar,CalDevice[iNameIndex],iStrLength) ){
							bCalDeviceExist = TRUE;
							break;
						}
						iNameIndex ++;
					}
				if(bCalDeviceExist){
						switch(iNameIndex)
						{
							case 0:
								deviceInfo[i].iType = Hp8960;
								deviceInfo[i].iPad= pad;
								deviceInfo[i].iAddress = result[i];
								break;
							case 1:
								deviceInfo[i].iType = RsCmu200;
								deviceInfo[i].iPad= pad;
								if (result[i]== -1){
									deviceInfo[i].iAddress = 0;
								}else
									deviceInfo[i].iAddress = result[i]-24576;
								break;
							default :
								deviceInfo[i].iType = Unknown;
						}
					}
			}
			else if(0 ==_strnicmp(buffer,WILLTEK_Device,sizeof(WILLTEK_Device)-1))
			{
				deviceInfo[i].iType = WillTek;
				deviceInfo[i].iPad= pad;
				deviceInfo[i].iAddress = result[i]&0xFF;

			}
			i++;
		}while( (i< num_listeners) && bGpibOK); 
	}

	for(i=0;i<num_listeners;i++){
		if ( i ==0 ){
			iCalDevicePad= (eGPIBDeviceType)deviceInfo[i].iType;
			iDeviceAddress = (eGPIBDeviceType)deviceInfo[i].iAddress;		
			if(i == num_listeners-1){
				break;
			}
		}
		else if ( i ==1 ){
			iCalDevicePad2= (eGPIBDeviceType)deviceInfo[i].iType;
			iDeviceAddress2 = (eGPIBDeviceType)deviceInfo[i].iAddress;
			if(i == num_listeners-1){
				break;
			}
		} 
		else if ( i ==2 ){
			iCalDevicePad3= (eGPIBDeviceType)deviceInfo[i].iType;
			iDeviceAddress3 = (eGPIBDeviceType)deviceInfo[i].iAddress;
			if(i == num_listeners-1){
				break;
			}
		}
	}

	return SUCCESS;
}
