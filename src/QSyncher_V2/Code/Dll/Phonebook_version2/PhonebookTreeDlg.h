/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhonebookTreeDlg.h  $
 *  $Revision:   1.3  $
 *  $Date:   Nov 13 2007 10:05:50  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookTreeDlg.h.-arc  $
 * 
 *    Rev 1.3   Nov 13 2007 10:05:50   Alan Tu
 * update UI
 *
 *
 ***************************************************************************/
#pragma once
#include "resource.h"
#include "../../Components/UIFramework/interface/CQCommonTreeCtrl.h"
#include "../../components/uiframework/porting/qportingtreeui.h"
#include "PhonebookDatasource.h"
#include "afxcmn.h"
#include "PhonebookSubmenu.h"
#include "../../Components/UIFramework/interface/QCommonControlBase.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"


// CPhonebookTreeDlg dialog

class CPhonebookTreeDlg : public QSYNC_LIB::Win32DoubleBufferDialogWidge , public QSYNC_LIB::INotify
{
	DECLARE_DYNAMIC(CPhonebookTreeDlg)

public:
	CPhonebookTreeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPhonebookTreeDlg();	

	//INotify
	virtual void Event(const TSTRING& strEvent,long nParam);

	BOOL ShowWindow(int nShow);

// Dialog Data
	enum { IDD = IDD_PHONEBOOK_TREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual		 BOOL OnInitDialog();	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	//afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	QSYNC_LIB::CQPortingTreeUI		m_TreeCtrl;	
	QSYNC_LIB::CPhonebookDatasource	m_TreeDatasource;
	QSYNC_LIB::CQCommonTreeCtrl		m_TreeLogic;

	QSYNC_LIB::QCommonControlBase	m_QCommonControlBase_Tree;
	CPhonebookSubMenu				m_submenu;
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg void OnDestroy();
};
