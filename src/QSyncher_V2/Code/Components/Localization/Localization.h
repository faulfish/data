///////////////////////////////////////////////////////////
//  Localization.h
//  Implementation of the Class Localization
//  Created on:      06-Feb-2007 11:25:55
///////////////////////////////////////////////////////////

#if !defined(EA_89C5B65B_A4FD_495b_84D6_2A266DDD74C9__INCLUDED_)
#define EA_89C5B65B_A4FD_495b_84D6_2A266DDD74C9__INCLUDED_

#include "ILocalization.h"
#include <map>
#include "..\..\Components\ConfigureTool\IConfigure.h"

namespace QSYNC_LIB
{
	#define User_Language	_T("User_Language")
	#define ModuleName		_T("Localization")

	class Localization : public ILocalization , public CNotifyBase
	{
	public:
		Localization(void);
		virtual ~Localization(void);
		virtual void GetString(const TSTRING& strID,TSTRING& strRes);
		virtual bool SetRegion(const TSTRING& strRegion);
		virtual TSTRING GetString(const TSTRING& strID);
		virtual int GetRegionNum(void);										 //mei adds
		virtual void GetRegion(int nIndex, TSTRING& strRegion);				 //mei adds
		virtual void GetRegionTitle(int nIndex, TSTRING& strRegionTitle);    //mei adds
		virtual void GetCurrentRegion(TSTRING& strRegion);					 //mei adds
		virtual void GetDefaultRegion(TSTRING& strRegion);					 //mei adds
		virtual LCID GetCurrentLCID();										 //mei adds
		virtual bool GetDate(LCID lcid, int nYear, int nMonth, int nDay, LPCTSTR lpFormat, TSTRING& strDate, DWORD dwflags=DATE_SHORTDATE);	   //mei adds
		virtual bool GetTime(LCID lcid, int nHour, int nMinute, int nSecond, LPCTSTR lpFormat, TSTRING& strTime, DWORD dwflags=TIME_NOSECONDS);  //mei adds
		virtual bool Register(INotify* pNotify,const TSTRING& strEvent);
		virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent);
		virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent);
		virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent);
	protected:
		bool LoadRegionData(const TSTRING& strRegion);
		void GetStringData(const TSTRING& strINIFileName,const TSTRING& strRegion);
		void InsertData(const TSTRING& strKeyValue,const TSTRING& strRegion);
		void InitVectorLCID(LPCTSTR lpFolderPath);							  //mei adds
		LCID GetLCID(LPCTSTR lpINIFilePath);								  //mei adds
		int FindLCIDLocation(LPCTSTR lpRegion);								  //mei adds 
		bool LogUnfoundID(const TSTRING& strRegion,const TSTRING& strID, const TSTRING& strValue); //mei adds
		bool IsNormalStringID(const TSTRING& strID);

	private:
		TSTRING													m_strCurrentRegion;
		TSTRING													m_strDefaultRegion;
		LCID															m_CurrentLCID;
		TSTRING													m_strINIFolderPath;
		TSTRING													m_strCurrentLangMenu;
		std::map<TSTRING,TSTRING>				m_mapStrings; 
		std::vector<LCID>										m_vectorLCID;
		QSYNC_LIB::IConfigure*							m_IConfigure;
	};
}
#endif // !defined(EA_89C5B65B_A4FD_495b_84D6_2A266DDD74C9__INCLUDED_)
