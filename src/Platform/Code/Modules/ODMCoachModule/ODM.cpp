#include "StdAfx.h"
#include "ODM.h"
#include "..\..\RequestManager\Request\ODMRequest.h"
#include "..\..\RequestManager\Request\DownloadRequest.h"
#include "..\..\RequestManager\Request\ATRequest.h"
#include <time.h>
#include "..\..\CommonUtil\MSXMLImpl.h"
#pragma comment(lib, "comsuppw.lib")
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"

CODM::CODM(int nCOMPort):CInterfaceBase(nCOMPort),m_NVItem(nCOMPort)
{
}

CODM::~CODM()
{
}

bool CODM::EnableDiagEvent(bool bEnable)
{
	return CInterfaceBase::EnableDiagEvent(bEnable);
}

void CODM::UpdateElement(XMLElement& Element,const TSTRING& strName,const TSTRING& strValue)
{
	using namespace XMLMANAGER;
	XMLElement namevalueElement(_T("namevalue"));
	XMLElement nameElement(_T("name"));
	nameElement.setvalue(strName);
	namevalueElement.getAllElement()->push_back(nameElement);
	XMLElement valueElement(_T("value"));
	valueElement.setvalue(strValue);
	namevalueElement.getAllElement()->push_back(valueElement);
	Element.getAllElement()->push_back(namevalueElement);
}

void CODM::AddProfile(XMLElement& Element,const TSTRING& strProfileName,std::vector<NameValue>& ProfileValues)
{
	using namespace XMLMANAGER;
	XMLElement profileElement(_T("profile"));	
	XMLElement profilenameElement(_T("profilename"));
	profilenameElement.setvalue(strProfileName);
	profileElement.getAllElement()->push_back(profilenameElement);	

	for(std::vector<NameValue>::iterator Iter = ProfileValues.begin();Iter != ProfileValues.end();Iter++)
		UpdateElement(profileElement,Iter->m_strName,Iter->m_strValue);
	ProfileValues.clear();

	Element.getAllElement()->push_back(profileElement);	
}

//NV_SYS_CUSTOM_INI_FILE_XSUM_I
TSTRING CODM::GetINIFile()
{	
	using namespace QSYNC_LIB ;

	TSTRING strINIFile;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	
	//*#06# nv-item ID 2697,  2696, 3497,3499,3188
	bool bRes = false;
	bRes = m_NVItem.ReadNVItem(2696,ReadResp);	//NV_SYS_CUSTOM_INI_FILE_NAME_I
	strINIFile += GetTSTRING((const char*)ReadResp.item_data) + _T("\r\n");
	bRes = m_NVItem.ReadNVItem(3497,ReadResp);	
	strINIFile += GetTSTRING((const char*)ReadResp.item_data) + _T("\r\n");
	bRes = m_NVItem.ReadNVItem(3499,ReadResp);	
	strINIFile += GetTSTRING((const char*)ReadResp.item_data) + _T("\r\n");
	
	return strINIFile;
}

#include  <stdlib.h>

bool CODM::WriteExceptionToEFS(int nVersion)
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CWriteExceptionToEFSRequest(nVersion);	
	return Execute(ptRequest);
}

bool CODM::GetMotoXPRSData(opxprofile_data_Melos& MotoXprsData)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CMotoRequestBase(1);
	if(Execute(ptRequest))
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);		
		::memcpy(&MotoXprsData,ptBufferReceived->GetData() + 4,sizeof(opxprofile_data_Lavernock));
		bRes = true;
	}
	else
	{
		bRes = false;
	}

	return bRes;
}

bool CODM::GetMotoXPRSData(opxprofile_data_Lavernock& MotoXprsData)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CMotoRequestBase(1);
	if(Execute(ptRequest))
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);		
		::memcpy(&MotoXprsData,ptBufferReceived->GetData() + 4,sizeof(opxprofile_data_Lavernock));
		bRes = true;
	}
	else
	{
		bRes = false;
	}

	return bRes;
}

bool CODM::GetMotoXPRSData(opxprofile_data& MotoXprsData)
{//For Athens project
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CMotoRequestBase(0);
	if(Execute(ptRequest))
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);		
		::memcpy(&MotoXprsData,ptBufferReceived->GetData() + 4,sizeof(opxprofile_data));
		bRes = true;
	}
	else
	{
		bRes = false;
	}

	return bRes;
}

bool CODM::GetMotoXprsLogger_Melos(TCHAR* szPATH,int nLen)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
		
	TSTRING strException;
	//Get exception
	{
		bRes = WriteExceptionToEFS(1);

		//For get exception file by QLIB function
		strException = QLIB_GetExceptionFile();
	}

	//Get MotoXPRS data
	opxprofile_data_Melos MotoXprsData = {0};
	if(bRes)
		bRes = GetMotoXPRSData(MotoXprsData);

	if(bRes)
	{
		TSTRING strBTAddr = m_NVItem.GetBTAddr();
		TSTRING strIMEI = m_NVItem.GetIMEI();
		//for(size_t nIndex = 0;nIndex < 8 && nIndex < strIMEI.size();nIndex++)
		//	MotoXprsData.ucIMEI[nIndex] = strIMEI[nIndex];
				
		//Get factory data
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		bool bFactoryRes = m_NVItem.ReadNVItem(NV_FACTORY_DATA_1_I,ReadResp);

		TSTRING strDOM1;
		TSTRING strDOM2;
		TSTRING strDOM3;
		if(bFactoryRes)
		{
			//Get ODM
			int nODM1Date = ReadResp.item_data[5];
			int nODM1Month = ReadResp.item_data[6];
			int nODM1Year = ReadResp.item_data[7];
			int nODM2Date = ReadResp.item_data[17];
			int nODM2Month = ReadResp.item_data[18];
			int nODM2Year = ReadResp.item_data[19];
			int nODM3Date = ReadResp.item_data[29];
			int nODM3Month = ReadResp.item_data[30];
			int nODM3Year = ReadResp.item_data[31];

			strDOM1 = INTToString(nODM1Year) + _T("\\") + INTToString(nODM1Month) + _T("\\") + INTToString(nODM1Date);
			strDOM2 = INTToString(nODM2Year) + _T("\\") + INTToString(nODM2Month) + _T("\\") + INTToString(nODM2Date);
			strDOM3 = INTToString(nODM3Year) + _T("\\") + INTToString(nODM3Month) + _T("\\") + INTToString(nODM3Date);

			//Get HW version
			for(int i=89; i <= 93; i++)
				MotoXprsData.HWVer[i-89] = isascii(ReadResp.item_data[i]) ? ReadResp.item_data[i] : 0;
		}

		using namespace XMLMANAGER;
		MSXMLLibrary mslib;
		XMLMANAGER::XMLDocument xmlDoc(&mslib);

		XMLElement MotoXPRSLogElement(_T("MotoXPRSLog"));
		{			
			UpdateElement(MotoXPRSLogElement,_T("LOG_FILE_FORMAT_VERSION"),_T("1.1"));			

			TCHAR szNumBuffer[512] = {0};

			std::vector<NameValue> ProfileValues;
			ProfileValues.push_back(NameValue(_T("IMEI number"),strIMEI));
			ProfileValues.push_back(NameValue(_T("DOM1"),strDOM1));
			ProfileValues.push_back(NameValue(_T("DOM2"),strDOM2));
			ProfileValues.push_back(NameValue(_T("DOM3"),strDOM3));
			ProfileValues.push_back(NameValue(_T("Product ID"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("TFT on timer"),INTToString(MotoXprsData.uiTFTONCnt/60)));

			TSTRING strFailByte;
			if(MotoXprsData.cFailByteId == 0)
				strFailByte = _T("00");
			else if(MotoXprsData.cFailByteId == 1)
				strFailByte = _T("01");
			else if(MotoXprsData.cFailByteId == 2)
				strFailByte = _T("11");

			ProfileValues.push_back(NameValue(_T("Fail Byte Identifier"),strFailByte));
			ProfileValues.push_back(NameValue(_T("Software version"),ToString(MotoXprsData.SWVer)));
			ProfileValues.push_back(NameValue(_T("Hardware version"),ToString(MotoXprsData.HWVer)));
			//ProfileValues.push_back(NameValue(_T("Lower Board Id"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("PCB Id"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("Keypad Board Id"),szNumBuffer));
			ProfileValues.push_back(NameValue(_T("LCM Id"),HEXToString(MotoXprsData.LCMdisplayID)));
			ProfileValues.push_back(NameValue(_T("Flash Id"),HEXToString(MotoXprsData.NANDFlashID)));
			//ProfileValues.push_back(NameValue(_T("Camera Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("BT Address"),strBTAddr));
			ProfileValues.push_back(NameValue(_T("INI Files"),GetINIFile()));
			AddProfile(MotoXPRSLogElement,_T("Phone Specific Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of soft reset"),ToString(MotoXprsData.SoftResetCnt)));
			ProfileValues.push_back(NameValue(_T("Count of power cut"),ToString(MotoXprsData.PowerCutCnt)));
			AddProfile(MotoXPRSLogElement,_T("Reset Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Total accumulated call timed"),INTToString(MotoXprsData.uiTotalDroppedCallCnt)));
			ProfileValues.push_back(NameValue(_T("Last completed call"),TimeToString(MotoXprsData.uiLastCall)));
			
			ProfileValues.push_back(NameValue(_T("Count of total dropped calls"),INTToString(MotoXprsData.uiTotalCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of dropped calls (call received)"),ToString(MotoXprsData.usIncomingDroppedCallCnt)));
			ProfileValues.push_back(NameValue(_T("Count of dropped calls (called out)"),ToString(MotoXprsData.usOutgoingDroppedCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of calls (incoming)"),ToString(MotoXprsData.usIncomingCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of calls (outgoing)"),ToString(MotoXprsData.usOutgoingCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of SMS (incoming)"),ToString(MotoXprsData.usIncomingSMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of SMS (outgoing)"),ToString(MotoXprsData.usOutgoingSMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of MMS (incoming)"),ToString(MotoXprsData.usIncomingMMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of MMS (outgoing)"),ToString(MotoXprsData.usOutgoingMMSCnt)));
			AddProfile(MotoXPRSLogElement,_T("Call Information"),ProfileValues);
			
			ProfileValues.push_back(NameValue(_T("KEY_0"),ToString(MotoXprsData.usKEY0Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_1"),ToString(MotoXprsData.usKEY1Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_2"),ToString(MotoXprsData.usKEY2Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_3"),ToString(MotoXprsData.usKEY3Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_4"),ToString(MotoXprsData.usKEY4Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_5"),ToString(MotoXprsData.usKEY5Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_6"),ToString(MotoXprsData.usKEY6Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_7"),ToString(MotoXprsData.usKEY7Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_8"),ToString(MotoXprsData.usKEY8Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_9"),ToString(MotoXprsData.usKEY9Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_STAR"),ToString(MotoXprsData.usKEYStarCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_POUND"),ToString(MotoXprsData.usKEYPoundCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_SOFTKEY_1"),ToString(MotoXprsData.usSoftKEY1Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_SOFTKEY_2"),ToString(MotoXprsData.usSoftKEY2Cnt)));			
			ProfileValues.push_back(NameValue(_T("KEY_SEND"),ToString(MotoXprsData.usSendKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_END"),ToString(MotoXprsData.usEndKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_POWER"),ToString(static_cast<unsigned short>(0))));//??
			ProfileValues.push_back(NameValue(_T("KEY_VOLUME_UP"),ToString(MotoXprsData.usVolumnUpCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_VOLUME_DOWN"),ToString(MotoXprsData.usVolumnDownCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_UP"),ToString(MotoXprsData.usJoyStickUpCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_DOWN"),ToString(MotoXprsData.usJoyStickdownCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Clear"),ToString(MotoXprsData.usClearKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Menu"),ToString(MotoXprsData.usMenuKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Smart"),ToString(MotoXprsData.usSmartKEYCnt)));			
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_LEFT"),ToString(MotoXprsData.usJoyStickLeftCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_RIGHT"),ToString(MotoXprsData.usJoyStickRightCnt)));
			AddProfile(MotoXPRSLogElement,_T("Key Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of flip open times"),ToString(MotoXprsData.usFlipOpenCnt)));
			AddProfile(MotoXPRSLogElement,_T("Physical Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Wired headset plug in counter"),ToString(MotoXprsData.usWiredHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Bluetooth headset plug in counter"),ToString(MotoXprsData.usBTHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Bluetooth enable timer"),ToString(MotoXprsData.uiBTEnableTmr/60)));			 
			ProfileValues.push_back(NameValue(_T("Camera VGA use counter"),ToString(MotoXprsData.usCameraVGACnt )));
			ProfileValues.push_back(NameValue(_T("Camera VGA use timer"),ToString(MotoXprsData.uiCameraVGATmr/60 )));			
			ProfileValues.push_back(NameValue(_T("Camera 2M use counter"),ToString(MotoXprsData.usCamera2MCnt )));
			ProfileValues.push_back(NameValue(_T("Camera 2M use timer"),ToString(MotoXprsData.uiCamera2MTmr/60 )));
			ProfileValues.push_back(NameValue(_T("Browser use counter"),ToString(MotoXprsData.usBrowseCnt)));
			ProfileValues.push_back(NameValue(_T("FM Radio use counter"),ToString(MotoXprsData.usFMRadioCnt)));
			ProfileValues.push_back(NameValue(_T("Game use counter"),ToString(MotoXprsData.usGameUsageCnt)));
			ProfileValues.push_back(NameValue(_T("Speaker Phone use counter"),ToString(MotoXprsData.usSpeakerPhoneCnt)));
			ProfileValues.push_back(NameValue(_T("Speaker Phone use timer"),ToString(MotoXprsData.uiSpeakerPhoneTmr/60)));
			ProfileValues.push_back(NameValue(_T("Vibrator use counter"),ToString(MotoXprsData.usVibratorCnt)));
			ProfileValues.push_back(NameValue(_T("Vibrator use timer"),ToString(MotoXprsData.uiVibratorTmr/60)));
			ProfileValues.push_back(NameValue(_T("Display backlight enable counter"),ToString(MotoXprsData.usBacklightDisplayOnCnt)));
			ProfileValues.push_back(NameValue(_T("Display backlight enable timer"),ToString(MotoXprsData.uiBacklightDisplayOnTmr/60)));
			ProfileValues.push_back(NameValue(_T("Keypad backlight enable counter"),ToString(MotoXprsData.usBacklightKeypadOnCnt)));
			ProfileValues.push_back(NameValue(_T("Keypad backlight enable timer"),ToString(MotoXprsData.uiBacklightKeypadOnTmr/60)));
			ProfileValues.push_back(NameValue(_T("Network Search counter"),ToString(MotoXprsData.usNetworkSearchCnt)));
			ProfileValues.push_back(NameValue(_T("No-service area timer"),ToString(MotoXprsData.uiNoServiceAreaTmr/60)));		
			ProfileValues.push_back(NameValue(_T("T-Flash plug in counter"),ToString(MotoXprsData.usTFlashXferCnt)));
			ProfileValues.push_back(NameValue(_T("0 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[0]/60)));
			ProfileValues.push_back(NameValue(_T("1 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[1]/60)));
			ProfileValues.push_back(NameValue(_T("2 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[2]/60)));
			ProfileValues.push_back(NameValue(_T("3 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[3]/60)));
			ProfileValues.push_back(NameValue(_T("4 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[4]/60)));
			ProfileValues.push_back(NameValue(_T("5 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[5]/60)));
			//ProfileValues.push_back(NameValue(_T("Charger plug in counter"),ToString(static_cast<unsigned short>(0))));//??
			AddProfile(MotoXPRSLogElement,_T("Timer Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Select Use Time"),ToString(MotoXprsData.usSelectUseTime)));
			ProfileValues.push_back(NameValue(_T("First Use Time"),TimeToString(MotoXprsData.usFirstUseTime)));
			ProfileValues.push_back(NameValue(_T("Last Use Time"),TimeToString(MotoXprsData.usLastUseTime)));			

			//ProfileValues.push_back(NameValue(_T("TTF"),ToString(MotoXprsData.TTF)));

			ProfileValues.push_back(NameValue(_T("Number of different SIM card insertions"),ToString(MotoXprsData.usDiffSIMInsertCnt)));
			AddProfile(MotoXPRSLogElement,_T("SIM Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("The number of times a USB cable was connected"),ToString(MotoXprsData.usUSBCABLECnt)));
			ProfileValues.push_back(NameValue(_T("The number of times a wall outlet was connected"),ToString(MotoXprsData.usWallOutletCableCnt)));
			AddProfile(MotoXPRSLogElement,_T("External Entity Information"),ProfileValues);

			int nIndex = 0;
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			AddProfile(MotoXPRSLogElement,_T("Charging Information"),ProfileValues);

			nIndex = 0;
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 00 ~ 01 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 01 ~ 02 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 02 ~ 03 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 03 ~ 04 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 04 ~ 05 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 05 ~ 06 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 06 ~ 07 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 07 ~ 08 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 08 ~ 09 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 09 ~ 10 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 10 ~ 11 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 11 ~ 12 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 12 ~ 13 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 13 ~ 14 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 14 ~ 15 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 15 ~ 16 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 16 ~ 17 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 17 ~ 18 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 18 ~ 19 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 19 ~ 20 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 20 ~ 21 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 21 ~ 22 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 22 ~ 23 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 23 ~ 24 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 00 ~ 01 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 01 ~ 02 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 02 ~ 03 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 03 ~ 04 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 04 ~ 05 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 05 ~ 06 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 06 ~ 07 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 07 ~ 08 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 08 ~ 09 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 09 ~ 10 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 10 ~ 11 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 11 ~ 12 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 12 ~ 13 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 13 ~ 14 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 14 ~ 15 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 15 ~ 16 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 16 ~ 17 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 17 ~ 18 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 18 ~ 19 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 19 ~ 20 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 20 ~ 21 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 21 ~ 22 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 22 ~ 23 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 23 ~ 24 o'clock"),GetPower(nIndex++,MotoXprsData)));
			AddProfile(MotoXPRSLogElement,_T("Power Information"),ProfileValues);
			
			/*New items from lavernock to melos
			unsigned short     download_times;			  //number of download
			unsigned long long start_download_datetime_ms;// start time of download
			unsigned long long end_download_datetime_ms;  // end time of download
			unsigned char      cnv_restore_status_code;	//CEFS NV restore status
			unsigned char      nv_partition_restore_status_code;   //partition NV restore status
			unsigned short     last_nv_restore_ms;        // last time of nv restore
			unsigned long long alarm_user_setting_datetime_ms;	//alarm user setting time
			unsigned long long alarm_int_datetime_ms;		//alarm interrupt
			unsigned char    ucLastMsgType;			  //last message type
			unsigned int uiLastAlarm;					 //last alarm launch time
			unsigned int uiLastMC;					 //last master clear time
			unsigned int uiLastMR;					 //last master reset time
			*/
			ProfileValues.push_back(NameValue(_T("Number of download"),ToString(MotoXprsData.download_times)));
			ProfileValues.push_back(NameValue(_T("Start time of download"),ToString(MotoXprsData.start_download_datetime_ms)));
			ProfileValues.push_back(NameValue(_T("End time of download"),ToString(MotoXprsData.end_download_datetime_ms)));
			ProfileValues.push_back(NameValue(_T("CEFS NV restore status"),ToString(MotoXprsData.cnv_restore_status_code)));
			ProfileValues.push_back(NameValue(_T("Partition NV restore status"),ToString(MotoXprsData.nv_partition_restore_status_code)));
			ProfileValues.push_back(NameValue(_T("Last time of nv restore"),ToString(MotoXprsData.last_nv_restore_ms)));
			ProfileValues.push_back(NameValue(_T("Alarm user setting time"),ToString(MotoXprsData.alarm_user_setting_datetime_ms)));
			ProfileValues.push_back(NameValue(_T("Alarm interrupt"),ToString(MotoXprsData.alarm_int_datetime_ms)));
			ProfileValues.push_back(NameValue(_T("Last message type"),ToString(MotoXprsData.ucLastMsgType)));
			ProfileValues.push_back(NameValue(_T("Last alarm launch time"),TimeToString(MotoXprsData.uiLastAlarm)));
			ProfileValues.push_back(NameValue(_T("Last master clear time"),TimeToString(MotoXprsData.uiLastMC)));
			ProfileValues.push_back(NameValue(_T("Last master reset time"),TimeToString(MotoXprsData.uiLastMR)));
			AddProfile(MotoXPRSLogElement,_T("Miscellaneous"),ProfileValues);

			// 20080903 Embert Tsai
			// Remove special character '01' from exception log.
			TCHAR	chFind = 1;
			TCHAR	chNew = _T(' ');;	// Space
			TSTRING::size_type dwIndex = 0;
			do{
				dwIndex = strException.find(chFind, dwIndex);
				if ( TSTRING::npos != dwIndex){
					strException[dwIndex] = chNew;
				}else{
					break;
				}
			} while (true);
			// End of modification by Embert

			ProfileValues.push_back(NameValue(_T("EXCEPTION LOG"),strException));
			AddProfile(MotoXPRSLogElement,_T("Exception Information"),ProfileValues);
		}

		xmlDoc.getAllElement()->push_back(MotoXPRSLogElement);

		time_t ltime;
		tm today;

		::_tzset();
		::time( &ltime );
		::_localtime64_s( &today, &ltime );

		TCHAR szDatatime[128] = {0};
		::_tcsftime( szDatatime, 128,_T("_%Y%m%d_%H%M%S_"), &today );
		TSTRING strDatetime = szDatatime;

		TCHAR szFullPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szFullPath,MAX_PATH);
		::PathRemoveFileSpec(szFullPath);
		::PathAddBackslash(szFullPath);
		TSTRING strPath = szFullPath;

		//Sample
		//cci_w270_004401026357018_20080313_104113_1.0_masc_ACC03.xml
		TSTRING strXPRSLOGFile = strPath + _T("Qisda_Melos_") + strIMEI + strDatetime + _T("1.0.xml");
		xmlDoc.save(strXPRSLOGFile);

		if(nLen > 0 && static_cast<int>(strXPRSLOGFile.size()) < nLen)
			::_tcscpy_s(szPATH,nLen,strXPRSLOGFile.c_str());
		else
			bRes = false;
	}

	return bRes;
}

bool CODM::GetMotoXprsLogger_Lavernock(TCHAR* szPATH,int nLen)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
		
	TSTRING strException;
	//Get exception
	{
		bRes = WriteExceptionToEFS(1);

		//For get exception file by QLIB function
		strException = QLIB_GetExceptionFile();
	}

	//Get MotoXPRS data
	opxprofile_data_Lavernock MotoXprsData = {0};
	if(bRes)
		bRes = GetMotoXPRSData(MotoXprsData);

	if(bRes)
	{
		TSTRING strBTAddr = m_NVItem.GetBTAddr();
		TSTRING strIMEI = m_NVItem.GetIMEI();
		//for(size_t nIndex = 0;nIndex < 8 && nIndex < strIMEI.size();nIndex++)
		//	MotoXprsData.ucIMEI[nIndex] = strIMEI[nIndex];
				
		//Get factory data
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		bool bFactoryRes = m_NVItem.ReadNVItem(NV_FACTORY_DATA_1_I,ReadResp);

		TSTRING strDOM1;
		TSTRING strDOM2;
		TSTRING strDOM3;
		unsigned char    HWVer[20] = {0};
		if(bFactoryRes)
		{
			//Get ODM
			int nODM1Date = ReadResp.item_data[5];
			int nODM1Month = ReadResp.item_data[6];
			int nODM1Year = ReadResp.item_data[7];
			int nODM2Date = ReadResp.item_data[17];
			int nODM2Month = ReadResp.item_data[18];
			int nODM2Year = ReadResp.item_data[19];
			int nODM3Date = ReadResp.item_data[29];
			int nODM3Month = ReadResp.item_data[30];
			int nODM3Year = ReadResp.item_data[31];

			strDOM1 = INTToString(nODM1Year) + _T("\\") + INTToString(nODM1Month) + _T("\\") + INTToString(nODM1Date);
			strDOM2 = INTToString(nODM2Year) + _T("\\") + INTToString(nODM2Month) + _T("\\") + INTToString(nODM2Date);
			strDOM3 = INTToString(nODM3Year) + _T("\\") + INTToString(nODM3Month) + _T("\\") + INTToString(nODM3Date);

			//Get HW version
			for(int i=89; i <= 93; i++)
				HWVer[i-89] = isascii(ReadResp.item_data[i]) ? ReadResp.item_data[i] : 0;

			// 94-103 PICASSO NO (10bytes)
			//for(int i=94; i <= 103; i++)
			//	MotoXprsData.PICASSO[i-94] = ReadResp.item_data[i];

			// 104-113 TRANCEIVER NO (10bytes)
			//for(int i=104; i <= 113; i++)
			//	MotoXprsData.SJUG[i-104] = ReadResp.item_data[i];				
		}

		using namespace XMLMANAGER;
		MSXMLLibrary mslib;
		XMLMANAGER::XMLDocument xmlDoc(&mslib);

		XMLElement MotoXPRSLogElement(_T("MotoXPRSLog"));
		{			
			UpdateElement(MotoXPRSLogElement,_T("LOG_FILE_FORMAT_VERSION"),_T("1.1"));			

			TCHAR szNumBuffer[512] = {0};

			std::vector<NameValue> ProfileValues;
			ProfileValues.push_back(NameValue(_T("IMEI number"),strIMEI));
			//ProfileValues.push_back(NameValue(_T("PICASSO number"),ToString(MotoXprsData.PICASSO)));
			//ProfileValues.push_back(NameValue(_T("SJUG number"),ToString(MotoXprsData.SJUG)));
			ProfileValues.push_back(NameValue(_T("DOM1"),strDOM1));
			ProfileValues.push_back(NameValue(_T("DOM2"),strDOM2));
			ProfileValues.push_back(NameValue(_T("DOM3"),strDOM3));
			ProfileValues.push_back(NameValue(_T("Product ID"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("TFT on timer"),INTToString(MotoXprsData.uiTFTONCnt/60)));

			TSTRING strFailByte;
			if(MotoXprsData.cFailByteId == 0)
				strFailByte = _T("00");
			else if(MotoXprsData.cFailByteId == 1)
				strFailByte = _T("01");
			else if(MotoXprsData.cFailByteId == 2)
				strFailByte = _T("11");

			ProfileValues.push_back(NameValue(_T("Fail Byte Identifier"),strFailByte));
			ProfileValues.push_back(NameValue(_T("Software version"),ToString(MotoXprsData.SWVer)));
			ProfileValues.push_back(NameValue(_T("Hardware version"),ToString(HWVer)));
			//ProfileValues.push_back(NameValue(_T("Lower Board Id"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("PCB Id"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("Keypad Board Id"),szNumBuffer));
			ProfileValues.push_back(NameValue(_T("LCM Id"),HEXToString(MotoXprsData.LCMdisplayID)));
			ProfileValues.push_back(NameValue(_T("Flash Id"),HEXToString(MotoXprsData.NANDFlashID)));
			//ProfileValues.push_back(NameValue(_T("Camera Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("BT Address"),strBTAddr));
			ProfileValues.push_back(NameValue(_T("INI Files"),GetINIFile()));
			AddProfile(MotoXPRSLogElement,_T("Phone Specific Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of soft reset"),ToString(MotoXprsData.SoftResetCnt)));
			ProfileValues.push_back(NameValue(_T("Count of power cut"),ToString(MotoXprsData.PowerCutCnt)));
			AddProfile(MotoXPRSLogElement,_T("Reset Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Total accumulated call timed"),INTToString(MotoXprsData.uiTotalDroppedCallCnt)));
			ProfileValues.push_back(NameValue(_T("Last completed call"),TimeToString(MotoXprsData.uiLastCall)));
			
			ProfileValues.push_back(NameValue(_T("Count of total dropped calls"),INTToString(MotoXprsData.uiTotalCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of dropped calls (call received)"),ToString(MotoXprsData.usIncomingDroppedCallCnt)));
			ProfileValues.push_back(NameValue(_T("Count of dropped calls (called out)"),ToString(MotoXprsData.usOutgoingDroppedCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of calls (incoming)"),ToString(MotoXprsData.usIncomingCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of calls (outgoing)"),ToString(MotoXprsData.usOutgoingCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of SMS (incoming)"),ToString(MotoXprsData.usIncomingSMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of SMS (outgoing)"),ToString(MotoXprsData.usOutgoingSMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of MMS (incoming)"),ToString(MotoXprsData.usIncomingMMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of MMS (outgoing)"),ToString(MotoXprsData.usOutgoingMMSCnt)));
			AddProfile(MotoXPRSLogElement,_T("Call Information"),ProfileValues);
			
			ProfileValues.push_back(NameValue(_T("KEY_0"),ToString(MotoXprsData.usKEY0Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_1"),ToString(MotoXprsData.usKEY1Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_2"),ToString(MotoXprsData.usKEY2Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_3"),ToString(MotoXprsData.usKEY3Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_4"),ToString(MotoXprsData.usKEY4Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_5"),ToString(MotoXprsData.usKEY5Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_6"),ToString(MotoXprsData.usKEY6Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_7"),ToString(MotoXprsData.usKEY7Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_8"),ToString(MotoXprsData.usKEY8Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_9"),ToString(MotoXprsData.usKEY9Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_STAR"),ToString(MotoXprsData.usKEYStarCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_POUND"),ToString(MotoXprsData.usKEYPoundCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_SOFTKEY_1"),ToString(MotoXprsData.usSoftKEY1Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_SOFTKEY_2"),ToString(MotoXprsData.usSoftKEY2Cnt)));			
			ProfileValues.push_back(NameValue(_T("KEY_SEND"),ToString(MotoXprsData.usSendKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_END"),ToString(MotoXprsData.usEndKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_POWER"),ToString(static_cast<unsigned short>(0))));//??
			ProfileValues.push_back(NameValue(_T("KEY_VOLUME_UP"),ToString(MotoXprsData.usVolumnUpCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_VOLUME_DOWN"),ToString(MotoXprsData.usVolumnDownCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_UP"),ToString(MotoXprsData.usJoyStickUpCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_DOWN"),ToString(MotoXprsData.usJoyStickdownCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Clear"),ToString(MotoXprsData.usClearKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Menu"),ToString(MotoXprsData.usMenuKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Smart"),ToString(MotoXprsData.usSmartKEYCnt)));			
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_LEFT"),ToString(MotoXprsData.usJoyStickLeftCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_RIGHT"),ToString(MotoXprsData.usJoyStickRightCnt)));
			AddProfile(MotoXPRSLogElement,_T("Key Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of flip open times"),ToString(MotoXprsData.usFlipOpenCnt)));
			AddProfile(MotoXPRSLogElement,_T("Physical Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Wired headset plug in counter"),ToString(MotoXprsData.usWiredHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Bluetooth headset plug in counter"),ToString(MotoXprsData.usBTHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Bluetooth enable timer"),ToString(MotoXprsData.uiBTEnableTmr/60)));			 
			ProfileValues.push_back(NameValue(_T("Camera VGA use counter"),ToString(MotoXprsData.usCameraVGACnt )));
			ProfileValues.push_back(NameValue(_T("Camera VGA use timer"),ToString(MotoXprsData.uiCameraVGATmr/60 )));			
			ProfileValues.push_back(NameValue(_T("Camera 2M use counter"),ToString(MotoXprsData.usCamera2MCnt )));
			ProfileValues.push_back(NameValue(_T("Camera 2M use timer"),ToString(MotoXprsData.uiCamera2MTmr/60 )));
			ProfileValues.push_back(NameValue(_T("Browser use counter"),ToString(MotoXprsData.usBrowseCnt)));
			ProfileValues.push_back(NameValue(_T("FM Radio use counter"),ToString(MotoXprsData.usFMRadioCnt)));
			ProfileValues.push_back(NameValue(_T("Game use counter"),ToString(MotoXprsData.usGameUsageCnt)));
			ProfileValues.push_back(NameValue(_T("Speaker Phone use counter"),ToString(MotoXprsData.usSpeakerPhoneCnt)));
			ProfileValues.push_back(NameValue(_T("Speaker Phone use timer"),ToString(MotoXprsData.uiSpeakerPhoneTmr/60)));
			ProfileValues.push_back(NameValue(_T("Vibrator use counter"),ToString(MotoXprsData.usVibratorCnt)));
			ProfileValues.push_back(NameValue(_T("Vibrator use timer"),ToString(MotoXprsData.uiVibratorTmr/60)));
			ProfileValues.push_back(NameValue(_T("Display backlight enable counter"),ToString(MotoXprsData.usBacklightDisplayOnCnt)));
			ProfileValues.push_back(NameValue(_T("Display backlight enable timer"),ToString(MotoXprsData.uiBacklightDisplayOnTmr/60)));
			ProfileValues.push_back(NameValue(_T("Keypad backlight enable counter"),ToString(MotoXprsData.usBacklightKeypadOnCnt)));
			ProfileValues.push_back(NameValue(_T("Keypad backlight enable timer"),ToString(MotoXprsData.uiBacklightKeypadOnTmr/60)));
			ProfileValues.push_back(NameValue(_T("Network Search counter"),ToString(MotoXprsData.usNetworkSearchCnt)));
			ProfileValues.push_back(NameValue(_T("No-service area timer"),ToString(MotoXprsData.uiNoServiceAreaTmr/60)));		
			ProfileValues.push_back(NameValue(_T("T-Flash plug in counter"),ToString(MotoXprsData.usTFlashXferCnt)));
			ProfileValues.push_back(NameValue(_T("0 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[0]/60)));
			ProfileValues.push_back(NameValue(_T("1 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[1]/60)));
			ProfileValues.push_back(NameValue(_T("2 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[2]/60)));
			ProfileValues.push_back(NameValue(_T("3 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[3]/60)));
			ProfileValues.push_back(NameValue(_T("4 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[4]/60)));
			ProfileValues.push_back(NameValue(_T("5 bar RSSI timer"),ToString(MotoXprsData.uiRSSITmr[5]/60)));
			//ProfileValues.push_back(NameValue(_T("Charger plug in counter"),ToString(static_cast<unsigned short>(0))));//??
			AddProfile(MotoXPRSLogElement,_T("Timer Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Select Use Time"),ToString(MotoXprsData.usSelectUseTime)));
			ProfileValues.push_back(NameValue(_T("First Use Time"),TimeToString(MotoXprsData.usFirstUseTime)));
			ProfileValues.push_back(NameValue(_T("Last Use Time"),TimeToString(MotoXprsData.usLastUseTime)));			

			//ProfileValues.push_back(NameValue(_T("TTF"),ToString(MotoXprsData.TTF)));

			ProfileValues.push_back(NameValue(_T("Number of different SIM card insertions"),ToString(MotoXprsData.usDiffSIMInsertCnt)));
			AddProfile(MotoXPRSLogElement,_T("SIM Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("The number of times a USB cable was connected"),ToString(MotoXprsData.usUSBCABLECnt)));
			ProfileValues.push_back(NameValue(_T("The number of times a wall outlet was connected"),ToString(MotoXprsData.usWallOutletCableCnt)));
			AddProfile(MotoXPRSLogElement,_T("External Entity Information"),ProfileValues);

			int nIndex = 0;
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,1 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,0 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,1 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,2 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("USB,3 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,0 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,1 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,2 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));	
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Wall outlet,3 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			AddProfile(MotoXPRSLogElement,_T("Charging Information"),ProfileValues);

			nIndex = 0;
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 00 ~ 01 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 01 ~ 02 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 02 ~ 03 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 03 ~ 04 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 04 ~ 05 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 05 ~ 06 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 06 ~ 07 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 07 ~ 08 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 08 ~ 09 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 09 ~ 10 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 10 ~ 11 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 11 ~ 12 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 12 ~ 13 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 13 ~ 14 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 14 ~ 15 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 15 ~ 16 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 16 ~ 17 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 17 ~ 18 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 18 ~ 19 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 19 ~ 20 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 20 ~ 21 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 21 ~ 22 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 22 ~ 23 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 23 ~ 24 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 00 ~ 01 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 01 ~ 02 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 02 ~ 03 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 03 ~ 04 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 04 ~ 05 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 05 ~ 06 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 06 ~ 07 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 07 ~ 08 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 08 ~ 09 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 09 ~ 10 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 10 ~ 11 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 11 ~ 12 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 12 ~ 13 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 13 ~ 14 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 14 ~ 15 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 15 ~ 16 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 16 ~ 17 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 17 ~ 18 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 18 ~ 19 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 19 ~ 20 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 20 ~ 21 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 21 ~ 22 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 22 ~ 23 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 23 ~ 24 o'clock"),GetPower(nIndex++,MotoXprsData)));
			AddProfile(MotoXPRSLogElement,_T("Power Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of uP reset"),ToString(MotoXprsData.usuPResetCnt)));
			ProfileValues.push_back(NameValue(_T("Hit of shake twice"),ToString(MotoXprsData.uiHitShakeTwiceCnt)));
			ProfileValues.push_back(NameValue(_T("Hit of Hit of left/right"),ToString(MotoXprsData.uiHitLeftRightCnt)));
			ProfileValues.push_back(NameValue(_T("Interrupt count of shake twice"),ToString(MotoXprsData.uiINTShakeTwiceCnt)));
			ProfileValues.push_back(NameValue(_T("Interrupt count of left/right"),ToString(MotoXprsData.uiINTLeftRightCnt)));
			ProfileValues.push_back(NameValue(_T("Duration of shake twice (ms)"),ToString(MotoXprsData.usShakeTwiceTmr)));
			ProfileValues.push_back(NameValue(_T("Duration of tip tap (ms)"),ToString(MotoXprsData.usTipTapTmr)));
			ProfileValues.push_back(NameValue(_T("Count of free fall"),ToString(MotoXprsData.usFreeFallCnt)));
			ProfileValues.push_back(NameValue(_T("Latest working normal uP version"),ToString(MotoXprsData.ucuPVersion)));
			ProfileValues.push_back(NameValue(_T("uP redownload count"),ToString(MotoXprsData.usuPRedownloadCnt)));
			AddProfile(MotoXPRSLogElement,_T("GSensor Information"),ProfileValues);
			// 20080903 Embert Tsai
			// Remove special character '01' from exception log.
			TCHAR	chFind = 1;
			TCHAR	chNew = _T(' ');;	// Space
			TSTRING::size_type dwIndex = 0;
			do{
				dwIndex = strException.find(chFind, dwIndex);
				if ( TSTRING::npos != dwIndex){
					strException[dwIndex] = chNew;
				}else{
					break;
				}
			} while (true);
			// End of modification by Embert

			ProfileValues.push_back(NameValue(_T("EXCEPTION LOG"),strException));
			AddProfile(MotoXPRSLogElement,_T("Exception Information"),ProfileValues);
		}

		xmlDoc.getAllElement()->push_back(MotoXPRSLogElement);

		time_t ltime;
		tm today;

		::_tzset();
		::time( &ltime );
		::_localtime64_s( &today, &ltime );

		TCHAR szDatatime[128] = {0};
		::_tcsftime( szDatatime, 128,_T("_%Y%m%d_%H%M%S_"), &today );
		TSTRING strDatetime = szDatatime;

		TCHAR szFullPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szFullPath,MAX_PATH);
		::PathRemoveFileSpec(szFullPath);
		::PathAddBackslash(szFullPath);
		TSTRING strPath = szFullPath;

		//Sample
		//cci_w270_004401026357018_20080313_104113_1.0_masc_ACC03.xml
		TSTRING strXPRSLOGFile = strPath + _T("Qisda_Lavernock_") + strIMEI + strDatetime + _T("1.0.xml");
		xmlDoc.save(strXPRSLOGFile);

		if(nLen > 0 && static_cast<int>(strXPRSLOGFile.size()) < nLen)
			::_tcscpy_s(szPATH,nLen,strXPRSLOGFile.c_str());
		else
			bRes = false;
	}

	return bRes;
}

bool CODM::GetMotoXprsLogger_Athens(TCHAR* szPATH,int nLen)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
		
	TSTRING strException;
	//Get exception
	{
		bRes = WriteExceptionToEFS(0);

		//For get exception file by QLIB function
		strException = QLIB_GetExceptionFile();
	}

	//Get MotoXPRS data
	opxprofile_data MotoXprsData = {0};
	if(bRes)
		bRes = GetMotoXPRSData(MotoXprsData);

	if(bRes)
	{
		TSTRING strIMEI = m_NVItem.GetIMEI();
		for(size_t nIndex = 0;nIndex < 8 && nIndex < strIMEI.size();nIndex++)
			MotoXprsData.ucIMEI[nIndex] = strIMEI[nIndex];
				
		//Get factory data
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		bool bFactoryRes = m_NVItem.ReadNVItem(NV_FACTORY_DATA_1_I,ReadResp);

		TSTRING strDOM1;
		TSTRING strDOM2;
		TSTRING strDOM3;
		if(bFactoryRes)
		{
			//Get ODM
			int nODM1Date = ReadResp.item_data[5];
			int nODM1Month = ReadResp.item_data[6];
			int nODM1Year = ReadResp.item_data[7];
			int nODM2Date = ReadResp.item_data[17];
			int nODM2Month = ReadResp.item_data[18];
			int nODM2Year = ReadResp.item_data[19];
			int nODM3Date = ReadResp.item_data[29];
			int nODM3Month = ReadResp.item_data[30];
			int nODM3Year = ReadResp.item_data[31];

			strDOM1 = INTToString(nODM1Year) + _T("\\") + INTToString(nODM1Month) + _T("\\") + INTToString(nODM1Date);
			strDOM2 = INTToString(nODM2Year) + _T("\\") + INTToString(nODM2Month) + _T("\\") + INTToString(nODM2Date);
			strDOM3 = INTToString(nODM3Year) + _T("\\") + INTToString(nODM3Month) + _T("\\") + INTToString(nODM3Date);

			//Get HW version
			for(int i=89; i <= 93; i++)
				MotoXprsData.HWVer[i-89] = isascii(ReadResp.item_data[i]) ? ReadResp.item_data[i] : 0;

			// 94-103 PICASSO NO (10bytes)
			for(int i=94; i <= 103; i++)
				MotoXprsData.PICASSO[i-94] = ReadResp.item_data[i];

			// 104-113 TRANCEIVER NO (10bytes)
			for(int i=104; i <= 113; i++)
				MotoXprsData.SJUG[i-104] = ReadResp.item_data[i];				
		}

		using namespace XMLMANAGER;
		MSXMLLibrary mslib;
		XMLMANAGER::XMLDocument xmlDoc(&mslib);

		XMLElement MotoXPRSLogElement(_T("MotoXPRSLog"));
		{			
			UpdateElement(MotoXPRSLogElement,_T("LOG_FILE_FORMAT_VERSION"),_T("1.1"));			

			TCHAR szNumBuffer[512] = {0};

			std::vector<NameValue> ProfileValues;
			ProfileValues.push_back(NameValue(_T("IMEI number"),strIMEI));
			ProfileValues.push_back(NameValue(_T("PICASSO number"),ToString(MotoXprsData.PICASSO)));
			ProfileValues.push_back(NameValue(_T("SJUG number"),ToString(MotoXprsData.SJUG)));
			ProfileValues.push_back(NameValue(_T("DOM1"),strDOM1));
			ProfileValues.push_back(NameValue(_T("DOM2"),strDOM2));
			ProfileValues.push_back(NameValue(_T("DOM3"),strDOM3));
			ProfileValues.push_back(NameValue(_T("Product ID"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("TFT on timer"),INTToString(MotoXprsData.uiTFTONCnt)));

			TSTRING strFailByte;
			if(MotoXprsData.cFailByteId == 0)
				strFailByte = _T("00");
			else if(MotoXprsData.cFailByteId == 1)
				strFailByte = _T("01");
			else if(MotoXprsData.cFailByteId == 2)
				strFailByte = _T("11");

			ProfileValues.push_back(NameValue(_T("Fail Byte Identifier"),strFailByte));
			ProfileValues.push_back(NameValue(_T("Software version"),ToString(MotoXprsData.SWVer)));
			ProfileValues.push_back(NameValue(_T("Hardware version"),ToString(MotoXprsData.HWVer)));
			//ProfileValues.push_back(NameValue(_T("Lower Board Id"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("PCB Id"),szNumBuffer));//??
			//ProfileValues.push_back(NameValue(_T("Keypad Board Id"),szNumBuffer));
			ProfileValues.push_back(NameValue(_T("LCM Id"),HEXToString(MotoXprsData.LCMdisplayID)));
			ProfileValues.push_back(NameValue(_T("Flash Id"),HEXToString(MotoXprsData.NANDFlashID)));
			//ProfileValues.push_back(NameValue(_T("Camera Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("INI Files"),GetINIFile()));
			AddProfile(MotoXPRSLogElement,_T("Phone Specific Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of soft reset"),ToString(MotoXprsData.SoftResetCnt)));
			ProfileValues.push_back(NameValue(_T("Count of power cut"),ToString(MotoXprsData.PowerCutCnt)));
			AddProfile(MotoXPRSLogElement,_T("Reset Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Total accumulated call timed"),INTToString(MotoXprsData.uiTotalCallCnt)));
			ProfileValues.push_back(NameValue(_T("Count of dropped calls (call received)"),ToString(MotoXprsData.usIncomingDroppedCallCnt)));
			ProfileValues.push_back(NameValue(_T("Count of dropped calls (called out)"),ToString(MotoXprsData.usOutgoingDroppedCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of calls (incoming)"),ToString(MotoXprsData.usIncomingCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of calls (outgoing)"),ToString(MotoXprsData.usOutgoingCallCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of SMS (incoming)"),ToString(MotoXprsData.usIncomingSMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of SMS (outgoing)"),ToString(MotoXprsData.usOutgoingSMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of MMS (incoming)"),ToString(MotoXprsData.usIncomingMMSCnt)));			
			ProfileValues.push_back(NameValue(_T("Count of MMS (outgoing)"),ToString(MotoXprsData.usOutgoingMMSCnt)));
			AddProfile(MotoXPRSLogElement,_T("Call Information"),ProfileValues);
			
			ProfileValues.push_back(NameValue(_T("KEY_0"),ToString(MotoXprsData.usKEY0Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_1"),ToString(MotoXprsData.usKEY1Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_2"),ToString(MotoXprsData.usKEY2Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_3"),ToString(MotoXprsData.usKEY3Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_4"),ToString(MotoXprsData.usKEY4Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_5"),ToString(MotoXprsData.usKEY5Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_6"),ToString(MotoXprsData.usKEY6Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_7"),ToString(MotoXprsData.usKEY7Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_8"),ToString(MotoXprsData.usKEY8Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_9"),ToString(MotoXprsData.usKEY9Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_STAR"),ToString(MotoXprsData.usKEYStarCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_POUND"),ToString(MotoXprsData.usKEYPoundCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_SOFTKEY_1"),ToString(MotoXprsData.usSoftKEY1Cnt)));
			ProfileValues.push_back(NameValue(_T("KEY_SOFTKEY_2"),ToString(MotoXprsData.usSoftKEY2Cnt)));			
			ProfileValues.push_back(NameValue(_T("KEY_SEND"),ToString(MotoXprsData.usSendKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_END"),ToString(MotoXprsData.usEndKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_POWER"),ToString(static_cast<unsigned short>(0))));//??
			ProfileValues.push_back(NameValue(_T("KEY_VOLUME_UP"),ToString(MotoXprsData.usVolumnUpCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_VOLUME_DOWN"),ToString(MotoXprsData.usVolumnDownCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_UP"),ToString(MotoXprsData.usJoyStickUpCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_DOWN"),ToString(MotoXprsData.usJoyStickdownCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Clear"),ToString(MotoXprsData.usClearKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Menu"),ToString(MotoXprsData.usMenuKEYCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_Smart"),ToString(MotoXprsData.usSmartKEYCnt)));			
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_LEFT"),ToString(MotoXprsData.usJoyStickLeftCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_RIGHT"),ToString(MotoXprsData.usJoyStickRightCnt)));
			AddProfile(MotoXPRSLogElement,_T("Key Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of flip open times"),ToString(MotoXprsData.usFlipOpenCnt)));
			AddProfile(MotoXPRSLogElement,_T("Physical Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Wired headset plug in counter"),ToString(MotoXprsData.usWiredHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Bluetooth headset plug in counter"),ToString(MotoXprsData.usBTHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Camera VGA use counter"),ToString(MotoXprsData.usCameraVGACnt )));
			ProfileValues.push_back(NameValue(_T("Camera 2M use counter"),ToString(MotoXprsData.usCamera2MCnt )));
			ProfileValues.push_back(NameValue(_T("Browser use counter"),ToString(MotoXprsData.usBrowseCnt)));
			ProfileValues.push_back(NameValue(_T("FM Radio use counter"),ToString(MotoXprsData.usFMRadioCnt)));
			ProfileValues.push_back(NameValue(_T("Game use counter"),ToString(MotoXprsData.usGameUsageCnt)));
			ProfileValues.push_back(NameValue(_T("Speaker Phone use counter"),ToString(MotoXprsData.usSpeakerPhoneCnt)));
			ProfileValues.push_back(NameValue(_T("Network Search counter"),ToString(MotoXprsData.usNetworkSearchCnt)));
			ProfileValues.push_back(NameValue(_T("T-Flash plug in counter"),ToString(MotoXprsData.usTFlashXferCnt)));
			//ProfileValues.push_back(NameValue(_T("Charger plug in counter"),ToString(static_cast<unsigned short>(0))));//??
			AddProfile(MotoXPRSLogElement,_T("Timer Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Select Use Time"),ToString(MotoXprsData.usSelectUseTime)));
			ProfileValues.push_back(NameValue(_T("First Use Time"),TimeToString(MotoXprsData.usFirstUseTime)));
			ProfileValues.push_back(NameValue(_T("Last Use Time"),TimeToString(MotoXprsData.usLastUseTime)));			

			ProfileValues.push_back(NameValue(_T("TTF"),ToString(MotoXprsData.TTF)));

			ProfileValues.push_back(NameValue(_T("Number of different SIM card insertions"),ToString(MotoXprsData.usDiffSIMInsertCnt)));
			AddProfile(MotoXPRSLogElement,_T("SIM Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("The number of times a USB cable was connected"),ToString(MotoXprsData.usUSBCABLECnt)));
			ProfileValues.push_back(NameValue(_T("The number of times a wall outlet was connected"),ToString(MotoXprsData.usWallOutletCableCnt)));
			AddProfile(MotoXPRSLogElement,_T("External Entity Information"),ProfileValues);

			int nIndex = 0;
			ProfileValues.push_back(NameValue(_T("0 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("1 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("1 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("1 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("1 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("1 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("1 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("3 bar charging 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("3 bar charging 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("3 bar charging 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("3 bar charging 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("3 bar charging 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("3 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("1 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("2 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 0~1 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 1~2 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 2~3 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 3~4 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 4~5 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 5~6 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 6~9 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 9~12 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 12~15 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 18~21 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			ProfileValues.push_back(NameValue(_T("3 bar plug-in charger 21~24 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
			AddProfile(MotoXPRSLogElement,_T("Charging Information"),ProfileValues);

			nIndex = 0;
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 00 ~ 01 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 01 ~ 02 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 02 ~ 03 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 03 ~ 04 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 04 ~ 05 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 05 ~ 06 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 06 ~ 07 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 07 ~ 08 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 08 ~ 09 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 09 ~ 10 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 10 ~ 11 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 11 ~ 12 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 12 ~ 13 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 13 ~ 14 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 14 ~ 15 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 15 ~ 16 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 16 ~ 17 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 17 ~ 18 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 18 ~ 19 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 19 ~ 20 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 20 ~ 21 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 21 ~ 22 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 22 ~ 23 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 23 ~ 24 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-On  Count at 00 ~ 01 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 01 ~ 02 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 02 ~ 03 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 03 ~ 04 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 04 ~ 05 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 05 ~ 06 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 06 ~ 07 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 07 ~ 08 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 08 ~ 09 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 09 ~ 10 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 10 ~ 11 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 11 ~ 12 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 12 ~ 13 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 13 ~ 14 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 14 ~ 15 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 15 ~ 16 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 16 ~ 17 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 17 ~ 18 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 18 ~ 19 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 19 ~ 20 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 20 ~ 21 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 21 ~ 22 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 22 ~ 23 o'clock"),GetPower(nIndex++,MotoXprsData)));
			ProfileValues.push_back(NameValue(_T("Power-Off  Count at 23 ~ 24 o'clock"),GetPower(nIndex++,MotoXprsData)));
			AddProfile(MotoXPRSLogElement,_T("Power Information"),ProfileValues);
			// 20080903 Embert Tsai
			// Remove special character '01' from exception log.
			TCHAR	chFind = 1;
			TCHAR	chNew = _T(' ');;	// Space
			TSTRING::size_type dwIndex = 0;
			do{
				dwIndex = strException.find(chFind, dwIndex);
				if ( TSTRING::npos != dwIndex){
					strException[dwIndex] = chNew;
				}else{
					break;
				}
			} while (true);
			// End of modification by Embert

			ProfileValues.push_back(NameValue(_T("EXCEPTION LOG"),strException));
			AddProfile(MotoXPRSLogElement,_T("Exception Information"),ProfileValues);
		}

		xmlDoc.getAllElement()->push_back(MotoXPRSLogElement);

		time_t ltime;
		tm today;

		::_tzset();
		::time( &ltime );
		::_localtime64_s( &today, &ltime );

		TCHAR szDatatime[128] = {0};
		::_tcsftime( szDatatime, 128,_T("_%Y%m%d_%H%M%S_"), &today );
		TSTRING strDatetime = szDatatime;

		TCHAR szFullPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szFullPath,MAX_PATH);
		::PathRemoveFileSpec(szFullPath);
		::PathAddBackslash(szFullPath);
		TSTRING strPath = szFullPath;

		//Sample
		//cci_w270_004401026357018_20080313_104113_1.0_masc_ACC03.xml
		TSTRING strXPRSLOGFile = strPath + _T("Qisda_VE538_") + strIMEI + strDatetime + _T("1.0.xml");
		xmlDoc.save(strXPRSLOGFile);

		if(nLen > 0 && static_cast<int>(strXPRSLOGFile.size()) < nLen)
			::_tcscpy_s(szPATH,nLen,strXPRSLOGFile.c_str());
		else
			bRes = false;
	}

	return bRes;
}

TSTRING CODM::ToString(unsigned char* szValue)
{
	return (char *)(szValue);
}

TSTRING CODM::HEXToString(unsigned int usValue)
{
	TCHAR szNumBuffer[50] = {0};
	//::sprintf(szNumBuffer,_T("%d"),IntEndianTransfer(usValue));
	sprintf_s(szNumBuffer,50,_T("%0X"),(usValue));
	return szNumBuffer;
}

#include <time.h>

TSTRING CODM::TimeToString(unsigned int usValue)
{
	//Qualcomm's base date 1980 / 1 / 6 00:00:00 GMT
	struct tm tQualcomm = { 0, 0, 0, 6, 1, 80 };
	time_t tQualcommBase = ::mktime( &tQualcomm );
	//Win32's base date 1970 / 1 / 1 00:00:00 GMT
	struct tm tWin32 = { 0, 0, 0, 1, 1, 70 };
	time_t tWIN32Base = ::mktime( &tWin32 );

	double elapsed_time = ::difftime( tQualcommBase, tWIN32Base );

	TCHAR szBuf[128] = {0};
	time_t ltime = usValue + elapsed_time;

	struct tm tTime;
	//The long value timer represents the seconds elapsed since midnight (00:00:00), January 1, 1970, UTC
	::_localtime64_s( &tTime, &ltime );

	::_tcsftime( szBuf, 128,_T("%Y\\%m\\%d %H:%M:%S"), &tTime );

	return szBuf;
}

TSTRING CODM::INTToString(unsigned int usValue)
{
	TCHAR szNumBuffer[50] = {0};
	//::sprintf(szNumBuffer,_T("%d"),IntEndianTransfer(usValue));
	sprintf_s(szNumBuffer,50,_T("%d"),(usValue));
	return szNumBuffer;
}

TSTRING CODM::ToString(unsigned short usValue)
{	
	TCHAR szNumBuffer[50] = {0};
	//::sprintf(szNumBuffer,_T("%u"),ShortEndianTransfer(usValue));
	sprintf_s(szNumBuffer,50,_T("%u"),(usValue));
	return szNumBuffer;
}

TSTRING CODM::GetCharger(int nIndex,opxprofile_data_Melos& MotoXprsData)
{
	unsigned short nShortUSB = MotoXprsData.usChargingProfile[nIndex];
	unsigned short nShortPower = MotoXprsData.usChargingProfile[nIndex+72];
	return ToString(nShortUSB + nShortPower);
}

TSTRING CODM::GetPower(int nIndex,opxprofile_data_Melos& MotoXprsData)
{
	unsigned short nPower = MotoXprsData.usPowerProfile[nIndex];
	return ToString(nPower);
}

TSTRING CODM::GetCharger(int nIndex,opxprofile_data_Lavernock& MotoXprsData)
{
	unsigned short nShortUSB = MotoXprsData.usChargingProfile[nIndex];
	unsigned short nShortPower = MotoXprsData.usChargingProfile[nIndex+72];
	return ToString(nShortUSB + nShortPower);
}

TSTRING CODM::GetPower(int nIndex,opxprofile_data_Lavernock& MotoXprsData)
{
	unsigned short nPower = MotoXprsData.usPowerProfile[nIndex];
	return ToString(nPower);
}

TSTRING CODM::GetCharger(int nIndex,opxprofile_data& MotoXprsData)
{
	unsigned short nShortUSB = MotoXprsData.usChargingProfile[nIndex];
	unsigned short nShortPower = MotoXprsData.usChargingProfile[nIndex+72];
	return ToString(nShortUSB + nShortPower);
}

TSTRING CODM::GetPower(int nIndex,opxprofile_data& MotoXprsData)
{
	unsigned short nPower = MotoXprsData.usPowerProfile[nIndex];
	return ToString(nPower);
}

#include "../../QLib_Export/QLib_export.h"

void CODM::QLIB_Reset()
{
	TemporaryCloseComport tempClose(this);

	QL_Reset(tempClose.GetComport());
	Sleep(15000);
}

bool CODM::QLIB_CapturePicture_ForSPU(char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile)
{
	TemporaryCloseComport tempClose(this);
	char szPort[20] = {0};
	sprintf_s(szPort,20,"%d",tempClose.GetComport());
	return QL_STL_CapturePicture_ForSPU(szPort,sznSensorID,sznWidth,sznHeight,strTargetFile);
}

#include <fstream>
#include <iostream>

bool CODM::Vibrator(bool bEnable,bool& bSuccess)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
		
	CRequestPtr ptRequest = new CVibratorRequest(bEnable,bSuccess);
	bRes = Execute(ptRequest);

	if(bRes == false || bSuccess == false)
		PopLastLog();

	return bRes;
}

bool CODM::GSensorOffset(bool& bSuccess,int& nOffsetX,int& nOffsetY,int& nOffsetZ)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
		
	CRequestPtr ptRequest = new CGSensorRequest(bSuccess,nOffsetX,nOffsetY,nOffsetZ);
	bRes = Execute(ptRequest);

	if(bRes == false || bSuccess == false)
		PopLastLog();

	return bRes;
}

bool CODM::GSensorCalibration(bool& bSuccess,int& nRet)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
		
	CRequestPtr ptRequest = new CGSensorRequest(7,nRet,bSuccess);
	bRes = Execute(ptRequest);

	if(bRes == false || bSuccess == false)
		PopLastLog();

	return bRes;
}

bool CODM::IsGSensorWork(int& nX,int& nY,int& nZ)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
		
	CRequestPtr ptRequest = new CGSensorRequest(nX,nY,nZ,bRes);
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::IsGSensorWork(bool& bIsGSensorWorking,bool& bIsUpWorking)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	int nTemp = 0;	
	CRequestPtr ptRequest = new CGSensorRequest(bIsGSensorWorking,bIsUpWorking,nTemp);	
	if(Execute(ptRequest))
	{
		bRes = bIsGSensorWorking && bIsUpWorking;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::QLIB_ReplaceFile(const std::string& strEFSFilePath, const std::string& strNewLocalFilePath)
{
	TemporaryCloseComport tempClose(this);

	bool bRes = false;

	bRes = Q_DeleteFile(strEFSFilePath.c_str(),tempClose.GetComport());
	bRes = Q_UploadFile(strNewLocalFilePath.c_str(),strEFSFilePath.c_str(),tempClose.GetComport());

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::QLIB_GetCEFSFilename(char* szBuffer,int szBufferSize)
{
	TemporaryCloseComport tempClose(this);

	char szFolder[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szFolder,CSIDL_COMMON_TEMPLATES,"CEFS");
	string strTargetFolder = szFolder;
	string strCEFSDATAName = "qvcefs.dat";
	
	char szCOMPort[20] = {0};
	sprintf_s(szCOMPort,20,"%04d",tempClose.GetComport());
	string strTargetFile = strTargetFolder + szCOMPort + strCEFSDATAName;

	strCEFSDATAName = "/brew/qvcefs.dat";
	bool bRes = Q_STL_DownloadFile(strTargetFile.c_str(),strCEFSDATAName.c_str(),tempClose.GetComport());
	if(bRes)
	{
		ifstream file;
		file.open( strTargetFile.c_str());
		if(file && szBuffer && szBufferSize > 0)
		{
			file.getline(szBuffer,szBufferSize);
		}
		file.close( );
	}
	return true;
}

TSTRING CODM::QLIB_GetExceptionFile()
{
	TemporaryCloseComport tempClose(this);

	TSTRING strException = _T("None");

	char szFolder[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szFolder,CSIDL_COMMON_TEMPLATES,"Exception");
	string strTargetFolder = szFolder;

	string strLogIndexFileName = "exp_log_index.txt";
	string strTargetFile = strTargetFolder + strLogIndexFileName;
	bool bRes = Q_STL_DownloadFile(strTargetFile.c_str(),strLogIndexFileName.c_str(),tempClose.GetComport());
	if(bRes)
	{
		//exp_log_index.txt => 0 0
		//exp_log_0_0.txt
		ifstream file;
		char cIndex1,cIndex2;
		// Open and close with a basic_filebuf
		file.open( strTargetFile.c_str());
		file >> cIndex1;
		file >> cIndex2;
		file.close( );

		string strLatestLogFileName = "exp_log_";
		strLatestLogFileName += cIndex1;
		strLatestLogFileName += "_";
		strLatestLogFileName += cIndex2;
		strLatestLogFileName += ".txt";

		string strTargetFile = strTargetFolder + strLatestLogFileName;

		bRes = Q_STL_DownloadFile(strTargetFile.c_str(),strLatestLogFileName.c_str(),tempClose.GetComport());
		if(bRes)
		{
			strException.clear();
			ifstream file;
			// Open and close with a basic_filebuf
			file.open( strTargetFile.c_str());
			while(file)
			{				
				char szBuf[1024] = {0};
				file.getline(szBuf,1024);

				//Transfer to XML format
				/*
				string strTemp = szBuf;
				for(string::size_type nIndex = 0;nIndex < strTemp.size();nIndex++)
				{
					switch(strTemp[nIndex])
					{
					case '<':strException += "&lt;";break;
					case '>':strException += "&gt;";break;
					case '&':strException += "&amp;";break;
					case '\'':strException += "&apos;";break;
					case '"':strException += "&quot;";break;
					default:strException += strTemp[nIndex];break;
					};
				};
				*/
				strException += szBuf;
				strException += _T("\n");
			};
			file.close( );
		}
	}

	return strException;
}

bool CODM::WriteSimData(BYTE *pSimDataBuf, int nSimDataBufLen)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	int nStat = 0;
	CRequestPtr ptRequest = new CWriteSIMDataRequest(pSimDataBuf,nSimDataBufLen,nStat);	
	if(Execute(ptRequest))	
		bRes = nStat == 0;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::WriteFailureByte(BYTE nFailureByte, int nType)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD nFunction = 2;
	BYTE byteFailure = nFailureByte;
	bool bSuccess = false;
	CRequestPtr ptRequest = new CFailureByteRequest(nFunction,byteFailure,bSuccess,nType);	
	if(Execute(ptRequest))
	{
		bRes = bSuccess;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CODM::WriteDefaultPhoneCode()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	bool bSuccess = false;
	CRequestPtr ptRequest = new CWriteDefaultPhoneCodeRequest(bSuccess);	
	if(Execute(ptRequest))
	{
		bRes = bSuccess;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::FreezeOTP()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	bool bSuccess = false;
	CRequestPtr ptRequest = new CFreezeOTPRequest(bSuccess);	
	if(Execute(ptRequest))
	{
		bRes = bSuccess;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}


bool CODM::IsOTPFrozen(int& nLastError)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	bool bSuccess = false;
	CRequestPtr ptRequest = new CQueryFreezeOTPRequest(bSuccess,nLastError);	
	if(Execute(ptRequest))
		bRes = bSuccess;
	else
		PopLastLog();

	return bRes;
}

bool CODM::DLRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CDLRAWRequest(szCmdInput, nCmdLength, bAddCRC);	
	bRes = Execute(ptRequest);
	return bRes;
}

bool CODM::DiagRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CDiagRAWRequest(szCmdInput, nCmdLength, bAddCRC,bHasReturn);	
	if(Execute(ptRequest))
		bRes = true;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::ByPassCellTemperatureCheckRequest()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CByPassCellTemperatureCheckRequest;
	if(Execute(ptRequest))
		bRes = true;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::GetLavernockHWVersion(int& nVersion)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CAskLavernockVarientRequest(nVersion);
	if(Execute(ptRequest))
		bRes = true;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::GetLavernockHWVersionV2(int& nVarientVersion,int& nEVTVersion)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CAskLavernockVarientRequest(nVarientVersion,nEVTVersion);
	if(Execute(ptRequest))
		bRes = true;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::KeypadBacklight(bool bEnable)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CKeypadRequest(4,bEnable,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::KeypadPressTest(bool bEnable)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CKeypadRequest(5,bEnable,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::USBChargingSwitch(bool bEnable)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CChargingSwitchRequest(6,bEnable,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::CoinCellChargingSwitch(bool bEnable)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CChargingSwitchRequest(4,bEnable,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::DisplayTest(short Color565,BYTE byteBacklight)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CDisplayRequest(Color565,byteBacklight,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::GetCoincellVoltage(int& nVoltage,bool& bSuccess)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CReadCoincellVoltageRequest(nVoltage,bSuccess);
	bRes = Execute(ptRequest) && bSuccess;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::GetFlipStatus(bool& bOpen)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CFlipStatusRequest(bOpen,bRes);
	bRes = Execute(ptRequest) && bRes;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::CaptureScreenshot()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CCaptureScreenshotRequest();
	bRes = Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::BatteryTemperatureDetectionSwitch(bool bEnable)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CBatteryTemperatureDetectionSwitchRequest(bEnable,bRes);
	bRes = Execute(ptRequest) && bRes;

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CODM::ATEnableKeypadEvent(ISPU_CALLBACK *pISPU_CALLBACK,int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CATEnableKeypadEventRequest(true,bRes,nTimeout,pISPU_CALLBACK);
	bRes = Execute(ptRequest) && bRes;

	if(bRes == false)
		PopLastLog();

	return bRes;
}