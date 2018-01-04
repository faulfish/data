#pragma once

#include "CTabCtrlSSL/TabPageSSL.h"

// CIPhonebookTestPage dialog

class CIPhonebookTestPage : public CTabPageSSL
{
	DECLARE_DYNAMIC(CIPhonebookTestPage)

public:
	CIPhonebookTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIPhonebookTestPage();

// Dialog Data
	enum { IDD = IDD_IPhonebook_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
