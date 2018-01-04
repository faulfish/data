#if !defined(AFX_PASSWORD_H__D1175E80_9989_4AB3_9855_7305920E707F__INCLUDED_)
#define AFX_PASSWORD_H__D1175E80_9989_4AB3_9855_7305920E707F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PASSWORD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPASSWORD dialog

class CPASSWORD : public CDialog
{
// Construction
public:
	CPASSWORD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPASSWORD)
	enum { IDD = IDD_PASSWORD };
	CEdit	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPASSWORD)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

private:
	CStringA m_strSOPW;
protected:

	// Generated message map functions
	//{{AFX_MSG(CPASSWORD)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORD_H__D1175E80_9989_4AB3_9855_7305920E707F__INCLUDED_)
