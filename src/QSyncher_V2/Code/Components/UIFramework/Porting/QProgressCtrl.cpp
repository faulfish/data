/*********************************************************************/
/** @file:	QProgressCtrl.cpp
  
  Implements the CQProgressCtrl class.
  
  $Original Author:   Steven CC Chen  $
  $Version:	 $
  $Date:   Mar 05 2007 22:02:26  $
  */
/*********************************************************************/

/*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/UIFramework/Porting/QProgressCtrl.cpp.-arc  $
   
      Rev 1.0   Mar 05 2007 22:02:26   steven cc chen
   Initial revision.
 */

// QProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "QProgressCtrl.h"


namespace QSYNC_LIB
{

//////////////////////////////////////////////////////////////////////////////////////
// CQProgressCtrl

IMPLEMENT_DYNAMIC(CQProgressCtrl, CProgressCtrl)

CQProgressCtrl::CQProgressCtrl()
{
	// To initialize all data members:
	
	this->m_nMinPos					= QPROGRESSCTRL_DEFAULT_MIN_POS;
	this->m_nMaxPos					= QPROGRESSCTRL_DEFAULT_MAX_POS;
	this->m_strCaption				= QPROGRESSCTRL_DEFAULT_CAPTION;
	this->m_strCaptionPercentChar	= QPROGRESSCTRL_DEFAULT_CAPTION_PERCENT_CHAR;
	this->m_crTextColor				= QPROGRESSCTRL_DEFAULT_TEXT_COLOR;						//(COLORREF)::GetSysColor(COLOR_HIGHLIGHTTEXT);
	this->m_hFgBrush				= ::CreateSolidBrush(QPROGRESSCTRL_DEFAULT_FG_COLOR);	//::GetSysColorBrush(COLOR_HIGHLIGHT);
	this->m_hBgBrush				= ::CreateSolidBrush(QPROGRESSCTRL_DEFAULT_BG_COLOR);	//::GetSysColorBrush(COLOR_BACKGROUND);
}

CQProgressCtrl::~CQProgressCtrl()
{
	;
}



// CQProgressCtrl private functions

BOOL CQProgressCtrl::SetupMuiltLangLogFont(CDC* pDC)
{
	BOOL bRet = TRUE;
	
	
	ZeroMemory(&m_MultiLangLogFont, sizeof(LOGFONT));
	pDC->GetCurrentFont()->GetLogFont(&m_MultiLangLogFont);
	m_MultiLangLogFont.lfWeight		= FW_NORMAL;  //(FW_DONTCARE, FW_THIN, FW_NORMAL, FW_SEMIBOLD, FW_BOLD, FW_EXTRABOLD, FW_HEAVY.)
	m_MultiLangLogFont.lfItalic		= FALSE;
	m_MultiLangLogFont.lfUnderline	= FALSE;
	m_MultiLangLogFont.lfStrikeOut	= FALSE;
	
	switch( PRIMARYLANGID(LANGIDFROMLCID(::GetSystemDefaultLCID())) )  //(若要抓最原本的Winodws語言版本,請用GetUserDefaultLCID().)
	{
		case LANG_ENGLISH:
			{
				m_MultiLangLogFont.lfCharSet = ANSI_CHARSET;
				_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("Verdana"));
				m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (9*10) / 72) / 10.0 + 0.5) );  //(Verdana 9)
			}
			break;
		case LANG_CHINESE:
			{
				if(SUBLANG_CHINESE_TRADITIONAL == SUBLANGID(LANGIDFROMLCID(::GetSystemDefaultLCID())))
				{
					m_MultiLangLogFont.lfCharSet = CHINESEBIG5_CHARSET;
					_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("Arial"));
					m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (9*10) / 72) / 10.0 + 0.5) );  //(Arial 9)
				}
				else if(SUBLANG_CHINESE_SIMPLIFIED == SUBLANGID(LANGIDFROMLCID(::GetSystemDefaultLCID())))
				{
					m_MultiLangLogFont.lfCharSet = GB2312_CHARSET;
					_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("SimSun"));
					m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (10*10) / 72) / 10.0 + 0.5) );  //(宋体 10)
				}
				else
				{
					m_MultiLangLogFont.lfCharSet = DEFAULT_CHARSET;
					_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("MS Shell Dlg"));
					m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (9*10) / 72) / 10.0 + 0.5) );
				}
			}
			break;
		case LANG_JAPANESE:
			{
				m_MultiLangLogFont.lfCharSet = SHIFTJIS_CHARSET;
				_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("MS UI Gothic"));
				m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (10*10) / 72) / 10.0 + 0.5) );
			}
			break;
		case LANG_KOREAN:
			{
				m_MultiLangLogFont.lfCharSet = HANGUL_CHARSET;  //JOHAB_CHARSET
				_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("Gulim"));
				m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (10*10) / 72) / 10.0 + 0.5) );
			}
			break;
		case LANG_THAI:
			{
				m_MultiLangLogFont.lfCharSet = THAI_CHARSET;
				_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("Tahoma"));  //(*"DB Thaitext")
				m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (11*10) / 72) / 10.0 + 0.5) );
			}
			break;
		case LANG_ARABIC:
			{
				m_MultiLangLogFont.lfCharSet = ARABIC_CHARSET;
				_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("Tahoma"));
				m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (11*10) / 72) / 10.0 + 0.5) );
			}
			break;
		default:
			{
				m_MultiLangLogFont.lfCharSet = DEFAULT_CHARSET;
				_tcscpy_s(m_MultiLangLogFont.lfFaceName, _T("MS Shell Dlg"));
				m_MultiLangLogFont.lfHeight = -(int)(fabs( (float)(::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY) * (9*10) / 72) / 10.0 + 0.5) );
			}
			break;
	}
	
	return bRet;
}



// CQProgressCtrl public methods and operations

int CQProgressCtrl::GetMinPos()
{
	return this->m_nMinPos;
}


int CQProgressCtrl::GetMaxPos()
{
	return this->m_nMaxPos;
}


BOOL CQProgressCtrl::SetMinMaxPos(int nMinPos, int nMaxPos)
{
	BOOL bRet = FALSE;
	
	if( (nMinPos < nMaxPos) && (nMaxPos <= QPROGRESSCTRL_POS_MAX_VALUE) )
	{
		this->m_nMinPos = nMinPos;
		this->m_nMaxPos = nMaxPos;
		
		this->SetRange32(this->m_nMinPos, this->m_nMaxPos);
		
		bRet = TRUE;
	}else;
	
	return bRet;
}


CString CQProgressCtrl::GetCaption()
{
	return this->m_strCaption;
}


void CQProgressCtrl::SetCaption(LPCTSTR pszCaption)
{
	this->m_strCaption = CString(pszCaption);
}


CString CQProgressCtrl::GetCaptionPercentChar()
{
	return this->m_strCaptionPercentChar;
}


void CQProgressCtrl::SetCaptionPercentChar(LPCTSTR pszPercentChar)
{
	this->m_strCaptionPercentChar = CString(pszPercentChar);
}


COLORREF CQProgressCtrl::GetTextColor()
{
	return this->m_crTextColor;
}


void CQProgressCtrl::SetTextColor(COLORREF crTextColor)
{
	this->m_crTextColor = crTextColor;
}


HBRUSH CQProgressCtrl::GetFgBrush()
{
	return this->m_hFgBrush;
}


BOOL CQProgressCtrl::SetFgBrush(HBRUSH hFgBrush)
{
	BOOL bRet = FALSE;
	
	if(hFgBrush != NULL)
	{
		this->m_hFgBrush = hFgBrush;
		
		bRet = TRUE;
	}else;
	
	return bRet;
}


HBRUSH CQProgressCtrl::GetBgBrush()
{
	return this->m_hBgBrush;
}


BOOL CQProgressCtrl::SetBgBrush(HBRUSH hBgBrush)
{
	BOOL bRet = FALSE;
	
	if(hBgBrush != NULL)
	{
		this->m_hBgBrush = hBgBrush;
		
		bRet = TRUE;
	}else;
	
	return bRet;
}



// CQProgressCtrl override functions

///////////////////////////////////////////
////////////////// to-do //////////////////
///////////////////////////////////////////



// CQProgressCtrl message handlers

BEGIN_MESSAGE_MAP(CQProgressCtrl, CProgressCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


int CQProgressCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CProgressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Do the initialization:
	this->SetupMuiltLangLogFont(this->GetDC());
	
	return 0;
}


void CQProgressCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect, rectOriginal;
	BOOL bIsVerticalDraw = FALSE;
	int nCurrentPos = 0;
	int nOriginalLeft = 0;
	int nOriginalWidth = 0;
	int nOriginalBottom = 0;
	int nOriginalHeight = 0;
	CString szDrawText;
	COLORREF crClientEdgeColor = 0;
	COLORREF crTextShadowColor = 0;
	
	
	::GetClientRect(this->GetSafeHwnd(), &rect);
	rectOriginal.CopyRect(&rect);
	
	if(this->GetStyle() & PBS_VERTICAL)
		bIsVerticalDraw = TRUE;
	
	nCurrentPos = this->GetPos();
	nOriginalLeft = rectOriginal.left;
	nOriginalWidth = (rectOriginal.right - rectOriginal.left);
	nOriginalBottom = rectOriginal.bottom;
	nOriginalHeight = (rectOriginal.bottom - rectOriginal.top);
	
	///crClientEdgeColor = RGB(\
	///		abs(GET_COLOR_R_CHANNEL(QPROGRESSCTRL_DEFAULT_FG_COLOR) - GET_COLOR_R_CHANNEL(QPROGRESSCTRL_DEFAULT_BG_COLOR)),\
	///		abs(GET_COLOR_G_CHANNEL(QPROGRESSCTRL_DEFAULT_FG_COLOR) - GET_COLOR_G_CHANNEL(QPROGRESSCTRL_DEFAULT_BG_COLOR)),\
	///		abs(GET_COLOR_B_CHANNEL(QPROGRESSCTRL_DEFAULT_FG_COLOR) - GET_COLOR_B_CHANNEL(QPROGRESSCTRL_DEFAULT_BG_COLOR))\
	///		);
	crClientEdgeColor = RGB(\
		GET_COLOR_R_CHANNEL(QPROGRESSCTRL_DEFAULT_BG_COLOR) + 5,\
		GET_COLOR_G_CHANNEL(QPROGRESSCTRL_DEFAULT_BG_COLOR) + 5,\
		GET_COLOR_B_CHANNEL(QPROGRESSCTRL_DEFAULT_BG_COLOR) + 5\
		);
	crTextShadowColor = RGB(\
		GET_COLOR_R_CHANNEL(this->m_crTextColor) + 80,\
		GET_COLOR_G_CHANNEL(this->m_crTextColor) + 80,\
		GET_COLOR_B_CHANNEL(this->m_crTextColor) + 80\
		);
	
	dc.SetBkMode(TRANSPARENT);
	
	if( (nCurrentPos >= this->m_nMinPos) && (nCurrentPos <= this->m_nMaxPos) )
	{
		if(bIsVerticalDraw)
		{
			// To calculate the current progress top position:
			rect.top = nOriginalBottom - ((nOriginalHeight * nCurrentPos) / (this->m_nMaxPos - this->m_nMinPos));
			
			// To draw brush:
			dc.FillRect(&rectOriginal, CBrush::FromHandle(this->m_hBgBrush));
			dc.FillRect(&rect, CBrush::FromHandle(this->m_hFgBrush));
			
			// To draw edge:
			dc.FrameRect(&rectOriginal, CBrush::FromHandle(::CreateSolidBrush(crClientEdgeColor)));
			dc.DrawEdge(&rect, BDR_RAISEDINNER, BF_TOP);
		}
		else
		{
			// To calculate the current progress right position:
			rect.right = nOriginalLeft + ((nOriginalWidth * nCurrentPos) / (this->m_nMaxPos - this->m_nMinPos));
			
			// To draw brush:
			dc.FillRect(&rectOriginal, CBrush::FromHandle(this->m_hBgBrush));
			dc.FillRect(&rect, CBrush::FromHandle(this->m_hFgBrush));
			
			// To draw edge:
			dc.FrameRect(&rectOriginal, CBrush::FromHandle(::CreateSolidBrush(crClientEdgeColor)));
			dc.DrawEdge(&rect, BDR_RAISEDINNER, BF_RIGHT);
		}
		
		// To render the text for drawing:
		CString strPercent;
		strPercent.Format(_T("%d"), nCurrentPos);
		szDrawText = this->m_strCaption + strPercent + this->m_strCaptionPercentChar;
		
		// To draw text:
		CDC* pDC = CDC::FromHandle(dc.m_hDC);
		this->SetupMuiltLangLogFont(pDC);
		this->m_MultiLangLogFont.lfWeight = FW_NORMAL;
		CFont newFont;
		newFont.CreateFontIndirect(&(this->m_MultiLangLogFont));
		CFont* pOldFont = pDC->SelectObject(&newFont);
		//(text shadow)
		::SetTextColor(dc.m_hDC, crTextShadowColor);  //(light shadow)
		CRect rectTextShadow;
		rectTextShadow.left	= rectOriginal.left + 0;
		rectTextShadow.top	= rectOriginal.top + 1;
		rectTextShadow.right= rectOriginal.right + 0;
		rectTextShadow.bottom= rectOriginal.bottom + 1;
		dc.DrawText(szDrawText, &rectTextShadow, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_SINGLELINE);
		//(text face)
		::SetTextColor(dc.m_hDC, this->m_crTextColor);
		dc.DrawText(szDrawText, &rectOriginal, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_SINGLELINE);
		pDC->SelectObject(pOldFont);
		newFont.DeleteObject();
	}else;
	
	// Do not call CProgressCtrl::OnPaint() for painting messages
}

}