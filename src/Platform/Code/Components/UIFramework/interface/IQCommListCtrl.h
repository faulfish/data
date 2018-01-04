///////////////////////////////////////////////////////////
//  CQCommListCtrl.h
//  Implementation of the Class CQCommListCtrl
//  Created on:      15-Feb-2007 11:07:34
//  Original author: Eddy Guo
///////////////////////////////////////////////////////////

#if !defined(EA_1B553248_F342_4409_9BBC_81164011EC0F__INCLUDED_)
#define EA_1B553248_F342_4409_9BBC_81164011EC0F__INCLUDED_

#include "..\..\..\CommonUtil\NotifyBase.h"
#include "IListDataSource.h"
#include "IQListCtrlUI.h"
#include "QControlNotify.h"

namespace QSYNC_LIB
{

	class COMMONUI_API CQCommListCtrl : public CNotifyBase , public INotify
	{

	public:
		CQCommListCtrl(IDataSource* source, IQListCtrlUI* listCtrl);
		virtual ~CQCommListCtrl();

		virtual void Event(const TSTRING& strEvent, long nParam);
		virtual bool Register(INotify* pNotify, const TSTRING& strEvent) 
		{
			return CNotifyBase::Register(pNotify, strEvent);
		}
		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent) 
		{
			return CNotifyBase::Register(pEventFunc, strEvent);
		}
		virtual bool Unregister(INotify* pNotify, const TSTRING& strEvent) 
		{
			return CNotifyBase::Unregister(pNotify, strEvent);
		}
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) 
		{
			return CNotifyBase::Unregister(pEventFunc, strEvent);
		}
		virtual bool GetSelectedItem(vector<FireListDataParameter>& vectSelected)
		{
			bool bRes = false;
			if(m_pQListCtrl)
				bRes = m_pQListCtrl->GetSelectedItem(vectSelected);
			return bRes;
		}
		virtual void RefreshClear()
		{
			if(m_pQDataSource)
				m_pQDataSource->RefreshClear();
			if(m_pQListCtrl)
				m_pQListCtrl->RefreshClear();
		}

	private:
		IDataSource* m_pQDataSource;
		IQListCtrlUI* m_pQListCtrl;

	};
}
#endif // !defined(EA_1B553248_F342_4409_9BBC_81164011EC0F__INCLUDED_)
