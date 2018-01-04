///////////////////////////////////////////////////////////
//  QCommonControlBase.h
//  Implementation of the Class QCommonControlBase
//  Created on:      12-Feb-2007 17:38:04
//  Original author: Alan tu
///////////////////////////////////////////////////////////

#if !defined(EA_36791279_4E20_4158_9DF4_05883A11BA15__INCLUDED_)
#define EA_36791279_4E20_4158_9DF4_05883A11BA15__INCLUDED_

#include "ISubMenu.h"
#include "..\..\..\CommonUtil\NotifyBase.h"
#include "..\..\..\ShareLib\loki-0.1.5\include\loki\Singleton.h"
#include "../CommUIExportAPI.h"

namespace QSYNC_LIB {

class COMMONUI_API QCommonControlBase : public CNotifyBase, public INotify
{

public:
	QCommonControlBase():m_pISubMenu(NULL),m_hWnd(NULL){}
	virtual ~QCommonControlBase()
	{
		if(::IsWindow(m_hWnd))
		{
			::DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
	}

	bool SetSubMenu(ISubMenu* pISubMenu);
	virtual void Event(const TSTRING& strEvent, long lparam);
protected:
	BOOL CreateGhostWnd();
	BOOL RegisterWindow();
	static LRESULT CALLBACK stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
private:
	ISubMenu*	m_pISubMenu;
	HWND		m_hWnd;
};

}
//typedef Loki::SingletonHolder<QSYNC_LIB::QCommonControlBase> CommonControlBaseSingleton;

#endif // !defined(EA_36791279_4E20_4158_9DF4_05883A11BA15__INCLUDED_)

/**************************************************************************/
//sample
/*
class MySubMenu : public QSYNC_LIB::ISubMenu
{	
public:
	virtual int GetMenuItemCount(){return 3;}
	virtual bool GetMenuItem(int nIndex, CString& strDisplayID, CString& strNotifyID)
	{
		bool bRes = false;
		if(nIndex == 0)
		{
			strDisplayID = _T("HELLO");
			strNotifyID = _T("HELLO_NOTIFY");
			bRes = true;
		}
		else if(nIndex == 1)
		{
			strDisplayID = _T("WORLD");
			strNotifyID = _T("WORLD_NOTIFY");
			bRes = true;
		}
		else if(nIndex == 2)
		{
			strDisplayID = _T("!");
			strNotifyID = _T("!_NOTIFY");
			bRes = true;
		}
		return bRes;
	}
};

MySubMenu subMemu;
QCommonControlBase SubmenuBase;
void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	using namespace QSYNC_LIB;

	ClientToScreen(&point);
	QPoint ptPoint;
	ptPoint.m_nPosX = point.x;
	ptPoint.m_nPosY = point.y;	
	SubmenuBase.Event(EVENT_SUBMENU,(long)&ptPoint);	

	CWnd::OnRButtonDown(nFlags, point);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	using namespace QSYNC_LIB;
	SubmenuBase.SetSubMenu(&subMemu);
	for(int i =0;i<subMemu.GetMenuItemCount();i++)
	{
		CString strDisplay,strNotifyID;
		subMemu.GetMenuItem(i,strDisplay,strNotifyID);
		SubmenuBase.Register(this,strNotifyID.GetBuffer(0));
	}

	return 0;
}

void CChildView::Event(const TSTRING& strEvent, long lparam)
{
	AfxMessageBox(strEvent.c_str());
}
*/
/**************************************************************************/
