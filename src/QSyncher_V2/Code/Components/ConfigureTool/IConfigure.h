///////////////////////////////////////////////////////////
//  IConfigure.h
//  Implementation of the Interface IConfigure
//  Created on:      16-Jul-2007 14:57:19
//  Original author: mei
///////////////////////////////////////////////////////////

#ifndef _CONFIGURETOOL_ICONFIGURE_HPP__
#define _CONFIGURETOOL_ICONFIGURE_HPP__

#pragma once

#include <string>

#ifdef		CONFIGURETOOL_EXPORTS
#define		CONFIGURETOOL_DLL_DECLSPEC		__declspec(dllexport)
#else
#define		CONFIGURETOOL_DLL_DECLSPEC		__declspec(dllimport)
#endif


namespace QSYNC_LIB{	


#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	const LPCTSTR EVENT_CONFIGURETOOL  = _TC("ConfigureTool");
	const LPCTSTR STORAGE_INIFILE = _TC("IniFile");


	class IConfigure
	{
	public:
		virtual bool Load(const TSTRING& strStoreType = STORAGE_INIFILE) =0;
		virtual bool Save(const TSTRING& strStoreType = STORAGE_INIFILE) =0;
		virtual int GetCfgVar(const TSTRING& strName, int nDefault) = 0;
		virtual bool SetCfgVar(const TSTRING& strName, int nVar) = 0;
		virtual TSTRING GetCfgVar(const TSTRING& strName,const TSTRING& strDefault) = 0;
		virtual bool SetCfgVar(const TSTRING& strName, const TSTRING& strVar) = 0;
	};

	CONFIGURETOOL_DLL_DECLSPEC IConfigure* CreateIConfigure(LPCTSTR szConfigureTitle, LPCTSTR szFolderName = _TC("QSyncer"));
	CONFIGURETOOL_DLL_DECLSPEC bool DeleteIConfigure(LPCTSTR szConfigureTitle);

	template<class TYPE>
	class CCfgType
	{
	public:
		CCfgType(IConfigure* pICfg, const TSTRING& strName):m_strKeyName(strName), m_pICfg(pICfg){}
		CCfgType(IConfigure* pICfg, const TSTRING& strName, const TYPE& Value)
			:m_strKeyName(strName), m_pICfg(pICfg), m_LocalVar(Value)
		{
			m_pICfg->SetCfgVar(m_strKeyName,m_LocalVar);
		}
		~CCfgType(){}
		
		CCfgType& operator=(const TYPE& Value)
		{
			if(m_pICfg->SetCfgVar(m_strKeyName,Value))
				m_LocalVar = Value;
			return *this;
		}

		CCfgType& operator=(const CCfgType& CfgType)
		{
			if(m_pICfg->SetCfgVar(m_strKeyName,CfgType.m_LocalVar))
				m_LocalVar = CfgType.m_LocalVar;
			return *this;
		}

		operator const TYPE&()
		{		
			return Get();
		}

		const TYPE& Get()
		{
			m_LocalVar = m_pICfg->GetCfgVar(m_strKeyName,m_LocalVar);
			return m_LocalVar;
		}

		static void Get(IConfigure* pICfg, const TSTRING& strName,TYPE& Value) 
		{
			Value = pICfg->GetCfgVar(strName,Value);
		}
		
		bool Set(const TYPE& Value)
		{
			return m_pICfg->SetCfgVar(m_strKeyName,Value);
		}

		static bool Set(IConfigure* pICfg, const TSTRING& strName,const TYPE& Value)
		{
			return pICfg->SetCfgVar(strName,Value);
		}
	private:
		TSTRING m_strKeyName;
		TYPE	m_LocalVar;
		IConfigure* m_pICfg;
	};

	typedef CCfgType<int>		CCfgInt;
	typedef CCfgType<TSTRING>	CCfgStr;

} // namespace QSYNC_LIB
#endif // #ifndef _CONFIGURETOOL_ICONFIGURE_HPP__