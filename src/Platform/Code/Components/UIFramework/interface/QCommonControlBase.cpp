///////////////////////////////////////////////////////////
//  QCommonControlBase.cpp
//  Implementation of the Class QCommonControlBase
//  Created on:      12-Feb-2007 17:38:05
//  Original author: Alan tu
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "QCommonControlBase.h"
#include "QControlNotify.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB {

bool QCommonControlBase::SetSubMenu(ISubMenu* pISubMenu)
{
	bool bRes = false;
	if(pISubMenu)
	{
		m_pISubMenu = pISubMenu;
		for(int i=0;i<m_pISubMenu->GetMenuItemCount();i++)
		{
			TSTRING strDisplayID,strNotifyID;
			if(m_pISubMenu->GetMenuItem(i,strDisplayID,strNotifyID))
				DefineNotify(strNotifyID);
		}
	}
	return bRes;
}


void QCommonControlBase::Event(const TSTRING& strEvent, long lparam)
{
	if(strEvent == EVENT_SUBMENU)
	{
		CreateGhostWnd();
		QPoint *pQPoint = (QPoint *)lparam;
		HMENU hMenu = ::CreatePopupMenu();

		if(hMenu)
		{		
			for(int i=0;i<m_pISubMenu->GetMenuItemCount();i++)
			{
				TSTRING strDisplayID,strNotifyID;
				if(m_pISubMenu->GetMenuItem(i,strDisplayID,strNotifyID))
				{
					MENUITEMINFO menuInfo;
					::ZeroMemory(&menuInfo,sizeof(MENUITEMINFO));
					menuInfo.cbSize = sizeof(MENUITEMINFO);
					menuInfo.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID;
					menuInfo.wID = i;
					menuInfo.fType = MFT_STRING;
					menuInfo.fState = MFS_DEFAULT;

					if(m_pISubMenu->IsEnable(i))
						menuInfo.fState |= MFS_ENABLED;
					else
						menuInfo.fState |= MFS_DISABLED;

					if(m_pISubMenu->IsChecked(i))
						menuInfo.fState |= MFS_CHECKED;
					else 
						menuInfo.fState |= MFS_UNCHECKED;

					menuInfo.dwTypeData = const_cast<LPTSTR>(strDisplayID.c_str());
					menuInfo.cch = static_cast<UINT>(strDisplayID.size());

					::InsertMenuItem(hMenu,i,true,&menuInfo);
				}
			}
			::TrackPopupMenu(hMenu,TPM_LEFTALIGN |TPM_RIGHTBUTTON, pQPoint->m_nPosX,pQPoint->m_nPosY,0, m_hWnd,NULL);

			::DestroyMenu(hMenu);
		}
	}
	else if(m_pISubMenu)
	{
		bool bExist = false;
		for(int i=0;i<m_pISubMenu->GetMenuItemCount();i++)
		{
			TSTRING strDisplayID,strNotifyID;
			if(m_pISubMenu->GetMenuItem(i,strDisplayID,strNotifyID))
			{
				if(strEvent.c_str() == strNotifyID)
				{
					Fire(strNotifyID,0);
					break;
				}
			}
		}
	}
}

BOOL QCommonControlBase::CreateGhostWnd()
{
	if(!IsWindow(m_hWnd))
	{
		RegisterWindow();
		m_hWnd = ::CreateWindow(
			_T("CommonMenuWnd"),_T("Empty"),WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			NULL,NULL,::AfxGetApp()->m_hInstance,this);
	}	
	return IsWindow(m_hWnd);
}
BOOL QCommonControlBase::RegisterWindow()
{
	WNDCLASSEX wcx;
	memset(&wcx,0,sizeof(WNDCLASSEX));

	wcx.cbSize = sizeof(WNDCLASSEX);						// Size of structure
	wcx.style = CS_HREDRAW | CS_VREDRAW;					// Redraw if size changes
	wcx.lpfnWndProc = QCommonControlBase::stWinMsgHandler;	// Points to window procedure
	wcx.cbClsExtra = 0;										// No extra class memory
	wcx.cbWndExtra = 0;										// No extra window memory
	wcx.hInstance = ::AfxGetApp()->m_hInstance;				// Handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// Predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);				// Predefined arrow
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// White background brush
	wcx.lpszMenuName = NULL;								// Name of menu resource
	wcx.lpszClassName = _T("CommonMenuWnd");					// Name of window class
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			// Small class icon

	return ::RegisterClassEx(&wcx) != NULL;
}

LRESULT CALLBACK QCommonControlBase::stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    switch (uMsg) 
    { 
        case WM_NCCREATE:			
			::SetWindowLong(hwnd, GWL_USERDATA,(long)((LPCREATESTRUCT(lParam))->lpCreateParams));
			return TRUE;
 
        case WM_COMMAND:
			{
			TSTRING strDisplayID,strNotifyID;
			QCommonControlBase* pThis = (QCommonControlBase*)GetWindowLong(hwnd,GWL_USERDATA);			
			if(pThis && pThis->m_pISubMenu && pThis->m_pISubMenu->GetMenuItem(LOWORD(wParam),strDisplayID,strNotifyID))
				pThis->Fire(strNotifyID,0);
			}
			return TRUE;
		default: 
			break;

	};
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

}

#pragma warning(pop)