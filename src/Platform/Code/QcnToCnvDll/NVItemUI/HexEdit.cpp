#include "stdafx.h"
#include "HexEdit.h"
#include "NVItemDataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char gcHexTable[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
	m_pData			= NULL;

	m_length		= 0;
	m_topindex		= 0;
	m_bpr			= 8;		// byte per row
	m_lpp			= 1;

	m_bShowHex		= TRUE;
	m_bShowAscii	= TRUE;
	m_bShowAddress	= TRUE;
	// I change it to short address (2bytes -> 65535. It's enough)
	m_bAddressIsWide= FALSE;		// 4/8 byte address

	m_offAddress	= 0;
	m_offHex		= 0;
	m_offAscii		= 0;

	m_bUpdate = TRUE;			// update font info
	m_bNoAddressChange = FALSE;
	m_currentMode = EDIT_NONE;

	m_editPos.x = m_editPos.y = 0;
	m_currentAddress = 0;
	m_bHalfPage = TRUE;

	m_selStart	= NO_SELECTED;
	m_selEnd	= NO_SELECTED;
	m_bDirty = FALSE;

	m_Font.CreateFont(-12, 0,0,0,0,0,0,0,0,0,0,0,0, _T("Courier New"));

	pOwnerView = NULL;
	// Initializes the OLE DLLs. (??)
	if(!gbInit)
	{
		//		AfxOleInit();
		gbInit = true;
	}
	m_isEdit = true;
}

CHexEdit::~CHexEdit()
{
	if(m_pData != NULL)
		delete [] m_pData;
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers
CPoint CHexEdit::CalcPos(int x, int y)
{
	int xp;

	y /= m_lineHeight;
	if(y < 0 || y > m_lpp)
		return CPoint(-1, -1);

	if(y * m_bpr > m_length)
		return CPoint(-1, -1);

	x += m_nullWidth;
	x /= m_nullWidth;



	if(m_bShowAddress && x <= (m_bAddressIsWide ? 8 : 4))
	{
		m_currentAddress = m_topindex + (m_bpr * y);
		m_currentMode = EDIT_NONE;
		return CPoint(0, y);
	}

	xp = (m_offHex  / m_nullWidth) + m_bpr * 3;
	if(m_bShowHex && x < xp)
	{
		/* In current design, m_bAddressIsWide is alwasy false,
		Thus I make these codes as comments. This modification
		is to correct inproper caret location in short address mode.
		if(m_bAddressIsWide)
		{
		if(x%3)
		x--;
		}
		else  */
		{
			if((x+1)%3)
				x--;
		}
		m_currentAddress = m_topindex + (m_bpr * y) + (x - (m_offHex  / m_nullWidth)) / 3;
		m_currentMode =  ((x%3) & 0x01) ? EDIT_LOW : EDIT_HIGH;
		return CPoint(x, y);
	}

	xp = (m_offAscii  / m_nullWidth) + m_bpr;
	if(m_bShowAscii && x <= xp)
	{
		m_currentAddress = m_topindex + (m_bpr * y) + (x - (m_offAscii  / m_nullWidth));
		m_currentMode = EDIT_ASCII;
		return CPoint(x, y);
	}

	return CPoint(-1,-1);
}
// Virtual function
BOOL CHexEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRet;

	dwStyle |= WS_VSCROLL;
	bRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if(bRet)
		SetFont(&m_Font);
	return bRet;
}

void CHexEdit::CreateAddressCaret()
{
	//		The DestroyCaret() function destroys the caret's current shape, frees the
	//	caret from the window, and removes the caret from the screen. If the caret
	//	shape is based on a bitmap, DestroyCaret does not free the bitmap.
	//		A window should create a caret only when it has the keyboard focus
	//	or is active. The window should destroy the caret before losing the keyboard
	//	focus or becoming inactive.
	DestroyCaret();
	//		Creates a solid rectangle for the system caret and claims ownership of
	//	the caret. The caret shape can be a line or block.
	//		The system caret is a shared resource. CWnd should create a caret
	//	only when it has the input focus or is active. It should destroy the
	//	caret before it loses the input focus or becomes inactive.
	CreateSolidCaret(m_nullWidth * (m_bAddressIsWide ? 8 : 4), m_lineHeight);
}

void CHexEdit::CreateEditCaret()
{
	// Help: Refre to CreateAddressCaret()
	DestroyCaret();
	CreateSolidCaret(m_nullWidth, m_lineHeight);
}

int CHexEdit::GetData(LPBYTE pByte, int iLen)
{
	memcpy(pByte, m_pData, min(iLen, m_length));
	return m_length;
}

CSize CHexEdit::GetSel()
{
	return CSize(m_selStart, m_selEnd);
}

inline BOOL CHexEdit::IsSelected()
{
	return m_selStart != NO_SELECTED;
}

void CHexEdit::Move(int x, int y)
{
	switch(m_currentMode)
	{
	case EDIT_NONE:
		return;
	case EDIT_HIGH:
		if(x != 0)
			m_currentMode = EDIT_LOW;
		if(x == -1)
			m_currentAddress --;
		m_currentAddress += y* m_bpr;
		break;
	case EDIT_LOW:
		if(x != 0)
			m_currentMode = EDIT_HIGH;
		if(x == 1)
			m_currentAddress++;
		m_currentAddress += y* m_bpr;
		break;
	case EDIT_ASCII:
		{
			m_currentAddress += x;
			m_currentAddress += y*m_bpr;
		}
		break;
	}
	if(m_currentAddress < 0)
		m_currentAddress = 0;

	if(m_currentAddress >= m_length)
	{
		m_currentAddress -= x;
		m_currentAddress -= y*m_bpr;
	}
	m_bNoAddressChange = TRUE;
	if(m_currentAddress < m_topindex)
	{
		OnVScroll(SB_LINEUP, 0, NULL);
	}
	if(m_currentAddress >= m_topindex + m_lpp*m_bpr)
	{
		OnVScroll(SB_LINEDOWN, 0, NULL);
	}
	m_bNoAddressChange = FALSE;
	//ScrollIntoView(m_currentAddress);
	RepositionCaret(m_currentAddress);
}

void CHexEdit::NormalizeSel(void)
{
	if(m_selStart > m_selEnd)
		m_selStart ^= m_selEnd ^= m_selStart ^= m_selEnd;
}

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_isEdit)
		return;
	nFlags;nRepCnt;
	if(!m_pData)
		return;
	if(nChar == '\t')
		return;
	/*	if(GetKeyState(VK_CONTROL) & 0x80000000)
	{
	switch(nChar)
	{
	case 0x03:
	if(IsSelected())
	OnEditCopy();
	return;
	case 0x16:
	OnEditPaste();
	return;
	case 0x18:
	if(IsSelected())
	OnEditCut();
	return;
	case 0x1a:
	OnEditUndo();
	return;
	}
	} */

	if(nChar == 0x08)
	{
		if(m_currentAddress > 0)
		{
			m_currentAddress--;
			// I just want to go back 1 byte and don't want to delete current byte
			//SelDelete(m_currentAddress, m_currentAddress+1);
			RepositionCaret(m_currentAddress);
			RedrawWindow();
		}
		return;
	}

	SetSel(-1, -1);
	switch(m_currentMode)
	{
	case EDIT_NONE:
		return;
	case EDIT_HIGH:
	case EDIT_LOW:
		if((nChar >= '0' && nChar <= '9') || (nChar >= 'a' && nChar <= 'f'))
		{
			UINT b = nChar - '0';
			if(b > 9)
				b = 10 + nChar - 'a';

			if(m_currentMode == EDIT_HIGH)
			{
				m_pData[m_currentAddress] = (unsigned char)((m_pData[m_currentAddress] & 0x0f) | (b << 4));
			}
			else
			{
				m_pData[m_currentAddress] = (unsigned char)((m_pData[m_currentAddress] & 0xf0) | b);
			}
			Move(1,0);
			// It means memory contents are changed!
			m_bDirty = TRUE;
		}
		break;
	case EDIT_ASCII:
		m_pData[m_currentAddress] = (unsigned char)nChar;
		Move(1,0);
		m_bDirty = TRUE;
		break;
	}
	RedrawWindow();


	//	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CHexEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return CEdit::OnEraseBkgnd(pDC);
	pDC;

	return TRUE;
}

UINT CHexEdit::OnGetDlgCode()
{
	// TODO: Add your message handler code here and/or call default
	//return CEdit::OnGetDlgCode();
	return DLGC_WANTALLKEYS;
}

void CHexEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	nFlags;	nRepCnt;

	BOOL bShift = GetKeyState(VK_SHIFT) & 0x80000000;
	BOOL bac = m_bNoAddressChange;
	m_bNoAddressChange = TRUE;
	switch(nChar)
	{
	case VK_DOWN:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			Move(0,1);
			m_selEnd   = m_currentAddress;
			if(m_currentMode == EDIT_HIGH || m_currentMode == EDIT_LOW)
				m_selEnd++;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		Move(0,1);
		break;
	case VK_UP:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			Move(0,-1);
			m_selEnd   = m_currentAddress;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		Move(0,-1);
		break;
	case VK_LEFT:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			Move(-1,0);
			m_selEnd   = m_currentAddress;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		Move(-1,0);
		break;
	case VK_RIGHT:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			Move(1,0);
			m_selEnd   = m_currentAddress;
			if(m_currentMode == EDIT_HIGH || m_currentMode == EDIT_LOW)
				m_selEnd++;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		Move(1,0);
		break;
	case VK_PRIOR:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			OnVScroll(SB_PAGEUP, 0, NULL);
			Move(0,0);
			m_selEnd   = m_currentAddress;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		OnVScroll(SB_PAGEUP, 0, NULL);
		Move(0,0);
		break;
	case VK_NEXT:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			OnVScroll(SB_PAGEDOWN, 0, NULL);
			Move(0,0);
			m_selEnd   = m_currentAddress;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		OnVScroll(SB_PAGEDOWN, 0, NULL);
		Move(0,0);
		break;
	case VK_HOME:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			if(GetKeyState(VK_CONTROL) & 0x80000000)
			{
				OnVScroll(SB_THUMBTRACK, 0, NULL);
				Move(0,0);
			}
			else
			{
				m_currentAddress /= m_bpr;
				m_currentAddress *= m_bpr;
				Move(0,0);
			}
			m_selEnd   = m_currentAddress;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		if(GetKeyState(VK_CONTROL) & 0x80000000)
		{
			OnVScroll(SB_THUMBTRACK, 0, NULL);
			m_currentAddress = 0;
			Move(0,0);
		}
		else
		{
			m_currentAddress /= m_bpr;
			m_currentAddress *= m_bpr;
			Move(0,0);
		}
		break;
	case VK_END:
		if(bShift)
		{
			if(!IsSelected())
			{
				m_selStart = m_currentAddress;
			}
			if(GetKeyState(VK_CONTROL) & 0x80000000)
			{
				m_currentAddress = m_length-1;
				OnVScroll(SB_THUMBTRACK, ((m_length+(m_bpr/2)) / m_bpr) - m_lpp, NULL);
				Move(0,0);
			}
			else
			{
				m_currentAddress /= m_bpr;
				m_currentAddress *= m_bpr;
				m_currentAddress += m_bpr - 1;
				if(m_currentAddress > m_length)
					m_currentAddress = m_length-1;
				Move(0,0);
			}
			m_selEnd   = m_currentAddress;
			RedrawWindow();
			break;
		}
		else
			SetSel(-1, -1);
		if(GetKeyState(VK_CONTROL) & 0x80000000)
		{
			m_currentAddress = m_length-1;
			if(m_bHalfPage)
				OnVScroll(SB_THUMBTRACK, 0, NULL);
			else
				OnVScroll(SB_THUMBTRACK, ((m_length+(m_bpr/2)) / m_bpr) - m_lpp, NULL);
			Move(0,0);
		}
		else
		{
			m_currentAddress /= m_bpr;
			m_currentAddress *= m_bpr;
			m_currentAddress += m_bpr - 1;
			if(m_currentAddress > m_length)
				m_currentAddress = m_length-1;
			Move(0,0);
		}
		break;
	case VK_INSERT:
		//SelInsert(m_currentAddress, max(1, m_selEnd-m_selStart));
		//RedrawWindow();
		break;
	case VK_DELETE:
		/*if(IsSelected())
		{
		OnEditClear();
		}
		else
		{
		SelDelete(m_currentAddress, m_currentAddress+1);
		RedrawWindow();
		}*/
		break;
	case '\t':
		switch(m_currentMode)
		{
		case EDIT_NONE:
			m_currentMode = EDIT_HIGH;
			break;
		case EDIT_HIGH:
		case EDIT_LOW:
			m_currentMode = EDIT_ASCII;
			break;
		case EDIT_ASCII:
			m_currentMode = EDIT_HIGH;
			break;
		}
		Move(0,0);
		break;

	}
	m_bNoAddressChange = bac;
}

void CHexEdit::OnKillFocus(CWnd* pNewWnd)
{
	//CEdit::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here

	if(m_bDirty&&pOwnerView !=NULL)
	{
		((CNVItemDataView*)pOwnerView)->SaveDataChage(m_itemId,m_pData);
	}
	DestroyCaret();
	CWnd::OnKillFocus(pNewWnd);
}

void CHexEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnLButtonDown(nFlags, point);
	SetFocus();
	if(!m_pData)
		return;

	if(nFlags & MK_SHIFT)
	{
		m_selStart = m_currentAddress;
	}
	CPoint pt = CalcPos(point.x, point.y);
	if(pt.x > -1)
	{
		m_editPos = pt;
		pt.x *= m_nullWidth;
		pt.y *= m_lineHeight;

		if(pt.x == 0 && m_bShowAddress)
			CreateAddressCaret();
		else
			CreateEditCaret();

		SetCaretPos(pt);
		if(nFlags & MK_SHIFT)
		{
			m_selEnd = m_currentAddress;
			if(m_currentMode == EDIT_HIGH || m_currentMode == EDIT_LOW)
				m_selEnd++;
			RedrawWindow();
		}
	}
	if(!(nFlags & MK_SHIFT))
	{
		if(DragDetect(point))
		{
			m_selStart = m_currentAddress;
			m_selEnd   = m_selStart;
			SetCapture();
		}
		else
		{
			BOOL bsel = m_selStart != 0xffffffff;

			m_selStart = 0xffffffff;
			m_selEnd   = 0xffffffff;
			if(bsel)
				RedrawWindow();
		}
	}
	if(!IsSelected())
	{
		ShowCaret();
	}

}

void CHexEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnLButtonUp(nFlags, point);
	if(IsSelected())
		ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CHexEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnMouseMove(nFlags, point);
	if(!m_pData)
		return;

	if(nFlags & MK_LBUTTON && m_selStart != NO_SELECTED)
	{
		CRect rc;
		GetClientRect(&rc);
		if(!rc.PtInRect(point))
		{
			if(point.y < 0)
			{
				OnVScroll(SB_LINEUP, 0, NULL);
				point.y = 0;
			}
			else if(point.y > rc.Height())
			{
				OnVScroll(SB_LINEDOWN, 0, NULL);
				point.y = rc.Height() -1;
			}
		}

		//
		// we are selecting
		//
		int	 seo = m_selEnd;
		CPoint pt = CalcPos(point.x, point.y);
		if(pt.x > -1)
		{
			m_selEnd = m_currentAddress;
			if(m_currentMode == EDIT_HIGH || m_currentMode == EDIT_LOW)
				m_selEnd++;
		}
		if(IsSelected())
			DestroyCaret();

		if(seo != m_selEnd)
			RedrawWindow();
	}

}

void CHexEdit::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CEdit::OnNcPaint() for painting messages
	CEdit::OnNcPaint();
	HideCaret();
}

void CHexEdit::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CEdit::OnPaint() for painting messages
	CPaintDC	pdc(this); // device context for painting
	CDC			dc;
	CRect		rc;
	CBitmap		bm;
	CBrush		b;
	int			height		= 0;
	int			x,y;
	char		buf[256];


	GetClientRect(rc);
	dc.CreateCompatibleDC(CDC::FromHandle(pdc.m_ps.hdc));

	bm.CreateCompatibleBitmap(CDC::FromHandle(pdc.m_ps.hdc), rc.Width(), rc.Height());
	dc.SelectObject(bm);

	// Fill the Hex edit client area with "white" color
	b.CreateSolidBrush(RGB(0xff,0xff,0xff));
	dc.FillRect(rc, &b);

	ASSERT(m_currentAddress >= 0);
	ASSERT(m_topindex >= 0);

	dc.SelectObject(m_Font);

	x = rc.TopLeft().x;
	y = rc.TopLeft().y;

	dc.SetBoundsRect(&rc, DCB_DISABLE);

	if(m_pData)
	{
		//
		// get char dimensions
		//
		if(m_bUpdate)
		{
			dc.GetCharWidth('0', '0', &m_nullWidth);
			CSize sz = dc.GetTextExtent(_T("0"), 1);
			m_lineHeight = sz.cy;

			m_offHex	= m_bShowAddress ? (m_bAddressIsWide ? m_nullWidth * 9 : m_nullWidth * 5) : 0;
			m_offAscii	= m_bShowAddress ? (m_bAddressIsWide ? m_nullWidth * 9 : m_nullWidth * 5) : 0;
			m_offAscii += m_bShowHex	 ? (m_bpr * 3 * m_nullWidth) : 0;

			m_lpp = rc.Height() / m_lineHeight;
			m_bHalfPage = FALSE;
			if(m_lpp * m_bpr > m_length)
			{
				m_lpp = (m_length + (m_bpr/2)) / m_bpr ;
				if(m_length % m_bpr != 0)
				{
					m_bHalfPage = TRUE;
					m_lpp++;
				}
			}
			m_bUpdate = FALSE;
			UpdateScrollbars();
		}

		TRACE(_T("%i %i\n"), m_topindex, m_selStart);

		height = rc.Height() / m_lineHeight;
		height *= m_lineHeight;

		if(m_bShowAddress)
		{
			char fmt[8] = {'%','0','8','l','X'};
			fmt[2] = m_bAddressIsWide ? '8' : '4';
			int w = m_bAddressIsWide ? 8 : 4;
			y = 0;
			CRect rcd = rc;
			rcd.TopLeft().x = m_offAddress;
			for(int	 i = m_topindex; (i < m_length) && (rcd.TopLeft().y < height); i+= m_bpr)
			{
				sprintf(buf, fmt, i);
				dc.DrawText(buf, w, rcd, DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
				rcd.TopLeft().y += m_lineHeight;
			}
		}
		if(m_bShowHex)
		{
			y = 0;
			CRect rcd = rc;
			rcd.TopLeft().x = x = m_offHex;

			if(m_selStart != 0xffffffff && (m_currentMode == EDIT_HIGH || m_currentMode == EDIT_LOW))
			{
				int	 i;
				int	 n = 0;
				int	 selStart = m_selStart, selEnd = m_selEnd;
				if(selStart > selEnd)
					selStart ^= selEnd ^= selStart ^= selEnd;

				for(i = m_topindex; (i < selStart) && (y < height); i++)
				{
					char* p = &buf[0];
					TOHEX(m_pData[i], p);
					*p++ = ' ';
					dc.TextOut(x, y, buf, 3);
					x += m_nullWidth * 3;
					n++;
					if(n == m_bpr)
					{
						n = 0;
						x = m_offHex;
						y += m_lineHeight;
					}
				}
				dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
				dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
				for(; (i < selEnd) && (i < m_length) && (y < height); i++)
				{
					char* p = &buf[0];
					TOHEX(m_pData[i], p);
					*p++ = ' ';
					dc.TextOut(x, y, buf, 3);
					x += m_nullWidth * 3;
					n++;
					if(n == m_bpr)
					{
						n = 0;
						x = m_offHex;
						y += m_lineHeight;
					}
				}
				//				dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
				//				dc.SetBkColor(GetSysColor(COLOR_WINDOW));
				dc.SetTextColor(RGB(64,96,64));
				dc.SetBkColor(RGB(255,255,255));

				for(; (i < m_length) && (y < height); i++)
				{
					char* p = &buf[0];
					TOHEX(m_pData[i], p);
					*p++ = ' ';
					dc.TextOut(x, y, buf, 3);
					x += m_nullWidth * 3;
					n++;
					if(n == m_bpr)
					{
						n = 0;
						x = m_offHex;
						y += m_lineHeight;
					}
				}
			}
			else
			{
				for(int	 i = m_topindex; (i < m_length) && (rcd.TopLeft().y < height);)
				{
					char* p = &buf[0];
					int n;
					for(n = 0; (n < m_bpr) && (i < m_length); n++)
					{
						TOHEX(m_pData[i], p);
						*p++ = ' ';
						i++;
					}
					while(n < m_bpr)
					{
						*p++ = ' ';	*p++ = ' ';	*p++ = ' ';
						n++;
					}

					dc.DrawText(buf, m_bpr*3, rcd, DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
					rcd.TopLeft().y += m_lineHeight;
				}
			}
		}
		if(m_bShowAscii)
		{
			y = 0;
			CRect rcd = rc;
			rcd.TopLeft().x = x = m_offAscii;
			if(m_selStart != 0xffffffff && m_currentMode == EDIT_ASCII)
			{
				int	 i;
				int	 n = 0;
				int	 selStart = m_selStart, selEnd = m_selEnd;
				if(selStart > selEnd)
					selStart ^= selEnd ^= selStart ^= selEnd;

				for(i = m_topindex; (i < selStart) && (y < height); i++)
				{
					buf[0] = isprint(m_pData[i]) ? m_pData[i] : '.';
					dc.TextOut(x, y, buf, 1);
					x += m_nullWidth;
					n++;
					if(n == m_bpr)
					{
						n = 0;
						x = m_offAscii;
						y += m_lineHeight;
					}
				}
				dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
				dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
				for(; (i < selEnd) && (y < height); i++)
				{
					buf[0] = isprint(m_pData[i]) ? m_pData[i] : '.';
					dc.TextOut(x, y, buf, 1);
					x += m_nullWidth;
					n++;
					if(n == m_bpr)
					{
						n = 0;
						x = m_offAscii;
						y += m_lineHeight;
					}
				}
				dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
				dc.SetBkColor(GetSysColor(COLOR_WINDOW));
				for(; (i < m_length) && y < height; i++)
				{
					buf[0] = isprint(m_pData[i]) ? m_pData[i] : '.';
					dc.TextOut(x, y, buf, 1);
					x += m_nullWidth;
					n++;
					if(n == m_bpr)
					{
						n = 0;
						x = m_offAscii;
						y += m_lineHeight;
					}
				}
			}
			else
			{
				for(int	 i = m_topindex; (i < m_length) && (rcd.TopLeft().y < height);)
				{
					char* p = &buf[0];
					int	 n;
					for(n = 0; (n < m_bpr) && (i < m_length); n++)
					{
						*p++ = isprint(m_pData[i]) ? m_pData[i] : '.';
						i++;
					}
					dc.DrawText(buf, n, rcd, DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
					rcd.TopLeft().y += m_lineHeight;
				}
			}
		}
	}
	pdc.BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);


}

void CHexEdit::OnSetFocus(CWnd* pOldWnd)
{
	//CEdit::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	if(m_pData && !IsSelected())
	{
		m_bDirty = FALSE;
		if(m_editPos.x == 0 && m_bShowAddress)
			CreateAddressCaret();
		else
			CreateEditCaret();
		SetCaretPos(m_editPos);
		ShowCaret();
	}
	CWnd::OnSetFocus(pOldWnd);
}

void CHexEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

}

void CHexEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
	int oa = m_topindex;
	pScrollBar;

	if(!m_pData)
		return;
	switch(nSBCode)
	{
	case SB_LINEDOWN:
		if(m_topindex < m_length - m_lpp*m_bpr)
		{
			//ScrollWindow(0, -m_lineHeight, rc);
			m_topindex += m_bpr;
			RedrawWindow();
		}
		break;

	case SB_LINEUP:
		if(m_topindex > 0)
		{
			//ScrollWindow(0, m_lineHeight, rc);
			m_topindex -= m_bpr;
			RedrawWindow();
		}
		break;

	case SB_PAGEDOWN:
		if(m_topindex < m_length - m_lpp*m_bpr)
		{
			m_topindex += m_bpr * m_lpp;
			if(m_topindex > m_length - m_lpp*m_bpr)
				m_topindex = m_length - m_lpp*m_bpr;
			RedrawWindow();
		}
		break;

	case SB_PAGEUP:
		if(m_topindex > 0)
		{
			m_topindex -= m_bpr * m_lpp;
			if(m_topindex < 0)
				m_topindex = 0;
			RedrawWindow();
		}
		break;

	case SB_THUMBTRACK:
		m_topindex = nPos * m_bpr;
		RedrawWindow();
		break;
	}
	::SetScrollPos(this->m_hWnd, SB_VERT, m_topindex / m_bpr, TRUE);
	if(!m_bNoAddressChange)
		m_currentAddress += (m_topindex - oa);
	RepositionCaret(m_currentAddress);
}

BOOL CHexEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	//return CEdit::PreCreateWindow(cs);
	cs.style |= WS_VSCROLL;
	return CEdit::PreCreateWindow(cs);
}

void CHexEdit::RepositionCaret(int p)
{
	int x, y;

	y = (p - m_topindex) / m_bpr;
	x = (p - m_topindex) % m_bpr;

	switch(m_currentMode)
	{
	case EDIT_NONE:
		CreateAddressCaret();
		x = 0;
		break;
	case EDIT_HIGH:
		CreateEditCaret();
		x *= m_nullWidth*3;
		x += m_offHex;
		break;
	case EDIT_LOW:
		CreateEditCaret();
		x *= m_nullWidth*3;
		x += m_nullWidth;
		x += m_offHex;
		break;
	case EDIT_ASCII:
		CreateEditCaret();
		x *= m_nullWidth;
		x += m_offAscii;
		break;
	}
	m_editPos.x = x;
	m_editPos.y = y*m_lineHeight;
	CRect rc;
	GetClientRect(&rc);
	if(rc.PtInRect(m_editPos))
	{
		SetCaretPos(m_editPos);
		ShowCaret();
	}
}

void CHexEdit::ScrollIntoView(int p)
{
	if(p < m_topindex || p > m_topindex + m_lpp*m_bpr)
	{
		m_topindex = (p/m_bpr) * m_bpr;
		m_topindex -= (m_lpp / 3) * m_bpr;
		if(m_topindex < 0)
			m_topindex = 0;

		UpdateScrollbars();
		RedrawWindow();
	}
}

void CHexEdit::SetBPR(int bpr)
{
	m_bpr = bpr;
	m_bUpdate		= TRUE;
}

void CHexEdit::SetData(int itemId,LPBYTE p, int len)
{
	//if(m_bDirty && pOwnerView !=NULL)
	//{
	//	((CNVItemDataView*)pOwnerView)->SaveDataChage(m_itemId,m_pData);
	//}
	SetWindowText(_T(""));
	if(p!=NULL)
	{
		delete [] m_pData;
		m_pData = (LPBYTE) new BYTE[len];
		memcpy(m_pData, p, len);
		//SetSel(-1, -1);
		m_itemId = itemId;
	}
	m_length = len;
	m_currentAddress = 0;
	m_editPos.x = m_editPos.y = 0;
	m_currentMode = EDIT_HIGH;
	m_topindex = 0;
	m_bUpdate = TRUE;
	OnPaint();
}

void CHexEdit::SetOptions(BOOL bAddress, BOOL bHex, BOOL bAscii, BOOL bWide)
{
	m_bShowHex		= bHex;
	m_bShowAscii	= bAscii;
	m_bShowAddress	= bAddress;
	//m_bAddressIsWide= bWide;		// 4/8 byte address
	m_bAddressIsWide= FALSE;		// default to short
	m_bUpdate		= TRUE;
}

void CHexEdit::SetSel(int s, int e)
{
	DestroyCaret();
	m_selStart = s;
	m_selEnd = e;
	RedrawWindow();
	if(m_editPos.x == 0 && m_bShowAddress)
		CreateAddressCaret();
	else
		CreateEditCaret();
	SetCaretPos(m_editPos);
	ShowCaret();
}

void CHexEdit::UpdateScrollbars()
{
	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = (m_length / m_bpr) - 1;
	si.nPage = m_lpp;
	si.nPos = m_topindex / m_bpr;

	// Change by Cipher
	::SetScrollRange(this->m_hWnd,SB_VERT,si.nMin,si.nMax,TRUE);
	// Changed by Cipher

	::SetScrollInfo(this->m_hWnd, SB_VERT, &si, TRUE);
	if(si.nMax > (int)si.nPage)
		::EnableScrollBar(this->m_hWnd, SB_VERT, ESB_ENABLE_BOTH);

	CRect rc;
	GetClientRect(&rc);

	si.nMin = 0;
	si.nMax =((m_bShowAddress ? (m_bAddressIsWide ? 8 : 4) : 0) +
		(m_bShowHex ? m_bpr * 3 : 0) +
		(m_bShowAscii ? m_bpr : 0) ) * m_nullWidth;
	si.nPage = 1;
	si.nPos = 0;
	// Changed By cipher
	::ShowScrollBar(this->m_hWnd,SB_HORZ,FALSE);
	// End Changed By cipher

	//::SetScrollInfo(this->m_hWnd, SB_HORZ, &si, TRUE);
}

