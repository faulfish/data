#pragma once

#include "CTabCtrlSSL/TabPageSSL.h"
#include "../MobileInterface/Interface_Include/ICommMobile.h"
#include "afxwin.h"

// CIEFSTestPage dialog

class CIEFSTestPage : public CTabPageSSL 
{
	DECLARE_DYNAMIC(CIEFSTestPage)

public:
	CIEFSTestPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIEFSTestPage();

private:
	class CIEFSNotify : public QSYNC_LIB::INotify
	{
	  private:
          CIEFSTestPage *m_outter;
	  public:
		  CIEFSNotify( CIEFSTestPage *outter): m_outter(outter){}
	  ////////////////////////////////////////////////////////////////
	  //INotify
	  virtual void Event(const TSTRING& strEvent,long nParam)
	  { 
		  m_outter->OnIEFSEventNotify( strEvent, nParam);
	  }
	};

	CIEFSNotify*  m_pIEFSNotify;
    
	void OnIEFSEventNotify(const TSTRING& strEvent,long nParam);

// Dialog Data
	enum { IDD = IDD_IEFS_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNewInterface();
public:
	afx_msg void OnBnClickedExecFunction();
public:
	int m_nIEFS_Function;

private:
	IEFS* m_pIEFS;
	afx_msg void OnBnClickedDeleteInterface();
	CEdit m_Result_Edit;
	CString m_Param1;
	CString m_Param2;
	CString m_Param3;
	CString m_Param4;
	CListBox m_ResList;
};
