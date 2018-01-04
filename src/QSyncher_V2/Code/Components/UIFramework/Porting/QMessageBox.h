#ifndef __QMESSAGEBOX_H__
#define __QMESSAGEBOX_H__

#pragma once

#include "QDialog.h"
#include "..\LayoutManager\Win32FrameworkWidge.h"
#include "..\LayoutManager\BoxLayout.h"
#include "..\LayoutManager\SplitLayout.h"
#include "..\LayoutManager\WIN32IconButtonWidgeVer2.h"

// QMessageBox dialog

namespace QSYNC_LIB {

	enum MESSAGERETURN	
	{
		QIDOK		=	1,		//   The OK button was selected.
		QIDCANCEL	=	2,		//   The Cancel button was selected.
		QIDABORT	=	3,		//   The Abort button was selected.
		QIDRETRY	=	4,		//   The Retry button was selected.
		QIDIGNORE	=	5,		//   The Ignore button was selected.
		QIDYES		=	6,		//   The Yes button was selected.
		QIDNO		=	7		//   The No button was selected.
	};

	enum MESSAGETYPE
	{		
		QMB_OK					=	0x00000000L,//   The message box contains one pushbutton: OK.
		QMB_OKCANCEL,							//   The message box contains two pushbuttons: OK and Cancel.
		QMB_ABORTRETRYIGNORE,					//   The message box contains three pushbuttons: Abort, Retry, and Ignore.		
		QMB_YESNOCANCEL,						//   The message box contains three pushbuttons: Yes, No, and Cancel.
		QMB_YESNO,								//   The message box contains two pushbuttons: Yes and No.
		QMB_RETRYCANCEL							//   The message box contains two pushbuttons: Retry and Cancel.				
	};

class QMessageBox : public Win32DoubleBufferQDialogWidge
{
	DECLARE_DYNAMIC(QMessageBox)

public:
	QMessageBox(CWnd* pParent = NULL);   // standard constructor
	QMessageBox(const std::wstring& strMessage, const TSTRING& strTitle, MESSAGETYPE nID,CWnd* pParent =NULL);
	virtual ~QMessageBox();
	void SetMessageType(MESSAGETYPE nID){m_nID = nID; SetWarningImage();}
	void SetWarningText(const std::wstring& strWarningText){m_strWarningText = strWarningText;}

	virtual BOOL OnInitDialog();
	virtual void OnSizing(UINT fwSide, LPRECT pRect);
	virtual BOOL		PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	bool	SetUI();
	BoxLayout<HDC>* SetButtonLayout();
	void SetWarningImage();

//UI setting functions:
	void SetBackground(int);
	SplitLayout<HDC>* CreateCaptionLayout();
	SplitLayout<HDC>* CreateClientLayout();

//Button functions:
	static void OK();
	static void Cancel();
	static void Retry();
	static void Yes();
	static void No();
	static void Abort();
	static void Ignore();

	static void WinClose();
	void FnWinClose();

private:
	std::wstring m_strWarningImage;
	std::wstring m_strWarningText;
	std::wstring m_strTitleText;
	INT m_nNumberOfTextLines;
	MESSAGETYPE	 m_nID;
	ILocalization*								m_pILocalization;
};
}//namespace QSYNC_LIB

#endif // __QMESSAGEBOX_H__