
#include "stdafx.h"
#include "QEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQEditCtrl

using namespace QSYNC_LIB;

CQEditCtrl::CQEditCtrl():
m_crText(RGB(0, 0, 0)),
m_crBackGnd(RGB(255, 255, 55)),
m_pFont(NULL),
m_szCheckingWords(NULL),
m_nCheckingWordsLength(0),
m_nLimitLength(0)
{
}

CQEditCtrl::~CQEditCtrl() {
    //delete brush
    if(m_brBackGnd.GetSafeHandle())
        m_brBackGnd.DeleteObject();
    if(m_pFont) 
		delete m_pFont;
    if(m_szCheckingWords) 
		delete [] m_szCheckingWords;
}


BEGIN_MESSAGE_MAP(CQEditCtrl, CEdit)
    //{{AFX_MSG_MAP(CReadOnlyEdit)
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
    ON_CONTROL_REFLECT(EN_CHANGE, &CQEditCtrl::OnEnChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQEditCtrl message handlers

HBRUSH CQEditCtrl::CtlColor(CDC* pDC, UINT nCtlColor)  {
    // TODO: Return a non-NULL brush if the parent's handler should not be called

    //set text color
    pDC->SetTextColor(m_crText);
    //set the text's background color
    pDC->SetBkColor(m_crBackGnd);

    //return the brush used for background this sets control background
    return m_brBackGnd;
}

void CQEditCtrl::SetTextColor(COLORREF rgb) {
    //set text color ref
    m_crText = rgb;

    //redraw
    Invalidate(TRUE);
}

void CQEditCtrl::SetBackColor(COLORREF rgb) {
    //set background color ref (used for text's background)
    m_crBackGnd = rgb;

    //free brush
    if(m_brBackGnd.GetSafeHandle())
        m_brBackGnd.DeleteObject();
    //set brush to new color
    m_brBackGnd.CreateSolidBrush(rgb);

    //redraw
    Invalidate(TRUE);
}

void CQEditCtrl::SetTextColor(BYTE r, BYTE g, BYTE b) {
    SetTextColor(RGB(r, g, b));
}

void CQEditCtrl::SetBackColor(BYTE r, BYTE g, BYTE b) {
    SetBackColor(RGB(r, g, b));
}

void CQEditCtrl::Setfont(TCHAR* szFontName) { 
    if(m_pFont) delete m_pFont;
    m_pFont = new CFont; 
    m_pFont->CreateFont(20, // nHeight 
        0,                  // nWidth 
        0,                  // nEscapement 
        0,                  // nOrientation 
        FW_BOLD,            // nWeight 
        TRUE,               // bItalic 
        FALSE,              // bUnderline 
        0,                  // cStrikeOut 
        DEFAULT_CHARSET,    // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS,// nClipPrecision 
        DEFAULT_QUALITY,    // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        szFontName);        // lpszFac 
    __super::SetFont(m_pFont);
}
void QSYNC_LIB::CQEditCtrl::OnEnChange()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CEdit::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    if(!CheckWords()) 
	{
		SetWindowText(m_strOrgText);
	}
	GetWindowText(m_strOrgText);
}

void QSYNC_LIB::CQEditCtrl::SetCheckingWords(TCHAR* szWords, UINT nWordCount) {
    if(szWords && nWordCount != 0) {
        m_szCheckingWords = new TCHAR[nWordCount+1];
        memset(m_szCheckingWords, 0, sizeof(TCHAR)*(nWordCount+1));
		memcpy(m_szCheckingWords, szWords, sizeof(TCHAR)*(nWordCount));
        //_tcscpy_s(m_szCheckingWords, nWordCount, szWords);
        m_nCheckingWordsLength = nWordCount;
    }
}

void QSYNC_LIB::CQEditCtrl::SetTextLength(UINT nLength) {
    m_nLimitLength = nLength;
    SetLimitText(nLength);
}

bool QSYNC_LIB::CQEditCtrl::CheckWords() {
    bool bRes = true;
	CString strText;
	GetWindowText(strText);
	if(m_nCheckingWordsLength == 0 || strText.IsEmpty()) return bRes;
	strText.Trim(m_szCheckingWords);
    bRes = strText.IsEmpty();
	return bRes;
}

bool QSYNC_LIB::CQEditCtrl::CheckWords(TCHAR* szCheckingWords) {
	bool bRes = true;
	CString strText;
	GetWindowText(strText);
	if(_tcslen(szCheckingWords) == 0 || strText.IsEmpty()) return bRes;
	strText.Trim(szCheckingWords);
	bRes = strText.IsEmpty();
	return bRes;
}

bool QSYNC_LIB::CQEditCtrl::CheckWords(pFnCheckWords pFn, LPVOID lpParam) {
    return pFn(lpParam);
}

UINT QSYNC_LIB::CQEditCtrl::GetLengthInByte() {
    CString strText;
    GetWindowText(strText);
    return (UINT)strText.GetLength() * (UINT)sizeof(TCHAR);
}

UINT QSYNC_LIB::CQEditCtrl::GetLengthInByte(pFnGetLength pFn, LPVOID lpParam) {
    return pFn(lpParam);
}