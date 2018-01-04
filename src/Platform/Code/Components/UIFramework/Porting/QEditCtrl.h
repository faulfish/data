
#if !defined(AFX_CQEditCtrl_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_)
#define AFX_CQEditCtrl_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CQEditCtrl.h : header file
//
#include "../CommUIExportAPI.h"
/////////////////////////////////////////////////////////////////////////////
// CQEditCtrl window

typedef bool (*pFnCheckWords)(LPVOID);
typedef UINT (*pFnGetLength)(LPVOID);

namespace QSYNC_LIB {

class COMMONUI_API CQEditCtrl : public CEdit
{
public:
    CQEditCtrl();
    virtual ~CQEditCtrl();

public:
    // Color control here
    void SetTextColor(COLORREF rgb);
    void SetBackColor(COLORREF rgb);
    void SetTextColor(BYTE r, BYTE g, BYTE b);
    void SetBackColor(BYTE r, BYTE g, BYTE b);
    // Font Control here
    void Setfont(TCHAR* szFontName);
    // set the checking words
    // "0123456789", "A-Z|a-z", "<>?:"{}"
    void SetCheckingWords(TCHAR* szWords, UINT nWordCount);
    // if the bInByte is true, check the length in byte(no matter what type it is)
    // if the bInByte is false, check the length in word(one TCHAR)
    void SetTextLength(UINT nLength);
    // Alignment Control here

private:
    bool CheckWords();
    bool CheckWords(TCHAR* szCheckingWords);
    UINT GetLengthInByte();
    bool CheckWords(pFnCheckWords pFn, LPVOID lpParam);
    UINT GetLengthInByte(pFnGetLength pFn, LPVOID lpParam);

    // Generated message map functions
protected:
    // text color
    COLORREF m_crText;
    // background color
    COLORREF m_crBackGnd;
    //background brush
    CBrush m_brBackGnd;
    CFont* m_pFont;
    TCHAR* m_szCheckingWords;
    UINT m_nCheckingWordsLength;
    UINT m_nLimitLength;
	CString m_strOrgText;

protected:
DECLARE_MESSAGE_MAP()
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
public:
	afx_msg void OnEnChange();
};

}
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CQEditCtrl_H__8B87D5E4_AB1C_469A_AE48_733BDD5B390C__INCLUDED_)
