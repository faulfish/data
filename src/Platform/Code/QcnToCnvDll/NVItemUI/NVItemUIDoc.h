// NVItemUIDoc.h : interface of the CNVItemUIDoc class
//


#pragma once
#include "NVItemTreeView.h"
//#include "NVArray.h"

#include "..\..\QcnToCnvDll\QcnToCnvDll\QcnToCnvDll.h"


// CSetNVRangeDlg dialog

class CSetNVRangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetNVRangeDlg)

public:
	CSetNVRangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetNVRangeDlg();

	// Dialog Data
	enum { IDD = IDD_NV_RANGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nMin;
	int m_nMax;
	int GetMinData(void);
	int GetMaxData(void);
	void SetMinData(int nMin);
	void SetMaxData(int nMax);
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};

// CSaveDialog dialog

class CSaveDialog : public CDialog
{
	DECLARE_DYNAMIC(CSaveDialog)

public:
	CSaveDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveDialog();

	// Dialog Data
	enum { IDD = IDD_SAVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveAs();
	afx_msg void OnBnClickedNoSave();
};
//----------------------------------------------------------

class CNVItemUIDoc : public CDocument
{
protected: // create from serialization only
	CNVItemUIDoc();
	DECLARE_DYNCREATE(CNVItemUIDoc)

// Attributes
public:

// Operations
private:
	HANDLE	m_hOperator;
	BYTE*	m_NVItemContent;
	CString m_NVItemHex;
	int		m_NVItemID;
	int		m_NVItemLen;
	CString m_FilePath;
	CString m_FileName;
	CString m_ConName;
	int		m_FileType;
	BOOL    m_IsModify;
	CString m_comPort;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CNVItemUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:

	CNVItemTreeView* GetNVItemTreeView(void); // Get pointer to Tree View with all NV Items
	//CNVItemDataList* GetNVItemListView(void); // Get pointer to file info View 
	//CFileTreeView* GetExcInfoView(void); // Get pointer to excute info View 	
	void		GetNVArray(CString filePath);
	void		NVItemDataToNVTree(NVArray & m_NVDataBase);
	void		VecNVItemDataToNVTree(std::vector<CNVItemData>& NVItemArray);
	////////////
	void		GetNVItem(int mNVItemId);
	void		EditViewFunc(CNVItemData & mNVitem);
	int			GetNVItemLen();
	BYTE*       GetNVItemData();
	void        ShowExeMeg(const char* szInfo,int flag);
	void		SaveDataChange(int itemId,BYTE* itemContent);
	void		SortAscNVItemArray();
	void		SortDesNVItemArray();
	bool        SetCom();
	int			GetMoblieNVItemByID();
	CNVItemData*GetMobileNVItem();

	static void FillTreeCallBackFunc(LPVOID m_doc,NVArray & m_NVDataBase);
	static void VecTreeCallBackFunc(LPVOID m_doc,std::vector<CNVItemData>& NVItemArray);
	static void EditCallBackFunc(LPVOID m_doc,CNVItemData & mNVitem);
	static void ListCallBackFunc(LPVOID m_doc,const char* szInfo,int flag);
protected:
	virtual BOOL SaveModified();
	BOOL		 SaveAs();
public:
	void         IsEdit(bool IsModify);
	afx_msg void OnOpenNvfile();
	afx_msg void OnConvertToBrt();
	afx_msg void OnConvertToCnv();
	afx_msg void OnConvertToQcn();
	afx_msg void OnUpdateConvertToBrt(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertToCnv(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertToQcn(CCmdUI *pCmdUI);
	afx_msg void OnSetNVRange();
	afx_msg void OnReadAllNV();
	afx_msg void OnUpdateSetNVRange(CCmdUI *pCmdUI);
	afx_msg void OnUpdateReadAllNV(CCmdUI* pCmdUI);
private:
	bool m_bIsConToBrtOpen;
	bool m_bIsConToCnvOpen;
	bool m_bIsConToQcnOpen;
public:
	int m_nNVRangeMin;
	int m_nNVRangeMax;
};


