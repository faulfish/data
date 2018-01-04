#include "StdAfx.h"
#include "1FFNameMaker.h"

CMulti1FFNameMaker::CMulti1FFNameMaker(void) : 
	m_strMarketName(_T("WellFleet"))
#ifdef Lavernock
	,m_strNamingRuleINIFilePath(_T(".\\Multi1FFMakerSetting_Lavernock.ini"))
#endif
#ifdef Melos
	,m_strNamingRuleINIFilePath(_T(".\\Multi1FFMakerSetting_Melos.ini"))
#endif

{
}

CMulti1FFNameMaker::~CMulti1FFNameMaker(void)
{
}

bool CMulti1FFNameMaker::GetMotName(TSTRING& strMotName)
{
	using namespace QSYNC_LIB;
	bool bRes = true;

	strMotName = m_strMarketName;

	vector<TSTRING>::iterator iter;
	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
		TSTRING strTemp = (*iter).substr(0, 5);
		if(strTemp == _T("AMSS_"))
			break;
	}
	vector<TSTRING> vecStrAMSS;
	Tokenize<TSTRING>(_T("_"), *iter, vecStrAMSS, false);
	strMotName += _T("_") + vecStrAMSS.at(2); //SIGNED or UNSIGNED
	strMotName += _T("_[") + vecStrAMSS.at(3) + _T("]"); //SW Version

	TSTRING strLOCFileName;
	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
		CString strFileFullName = (*iter).c_str();
		CString strFileNameExtension = strFileFullName.Mid(strFileFullName.ReverseFind(_T('.')) + 1);
		if(strFileNameExtension.MakeLower() == _T("du"))
		{
			strLOCFileName = (TSTRING)strFileFullName.Left(strFileFullName.ReverseFind(_T('.')));
			break;
		}
	}
	vector<TSTRING> vecStrVar;
	Tokenize<TSTRING>(_T("_"), strLOCFileName, vecStrVar, false);
	strMotName += _T("_[") + vecStrVar.at(1); //Country(Region)
	strMotName += _T("_") + vecStrVar.at(2); //Customer

	if(vecStrVar.at(5).substr(0, 4) == _T("LP00"))
		strMotName += _T("_LG") + vecStrVar.at(5).substr(4, 2); //Language pack
	else
		strMotName += _T("_") + vecStrVar.at(5); //Language pack

	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
		CString strFileFullName = (*iter).c_str();
		CString strFileNameExtension = strFileFullName.Mid(strFileFullName.ReverseFind(_T('.')) + 1);
		if(strFileNameExtension.MakeLower() == _T("loc"))
		{
			strLOCFileName = strFileFullName.Left(strFileFullName.ReverseFind(_T('.')));
			break;
		}
	}
	vector<TSTRING> vecStrLoc;
	Tokenize<TSTRING>(_T("_"), *iter, vecStrLoc, false);
	strMotName += _T("_") + strLOCFileName + _T("]"); //LOC filename

	strMotName += _T(".mot");
	return bRes;
}

void GetINISetting(const CString& strINIFileName,const CString& strSection, map<CString, CString>* pMapSectionContent)
{
	pMapSectionContent->clear();
	const DWORD dwBufferSize = 10240;
	TCHAR ac_Result[dwBufferSize] = {0};  //change size depending on needs	
	DWORD lRetValue = ::GetPrivateProfileSection(strSection, ac_Result, dwBufferSize,strINIFileName);
	if (lRetValue == 0)
	{
		ASSERT(0);
	}
	if(lRetValue == (dwBufferSize-2))
	{
		ASSERT(0);//need to expand buffer size
	}

	CString strKey;
	CString strValue;
	bool bKeyFlag = true;
	for(DWORD i=0; i<lRetValue; ++i)
	{
		if(ac_Result[i] != _T('\0')) 
		{
			if (ac_Result[i] == _T('='))
			{
				bKeyFlag = false;
				continue;
			}
			(bKeyFlag ? strKey : strValue) += ac_Result[i] ;
		} 
		else 
		{
			bKeyFlag = true;
			pMapSectionContent->insert( pair<CString, CString>(strKey, strValue) );
			strKey.Empty();
			strValue.Empty();
		}
	}
}

//bool CMulti1FFNameMaker::GetMotNameEx(TSTRING& strMotName)
//{
//	using namespace QSYNC_LIB;
//	bool bRes = true;
//	map<CString, CString> MapSWNamingRule;
//	GetINISetting(m_strNamingRuleINIFilePath, _T("SWNamingRule"), &MapSWNamingRule);
//	for(map<CString, CString>::iterator iter = MapSWNamingRule.begin(); iter != MapSWNamingRule.end(); ++iter)	{
//		vector<CString> vecSWNamingRule;
//		Tokenize<CString>(_T("_"), iter->second, vecSWNamingRule, false);
//		m_MapSWNamingRule.insert(pair<CString,  vector<CString>>(iter->first,vecSWNamingRule));
//	}
//	map<CString, CString> MapVariantNamingRule;
//	GetINISetting(m_strNamingRuleINIFilePath, _T("VariantNamingRule"), &MapVariantNamingRule);
//	for(map<CString, CString>::iterator iter = MapVariantNamingRule.begin(); iter != MapVariantNamingRule.end(); ++iter)	{
//		vector<CString> vecVariantNamingRule;
//		Tokenize<CString>(_T("_"), iter->second, vecVariantNamingRule, false);
//		m_MapVariantNamingRule.insert(pair<CString,  vector<CString>>(iter->first,vecVariantNamingRule));
//	}
//	map<CString, CString> MapLockNamingRule;
//	GetINISetting(m_strNamingRuleINIFilePath, _T("LockNamingRule"), &MapLockNamingRule);
//	for(map<CString, CString>::iterator iter = MapLockNamingRule.begin(); iter != MapLockNamingRule.end(); ++iter)	{
//		vector<CString> vecLockNamingRule;
//		Tokenize<CString>(_T("_"), iter->second, vecLockNamingRule, false);
//		m_MapLockNamingRule.insert(pair<CString,  vector<CString>>(iter->first,vecLockNamingRule));
//	}
//	GetINISetting(m_strNamingRuleINIFilePath, _T("OutputNamingRule"), &m_MapOutputNamingRule);
//
//	int nIndex = 0;
//	CString strApp(_T("App0")), strValue(_T("Value0"));
//	while (bRes && m_MapOutputNamingRule.find(strApp) != m_MapOutputNamingRule.end() && m_MapOutputNamingRule.find(strValue) != m_MapOutputNamingRule.end())
//	{
//		CString strResult;
//		bRes = MakeStringByRule(m_MapOutputNamingRule[strApp], m_MapOutputNamingRule[strValue], strResult);
//		if(bRes)
//			strMotName += (TSTRING)strResult;
//
//		++nIndex;
//		strApp.Format(_T("App%d"), nIndex);
//		strValue.Format(_T("Value%d"), nIndex);
//	}
//	return bRes;
//}
//
//bool CMulti1FFNameMaker::MakeStringByRule(const CString& strApp, const CString& strValue, CString& strResult)
//{
//	using namespace QSYNC_LIB;
//	bool bRes = true;
//	bool bFlagGetFileName = true;
//	CString strFileName;
//	if(strApp == _T("SWNamingRule")) //用檔頭辨識檔案
//	{
//		vector<TSTRING>::iterator iter ;
//		for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
//			TSTRING strTemp = (*iter).substr(0, strApp.GetLength());
//			if(strTemp.c_str() == strApp)	{
//				strFileName = (*iter).c_str();
//				break;
//			}
//		}
//	}
//	else if(strApp == _T("VariantNamingRule") || strApp == _T("LockNamingRule")) //用副檔名辨識檔案
//	{
//		vector<TSTRING>::iterator iter ;
//		for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
//			CString strFileNameExtension = (*iter).c_str();
//			strFileNameExtension = strFileNameExtension.Mid(strFileNameExtension.ReverseFind(_T('.')) + 1);
//			if(strFileNameExtension == strApp)	{
//				strFileName = (*iter).c_str();
//				break;
//			}
//		}
//	}
//	else
//		bFlagGetFileName = false;
//
//	if(bFlagGetFileName)
//	{
//		vector<CString> vecString;
//		strFileName = strFileName.Left(strFileName.ReverseFind(_T('.')));
//		Tokenize<CString>(_T("+"), strFileName, vecString, false);
//		for (vector<CString>::iterator iter = vecString.begin(); iter != vecString.end(); ++iter){
//			vector<CString> vecSection;
//			Tokenize<CString>(_T(":"), *iter, vecSection, false);
//			if(vecSection.at(0) == _T("UserDefined"))
//				strResult += vecSection.at(1);
//			//else if(vecSection.)
//		}
//	}
//
//	return bRes;
//}
//
bool CMulti1FFNameMaker::GetMotName_3(TSTRING& strMotName)
{
	using namespace QSYNC_LIB;
	bool bRes = true;
	GetINISetting(m_strNamingRuleINIFilePath, _T("OutputNamingRule"), &m_MapOutputNamingRule);

	int nIndex = 0;
	CString strApp(_T("App0")), strValue(_T("Value0"));
	while (bRes && m_MapOutputNamingRule.find(strApp) != m_MapOutputNamingRule.end() && m_MapOutputNamingRule.find(strValue) != m_MapOutputNamingRule.end())
	{
		CString strResult;
		bRes = MakeStringByRule_3(m_MapOutputNamingRule[strApp], m_MapOutputNamingRule[strValue], strResult);
		if(bRes)
			strMotName += (TSTRING)strResult;

		++nIndex;
		strApp.Format(_T("App%d"), nIndex);
		strValue.Format(_T("Value%d"), nIndex);
	}
	if (bRes)
		strMotName += _T(".") + m_MapOutputNamingRule[_T("Extension")];
	return bRes;
}

bool CMulti1FFNameMaker::MakeStringByRule_3(const CString& strApp, const CString& strValue, CString& strResult)
{
	using namespace QSYNC_LIB;
	bool bRes = true;
	

	CString strFileName;
	if (strApp != _T("SW") && strApp != _T("Variant") && strApp != _T("Lock"))
	{
		TCHAR szPath[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, szPath); 
		const DWORD dwBufferSize = 100;
		TCHAR ac_Result[dwBufferSize] = {0};  //change size depending on needs	
		DWORD lRetValue = ::GetPrivateProfileString(strApp,strValue,NULL,ac_Result,dwBufferSize,m_strNamingRuleINIFilePath);
		ASSERT(lRetValue != 0);
		::SetCurrentDirectory(szPath);

		if (lRetValue == 0)
		{
			ASSERT(0);	bRes = false;
		}
		if(lRetValue == (dwBufferSize-2))
		{
			ASSERT(0);	bRes = false; //need to expand buffer size
		}
		strResult = ac_Result;
	}
	else
		
	//	if(strApp == _T("SWNamingRule")) //用檔頭辨識檔案
	//{
	//	vector<TSTRING>::iterator iter ;
	//	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
	//		TSTRING strTemp = (*iter).substr(0, strApp.GetLength());
	//		if(strTemp.c_str() == strApp)	{
	//			strFileName = (*iter).c_str();
	//			break;
	//		}
	//	}
	//}
	//else if(strApp == _T("VariantNamingRule") || strApp == _T("LockNamingRule")) //用副檔名辨識檔案
	//{
	//	vector<TSTRING>::iterator iter ;
	//	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
	//		CString strFileNameExtension = (*iter).c_str();
	//		strFileNameExtension = strFileNameExtension.Mid(strFileNameExtension.ReverseFind(_T('.')) + 1);
	//		if(strFileNameExtension == strApp)	{
	//			strFileName = (*iter).c_str();
	//			break;
	//		}
	//	}
	//}

	//if(bFlagGetPrivateSection == false)
	{
		//strFileName = strFileName.Left(strFileName.ReverseFind(_T('.')));//去掉副檔名
		//vector<CString> vecFileSection;
		//Tokenize<CString>(_T("_"), strFileName, vecFileSection, false);

		vector<CString> vecString;
		Tokenize<CString>(_T("+"), strValue, vecString, false);

		for (vector<CString>::iterator iter = vecString.begin(); iter != vecString.end(); ++iter){
			vector<CString> vecSection;
			Tokenize<CString>(_T(":"), *iter, vecSection, false);
			if(vecSection.at(0) == _T("UserDefined"))
				strResult += vecSection.at(1);
			else	{
				CString strFileName;
				 if (strApp == _T("SW"))
					 bRes = FindFileNameByHeader(vecSection.at(0), strFileName);
				 else
					 bRes = FindFileNameByExtension(vecSection.at(0), strFileName);

				 if (bRes == false)
				 {
					 AfxMessageBox(_T("Naming rule format is wrong!!"));
					 break;
				 }

				 vector<CString> vecFileNameSection;
				 Tokenize<CString>(_T("_"), strFileName, vecFileNameSection, false);
				 if(vecSection.size() == 2)
					 if (vecSection.at(1) == _T("WholeName"))
						 strResult += strFileName;
					 else
						 strResult += vecFileNameSection.at(_ttoi(vecSection.at(1)));
				 else if (vecSection.size() == 3) {
					 int nIndex = _ttoi(vecSection.at(2));
					 strResult += vecFileNameSection.at(_ttoi(vecSection.at(1))).Mid(nIndex, 1);
				 }
				 else	 {
					 bRes = false;
					 AfxMessageBox(_T("Naming rule format is wrong!!"));
				 }
			}
		}// end for
	}

	return bRes;
}

bool CMulti1FFNameMaker::FindFileNameByHeader(const CString& strHeader, CString& strFileName)
{
	bool bRes = false;
	vector<TSTRING>::iterator iter ;
	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
		TSTRING strTemp = (*iter).substr(0, strHeader.GetLength());
		if(bRes = strTemp.c_str() == strHeader)	{
			strFileName = (*iter).c_str();
			break;
		}
	}
	if(bRes)
		strFileName = strFileName.Left(strFileName.ReverseFind(_T('.'))); //去掉副檔名

	return bRes;
}

bool CMulti1FFNameMaker::FindFileNameByExtension(const CString& strFileNameExtension, CString& strFileName)
{
	bool bRes = false;
	vector<TSTRING>::iterator iter ;
	for (iter = m_vstrFiles.begin(); iter != m_vstrFiles.end(); ++iter){
		CString strTemp = (*iter).c_str();
		strTemp = strTemp.Mid(strTemp.ReverseFind(_T('.')) + 1);
		if(bRes = strFileNameExtension == strTemp)	{
			strFileName = (*iter).c_str();
			break;
		}
	}
	if(bRes)
		strFileName = strFileName.Left(strFileName.ReverseFind(_T('.'))); //去掉副檔名

	return bRes;
}