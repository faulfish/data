#ifndef __ANONYMOUS_CONTACT_DIALOG_H__
#define __ANONYMOUS_CONTACT_DIALOG_H__

#include "../../Components/Localization/ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "../../Components/UIFramework/Porting/QEditCtrl.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "Resource.h"
#pragma once

using namespace QSYNC_LIB;

// CAnonymousContactDialog dialog

class CAnonymousContactDialog : public Win32DoubleBufferQDialogWidge,public QSYNC_LIB::CINotifyBase
{
	DECLARE_DYNAMIC(CAnonymousContactDialog)

public:
    // Dialog Data
    enum { IDD = IDD_MESSAGE_ANONYMOUS_CONTACT_DIALOG };
	CAnonymousContactDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnonymousContactDialog();
	void Event(const TSTRING& strEvent, long nParam);

    void SetExtractStrings(CString& strContact1, CString& strContact2, CString& strContact3, CString& strContact4, CString& strContact5);

private:
    CString* m_pStrContact1;
    CString* m_pStrContact2;
    CString* m_pStrContact3;
    CString* m_pStrContact4;
    CString* m_pStrContact5;

	WIN32IconButton* m_pBtnCancel;
	WIN32IconButton* m_pBtnOK;
	WIN32IconButton* m_pWinCloseButton;

	IPIMCONFIG* m_pIPIMConfig;

	CQEditCtrl					m_QEditCtrlContact1;
	CQEditCtrl					m_QEditCtrlContact2;
	CQEditCtrl					m_QEditCtrlContact3;
	CQEditCtrl					m_QEditCtrlContact4;
	CQEditCtrl					m_QEditCtrlContact5;

private:
	void SetUI();
	void SetXMLUI();
	void SetBackground();
	void SetPhoneCheckingWords();
	bool GetLimitation(const TSTRING& strKey, TSTRING& strLimitation);
	SplitLayout<HDC>* CreateCaptionLayout();
	SplitLayout<HDC>* CreateClientLayout();
	SplitLayout<HDC>* CreateSubWindowLayout();
	BoxLayout<HDC>* CreateButtonLayout();
	SplitLayout<HDC>* CreateSubclientLayout();

	//Button function:
	static void WinClose();
	void FnWinClose();
	static void Cancel();
	void OnBnClickedCancel();
	static void OK();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

#endif