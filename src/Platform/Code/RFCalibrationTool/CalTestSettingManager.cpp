#include "StdAfx.h"
#include "CalTestSettingManager.h"
#include "typedef.h"

CCalTestSettingManager::CCalTestSettingManager(void)
{
	m_ConfigSettings.m_hMessageHandlingWnd = 0;
	m_ConfigSettings.m_bForcePass = FALSE;
	m_strProcFunc = _T("");
	m_TesterDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerDeviceGPIBAddr = 0;
}

CCalTestSettingManager::~CCalTestSettingManager(void)
{
	Clear();
}

void CCalTestSettingManager::Clear()
{
	m_TesterDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerDeviceGPIBAddr = 0;
	m_ConfigSettings.vProcessSettingMapPair.clear();
	m_ConfigSettings.vDeviceSettingMapPair.clear();
	m_ConfigSettings.cMainRFSettingMap.Clear();

	m_ConfigSettings.cRFEquipmentConfig.cGPIBAddressMap.Clear();
	m_ConfigSettings.cRFEquipmentConfig.cEquipmentTestSettings.Clear();
	m_ConfigSettings.cRFEquipmentConfig.vRFCableRXMapPair.clear();
	m_ConfigSettings.cRFEquipmentConfig.vRFCableTXMapPair.clear();
}

void CCalTestSettingManager::SetMessageHandlingHwnd(CONST HWND hWnd)
{
	m_ConfigSettings.m_hMessageHandlingWnd = hWnd;
}

void CCalTestSettingManager::SetComPortNum(CONST UINT nPortNum)
{
	m_ConfigSettings.m_nComPORT = nPortNum;
}

void CCalTestSettingManager::SetTesterDevice(eGPIBDeviceType testerDevice)
{
	m_TesterDevice = testerDevice;
}

void CCalTestSettingManager::SetPowerSupplyDevice(eGPIBDeviceType powerDevice, INT gpibAddr)
{
	m_PowerDevice = powerDevice;
	m_PowerDeviceGPIBAddr = gpibAddr;
}

void CCalTestSettingManager::SetCalTestItems(CONST CString& strFunc, CONST CStringArray& vecItems)
{
	m_strProcFunc = strFunc;

	m_vecGSMBand.RemoveAll();
	m_vecWCDMABand.RemoveAll();
	for (int i = 0; i < vecItems.GetSize(); i++) {
		CString strBand = vecItems.GetAt(i);
		if (strBand.Find(_T("GSM")) != -1) {
			strBand.Replace(_T("GSM"), _T(""));
			strBand.Trim();
			m_vecGSMBand.Add(strBand);
		}
		else if (strBand.Find(_T("WCDMA")) != -1) {
			strBand.Replace(_T("WCDMA"), _T(""));
			strBand.Trim();
			m_vecWCDMABand.Add(strBand);
		}
		else
			ASSERT(0);
	}
}

BOOL CCalTestSettingManager::ConvertFrom(CMyMSXML* pRFXML, CMyMSXML* pConfigXML)
{
	ASSERT(pRFXML != NULL && pConfigXML != NULL);
	if (!pRFXML->IsLoad() || !pConfigXML->IsLoad())
		return FALSE;

	GetDeviceSetting(pRFXML);
	GetRFProcessor(pRFXML);
	GetRFProcessorListAndProcessObj(pRFXML, _T("GSM"), m_vecGSMBand);
	GetRFProcessorListAndProcessObj(pRFXML, _T("WCDMA"),  m_vecWCDMABand);

	AddTxLinMasterObjToProcessObject(pRFXML); // special case
	
	GetEquipmentSetting(pConfigXML);
	GetRFCable(pConfigXML);

	// post-processing; auto-set the tester device and power supply
	SaveTesterDeviceToConfig();
//	SavePowerSupplyDeviceToConfig();	

	return TRUE;
}

BOOL CCalTestSettingManager::CheckPowerSupplyValid()
{
	BOOL bFound = FALSE;
	Map<int, int> &GPIBAddr = m_ConfigSettings.cRFEquipmentConfig.cGPIBAddressMap;
	Map<int, int>::KeyValueMapItr itInt2Int;
	for (itInt2Int = GPIBAddr.Begin(); itInt2Int != GPIBAddr.End(); itInt2Int++) {
		if (itInt2Int->second == m_PowerDeviceGPIBAddr) {
			bFound = TRUE;
		}
	}
	
	return bFound;
}

BOOL CCalTestSettingManager::GetDeviceSetting(CMyMSXML* pRFXML)
{
	ASSERT(pRFXML != NULL);
	if (pRFXML == NULL)
		return FALSE;

	SaveDeviceSettingToMapFromRFXML(pRFXML, _T("//PredefinedObjectSet"), m_ConfigSettings.vDeviceSettingMapPair);	

	return TRUE;
}

BOOL CCalTestSettingManager::GetRFProcessor(CMyMSXML* pRFXML)
{
	XMLNodeList nodeList = pRFXML->GetChildNodes(_T("//CRFCalTestProcessor"));
	for (UINT i = 0; i < pRFXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pRFXML->GetNodeListItem(nodeList, i);

		if (pRFXML->GetNodeType(node) != XMLNodeType_Element)
			continue;

		CString strName = pRFXML->GetNodeName(node);
		if (strName != _T("List")) {
			CString strValue = pRFXML->GetNodeText(node);
			String strTemp = strValue;
			m_ConfigSettings.cMainRFSettingMap.Add((LPCTSTR)strName, &strTemp);
		}
	}

	return TRUE;
}

BOOL CCalTestSettingManager::GetRFProcessorListAndProcessObj(CMyMSXML* pRFXML, CONST CString& strBand, CONST CStringArray& vecBandItems)
{
	if (vecBandItems.GetSize() == 0)
		return TRUE;

	// set all items to false 
	XMLNodeList nodeList = pRFXML->Search(_T("//CRFCalTestProcessor/List/*"));
	for (UINT i = 0; i < pRFXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pRFXML->GetNodeListItem(nodeList, i);
		CString strNodeBand = pRFXML->GetNodeAttrValue(node, _T("Type"));
		CString strNodeFunc = pRFXML->GetNodeAttrValue(node, _T("Group"));

		if (strNodeBand == strBand && m_strProcFunc.Find(strNodeFunc) != -1)
			pRFXML->SetNodeAttrValue(node, _T("Enabled"), _T("FALSE"));
	}

	// check item one by one whether an item contains CalItems(800, 900,...)
	CString strEnabledItems;
	for (UINT i = 0; i < pRFXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pRFXML->GetNodeListItem(nodeList, i);		
		CString strNodeBand = pRFXML->GetNodeAttrValue(node, _T("Type"));
		CString strNodeFunc = pRFXML->GetNodeAttrValue(node, _T("Group"));

		if (strNodeBand != strBand || m_strProcFunc.Find(strNodeFunc) == -1)
			continue;

		CString strNodeName = pRFXML->GetNodeAttrValue(node, _T("Name"));
		BOOL bToCal = FALSE;

		if (strNodeName.Find(_T("MeasureLoop")) != -1) {	// special case
			CString strNodeBandNum = strNodeName;
			strNodeBandNum.Replace(_T("MeasureLoop"), _T(""));
			strNodeBandNum.Replace(strBand, _T(""));
			strNodeBandNum.Trim();
			
			for (INT iBandIdx = 0; iBandIdx < vecBandItems.GetSize(); iBandIdx++) {
				if (strNodeBandNum == vecBandItems.GetAt(iBandIdx)) {
					bToCal = TRUE;
					break;
				}
			}
		}
		else {
			CString strProcessObjBandListXMLPath;
			strProcessObjBandListXMLPath.Format(_T("//ProcessObject[@Name=\"%s\"]/List/*"), strNodeName);
			XMLNodeList nodeProcessObjBandList = pRFXML->Search(strProcessObjBandListXMLPath);

			if (pRFXML->GetNodeListSize(nodeProcessObjBandList) == 0) // means fundamental and necessary object
				bToCal = TRUE;
			else {
				for (UINT iBandListIdx = 0; iBandListIdx < pRFXML->GetNodeListSize(nodeProcessObjBandList); iBandListIdx++) {
					XMLNode nodeBand = pRFXML->GetNodeListItem(nodeProcessObjBandList, iBandListIdx);
					CString strBandName = pRFXML->GetNodeAttrValue(nodeBand, _T("Name"));

					BOOL bToCalItem = FALSE;
					for (INT iBandIdx = 0; iBandIdx < vecBandItems.GetSize(); iBandIdx++) {
						if (strBandName == vecBandItems.GetAt(iBandIdx)) {
							bToCal = TRUE;
							bToCalItem = TRUE;
							break;
						}
					}

					if (bToCalItem)
						pRFXML->SetNodeAttrValue(nodeBand, _T("Enabled"), _T("TRUE"));
					else
						pRFXML->SetNodeAttrValue(nodeBand, _T("Enabled"), _T("FALSE"));
				}
			}
		}

		if (bToCal) {
			pRFXML->SetNodeAttrValue(node, _T("Enabled"), _T("FALSE"));

			if (strEnabledItems.IsEmpty())
				strEnabledItems = strNodeName;
			else
				strEnabledItems = strEnabledItems + _T(",") + strNodeName;

			CString strProcessObjXMLPath;
			strProcessObjXMLPath.Format(_T("//ProcessObject[@Name=\"%s\"]"), strNodeName);
			SaveProcessObjectToMapFromRFXML(pRFXML, strProcessObjXMLPath, m_ConfigSettings.vProcessSettingMapPair);
		}
	}
	
	PString pstrOriList = m_ConfigSettings.cMainRFSettingMap.Lookup(_T("List"));
	if (pstrOriList != NULL)
		strEnabledItems = CString(pstrOriList->c_str()) + _T(",") + strEnabledItems;;
		
	String strTemp = strEnabledItems;
	m_ConfigSettings.cMainRFSettingMap.Add(_T("List"), &strTemp);

	return TRUE;
}

BOOL CCalTestSettingManager::AddTxLinMasterObjToProcessObject(CMyMSXML* pRFXML)
{	
	BOOL bAddTxLinMasterObj = FALSE;
	StrStrMapPairVtr::iterator itTxRxSmartAlignmentCal;
	for (StrStrMapPairVtr::iterator ix = m_ConfigSettings.vProcessSettingMapPair.begin(); ix != m_ConfigSettings.vProcessSettingMapPair.end(); ix++) {
		PCStrStrMap pcSettingMap = &(ix->second);
		PString pstrObjName = pcSettingMap->Lookup(_T("Name"));
		if (pstrObjName == NULL)
			continue;

		if ((*pstrObjName) == _T("TxRxSmartAlignmentCal")) {
			bAddTxLinMasterObj = TRUE;
			itTxRxSmartAlignmentCal = ix;
			break;
		}
	}

	if (!bAddTxLinMasterObj)
		return TRUE;

	CString strTxLinMasterXMLPath;
	strTxLinMasterXMLPath.Format(_T("//ProcessObject[@Name=\"TxLinMaster\"]"));
	XMLNode node = pRFXML->SearchNode(strTxLinMasterXMLPath);

	if (pRFXML->GetNodeType(node) != XMLNodeType_Element)
		return TRUE;

	CString strClsName = pRFXML->GetNodeAttrValue(node, _T("ClassName"));		
	CStrStrMap mapAttrList;
	String strTemp = _T("TxLinMaster");
	mapAttrList.Add(_T("Name"), &strTemp);

	XMLNodeList nodeChildList = pRFXML->GetChildNodes(node);
	for (UINT j = 0; j < pRFXML->GetNodeListSize(nodeChildList); j++) {
		XMLNode nodeAttr = pRFXML->GetNodeListItem(nodeChildList, j);

		if (pRFXML->GetNodeType(nodeAttr) != XMLNodeType_Element)
			continue;

		CString strAttrName = pRFXML->GetNodeName(nodeAttr);
		CString strAttrValue;

		if (strAttrName == _T("List")) {
			// Get node items of Enabled=True
			XMLNodeList nodeItemList = pRFXML->GetChildNodes(nodeAttr);

			for (UINT k = 0; k < pRFXML->GetNodeListSize(nodeItemList); k++) {
				XMLNode nodeItem = pRFXML->GetNodeListItem(nodeItemList, k);
				CString strEnabled = pRFXML->GetNodeAttrValue(nodeItem, _T("Enabled"));

				if (strEnabled.CompareNoCase(_T("TRUE")) == 0){
					CString nodeItemName = pRFXML->GetNodeAttrValue(nodeItem, _T("Name"));

					if (strAttrValue.IsEmpty())
						strAttrValue = nodeItemName;
					else
						strAttrValue = strAttrValue + _T(",") + nodeItemName;
				}					
			}
		}
		else
			strAttrValue = pRFXML->GetNodeText(nodeAttr);

		strTemp = strAttrValue;
		mapAttrList.Add((LPCTSTR)strAttrName, &strTemp);
	}

	// insert TxLinMaster before TxRxSmartAlignmentCal
	m_ConfigSettings.vProcessSettingMapPair.insert(itTxRxSmartAlignmentCal, make_pair(strClsName, mapAttrList));

	return TRUE;
}

BOOL CCalTestSettingManager::GetRFCable(CMyMSXML* pConfigXML)
{
	ASSERT(pConfigXML != NULL);
	if (pConfigXML == NULL)
		return FALSE;

	XMLNodeList nodeList = pConfigXML->Search(_T("//RfSettings/RfCable/CurrentRfLosses"));
	if (pConfigXML->GetNodeListSize(nodeList) != 1)
		return FALSE;

	XMLNode node = pConfigXML->GetNodeListItem(nodeList, 0);
	CString strCurrentRFLoss = pConfigXML->GetNodeText(node);

	CString strRFLossSettingXMLPath;
	strRFLossSettingXMLPath.Format(_T("//RfSettings/RfCable/RfLosses[@name=\"%s\"]"), strCurrentRFLoss);

	SaveRFCableDataToMapFromConfigXML(pConfigXML, strRFLossSettingXMLPath + _T("/RfType[@name=\"RX\"]"), m_ConfigSettings.cRFEquipmentConfig.vRFCableRXMapPair);
	SaveRFCableDataToMapFromConfigXML(pConfigXML, strRFLossSettingXMLPath + _T("/RfType[@name=\"TX\"]"), m_ConfigSettings.cRFEquipmentConfig.vRFCableTXMapPair);

	return TRUE;
}

BOOL CCalTestSettingManager::GetEquipmentSetting(CMyMSXML* pConfigXML)
{
	ASSERT(pConfigXML != NULL);
	if (pConfigXML == NULL)
		return FALSE;

	SaveGPIBAddrToMapFromConfigXML(pConfigXML, m_ConfigSettings.cRFEquipmentConfig.cGPIBAddressMap);
	SaveEquipmentTestSetFromConfigXML(pConfigXML, m_ConfigSettings.cRFEquipmentConfig.cEquipmentTestSettings);

	return TRUE;
}

BOOL CCalTestSettingManager::SaveDeviceSettingToMapFromRFXML(CMyMSXML* pRFXML, LPCTSTR NodePath, StrStrMapPairVtr& vecStore)
{
	XMLNodeList nodeList = pRFXML->GetChildNodes(NodePath);
	for (UINT i = 0; i < pRFXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pRFXML->GetNodeListItem(nodeList, i);

		if (pRFXML->GetNodeType(node) != XMLNodeType_Element)
			continue;
		
		CString strClsName = pRFXML->GetNodeAttrValue(node, _T("ClassName"));
		CString strName = pRFXML->GetNodeAttrValue(node, _T("Name"));		
		CStrStrMap mapAttrList;
		String strTemp;

		strTemp = strName;
		mapAttrList.Add(_T("Name"),&strTemp);

		XMLNodeList nodeChildList = pRFXML->GetChildNodes(node);
		for (UINT j = 0; j < pRFXML->GetNodeListSize(nodeChildList); j++) {
			XMLNode nodeAttr = pRFXML->GetNodeListItem(nodeChildList, j);

			if (pRFXML->GetNodeType(nodeAttr) != XMLNodeType_Element)
				continue;

			CString strAttrName = pRFXML->GetNodeName(nodeAttr);
			CString strAttrValue;

			if (strAttrName == _T("List")) {
				// Get node items of Enabled=True
				XMLNodeList nodeItemList = pRFXML->GetChildNodes(nodeAttr);

				for (UINT k = 0; k < pRFXML->GetNodeListSize(nodeItemList); k++) {
					XMLNode nodeItem = pRFXML->GetNodeListItem(nodeItemList, k);
					CString strEnabled = pRFXML->GetNodeAttrValue(nodeItem, _T("Enabled"));
					
					if (strEnabled.CompareNoCase(_T("TRUE")) == 0){
						CString nodeItemName = pRFXML->GetNodeAttrValue(nodeItem, _T("Name"));

						if (strAttrValue.IsEmpty())
							strAttrValue = nodeItemName;
						else
							strAttrValue = strAttrValue + _T(",") + nodeItemName;
					}					
				}
			}
			else
				strAttrValue = pRFXML->GetNodeText(nodeAttr);

			strTemp = strAttrValue;
			mapAttrList.Add((LPCTSTR)strAttrName, &strTemp);
		}

		vecStore.push_back(make_pair(strClsName, mapAttrList));
	}

	return TRUE;
}

BOOL CCalTestSettingManager::SaveProcessObjectToMapFromRFXML(CMyMSXML* pRFXML, LPCTSTR NodePath, StrStrMapPairVtr& vecStore)
{	
	XMLNode node = pRFXML->SearchNode(NodePath);

	// only get the items listed and enabled in the <CRFCalTestProcessor>/<List> list
	CString strName = pRFXML->GetNodeAttrValue(node, _T("Name"));
	CString strClsName = pRFXML->GetNodeAttrValue(node, _T("ClassName"));		
	CStrStrMap mapAttrList;
	String strTemp;

	strTemp = strName;
	mapAttrList.Add(_T("Name"),&strTemp);

	XMLNodeList nodeChildList = pRFXML->GetChildNodes(node);
	for (UINT j = 0; j < pRFXML->GetNodeListSize(nodeChildList); j++) {
		XMLNode nodeAttr = pRFXML->GetNodeListItem(nodeChildList, j);

		if (pRFXML->GetNodeType(nodeAttr) != XMLNodeType_Element)
			continue;

		CString strAttrName = pRFXML->GetNodeName(nodeAttr);
		CString strAttrValue;

		if (strAttrName == _T("List")) {
			// Get node items of Enabled=True
			XMLNodeList nodeItemList = pRFXML->GetChildNodes(nodeAttr);

			for (UINT k = 0; k < pRFXML->GetNodeListSize(nodeItemList); k++) {
				XMLNode nodeItem = pRFXML->GetNodeListItem(nodeItemList, k);
				CString strEnabled = pRFXML->GetNodeAttrValue(nodeItem, _T("Enabled"));

				if (strEnabled.CompareNoCase(_T("TRUE")) == 0){
					CString nodeItemName = pRFXML->GetNodeAttrValue(nodeItem, _T("Name"));

					if (strAttrValue.IsEmpty())
						strAttrValue = nodeItemName;
					else
						strAttrValue = strAttrValue + _T(",") + nodeItemName;
				}					
			}
		}
		else
			strAttrValue = pRFXML->GetNodeText(nodeAttr);

		strTemp = strAttrValue;
		mapAttrList.Add((LPCTSTR)strAttrName, &strTemp);
	}

	vecStore.push_back(make_pair(strClsName, mapAttrList));

	return TRUE;
}

BOOL CCalTestSettingManager::SaveRFCableDataToMapFromConfigXML(CMyMSXML* pXML, LPCTSTR NodePath, StrStrMapPairVtr& vecStore)
{
	XMLNodeList nodeList = pXML->GetChildNodes(NodePath);
	for (UINT i = 0; i < pXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pXML->GetNodeListItem(nodeList, i);

		if (pXML->GetNodeType(node) != XMLNodeType_Element)
			continue;

		CString strBand = pXML->GetNodeAttrValue(node, _T("name"));
		CStrStrMap mapList;

		XMLNodeList nodeChildList = pXML->GetChildNodes(node);
		for (UINT j = 0; j < pXML->GetNodeListSize(nodeChildList); j++) {
			XMLNode nodeChild = pXML->GetNodeListItem(nodeChildList, j);
			CString strChannel = pXML->GetNodeAttrValue(nodeChild, _T("name"));

			CString strRfLossXMLPath;
			strRfLossXMLPath.Format(_T("%s/RfBand[@name=\"%s\"]/RfChannel[@name=\"%s\"]/RfLoss"), NodePath, strBand, strChannel);

			XMLNodeList nodeRfLosses = pXML->Search(strRfLossXMLPath);
			if (pXML->GetNodeListSize((nodeRfLosses)) != 1)
				continue;

			XMLNode nodeRfLoss = pXML->GetNodeListItem(nodeRfLosses, 0);
			String strRfLoss = pXML->GetNodeText(nodeRfLoss);

			mapList.Add((LPCTSTR)strChannel, &strRfLoss);
		}

		vecStore.push_back(make_pair(strBand, mapList));
	}

	return TRUE;
}

BOOL CCalTestSettingManager::SaveGPIBAddrToMapFromConfigXML(CMyMSXML* pXML, CIntIntMap& mapStore)
{
	XMLNodeList nodeList = pXML->Search(_T("//EquipmentSettings/PowerSupplyId"));
	for (UINT i = 0; i < pXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pXML->GetNodeListItem(nodeList, i);
		CString strPowerSupplyId = pXML->GetNodeAttrValue(node, _T("name"));
		
		CString strGpibAddressXMLPath;
		strGpibAddressXMLPath.Format(_T("//EquipmentSettings/PowerSupplyId[@name=\"%s\"]/PowerSupply/GpibAddress"), strPowerSupplyId);

		XMLNodeList nodeTempList = pXML->Search(strGpibAddressXMLPath);
		if (pXML->GetNodeListSize(nodeTempList) != 1)
			continue;

		XMLNode nodeGpibAddress = pXML->GetNodeListItem(nodeTempList, 0);
		CString strGpibAddress = pXML->GetNodeText(nodeGpibAddress);
	
		int iGpibAddress = _ttoi(strGpibAddress);
		mapStore.Add(_ttoi(strPowerSupplyId), &iGpibAddress);
	}

	return TRUE;
}

BOOL CCalTestSettingManager::SaveEquipmentTestSetFromConfigXML(CMyMSXML* pXML, CStrStrMap& mapStore)
{
	XMLNodeList nodeList = pXML->GetChildNodes(_T("//EquipmentSettings/TestSet"));
	for (UINT i = 0; i < pXML->GetNodeListSize(nodeList); i++) {
		XMLNode node = pXML->GetNodeListItem(nodeList, i);
		CString strItemName = pXML->GetNodeName(node);
		String strItemValue = pXML->GetNodeText(node);

		mapStore.Add((LPCTSTR)strItemName, &strItemValue);
	}

	return TRUE;
}

BOOL CCalTestSettingManager::SaveTesterDeviceToConfig()
{
	if (m_TesterDevice == GPIB_DEVICE_UNKNOWN)
		return TRUE;

	PString pstrDeviceName = m_ConfigSettings.cMainRFSettingMap.Lookup(_T("TesterDevice"));
	if (pstrDeviceName == NULL)
		return FALSE;

	CString strCurDevice;
	switch (m_TesterDevice) {
		case RsCmu200:
			strCurDevice = _T("CMU200");
			break;
		case WillTek:
			strCurDevice = _T("WillTek");
			break;
		case Hp8960:
			strCurDevice = _T("Hp8960");
			break;
		default:
			ASSERT(0);
			break;
	}

	String strCurDeviceTmp = strCurDevice;

	m_ConfigSettings.cMainRFSettingMap.Add(_T("TesterDevice"), &strCurDeviceTmp);
	return TRUE;
}

BOOL CCalTestSettingManager::SavePowerSupplyDeviceToConfig()
{
	if (m_PowerDevice == GPIB_DEVICE_UNKNOWN)
		return TRUE;

	PString pstrPowerSupplyName = m_ConfigSettings.cMainRFSettingMap.Lookup(_T("PowerSupply"));
	if (pstrPowerSupplyName == NULL)
		return FALSE;

	CString strCurDevice;
	switch (m_PowerDevice) {
		case HpE3631:
			strCurDevice = _T("HpE3631");
			break;
		case Hp66311:
			strCurDevice = _T("Hp66311");
			break;
		case Hp66332A:
			strCurDevice = _T("Hp66332A");
			break;
		default:
			ASSERT(0);
			break;
	}

	String strCurDeviceTmp = strCurDevice;

	m_ConfigSettings.cMainRFSettingMap.Add(_T("PowerSupply"), &strCurDeviceTmp);
	return TRUE;
}