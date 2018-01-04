#ifndef _CUIOleDataSource_H_
#define _CUIOleDataSource_H_
#pragma once
#include "..\interface\IQListCtrlUI.h"

// CUIOleDataSource command target

class CUIDropSource;
class CUIOleDataSource;

class CUIDropSource : public COleDropSource
{
public:
	CUIDropSource(CUIOleDataSource* pUIOleDataSource):m_pUIOleDataSource(pUIOleDataSource){}
	~CUIDropSource(){}

	virtual BOOL OnBeginDrag(CWnd* pWnd );
	virtual SCODE GiveFeedback( DROPEFFECT dropEffect );
	virtual SCODE QueryContinueDrag( BOOL bEscapePressed, DWORD dwKeyState );
private:
	CUIOleDataSource* m_pUIOleDataSource;
	DROPEFFECT			m_LastEffect;
};

class COMMONUI_API CUIOleDataSource : public COleDataSource , public QSYNC_LIB::CNotifyBase
{
	DECLARE_DYNAMIC(CUIOleDataSource)

public:
	CUIOleDataSource();
	virtual ~CUIOleDataSource();
	int BeginDrag(const QSYNC_LIB::CDragDropFireParam& dragFireParam);
	virtual BOOL OnSetData( LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,  BOOL bRelease );
	virtual BOOL OnRenderGlobalData( LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal );
	virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile );
	virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc,LPSTGMEDIUM lpStgMedium );
	virtual void Fire(const TSTRING& strEvent,long nParam = 0);
protected:
	DECLARE_MESSAGE_MAP()
private:
	CUIDropSource			m_UIDropSource;
};

#endif
