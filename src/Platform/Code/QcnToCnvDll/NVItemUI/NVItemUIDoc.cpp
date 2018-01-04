// NVItemUIDoc.cpp : implementation of the CNVItemUIDoc class
//

#include "stdafx.h"
#include "NVItemUI.h"
#include "NVItemUIDoc.h"
#include "ChildFrm.h"
#include "NVItemListView.h"
#include "NVItemDataView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// SetNVRangeDlg.cpp : implementation file
//

// CSetNVRangeDlg dialog

IMPLEMENT_DYNAMIC(CSetNVRangeDlg, CDialog)

CSetNVRangeDlg::CSetNVRangeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSetNVRangeDlg::IDD, pParent)
, m_nMin(0)
, m_nMax(8000)
{

}

CSetNVRangeDlg::~CSetNVRangeDlg()
{
}

void CSetNVRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetNVRangeDlg, CDialog)
END_MESSAGE_MAP()


// CSetNVRangeDlg message handlers

int CSetNVRangeDlg::GetMinData(void)
{
	return m_nMin;
}

int CSetNVRangeDlg::GetMaxData(void)
{
	return m_nMax;
}

void CSetNVRangeDlg::SetMinData(int nMin)
{
	m_nMin = nMin;
}

void CSetNVRangeDlg::SetMaxData(int nMax)
{
	m_nMax = nMax;
}

BOOL CSetNVRangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->UpdateData(FALSE);
	SetDlgItemInt(IDC_EDIT_MIN, m_nMin);
	SetDlgItemInt(IDC_EDIT_MAX, m_nMax);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetNVRangeDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	this->UpdateData(TRUE);
	m_nMin = GetDlgItemInt(IDC_EDIT_MIN);
	m_nMax = GetDlgItemInt(IDC_EDIT_MAX);


	CDialog::OnOK();
}


// SaveDialog.cpp : implementation file
//

// CSaveDialog dialog

IMPLEMENT_DYNAMIC(CSaveDialog, CDialog)

CSaveDialog::CSaveDialog(CWnd* pParent /*=NULL*/)
: CDialog(CSaveDialog::IDD, pParent)
{

}

CSaveDialog::~CSaveDialog()
{
}

void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
	ON_BN_CLICKED(ID_SAVE_AS, &CSaveDialog::OnBnClickedSaveAs)
	ON_BN_CLICKED(ID_NO_SAVE, &CSaveDialog::OnBnClickedNoSave)
END_MESSAGE_MAP()


// CSaveDialog message handlers

void CSaveDialog::OnBnClickedSaveAs()
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(ID_SAVE_AS); 
}

void CSaveDialog::OnBnClickedNoSave()
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(ID_NO_SAVE); 
}
//-----------------------------------------------

// CNVItemUIDoc

IMPLEMENT_DYNCREATE(CNVItemUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CNVItemUIDoc, CDocument)
	ON_COMMAND(ID_CONVERT_CONVERTTOBRT, &CNVItemUIDoc::OnConvertToBrt)
	ON_COMMAND(ID_CONVERT_CONVERTTOCNV, &CNVItemUIDoc::OnConvertToCnv)
	ON_COMMAND(ID_CONVERT_CONVERTTOQCN, &CNVItemUIDoc::OnConvertToQcn)
	ON_COMMAND(ID_READ_ALL_NV, &CNVItemUIDoc::OnReadAllNV)
	ON_COMMAND(ID_SET_NV_RANGE, &CNVItemUIDoc::OnSetNVRange)
	ON_COMMAND(ID_OPEN_NVFILE, &CNVItemUIDoc::OnOpenNvfile)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_CONVERTTOBRT, &CNVItemUIDoc::OnUpdateConvertToBrt)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_CONVERTTOCNV, &CNVItemUIDoc::OnUpdateConvertToCnv)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_CONVERTTOQCN, &CNVItemUIDoc::OnUpdateConvertToQcn)
	ON_UPDATE_COMMAND_UI(ID_READ_ALL_NV, &CNVItemUIDoc::OnUpdateReadAllNV)
	ON_UPDATE_COMMAND_UI(ID_SET_NV_RANGE, &CNVItemUIDoc::OnUpdateSetNVRange)
END_MESSAGE_MAP()


// CNVItemUIDoc construction/destruction

CNVItemUIDoc::CNVItemUIDoc()
: m_bIsConToBrtOpen(false)
, m_bIsConToCnvOpen(false)
, m_bIsConToQcnOpen(false)
, m_hOperator(NULL)
, m_nNVRangeMin(0)
, m_nNVRangeMax(9000)
{
	// TODO: add one-time construction code here
	m_IsModify = false;
}

CNVItemUIDoc::~CNVItemUIDoc()
{
	if(m_hOperator!=NULL)
		CloseOperatorHandle(m_hOperator);
}

BOOL CNVItemUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CNVItemUIDoc serialization

void CNVItemUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CNVItemUIDoc diagnostics

#ifdef _DEBUG
void CNVItemUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNVItemUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNVItemUIDoc commands


//////////////////TreeView///////////////////////
CNVItemTreeView*  CNVItemUIDoc::GetNVItemTreeView(void)
{
	POSITION firstView = this->GetFirstViewPosition();
	CNVItemTreeView *view = (CNVItemTreeView*)this->GetNextView(firstView);

	CChildFrame *frame  = (CChildFrame*) view->GetParentFrame();

	CNVItemTreeView *NVTree  = (CNVItemTreeView*) frame->m_wndSplitter1.GetPane(0,0);

	return NVTree;
}




void CNVItemUIDoc::GetNVArray(CString filePath)
{
	ASSERT(m_hOperator == NULL);
	m_hOperator = GetOperatorHandle();
	m_FilePath = filePath;
	CString filetype = filePath.Mid(filePath.ReverseFind(_T('.'))+1);
	if(filetype ==_T("brt") || filetype == _T("BRT"))
	{
		m_FileType = BRT;
		m_bIsConToCnvOpen = TRUE;
		m_bIsConToQcnOpen = TRUE;
	}else if(filetype ==_T("cnv") || filetype == _T("CNV"))
	{
		m_FileType = CNV;
		m_bIsConToBrtOpen = TRUE;
		m_bIsConToQcnOpen = TRUE;
	}else if(filetype ==_T("du") || filetype == _T("DU"))
	{
		m_FileType = DU;
		IsEdit(false);
	}else if(filetype ==_T("qcn") || filetype == _T("QCN"))
	{
		m_FileType = QCN;
		m_bIsConToBrtOpen = TRUE;
		m_bIsConToCnvOpen = TRUE;
	}
	SetTitle(m_FilePath);
	m_ConName = filePath.Right(filePath.GetLength()-filePath.ReverseFind('\\')-1);
	m_FileName = m_ConName.Left(m_ConName.ReverseFind('.'));

	SetCallBackFuncs(m_hOperator,this, &CNVItemUIDoc::FillTreeCallBackFunc,&CNVItemUIDoc::VecTreeCallBackFunc,&CNVItemUIDoc::EditCallBackFunc,&CNVItemUIDoc::ListCallBackFunc);
	FileToNVArray(m_hOperator,m_FilePath,m_FileType);

}

void CNVItemUIDoc::FillTreeCallBackFunc(LPVOID m_doc,NVArray & m_NVDataBase)
{

	CNVItemUIDoc * pDoc = (CNVItemUIDoc*) m_doc;
	pDoc->NVItemDataToNVTree(m_NVDataBase);
}

void CNVItemUIDoc::NVItemDataToNVTree(NVArray & m_NVDataBase)
{

	CString ItenID,index;
	HTREEITEM NVGroupPos;
	CNVItemTreeView *TreeAll = GetNVItemTreeView();
	TreeAll->GetTreeCtrl().DeleteAllItems();
	int NbOfTreeElements = m_NVDataBase.GetCount();
	NVGroupPos = TreeAll->GetTreeCtrl().InsertItem(_T("NVItems"),0,1);
	for(int i=0;i<NbOfTreeElements;i++)
	{ 
		index.Format(_T("%d"),i);
		ItenID.Format(_T("%d"),m_NVDataBase[i].GetItemID());
		ItenID = _T("(Id=")+ItenID;
		ItenID = ItenID +_T( ")");
		ItenID = index +ItenID;
		ItenID =_T("index :")+ItenID;
		TreeAll->GetTreeCtrl().InsertItem(ItenID,1,2,NVGroupPos);
	}
	GetNVItemTreeView()->GetTreeCtrl().SortChildren(TVI_ROOT);
}

void CNVItemUIDoc::VecTreeCallBackFunc(LPVOID m_doc,std::vector<CNVItemData>& NVItemArray)
{

	CNVItemUIDoc * pDoc = (CNVItemUIDoc*) m_doc;
	pDoc->VecNVItemDataToNVTree(NVItemArray);
}

void CNVItemUIDoc::VecNVItemDataToNVTree(std::vector<CNVItemData>& NVItemArray)
{

	CString ItenID,index;
	HTREEITEM NVGroupPos;
	CNVItemTreeView *TreeAll = GetNVItemTreeView();
	TreeAll->GetTreeCtrl().DeleteAllItems();
	int NbOfTreeElements = NVItemArray.size();
	NVGroupPos = TreeAll->GetTreeCtrl().InsertItem(_T("NVItems"),0,1);
	for(int i=0;i<NbOfTreeElements;i++)
	{ 
		index.Format(_T("%d"),i);
		ItenID.Format(_T("%d"),NVItemArray[i].GetItemID());
		ItenID = _T("(Id=")+ItenID;
		ItenID = ItenID +_T( ")");
		ItenID = index +ItenID;
		ItenID =_T("index :")+ItenID;
		TreeAll->GetTreeCtrl().InsertItem(ItenID,1,2,NVGroupPos);
	}
	GetNVItemTreeView()->GetTreeCtrl().SortChildren(TVI_ROOT);
}

void CNVItemUIDoc::SortAscNVItemArray()
{
	SortAscByID(m_hOperator);
}
void CNVItemUIDoc::SortDesNVItemArray()
{
	SortDesByID(m_hOperator);
}
//////////////////EditView///////////////////////
void CNVItemUIDoc::GetNVItem(int mNVItemId)
{
	m_NVItemID = mNVItemId;
	CString strInfo = _T("");
	strInfo.Format(_T("Get NVItem Data (ID=%d)"),mNVItemId );
	//SaveNVItemObject(hResource);
	int bRes =	GetNVItemObject(m_hOperator,mNVItemId);
	if(bRes<0)
	{
		ShowExeMeg(strInfo+_T(" Error!"),bRes);
	}else
	{
		ShowExeMeg(strInfo+_T(" Success!"),bRes);
	}
}

void CNVItemUIDoc::EditCallBackFunc(LPVOID m_doc,CNVItemData & mNVitem)
{

	CNVItemUIDoc * pDoc = (CNVItemUIDoc*) m_doc;
	pDoc->EditViewFunc(mNVitem);
}

void CNVItemUIDoc::EditViewFunc(CNVItemData & mNVitem)
{
	m_NVItemContent = mNVitem.GetItemContent();
	//CString ItemData(m_NVItemContent);
	m_NVItemLen = mNVitem.GetNVContentLen();
}


int CNVItemUIDoc::GetNVItemLen()
{
	return m_NVItemLen;
}

BYTE * CNVItemUIDoc::GetNVItemData()
{
	return m_NVItemContent;
}

CNVItemData * CNVItemUIDoc::GetMobileNVItem()
{
	return GetMobCNVItem(m_hOperator);
}

void CNVItemUIDoc::SaveDataChange(int itemId,BYTE* itemContent)
{
	m_IsModify = TRUE;
	SaveNVItemObject(m_hOperator,itemId,itemContent);
}

//////////////////ListView///////////////////////

void CNVItemUIDoc::ListCallBackFunc(LPVOID m_doc,const char* szInfo,int flag)
{
	CNVItemUIDoc * pDoc = (CNVItemUIDoc*) m_doc;
	pDoc->ShowExeMeg(szInfo,flag);
}

void CNVItemUIDoc::ShowExeMeg(const char* szInfo,int flag)
{
	POSITION firstView = this->GetFirstViewPosition();
	CNVItemTreeView *view = (CNVItemTreeView*)this->GetNextView(firstView);
	CChildFrame *frame  = (CChildFrame*) view->GetParentFrame();
	CNVItemListView * pListView= (CNVItemListView*) frame->m_wndSplitter2.GetPane(1,0);
	pListView->ShowMesInfo(szInfo,flag);
}


BOOL CNVItemUIDoc::SaveModified()
{
	// TODO: Add your specialized code here and/or call the base class
	SetFocus(NULL);
	if(m_IsModify)
	{
		CSaveDialog saveDlg;
		//int i = saveDlg.DoModal();
		switch(saveDlg.DoModal())
		{
		case IDOK:
			SaveNVItemFileChange(m_hOperator,m_FilePath,m_FileType,m_FileType);
			SetModifiedFlag(FALSE); 
			return CDocument::SaveModified();
			break;
		case ID_SAVE_AS:
			if(SaveAs()==0)
			{
				SetModifiedFlag(FALSE); 
				return CDocument::SaveModified();
			}
			return FALSE;
			break;
		case ID_NO_SAVE:
			SetModifiedFlag(FALSE); 
			return CDocument::SaveModified();
			break;
		case IDCANCEL:
			return FALSE;
			break;
		}
	}
	return TRUE;
}

BOOL CNVItemUIDoc::SaveAs()
{
	CString szFilters;
	CString szType;
	switch(m_FileType)
	{
	case BRT:
		szFilters = _T("BRT (*.brt)|*.brt||");
		szType = _T(".brt");
		break;
	case CNV:
		szFilters =  _T("CNV (*.cnv)|*.cnv||");
		szType = _T(".qcn");
		break;
		//case DU:
		//	szFilters =  _T("DU (*.du)|*.du||");
		//	break;
	case QCN:
		szFilters =  _T("QCN (*.qcn)|*.qcn||");
		szType = _T(".qcn");
		break;
	}

	CFileDialog fileDlg (FALSE, NULL, NULL,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, NULL);
	if(fileDlg.DoModal()==IDOK)  
	{  
		CString filePath = fileDlg.GetPathName()+szType;
		return SaveNVItemFileChange(m_hOperator,filePath,m_FileType,m_FileType);
	}else return TRUE;
}

void CNVItemUIDoc::IsEdit(bool IsModify)
{
	POSITION firstView = this->GetFirstViewPosition();
	CNVItemTreeView *view = (CNVItemTreeView*)this->GetNextView(firstView);
	CNVItemDataView * pListView = (CNVItemDataView *)this->GetNextView(firstView);
	//CChildFrame *frame  = (CChildFrame*) view->GetParentFrame();
	//CNVItemDataView * pListView= (CNVItemDataView*) frame->m_wndSplitter2.GetPane(0,0);
	pListView->setIsEdit(IsModify);
}

void CNVItemUIDoc::OnConvertToBrt()
{
	// TODO: Add your command handler code here
	CFileDialog fileDlg (FALSE, NULL, m_FileName,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY,_T("BRT (*.brt)|*.brt||"), NULL);
	if(fileDlg.DoModal()==IDOK)  
	{  
		CString filePath = fileDlg.GetPathName()+_T(".brt");
		int bRes=SaveNVItemFileChange(m_hOperator,filePath,m_FileType,BRT);
		if(bRes<0)
		{
			ShowExeMeg(_T("Convert ")+m_ConName+_T(" to QCN File Error!"),bRes);
		}else
		{
			ShowExeMeg(_T("Convert ")+m_ConName+_T(" to QCN File Success!"),bRes);
			ShowExeMeg(_T("File Path:")+filePath,bRes);
		}
	}
}

void CNVItemUIDoc::OnConvertToCnv()
{
	// TODO: Add your command handler code here
	CFileDialog fileDlg (FALSE, NULL, m_FileName,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, _T("CNV (*.cnv)|*.cnv||"), NULL);
	if(fileDlg.DoModal()==IDOK)  
	{  
		CString filePath = fileDlg.GetPathName()+_T(".cnv");
		int bRes=SaveNVItemFileChange(m_hOperator,filePath,m_FileType,CNV);
		if(bRes<0)
		{
			ShowExeMeg(_T("Convert ")+m_ConName+_T(" to CNV File Error!"),bRes);
		}else
		{
			ShowExeMeg(_T("Convert ")+m_ConName+_T(" to CNV File Success!"),bRes);
			ShowExeMeg(_T("File Path:")+filePath,bRes);
		}
	}
}

void CNVItemUIDoc::OnConvertToQcn()
{
	// TODO: Add your command handler code here
	CFileDialog fileDlg (FALSE, NULL, m_FileName,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY,_T("QCN (*.qcn)|*.qcn||"), NULL);
	if(fileDlg.DoModal()==IDOK)  
	{  
		CString filePath = fileDlg.GetPathName()+_T(".qcn");
		int bRes = SaveNVItemFileChange(m_hOperator,filePath,m_FileType,QCN);
		if(bRes<0)
		{
			ShowExeMeg(_T("Convert ")+m_ConName+_T(" to QCN File Error!"),bRes);
		}else
		{
			ShowExeMeg(_T("Convert ")+m_ConName+_T(" to QCN File Success!"),bRes);
			ShowExeMeg(_T("File Path:")+filePath,bRes);
		}
	}
}

void CNVItemUIDoc::OnUpdateConvertToBrt(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConToBrtOpen);
}

void CNVItemUIDoc::OnUpdateConvertToCnv(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConToCnvOpen);
}

void CNVItemUIDoc::OnUpdateConvertToQcn(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsConToQcnOpen);
}


bool CNVItemUIDoc::SetCom()
{
	bool bRes = FALSE;
	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd(); 
	m_comPort = pFrame->getComPort();
	if(m_comPort!=_T(""))
		bRes = TRUE;
	return bRes;
}


int CNVItemUIDoc::GetMoblieNVItemByID()
{
	int bRes = 0;
	if(!SetCom())
	{
		//::MessageBox(_T("Please select ComPort!!!"));
		ShowExeMeg(_T("Please select ComPort!!!"),-1);
		return bRes;
	}
	bRes = GetMoblieNVItemData(m_hOperator,m_comPort.GetBuffer());
	return bRes;
}

void CNVItemUIDoc::OnReadAllNV()
{

	if(m_hOperator != NULL)
	{
		if(m_IsModify)
		{
			int nRes = AfxMessageBox(_T("The data has been modified, Do you want to save them?"),MB_YESNO);
			if(nRes == IDYES)
			{
				SaveModified();
			}
		}	
		CloseOperatorHandle(m_hOperator);
		m_hOperator = NULL;
	}

	if(!SetCom())
	{
		//::MessageBox(_T("Please select ComPort!!!"));
		ShowExeMeg(_T("Please select ComPort!!!"),-1);
		return ;
	}

	m_hOperator = GetOperatorHandle();
	SetCallBackFuncs(m_hOperator,this, &CNVItemUIDoc::FillTreeCallBackFunc,&CNVItemUIDoc::VecTreeCallBackFunc,&CNVItemUIDoc::EditCallBackFunc,&CNVItemUIDoc::ListCallBackFunc);

	int nRes = GetAllNVFromMobile(m_hOperator, m_comPort.GetBuffer(), m_nNVRangeMin, m_nNVRangeMax);

	m_FileType = RAWDATA;

	m_bIsConToBrtOpen = TRUE;
	m_bIsConToQcnOpen = TRUE;
	m_bIsConToCnvOpen = TRUE;
}

void CNVItemUIDoc::OnSetNVRange()
{
	CSetNVRangeDlg NVRangeDlg;
	NVRangeDlg.SetMaxData(m_nNVRangeMax);
	NVRangeDlg.SetMinData(m_nNVRangeMin);
	NVRangeDlg.DoModal();
	m_nNVRangeMin = NVRangeDlg.GetMinData();
	m_nNVRangeMax = NVRangeDlg.GetMaxData();


}
void CNVItemUIDoc::OnUpdateReadAllNV(CCmdUI *pCmdUI)
{

}
void CNVItemUIDoc::OnUpdateSetNVRange(CCmdUI *pCmdUI)
{
}


void CNVItemUIDoc::OnOpenNvfile()
{
	// TODO: Add your command handler code here

	if(m_hOperator != NULL)
	{
		if(m_IsModify)
		{
			int nRes = AfxMessageBox(_T("The data has been modified, Do you want to save them?"),MB_YESNO);
			if(nRes == IDYES)
			{
				SaveModified();
			}
		}
		CloseOperatorHandle(m_hOperator);
		m_hOperator = NULL;
	}

	TCHAR szFilters[]=  _T("Roaming FilesList (*.brt;*.cnv;*.du;*.qcn)|*.brt;*.cnv;*.du;*.qcn||");
	CFileDialog fileDlg (TRUE, NULL, NULL,OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, NULL);
	if(fileDlg.DoModal()==IDOK)  
	{   
		m_FilePath = fileDlg.GetPathName();
		GetNVArray(m_FilePath);
	}
}