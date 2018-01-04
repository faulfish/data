#pragma once
#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\QisdaNVModule\NVItem.h"
#include <string>
#include <vector>
#include "..\..\CommonUtil\XMLManager.h"
#include "Header_MotoXPRS.h"
#include "..\..\FactoryTool\SPU\ISPU_CALLBACK.h"

#ifdef ODM_COACH_MODULE_EXPORTS
#define ODM_COACH_API extern "C" __declspec(dllexport)
#define ODM_COACH_CLASS_API __declspec(dllexport)
#else
#define ODM_COACH_API extern "C" __declspec(dllimport)
#define ODM_COACH_CLASS_API __declspec(dllimport)
#endif

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class ODM_COACH_CLASS_API CODM :public CInterfaceBase
{
	class NameValue
	{
		friend class CODM;
	public:
		NameValue(const TSTRING& strName,const TSTRING& strValue):m_strName(strName),m_strValue(strValue){}
	protected:
		TSTRING m_strName;
		TSTRING m_strValue;
	};
public:
	CODM(int nCOMPort);
	virtual ~CODM();

	virtual bool EnableDiagEvent(bool bEnable);

	bool GetLavernockHWVersion(int& nVersion);
	bool GetLavernockHWVersionV2(int& nVarientVersion,int& nEVTVersion);
	bool GetMotoXprsLogger_Athens(TCHAR* szPATH,int nLen);//	Get MotoXprs Logger
	bool GetMotoXprsLogger_Lavernock(TCHAR* szPATH,int nLen);//	Get MotoXprs Logger
	bool GetMotoXprsLogger_Melos(TCHAR* szPATH,int nLen);//	Get MotoXprs Logger
	bool WriteFailureByte(BYTE nFailureByte,int nType = 0);
	bool QLIB_ReplaceFile(const std::string& strEFSFilePath, const std::string& strNewLocalFilePath);
	bool QLIB_CapturePicture_ForSPU(char* sznSensorID,char* sznWidth,char* sznHeight,char* strTargetFile);
	
	TSTRING GetINIFile();
	bool WriteSimData(BYTE *pSimDataBuf, int nSimDataBufLen);
	bool GetMotoXPRSData(opxprofile_data_Melos& MotoXprsData);
	bool GetMotoXPRSData(opxprofile_data_Lavernock& MotoXprsData);
	bool GetMotoXPRSData(opxprofile_data& MotoXprsData);
	bool WriteDefaultPhoneCode();

	bool FreezeOTP();
	bool IsOTPFrozen(int& nLastError);
	bool IsGSensorWork(bool& bIsGSensorWorking,bool& bIsUpWorking);
	bool IsGSensorWork(int& nX,int& nY,int& nZ);
	bool GSensorCalibration(bool& bSuccess,int& nRet);
	bool GSensorOffset(bool& bSuccess,int& nOffsetX,int& nOffsetY,int& nOffsetZ);
	bool QLIB_GetCEFSFilename(char* szBuffer,int szBufferSize);
	bool Vibrator(bool bEnable,bool& bSuccess);
	bool GetCoincellVoltage(int& nVoltage,bool& bSuccess);
	bool GetFlipStatus(bool& bOpen);

	bool DLRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
	bool DiagRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn);
	bool ByPassCellTemperatureCheckRequest();
	bool USBChargingSwitch(bool bEnable);
	bool CoinCellChargingSwitch(bool bEnable);
	bool BatteryTemperatureDetectionSwitch(bool bEnable);
	bool KeypadBacklight(bool bEnable);
	bool KeypadPressTest(bool bEnable);
	bool DisplayTest(short Color565,BYTE byteBacklight);

	bool CaptureScreenshot();

	bool ATEnableKeypadEvent(ISPU_CALLBACK *pISPU_CALLBACK,int nTimeout);
	
protected:
	bool WriteExceptionToEFS(int nVersion);

private:
	void UpdateElement(XMLMANAGER::XMLElement& Element,const TSTRING& strName,const TSTRING& strValue);
	void AddProfile(XMLMANAGER::XMLElement& Element,const TSTRING& strProfileName,std::vector<NameValue>& ProfileValues);
	TSTRING GetCharger(int nIndex,opxprofile_data_Melos& MotoXprsData);
	TSTRING GetPower(int nIndex,opxprofile_data_Melos& MotoXprsData);
	TSTRING GetCharger(int nIndex,opxprofile_data_Lavernock& MotoXprsData);
	TSTRING GetPower(int nIndex,opxprofile_data_Lavernock& MotoXprsData);
	TSTRING GetCharger(int nIndex,opxprofile_data& MotoXprsData);
	TSTRING GetPower(int nIndex,opxprofile_data& MotoXprsData);
	TSTRING ToString(unsigned short usValue);
	TSTRING ToString(unsigned char* szValue);
	TSTRING INTToString(unsigned int usValue);
	TSTRING HEXToString(unsigned int usValue);
	TSTRING TimeToString(unsigned int usValue);

	void QLIB_Reset();
	TSTRING QLIB_GetExceptionFile();	
private:
	CNVItem		m_NVItem;
};
