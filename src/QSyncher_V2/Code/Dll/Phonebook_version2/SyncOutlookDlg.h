#pragma once

#include "Resource.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"

namespace QSYNC_LIB
{

// CSyncOutlookDlg dialog

class CSyncOutlookDlg : public Win32DoubleBufferQDialogWidge
{
	DECLARE_DYNAMIC(CSyncOutlookDlg)

public:
	CSyncOutlookDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSyncOutlookDlg();
	virtual BOOL OnInitDialog();
	void SetXMLUI();

	//For QDialog
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

// Dialog Data
	enum { IDD = IDD_SYNC_OUTLOOK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

}//End namespace QSYNC_LIB