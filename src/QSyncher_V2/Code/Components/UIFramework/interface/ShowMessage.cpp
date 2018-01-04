#pragma once
#include "ShowMessage.h"
#include "stdafx.h"
#include "../../../CommonUtil/UnicodeUtility.h"
#include "../Porting/QMessageBox.h"


namespace QSYNC_LIB
{
	COMMONUI_API MESSAGERETURN ShowMessage(HWND hWnd, const TSTRING& strMessage, const TSTRING& strTitle, MESSAGETYPE nID = QMB_OK)
	{
		CWnd *wnd = CWnd::FromHandle(hWnd);
		QMessageBox Message(GetUnicode(strMessage), strTitle, (MESSAGETYPE)nID,wnd);
		return (MESSAGERETURN)Message.DoModal();
	}
	COMMONUI_API MESSAGERETURN ShowMessage(HWND hWnd, LPCTSTR strMessage, LPCTSTR strTitle, MESSAGETYPE nID = QMB_OK)
	{
		CWnd *wnd = CWnd::FromHandle(hWnd);
		QMessageBox Message(GetUnicode(strMessage), strTitle, (MESSAGETYPE)nID,wnd);
		return (MESSAGERETURN)Message.DoModal();
	}
}

	

