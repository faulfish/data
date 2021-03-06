/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
*
*               BenQ QSyncher Project
*
*	   Copyright (C) 2004-2006 BenQ Corp.
*
***************************************************************************
*
*  $Workfile:   SyncStatic.cpp  $
*  $Revision:   1.1  $
*  $Date:   Jan 17 2008 15:04:26  $
*
***************************************************************************
*
* File Description
* ----------------
* 
***************************************************************************
*
* Revision Details
* ----------------
* 
***************************************************************************/
/*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/UIFramework/Porting/SyncStatic.cpp.-arc  $
   
      Rev 1.1   Jan 17 2008 15:04:26   Eddy Guo
   remove warning
   
      Rev 1.0   Sep 12 2007 09:34:18   Alan Tu
   Initial revision.

Rev 1.0   Oct 05 2006 15:46:08   Neko Hsieh
Initial revision.
* 
*/
/**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */

//###########################################################################//
//  "CSyncStatic" Window Control                                             //
//  - a member of CSyncXXX Series Steven-generated Window Controls -         //
//                                                                           //
//  Author      :  Steven CC Chen <Steven.CC.Chen@BenQ.com>                  //
//  Release Date:  2005/09/13                                                //
//  Revision    :  2006/10/05                                                //
//  Copyright(C) 2005-2006  All rights reserved.                             //
//###########################################################################//

// SyncStatic.cpp : implementation file
//


#include	"stdafx.h"
#include	"SyncStatic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CSyncStatic

CSyncStatic::CSyncStatic(CString szFontName		/*=""*/,
						 int nLogicFontSize		/*=9*/,
						 LONG nFontWeight		/*=FW_NORMAL*/,
						 BOOL bIfItalic			/*=FALSE*/,
						 BOOL bIfUnderline		/*=FALSE*/,
						 BOOL bIf3D				/*=FALSE*/,
						 COLORREF TextFaceColor	/*=RGB(0,0,0)*/,
						 COLORREF TextShadowColor /*::GetSysColor(COLOR_3DSHADOW)*/,
						 BOOL bIfTransparentBk	/*=TRUE*/)
{
	EnableAutomation();

	//------------------------- Steven-generated codes -------------------------
	if( !szFontName.IsEmpty() )
		this->m_szFontName = szFontName;
	else;

	this->m_nLogicFontSize = nLogicFontSize;
	this->m_nFontWeight = nFontWeight,
		this->m_bIfItalic = bIfItalic,
		this->m_bIfUnderline = bIfUnderline,
		this->m_bIf3D = bIf3D;
	this->m_TextFaceColor = TextFaceColor;
	this->m_TextShadowColor = TextShadowColor;
	this->m_bIfTransparentBk = bIfTransparentBk;
	//------------------------- Steven-generated codes -------------------------
}


//(*ps. If the following codes doesnt work when the control is generated by drag-and-drop in VC (not by the Create method),
//      you should modify the style of this control via the ResourceView manually instead.)
BOOL CSyncStatic::PreCreateWindow(CREATESTRUCT& cs)
{
	//------------------------- Steven-generated codes -------------------------
	if( this->m_bIfTransparentBk )
		cs.dwExStyle |= WS_EX_TRANSPARENT;
	else
		cs.dwExStyle &= ~WS_EX_TRANSPARENT;


	return CStatic::PreCreateWindow(cs);
	//------------------------- Steven-generated codes -------------------------
}


CSyncStatic::~CSyncStatic()
{
	;
}


void CSyncStatic::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.
	CStatic::OnFinalRelease();
}


void CSyncStatic::set_FontName(CString szFontName)
{
	//------------------------- Steven-generated codes -------------------------
	if( !szFontName.IsEmpty() )
	{
		this->m_szFontName = szFontName;
	}else;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_FontSize(int nLogicFontSize)
{
	//------------------------- Steven-generated codes -------------------------
	this->m_nLogicFontSize = nLogicFontSize;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_FontWeight(LONG nFontWeight)
{
	//------------------------- Steven-generated codes -------------------------
	this->m_nFontWeight = nFontWeight;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_Italic(BOOL bIfItalic)
{
	//------------------------- Steven-generated codes -------------------------
	this->m_bIfItalic = bIfItalic;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_Underline(BOOL bIfUnderline)
{
	//------------------------- Steven-generated codes -------------------------
	this->m_bIfUnderline = bIfUnderline;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_3DText(BOOL bIf3D, COLORREF TextShadowColor /*::GetSysColor(COLOR_3DSHADOW)*/)
{
	//------------------------- Steven-generated codes -------------------------
	if( bIf3D )
		this->m_TextShadowColor = TextShadowColor;
	else;

	this->m_bIf3D = bIf3D;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_TextFaceColor(COLORREF TextFaceColor)
{
	//------------------------- Steven-generated codes -------------------------
	this->m_TextFaceColor = TextFaceColor;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::set_TransparentBk(BOOL bIfTransparentBk)
{
	//------------------------- Steven-generated codes -------------------------
	this->m_bIfTransparentBk = bIfTransparentBk;
	//------------------------- Steven-generated codes -------------------------
}


void CSyncStatic::RedrawText()
{
	//------------------------- Steven-generated codes -------------------------
	this->RedrawWindow(NULL, NULL, RDW_UPDATENOW);
	//------------------------- Steven-generated codes -------------------------
}


BEGIN_MESSAGE_MAP(CSyncStatic, CStatic)
	//{{AFX_MSG_MAP(CSyncStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSyncStatic, CStatic)
	//{{AFX_DISPATCH_MAP(CSyncStatic)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ISyncStatic to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {EAE42CD9-E783-49EF-B311-0D091E3E86D5}
static const IID IID_ISyncStatic =
{ 0xeae42cd9, 0xe783, 0x49ef, { 0xb3, 0x11, 0xd, 0x9, 0x1e, 0x3e, 0x86, 0xd5 } };

BEGIN_INTERFACE_MAP(CSyncStatic, CStatic)
	INTERFACE_PART(CSyncStatic, IID_ISyncStatic, Dispatch)
END_INTERFACE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSyncStatic message handlers

void CSyncStatic::OnPaint()
{
	//------------------------- Steven-generated codes -------------------------
	CPaintDC dc(this);
	CRect rect;
	::GetClientRect(this->m_hWnd, &rect);
	CString szItemText;
	this->GetWindowText(szItemText);



	LOGFONT lfFont;
	ZeroMemory(&lfFont, sizeof(LOGFONT));
	this->GetFont()->GetLogFont(&lfFont);
	lfFont.lfWeight		= this->m_nFontWeight;
	lfFont.lfItalic		= this->m_bIfItalic;
	lfFont.lfUnderline	= this->m_bIfUnderline;
	lfFont.lfStrikeOut	= FALSE;
	lfFont.lfCharSet	= DEFAULT_CHARSET;
	if( !this->m_szFontName.IsEmpty() )
	{
		lstrcpy(lfFont.lfFaceName, this->m_szFontName);
	}else;

	double nCaps = ::GetDeviceCaps(dc.m_hDC,LOGPIXELSY);
	double nHeight = nCaps * (m_nLogicFontSize*10) / 72;
	lfFont.lfHeight = (long)(fabs(nHeight) / 10.0 + 0.5);
	lfFont.lfHeight *= -1;

	CFont * pNewFont = new CFont();
	pNewFont->CreateFontIndirect(&lfFont);
	HFONT hOldFont = (HFONT) ::SelectObject(dc.m_hDC, pNewFont->GetSafeHandle());


	::SetBkMode(dc.m_hDC, TRANSPARENT);

	if( this->m_bIf3D )
	{
		::SetTextColor(dc.m_hDC, this->m_TextShadowColor);
		CRect rectTextShadow;
		rectTextShadow.left		= rect.left + 1;
		rectTextShadow.top		= rect.top + 1;
		rectTextShadow.right	= rect.right + 1;
		rectTextShadow.bottom	= rect.bottom + 1;
		::DrawText(dc.m_hDC, szItemText, -1, &rectTextShadow, DT_EDITCONTROL | DT_WORDBREAK | DT_LEFT | DT_NOPREFIX);

		::SetTextColor(dc.m_hDC, this->m_TextFaceColor);
		CRect rectTextFace;
		rectTextFace.left		= rect.left;
		rectTextFace.top		= rect.top;
		rectTextFace.right		= rect.right;
		rectTextFace.bottom		= rect.bottom;
		::DrawText(dc.m_hDC, (LPCTSTR)szItemText, -1, &rectTextFace, DT_EDITCONTROL | DT_WORDBREAK | DT_LEFT | DT_NOPREFIX);
	}
	else
	{
		::SetTextColor(dc.m_hDC, this->m_TextFaceColor);
		::DrawText(dc.m_hDC, (LPCTSTR)szItemText, -1, &rect, DT_EDITCONTROL | DT_WORDBREAK | DT_LEFT | DT_NOPREFIX);
	}


	::SelectObject(dc.m_hDC, hOldFont);
	pNewFont->DeleteObject();
	delete pNewFont;
	//------------------------- Steven-generated codes -------------------------

	// Do not call CStatic::OnPaint() for painting messages
}



