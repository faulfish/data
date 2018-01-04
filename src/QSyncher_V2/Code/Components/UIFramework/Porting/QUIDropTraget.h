
#pragma once
#include "../../../CommonUtil/NotifyBase.h"
#include "../interface/QControlNotify.h"
#include "../interface/IQListCtrlUI.h"
namespace QSYNC_LIB
{

// CListUIDropTraget command target

class CQListCtrl;

class COMMONUI_API CUIDropTarget : public COleDropTarget, public CNotifyBase
{
    DECLARE_DYNAMIC(CUIDropTarget)

public:
	CUIDropTarget();
	virtual ~CUIDropTarget();
    BOOL Register(CWnd* pWnd);
	bool IsDropable()
	{
		return m_bIsDropable;
	}
	void SetDropable(bool bEnable)
	{
		if(m_bIsDropable != bEnable)
			m_bIsDropable = bEnable;
	}
	void EnableSelf(bool bEnableSelf)
	{
		m_bEnableSelf = bEnableSelf; 
	}
    virtual bool Register(INotify* pNotify,const TSTRING& strEvent);
	virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent);
	virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent);
	virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent);
protected:
    //DECLARE_MESSAGE_MAP()
	virtual DROPEFFECT	OnDragEnter(CWnd * pWnd, COleDataObject * pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT	OnDragOver(CWnd * pWnd, COleDataObject * pDataObject, DWORD dwKeyState, CPoint point);
    virtual void						OnDragLeave(CWnd * pWnd);
	virtual BOOL					OnDrop(CWnd * pWnd, COleDataObject * pDataObject, DROPEFFECT dropEffect, CPoint point);

private:
    void						AddFile(LPTSTR fileName);
	int						HDROPToCDragDropFireParam(COleDataObject * pDataObject,CDragDropFireParam* pdropFireParam);
	DROPEFFECT GetDefaultEffect(DWORD dwKeyState) const;
	DROPEFFECT HandleDropEvent(COleDataObject * pDataObject, DWORD dwKeyState, CPoint point,const TSTRING& strEvent);

private:
	UINT			m_nClipboardFormat;
    bool				m_bIsDropable;
	bool				m_bEnableSelf;
	CWnd*		m_pSelfWnd;
};



} // end of namespace