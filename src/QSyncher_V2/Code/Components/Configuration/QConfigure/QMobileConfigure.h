#ifndef _QSYNC_QMOBILECONFIG_H_
#define _QSYNC_QMOBILECONFIG_H_

#ifdef _QCONFIGUREDLL
#define		 QCONFIGURE_CLASS_DLLEXT	__declspec(dllexport)
#else
#define		 QCONFIGURE_CLASS_DLLEXT	__declspec(dllimport)
#endif

#include <string>
#include <map>

namespace QSYNC_LIB
{	
#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	using namespace std;

	class QCONFIGURE_CLASS_DLLEXT CQSetting
	{
		class SettingValue
		{
		public:
			SettingValue(const TSTRING& strValue,const TSTRING& strChild):m_strValue(strValue),m_strChild(strChild){}
			TSTRING m_strValue;
			TSTRING m_strChild;
		};

		typedef map<TSTRING,SettingValue> ConfigSettings;
		typedef pair<TSTRING,SettingValue> ConfigSettingsPair;
		typedef map<TSTRING,SettingValue>::iterator ConfigSettingsIter;
		typedef map<TSTRING,SettingValue>::const_iterator ConfigSettingsCIter;
	public:
		CQSetting(const TSTRING& strName = _T("")):m_strName(strName){}
		~CQSetting(){}
		const TSTRING& Name() const {return m_strName;}
		void SetName(const TSTRING& strName) {m_strName = strName;}
		TSTRING Get(const TSTRING& strName)
		{
			TSTRING strValue;
			ConfigSettingsIter Iter = m_settings.find(strName);
			if(Iter != m_settings.end())
				strValue = Iter->second.m_strValue;
			return strValue;
		}
		TSTRING GetChild(const TSTRING& strName) const
		{
			TSTRING strChild;
			ConfigSettingsCIter Iter = m_settings.find(strName);
			if(Iter != m_settings.end())
				strChild = Iter->second.m_strChild;
			return strChild;
		}
		TSTRING GetValue(int nIndex) const
		{
			TSTRING strValue;
			if(nIndex >= 0 && nIndex < Size())
			{
				ConfigSettingsCIter Iter = m_settings.begin();
				advance(Iter,nIndex);
				strValue = Iter->second.m_strValue;
			}
			return strValue;
		}
		TSTRING GetName(int nIndex) const
		{
			TSTRING strValue;
			if(nIndex >= 0 && nIndex < Size())
			{
				ConfigSettingsCIter Iter = m_settings.begin();
				advance(Iter,nIndex);
				strValue = Iter->first;
			}
			return strValue;
		}	
		void Add(const TSTRING& strName,const TSTRING& strValue,const TSTRING& strChild)
		{
			m_settings.insert(ConfigSettingsPair(strName,SettingValue(strValue,strChild)));
		}
		int Size() const {return static_cast<int>(m_settings.size());}
	protected:
		TSTRING				m_strName;
		ConfigSettings		m_settings;
	};

	class CQConfigXMLParser;

	class QCONFIGURE_CLASS_DLLEXT CQMobileConfigure
	{
	public:
		CQMobileConfigure(void);
		~CQMobileConfigure(void);

		bool    LoadDefaultXML();
		bool    LoadQConfigXML( const TSTRING& strQConfigXML );
		bool    GetMobileSettings( const TSTRING& strMobID, CQSetting& oMobSetting );
		bool    GetSettings( const TSTRING& strAppSetting, CQSetting& oSettings );

	private:
		CQConfigXMLParser*	m_pConfigParser;
		TSTRING						m_strDeafultPath;
		bool									m_bLoaded;
	};


	QCONFIGURE_CLASS_DLLEXT CQMobileConfigure* GetMobileConFig();
	QCONFIGURE_CLASS_DLLEXT void ReleaseMobileConFig();
}


#endif