#include "StdAfx.h"
#include "ODM.h"
#include "..\RequestManager\Request\ODMRequest.h"
#include <time.h>
#include "..\CommonUtil\MSXMLImpl.h"
#pragma comment(lib, "comsuppw.lib")

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

TSTRING CODM::GetIMEI()
{
	char szIMEI100[16]  = {0};
	DIAG_NVITEM_PACKET_F_type IMEIReadResp = {0};
	bool bIMEIRes = m_NVItem.ReadNVItem(NV_UE_IMEI_I,IMEIReadResp);	
	if(bIMEIRes && IMEIReadResp.nv_stat==0)
	{
		char sDigitIMEI[128*2+1] = {0};
		HexBcdToDigit(IMEIReadResp.item_data, 128, sDigitIMEI, 128*2+1);		
		for(int i=0; i<15; i++)
			szIMEI100[i] = sDigitIMEI[i+3];
	}
	return szIMEI100;
}

bool CODM::GetMotoXprsLogger(TCHAR* szPATH,int nLen)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	bool bFactoryRes = m_NVItem.ReadNVItem(NV_FACTORY_DATA_1_I,ReadResp);

	CRequestPtr ptRequest = new CMotoRequestBase();	
	bRes = Execuate(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);

		opxprofile_data MotoXprsData = {0};
		::memcpy(&MotoXprsData,ptBufferReceived->GetData() + 4,sizeof(opxprofile_data));

		//For sample only
		//For test use QLIB function
		//QLIB_Reset();

		TSTRING strIMEI = GetIMEI();
		for(size_t nIndex = 0;nIndex < 8 && nIndex < strIMEI.size();nIndex++)
			MotoXprsData.ucIMEI[nIndex] = strIMEI[nIndex];

		TSTRING strDOM1;
		TSTRING strDOM2;
		TSTRING strDOM3;
		if(bFactoryRes)
		{
			//Get SW version
			//for(int i=85; i <= 88; i++)
			//	MotoXprsData.SWVer[i-85] = ReadResp.item_data[i];

			//Get ODM
			int nODM1Date = ReadResp.item_data[20];
			int nODM1Month = ReadResp.item_data[21];
			int nODM1Year = ReadResp.item_data[22];
			int nODM2Date = ReadResp.item_data[23];
			int nODM2Month = ReadResp.item_data[24];
			int nODM2Year = ReadResp.item_data[25];
			int nODM3Date = ReadResp.item_data[26];
			int nODM3Month = ReadResp.item_data[27];
			int nODM3Year = ReadResp.item_data[28];

			strDOM1 = INTToString(nODM1Year) + _T("\\") + INTToString(nODM1Month) + _T("\\") + INTToString(nODM1Date);
			strDOM2 = INTToString(nODM2Year) + _T("\\") + INTToString(nODM2Month) + _T("\\") + INTToString(nODM2Date);
			strDOM3 = INTToString(nODM3Year) + _T("\\") + INTToString(nODM3Month) + _T("\\") + INTToString(nODM3Date);

			//Get HW version
			for(int i=89; i <= 93; i++)
				MotoXprsData.HWVer[i-89] = ReadResp.item_data[i];

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
			ProfileValues.push_back(NameValue(_T("Lower Board Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("PCB Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("Keypad Board Id"),szNumBuffer));
			ProfileValues.push_back(NameValue(_T("LCM Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("Flash Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("Camera Id"),szNumBuffer));//??
			ProfileValues.push_back(NameValue(_T("EXCEPTION LOG"),szNumBuffer));//??
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
			//usClearKEYCnt??
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_LEFT"),ToString(MotoXprsData.usJoyStickLeftCnt)));
			ProfileValues.push_back(NameValue(_T("KEY_JOYSTICK_RIGHT"),ToString(MotoXprsData.usJoyStickRightCnt)));
			AddProfile(MotoXPRSLogElement,_T("Key Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Count of flip open times"),ToString(MotoXprsData.usFlipOpenCnt)));
			AddProfile(MotoXPRSLogElement,_T("Physical Information"),ProfileValues);

			ProfileValues.push_back(NameValue(_T("Wired headset plug in counter"),ToString(MotoXprsData.usWiredHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Bluetooth headset plug in counter"),ToString(MotoXprsData.usBTHeadsetCnt)));
			ProfileValues.push_back(NameValue(_T("Camera use counter"),ToString(MotoXprsData.usCameraVGACnt + MotoXprsData.usCamera2MCnt)));//??
			ProfileValues.push_back(NameValue(_T("Browser use counter"),ToString(MotoXprsData.usBrowseCnt)));
			ProfileValues.push_back(NameValue(_T("FM Radio use counter"),ToString(MotoXprsData.usFMRadioCnt)));
			ProfileValues.push_back(NameValue(_T("Game use counter"),ToString(MotoXprsData.usGameUsageCnt)));
			ProfileValues.push_back(NameValue(_T("Speaker Phone use counter"),ToString(MotoXprsData.usSpeakerPhoneCnt)));
			ProfileValues.push_back(NameValue(_T("Network Search counter"),ToString(MotoXprsData.usNetworkSearchCnt)));
			ProfileValues.push_back(NameValue(_T("T-Flash plug in counter"),ToString(MotoXprsData.usTFlashXferCnt)));
			ProfileValues.push_back(NameValue(_T("Charger plug in counter"),ToString(static_cast<unsigned short>(0))));//??
			AddProfile(MotoXPRSLogElement,_T("Timer Information"),ProfileValues);

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
			ProfileValues.push_back(NameValue(_T("4 bar charging 5~ hour counter"),GetCharger(nIndex++,MotoXprsData)));
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
			ProfileValues.push_back(NameValue(_T("0 bar plug-in charger 15~18 hour counter"),GetCharger(nIndex++,MotoXprsData)));			
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

		//Sample
		//cci_w270_004401026357018_20080313_104113_1.0_masc_ACC03.xml
		TSTRING strXPRSLOGFile = _T("c:\\Qisda_Wellfleet_") + strIMEI + strDatetime + _T("1.0_ACC03.xml");
		xmlDoc.save(strXPRSLOGFile);

		if(nLen > 0 && strXPRSLOGFile.size() < nLen)
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

TSTRING CODM::INTToString(unsigned int usValue)
{
	TCHAR szNumBuffer[50] = {0};
	::sprintf(szNumBuffer,_T("%u"),IntEndianTransfer(usValue));
	return szNumBuffer;
}

TSTRING CODM::ToString(unsigned short usValue)
{	
	TCHAR szNumBuffer[50] = {0};
	::sprintf(szNumBuffer,_T("%u"),ShortEndianTransfer(usValue));
	return szNumBuffer;
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

#include "../QLib_Export/QLib_export.h"

void CODM::QLIB_Reset()
{
	TemporaryCloseComport tempClose(this);

	QL_Reset(90);
	Sleep(15000);
}