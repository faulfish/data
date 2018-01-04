// ColoredStatusBarCtrl.cpp: implementation of the CColoredStatusBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "ColoredStatusBarCtrl.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColoredStatusBarCtrl::CColoredStatusBarCtrl()
{

}

CColoredStatusBarCtrl::~CColoredStatusBarCtrl()
{

}

void CColoredStatusBarCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
			dc.SetTextColor(RGB(125, 0, 255));
			//dc.TextOut(rect.left+2, rect.top+1, "   TEST RECORD");
			dc.DrawText("TEST       RECORD", rect,DT_CENTER|DT_VCENTER);
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
			dc.SetTextColor(RGB(125, 0, 255));
			//dc.TextOut(rect.left+2, rect.top+1,"     TEST RECORD RESET");
			dc.DrawText("TEST  RECORD  RESET", rect,DT_CENTER|DT_VCENTER);
			break;
	}

	// Detach from the CDC object, otherwise the hDC will be
	// destroyed when the CDC object goes out of scope
	dc.Detach();
}
