#include "StdAfx.h"
#include "VCardCtrl.h"


namespace QSYNC_LIB {
	CVCardCtrl::CVCardCtrl(IPIMObjPt PIMObjPt):m_PIMObjPt(PIMObjPt), m_pIPIMConfig(NULL)
	{
	}

	CVCardCtrl::~CVCardCtrl(void)
	{
		m_pIPIMConfig = NULL;
	}
	std::wstring CVCardCtrl::GetWString( const char * szName )// force to return a value
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

	
	//szKey = PropertyName(without .num)  ex. SetSpecificString("ADR",_T("HOME"),0,1)
	bool CVCardCtrl::SetSpecificString(const TSTRING& strValue, const char* szKey, 
	const TSTRING& strNULLParas/*Para1;Para2;.*/,
	int nPropertyCount/*for more than one same Parameter, you can set default=0*/, int nValueCount)
	{
		bool bRes = false;
	
		CKeyCtrl Key(szKey);
		TSTRING strPropertyName;
		// 1. Get Property Name and all related property(0~n)
		if(GetCmnStrData(Key.GetPropertyName().c_str(), strPropertyName)) 
		{
			int nPropertyTotalCount = GetRepeatCount<TSTRING>(_T(";"), Key.GetTTrimProperty(), strPropertyName, true);
			if(nPropertyTotalCount > 0)
			{
				int nPropertyCounter = -1;
				bool bReset = false;
				for(int nCount =0; nCount < nPropertyTotalCount ; nCount++)
				{
					TSTRING strNULL;
					//2. Get each property's NULL and compare it
					if(GetCmnStrData(Key.GetNULL(nCount).c_str(), strNULL) && CompareParas(strNULLParas, strNULL))
					{
						++nPropertyCounter;
						// 3. if property's NULL is the same and Counter == count, then set string! ;)
						if(nPropertyCounter == nPropertyCount) 
						{
							bRes = SetCmnStrData(Key.GetFullKey(nCount, nValueCount).c_str(), strValue.c_str());
							bReset = true;
							break;
						}
					}
					else if(!GetCmnStrData(Key.GetNULL(nCount).c_str(), strNULL) && nPropertyTotalCount == 1) // for N
					{
						bRes = SetCmnStrData(Key.GetNum(nValueCount).c_str(), strValue.c_str());
						bReset = true;
						break;
					}
				}
				if(!bReset)
					bRes = AddNewPIMEntity(szKey, strValue, strNULLParas, nValueCount);
			}
			else  // if not existed, create one!
				bRes = AddNewPIMEntity(szKey, strValue, strNULLParas, nValueCount);
		}
	
		return bRes;
	}

	// szName should be key without ":number"!!
	TSTRING CVCardCtrl::GetSpecificString( const char* szName, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount)
	{
		TSTRING strRes = _T("");
		vector<TSTRING> vstrRes;
		TSTRING strKey = CharToWideChar(szName, CP_UTF8);
		TSTRING strOrgKey = strKey;

		TSTRING strPropertyName = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, KEYWORD_PROPERTY_NAME, false);
		std::string strTmpKeyWord = WideCharToChar(strPropertyName.c_str(), CP_ACP);
		if(GetCmnStrData(strTmpKeyWord.c_str(), strPropertyName)) // reuse strPropertyName
		{
			int nPropertyTotalCount = GetRepeatCount<TSTRING>(strKey, strPropertyName);
			for(int nCount =0; nCount<nPropertyTotalCount ; nCount++)
			{
				strKey = strOrgKey;
				TSTRING strCount;
				iToTStr<TSTRING>(nCount, strCount);
				if(nPropertyTotalCount >1 && nCount > 0)
					strKey += _T(".") + strCount;
				TSTRING strNULL = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, strKey, KEYWORD_PROPERTY_NULL, false);

				std::string strKeyNULL = WideCharToChar(strNULL.c_str(), CP_ACP);
				if(GetCmnStrData(strKeyNULL.c_str(), strNULL) && CompareParas(strParameter, strNULL))  // reuse strNULL
				{
					TSTRING strMainKey;
					if(nValueCount ==0)
					{	
						strMainKey = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, strKey); 
					}
					else if(nValueCount >0)
					{
						TSTRING strValueCount;
						iToTStr<TSTRING>(nValueCount, strValueCount);
						strMainKey = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, strKey, strValueCount,false); 
					}
					std::string strKeyWord = WideCharToChar(strMainKey.c_str(),	CP_ACP);
					TSTRING strValue;
					GetCmnStrData(strKeyWord.c_str(), strValue);
					vstrRes.push_back(strValue);
				}
			}
			if(!vstrRes.empty() && nPropertyCount< (int)vstrRes.size())
			{		
				strRes = vstrRes.at(nPropertyCount);
			}
		}
		return strRes;
	}

	std::wstring CVCardCtrl::GetSpecificWString( const char* szName, const TSTRING& strParameter, int nPropertyCount ,int nValueCount)
	{
		std::wstring strRes = L"";
		vector<std::wstring> vstrRes;
		TSTRING strKey = CharToWideChar(szName, CP_UTF8);
		TSTRING strOrgKey = strKey;

		TSTRING strPropertyName = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, KEYWORD_PROPERTY_NAME, false);
		std::string strTmpKeyWord = WideCharToChar(strPropertyName.c_str(), CP_ACP);
		if(GetCmnStrData(strTmpKeyWord.c_str(), strPropertyName)) // reuse strPropertyName
		{
			int nPropertyTotalCount = GetRepeatCount<TSTRING>(strKey, strPropertyName);
			for(int nCount =0; nCount<nPropertyTotalCount ; nCount++)
			{
				strKey = strOrgKey;
				TSTRING strCount;
				iToTStr<TSTRING>(nCount, strCount);
				if(nPropertyTotalCount >1 && nCount > 0)
					strKey += _T(".") + strCount;
				TSTRING strNULL = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, strKey, KEYWORD_PROPERTY_NULL, false);

				std::string strKeyNULL = WideCharToChar(strNULL.c_str(), CP_ACP);
				if(GetCmnStrData(strKeyNULL.c_str(), strNULL))  // reuse strNULL
				{
					if(CompareParas(strParameter, strNULL))
					{
						TSTRING strMainKey;
						if(nValueCount ==0)
						{	
							strMainKey = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, strKey); 
						}
						else if(nValueCount >0)
						{
							TSTRING strValueCount;
							iToTStr<TSTRING>(nValueCount, strValueCount);
							strMainKey = StrConnect<TSTRING>(_T(":"), OBJECT_NAME_VCARD, strKey, strValueCount,false); 
						}
						std::string strKeyWord = WideCharToChar(strMainKey.c_str(),	CP_ACP);

						vstrRes.push_back(GetWString(strKeyWord.c_str()));
					}
				}
			}
			if(!vstrRes.empty() && nPropertyCount< (int)vstrRes.size())
			{		
				strRes = vstrRes.at(nPropertyCount);
			}
		}
		return strRes;
	}
	bool CVCardCtrl::CompareParas(const TSTRING& strSpecificPara, const TSTRING& strParameter)
	{
		bool bRes = false;
		vector<TSTRING> vStrSpecParas, vStrOrgParas;
		bool bSpec= Tokenize<TSTRING>(_T(";"), strSpecificPara, vStrSpecParas, false);
		bool bOrg = Tokenize<TSTRING>(_T(";"), strParameter, vStrOrgParas, false);

		if(bSpec && bOrg && vStrSpecParas.size() == vStrOrgParas.size())
		{
			sort(vStrSpecParas.begin(), vStrSpecParas.end());
			sort(vStrOrgParas.begin(), vStrOrgParas.end());
			bool bCompare = true;
			for(int i=0 ; i< (int)vStrSpecParas.size() && bCompare; i++)
			{
				bCompare = vStrSpecParas[i]==vStrOrgParas[i];
			}	
			bRes = bCompare;
		}
		return bRes;
	}

	int CVCardCtrl::GetSpecificInt(const char* szName, const TSTRING& strParameter)
	{
		int nRes = -1;
		TSTRING strData = GetSpecificString(szName, strParameter,0);
		if(strData.length()!=0)
		{
			nRes = _ttoi(strData.c_str());
		}
		return nRes;
	}
	bool CVCardCtrl::SetEmptyIPIMObjPt()
	{
		bool bRes = false;
		TSTRING szFakeVCard = _T(\
"BEGIN:VCARD\r\n\
N;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;;;;\r\n\
FN;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
ADR;WORK;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;;;;;;\r\n\
ADR;HOME;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;;;;;;\r\n\
TEL;HOME;VOICE:\r\n\
TEL;WORK;VOICE:\r\n\
TEL;HOME;CELL:\r\n\
TEL;WORK;CELL:\r\n\
TEL;HOME;FAX:\r\n\
TEL;WORK;FAX:\r\n\
TEL;HOME;PREF:\r\n\
TEL;WORK;PREF:\r\n\
EMAIL;HOME:\r\n\
EMAIL;HOME:\r\n\
EMAIL;WORK:\r\n\
EMAIL;WORK:\r\n\
URL;WORK:\r\n\
URL;HOME:\r\n\
ORG;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
TITLE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
ROLE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
NOTE;WORK;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
NOTE;HOME;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:\r\n\
BDAY:\r\n\
X-BENQ;CallerID:\r\n\
VERSION:2.1\r\n\
END:VCARD");
		if(m_PIMObjPt)
			bRes = m_PIMObjPt->decode(szFakeVCard);

		return bRes;
	}

	bool CVCardCtrl::SetEmptySIMIPIMObjPt()
	{
		bool bRes = false;
		TSTRING szFakeVCard = _T(\
"BEGIN:VCARD\r\n\
N;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;;;;\r\n\
TEL;HOME;VOICE:\r\n\
X-BENQ;CallerID:\r\n\
VERSION:2.1\r\n\
END:VCARD");
		if(m_PIMObjPt)
			bRes = m_PIMObjPt->decode(szFakeVCard);
		return bRes;
	}

	bool CVCardCtrl::SetCmnStrData(const char* szName, const TCHAR* szData)
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
	bool CVCardCtrl::SetCmnWStrData(const char * szName, const wchar_t* szData)
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

	bool CVCardCtrl::SetCmnIntData(const char * szName, int nData)
	{	
		size_t npos = -1;
		CommonDataPt sptemData = new CommonData;
		sptemData->SetName(szName);
		sptemData->SetInteger(nData);
		bool bSet = npos != m_PIMObjPt->SetByName(szName, sptemData) ? true : false;
		return bSet;
	}
	bool CVCardCtrl::GetCmnStrData(const char* cpKey, TSTRING& strCmnData)
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
	bool CVCardCtrl::GetCmnIntData( const char * szName, long& nInt )
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


	
	//cpKey needs to be full key.
	bool CVCardCtrl::SaveCmnData(const char* cpKey, const TSTRING& strValue)
	{
		bool bRet = true;
		TSTRING strGetValue = _T("");
		bool bGet = GetCmnStrData(cpKey, strGetValue);

		if(strValue.size() > 0)
			bRet = SmartAddPIMEntity(cpKey, strValue);	
		else if(bGet && strValue.size() == 0)
			bRet = SmartDeletePIMEntity(cpKey);

		return bRet;
	}

	
	bool CVCardCtrl::AddNewPIMEntity(const char* cpKey, const TSTRING& strValue, const TSTRING& strNULLParas, int nValueCount)
	{
		bool bRes = true;
		if(!strValue.empty())
		{
			CKeyCtrl Key(cpKey);
			std::string strKey = Key.GetPureKey();

			bool bAddPropertyName = true;
			// Add Key into "VCARD:PropertyName"
			if(strKey == KEYWORD_NAME || strKey == KEYWORD_ROLE || strKey == KEYWORD_DISPLAY 
			|| strKey == KEYWORD_COMPANY || strKey == KEYWORD_TITLE)
				bAddPropertyName = AddPIMProptyName(cpKey);  // Those keys are unique, and can't be increased.
			else
				bAddPropertyName = AddPIMProptyName(cpKey,true);

			// Reset cpKey
			TSTRING strPropertyName;
			GetCmnStrData(Key.GetPropertyName().c_str(), strPropertyName);
			int nPropertyCount = GetRepeatCount<TSTRING>(_T(";"),Key.GetTTrimProperty().c_str(),strPropertyName,true);
			nPropertyCount = nPropertyCount == 1 ? -1 : nPropertyCount -1;
			std::string strNewKey = Key.GetPropertyNum(nPropertyCount);
			Key.ResetOrgKey(strNewKey);

			bool bAddNULLParas = true;
			// Add NULL Parameter
			if(!strNULLParas.empty() && (strKey == KEYWORD_XBENQ || strKey == KEYWORD_TEL 
			|| strKey == KEYWORD_NOTE || strKey == KEYWORD_EMAIL || strKey == KEYWORD_ADDRESS)) 
				bAddNULLParas = AddNULLParas(strNewKey.c_str(), strNULLParas); // add NULL Paras

			bool bAddPIMParas = true;
			// Add Encoding and Charset
			if(strKey == KEYWORD_NAME || strKey == KEYWORD_DISPLAY || strKey == KEYWORD_ADDRESS
			|| strKey == KEYWORD_COMPANY || strKey == KEYWORD_TITLE || strKey == KEYWORD_ROLE
			|| strKey == KEYWORD_NOTE)
				bAddPIMParas = AddPIMParas(strNewKey.c_str(), true, true);

			bool bAddNameCount = true;
			bool bAddADRCount = true;
			// Add Count
			long nCount =0;
			if(strKey == KEYWORD_NAME && !GetCmnIntData(Key.GetCount().c_str(), nCount))
				bAddNameCount = SetCmnIntData(Key.GetCount().c_str(), 5);
			if(strKey == KEYWORD_ADDRESS && !GetCmnIntData(Key.GetCount().c_str(), nCount))
				bAddADRCount = SetCmnIntData(Key.GetCount().c_str(), 7);

			// Add Value
			bool bAddValue = true;
			nCount = 0;
			if(GetCmnIntData(Key.GetCount().c_str(), nCount))
				bAddValue = SetCmnStrData(Key.GetNum(nValueCount).c_str(), strValue.c_str());
			else
				bAddValue = SetCmnStrData(Key.GetFullKey().c_str(), strValue.c_str());

			bRes = bAddPropertyName && bAddNULLParas && bAddPIMParas && bAddNameCount && bAddADRCount && bAddValue;
		}
		return bRes;
	}

	//cpKey = VCARD:PropertyName
	bool CVCardCtrl::SmartAddPIMEntity(const char* cpKey, const TSTRING& strValue)
	{
		bool bRet = true;
		std::string strKey = cpKey;
		TSTRING strNoUse;
		if( strKey == KEYWORD_NAME || strKey == KEYWORD_ADDRESS
			|| strKey == KEYWORD_ROLE || strKey == KEYWORD_DISPLAY || strKey == KEYWORD_COMPANY 
			|| strKey == KEYWORD_TITLE || strKey == KEYWORD_NOTE)
		{
			bRet = AddPIMParas(cpKey, true, true); // add Encoding and charset
		}
		
		if(bRet)
		{
			bRet = AddPIMProptyName(cpKey);
			if(bRet)
				bRet = SetCmnStrData(cpKey, strValue.c_str());
		}
		return bRet;
	}

	//cpKey = VCARD:PropertyName (with .num)
	bool CVCardCtrl::AddNULLParas(const char* cpKey, const TSTRING& strNULLParas) // Para1;Para2;...
	{
		bool bRes = true;
		CKeyCtrl Key(cpKey);
		vector<TSTRING> vstrNULLParas;
		Tokenize<TSTRING>(_T(";"), strNULLParas, vstrNULLParas, false);
		TSTRING strAddToNULL;
		TSTRING strOrgNULL = _T("");
		GetCmnStrData(Key.GetNULL().c_str(), strOrgNULL);
		if(!strOrgNULL.empty())
		{ // add additional Parameters which didn't exist before
			/*for(vector<TSTRING>::iterator iter = vstrNULLParas.begin(); iter!= vstrNULLParas.end(); ++iter)
			{
				if(0 == GetRepeatCount<TSTRING>(_T(";"), (*iter), strOrgNULL, true))
				{
					strOrgNULL = strOrgNULL.empty() ? (*iter) : (strOrgNULL + _T(";") + (*iter));
				}
			}*/
			bRes = SetCmnStrData(Key.GetNULL().c_str(), strNULLParas.c_str()); // overwrite original string
		}
		else
		{
			//Add Parameter NULL to Parameter list
			TSTRING strParaNameValue;
			GetCmnStrData(Key.GetParaName().c_str(), strParaNameValue);
			if(0 == GetRepeatCount<TSTRING>(_T(";"), _T("NULL"), strParaNameValue, true))
			{
				strParaNameValue = StrConnect<TSTRING>(_T(";"),strParaNameValue, _T("NULL"));
				SetCmnStrData(Key.GetParaName().c_str(), strParaNameValue.c_str());
			}
			//Set Value
			bRes = SetCmnStrData(Key.GetNULL().c_str(), strNULLParas.c_str());
		}
		return bRes;
	}
	bool CVCardCtrl::AddPIMParas(const char* cpKey, bool bEncoding, bool bCharset)
	{
		bool bRet = true;
		if(bEncoding || bCharset)
		{
			TSTRING strKey, strFirst, strSecond= _T("");
			TSTRING strValue = _T("");

			CKeyCtrl Key(cpKey);
			strKey = Key.GetTFullKey();

			GetCmnStrData(Key.GetParaName().c_str(), strValue);
			if(bEncoding)
			{
				if(GetRepeatCount<TSTRING>(_T(";"),PARAMETER_NAME_ENCODING,strValue) == 0)
					strValue = StrConnect<TSTRING>( _T(";"), strValue, PARAMETER_NAME_ENCODING, false);
				SetCmnStrData( Key.GetParaName(PARAMETER_NAME_ENCODING).c_str(), PARAMETER_VALUE_QP);
			}
			if(bCharset)
			{
				if(GetRepeatCount<TSTRING>(_T(";"),PARAMETER_NAME_CHARSET,strValue) == 0)
					strValue = StrConnect<TSTRING>( _T(";"), strValue, PARAMETER_NAME_CHARSET, false);
				SetCmnStrData(Key.GetParaName(PARAMETER_NAME_CHARSET).c_str(), PARAMETER_VALUE_UTF8);
			}
			SetCmnStrData(Key.GetParaName().c_str(), strValue.c_str());
		}
		return bRet;
	}


	bool CVCardCtrl::AddPIMProptyName(const char* cpKey, bool bIncrease)
	{
		bool bRet = false;
		CKeyCtrl Key(cpKey);
		TSTRING strPName;
		if(GetCmnStrData(Key.GetPropertyName().c_str(), strPName))
		{
			int nRepeatCount = GetRepeatCount<TSTRING>(_T(";"),Key.GetTTrimProperty().c_str(),strPName,true);
			if( nRepeatCount == 0 )
				strPName = StrConnect<TSTRING>(_T(";"), strPName, Key.GetTTrimProperty().c_str(), false);
			else
			{
				if(bIncrease)
					strPName = StrConnect<TSTRING>(_T(";"), strPName, Key.GetTPropertyNum(nRepeatCount).c_str(), false);
			}
			bRet = SetCmnStrData(Key.GetPropertyName().c_str(), strPName.c_str());
		}
		return bRet;
	}

	bool CVCardCtrl::SmartDeletePIMEntity(const char* cpKey)
	{
		bool bRet = false;
		if(DeletePIMParas(cpKey) && DeletePIMValues(cpKey) && DeletePIMProptyName(cpKey))
		{
			bRet = true;
		}
		return bRet;
	}
	bool CVCardCtrl::DeletePIMParas(const char* cpKey)
	{
		bool bRet = true;

		TSTRING strParas = _T("");
		TSTRING strKey = ASCIItoUnicode(cpKey);
		TSTRING strKeyParaName = StrConnect<TSTRING>(_T(":"), strKey, KEYWORD_PARAMETER_NAME, false);
		bool bGet = GetCmnStrData( (LPCSTR)UnicodetoASCII(strKeyParaName.c_str()), strParas );

		if(bGet)
		{
			vector<TSTRING> strvDes;
			Tokenize<TSTRING>(_T(";"), strParas, strvDes, false);
			for(vector<TSTRING>::iterator iter = strvDes.begin(); iter != strvDes.end() && bRet; iter++)
			{
				TSTRING strPara = StrConnect<TSTRING>(_T(":"), strKey, (*iter), false);
				bRet = m_PIMObjPt->RemoveByName((LPCSTR)UnicodetoASCII(strPara.c_str()));
			}
			if(bRet)
				bRet = m_PIMObjPt->RemoveByName((LPCSTR)UnicodetoASCII(strKeyParaName.c_str()));
		}

		return bRet;
	}
	bool CVCardCtrl::DeletePIMValues(const char* cpKey)
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
	bool CVCardCtrl::DeletePIMProptyName(const char* cpKey)
	{
		bool bRet = false;
		TSTRING strKey, strFirst, strSecond, strVPN, strVPNValue = _T("");
		strKey = ASCIItoUnicode(cpKey);
		Split<TSTRING>(_T(":"), strKey, strFirst, strSecond);
		Split<TSTRING>(_T(":"), strSecond, strKey, strFirst);

		strVPN = StrConnect<TSTRING>(_T(":"),OBJECT_NAME_VCARD, KEYWORD_PROPERTY_NAME, false);

		if(GetCmnStrData((LPCSTR)UnicodetoASCII(strVPN.c_str()), strVPNValue))
		{
			vector<TSTRING> strvDes;
			Tokenize<TSTRING>(_T(";"), strVPNValue, strvDes, false);
			for(vector<TSTRING>::iterator iter = strvDes.begin(); iter != strvDes.end(); iter++)
			{
				if((*iter) == strKey)
				{
					strvDes.erase(iter);
					break;
				}
			}
			strVPNValue = StrConnect<TSTRING>(_T(";"), strvDes, false);
			bRet = SetCmnStrData((LPCSTR)UnicodetoASCII(strVPN.c_str()), strVPNValue.c_str());
		}
		return bRet;
	}

	bool CVCardCtrl::GetLimitation(const TSTRING& strKey, TSTRING& strLimitation)
	{
		bool bRes = true;
		if(!m_pIPIMConfig)
			m_pIPIMConfig = (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("Phonebook"), eIPIMConfigure);
		
		if(m_pIPIMConfig)
		{
			m_pIPIMConfig->GetProperty(_T("PhonebookConfig"), strKey, strLimitation);
			bRes = (!strLimitation.empty());
		}
		else
		{
			bRes = false;
		}
		return bRes;
	}
	int CVCardCtrl::GetLimitation(const TSTRING& strKey)
	{
		int nLimitation = -1;
		TSTRING strLimitation;
		if(GetLimitation(strKey, strLimitation))
		{
			nLimitation = _ttoi(strLimitation.c_str());
		}
		return nLimitation;
	}

	bool CVCardCtrl::CheckLimitation(const std::string& strOrgKey, TSTRING& strValue)
	{
		bool bRes = true;
		TSTRING strConfigKey;
		if(IsKey(KEYWORD_DISPLAY, strOrgKey, strConfigKey) || IsKey(KEYWORD_COMPANY, strOrgKey, strConfigKey) || 
		   IsKey(KEYWORD_TITLE, strOrgKey, strConfigKey) || IsKey(KEYWORD_NOTE, strOrgKey, strConfigKey) ||
		   IsKey(KEYWORD_TEL, strOrgKey, strConfigKey) || IsKey(KEYWORD_EMAIL, strOrgKey, strConfigKey) ||
		   IsKey(KEYWORD_URL, strOrgKey, strConfigKey) || IsKey(KEYWORD_ADDRESS, strOrgKey, strConfigKey) ||
		   IsKey(KEYWORD_NAME, strOrgKey, strConfigKey))
		{ 
			bRes = IsInConfigLimitation(strConfigKey, strValue, IsUTF8(strOrgKey));
		}
		return bRes;
	}

	bool CVCardCtrl::IsUTF8(const std::string& strOrgKey)
	{
		bool bRes = false;
		CKeyCtrl Key(strOrgKey);
		TSTRING strValue;
		if(GetCmnStrData(Key.GetParaName(PARAMETER_NAME_ENCODING).c_str(), strValue))
			bRes = (strValue == PARAMETER_VALUE_QP);
		return bRes;
	}
	bool CVCardCtrl::IsInConfigLimitation(const TSTRING& strKey, TSTRING& strValue, bool bUTF8)
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
	bool CVCardCtrl::TrimValue(int nLimitation, TSTRING& strValue, bool bUTF8)
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

	bool CVCardCtrl::IsKey(const std::string strKey/*.h Key*/, const std::string cstrOrg/*parameter key*/, TSTRING& tstrConfigKey/*for IPIMConfig Key*/ )
	{  // Compare and produce IPIMConfig Key
		bool bRes = false;
		
		TSTRING tstrKey = GetTSTRING(strKey); 
		TSTRING	tstrOrg = GetTSTRING(cstrOrg);

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


	TSTRING	CVCardCtrl::GetOfficialDisplayName()
	{
		TSTRING strRes;
		GetCmnStrData(KEYWORD_DISPLAY, strRes);
		if(strRes == _T(""))
		{
			TSTRING strFirstName, strLastName;
			GetCmnStrData(KEYWORD_LASTNAME, strLastName);
			GetCmnStrData(KEYWORD_FIRSTNAME, strFirstName);
			strRes = StrConnect<TSTRING>(_T(", "), strLastName, strFirstName);
		}
		if(strRes == _T(""))
			GetCmnStrData(KEYWORD_COMPANY, strRes);
		if(strRes == _T(""))
			strRes = GetSpecificString("TEL", _T("HOME;VOICE"), 0, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("TEL", _T("WORK;VOICE"), 0, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("TEL", _T("HOME;CELL"), 0, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("TEL", _T("WORK;CELL"), 0, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("EMAIL", _T("HOME"), 0, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("EMAIL", _T("HOME"), 1, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("EMAIL", _T("WORK"), 0, 0);
		if(strRes == _T(""))
			strRes = GetSpecificString("EMAIL", _T("WORK"), 1, 0);

		return strRes;
	}


}//namespace QSYNC_LIB