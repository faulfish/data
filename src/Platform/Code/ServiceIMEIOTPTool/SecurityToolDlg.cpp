// SecurityToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecurityToolDlg.h"
#include "QLib.h"
#include "PhoneOperation.h"
#include "diagcommand.h"
#include "CallbackTypes.h"
#include "WK.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static char *MsgBoxTitle = "OneTimeProgramming";
const UINT g_RetryTimes = 1;

// CSecurityToolDlg dialog
IMPLEMENT_DYNAMIC(CSecurityToolDlg, CDialog)

CSecurityToolDlg::CSecurityToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecurityToolDlg::IDD, pParent)
	, m_strIMEI(_T(""))
	, m_strFSN(_T(""))
	, m_strBTAddr(_T(""))
{
	m_nTimer = 0;
	m_nTimeCounter = 0;

#ifdef _DEBUG
	m_FirmCode = 10;
	m_UserCode = 13;
#else
	m_FirmCode = 864;
	m_UserCode = 1041;
#endif
}

void CSecurityToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IMEI, m_strIMEI);
	DDV_MaxChars(pDX, m_strIMEI, 15);
	DDX_Text(pDX, IDC_EDIT_BTADDR, m_strBTAddr);
	DDV_MaxChars(pDX, m_strBTAddr, 12);
}

BEGIN_MESSAGE_MAP(CSecurityToolDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRD_FINISH, OnWorkThreadFinish)
	ON_BN_CLICKED(IDC_BTN_WRITE_IMEI_FSN, &CSecurityToolDlg::OnBnClickedBtnWriteImeiFsn)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSecurityToolDlg message handlers
BOOL CSecurityToolDlg::OnInitDialog()
{
	m_Check = TRUE;
	KeyCheck();

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here	
	m_nComPort = 0;
	memset(m_bStepCheck, 0, sizeof(BOOL)*STEP_COUNT);

	m_bmpCheck = Bitmap::FromResource(AfxGetInstanceHandle(), (const WCHAR*)MAKEINTRESOURCE(IDB_CHECK));
 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSecurityToolDlg::SetCode(long firmCode, long userCode)
{
}

BOOL CSecurityToolDlg::KeyCheck()
{
	if (!m_Check)
		return TRUE;

	while(1){
		// Is Wibu key correct?
		if (IsDongleCodeCorrect())
			return TRUE;
		else{
			string strOutput;
			OutputPrompt(MsgBoxTitle, "Please make sure the WibuKey with correct code is inserted!!", "YesNo", strOutput);
			if (strOutput == "No")
				break;
		}
	}

	OnCancel();
	return FALSE;
}

BOOL CSecurityToolDlg::IsDongleCodeCorrect()
{
	// Check the firm code and user code
	ULONG flCtrl = WKB_STDCTRL| WKB_OPEN_FIND| WKB_VERSION2| WKB_OPEN_NONETLIMIT;
	HWKBENTRY hwkbe = WkbOpen2(HWKB_LOCAL, flCtrl, NULL, m_FirmCode, m_UserCode, NULL);

	if (hwkbe == 0)	
		return FALSE;	

	WkbClose2(hwkbe);
	return TRUE;
}

long CSecurityToolDlg::GetComPort()
{
	unsigned short iNumPorts = 0x1, pPortList;
	char res = QLIB_GetAvailablePhonesPortList(&iNumPorts, &pPortList, 0, NULL);

	string strOutput;
	if (iNumPorts == 0){
		OutputPrompt(MsgBoxTitle, "Connect phone failed...", "OK", strOutput);
		return 0;
	}

	char comText[20] = {0};
	sprintf(comText, "COM: %d", pPortList);
	SetDlgItemText(IDC_COMPORT, comText);

	m_nComPort = pPortList;
	return pPortList;
}

void CSecurityToolDlg::OnBnClickedBtnWriteImeiFsn()
{
	if (GetComPort() == 0)	
		return;

	EnableButton(FALSE);
	EnableAllItems(FALSE);
	UpdateData();

	// Get Data Ready
	RandomGenFSN();

	string strOutput;
	if (m_strIMEI.GetLength() != 15 || !CheckIMEIValid(m_strIMEI)){
		OutputPrompt(MsgBoxTitle, "Invalid IMEI", "OK", strOutput);
		EnableButton(TRUE);
		return;
	}

	if (m_strBTAddr.GetLength() != 12 || !CheckBTAddrValid(m_strBTAddr)){
		OutputPrompt(MsgBoxTitle, "Invalid BTAddr", "OK", strOutput);
		EnableButton(TRUE);
		return;
	}	

	// ---------------
	m_nTimer = SetTimer(10, 800, 0);
	m_nTimeCounter = 0;
	m_hThread = CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->OneStopProgramming, (LPVOID)this, 0, NULL);	

	return;
}

void CSecurityToolDlg::EnableButton(BOOL bEnable)
{
	CWnd *pWnd = GetDlgItem(IDC_BTN_WRITE_IMEI_FSN);
	pWnd->EnableWindow(bEnable);	

	pWnd = GetDlgItem(IDC_EDIT_IMEI);
	pWnd->EnableWindow(bEnable);

	pWnd = GetDlgItem(IDC_EDIT_BTADDR);
	pWnd->EnableWindow(bEnable);
}

void CSecurityToolDlg::EnableAllItems(BOOL bEnable)
{
	for (int i = 0; i < 5; i++)
		EnableItem(i, bEnable);
}

void CSecurityToolDlg::EnableItem(UINT nStepItem, BOOL bEnable)
{
	m_bStepCheck[nStepItem] = bEnable;
	CRect rcItemRegion(26, 173, 40, 290);
	InvalidateRect(rcItemRegion);
	UpdateWindow();
}

BOOL CSecurityToolDlg::OSP_ChangeToFTM()
{
	ASSERT (m_nComPort != 0);

	if (!PHONEOP::ChangeToFTMMode(m_nComPort)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Change To FTMMode failed", "OK", strOutput);
		return FALSE;
	}

	if (!PHONEOP::RestartPhone(m_nComPort, 25000)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Restart phone failed", "OK", strOutput);
		return FALSE;
	}

	return TRUE;
}

BOOL CSecurityToolDlg::OSP_RestoreBRT()
{
	ASSERT (m_nComPort != 0);

	CString strBRTFilePath = GetBRTFilePath();

	if (!PHONEOP::RestoreBRT(m_nComPort, (LPCTSTR)strBRTFilePath)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Backup BRT failed", "OK", strOutput);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CSecurityToolDlg::OSP_QFuse()
{
    unsigned long nReg[5] = {0};
    PHONEOP::CheckHWVersion(m_nComPort, nReg);
    PHONEOP::WriteQFuse(m_nComPort, 0, nReg);
	PHONEOP::ChangeToFTMMode(m_nComPort);
    PHONEOP::RestartPhone(m_nComPort, 25000);
    PHONEOP::CheckQFuse(m_nComPort, nReg);
	return TRUE;
}

BOOL CSecurityToolDlg::OSP_IMEI()
{
	if (!SetIMEI())		return FALSE;
	if (!SetFSN())		return FALSE;
	if (!SetBTAddr())	return FALSE;

	return TRUE;
}

BOOL CSecurityToolDlg::OSP_OTP()
{
	ASSERT (m_nComPort != 0);

	if (!PHONEOP::FreezeOTP(m_nComPort)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Freeze OTP failed", "OK", strOutput);
		return FALSE;
	}

	if (!PHONEOP::BackNV(m_nComPort)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Backup Mobile Settings failed", "OK", strOutput);
		return FALSE;
	}

	return TRUE;
}

BOOL CSecurityToolDlg::OSP_ChangeToOnline()
{
	ASSERT (m_nComPort != 0);

	if (!PHONEOP::ChangeToOnlineMode(m_nComPort)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Change To Online failed", "OK", strOutput);
		return FALSE;
	}

	if (!PHONEOP::RestartPhone(m_nComPort, 5000)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Restart phone failed", "OK", strOutput);
		return FALSE;
	}
}

BOOL CSecurityToolDlg::GetIMEI()
{
	ASSERT (m_nComPort != 0);

	BYTE imei[20] = {0}, emptyimei[20] = {0};
	BOOL bSuccess = FALSE;
	for (int i = 0; i < g_RetryTimes; i++){
		if (!PHONEOP::GetIMEI(m_nComPort, imei, 20))
			continue;

		if (memcmp(imei, emptyimei, 15) == 0)
			continue;

		bSuccess = TRUE;
		break;
	}

	if (!bSuccess){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Read IMEI failed", "OK", strOutput);
		return false;
	}

	m_strIMEI = imei;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CSecurityToolDlg::SetIMEI()
{
	ASSERT (m_nComPort != 0);

	if (!PHONEOP::SetIMEI(m_nComPort, (BYTE*)(LPCTSTR)m_strIMEI)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Write IMEI failed", "OK", strOutput);
		return FALSE;
	}

	return TRUE;
}

BOOL CSecurityToolDlg::GetFSN()
{
	ASSERT (m_nComPort != 0);

	BYTE fsn[40] = {0}, emptyfsn[40] = {0};
	BYTE SPCCode[7] = {'0', '0', '0', '0', '0', '0'};

	BOOL bSuccess = FALSE;
	for (int i = 0; i < g_RetryTimes; i++){
		if (!PHONEOP::GetFSN(m_nComPort, fsn, 512, SPCCode))
			continue;

		if (memcmp(fsn, emptyfsn, 32) == 0)
			continue;

		bSuccess = TRUE;
		break;
	}

	if (!bSuccess){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Read FSN failed", "OK", strOutput);
		return FALSE;
	}	

	char strFSN[512] = {0};
	char byteHex[10]= {0};
	for (int i = 0; i < 16; i++){		
		sprintf(byteHex, "%02x", fsn[i]);
		strcat(strFSN, byteHex);
	}
	m_strFSN = strFSN;
	UpdateData(FALSE);

	return TRUE;
}

BOOL CSecurityToolDlg::SetFSN()
{
	ASSERT (m_nComPort != 0);
	
	BYTE hexFSN[20] = {0};
	for (int i = 0; i < 16; i++){
		char hexByte[5] = {0};
		char hexValue;
		hexByte[0] = '0';
		hexByte[1] = 'x';
		hexByte[2] = m_strFSN[2*i];
		hexByte[3] = m_strFSN[2*i+1];
		hexByte[4] = 0;
		sscanf(hexByte, "%x", &hexValue);
		hexFSN[i] = hexValue;
	}

	BYTE SPCCode[7] = {'0', '0', '0', '0', '0', '0'};
	if (!PHONEOP::SetFSN(m_nComPort, hexFSN, SPCCode)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Write FSN failed", "OK", strOutput);
		return FALSE;
	}

	return TRUE;
}

BOOL CSecurityToolDlg::GetBTAddr()
{
	ASSERT (m_nComPort != 0);

	BYTE btAddr[20] = {0}, emptyAddr[20] = {0};
    BOOL bSuccess = FALSE;
	for (int i = 0; i < g_RetryTimes; i++){
		if (!PHONEOP::GetBTAddress(m_nComPort, btAddr, 20))
			continue;

		if (memcmp(btAddr, emptyAddr, 20) == 0)
			continue;

		bSuccess = TRUE;
		break;
	}

	if (!bSuccess){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Read BTAddress failed", "OK", strOutput);
		return FALSE;
	}
	m_strBTAddr = btAddr;
	UpdateData(FALSE);

	return TRUE;
}

BOOL CSecurityToolDlg::SetBTAddr()
{
	ASSERT (m_nComPort != 0);

	if (!PHONEOP::SetBTAddress(m_nComPort, (BYTE*)(LPCTSTR)m_strBTAddr)){
		string strOutput;
		OutputPrompt(MsgBoxTitle, "Write BTAddress failed", "OK", strOutput);
		return FALSE;
	}
	return TRUE;
}

CString CSecurityToolDlg::GetBRTFilePath()
{
	CString strModulePathName;
	::GetModuleFileName(AfxGetApp()->m_hInstance, strModulePathName.GetBuffer(_MAX_PATH), _MAX_PATH);	
	strModulePathName.ReleaseBuffer();
	strModulePathName = strModulePathName.Left(strModulePathName.ReverseFind(_T('\\')));

	// currently only consider for Athens
	return strModulePathName+_T("\\WellFleet_ATHB1A _PCR1_QV002.brt");
}

void CSecurityToolDlg::RandomGenFSN()
{
	TCHAR literals[16] = {_T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9'), 
		_T('a'), _T('b'), _T('c'), _T('d'), _T('e'), _T('f')};

	srand((unsigned)time(NULL));

	m_strFSN = _T("");
	for (UINT i = 0; i < 32; i++)
		m_strFSN += literals[rand()%16];

	ASSERT(m_strFSN.GetLength() == 32);
}

BOOL CSecurityToolDlg::CheckIMEIValid(LPCTSTR pIMEI)
{
	BOOL bValid = TRUE;
	for (int i = 0; i < 15; i++)
		if (_istdigit(pIMEI[i]) == 0) {
			bValid = FALSE;
			break;
		}

	if (!bValid)
		return bValid;

	int iSum = 0;
	BOOL bDouble = FALSE;
	for (int i = 0; i < 15; i++) {
		int iValue = pIMEI[i] - _T('0');
		if (bDouble)
			iValue = iValue << 1;
		
		iSum = iSum + (iValue/10 + iValue%10);
		bDouble = !bDouble;
	}

	if ((iSum%10) != 0)
		bValid = FALSE;

	return bValid;
}

BOOL CSecurityToolDlg::CheckBTAddrValid(LPCTSTR pBT)
{
	BOOL bValid = TRUE;
	for (int i = 0; i < 12; i++)
		if (_istdigit(pBT[i]) == 0 && _istalpha(pBT[i]) == 0) {
			bValid = FALSE;
			break;
		}

	return bValid;
}

UINT CSecurityToolDlg::OneStopProgramming(LPVOID param)
{
	CSecurityToolDlg* pDlg = (CSecurityToolDlg*)param;
	string strOutput;

	// -------------------------
	if (!pDlg->OSP_ChangeToFTM()){
		pDlg->SendMessage(WM_TRD_FINISH);
		return 0;
	}
	pDlg->EnableItem(0, TRUE);

	// -------------------------
	if (!pDlg->OSP_RestoreBRT()){
		pDlg->SendMessage(WM_TRD_FINISH);
		return 0;
	}
	pDlg->EnableItem(1, TRUE);

	// -------------------------
	if (!pDlg->OSP_QFuse()){
		pDlg->SendMessage(WM_TRD_FINISH);
		return 0;
	}
	pDlg->EnableItem(2, TRUE);

	// -------------------------
	if (!pDlg->OSP_IMEI()){
		pDlg->SendMessage(WM_TRD_FINISH);
		return 0;
	}
	pDlg->EnableItem(3, TRUE);

	// -------------------------
	if (!pDlg->OSP_OTP()){
		pDlg->SendMessage(WM_TRD_FINISH);
		return 0;
	}
	pDlg->EnableItem(4, TRUE);	

	// -------------------------
	if (!pDlg->OSP_ChangeToOnline()){
		pDlg->SendMessage(WM_TRD_FINISH);
		return 0;
	}	

	// -------------------------
	Sleep(1000);
	pDlg->SendMessage(WM_TRD_FINISH);
	OutputPrompt(MsgBoxTitle, "Programming completed", "OK", strOutput);

	return 0;
}

LRESULT CSecurityToolDlg::OnWorkThreadFinish(WPARAM wParam, LPARAM lParam)
{
	EnableButton(TRUE);
//	EnableAllItems(FALSE);

	if (m_hThread)
		CloseHandle(m_hThread);

	KillTimer(m_nTimer);
	m_nTimer = 0;

	CWnd *pWnd = GetDlgItem(IDC_BTN_WRITE_IMEI_FSN);
	pWnd->SetWindowText(_T("Run"));

	return 0;
}
void CSecurityToolDlg::OnClose()
{
	if (m_bmpCheck)
		delete m_bmpCheck;

	CDialog::OnClose();
}

void CSecurityToolDlg::OnPaint()
{
	Graphics graphics(GetDC()->m_hDC);
	ColorMap cMap;
	ImageAttributes	imageAtt;
	cMap.oldColor = Color(255, 0, 0, 255);
	cMap.newColor = Color(0, 255, 255, 255);	// transparent
	imageAtt.SetRemapTable(1, &cMap);

	for (int i = 0; i < STEP_COUNT; i++) 
		if (m_bStepCheck[i]) {
			Rect rcCheck(0, 0, 14, 14);
			rcCheck.X = 26;
			rcCheck.Y = 173 + i*20;
			graphics.DrawImage(m_bmpCheck, rcCheck, 0, 0, m_bmpCheck->GetWidth(), m_bmpCheck->GetHeight(), UnitPixel, &imageAtt);
		}	

	return CDialog::OnPaint();
}

void CSecurityToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	CWnd *pWnd = GetDlgItem(IDC_BTN_WRITE_IMEI_FSN);
	
	CString strText;
	switch (m_nTimeCounter) {
		case 0:
			strText = _T("Run     ");
			break;
		case 1:
			strText = _T("Run.    ");
			break;
		case 2:
			strText = _T("Run..   ");
			break;
		case 3:
			strText = _T("Run...  ");
			break;
		case 4:
			strText = _T("Run.... ");
			break;
		case 5:
			strText = _T("Run.....");
			break;
		default:
			ASSERT(0);
	}

	m_nTimeCounter = (m_nTimeCounter+1)%6;
	pWnd->SetWindowText(strText);
}