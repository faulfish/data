#pragma once

#include "CTabCtrlSSL/TabPageSSL.h"
#include "afxwin.h"

// CCommInterfaceTestPage dialog

class CCommInterfaceTestPage : public CTabPageSSL
{
	DECLARE_DYNAMIC(CCommInterfaceTestPage)

public:
	CCommInterfaceTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommInterfaceTestPage();

// Dialog Data
	enum { IDD = IDD_COMM_INTERFACE_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_InfoList;

public:
	void  UpdateInfoList();
};
