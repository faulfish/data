#ifndef _COUTLOOKCTRL_HPP__
#define _COUTLOOKCTRL_HPP__
#pragma once
#include "IOutlookCtrl.h"
#include "IOutlookAccess.h"
#include "OLCalAccess.h"
#include "OLContactAccess.h"

namespace QSYNC_LIB
{

class COutlookCtrl : public CTNotifyBase<IOutlookCtrl>, public QSYNC_LIB::INotify
{
	
public:
	COutlookCtrl(LPCTSTR cpName);
	virtual ~COutlookCtrl();
	virtual bool StartToFetch();
	virtual bool Abort();
	
	virtual bool GetByIndex(int nIndex, IPIMObjPt& PIMObjPt);
	virtual int	 GetCount();
	virtual bool Insert(IPIMObjPt PIMObjPt);
	virtual bool Update(IPIMObjPt PIMObjPt);
	virtual bool Delete(IPIMObjPt PIMObjPt);
	
	static UINT TaskWorkerThreadProc(LPVOID pParam);	
	virtual void Event(const TSTRING& strEvent, long nParam);
	TSTRING GetCtrlName(){return m_strCtrlName;}
protected:
	void WaitForThreadFinish();
private:
	IOutlookAccess*		m_pOLAccess;
	bool								m_bAbort;
	TSTRING					m_strCtrlName;
	CEvent						m_ThreadFinishEvent;
};



}//namespace QSYNC_LIB
#endif // _COUTLOOKCTRL_HPP__
