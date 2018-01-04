// ListUIDropTraget.cpp : implementation file
//

#include "stdafx.h"
#include "QUIDropTraget.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

// CListUIDropTraget
namespace QSYNC_LIB
{
    IMPLEMENT_DYNAMIC(CUIDropTarget, COleDropTarget)

    CUIDropTarget::CUIDropTarget():m_bIsDropable(false),m_pSelfWnd(NULL),m_bEnableSelf(false)
    {
        m_nClipboardFormat = ::RegisterClipboardFormat( _T("CF__QSYNC_NULL_MSG") );		

        CNotifyBase::DefineNotify(EVENT_DROP);
        CNotifyBase::DefineNotify(EVENT_DRAG_ENTER);
        CNotifyBase::DefineNotify(EVENT_DRAG_OVER);
        CNotifyBase::DefineNotify(EVENT_DRAG_LEAVE);

        ::OleInitialize(NULL);
    }

    CUIDropTarget::~CUIDropTarget() 
    {
        ::OleUninitialize();
    }

    BOOL CUIDropTarget::Register(CWnd* pWnd) 
    {
        m_pSelfWnd = pWnd;
        return COleDropTarget::Register(pWnd);
    }

    DROPEFFECT CUIDropTarget::OnDragEnter(CWnd * pWnd, COleDataObject * pDataObject, DWORD dwKeyState, CPoint point) 
    {
		return HandleDropEvent(pDataObject,dwKeyState,point,EVENT_DRAG_ENTER);
    }

	DROPEFFECT CUIDropTarget::OnDragOver(CWnd * pWnd, COleDataObject * pDataObject, DWORD dwKeyState, CPoint point)
	{
		DROPEFFECT effect = DROPEFFECT_NONE;
		effect = HandleDropEvent(pDataObject,dwKeyState,point,EVENT_DRAG_OVER);

		switch(effect)
		{
		case DROPEFFECT_NONE:
			TRACE(_T("TARGET DROPEFFECT_NONE\n"));
			break;
		case DROPEFFECT_COPY:
			TRACE(_T("TARGET DROPEFFECT_COPY\n"));
			break;
		case DROPEFFECT_MOVE:
			TRACE(_T("TARGET DROPEFFECT_MOVE\n"));
			break;
		case DROPEFFECT_LINK:
			TRACE(_T("TARGET DROPEFFECT_LINK\n"));
			break;
		};

		return effect;
	}

	DROPEFFECT CUIDropTarget::HandleDropEvent(COleDataObject * pDataObject, DWORD dwKeyState, CPoint point,const TSTRING& strEvent)
	{
		CDragDropFireParam dropFireParam;		
        dropFireParam.SetPoint(point);
		dropFireParam.SetKeyState(dwKeyState);
		UINT nFileCount = HDROPToCDragDropFireParam(pDataObject,&dropFireParam);
        dropFireParam.SetEffect(GetDefaultEffect(dwKeyState));
        if(m_bIsDropable)
			Fire(strEvent,(long)&dropFireParam);
		return dropFireParam.GetEffect();
	}

    void CUIDropTarget::OnDragLeave(CWnd * pWnd) 
    {
		Fire(EVENT_DRAG_LEAVE,NULL);
    }

    BOOL CUIDropTarget::OnDrop(CWnd * pWnd, COleDataObject * pDataObject, DROPEFFECT dropEffect, CPoint point) 
    {
        CDragDropFireParam dropFireParam;		
        dropFireParam.SetPoint(point);
        dropFireParam.SetEffect(dropEffect);
		if(pWnd)
			dropFireParam.SetTargetWnd(pWnd->GetSafeHwnd());

        BOOL retVal = FALSE;
        if(!m_bIsDropable) return FALSE;

		UINT nFileCount = HDROPToCDragDropFireParam(pDataObject,&dropFireParam);
        if(nFileCount > 0)
		{
            Fire(EVENT_DROP, (long)&dropFireParam);
            retVal = TRUE;
        }
        return retVal;
    }

	DROPEFFECT CUIDropTarget::GetDefaultEffect(DWORD dwKeyState) const
	{
		DROPEFFECT effect = DROPEFFECT_NONE;        
		if(m_bIsDropable)
		{
			if((dwKeyState & MK_CONTROL) == MK_CONTROL)
				effect = DROPEFFECT_COPY;
			else if((dwKeyState & MK_SHIFT) == MK_SHIFT)
				effect = DROPEFFECT_MOVE;
			else
				effect = DROPEFFECT_COPY;
		}
		return effect;
	}

	int CUIDropTarget::HDROPToCDragDropFireParam(COleDataObject * pDataObject,CDragDropFireParam* pdropFireParam)
	{
		int nResCount = 0;
        if(pDataObject->IsDataAvailable(CF_HDROP)) 
        {
            HGLOBAL hMemData = pDataObject->GetGlobalData(CF_HDROP);
			HDROP hDropFiles = (HDROP)::GlobalLock((HGLOBAL)hMemData);
            if(hDropFiles != NULL)
            {
                TCHAR chTemp[MAX_PATH+1] = {0};
                UINT nFileCount = ::DragQueryFile(hDropFiles, 0xFFFFFFFF, NULL, 0);
                for(UINT nCur = 0; nCur < nFileCount; ++nCur) 
                {
                    ::ZeroMemory(chTemp, MAX_PATH+1);
                    ::DragQueryFile(hDropFiles, nCur, chTemp, MAX_PATH+1);
                    pdropFireParam->Insert(chTemp);
					nResCount++;
                }
            }
			::GlobalUnlock(hMemData);
        }
		return nResCount;
	}

    bool CUIDropTarget::Register(EventFunc* pEventFunc, const TSTRING& strEvent) 
    {
        return CNotifyBase::Register(pEventFunc,strEvent);
    }

    bool CUIDropTarget::Register(INotify* pINotify, const TSTRING& strEvent) 
    {
        return CNotifyBase::Register(pINotify,strEvent);
    }

    bool CUIDropTarget::Unregister(INotify* pINotify, const TSTRING& strEvent) 
    {
        return CNotifyBase::Unregister(pINotify,strEvent);
    }

    bool CUIDropTarget::Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) 
    {
        return CNotifyBase::Unregister(pEventFunc,strEvent);
    }

} // end of namespace

#pragma warning(pop)