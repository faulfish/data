/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   QSDialogTemp.cpp  $
 *  $Revision:   1.0  $
 *  $Date:   Jan 12 2007 08:26:36  $ 
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
 ***************************************************************************/
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Calendar_Version2/QSDialogTemp.cpp.-arc  $
   
      Rev 1.0   Jan 12 2007 08:26:36   Evelyn Chen
   Initial revision.
   
      Rev 1.0   Oct 31 2006 17:18:54   Jane Yu
   Initial revision.
   
      Rev 1.6   15 Mar 2006 12:11:22   Keven Yang
   Update font code.
   Qsyncher SCR - for 763: [CUPS1A][File Manager]: Modify the layout of the Thai language.
   
      Rev 1.5   Jul 05 2005 21:30:12   Sky Yang
   extend tiltle width to prevent cutting text.
   projrect SCR- for 360: [Language] update text label.
   
      Rev 1.4   Apr 14 2005 20:38:22   Saffin Liao
   Prevent users from clicking enter,esc and alt+f4 to destroy dialog
   Qsyncher SCR-6 :QSyncher Dialog template  fix UI bug
   
      Rev 1.3   Mar 25 2005 14:52:34   Sky Yang
   tool crashes when switching File Manager view.
   
      Rev 1.2   Mar 14 2005 00:19:24   Sky Yang
   Only create button window and font resource once in OnInitialUpdate() since some modelless dialog (Frame/Statmp dialogs in Image Editor) may call twice.
   Resolution for 89: [Image] Apply Frame & Stamp dialog in Image Editor as QSyncher style.
   
      Rev 1.1   Mar 04 2005 17:01:40   Saffin Liao
   Fix the bug when the dialog height is grater than 400 pixel, a line will appear.
   Qsyncher SCR-6 :QSyncher Dialog template  fix UI bug
   
      Rev 1.0   Mar 02 2005 18:59:50   Neko Hsieh
   Initial revision.
  */
 
 /**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */
// QSDialogTemp.cpp : implementation file
//
 
#include "stdafx.h"
#include "resource.h"
#include "QSDialogTemp.h"
#include <Shlwapi.h> 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQSDialogTemp dialog


CQSDialogTemp::CQSDialogTemp(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CQSDialogTemp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    
	m_back_graphic.LoadBitmap(IDB_BACKGROUND_TEMP);
    m_back_graphic.GetBitmap(&m_graphic_bm);
	m_sys_button_size.cx = 22;
	m_sys_button_size.cy = 22;
	m_captionHeight = 30;
	m_captionLeftOffset  = 10;
	m_captionRightOffset =10;
	m_captionTopOffset = 5;
	m_stitle.LoadString(IDS_TOOL_NAME);
	m_backpattern.LoadBitmap(IDB_BK_MAIN);
	m_ControlBrush.CreatePatternBrush(&m_backpattern);
	m_style = QSD_HAS_CLOSE_SYS_BOX | QSD_HAS_MINIMUM_SYS_BOX;
}

CQSDialogTemp::~CQSDialogTemp()
{
	if(m_font.GetSafeHandle() != NULL)
		m_font.DeleteObject();
}

BOOL CQSDialogTemp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// specify font information.
	m_szFontName.LoadString(IDS_FM_FONT_TYPE);
	ULONG ulFontSize = StrToInt(m_szFontSize);
	LOGFONT defaultLOGFONT;
	CFont*  defaultFont = NULL;
	
	if(m_font.GetSafeHandle() == NULL) {
		defaultFont = this->GetFont();
		defaultFont->GetLogFont(&defaultLOGFONT);
		defaultLOGFONT.lfCharSet = DEFAULT_CHARSET;
		sprintf(defaultLOGFONT.lfFaceName, "%s", m_szFontName);
		m_font.CreateFontIndirect(&defaultLOGFONT);
	} else {
		m_font.GetLogFont(&defaultLOGFONT);
	}
 
    CRect rc;
	GetClientRect(&rc);
	CRect buttonRect;

	int sysBoxCnt = 0;
 

	if((m_style & QSD_HAS_CLOSE_SYS_BOX) && (m_close_button.GetSafeHwnd() == NULL))
	{
		buttonRect.SetRect(rc.right-m_captionRightOffset-m_sys_button_size.cx*(++sysBoxCnt),rc.top+m_captionTopOffset,
						   rc.right-m_captionRightOffset-m_sys_button_size.cx*(sysBoxCnt-1),rc.top+m_sys_button_size.cy+m_captionTopOffset);
		m_close_button.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, buttonRect, this, CAPTION_CLOSE_ID);
	
		m_close_button.SetSkin(IDB_CAPTION_CLOSE_N,IDB_CAPTION_CLOSE_X,
							   IDB_CAPTION_CLOSE_F,IDB_CAPTION_CLOSE_D,
							   IDB_CAPTION_CLOSE_N,0,0,0,0);
	}
    
	if((m_style & QSD_HAS_MINIMUM_SYS_BOX) && (m_minimize_button.GetSafeHwnd() == NULL))
	{
		buttonRect.SetRect(rc.right-m_captionRightOffset-m_sys_button_size.cx*(++sysBoxCnt),rc.top+m_captionTopOffset,
						   rc.right-m_captionRightOffset-m_sys_button_size.cx*(sysBoxCnt-1),rc.top+m_sys_button_size.cy+m_captionTopOffset);
    
		m_minimize_button.Create(_T(""), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, buttonRect, this, CAPTION_MINIMIZE_ID);
 	
		m_minimize_button.SetSkin(IDB_CAPTION_MINIMIZE_N,IDB_CAPTION_MINIMIZE_X,
								  IDB_CAPTION_MINIMIZE_F,IDB_CAPTION_MINIMIZE_D,
								  IDB_CAPTION_MINIMIZE_N,0,0,0,0);
	}
	
	if(m_title.GetSafeHwnd() == NULL)
	{
		buttonRect.SetRect(rc.left+m_captionLeftOffset,rc.top+m_captionTopOffset,rc.right-m_captionTopOffset,rc.top+20+m_captionTopOffset);
		m_title.Create(m_stitle, WS_CHILD|WS_VISIBLE|SS_LEFT, buttonRect, this);
 
		m_title.SetFont(defaultLOGFONT);
		m_title.SetFontBold(false);
		m_title.SetTransparent(true);
	}

	HWND hwndOwner; 
	RECT rcDlg, rcOwner; 

	if ((hwndOwner = ::GetParent(this->m_hWnd)) == NULL)
		hwndOwner = ::GetDesktopWindow(); 

	::GetWindowRect(hwndOwner, &rcOwner); 
	::GetWindowRect(this->m_hWnd, &rcDlg); 
	::CopyRect(&rc, &rcOwner); 
	
	// Offset the owner and dialog box rectangles so that 
	// right and bottom values represent the width and 
	// height, and then offset the owner again to discard 
	// space taken up by the dialog box. 
	
	OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top); 
	OffsetRect(&rc, -rc.left, -rc.top); 
	OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom); 
	
	// The new position is the sum of half the remaining 
	// space and the owner's original position. 
	
	::SetWindowPos(this->GetSafeHwnd(), 
		HWND_TOP, 
		rcOwner.left + (rc.right / 2), 
		rcOwner.top + (rc.bottom / 2), 
		0, 0,          // ignores size arguments 
		SWP_NOSIZE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CQSDialogTemp, CDialog)
	//{{AFX_MSG_MAP(CQSDialogTemp)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQSDialogTemp message handlers

void CQSDialogTemp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect rc;
	GetClientRect(&rc);

    CDC MemDC;
	if (!MemDC.CreateCompatibleDC(&dc)) return; 

	MemDC.SelectObject(&m_back_graphic); // select back-image
	
	const int backimgWidth = m_graphic_bm.bmWidth;
	const int backimgHeight = m_graphic_bm.bmHeight;
	const int heightTopOffset = 31;
	const int heightButtomOffset = 30;
	const int widthOffset  = 30;
    int   stretchWidth = 0;
	int   stretchHeight=0;
	
	
	dc.BitBlt(0,0,widthOffset,heightTopOffset,&MemDC,0,0,SRCCOPY); // draw upper-left corner
 
	while(stretchWidth<rc.Width()-2*widthOffset) //draw upper-border
    {
	 dc.StretchBlt(widthOffset+stretchWidth,0,backimgWidth-2*widthOffset,heightTopOffset,&MemDC,
		                        widthOffset,0,backimgWidth-2*widthOffset,heightTopOffset,SRCCOPY);
     stretchWidth += backimgWidth-2*widthOffset;
	}
    
	stretchWidth = 0;
	
    while (stretchHeight<rc.Height()-heightTopOffset-heightButtomOffset) //draw center area
    {
	  while (stretchWidth<rc.Width()-2*widthOffset)
      {
       dc.StretchBlt(widthOffset+stretchWidth,heightTopOffset+stretchHeight,backimgWidth-2*widthOffset,backimgHeight-heightTopOffset-heightButtomOffset,&MemDC,
		                          widthOffset,heightTopOffset,backimgWidth-2*widthOffset,backimgHeight-heightTopOffset-heightButtomOffset,SRCCOPY);
	   stretchWidth += backimgWidth-2*widthOffset;
	  }
	   stretchWidth = 0;
	   stretchHeight += backimgHeight-heightTopOffset-heightButtomOffset;
	}

	stretchWidth = 0;
	while(stretchWidth<rc.Width()-2*widthOffset) //draw buttom border
    {
	 dc.StretchBlt(widthOffset+stretchWidth,rc.Height()-heightButtomOffset,backimgWidth-2*widthOffset,heightButtomOffset,&MemDC,
		          widthOffset,backimgHeight-heightButtomOffset,backimgWidth-2*widthOffset,heightButtomOffset,SRCCOPY);
     stretchWidth += backimgWidth-60;
	}

	stretchHeight = 0;
	
	while(stretchHeight<rc.Height()-heightTopOffset-heightButtomOffset) //draw left border
	{
	 dc.StretchBlt(0,heightTopOffset+stretchHeight,widthOffset,backimgHeight-heightTopOffset-heightButtomOffset,&MemDC,
		           0,heightTopOffset,widthOffset,backimgHeight-heightTopOffset-heightButtomOffset,SRCCOPY);
     stretchHeight += backimgHeight-heightTopOffset-heightButtomOffset;
	}

	stretchHeight = 0;
	while(stretchHeight<rc.Height()-heightTopOffset-heightButtomOffset) //draw right border
    {
	 dc.StretchBlt(rc.Width()-widthOffset,heightTopOffset+stretchHeight,widthOffset,backimgHeight-heightTopOffset-heightButtomOffset,&MemDC,
		         backimgWidth-widthOffset,heightTopOffset,widthOffset,backimgHeight-heightTopOffset-heightButtomOffset,SRCCOPY);
     stretchHeight += backimgHeight-heightTopOffset-heightButtomOffset;
	}
    
	// draw buttomleft corner
	dc.BitBlt(0,rc.Height()-heightButtomOffset,widthOffset,heightButtomOffset,&MemDC,0,backimgHeight-heightButtomOffset,SRCCOPY);
	// draw topright corner
	dc.BitBlt(rc.Width()-widthOffset,0,widthOffset,heightTopOffset,&MemDC,backimgWidth-widthOffset,0,SRCCOPY);
	// draw buttomright corner
	dc.BitBlt(rc.Width()-widthOffset,rc.Height()-heightButtomOffset,widthOffset,heightTopOffset,&MemDC,backimgWidth-widthOffset,backimgHeight-heightButtomOffset,SRCCOPY);

	// Do not call CDialog::OnPaint() for painting messages
}

void CQSDialogTemp::OnSize(UINT nType, int cx, int cy) 
{
//	CDialog::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);
	CRect buttonRect;

	int sysBoxCnt = 0;
	
	if((m_style & QSD_HAS_CLOSE_SYS_BOX) && (m_close_button.GetSafeHwnd() != NULL))
	{
		buttonRect.SetRect(rc.right-m_captionRightOffset-m_sys_button_size.cx*(++sysBoxCnt),rc.top+m_captionTopOffset,
						   rc.right-m_captionRightOffset-m_sys_button_size.cx*(sysBoxCnt-1),rc.top+m_sys_button_size.cy+m_captionTopOffset);
		m_close_button.MoveWindow(buttonRect);
	}
    
	if((m_style & QSD_HAS_MINIMUM_SYS_BOX) && (m_minimize_button.GetSafeHwnd() != NULL))
	{
		buttonRect.SetRect(rc.right-m_captionRightOffset-m_sys_button_size.cx*(++sysBoxCnt),rc.top+m_captionTopOffset,
						   rc.right-m_captionRightOffset-m_sys_button_size.cx*(sysBoxCnt-1),rc.top+m_sys_button_size.cy+m_captionTopOffset);
		m_minimize_button.MoveWindow(buttonRect);
	}
}

void CQSDialogTemp::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
    
	if (point.y <= m_captionHeight)
    {
	 PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
    }
}

HBRUSH CQSDialogTemp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkMode(TRANSPARENT);
		hbr = this->m_ControlBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CQSDialogTemp::OnSysClose()
{
	::PostMessage(this->GetSafeHwnd(), WM_SYSCOMMAND, SC_CLOSE, NULL);
}

void CQSDialogTemp::OnSysMinimize()
{
	::PostMessage(this->GetSafeHwnd(), WM_SYSCOMMAND, SC_MINIMIZE, NULL);
}

void CQSDialogTemp::SetTitleText(CString text)
{
    m_stitle = text;

	if(m_title.GetSafeHwnd() != NULL)
		m_title.SetWindowText(text);
}

void CQSDialogTemp::SetStyle(const UINT newStyle)
{
    m_style = (newStyle & (QSD_HAS_CLOSE_SYS_BOX | QSD_HAS_MINIMUM_SYS_BOX));
}

void CQSDialogTemp::OnSysCommand(UINT nID, LPARAM lParam) 
{
     CDialog::OnSysCommand(nID, lParam);
}
