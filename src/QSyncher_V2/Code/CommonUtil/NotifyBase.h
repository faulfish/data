#ifndef _NOTIFY_BASE_H_
#define _NOTIFY_BASE_H_

#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <afxmt.h>

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

	typedef void (EventFunc)(const TSTRING& strEvent,long nParam);

	interface INotify
	{
		virtual void Event(const TSTRING& strEvent,long nParam) = 0;
	};

	interface INotifyBase
	{	
		virtual bool Register(INotify* pNotify,const TSTRING& strEvent) = 0;
		virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) = 0;
		virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) = 0;
		virtual void DefineNotify(const TSTRING& strNewEvent) = 0;
		virtual void Fire(const TSTRING& strEvent,long nParam) = 0;
	};

	struct NotifyFuncData
	{
	public:
		INotify*	m_pINotify;
		EventFunc*	m_pEventFunc;
		NotifyFuncData(INotify*	pINotify):m_pINotify(pINotify),m_pEventFunc(NULL){}
		NotifyFuncData(EventFunc* pEventFunc):m_pINotify(NULL),m_pEventFunc(pEventFunc){}	
		NotifyFuncData(const NotifyFuncData& rhs):m_pINotify(rhs.m_pINotify),m_pEventFunc(rhs.m_pEventFunc){}
		virtual ~NotifyFuncData(){}	
		NotifyFuncData& operator=(const NotifyFuncData& rhs)
		{
			m_pEventFunc = rhs.m_pEventFunc;
			m_pINotify = rhs.m_pINotify;
			return *this;
		}	
		bool operator==(const NotifyFuncData& rhs) const
		{
			bool bRes = false;
			if(rhs.m_pINotify)
				bRes = (rhs.m_pINotify == m_pINotify);
			else if(rhs.m_pEventFunc)
				bRes = (rhs.m_pEventFunc == m_pEventFunc);
			return bRes;
		}
	};

	//typedef SmartPtr<NotifyFuncData> NotifyFuncDataPt;
	typedef vector<NotifyFuncData> INotifyPool;
	typedef vector<NotifyFuncData>::iterator INotifyPoolIter;
	typedef map<TSTRING,INotifyPool> NotifyMap;	
	typedef map<TSTRING,INotifyPool>::iterator NotifyMapIter;
	typedef pair <TSTRING,INotifyPool> NotifyMapPair;

	template<class ITempInterface>
	class CTNotifyBase : public ITempInterface
	{
	public:
		CTNotifyBase(){}
		virtual ~CTNotifyBase(){}
	private:
		NotifyMapIter exist(const TSTRING& strEvent)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			return m_NotifyMap.find(strEvent);
		}
	protected:
		virtual void DefineNotify(const TSTRING& strNewEvent)
		{		
			CSingleLock singleLock(&m_CritSection,TRUE);

			NotifyMapIter Iter = exist(strNewEvent);
			if(Iter == m_NotifyMap.end())
			{
				INotifyPool newPool;
				m_NotifyMap.insert ( NotifyMapPair ( strNewEvent,newPool ) );
			}
		}
		virtual void Fire(const TSTRING& strEvent,long nParam = 0)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			NotifyMapIter Iter = exist(strEvent);
			assert(Iter != m_NotifyMap.end());
			if(Iter != m_NotifyMap.end())
			{
				for(INotifyPoolIter INotifyPoolIterIter = Iter->second.begin();INotifyPoolIterIter != Iter->second.end();INotifyPoolIterIter++)
				{
					if(NULL != INotifyPoolIterIter->m_pINotify)
						INotifyPoolIterIter->m_pINotify->Event(strEvent,nParam);
					else if(NULL != INotifyPoolIterIter->m_pEventFunc)
						(*(INotifyPoolIterIter->m_pEventFunc))(strEvent,nParam);
				}
				bRes = true;
			}
		}
	private:	
		virtual bool Register(const NotifyFuncData& notifyData,const TSTRING& strEvent)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			NotifyMapIter Iter = exist(strEvent);		
			if(Iter != m_NotifyMap.end())
			{
				INotifyPoolIter INotifyPoolIterIter = find(Iter->second.begin(),Iter->second.end(),notifyData);
				if(INotifyPoolIterIter == Iter->second.end())
					Iter->second.push_back(notifyData);
				bRes = true;
			}
			else
			{
				assert(Iter != m_NotifyMap.end());
#ifdef _DEBUG
				CString strMsg;
				strMsg.Format(_T("Please check if had DefineNotify(%s)"),strEvent);
				AfxMessageBox(strMsg);
#endif
			}
			return bRes;
		}	
		virtual bool Unregister(const NotifyFuncData& notifyData,const TSTRING& strEvent)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			NotifyMapIter Iter = exist(strEvent);
			if(Iter != m_NotifyMap.end())
			{
				INotifyPoolIter INotifyPoolIterIter = find(Iter->second.begin(),Iter->second.end(),notifyData);
				if(INotifyPoolIterIter != Iter->second.end())
					Iter->second.erase(INotifyPoolIterIter);
				bRes = true;
			}
			return bRes;
		}
	public:
		virtual bool Register(INotify* pINotify,const TSTRING& strEvent)
		{
			return Register(NotifyFuncData(pINotify),strEvent);
		}
		virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent)
		{
			return Register(NotifyFuncData(pEventFunc),strEvent);
		}
		virtual bool Unregister(INotify* pINotify,const TSTRING& strEvent)
		{
			return Unregister(NotifyFuncData(pINotify),strEvent);
		}
		virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent)
		{
			return Unregister(NotifyFuncData(pEventFunc),strEvent);
		}
	private:
		NotifyMap		m_NotifyMap;
		CCriticalSection m_CritSection;
	};

	typedef CTNotifyBase<INotifyBase>	CNotifyBase;

	//Must implement INotify
	typedef CTNotifyBase<INotify>		CINotifyBase;
}

#endif //#ifndef _NOTIFY_BASE_H_