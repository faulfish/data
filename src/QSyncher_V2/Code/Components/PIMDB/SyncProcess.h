/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   syncprocess.h  $
 *  $Revision:   1.3  $
 *  $Date:   Mar 26 2008 18:30:48  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/PIMDB/SyncProcess.h.-arc  $
 * 
 *    Rev 1.3   Mar 26 2008 18:30:48   Jame Tsai
 * refactory
 * 
 *    Rev 1.2   Mar 19 2008 14:36:28   Alan Tu
 * add return value
 * 
 *    Rev 1.1   Dec 11 2007 09:16:16   Alan Tu
 * update
 * 
 *    Rev 1.0   Nov 12 2007 18:08:58   Alan Tu
 * Initial revision.
 *
 *
 ***************************************************************************/
#ifndef _SyncProcess_BASE_H_
#define _SyncProcess_BASE_H_

#pragma once

#include "SyncManager.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include <vector>

namespace QSYNC_LIB
{

#define		EVENT_SyncProcess_FINISH	_T("TNotify_SyncProcess_Finish")

	template <class SyncDevice>
	class SyncProcess : public CINotifyBase
	{
	public:
		SyncProcess(SyncDevice*	pQDataCtrl):m_pQDataCtrl(pQDataCtrl),m_bFinish(true)
		{
			DefineNotify(EVENT_SyncProcess_FINISH);
		}
		~SyncProcess(){}
		void push(IDevice* pIDevice)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			if(pIDevice)
				m_SyncPool.push_back(pIDevice);
		}
		IDevice* pop()
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			IDevice* pIDevice = NULL;
			if(!m_SyncPool.empty())
			{
				pIDevice = m_SyncPool.back();
				m_SyncPool.pop_back( );
			}
			return pIDevice;
		}
		void start(SyncType synctype)
		{
			assert(m_bFinish);
			if(m_bFinish)
			{
				m_synctype =  synctype;
				m_bFinish = false;
				Process();
			}
		}
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(g_strNotifyDBFinish == strEvent)
			{
				Process();
			}
		}
		bool IsFinish(){return m_bFinish;}

		virtual bool Register(INotify* pINotify,const TSTRING& strEvent)
		{
			return CINotifyBase::Register(pINotify,strEvent);
		}
		virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent)
		{
			return CINotifyBase::Register(pEventFunc,strEvent);
		}
		virtual bool Unregister(INotify* pINotify,const TSTRING& strEvent)
		{
			return CINotifyBase::Unregister(pINotify,strEvent);
		}
		virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent)
		{
			return CINotifyBase::Unregister(pEventFunc,strEvent);
		}
	protected:
		bool Process()
		{
			m_pTempIDevice = pop();
			if(m_pTempIDevice)
			{
				m_pTempIDevice->DataBaseRegister(this,g_strNotifyDBFinish);
				m_pQDataCtrl->Sync(m_pTempIDevice,m_synctype);
			}
			else
			{
				m_bFinish = true;
				Fire(EVENT_SyncProcess_FINISH);
			}
			return m_bFinish;
		}
	private:
		IDevice*							m_pTempIDevice;
		SyncDevice*					m_pQDataCtrl;
		SyncType						m_synctype;
		std::vector<IDevice*>	m_SyncPool;
		bool									m_bFinish;
		CCriticalSection			m_CritSection;
	};

}

#endif //_SyncProcess_BASE_H_