// ColoredStatusBarCtrlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "ColoredStatusBarCtrlCtrl.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrlCtrl

CColoredStatusBarCtrlCtrl::CColoredStatusBarCtrlCtrl()
{
}

CColoredStatusBarCtrlCtrl::~CColoredStatusBarCtrlCtrl()
{
}


BEGIN_MESSAGE_MAP(CColoredStatusBarCtrlCtrl, CStatusBarCtrl)
	//{{AFX_MSG_MAP(CColoredStatusBarCtrlCtrl)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrlCtrl message handlers

void CColoredStatusBarCtrlCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Attach to a CDC object
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	dc.SetBkMode(TRANSPARENT);

	// Get the pane rectangle and calculate text coordinates
	CRect rect(&lpDrawItemStruct->rcItem);

	switch(lpDrawItemStruct->itemID) 
	{

		case 0:
			dc.SetTextColor(RGB(0, 0, 255));
			//dc.TextOut(rect.left+2, rect.top+1, "         TEST   RECORD");
			dc.DrawText("TEST    RECORD", rect,DT_CENTER|DT_VCENTER);
			break;

		case 1:
			dc.SetTextColor(RGB(0, 0, 255));
			//dc.TextOut(rect.left+2, rect.top+1, Total_String);
			dc.DrawText(Total_String, rect,DT_VCENTER);
			break;

		case 2:
			dc.SetTextColor(RGB(0, 150, 0));
			//dc.TextOut(rect.left+2, rect.top+1, Pass_String);
			dc.DrawText(Pass_String, rect,DT_VCENTER);
			break;

		case 3:
			dc.SetTextColor(RGB(255, 0, 0));
			//dc.TextOut(rect.left+2, rect.top+1, Fail_String);
			dc.DrawText(Fail_String, rect,DT_VCENTER);
			break;

		case 4:
			dc.SetTextColor(RGB(0, 0, 255));
			//dc.TextOut(rect.left+2, rect.top+1,"     TEST RECORD RESET");
			dc.DrawText("TEST RECORD RESET", rect,DT_CENTER|DT_VCENTER);
			break;
	}

	// Detach from the CDC object, otherwise the hDC will be
	// destroyed when the CDC object goes out of scope
	dc.Detach();
}

void CColoredStatusBarCtrlCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char temp[50];
	ZeroMemory(temp,sizeof(temp));
	RECT StatusBar_Rect;
	this->GetRect(4,&StatusBar_Rect);//Reset Test Record item
    if((point.x>=StatusBar_Rect.left)&&
	   (point.x<=StatusBar_Rect.right)&&
	   (point.y>=StatusBar_Rect.top)&&
	   (point.y<=StatusBar_Rect.bottom))
	{
		if((!Start_Flag)&&TotalNo)//program is in idle and TotalNo!=0
		{
			if((::AfxMessageBox("WARING:You are trying to reset the test record to zero, click YES to reset, or click NO not to reset!!",MB_YESNO|MB_ICONINFORMATION)==IDYES))
			{
				TotalNo=0;
				PassNo=0;
				FailNo=0;
				_stprintf(Total_String,_T("TEST TOTAL:  %d"),TotalNo);
				sprintf(Pass_String,"TEST PASS:  %d",PassNo);
				sprintf(Fail_String,"TEST FAIL:  %d",FailNo);
				sprintf(temp,"%d",TotalNo);
				::WritePrivateProfileString("TEST_RECORD","TOTAL",temp,Ini_File_Path);

				sprintf(temp,"%d",PassNo);
				::WritePrivateProfileString("TEST_RECORD","PASS",temp,Ini_File_Path);

				sprintf(temp,"%d",FailNo);
				::WritePrivateProfileString("TEST_RECORD","FAIL",temp,Ini_File_Path);

				this->Invalidate();  
				this->UpdateWindow();
			}

		}

	}
	CStatusBarCtrl::OnLButtonDblClk(nFlags, point);
}
