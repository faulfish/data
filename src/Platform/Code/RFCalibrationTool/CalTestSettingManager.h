#pragma once

#include "MyMSXML.h"
#include "CommonUtil.h"
#include "GPIB_Device.h"

class CCalTestSettingManager
{
// Operations
public:
	void SetMessageHandlingHwnd(CONST HWND hWnd);
	void SetComPortNum(CONST UINT nPortNum);
	void SetTesterDevice(eGPIBDeviceType testDevice);
	void SetPowerSupplyDevice(eGPIBDeviceType powerDevice, INT gpibAddr);
	void SetCalTestItems(CONST CString& strFunc, CONST CStringArray& vecItems);
	BOOL ConvertFrom(CMyMSXML* pRFXML, CMyMSXML* pConfigXML);	
	BOOL CheckPowerSupplyValid();
	PCConfigSetting GetConfigSetting() {return &m_ConfigSettings;}

// Operations
private:
	BOOL GetDeviceSetting(CMyMSXML* pRFXML);
	BOOL GetRFProcessor(CMyMSXML* pRFXML);
	BOOL GetRFProcessorListAndProcessObj(CMyMSXML* pRFXML, CONST CString& strBand, CONST CStringArray& vecBandItems);

	BOOL AddTxLinMasterObjToProcessObject(CMyMSXML* pRFXML);

	BOOL GetRFCable(CMyMSXML* pConfigXML);
	BOOL GetEquipmentSetting(CMyMSXML* pConfigXML);
	
	BOOL SaveDeviceSettingToMapFromRFXML(CMyMSXML* pRFXML, LPCTSTR NodePath, StrStrMapPairVtr& vecStore);
	BOOL SaveProcessObjectToMapFromRFXML(CMyMSXML* pRFXML, LPCTSTR NodePath, StrStrMapPairVtr& vecStore);

	BOOL SaveRFCableDataToMapFromConfigXML(CMyMSXML* pXML, LPCTSTR NodePath, StrStrMapPairVtr& vecStore);	

	BOOL SaveGPIBAddrToMapFromConfigXML(CMyMSXML* pXML, CIntIntMap& mapStore);
	BOOL SaveEquipmentTestSetFromConfigXML(CMyMSXML* pXML, CStrStrMap& mapStore);

	BOOL SaveTesterDeviceToConfig();
	BOOL SavePowerSupplyDeviceToConfig();	

	void Clear();

// Attributes
private:
	CConfigSetting m_ConfigSettings;
	CString m_strProcFunc;
	CStringArray m_vecGSMBand;
	CStringArray m_vecWCDMABand;
	eGPIBDeviceType m_TesterDevice;
	eGPIBDeviceType m_PowerDevice;
	INT m_PowerDeviceGPIBAddr;

public:
	CCalTestSettingManager(void);
	virtual ~CCalTestSettingManager(void);
};
