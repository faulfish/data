// SyncOutlookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SyncOutlookDlg.h"


// CSyncOutlookDlg dialog
namespace QSYNC_LIB
{

IMPLEMENT_DYNAMIC(CSyncOutlookDlg, Win32DoubleBufferQDialogWidge)

CSyncOutlookDlg::CSyncOutlookDlg(CWnd* pParent /*=NULL*/)
	: Win32DoubleBufferQDialogWidge(CSyncOutlookDlg::IDD, pParent)
{

}

CSyncOutlookDlg::~CSyncOutlookDlg()
{
}
BOOL CSyncOutlookDlg::OnInitDialog()
{
	BOOL nRet = CQDialog::OnInitDialog();
	SetXMLUI();
	return nRet;
}
void CSyncOutlookDlg::SetXMLUI()
{
	//Set Background
	std::wstring strBkImage = L"PhoneBook\\Dlg\\Bk_SyncOutlook.png";
	SetBKImage(strBkImage,BK_Fixed);
	const int nMIN_WIDTH_MAINFRAME = GetBKImage()->GetWidth(); 
	const int nMIN_HEIGHT_MAINFRAME = GetBKImage()->GetHeight();
	SetMinWidth(nMIN_WIDTH_MAINFRAME);
	SetMinHeight(nMIN_HEIGHT_MAINFRAME);
	SetMaxWidth(nMIN_WIDTH_MAINFRAME);
	SetMaxHeight(nMIN_HEIGHT_MAINFRAME);
	//end set background
}

void CSyncOutlookDlg::DoDataExchange(CDataExchange* pDX)
{
	CQDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSyncOutlookDlg, CQDialog)
	ON_WM_SIZING()
END_MESSAGE_MAP()

void CSyncOutlookDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
	GeometryRect rcClient = GetGeometry();
	CQDialog::SetWindowRegion(pRect, rcClient);
	// TODO: Add your message handler code here
}

}//End namespace QSYNC_LIB
// CSyncOutlookDlg message handlers
