// TestUIDlg.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "TestUI.h"
#include "TestUIDlg.h"
#include "../QFlash/QZip.h"
#include "../odmtma_api/motorola_odm_api.h"
#include "../DownloadDll/QDOperator.h"
#include "../../QLib_Export/QLib_export.h"
#include "../../Diag_Export/QualcommDiag.h"
#include "../../../Code/CommonUtil/LogPolicy.h"
#include "../../../Code/CommonUtil/CalculateFunctionTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestUIDlg dialog
using namespace ODMTMA_LIB;

CTestUIDlg* g_pThis = NULL;

CTestUIDlg::CTestUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestUIDlg::IDD, pParent)
	,m_strRepeat(_T("200"))
	,m_ThreadStopEvent(FALSE,FALSE,_T("TestUIThreadSafeStopEvent"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ThreadStopEvent.ResetEvent();
}

void CTestUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LISTLog, m_ListLog);
	DDX_Control(pDX, IDC_LOGLIST, m_strListLog);
	DDX_Text(pDX, IDC_COMPort, m_strCOMPort);
	DDX_Text(pDX, IDC_REPEAT, m_strRepeat);
	DDV_MaxChars(pDX, m_strRepeat, 50000);
	DDX_Control(pDX, IDC_DISPLAY, m_DisplayCtrl);
	DDX_Control(pDX, IDC_DLRESET, m_ChangeDLCtrl);
	DDX_Control(pDX, IDC_BTADDR, m_BTAddrCtrl);
	DDX_Control(pDX, IDC_IMEI, m_IMEICtrl);
}

BEGIN_MESSAGE_MAP(CTestUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestUIDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestUIDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_ChangeDL, &CTestUIDlg::OnBnClickedChangedl)
	ON_BN_CLICKED(IDC_SAFESTOP, &CTestUIDlg::OnBnClickedSafestop)
	ON_BN_CLICKED(IDC_DLRESET, &CTestUIDlg::OnBnClickedDlreset)
	ON_BN_CLICKED(IDC_ChangeFTM, &CTestUIDlg::OnBnClickedChangeftm)
	ON_BN_CLICKED(IDC_BackupNV, &CTestUIDlg::OnBnClickedBackupnv)
	ON_BN_CLICKED(IDC_WriteBTAddr, &CTestUIDlg::OnBnClickedWritebtaddr)
	ON_BN_CLICKED(IDC_WriteIMEI, &CTestUIDlg::OnBnClickedWriteimei)
	ON_BN_CLICKED(IDC_NONETWORK, &CTestUIDlg::OnBnClickedTestNoNetwork)
END_MESSAGE_MAP()


// CTestUIDlg message handlers

BOOL CTestUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);
	g_pThis = this;

	/*BYTE test = 0;
	BYTE test1 = 0x00FF & 0x0001;
	BYTE test2 = test1 << 4;

	BYTE szFsn[16] = {0};
	for(int i=1; i<17; i++)
	{
		szFsn[i] = 0x01*i;
	}
	BYTE szFSN33[33] = {0};
	for(int i=0; i<16; i++)
	{
		szFSN33[i*2] = (szFsn[i] & 0xFF00);
		szFSN33[i*2+1] = (szFsn[i] & 0x00FF) << 4; 
	}*/

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}
typedef bool (*pfn_Download)(long, bool(*CallBack)(const char*, int , int));
typedef bool (*pfn_SetDLIniPath)(const TSTRING&);


void CTestUIDlg::TestDwonload()
{

	/*HMODULE hSysDll = ::LoadLibrary(_T("Download_Export.dll"));
	if(hSysDll)
	{
		pfn_Download Download= (pfn_Download)GetProcAddress(hSysDll, "Download");
		pfn_SetDLIniPath SetDLIniPath = (pfn_SetDLIniPath)GetProcAddress(hSysDll, "SetDLIniPath");
		if(SetDLIniPath)
			SetDLIniPath(_T("C:\\QSyncher_V2\\PlatformTool\\Code\\odmtma_api\\debug\\Download.ini"));
		if(Download)
		{
			bool bDownload = Download(32, CallBackFn);
		}
	}*/

	/*Zip*/
	//TSTRING strunZipFile = _T("..\\Debug\\abc.mot");
	//CQZip QZip;
	//QZip.Unzip(strunZipFile);
	//QZip.SetFolderPath(_T("..\\debug\\abc"));
	//bool bDelete = QZip.DeleteUnzipFiles();
	//bool bZip = QZip.Zip(_T("..\\Debug\\ccc.mot"), _T(""));

	/*ODM*/
	//ODM_Flash(0,"lala", "FIRMWARE", "ccc");

	/*QDOperator*/
	//Backup NV -> Compare Software Version -> Flash -> Flex -> Restore NV -> Flex
	/*CQDOperator QOperator;
	QOperator.SetCOMPort("COM5");
	QOperator.SetPbl("");
	QOperator.SetHex("..\\debug\\NPRG6260.hex");
	QOperator.SetOemsbl("..\\debug\\oemsblhd.mbn");
	QOperator.SetOemsblhd("..\\debug\\oemsbl.mbn");
	QOperator.SetAMSS("..\\debug\\amss.mbn");
	QOperator.SetAMSShd("..\\debug\\amsshd.mbn");
	QOperator.SetPartition("..\\debug\\partition.mbn");
	QOperator.SetQcsbl("..\\debug\\qcsbl.mbn");
	QOperator.SetQcsblhd("..\\debug\\qcsblhd_cfgdata.mbn");
	QOperator.SetCallBackFunc(&(CallBackFn));

	QOperator.DownloadFunc();*/

	//BOOL bIsTestMode = QL_IsTestMode(29);
	/*char szIMEI[100] = {0};
	bool bReadIMEI = QL_ReadIMEI(29,szIMEI);*/

	//Test SerialIOComm
	//CSerioIOComm SerialIO(_T("COM29"));


	/*CQualcommDiag DiagCmd(_T(""), 32);
	BYTE szWriteFSN[16] = {0};
	DiagCmd.GenFSN(szWriteFSN);
	DiagCmd.WriteFSN(szWriteFSN);
	BYTE szFSN[16] = {0};
	DiagCmd.ReadFSN(szFSN);*/

	/*BYTE szFsn[16] = {0};
	for(int i=0; i<16; i++)
	{
		szFsn[i] = 0x01*i;
	}
	BYTE szFSN33[33] = {0};
	for(int i=0; i<16; i++)
	{
		szFSN33[i*2] = (szFsn[i] & 0xF0);
		szFSN33[i*2+1] = (szFsn[i] & 0x0F); 
	}*/

	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	//if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
	{
		CQualcommDiag DiagCmd(_T(""), Ports[0]);
		CQualCmd QualCmd(&DiagCmd);

		QualCmd.ChangeToDLMode();

		UCHAR nStatus = -1;
		UCHAR nVersion = -1;
		QualCmd.CmdGetBootLoaderRevocation(&nStatus,&nVersion,-1);
		QualCmd.CmdGetPhonecodeRevocation(&nStatus,&nVersion,-1);

		QualCmd.ChangeToDiagMode(true);
	}
}

int CTestUIDlg::CallBackFn(const char* szInfo, int nStatus)
{
	return g_pThis->LogFn(szInfo, nStatus);
}
bool CTestUIDlg::CallBackFn(const char* szInfo, int nStep, int nProgress)
{
	return true;
}
int CTestUIDlg::LogFn(const char* szInfo, int nStatus)
{
	BOOL nRet = TRUE;

	int nNewIndex = m_ListLog.InsertItem(m_ListLog.GetItemCount(), szInfo,  I_IMAGECALLBACK);

	//7	CString strLog = szInfo;
	return nRet;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestUIDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestUIDlg::OnBnClickedButton1()
{
	TestDwonload();
}

//#include "../../Diag_Export/ODM.h"
void CTestUIDlg::OnBnClickedButton2()
{	
	CWaitCursor wait;
/*
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
	{
		CODM ODMFunction(Ports[0]);

		TCHAR szXPRSLOGFile[MAX_PATH] = {0};
		//TSTRING strXPRSLOGFile;
		bool bRes = ODMFunction.GetMotoXprsLogger(szXPRSLOGFile,MAX_PATH);
		assert(bRes);
	}*/
}

void CTestUIDlg::OnBnClickedButton3()
{
	CWaitCursor wait;
/*
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
	{
		CODM ODMFunction(Ports[0]);
		bool bRes = ODMFunction.EnableDiagEvent(true);
		assert(bRes);
	}*/
}

void CTestUIDlg::OnBnClickedButton4()
{
	CWaitCursor wait;
/*
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
	{
		CODM ODMFunction(Ports[0]);
		bool bRes = ODMFunction.EnableDiagEvent(false);
		assert(bRes);
	}*/
}

DWORD CTestUIDlg::ThreadProc(LPVOID lpParameter)
{
	CTestUIDlg* pThis = (CTestUIDlg*)lpParameter;
	
	if(!pThis->m_strCOMPort.IsEmpty())
		pThis->TestDL(_ttoi(pThis->m_strCOMPort),::_ttoi(pThis->m_strRepeat));

	return 0;
}

void CTestUIDlg::OnBnClickedChangedl()
{	
	UpdateData(TRUE);

	if(!m_strCOMPort.IsEmpty())
		ChangeToDL(_ttoi(m_strCOMPort));
}

void CTestUIDlg::DisplayString(const CString& strMsg)
{
	m_DisplayCtrl.SetWindowText(strMsg);
}

void CTestUIDlg::AddString(const CString& strMsg,bool bLog)
{
	//m_strListLog.AddString(LogPolicy::GetCurrentTime());
	m_strListLog.AddString(strMsg);
	m_strListLog.UpdateWindow();
	if(bLog)
		LogPolicy::Log(strMsg,m_strLogFile);
}

bool CTestUIDlg::ChangeToDL(int nPort)
{
	CQualcommDiag DiagCmd(_T(""), nPort);
	CQualCmd QualCmd(&DiagCmd);

	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);

	bool bRes = RUN_SUCCESSFULLY == QualCmd.ChangeToDLMode();
	if(bRes)
		AddString(_T("ChangeToDL Success"));
	else
		AddString(_T("ChangeToDL Fail"));

	return bRes;
}

bool CTestUIDlg::ChangeToFTM(int nPort)
{
	CQualcommDiag DiagCmd(_T(""), nPort);
	CQualCmd QualCmd(&DiagCmd);

	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);
	
	bool bRes = RUN_SUCCESSFULLY == QualCmd.ChangeToDiagMode();
	if(bRes)
		AddString(_T("ChangeToDIAG Success"));
	else
		AddString(_T("ChangeToDIAG Fail"));

	return bRes;
}

void CTestUIDlg::TestDL(int nPort,int nRepeatCount)
{
	CQualcommDiag DiagCmd(_T(""), nPort);
	CQualCmd QualCmd(&DiagCmd);

	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);

	int nSuccessCount = 0;
	int nFailCount = 0;
	for(int i=0;i<nRepeatCount;i++)
	{
		if(::WaitForSingleObject(m_ThreadStopEvent,0) == WAIT_OBJECT_0)
		{
			AddString("ChangeToDLMode(); SafeStop");
			break;
		}

		if(RUN_SUCCESSFULLY == QualCmd.ChangeToDLMode())
		{
			AddString("ChangeToDLMode(); SUCCESS");
			nSuccessCount++;

			if(RUN_SUCCESSFULLY == QualCmd.CmdReset(5000))
				AddString(_T("QualCmd.CmdReset(5000)) Success"));
			else
				AddString(_T("QualCmd.CmdReset(5000)) Fail"));

			QualCmd.CloseComPort();
			Sleep(10000);//Make sure comport release success

			//Wait until success to open
			while(1)
			{
				if(QualCmd.OpenComPort() <0)
				{
					Sleep(5000);
					AddString("Sleep(5000) for Reopen");
				}
				else
				{
					AddString("Reopen Success");
					break;
				}
			};
		}
		else
		{
			AddString("ChangeToDLMode(); FAIL");
			nFailCount++;
		}
		
		strFormat.Format(_T("COM%d Test DL&RESET, SUCCESS:%d,FAIL:%d"),nPort,nSuccessCount,nFailCount);
		AddString(strFormat);
	}

	DisplayString(strFormat);
	m_ChangeDLCtrl.EnableWindow(TRUE);
}

void CTestUIDlg::OnBnClickedSafestop()
{
	m_ThreadStopEvent.SetEvent();
}

void CTestUIDlg::OnBnClickedDlreset()
{
	m_ChangeDLCtrl.EnableWindow(FALSE);
	UpdateData(TRUE);
	DWORD dwThreadID;
	m_ThreadStopEvent.ResetEvent();
	::CreateThread(NULL,0,ThreadProc,this,0,&dwThreadID);	
}

void CTestUIDlg::OnBnClickedChangeftm()
{
	UpdateData(TRUE);
	if(!m_strCOMPort.IsEmpty())
		ChangeToFTM(_ttoi(m_strCOMPort));
}

void CTestUIDlg::OnBnClickedBackupnv()
{
	UpdateData(TRUE);
	int nPort = _ttoi(m_strCOMPort);

	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);

	AddString("ChangeToDLMode(); SUCCESS");

	bool bRes = false;
	CQualcommDiag Diag(_T(""),nPort);
	if(Diag.IsConnected())
	{
		bRes = Diag.Backup(8000);
	}
}

void CTestUIDlg::OnBnClickedWritebtaddr()
{
	UpdateData(TRUE);
	int nPort = _ttoi(m_strCOMPort);	

	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);

	bool bRes = false;
	CQualcommDiag Diag(_T(""),nPort);
	if(Diag.IsConnected())
	{
		CString strBTAddr;
		m_BTAddrCtrl.GetWindowText(strBTAddr);
		AddString(strBTAddr);
		if(!strBTAddr.IsEmpty())
		{
			char szBTAddr[100] = {0};
			long nBufferLen = 100;

			AddString("ReadBTAddr()");
			bRes = Diag.ReadBTAddr(szBTAddr,nBufferLen);
			if(!bRes)
				AddString("Diag.ReadBTAddr() Fail");
			else
				AddString(szBTAddr);
			
			AddString("Writebtaddr()");
			bRes = Diag.WriteBTAddr((LPTSTR)(LPCTSTR)strBTAddr);
			if(bRes)
			{
				AddString("ReadBTAddr()");
				bRes = Diag.ReadBTAddr(szBTAddr,nBufferLen);
				if(bRes)
					AddString(szBTAddr);
				else
					AddString("Diag.ReadBTAddr() Fail");
			}
			else
			{
				AddString("Diag.WriteBTAddr() Fail");
			}
		}
		else
			AddString("BT addr can't ne empty");
	}
	else
	{		
		AddString("Diag.IsConnected() Fail");
	}
}

void CTestUIDlg::OnBnClickedWriteimei()
{
	UpdateData(TRUE);
	int nPort = _ttoi(m_strCOMPort);

	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);

	bool bRes = false;
	CQualcommDiag Diag(_T(""),nPort);
	if(Diag.IsConnected())
	{
		char szIMEI[100] = {0};
		long nBufferLen = 100;

		AddString("ReadIMEI()");
		bRes = Diag.ReadIMEI(szIMEI);
		if(bRes)
			AddString(szIMEI);
		else
			AddString("ReadIMEI() Fail");

		AddString("WriteIMEI()");
		CString strIMEI;
		m_IMEICtrl.GetWindowText(strIMEI);
		AddString(strIMEI);
		bRes = Diag.WriteIMEI((LPTSTR)(LPCTSTR)strIMEI);
		if(bRes)
		{
			AddString("ReadIMEI()");
			bRes = Diag.ReadIMEI(szIMEI);
			if(bRes)
				AddString(szIMEI);
			else
				AddString("ReadIMEI() Fail");
		}
		else
			AddString("WriteIMEI() Fail");
	}
	else
	{		
		AddString("Diag.IsConnected() Fail");
	}
}

void CTestUIDlg::OnBnClickedTestNoNetwork()
{
	UpdateData(TRUE);
	int nPort = _ttoi(m_strCOMPort);
	CString strFormat;
	m_strLogFile.Format(_T("Test_COM%02d.txt"),nPort);

	for(int i=1; i<(::_ttoi(m_strRepeat)+1); i++)
	{
		char szBuffer[100] = {0};
		_itoa(i, szBuffer, 10);
		CString strTime = _T("-----------------")+ CString(szBuffer) + _T(" times-----------------");
		AddString(strTime);
		{
			CQualcommDiag Diag(_T(""),nPort);
			if(Diag.IsConnected())
			{
				if(Diag.Cmd_DIAG_CONTROL_F(MODE_OFFLINE_D_F))
					AddString("Diag.Cmd_DIAG_CONTROL_F(MODE_OFFLINE_D_F)");
				else
					AddString("Diag.Cmd_DIAG_CONTROL_F(MODE_OFFLINE_D_F) fail");
			}
		}
		{
			CQualcommDiag Diag(_T(""),nPort);
			if(Diag.IsConnected())
			{
				if(Diag.Cmd_DIAG_CONTROL_F(MODE_RESET_F))
					AddString("Diag.Cmd_DIAG_CONTROL_F(MODE_RESET_F)");
				else
					AddString("Diag.Cmd_DIAG_CONTROL_F(MODE_RESET_F) fail");
			}
		}
		AddString("sleep 38s");
		Sleep(38000);
		Sleep(0);
		{
			CQualcommDiag Diag(_T(""),nPort);
			if(Diag.IsConnected())
			{
				DWORD nStatus = 0;
				if(Diag.GetCMState(nStatus))
				{
					CString strSrcState;
					strSrcState.Format(_T("%2d"),nStatus);
					AddString("Diag.GetCMState = "+strSrcState );
					if(nStatus == 0)
					{
						break;
					}
				}
				else
					AddString("Diag.GetCMState fail");
				
			}
		}
	}
	
	
	


}