#pragma once
#include "resource.h"

// CWaitDialog dialog

class CWaitDialog : public CDialog
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
	CWaitDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaitDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual void OnOK();
};
