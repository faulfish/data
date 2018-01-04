// SPU_UIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SPU_UI.h"
#include "SPU_UIDlg.h"
#include "../../FactoryTool/spu/SPUExport.h"
#include "../../FactoryTool/SPU_Loader/LoaderExport.h"
#include "../../CommonUtil\UnicodeUtility.h"
#include "../../CommonUtil\Win32SHELLUtility.h"
#include "../../CommonUtil\CommonDrawer.h"
#include "../../Components\ReportCenter\IReport.h"
#include <Gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
#include <cassert>
#include "ConfigBZ.h"

// Marcio 20080327
CHandlerBz *pCHandlerBz = (CHandlerBz *) new CHandlerBz;

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(const CString& strPrompt,CWnd* pParent /*=NULL*/)
: CDialog(CInputDlg::IDD, pParent)
,m_strPrompt(strPrompt)
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_EDIT, m_szInput);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDlg message handlers
BOOL CInputDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	GetDlgItem(IDC_PROMPT)->SetWindowText(m_strPrompt);

	return TRUE;
}

void CInputDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	OnOK();
}

void UI_CallbackClass::SetBreak()
{
	m_bBreak = true;
}
void UI_CallbackClass::ResetBreak()
{
	m_bBreak = false;
}

bool UI_CallbackClass::Print(const TSTRING& strName,const TSTRING& strOutput)
{
	if(strName == "COMPort")
	{
		if(m_pCSPU_UIDlg)
			m_pCSPU_UIDlg->SetCOMPort(strOutput.c_str());
	}	
	else if(strName == "DisplayPicture")
	{
		if(m_pCSPU_UIDlg)
			m_pCSPU_UIDlg->SetDisplayImage(strOutput.c_str());
	}
	else if(strName == "+CKEV")
	{
		if(m_pCSPU_UIDlg)
			m_pCSPU_UIDlg->AppendDisplayImage(strOutput.c_str());
	}
	else
	{
		if(m_pCSPU_UIDlg)
			m_pCSPU_UIDlg->ShowMessage(strOutput.c_str());
	}
	return true;
}

bool UI_CallbackClass::CheckBreak()
{
	return m_bBreak;
}

bool UI_CallbackClass::WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize)
{
	bool bRes = false;
	if(_T("ComfirmYesNo") == strName)
	{
		UINT nID = AfxMessageBox(szBuffer,MB_YESNO|MB_ICONQUESTION);
		if(nID == IDYES)
			_tcscpy_s(szBuffer,nBufferSize,_T("Yes"));
		else
			_tcscpy_s(szBuffer,nBufferSize,_T("No"));
		bRes = true;
	}
	else if(_T("COMPort") == strName)
	{
		CString strCOMPort;
		if(m_pCSPU_UIDlg)
			strCOMPort = m_pCSPU_UIDlg->GetPort(strName.c_str());
		_tcscpy_s(szBuffer,nBufferSize,strCOMPort);
		bRes = true;
	}
	else if(_T("Modemport") == strName)
	{
		CString strCOMPort;
		if(m_pCSPU_UIDlg)
			strCOMPort = m_pCSPU_UIDlg->GetPort(strName.c_str());
		_tcscpy_s(szBuffer,nBufferSize,strCOMPort);
		bRes = true;
	}
	else if(_T("BTADDR") == strName)
	{
		//Alan 20090331
		char szBTAddr[20] = {0};
		::GetPrivateProfileString("BT", "BTAddress", "", szBTAddr, 20, ".\\BTAddress.ini");
		_tcscpy_s(szBuffer,nBufferSize,szBTAddr);
		bRes = true;
	}
	else if(_T("BTPIN") == strName)
	{		
		//Alan 20090331
		char szPIN[20] = {0};
		::GetPrivateProfileString("BT", "BTPINcode", "", szPIN, 20, ".\\BTAddress.ini");
		_tcscpy_s(szBuffer,nBufferSize,szPIN);
		bRes = true;
	}
	else if(_T("MotorolaModel") == strName && !m_pCSPU_UIDlg->IsQisdaTaipei())
	{
		if(pCHandlerBz)
		{
			_tcscpy_s(szBuffer,nBufferSize,pCHandlerBz->GetMotorolaModel());
			bRes = true;
		}
	}
	else
	{
		CInputDlg InputDialog(strName.c_str());
		if(InputDialog.DoModal() == IDOK)
		{	
			if(static_cast<int>(QSYNC_LIB::GetUnicode((LPCTSTR)InputDialog.m_szInput).size()) <= nBufferSize)
			{
				_tcscpy_s(szBuffer,nBufferSize,InputDialog.m_szInput);
				bRes = true;
			}
		}
	}
	return bRes;
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSPU_UIDlg dialog
UINT ExecuteCommandThread(LPVOID param)
{
	CSPU_UIDlg *pDlg = (CSPU_UIDlg*)param;
	SPU_RESULT sResult = ExecuteCommand(pDlg->m_hSPULoader);

	pDlg->UpdateExecuteResult(sResult);
	pDlg->m_pExecuteThread = NULL;

	return 1;
}

CSPU_UIDlg::CSPU_UIDlg(LPCTSTR cScript)
: CDialog(CSPU_UIDlg::IDD, NULL)
, m_nTotal(0)
, m_nPass(0)
, m_nFail(0)
, m_unUIType(-1)
, m_pCallBack(NULL)
, m_szScript(cScript)
,m_hSPULoader(NULL)
,m_pExecuteThread(NULL)
,m_nTotalTime(0)
,m_pImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSPU_UIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOTAL, m_nTotal);
	DDX_Text(pDX, IDC_EDIT_PASS, m_nPass);
	DDX_Text(pDX, IDC_EDIT_FAIL, m_nFail);
	DDX_Control(pDX, IDC_LIST_NOTE, m_lbNote);
}

BEGIN_MESSAGE_MAP(CSPU_UIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND( )
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE( )
	ON_BN_CLICKED(IDOK, &CSPU_UIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSPU_UIDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_LOG_BUTTON, &CSPU_UIDlg::OnBnClickedLogButton)
	ON_CBN_SELCHANGE(IDC_MOTNAME, &CSPU_UIDlg::OnCbnSelchangeMotname)
	ON_CBN_SELCHANGE(IDC_COMBOMOTMODEL, &CSPU_UIDlg::OnCbnSelchangeCombomotmodel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSPU_UIDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSPU_UIDlg message handlers

BOOL CSPU_UIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Marcio 20080327
	int nStatus = -1;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Marcio 20090403
	if (this->IsQisdaTaipei()==false)
	{
		pCHandlerBz->SetFactoryID("BZ");
		pCHandlerBz->LoadBZConfig();
	
		CButton* pButtonStart = (CButton*) (this->GetDlgItem(IDOK));
		assert(pButtonStart && ::IsWindow(pButtonStart->GetSafeHwnd()));
		CButton* pButtonClear = (CButton*) (this->GetDlgItem(IDC_BUTTON_CLEAR));
		assert(pButtonClear && ::IsWindow(pButtonClear->GetSafeHwnd()));
		CButton* pButtonLog = (CButton*) (this->GetDlgItem(IDC_LOG_BUTTON));
		CComboBox* pMotName = (CComboBox*) (this->GetDlgItem(IDC_MOTNAME));
		CEdit* pEditMotName = (CEdit*) (this->GetDlgItem(IDC_EDIT2));
		CEdit* pEditVersion = (CEdit*) (this->GetDlgItem(IDC_EDIT_VERSION));
		CEdit* pEditBenchId = (CEdit*) (this->GetDlgItem(IDC_EDIT_BENCHID));

		pEditMotName->SetWindowTextA("Escolha o Produto");
		pMotName->EnableWindow(true);
		pEditVersion->SetWindowTextA(BZ_VERSION);
		pEditBenchId->SetWindowTextA(pCHandlerBz->GetStationID());

		//20090402 Alan
		if(IsQisdaTaipei() == false)
		{
			pButtonStart->EnableWindow(FALSE);
			pButtonClear->EnableWindow(FALSE);
			pButtonLog->EnableWindow(FALSE);
		}

		nStatus = pCHandlerBz->InitializeEquipments();
		if(0==nStatus)
			nStatus = pCHandlerBz->InitComboModelName();

		//if(0==nStatus)
		//	pCHandlerBz->LoadBZConfig();
	}

	LoadScriptFile(m_szScript);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSPU_UIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CSPU_UIDlg::OnEraseBkgnd( CDC * pDC)
{
	using namespace Gdiplus;		
	using namespace COMMONCONTROL;

	//CRect rect(485,7,485 + nDisplayW, 7 + nDisplayH);
	CRect rect;
    _DoubleBufferPolicy doubleBuffer;
	GetClientRect(&rect);
	Graphics graphics(doubleBuffer.BeginHDC(pDC->GetSafeHdc(),rect));

	DWORD dwResult = GetSysColor(COLOR_3DFACE);
	SolidBrush blackBrush(Color(255, GetRValue(dwResult), GetGValue(dwResult), GetBValue(dwResult)));
   // Create a Rect object.
	Rect fillRect(rect.left, rect.top, rect.Width(), rect.Height());
   // Fill the rectangle.
   graphics.FillRectangle(&blackBrush, fillRect);

	if(m_pImage)
	{

		//Prepare Graphics
	
		UINT nW = m_pImage->GetWidth();
		UINT nH = m_pImage->GetHeight();

		REAL rRation = 1;
		if(nW > nH)
			rRation = nW / 400.0;
		else
			rRation = nH / 400.0;

		UINT nDisplayW = nW / rRation;
		UINT nDisplayH = nH / rRation;
		
		graphics.DrawImage(m_pImage,485,7,nDisplayW,nDisplayH);
		
		{
			CSingleLock waitLock(&m_WaitingObj);
			CSingleLock singleLock(&m_LockObj,TRUE);

			for(std::vector<std::string>::iterator Iter = m_AppendImages.begin();waitLock.IsLocked() == false && Iter != m_AppendImages.end();Iter++)
			{
				if(::_taccess(Iter->c_str(),0) != -1)
				{
					Image image(QSYNC_LIB::GetUnicode(Iter->c_str()).c_str());
					graphics.DrawImage(&image,485,7,nDisplayW,nDisplayH);
				}
			}
		}
	}

	return TRUE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSPU_UIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CSPU_UIDlg::AppendDisplayImage(const CString& strDisplayImage)
{
	char szTemp[5] = {0};
	szTemp[0] = atoi(strDisplayImage);
	std::string strChar = szTemp;
	switch(atoi(strDisplayImage))
	{
	case 0x2A:
		strChar = "70";
		break;
	case 0x23:
		strChar = "90";
		break;
	case 0xB9:
		strChar = "G";
		break;
	case 0x9E:
		strChar = "A";
		break;
	case 0x9F:
		strChar = "B";
		break;
	case 0x51:
		strChar = "E";
		break;
	case 0x50:
		strChar = "S";
		break;
	case 0x09:
		strChar = "L";
		break;
	case 0x0A:
		strChar = "R";
		break;
	case 0x54:
		strChar = "U";
		break;
	case 0x55:
		strChar = "D";
		break;
	case 0x8F:
		strChar = "X";
		break;
	case 0x90:
		strChar = "Y";
		break;
	case 0x70:
		strChar = "o";
		break;
	case 0x52:
		strChar = "C";
		break;
	};
	std::string strPicture = ".\\Pictures\\Keypad\\";
	strPicture += strChar;
	strPicture += ".png";

	if(::_taccess(strPicture.c_str(),0) == -1)
	{
		ShowMessage(strPicture.c_str());
	}
	else
	{
		CSingleLock waitLock(&m_WaitingObj,TRUE);
		CSingleLock singleLock(&m_LockObj,TRUE);

		m_AppendImages.push_back(strPicture);
		CRect rect(485,7,485+400,7+400);
		InvalidateRect(&rect,TRUE);
	}
}

void CSPU_UIDlg::SetDisplayImage(const CString& strDisplayImage)
{
	m_AppendImages.clear();
	m_strImagePath = strDisplayImage;

	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	if(m_strImagePath.IsEmpty() == false && ::_taccess(m_strImagePath,0) != -1)
	{
		m_pImage = new Image(QSYNC_LIB::GetUnicode((LPCTSTR)m_strImagePath).c_str());
	}
	InvalidateRect(NULL,TRUE);
	UpdateWindow();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSPU_UIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSPU_UIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	SYSTEMTIME sTime;
	::GetLocalTime(&sTime);
	//m_szTestTime.Format(_T("%d-%d-%d  %d:%d:%d"),sTime.wYear, sTime.wMonth,sTime.wDay,sTime.wHour,sTime.wMinute,sTime.wSecond);

	this->UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);

}

void CSPU_UIDlg::OnClose( )
{
	CString szTemp;
	GetDlgItem(IDOK)->GetWindowText(szTemp);
	if(szTemp == _T("Start"))
	{
		CDialog::OnClose( );
	}
}

void CSPU_UIDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_pImage)
		delete m_pImage;

	GdiplusShutdown(gdiplusToken);

	if(m_hSPULoader)
		ReleaseHandle(m_hSPULoader);

	if(m_pCallBack)
		delete m_pCallBack;

	// Alan 20090331
	if(pCHandlerBz)
		delete pCHandlerBz;
}

void CSPU_UIDlg::OnBnClickedOk()
{
	// Alan 20090331
	HMODULE hDll = NULL;
	// Marcio 20080327
	char szTrackID[1024] = {0};
	CString szTemp;
	int nStatus = -1;
	GetDlgItem(IDOK)->GetWindowText(szTemp);
	CEdit* pEditTRackId = (CEdit*) (this->GetDlgItem(IDC_EDIT_TRACKID));

	if(szTemp == _T("Start"))
	{		
		OnBnClickedButtonClear();
		SetDisplayImage(_T(""));

#ifdef _DEBUG
		hDll = ::LoadLibrary("MobileToolKit_StaticMD.dll");
#else
		hDll = ::LoadLibrary("MobileToolKit_StaticM.dll");
#endif
		assert(hDll);
		if(hDll)
		{
			typedef bool (*pFnReadTrackID)(char* szPort,char* szTrackID);
			pFnReadTrackID ReadTrackID = (pFnReadTrackID)GetProcAddress(hDll,"ReadTrackID");
			assert(ReadTrackID);
			if(ReadTrackID)
			{
				if(ReadTrackID((LPTSTR)(LPCTSTR)GetPort("COMport"),szTrackID))
				{}
				// Marcio 20080327
				//				AfxMessageBox(szTrackID);
			}
			::FreeLibrary(hDll);
			hDll = NULL;
		}


		m_start = clock();
		// Marcio 20080327
		if(IsQisdaTaipei())
		{
			if(m_hSPULoader == NULL)
				ShowMessage(_T("Please load script first!"));

			if(m_pExecuteThread == NULL)
			{
				if(m_pCallBack)
					m_pCallBack->ResetBreak();
				GetDlgItem(IDOK)->SetWindowText(_T("Break"));
				m_pExecuteThread = AfxBeginThread(ExecuteCommandThread,(LPVOID)this,THREAD_PRIORITY_NORMAL);
			}
		}
		else if(pCHandlerBz->IsFactoryBZ())
		{
			SetValue(IDC_EDIT_RESULT,"");
			m_lbNote.ResetContent();
			pCHandlerBz->SetTrackId(szTrackID); 
			pEditTRackId->SetWindowTextA(pCHandlerBz->GetTrackid());

			if( pCHandlerBz->IsChecStatusEnable() )
				nStatus= pCHandlerBz->CheckStatus();
			else
				nStatus = 0;

			if (0==nStatus)
			{
				nStatus = pCHandlerBz->StartHandlerLogResult();
				m_start = clock();
				if(m_hSPULoader == NULL)
					ShowMessage(_T("Please load script first!"));

				if(m_pExecuteThread == NULL)
				{
					if(m_pCallBack)
						m_pCallBack->ResetBreak();
					GetDlgItem(IDOK)->SetWindowText(_T("Break"));					
					m_pExecuteThread = AfxBeginThread(ExecuteCommandThread,(LPVOID)this,THREAD_PRIORITY_NORMAL);
				}
			}
		}
		else
		{//Undefine
			assert(0);
		}
	}		// Marcio 20080327
	else if(szTemp == _T("Break") && pCHandlerBz->IsFactoryBZ()==false)
	{
		m_pCallBack->SetBreak();
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}

void CSPU_UIDlg::UpdateExecuteResult(SPU_RESULT nResult)
{
	m_nTotal++;
	int nStatus =-1;

	// Marcio 20080327
	if (pCHandlerBz->IsFactoryBZ())
	{
		if (pCHandlerBz->IsPrintLabel()& nResult == SPU_SUCCESS)
			if(0 == pCHandlerBz->PrintLabel())
				pCHandlerBz->EvalHandlerLogResult("PRINTLBL","PRINT LABEL",0,0,0,0,0,0,"NA");
			else
			{
				pCHandlerBz->EvalHandlerLogResult("PRINTLBL","PRINT LABEL",1,0,0,0,0,1,"NA");
				nResult = SPU_FAIL;
			}

			if(nResult == SPU_SUCCESS)
			{
				//Marcio 20090403
				m_lbNote.AddString("PASS   ->    PASS     ->    PASS    ->    PASS");
				m_lbNote.AddString("PASS   ->    PASS     ->    PASS    ->    PASS");
				m_lbNote.AddString("PASS   ->    PASS     ->    PASS    ->    PASS");
				m_lbNote.AddString("PASS   ->    PASS     ->    PASS    ->    PASS");
				nStatus = pCHandlerBz->LogHandlerTestResult((char *)(LPCSTR) pCHandlerBz->GetTrackid(),(char *)(LPCSTR) pCHandlerBz->GetTrackid(), nResult);
			}
			else
			{
				//Marcio 20090403
				m_lbNote.AddString("FAIL-FAIL-FAIL-FAIL-FAIL-FAIL-FAIL-FAIL");
				m_lbNote.AddString("FAIL-FAIL-FAIL-FAIL-FAIL-FAIL-FAIL-FAIL");
				m_lbNote.AddString("FAIL-FAIL-FAIL-FAIL-FAIL-FAIL-FAIL-FAIL");
				nStatus = pCHandlerBz->LogHandlerTestResult((char *)(LPCSTR) pCHandlerBz->GetTrackid(),(char *)(LPCSTR) pCHandlerBz->GetTrackid(), 1);
			}

			pCHandlerBz->SetTrackId("0000000000");

	}
	if(nResult == SPU_SUCCESS)
		m_nPass++;
	else if(nResult == SPU_FAIL)
		m_nFail++;
	else
		;

	SetValue(IDC_EDIT_TOTAL,m_nTotal);
	SetValue(IDC_EDIT_PASS,m_nPass);
	SetValue(IDC_EDIT_FAIL,m_nFail);
	SetValue(IDC_EDIT_FAILRATE,(m_nFail*1.0)/m_nTotal);
	SetValue(IDC_EDIT_RESULT,nResult == SPU_SUCCESS ? _T("Success") : _T("Fail"));

	GetDlgItem(IDOK)->SetWindowText(_T("Start"));	
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CString strDuration;
	int nCuttentTime = clock() - m_start;
	m_nTotalTime += nCuttentTime;
	strDuration.Format(_T("%06d ms"),nCuttentTime);
	SetValue(IDC_EDIT_TIME,strDuration);
	strDuration.Format(_T("%06d ms"),m_nTotalTime/(m_nTotal));
	SetValue(IDC_EDIT_AVERAGETIME,strDuration);
}

void CSPU_UIDlg::LoadScriptFile(const CString& szFilePath)
{
	if(szFilePath.IsEmpty())
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		ShowMessage(_T("Please add the script Path when trige UI"));
	}
	else
	{
		m_szScript = szFilePath;

		if(m_pCallBack == NULL)
			m_pCallBack = new UI_CallbackClass(this);

		ASSERT(m_pCallBack != NULL);
		if(m_pCallBack)
		{
			m_hSPULoader = GetSPULoader(m_pCallBack);
			if(m_hSPULoader)
			{
				BOOL bOK = LoadScript(m_hSPULoader, m_szScript.GetBuffer(0));
				if(bOK)
				{
					const int nBufferSize = 2048;
					TCHAR szTemp[nBufferSize] = {0};
					GetDescription(m_hSPULoader,szTemp,nBufferSize);					
					Parser(szTemp);
					GetHeader(m_hSPULoader,szTemp,nBufferSize);
					Parser(szTemp);
					SetValue(IDC_Description,m_strDescription);
					SetWindowText(m_strTitle);
				}
			}
		}
	}
}

void CSPU_UIDlg::Parser(const TSTRING& strTemp)
{
	using namespace QSYNC_LIB;

	TSTRING strLine,strParameter;
	GetToken(strTemp,strLine,strParameter,_T("\r\n"));
	if(!strParameter.empty())
		Parser(strParameter);

	TSTRING strName,strValue;
	GetToken(strLine,strName,strValue,_T(" "));
	if(strName == _T("Station"))
		m_strTitle = strValue.c_str() + m_strTitle;
	else if(strName == _T("Version"))
		m_strTitle = m_strTitle + strValue.c_str();
	else if(strName == _T("UI"))
		m_strUIType = strValue.c_str();
	else
		m_strDescription =  strLine.c_str() + CString("\r\n") + m_strDescription;
}

void CSPU_UIDlg::ShowMessage(const CString& strMessage)
{
	m_lbNote.SetTopIndex(m_lbNote.InsertString(-1, strMessage));

	// Marcio 20080327
	if(IsQisdaTaipei())
	{
		IReportCenter::Log("SPU_UI",9999,-1,4,(LPCSTR)strMessage);
	}
	else if (pCHandlerBz->IsFactoryBZ())
		pCHandlerBz->AddLogResultLog(strMessage);
}

void CSPU_UIDlg::OnBnClickedButtonClear()
{
	// Delete every other item from the list box.
	while(m_lbNote.GetCount())
		m_lbNote.DeleteString( 0 );
}
void CSPU_UIDlg::SetValue(UINT nID,const CString& strValue)
{
	CString strTemp;
	GetDlgItem(nID)->SetWindowText(strValue);
}

void CSPU_UIDlg::SetValue(UINT nID,double nValue)
{
	CString strTemp;
	strTemp.Format(_T("%.02f"),nValue);
	SetValue(nID,strTemp);
}

void CSPU_UIDlg::SetValue(UINT nID,int nValue)
{
	CString strTemp;
	strTemp.Format(_T("%01d"),nValue);
	SetValue(nID,strTemp);
}

void CSPU_UIDlg::OnBnClickedLogButton()
{
	TCHAR szLogFolder[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szLogFolder,CSIDL_PERSONAL,_T("Log"));
	::ShellExecute(NULL,_T("open"),szLogFolder,NULL,NULL,SW_SHOW);
}

CString CSPU_UIDlg::GetPort(const CString& strPort)
{	
	//Alan 20090331
	const char szDefaultValue[] = "EMPTY";
	char szPort[20] = {0};
	::GetPrivateProfileString(strPort, "Port", szDefaultValue, szPort, 20, ".\\Comport.ini");
	assert(strlen(szPort) > 0 && strcmp(szPort,szDefaultValue) != 0);
	if(strlen(szPort) > 0 && strcmp(szPort,szDefaultValue) != 0)
		SetCOMPort(szPort);

	return m_strCOMPort;
}

void CSPU_UIDlg::SetCOMPort(const CString& strCOMPort)
{
	m_strCOMPort = strCOMPort;
}
void CSPU_UIDlg::OnCbnSelchangeMotname()
{
	// Marcio 20080327
	CString	strText;
	CString	strItem;
	int nIndex;

	CComboBox* pMotModel = (CComboBox*) (this->GetDlgItem(IDC_COMBOMOTMODEL));
	CComboBox* pModel = (CComboBox*) (this->GetDlgItem(IDC_MOTNAME));
	CEdit* pEditMotModel = (CEdit*) (this->GetDlgItem(IDC_EDIT3));
	CButton* pButtonConfig = (CButton*) (this->GetDlgItem(IDC_BUTTON1));


	nIndex = pModel->GetCurSel();
	pModel->GetLBText (nIndex, strItem);

	pCHandlerBz->SetMotName(strItem);
	if ( 0 != pCHandlerBz->InitHandlerLogResult())
		return;
	pCHandlerBz->AddComboMotModel(pCHandlerBz->GetMotModelfromModelFile(strItem));
	strItem = pCHandlerBz->GetOdmName(pCHandlerBz->GetMotName());
	pModel->EnableWindow(FALSE);
	pMotModel->EnableWindow(TRUE);
	pEditMotModel->SetWindowTextA("Escolha o Modelo");
	pButtonConfig->EnableWindow(FALSE);

}

void CSPU_UIDlg::OnCbnSelchangeCombomotmodel()
{
	// Marcio 20080327
	int nIndex;
	CString strItem;
	CString strSeparator = ",";

	CComboBox* pMotModel = (CComboBox*) (this->GetDlgItem(IDC_COMBOMOTMODEL));
	CButton* pButtonStart = (CButton*) (this->GetDlgItem(IDOK));

	nIndex = pMotModel->GetCurSel();

	pMotModel->GetLBText (nIndex, strItem);
	pCHandlerBz->SetMotorolaModel(strItem.Right(strItem.Find(strSeparator)+1));
	pMotModel->EnableWindow(FALSE);
	pButtonStart->EnableWindow(true);

}

bool CSPU_UIDlg::IsQisdaTaipei()
{
	bool bRes = false;
	//20090402 Alan
	char szTest[20] = {0};
	::GetPrivateProfileString("DEBUG", "test", "0", szTest, 20, ".\\TaipeiTestOnly.ini");
	bRes = strcmp(szTest,"1") == 0;
	return bRes;
}
void CSPU_UIDlg::OnBnClickedButton1()
{
	CConfigBZ dlg;
	dlg.DoModal();		
	pCHandlerBz->LoadBZConfig();
}
