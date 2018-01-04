// SecurityToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecurityToolDlg.h"
#include "SimEncorderApi.h"
#include "DlgKeyCheck.h"
#include "QLib.h"
#include "WK.h"
#include "PhoneOperation.h"
#include "DiagCommand.h"
#include <direct.h>
#include "..\STE\MiddleWare\CallbackTypes.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static char *MsgBoxTitle = "SecurityProgramming";

// CSecurityToolDlg dialog
IMPLEMENT_DYNAMIC(CSecurityToolDlg, CDialog)

CSecurityToolDlg::CSecurityToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecurityToolDlg::IDD, pParent)
	, m_sOldNWSCP(_T(""))
	, m_sOldSSCP(_T(""))
	, m_sNewNWSCP(_T(""))
	, m_sNewSSCP(_T(""))
	, m_strIMEI(_T(""))
	, m_strFSN(_T(""))
	, m_strLockFilePath(_T(""))
	, m_strOldSPC(_T(""))
	, m_strNewSPC(_T(""))
{
	m_FirmCode = 864;
	m_UserCode = 1041;
	m_MobileModel = MOBILEMODEL_LAST;
}

void CSecurityToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD_NWCP, m_sOldNWSCP);
	DDX_Text(pDX, IDC_EDIT_OLD_SSCP, m_sOldSSCP);
	DDX_Text(pDX, IDC_EDIT_NEW_NWCP, m_sNewNWSCP);
	DDX_Text(pDX, IDC_EDIT_NEW_SSCP, m_sNewSSCP);
	DDX_Text(pDX, IDC_EDIT_IMEI, m_strIMEI);
	DDX_Text(pDX, IDC_EDIT_FSN, m_strFSN);
	DDX_Text(pDX, IDC_EDIT_LOCKFILE, m_strLockFilePath);
	DDX_Text(pDX, IDC_EDIT_OLD_SPC, m_strOldSPC);
	DDX_Text(pDX, IDC_EDIT_NEW_SPC, m_strNewSPC);
}

BEGIN_MESSAGE_MAP(CSecurityToolDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN_LOCKFILE, &CSecurityToolDlg::OnBnClickedBtnOpenLockfile)
	ON_BN_CLICKED(IDC_BTN_WITE_SIMLOCK, &CSecurityToolDlg::OnBnClickedBtnWiteSimlock)
	ON_BN_CLICKED(IDC_BTN_WITE_SPC, &CSecurityToolDlg::OnBnClickedBtnWiteSpc)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSecurityToolDlg message handlers
BOOL CSecurityToolDlg::OnInitDialog()
{
	m_Check = true;

#ifdef _DEBUG 
//	m_Check = false;
#elif PHONE_ATHENSBT
	m_Check = false;
#endif

	KeyCheck();

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetTimer(10, 3000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSecurityToolDlg::SetCode(long firmCode, long userCode)
{
	m_FirmCode = firmCode;
	m_UserCode = userCode;
}

long CSecurityToolDlg::GetComPort()
{
	unsigned short iNumPorts = 0x1, pPortList;
	char res = QLIB_GetAvailablePhonesPortList(&iNumPorts, &pPortList, 0, NULL);

	if (iNumPorts == 0)
		return 0;

	char comText[20] = {0};
	sprintf(comText, "COM: %d", pPortList);
	SetDlgItemText(IDC_COMPORT, comText);

	return pPortList;
}

bool CSecurityToolDlg::ReadImeiFsn(const int portNum)
{
	using namespace PHONEOP;

	string strOutput;

	// FSN
	BYTE fsn[512] = {0}, emptyfsn[40] = {0};
	BYTE SPCCode[7] = {'0', '0', '0', '0', '0', '0'};
	if (m_strOldSPC.GetLength() == 6)
		memcpy((void*)SPCCode, (LPCTSTR)m_strOldSPC, sizeof(BYTE)*6);

	bool bSuccess = false;
	for (int i = 0; i < 5; i++){
		if (!GetFSN(portNum, fsn, 512, SPCCode))
			continue;
		
		if (memcmp(fsn, emptyfsn, 32) == 0)
			continue;

		bSuccess = true;
		break;
	}

	if (!bSuccess){
		OutputPrompt(MsgBoxTitle, "Read FSN failed", "OK", strOutput);
		return false;
	}

	m_strFSN = "";
	char byteHex[10]= {0};
	for (int i = 0; i < 16; i++){		
		sprintf(byteHex, "%02x", fsn[i]);
		m_strFSN += byteHex;
	}
	UpdateData(FALSE);

	// IMEI
	BYTE imei[20] = {0}, emptyimei[20] = {0};
	bSuccess = false;
	for (int i = 0; i < 5; i++){
		if (!GetIMEI(portNum, imei, 20))
			continue;
		
		if (memcmp(imei, emptyimei, 15) == 0)
			continue;

		bSuccess = true;
		break;
	}
	
	if (!bSuccess){
		OutputPrompt(MsgBoxTitle, "Read IMEI failed", "OK", strOutput);
		return false;
	}

	m_strIMEI = imei;
	UpdateData(FALSE);

	return true;
}

void CSecurityToolDlg::OnBnClickedBtnOpenLockfile()
{
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	CString cstrFilter = "SIM Lock Files (*.loc)|*.loc|All files(*.*)|*.*||";

	CString strWorkDir;
	_getcwd(strWorkDir.GetBuffer(_MAX_PATH), _MAX_PATH);
	strWorkDir.ReleaseBuffer();

	CFileDialog tDialog(TRUE, "loc", NULL, dwFlags, cstrFilter, this);
	if (tDialog.DoModal() == IDOK){
		SetDlgItemText(IDC_EDIT_LOCKFILE, tDialog.GetPathName());
		_chdir(strWorkDir);
	}
}

bool CSecurityToolDlg::KeyCheck()
{
	if (!m_Check)
		return true;

	while(1){
		// Is Wibu key correct?
		if (IsDongleCodeCorrect())
			return true;
		else{
			string strOutput;
			OutputPrompt(MsgBoxTitle, "Please make sure the WibuKey with correct code is inserted!!", "YesNo", strOutput);
			if (strOutput == "No")
				break;
		}
	}

/*	CDlgKeyCheck dlg;
	string strOutput;
	while(1){
		if (dlg.DoModal() == IDOK){
			m_FirmCode = _atoi64(dlg.m_strFirmCode);
			m_UserCode = _atoi64(dlg.m_strUserCode);

			if (IsDongleCodeCorrect())			
				return true;			
			else{
				OutputPrompt(MsgBoxTitle, "Please insert the WibuKey and input the correct WibuKey code", "OK", strOutput);
				continue;
			}
		}
		else
		{			
			break;
		}
	}
	*/
	OnCancel();
	return false;
}

bool CSecurityToolDlg::IsDongleCodeCorrect()
{
	// Check the firm code and user code
	ULONG flCtrl = WKB_STDCTRL| WKB_OPEN_FIND| WKB_VERSION2| WKB_OPEN_NONETLIMIT;
	HWKBENTRY hwkbe = WkbOpen2(HWKB_LOCAL, flCtrl, NULL, m_FirmCode, m_UserCode, NULL);

	if (hwkbe == 0)	
		return false;	

	WkbClose2(hwkbe);
	return true;
}

void CSecurityToolDlg::OnBnClickedBtnWiteSimlock()
{
	KeyCheck();

	const int portNum = GetComPort();
	string strOutput;

	if (portNum == 0){
		OutputPrompt(MsgBoxTitle, "Connect phone failed...", "OK", strOutput);
		return;
	}

	UpdateData();

	if (!ReadImeiFsn(portNum))
		return;	

	// Unlock first
	if (!UnLockSimData())
		return;

	// Check lock code
	if (m_sNewNWSCP == "")		m_sNewNWSCP = "11111111";
	if (m_sNewSSCP == "")		m_sNewSSCP = "21111111";

	UpdateData(FALSE);

	// Write Sim Data
	if (!WriteSimData())
		return;
	
	// Restart phone and check
	Sleep(3000);

	if (!(PHONEOP::QueryLockStatus(portNum))){
		OutputPrompt(MsgBoxTitle, "Write Sim Lock failed", "OK", strOutput);
		return;
	}

	OutputPrompt(MsgBoxTitle, "Write Sim Lock completed", "OK", strOutput);
}

bool CSecurityToolDlg::UnLockSimData()
{
	using namespace PHONEOP;

	const int portNum = GetComPort();
	string strOutput;

	bool nwStatus, spStatus, nsStatus, cpStatus, simStatus, masterStatus;
	DWORD status;

	if (!QueryLockStatus(portNum, &nwStatus, &spStatus, &nsStatus, &cpStatus, &simStatus, &masterStatus)){
		return true;	// assume that the only reason is empty sim data
	//	OutputPrompt(MsgBoxTitle, "Query Sim Lock Status failed", "OK", strOutput);
	//	return false;
	}

	// Sim Lock
	int unlockType[5] = {GSDIDIAG_PERSO_NW, GSDIDIAG_PERSO_NS,	GSDIDIAG_PERSO_SP, GSDIDIAG_PERSO_CP, GSDIDIAG_PERSO_SIM};
	bool lockStatus[5];
	lockStatus[0] = nwStatus;
	lockStatus[1] = nsStatus;
	lockStatus[2] = spStatus;
	lockStatus[3] = cpStatus;
	lockStatus[4] = simStatus;

	for (int i = 0; i < 5; i++){
		if (!lockStatus[i])
			continue;	

		if (m_sOldNWSCP.GetLength() != 8){
			OutputPrompt(MsgBoxTitle, "Invalid old NWSCP code", "OK", strOutput);
			return false;
		}
		
		if (!PHONEOP::UnLockSimData(portNum, unlockType[i], m_sOldNWSCP.GetBuffer(), &status)){
			m_sOldNWSCP.ReleaseBuffer();
			OutputPrompt(MsgBoxTitle, "Unlock failed", "OK", strOutput);
			return false;
		}
		m_sOldNWSCP.ReleaseBuffer();
		
		if (status != 0){
			OutputPrompt(MsgBoxTitle, "Wrong old NWSCP code", "OK", strOutput);
			return false;
		}
	}

	// Master Lock
	if (masterStatus)
	{
		if (m_sOldSSCP.GetLength() != 8){
			OutputPrompt(MsgBoxTitle, "Invalid old SSCP code", "OK", strOutput);
			return false;
		}

		if (!PHONEOP::UnLockPhoneMasterCode(portNum, m_sOldSSCP.GetBuffer(), &status))
		{
			m_sOldSSCP.ReleaseBuffer();
			OutputPrompt(MsgBoxTitle, "Unlock failed!", "OK", strOutput);
			return false;
		}
		m_sOldSSCP.ReleaseBuffer();

		/*
		if (!PHONEOP::UnLockSimData(portNum, GSDIDIAG_PERSO_PROP1, m_sOldSSCP.GetBuffer(), &status)){
			m_sOldSSCP.ReleaseBuffer();
			OutputPrompt(MsgBoxTitle, "Unlock failed", "OK", strOutput);
			return false;
		}
		m_sOldSSCP.ReleaseBuffer();
	*/
		if (status != 0){
			OutputPrompt(MsgBoxTitle, "Wrong old SSCP code", "OK", strOutput);
			return false;
		}
	}

	return true;
}

bool CSecurityToolDlg::WriteSimData()
{
	UpdateData();

	using namespace PHONEOP;

	const int portNum = GetComPort();
	if (portNum == 0)
		return false;

	GetMobileModel();	

	string strOutput;

	if (m_strFSN.GetLength() != 32) {
		OutputPrompt(MsgBoxTitle, "Invalid FSN", "OK", strOutput);
		return false;
	}

	if (m_strIMEI.GetLength() != 15) {
		OutputPrompt(MsgBoxTitle, "Invalid IMEI", "OK", strOutput);
		return false;
	}

	if (m_strLockFilePath == "") {
		OutputPrompt(MsgBoxTitle, "Invalid Lock File Path", "OK", strOutput);
		return false;
	}

	if (m_sNewNWSCP.GetLength() != 8) {
		OutputPrompt(MsgBoxTitle, "Invalid NWSCP code", "OK", strOutput);
		return false;
	}

	if (m_sNewSSCP.GetLength() != 8) {
		OutputPrompt(MsgBoxTitle, "Invalid SSCP code", "OK", strOutput);
		return false;
	}

	BYTE *encryptData = NULL;
	int szEncryptData;
	
	int simTypeID = -1;
	switch(m_MobileModel){
		case MOBILEMODEL_WELLFLEET:
			simTypeID = 1;
			break;
		case MOBILEMODEL_LAVERNOCK:
		case MOBILEMODEL_MELOS:
			simTypeID = 5;
			break;
		default:
			ASSERT(0);
	}

	// Generate sim data
	char *NWCode = m_sNewNWSCP.GetBuffer();
	char *SSCode = m_sNewSSCP.GetBuffer();
	int result = 0;
	if ((result = PHONEOP::GenSimFile(simTypeID, (char*)(LPCTSTR)m_strIMEI, (char*)(LPCTSTR)m_strFSN, (char*)SSCode, (char*)NWCode, (char*)(LPCTSTR)m_strLockFilePath, &encryptData, &szEncryptData)) != 0) {
		m_sNewNWSCP.ReleaseBuffer();
		m_sNewSSCP.ReleaseBuffer();
		OutputPrompt(MsgBoxTitle, "Generate Sim Data failed", "OK", strOutput);
		return false;
	}
	m_sNewNWSCP.ReleaseBuffer();
	m_sNewSSCP.ReleaseBuffer();

	// Write sim data
	DWORD iCmdStatus, iStatus;
	if (!PHONEOP::WriteSimData(portNum, encryptData, szEncryptData, &iCmdStatus, &iStatus)) {
		OutputPrompt(MsgBoxTitle, "Write Sim Lock failed", "OK", strOutput);
		delete []encryptData;
		return false;
	}
	delete []encryptData;

	if (iCmdStatus != 0 || iStatus != 0) {
		OutputPrompt(MsgBoxTitle, "Write Sim Lock failed", "OK", strOutput);
		return false;
	}

	// Write SimLock File Name
	if (m_MobileModel == MOBILEMODEL_WELLFLEET) {
		CString simFileName;
		int nBackSlashPos = m_strLockFilePath.ReverseFind('\\');
		if (nBackSlashPos == -1)
			simFileName = m_strLockFilePath;
		else
			simFileName = m_strLockFilePath.Right(m_strLockFilePath.GetLength()-nBackSlashPos-1);

		int nDotPos = simFileName.ReverseFind('.');
		if (nDotPos != -1)
			simFileName = simFileName.Left(nDotPos);
		
		if (!PHONEOP::WriteSimLockFileName(portNum, (char*)(LPCTSTR)simFileName)) {
			OutputPrompt(MsgBoxTitle, "Write Sim Lock File Name failed", "OK", strOutput);
			return false;
		}
	}

	return true;
}

void CSecurityToolDlg::OnBnClickedBtnWiteSpc()
{
	KeyCheck();

	const int portNum = GetComPort();
	string strOutput;

	if (portNum == 0){
		OutputPrompt(MsgBoxTitle, "Connect phone failed...", "OK", strOutput);
		return;
	}

	UpdateData();

	if (m_strOldSPC.GetLength() != 6){
		OutputPrompt(MsgBoxTitle, "Invalid old SPC code", "OK", strOutput);
		return;
	}

	if (m_strNewSPC.GetLength() != 6){
		OutputPrompt(MsgBoxTitle, "Invalid new SPC code", "OK", strOutput);
		return;
	}

	// Test if the SPC is correct
	if (!PHONEOP::QuerySPC(portNum, (BYTE*)(LPCTSTR)m_strOldSPC)){
		Sleep(3000);
		BYTE defaultSPC[7] = {'0', '0', '0', '0', '0', '0'};
		if (!PHONEOP::QuerySPC(portNum, defaultSPC)){
			OutputPrompt(MsgBoxTitle, "Wrong old SPC code", "OK", strOutput);
			return;
		}
	}
	
	// Write SPC code
	if (!PHONEOP::SetSPC(portNum, (BYTE*)(LPCTSTR)m_strNewSPC)){
		OutputPrompt(MsgBoxTitle, "Write new SPC failed", "OK", strOutput);
		return;
	}

	OutputPrompt(MsgBoxTitle, "Write newSPC completed", "OK", strOutput);
}

void CSecurityToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateTitleBarText();
}

void CSecurityToolDlg::UpdateTitleBarText()
{
	GetMobileModel();

	CString strTitleText = _T("SecurityTool");	
	if (m_MobileModel < MOBILEMODEL_LAST)
		strTitleText = CString(_T("SecurityTool - ")) + MOBILEMARKETNAME[m_MobileModel];

	SetWindowText(strTitleText);
}

void CSecurityToolDlg::GetMobileModel()
{
	m_MobileModel = MOBILEMODEL_LAST;

	const int portNum = GetComPort();
	if (portNum != 0)
		m_MobileModel = PHONEOP::GetCurrentMobileModel(portNum);
}