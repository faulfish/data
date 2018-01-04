#ifndef _CVCardCtrl_HPP_
#define _CVCardCtrl_HPP_
#pragma once

#ifdef		PIMDB_DLL
	#define		PIMDB_DLL_DECLSPEC		__declspec(dllexport)
#else
	#define		PIMDB_DLL_DECLSPEC		__declspec(dllimport)
#endif

#include "../../CommonUtil/PIMStringUtility.h"
#include "../../CommonUtil/UnicodeUtility.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../PIMDB/SyncManager.h"
#include "../PIMDB/PIMRecord.h"
#include <string>
#include <vector>
#include <cstringt.h>

namespace QSYNC_LIB 
{

#define			KEYWORD_VCARD		_T("VCARD")
#define			KEYWORD_NAME	    "VCARD:N"
#define			KEYWORD_LASTNAME    "VCARD:N:0"
#define			KEYWORD_FIRSTNAME   "VCARD:N:1"
#define			KEYWORD_DISPLAY	    "VCARD:FN"
#define			KEYWORD_COMPANY	    "VCARD:ORG"
#define			KEYWORD_TITLE	    "VCARD:TITLE"
#define			KEYWORD_ADDRESS	    "VCARD:ADR"
#define			KEYWORD_ADDRESS2	"VCARD:ADR.1"
#define			KEYWORD_ROLE		"VCARD:ROLE"
#define			KEYWORD_NOTE		"VCARD:NOTE"
#define			KEYWORD_NOTE2		"VCARD:NOTE.1"
#define			KEYWORD_BDAY		"VCARD:BDAY"
#define			KEYWORD_TEL			"VCARD:TEL"
#define			KEYWORD_EMAIL		"VCARD:EMAIL"
#define			KEYWORD_URL			"VCARD:URL"
#define			KEYWORD_XBENQ		"VCARD:X-BENQ"

	/*
	 * There are several kinds of key:
	 * such as VCARD:ADR / VCARD:ADR:ParaName / VCARD:ADR:Count / VCARD:PropertyName /
	 *		   VCARD:ADR:NULL / VCARD:ARR:0 / VCARD:ARR:1 / ADR
	 *
	 * => CKeyCtrl is a class to help you control those annoying keys. :)
	 *    You can get every kind of key, no matter what you give initially.
	 */
	class CKeyCtrl
	{
	public:
		CKeyCtrl(const std::string& strOrgKey)
			:m_strOrgKey(strOrgKey), m_strObjectName(KEYWORD_VCARD){GetTOrgProperty();}
		CKeyCtrl(const char* czKey)
			:m_strOrgKey(czKey), m_strObjectName(KEYWORD_VCARD){GetTOrgProperty();}
		CKeyCtrl(const TSTRING& tstrKey)
			:m_strOrgKey(GetAnsi(tstrKey)), m_strObjectName(KEYWORD_VCARD){GetTOrgProperty();}
		~CKeyCtrl(){}

		void ResetOrgKey(const std::string& strOrgKey){ m_strOrgKey = strOrgKey; }
		void SetObjectName(const TSTRING& strObjectName){m_strObjectName = strObjectName;}
		bool IsKeyEmpty(){ return m_strOrgKey.empty();}
		
		// might be "PropertyName.num" or "PropertyName", it depends on what you input in constrcutor
		TSTRING GetTOrgProperty(bool bReset = false) 
		{
			if(!bReset || m_strProperty.size() == 0 )
			{
				TSTRING strKey;
				strKey = GetTSTRING(m_strOrgKey);
				std::vector<TSTRING> vstrTokens;
				if(Tokenize<TSTRING>(_T(":"), strKey, vstrTokens, false) && vstrTokens.size() > 1)
				{
					strKey = vstrTokens[1];
				}
				m_strProperty = strKey;
			}
			return m_strProperty;
		}
		std::string GetOrgProperty(){return GetAnsi(m_strProperty);}
		
		// Get PropertyName.PropertyNum -> .PropertyNum exists if nPropertyNum > 0
		TSTRING GetTPropertyNum(int nPropertyNum)
		{
			TSTRING strNum;
			if(nPropertyNum > 0)
				iToTStr<TSTRING>(nPropertyNum, strNum);
			return StrConnect<TSTRING>(_T("."), GetTTrimProperty(), strNum);
		}
		std::string GetPropertyNum(int nPropertyNum){return GetAnsi(GetTPropertyNum(nPropertyNum));}
		
		// Get PropertyName
		TSTRING GetTTrimProperty() // trim .num
		{
			TSTRING strOrgKey = GetTOrgProperty();
			TSTRING strFirst, strSecond;
			Split<TSTRING>(_T("."), strOrgKey,strFirst, strSecond);
			return strFirst;
		}
		std::string GetTrimProperty(){ return GetAnsi(GetTTrimProperty()); }
		
		// Get "VCARD:*OrgProperty*:ParaName"  <- *OrgProperty* comes from GetTOrgProperty()
		TSTRING GetTParaName(){ return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTOrgProperty(), KEYWORD_PARAMETER_NAME, false);}
		std::string GetParaName(){ return GetAnsi(GetTParaName()); }
		
		// Get "VCARD:*OrgProperty*:WhateverYouWant"
		TSTRING GetTParaName(const TSTRING& strParaName){ return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTOrgProperty(), strParaName, false);}
		std::string GetParaName(const TSTRING& strParaName){ return GetAnsi(GetTParaName(strParaName));}
		
		// Get "VCARD:*OrgProperty*:NULL"  <- *OrgProperty* comes from GetTOrgProperty()
		TSTRING GetTNULL(){ return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTOrgProperty(), KEYWORD_PROPERTY_NULL, false); }
		std::string GetNULL(){ return GetAnsi(GetTNULL()); }
		
		TSTRING GetTNULL(int nPropertyNum){ return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTPropertyNum(nPropertyNum), KEYWORD_PROPERTY_NULL, false); }
		std::string GetNULL(int nPropertyNum){ return GetAnsi(GetTNULL(nPropertyNum)); }

		// Get "VCARD:*OrgProperty*:Count"  <- *OrgProperty* comes from GetTOrgProperty()
		TSTRING GetTCount(){ return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTOrgProperty(), KEYWORD_PROPERTY_COUNT, false);}
		std::string GetCount(){ return GetAnsi(GetTCount());}
		
		// Get "VCARD:PropertyName"
		TSTRING GetTPropertyName(){ return m_strObjectName + _T(":") + KEYWORD_PROPERTY_NAME;}
		std::string GetPropertyName(){return GetAnsi(GetTPropertyName());}
		
		// Get VCARD:*OrgProperty*:Num   <- *OrgProperty* comes from GetTOrgProperty()
		TSTRING GetTNum(int nValueNum)
		{
			TSTRING strNum;
			if(nValueNum > -1)
				iToTStr<TSTRING>(nValueNum, strNum);
			return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTOrgProperty(), strNum, false); 
		}
		std::string GetNum(int nValueNum){return GetAnsi(GetTNum(nValueNum));}
		
		// Get specified Key: VCARD:Property.PropertyNum:Num -> If Num < 0 or PropertyNum < 1, it'll be omitted.
		TSTRING GetTFullKey(int nPropertyNum, int nNum)
		{
			TSTRING strNum;
			if(nNum > 0)
				iToTStr<TSTRING>(nNum, strNum);
			return StrConnect<TSTRING>(_T(":"), m_strObjectName, GetTPropertyNum(nPropertyNum), strNum, false); 
		}
		std::string GetFullKey(int nPropertyNum, int nNum){return GetAnsi(GetTFullKey(nPropertyNum, nNum));}
		
		// Get sure Key: VCARD:*OrgProperty*
		TSTRING GetTFullKey(){ return m_strObjectName+_T(":")+ GetTOrgProperty();}
		std::string GetFullKey(){ return GetAnsi(GetTFullKey()); }
		
		// Get Pure Key: VCARD:Property
		TSTRING GetTPureKey(){ return m_strObjectName + _T(":") + GetTTrimProperty(); }
		std::string GetPureKey(){ return GetAnsi(GetTPureKey()); }


	private:
		std::string m_strOrgKey;
		TSTRING m_strObjectName;
		TSTRING m_strProperty;  // GetTOrgProperty()
	};


	class PIMDB_DLL_DECLSPEC CVCardCtrl
	{
	public:
		CVCardCtrl(IPIMObjPt PIMObjPt);
		virtual ~CVCardCtrl(void);

		TSTRING	GetOfficialDisplayName();
		std::wstring GetWString( const char * szName );

		// set default nPropertyCount = 0; or you need to know which Property you like having more than two same parameters.  
		std::wstring GetSpecificWString( const char* szName, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount=0);
		TSTRING GetSpecificString(const char* szName, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyNum,int nValueCount=0);
		bool SetSpecificString(const TSTRING& strValue, const char* szName, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount=0,int nValueCount=0);

		int GetSpecificInt(const char* szName, const TSTRING& strParameter); // Para1;Para2;...
		bool SetEmptyIPIMObjPt();
		bool SetEmptySIMIPIMObjPt();
		bool SaveCmnData(const char* cpKey, const TSTRING& strValue);

		bool GetCmnStrData(const char* cpKey, TSTRING& strCmnData);
		bool GetCmnIntData( const char * szName, long& nInt );         
		bool SetCmnStrData(const char * szName, const TCHAR* szData);  
		bool SetCmnIntData(const char * szName, int nData); 
		bool SetCmnWStrData(const char * szName, const wchar_t* szData);

		bool GetLimitation(const TSTRING& strKey, TSTRING& strLimitation);
		int  GetLimitation(const TSTRING& strKey);
	private:
		bool CompareParas(const TSTRING& strSpecificPara, const TSTRING& strParameter);

		bool AddNewPIMEntity(const char* cpKey, const TSTRING& strValue, const TSTRING& strNULLParas = _T(""), int nValueCount = 0);
		
		bool SmartAddPIMEntity(const char* cpKey, const TSTRING& strValue);  // mend parameters and set value.
		bool AddPIMProptyName(const char* cpKey,bool bIncrease = false);  // bIncrease=true means add one more PropertyName even if it's existed.
		bool AddPIMParas(const char* cpKey, bool bEncoding, bool bCharset);  // for adding Encoding and Charset
		bool AddNULLParas(const char* cpKey, const TSTRING& strNULLParas);   // for adding NULL Parameters

		bool SmartDeletePIMEntity(const char* cpKey);
		bool DeletePIMParas(const char* cpKey);
		bool DeletePIMValues(const char* cpKey);
		bool DeletePIMProptyName(const char* cpKey);

		bool CheckLimitation(const std::string& cpKey, TSTRING& strValue);
		bool IsInConfigLimitation(const TSTRING& strKey, TSTRING& strValue, bool bUTF8);
		bool IsKey(const std::string strKey, const std::string strOrg, TSTRING& tstrKey);
		bool IsUTF8(const std::string& strOrgKey);
		bool TrimValue(int nLimitation, TSTRING& strValue, bool bUTF8);
	private:
		IPIMObjPt m_PIMObjPt;
		IPIMCONFIG* m_pIPIMConfig;
	};

}//namespace QSYNC_LIB

#endif // _CVCardCtrl_HPP_