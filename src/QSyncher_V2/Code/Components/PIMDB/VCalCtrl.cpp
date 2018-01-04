#include "StdAfx.h"
#include "VCalCtrl.h"

namespace QSYNC_LIB {
CVCalCtrl::CVCalCtrl(IPIMObjPt PIMObjPt):m_PIMObjPt(PIMObjPt), m_pIPIMConfig(NULL)
{
	CreateILocalization(&m_pILocale);
}

CVCalCtrl::~CVCalCtrl(void)
{
	m_pIPIMConfig = NULL;
}

std::wstring CVCalCtrl::GetWString( const char * szName )// force to return a value
{
	std::wstring strRes = L"";
	CommonDataPt sptemData = new CommonData;
	size_t npos = -1;
	size_t nGet = m_PIMObjPt->GetByName(szName, sptemData);
	
	if(nGet != npos)
	{
#ifdef _UNICODE
		strRes = sptemData->GetWString(); 
#else
		const char* cptmp =  sptemData->GetString();
		strRes = CharToWideChar( cptmp, CP_UTF8 );
#endif
	}
	return strRes;
}

bool CVCalCtrl::SetCmnWStrData(const char * szName, const wchar_t* szData)
{
	bool bRes = false;

	size_t npos = -1;
	TSTRING strData;
	if(szData)
	{
#ifdef _UNICODE
		strData = szData;
#else
		strData = WideCharToChar(szData, CP_ACP);
#endif
	}
	bool bCheckLimitation = CheckLimitation(szName, strData);
	assert(bCheckLimitation);
	if(bCheckLimitation)
	{
		CommonDataPt sptemData = new CommonData;
		sptemData->SetName(szName);
		
	#ifdef _UNICODE
		sptemData->SetWString(strData.c_str()); 
	#else
		sptemData->SetString(strData.c_str()); 
	#endif
		bRes = npos != m_PIMObjPt->SetByName(szName, sptemData);
	}
	return bRes;
}

bool CVCalCtrl::SetCmnStrData(const char * szName, const TCHAR* szData)
{	
	bool bRes = false;
	size_t npos = -1;
	TSTRING strData;
	if(szData)
		strData = szData;
	bool bCheckLimitation = CheckLimitation(szName, strData);
	assert(bCheckLimitation);
	if(bCheckLimitation)
	{
		CommonDataPt sptemData = new CommonData;
		sptemData->SetName(szName);
		
	#ifdef _UNICODE
		sptemData->SetWString(strData.c_str()); 
	#else
		sptemData->SetString(strData.c_str()); 
	#endif
		bRes = npos != m_PIMObjPt->SetByName(szName, sptemData);
	}
	return bRes;
}
bool CVCalCtrl::SetCmnIntData(const char * szName, int nData)
{	
	size_t npos = -1;
	CommonDataPt sptemData = new CommonData;
	sptemData->SetName(szName);
	sptemData->SetInteger(nData);
	bool bSet = npos != m_PIMObjPt->SetByName(szName, sptemData) ? true : false;
	return bSet;
}
bool CVCalCtrl::GetCmnStrData(const char* cpKey, CString& strCmnData)
{
	bool bRet = false;
	CommonDataPt spData = NULL;
	size_t npos = -1;
	size_t nGet = m_PIMObjPt->GetByName(cpKey, spData);
	if(nGet != npos)
	{
		CommonDataType datatype = spData->GetType();
		if(datatype == WString)
		{
	#ifdef _UNICODE
			strCmnData = spData->GetWString();
	#endif

		}
		else if(datatype == String)
		{
	#ifndef _UNICODE
			strCmnData = spCommonData->GetString();
	#endif
		}
		bRet = true;
	}
	else
	{
		strCmnData = _T("");
	}
	return bRet;
}
bool CVCalCtrl::GetCmnIntData( const char * szName, long& nInt )
{
	CommonDataPt sptemData = new CommonData;
	size_t npos = -1;
	bool bRes = false;
	size_t nGet = m_PIMObjPt->GetByName(szName,sptemData);
	if(nGet != npos)
	{	
		nInt = sptemData->GetInteger();
		bRes = true;
	}
	return bRes;
}

bool CVCalCtrl::SetEmptyIPIMObjPt(void) 
{
	bool bRes = false;

	// Merlin Interface
	TSTRING szFakeVCalendar = _T(\
"BEGIN:VCALENDAR\r\n\
VERSION:1.0\r\n\
BEGIN:VEVENT\r\n\
SUMMARY;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:none\r\n\
DESCRIPTION;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
LOCATION;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
CATEGORIES:\r\n\
DTSTART:20000101T000000Z\r\n\
DTEND:20000101T000000Z\r\n\
DALARM:;P0000Y00M00DT00H00M00S;000\r\n\
RRULE:\r\n\
UID:0\r\n\
END:VEVENT\r\n\
END:VCALENDAR");

	if(m_PIMObjPt)
		bRes = m_PIMObjPt->decode(szFakeVCalendar);

	return bRes;
}



bool CVCalCtrl::SaveCmnData(const char* cpKey, const CString& strValue)
{
	bool bRet = true;
	CString strGetValue = _T("");
	bool bGet = GetCmnStrData(cpKey, strGetValue);
	
	if(strValue.GetLength() > 0)
		bRet = SmartAddPIMEntity(cpKey, strValue);	
	else if(bGet && strValue.GetLength() == 0)
		bRet = SmartDeletePIMEntity(cpKey);

	return bRet;
}

bool CVCalCtrl::SmartAddPIMEntity(const char* cpKey, const CString& strValue)
{
	bool bRet = true;
	CStringA strKey = cpKey;
	if(strKey == KEYWORD_SUBJECT || strKey == KEYWORD_DESCRIPTION || strKey == KEYWORD_ATTENDEE || strKey == KEYWORD_LOCATION)
	{
		bRet = AddPIMParas(cpKey, true, true);
	}
	else if(strKey == KEYWORD_DALARM)
	{
		bRet = AddPIMOtherValues(cpKey);
	}
	if(bRet)
	{
		bRet = AddPIMProptyName(cpKey);
		if(bRet)
			bRet = SetCmnStrData(cpKey, strValue);
	}
	return bRet;
}

bool CVCalCtrl::AddPIMParas(const char* cpKey, bool bEncoding, bool bCharset)
{
	bool bRet = true;
	if(bEncoding || bCharset)
	{
		TSTRING strKey, strFirst, strSecond, strKeyQP, strKeyCharset, strKeyParaName= _T("");
		CString strValue = _T("");
		
		strKey = ASCIItoUnicode(cpKey);
		strKeyParaName = StrConnect<TSTRING>(_T(":"), strKey, KEYWORD_PARAMETER_NAME, false);
		strKeyQP = StrConnect<TSTRING>(_T(":"), strKey, PARAMETER_NAME_ENCODING, false);
		strKeyCharset = StrConnect<TSTRING>(_T(":"), strKey, PARAMETER_NAME_CHARSET, false);
		
		GetCmnStrData( (LPCSTR)UnicodetoASCII(strKeyParaName.c_str()), strValue);
		if(bEncoding)
		{
			if(GetRepeatCount<CString>(_T(";"),PARAMETER_NAME_ENCODING,strValue) == 0)
				strValue = StrConnect<CString>( _T(";"), strValue, PARAMETER_NAME_ENCODING, false);
			SetCmnStrData((LPCSTR)UnicodetoASCII(strKeyQP.c_str()), PARAMETER_VALUE_QP);
		}
		if(bCharset)
		{
			if(GetRepeatCount<CString>(_T(";"),PARAMETER_NAME_CHARSET,strValue) == 0)
				strValue = StrConnect<CString>( _T(";"), strValue, PARAMETER_NAME_CHARSET, false);
			SetCmnStrData((LPCSTR)UnicodetoASCII(strKeyCharset.c_str()), PARAMETER_VALUE_UTF8);
		}
		SetCmnStrData((LPCSTR)UnicodetoASCII(strKeyParaName.c_str()), strValue);
	}
	return bRet;
}

bool CVCalCtrl::AddPIMOtherValues(const char* cpKey)
{
	bool bRet = true;
	TSTRING strCount = _T("");
	if(0 == strcmp(cpKey, KEYWORD_DALARM))
	{
		strCount = ASCIItoUnicode(cpKey);
		strCount.erase(strCount.size()-1);
		strCount += KEYWORD_PROPERTY_COUNT;
		SetCmnStrData(KEYWORD_DALARM_1, _T("P0000Y00M00DT00H00M00S"));
		SetCmnStrData(KEYWORD_DALARM_2, _T("000"));
		SetCmnIntData((LPCSTR)UnicodetoASCII(strCount.c_str()), 3);
	}
	return bRet;
}
bool CVCalCtrl::AddPIMProptyName(const char* cpKey)
{
	bool bRet = false;
	TSTRING strKey, strFirst, strSecond, strVPN = _T("");
	CString strVPNValue = _T("");
	strKey = ASCIItoUnicode(cpKey);
	Split<TSTRING>(_T(":"), strKey, strFirst, strSecond);
	Split<TSTRING>(_T(":"), strSecond, strKey, strFirst);

	strVPN = StrConnect<TSTRING>(_T(":"), KEYWORD_VEVENT, KEYWORD_PROPERTY_NAME, false);

	if(GetCmnStrData((LPCSTR)UnicodetoASCII(strVPN.c_str()), strVPNValue))
	{
		if(GetRepeatCount<CString>(_T(";"),strKey.c_str(),strVPNValue) == 0)
			strVPNValue = StrConnect<CString>(_T(";"), strVPNValue, strKey.c_str(), false);
		bRet = SetCmnStrData((LPCSTR)UnicodetoASCII(strVPN.c_str()), strVPNValue);
	}
	return bRet;
}



bool CVCalCtrl::SmartDeletePIMEntity(const char* cpKey)
{
	bool bRet = false;
	if(DeletePIMParas(cpKey) && DeletePIMValues(cpKey) && DeletePIMProptyName(cpKey))
	{
		bRet = true;
	}
	return bRet;
}
bool CVCalCtrl::DeletePIMParas(const char* cpKey)
{
	bool bRet = true;
	
	CString strParas = _T("");
	CString strKey = ASCIItoUnicode(cpKey);
	CString strKeyParaName = StrConnect<CString>(_T(":"), strKey, KEYWORD_PARAMETER_NAME, false);
	bool bGet = GetCmnStrData( (LPCSTR)UnicodetoASCII(strKeyParaName), strParas );

	if(bGet)
	{
		vector<CString> strvDes;
		Tokenize<CString>(_T(";"), strParas, strvDes, false);
		for(vector<CString>::iterator iter = strvDes.begin(); iter != strvDes.end() && bRet; iter++)
		{
			CString strPara = StrConnect<CString>(_T(":"), strKey, (*iter), false);
			bRet = m_PIMObjPt->RemoveByName((LPCSTR)UnicodetoASCII(strPara));
		}
		if(bRet)
			bRet = m_PIMObjPt->RemoveByName((LPCSTR)UnicodetoASCII(strKeyParaName));
	}
	
	return bRet;
}
bool CVCalCtrl::DeletePIMValues(const char* cpKey)
{
	bool bRet = true;
	
	long nInt = 0;
	CString strKey = ASCIItoUnicode(cpKey);
	CString strKeyValueCount = StrConnect<CString>(_T(":"), strKey, KEYWORD_PROPERTY_COUNT, false);
	bool bGet = GetCmnIntData( (LPCSTR)UnicodetoASCII(strKeyValueCount), nInt );
	if(bGet)
	{
		for(int nCount = 0; nCount < (int)nInt && bRet; nCount++)
		{
			CString strCount, strValue = _T("");
			if(iToTStr<CString>(nCount, strCount))
				strValue = StrConnect<CString>(_T(":"), strKey, strCount, false);
			bRet = m_PIMObjPt->RemoveByName((LPCSTR)UnicodetoASCII(strValue));
		}
	}
	else
	{
		bRet = m_PIMObjPt->RemoveByName(cpKey);
	}
	return bRet;
}
bool CVCalCtrl::DeletePIMProptyName(const char* cpKey)
{
	bool bRet = false;
	CString strKey, strFirst, strSecond, strVPN, strVPNValue = _T("");
	strKey = ASCIItoUnicode(cpKey);
	Split<CString>(_T(":"), strKey, strFirst, strSecond);
	Split<CString>(_T(":"), strSecond, strKey, strFirst);

	strVPN = StrConnect<CString>(_T(":"), KEYWORD_VEVENT, KEYWORD_PROPERTY_NAME, false);

	if(GetCmnStrData((LPCSTR)UnicodetoASCII(strVPN), strVPNValue))
	{
		vector<CString> strvDes;
		Tokenize<CString>(_T(";"), strVPNValue, strvDes, false);
		for(vector<CString>::iterator iter = strvDes.begin(); iter != strvDes.end(); iter++)
		{
			if((*iter) == strKey)
			{
				strvDes.erase(iter);
				break;
			}
		}
		strVPNValue = StrConnect<CString>(_T(";"), strvDes, false);
		bRet = SetCmnStrData((LPCSTR)UnicodetoASCII(strVPN), strVPNValue);
	}
	return bRet;
}


bool CVCalCtrl::GetLimitation(const TSTRING& strKey, TSTRING& strLimitation)
{
	bool bRes = true;
	if(!m_pIPIMConfig)
		m_pIPIMConfig = (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("Phonebook"), eIPIMConfigure);

	if(m_pIPIMConfig)
	{
		m_pIPIMConfig->GetProperty(_T("CalenderConfig"), strKey, strLimitation);
		bRes = (!strLimitation.empty());
	}
	else
	{
		bRes = false;
	}
	return bRes;
}
int CVCalCtrl::GetLimitation(const TSTRING& strKey)
{
	int nLimitation = -1;
	TSTRING strLimitation;
	if(GetLimitation(strKey, strLimitation))
	{
		nLimitation = _ttoi(strLimitation.c_str());
	}
	return nLimitation;
}

bool CVCalCtrl::CheckLimitation(const std::string& strOrgKey, TSTRING& strValue)
{
	bool bRes = true;
	TSTRING strConfigKey;
	if(IsKey(KEYWORD_SUBJECT, strOrgKey, strConfigKey) || IsKey(KEYWORD_LOCATION, strOrgKey, strConfigKey) || 
	   IsKey(KEYWORD_DESCRIPTION, strOrgKey, strConfigKey))
	{ 
		bRes = IsInConfigLimitation(strConfigKey, strValue, IsUTF8(strOrgKey));
	}
	return bRes;
}

bool CVCalCtrl::IsUTF8(const std::string& strOrgKey)
{
	bool bRes = false;
	TSTRING tstrOrgKey;
#ifdef _UNICODE
	tstrOrgKey = ansitowide(strOrgKey);
#else
	tstrOrgKey = strOrgKey;
#endif

	vector<TSTRING> vstrOrgKey;
	Tokenize<TSTRING>(_T(":"), tstrOrgKey, vstrOrgKey, false);

	std::string strKey;
	if(vstrOrgKey.size() == 2)
		strKey = strOrgKey + ":ENCODING";
	else
	{
#ifdef _UNICODE
		std::wstring wstrOrgKey = vstrOrgKey[0] + L":" + vstrOrgKey[1];
		strKey = widetoansi(wstrOrgKey) + ":ENCODING";
#else
		strKey = vstrOrgKey[0] + ":" + vstrOrgKey[1] + ":ENCODING";
#endif
	}
	CString strValue;
	if(GetCmnStrData(strKey.c_str(), strValue))
		bRes = (strValue == PARAMETER_VALUE_QP);
	return bRes;
}
bool CVCalCtrl::IsInConfigLimitation(const TSTRING& strKey, TSTRING& strValue, bool bUTF8)
{
	bool bRes = true;
	int nLimitation = GetLimitation(strKey);
	if(nLimitation != -1)
	{
		if(bUTF8)
		{
#ifdef _UNICODE
			std::string strAnsiValue = widetoansi(strValue, CP_UTF8);
			bRes = (nLimitation > (int)strAnsiValue.size());
#else
			bRes = (nLimitation > (int)strValue.size());
#endif
		}
		else
		{
			bRes = (nLimitation > (int)strValue.size());
		}

		if(!bRes)
		{
			bRes = TrimValue(nLimitation, strValue, bUTF8);
		}
	}
	return bRes;
}
bool CVCalCtrl::TrimValue(int nLimitation, TSTRING& strValue, bool bUTF8)
{
	bool bRes = true;
#ifdef _UNICODE
	std::string strAnsiValue = bUTF8 ? widetoansi(strValue, CP_UTF8) : widetoansi(strValue, CP_ACP);
	strAnsiValue = strAnsiValue.substr(0, nLimitation);
	strValue = bUTF8 ? ansitowide(strAnsiValue, CP_UTF8) : ansitowide(strAnsiValue, CP_ACP);
#else
	strValue = strValue.substr(0, nLimitation);
#endif
	return bRes;
}

bool CVCalCtrl::IsKey(const std::string strKey/*.h Key*/, const std::string cstrOrg/*parameter key*/, TSTRING& tstrConfigKey/*for IPIMConfig Key*/ )
{  // Compare and produce IPIMConfig Key
	bool bRes = false;

	TSTRING tstrKey, tstrOrg;
#ifdef _UNICODE
	tstrKey = ansitowide(strKey);
	tstrOrg = ansitowide(cstrOrg);
#else
	tstrKey = strKey;
	tstrOrg = cstrOrg;
#endif
	vector<TSTRING> vstrOrgKey;
	vector<TSTRING> vstrOrgKeyTemp;
	vector<TSTRING> vstrKey;
	Tokenize<TSTRING>(_T(":"), tstrKey, vstrKey, false);
	Tokenize<TSTRING>(_T(":"), tstrOrg, vstrOrgKey, false);
	vstrOrgKeyTemp = vstrOrgKey;
	if((vstrOrgKey.size() == 2 && (GetRepeatCount<TSTRING>(vstrKey[1], vstrOrgKeyTemp[1]) > 0) ))
	{
		tstrConfigKey = tstrKey;
		bRes = true;
	}
	else if(vstrOrgKey.size() == 3 && (GetRepeatCount<TSTRING>(vstrKey[1], vstrOrgKeyTemp[1]) > 0))
	{
		for(int i =0; i< 12; ++i)
		{	
			int nNumber = _ttoi(vstrOrgKey[2].c_str());
			if((nNumber == 0 && vstrOrgKey[2].length() ==1) || (i>0 &&nNumber == i))
			{
				tstrConfigKey = tstrKey+_T(":")+vstrOrgKey[2];
				bRes = true;
				break;
			}
			else if(nNumber == 0 && vstrOrgKey[2].length() !=1)
			{
				break;
			}
		}
	}
	return bRes;
}

}//namespace QSYNC_LIB