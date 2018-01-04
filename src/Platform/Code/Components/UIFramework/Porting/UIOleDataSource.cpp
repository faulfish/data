// UIOleDataSource.cpp : implementation file
//

#include "stdafx.h"
#include "UIOleDataSource.h"
#include "..\..\..\CommonUtil\DropFiles.h"
#include "../interface/QControlNotify.h"

// CUIOleDataSource

BOOL CUIDropSource::OnBeginDrag(CWnd* pWnd )
{
	TRACE(_T("CUIDropSource::OnBeginDrag\n"));
	return COleDropSource::OnBeginDrag(pWnd);
}

SCODE CUIDropSource::GiveFeedback( DROPEFFECT dropEffect )
{
	TRACE(_T("CUIDropSource::GiveFeedback\n"));
	/*
	Returns DRAGDROP_S_USEDEFAULTCURSORS if dragging is in progress, NOERROR if it is not.
	*/
	switch(dropEffect)
	{
	case DROPEFFECT_NONE:
		TRACE(_T("SOURCE DROPEFFECT_NONE\n"));
		break;
	case DROPEFFECT_COPY:
		TRACE(_T("SOURCE DROPEFFECT_COPY\n"));
		break;
	case DROPEFFECT_MOVE:
		TRACE(_T("SOURCE DROPEFFECT_MOVE\n"));
		break;
	case DROPEFFECT_LINK:
		TRACE(_T("SOURCE DROPEFFECT_LINK\n"));
		break;
	};
	m_LastEffect = dropEffect;
	SCODE sCode = COleDropSource::GiveFeedback(dropEffect);
	if(sCode == DRAGDROP_S_USEDEFAULTCURSORS)
		TRACE(_T("DRAGDROP_S_USEDEFAULTCURSORS\n"));
	else if(sCode == NOERROR)
		TRACE(_T("NOERROR\n"));
	else assert(0);
	return sCode;
}

SCODE CUIDropSource::QueryContinueDrag( BOOL bEscapePressed, DWORD dwKeyState )
{
	TRACE(_T("CUIDropSource::QueryContinueDrag\n"));
	/*
	DRAGDROP_S_CANCEL if the ESC key or right button is pressed, or left button is raised before dragging starts. DRAGDROP_S_DROP if a drop operation should occur. Otherwise S_OK.	
	Contains the state of the modifier keys on the keyboard. This is a combination of any number of the following: MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON, MK_MBUTTON, and MK_RBUTTON.
	*/
	SCODE sCode = COleDropSource::QueryContinueDrag(bEscapePressed, dwKeyState);
	if ( sCode == DRAGDROP_S_DROP ) 
	{
		TRACE(_T("DRAGDROP_S_DROP\n"));
		if(m_pUIOleDataSource)
			m_pUIOleDataSource->Fire(QSYNC_LIB::EVENT_DRAG_END,m_LastEffect);
	}
	return sCode;
}

IMPLEMENT_DYNAMIC(CUIOleDataSource, COleDataSource)


CUIOleDataSource::CUIOleDataSource():m_UIDropSource(this)
{
	DefineNotify(QSYNC_LIB::EVENT_DRAG_END);
}

CUIOleDataSource::~CUIOleDataSource()
{
}


BEGIN_MESSAGE_MAP(CUIOleDataSource, COleDataSource)
END_MESSAGE_MAP()

void CUIOleDataSource::Fire(const TSTRING& strEvent,long nParam)
{
	QSYNC_LIB::CNotifyBase::Fire(strEvent,nParam);
}

int CUIOleDataSource::BeginDrag(const QSYNC_LIB::CDragDropFireParam& dragFireParam)
{
	int nRet = 1;//1 means faile 0 means success
	CDropFiles dropFiles;

	if(dragFireParam.GetFileCount())
	{
		for(int i=0;i<dragFireParam.GetFileCount();i++)
			dropFiles.AddFile(dragFireParam.GetByIndex(i).c_str());
		dropFiles.CreateBuffer();
		CacheGlobalData( CF_HDROP, dropFiles.GetBuffer() );
		//DoDragDrop();
		DoDragDrop(dragFireParam.GetEffect(),NULL,&m_UIDropSource);
		//DoDragDrop(dragFireParam.GetEffect(),NULL,NULL);
		nRet = 0;
	}

	return nRet;
}

BOOL CUIOleDataSource::OnSetData( LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,  BOOL bRelease )
{
	return FALSE;
}

BOOL CUIOleDataSource::OnRenderGlobalData( LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal )
{
	return FALSE;
}

BOOL CUIOleDataSource::OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile )
{
	return FALSE;
}

BOOL CUIOleDataSource::OnRenderData(LPFORMATETC lpFormatEtc,LPSTGMEDIUM lpStgMedium )
{
	return FALSE;
}