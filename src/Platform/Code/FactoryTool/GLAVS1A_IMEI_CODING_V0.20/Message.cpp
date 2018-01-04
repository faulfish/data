// Message.cpp : implementation file
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "Message.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessage dialog


CMessage::CMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessage)
	DDX_Control(pDX, IDC_STATIC_ERROR, m_ERROR);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_MESSAGE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessage, CDialog)
	//{{AFX_MSG_MAP(CMessage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessage message handlers

HBRUSH CMessage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor ==CTLCOLOR_DLG)
         return m_brush;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CMessage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_brush.CreateSolidBrush(RGB(200,200,100));
	m_OK.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_OK)));
	//*******************************
	if(Error_Code_String=="PASS")
	{
		if(IMEI_BT_GSN_Flag)//already have BT/GSN
		{
			Message_String=Error_Code_String;
			m_ERROR.SetBkColor(RGB(0, 255, 0));
			m_ERROR.SetTextColor(RGB(0, 0, 255));
			m_ERROR.SetFontSize(50);
			m_ERROR.SetWindowText(Message_String);
			m_ERROR.SetFontBold(TRUE);

			if(Write_MAL_Flag)//write MAL
				Message_String=IMEI_FROM_SCANNER+"\n"+MAL_FROM_SCANNER+"\n"+GSN_String+"\n"+BT_Address;
            else
				Message_String=IMEI_FROM_SCANNER+"\n"+GSN_String+"\n"+BT_Address;

			m_MESSAGE.SetBkColor(RGB(0, 255, 0));
			m_MESSAGE.SetTextColor(RGB(0, 0, 255));
			m_MESSAGE.SetFontSize(25);
			m_MESSAGE.SetWindowText(Message_String);
			m_MESSAGE.SetFontBold(TRUE);
		}
		else
		{
			Message_String=Error_Code_String;
			m_ERROR.SetBkColor(RGB(0, 255, 0));
			m_ERROR.SetTextColor(RGB(0, 0, 255));
			m_ERROR.SetFontSize(50);
			m_ERROR.SetWindowText(Message_String);
			m_ERROR.SetFontBold(TRUE);

			m_MESSAGE.SetBkColor(RGB(0, 255, 0));
			m_MESSAGE.SetTextColor(RGB(0, 0, 255));
			m_MESSAGE.SetFontSize(25);
			m_MESSAGE.SetWindowText("");
			m_MESSAGE.SetFontBold(TRUE);
		}
	}
	else//error
	{
		if(IMEI_BT_GSN_Flag)//already have BT/GSN
		{
			Message_String=Error_Code_String;
			m_ERROR.SetBkColor(RGB(255, 0, 0));
			m_ERROR.SetTextColor(RGB(0, 0, 255));
			m_ERROR.SetFontSize(50);
			m_ERROR.SetWindowText(Message_String);
			m_ERROR.SetFontBold(TRUE);

			if(Write_MAL_Flag)//write MAL
				Message_String=IMEI_FROM_SCANNER+"\n"+MAL_FROM_SCANNER+"\n"+GSN_String+"\n"+BT_Address;
            else
				Message_String=IMEI_FROM_SCANNER+"\n"+GSN_String+"\n"+BT_Address;

			m_MESSAGE.SetBkColor(RGB(255, 0, 0));
			m_MESSAGE.SetTextColor(RGB(0, 0, 255));
			m_MESSAGE.SetFontSize(25);
			m_MESSAGE.SetWindowText(Message_String);
			m_MESSAGE.SetFontBold(TRUE);
		}
		else
		{
			Message_String=Error_Code_String;
			m_ERROR.SetBkColor(RGB(255, 0, 0));
			m_ERROR.SetTextColor(RGB(0, 0, 255));
			m_ERROR.SetFontSize(50);
			m_ERROR.SetWindowText(Message_String);
			m_ERROR.SetFontBold(TRUE);

			m_MESSAGE.SetBkColor(RGB(255, 0, 0));
			m_MESSAGE.SetTextColor(RGB(0, 0, 255));
			m_MESSAGE.SetFontSize(25);
			m_MESSAGE.SetWindowText("");
			m_MESSAGE.SetFontBold(TRUE);
		}
	}
	return true;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


