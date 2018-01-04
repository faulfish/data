// QFuseToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QFuseTool.h"
#include "QFuseToolDlg.h"
#include "../FactoryTool/MobileToolKit/MobileToolKit.h"
#include "../Modules/QisdaMiscellaneousModule/QisdaMiscellaneousModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Parameters for Wellfleet
#define WellfleetParameter1 _T("301007")
#define WellfleetParameter2 _T("40000E00")

// Parameters for Lavernock
#define LavernockParameter1 _T("301016")
#define LavernockParameter2 _T("40000600")

// CQFuseToolDlg dialog

CQFuseToolDlg::CQFuseToolDlg(CWnd* pParent /*=NULL*/)
: CDialog(CQFuseToolDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQFuseToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQFuseToolDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(ID_GET_PORT, &CQFuseToolDlg::OnBnClickedGetPort)
    ON_BN_CLICKED(ID_MSM6_READ_QFUSE, &CQFuseToolDlg::OnBnClickedMsm6ReadQfuse)
    ON_BN_CLICKED(ID_MSM6_WRITE_QFUSE, &CQFuseToolDlg::OnBnClickedMsm6WriteQfuse)
    ON_BN_CLICKED(ID_MSM7_READ_QFUSE, &CQFuseToolDlg::OnBnClickedMsm7ReadQfuse)
    ON_BN_CLICKED(ID_MSM7_WRITE_QFUSE, &CQFuseToolDlg::OnBnClickedMsm7WriteQfuse)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CQFuseToolDlg::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_CHANGE_FTM, &CQFuseToolDlg::OnBnClickedChangeFtm)
    ON_BN_CLICKED(IDC_CHANGE_ONLINE, &CQFuseToolDlg::OnBnClickedChangeOnline)
END_MESSAGE_MAP()


// CQFuseToolDlg message handlers

BOOL CQFuseToolDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    OnBnClickedGetPort();
    CStatic* staticModel = (CStatic*)GetDlgItem(IDC_STATIC_MODEL);
    CStringA strModel;
    staticModel->GetWindowText(strModel);
    if(strModel == "Athens") { // Wellfleet
        CEdit* pe1 = (CEdit*)GetDlgItem(IDC_EDIT1);
        pe1->SetWindowText(_T("0"));
        CEdit* pe2 = (CEdit*)GetDlgItem(IDC_EDIT2);
        pe2->SetWindowText(WellfleetParameter1);
        CEdit* pe3 = (CEdit*)GetDlgItem(IDC_EDIT3);
        pe3->SetWindowText(WellfleetParameter2);
    } else if(strModel == "Lavernock") {
        CEdit* pe1 = (CEdit*)GetDlgItem(IDC_EDIT1);
        pe1->SetWindowText(_T("0"));
        CEdit* pe2 = (CEdit*)GetDlgItem(IDC_EDIT2);
        pe2->SetWindowText(LavernockParameter1);
        CEdit* pe3 = (CEdit*)GetDlgItem(IDC_EDIT3);
        pe3->SetWindowText(LavernockParameter2);
    }
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQFuseToolDlg::OnPaint()
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
HCURSOR CQFuseToolDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CQFuseToolDlg::DeleteAllComportList() {
    CComboBox *pcb = (CComboBox*)GetDlgItem(IDC_COMBO1);
    while(pcb->GetCount() > 0) {
        pcb->DeleteString(0);
    }
}

void CQFuseToolDlg::AddPortToCombo(int nPorts,unsigned short* Ports) {
    CComboBox *pcb = (CComboBox*)GetDlgItem(IDC_COMBO1);
    for(int i = 0 ; i < nPorts; ++i) {
        CString strPort;
        strPort.Format(_T("COM%d"), Ports[i]);
        m_mapPortList[strPort] = Ports[i];
        pcb->AddString((LPCTSTR)strPort);
    }
    pcb->SetCurSel(0);
    pcb->GetLBText(0, m_strSelectedPort);
}

void CQFuseToolDlg::OnBnClickedGetPort()
{
    unsigned short nPorts = 20;
    unsigned short Ports[20] = {0};
    if(QL_GetDiagPort(&nPorts, Ports) && nPorts > 0) {
        DeleteAllComportList();
        AddPortToCombo(nPorts, Ports);
    }
    if(nPorts == 0) return;

    char szSW[512] = {0};
    char szHW[512] = {0};
    CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(Ports[0]);
    if(!pMsModule->GetSWHWVersion(szSW, szHW)) {
        delete pMsModule;
        return;
    }
    delete pMsModule;

    CStatic* staticModel = (CStatic*)GetDlgItem(IDC_STATIC_MODEL);
    staticModel->SetWindowText(szHW);
}

inline unsigned long CQFuseToolDlg::ConvertStringToHex(CString strHex) {
    unsigned long retVal = 0;
    LPCTSTR szHex = strHex.MakeUpper().MakeReverse().GetString();
    for(int i = 0; i < strHex.GetLength(); ++i) {
        if(szHex[i] >= '0' && szHex[i] <= '9')
            retVal += (szHex[i] - '0') * pow(16.0, i);
        else if(szHex[i] >= 'A' && szHex[i] <= 'F')
            retVal += (szHex[i] - 'A' + 10) * pow(16.0, i);
    }
    return retVal;
}

void CQFuseToolDlg::OnBnClickedMsm6ReadQfuse()
{
    // TODO: Add your control notification handler code here
    OnBnClickedGetPort();
    unsigned long reg1 = 0, reg2 = 0;
    bool bSuccess = false;
    ReadQFuseState(m_mapPortList[m_strSelectedPort], reg1, reg2, bSuccess);
    CString strTmp = _T("0");
    CEdit* pe1 = (CEdit*)GetDlgItem(IDC_EDIT1);
    pe1->SetWindowText(strTmp);
    strTmp.Format(_T("%X"), reg1);
    CEdit* pe2 = (CEdit*)GetDlgItem(IDC_EDIT2);
    pe2->SetWindowText(strTmp);
    strTmp.Format(_T("%X"), reg2);
    CEdit* pe3 = (CEdit*)GetDlgItem(IDC_EDIT3);
    pe3->SetWindowText(strTmp);
    ASSERT(bSuccess);
}

void CQFuseToolDlg::OnBnClickedMsm6WriteQfuse()
{
    // TODO: Add your control notification handler code here
    OnBnClickedGetPort();
    unsigned int nChain = 0;
    unsigned long nReg[5] = {0};
    CString strchain, strLSB, strMSB;
    CEdit* pe1 = (CEdit*)GetDlgItem(IDC_EDIT1);
    pe1->GetWindowText(strchain);
    nChain = ConvertStringToHex(strchain);
    CEdit* pe2 = (CEdit*)GetDlgItem(IDC_EDIT2);
    pe2->GetWindowText(strLSB);
    nReg[0] = ConvertStringToHex(strLSB);
    CEdit* pe3 = (CEdit*)GetDlgItem(IDC_EDIT3);
    pe3->GetWindowText(strMSB);
    nReg[1] = ConvertStringToHex(strMSB);
    WriteQFuse(m_mapPortList[m_strSelectedPort], nChain, nReg);
    Sleep(5000);
    PowerOff(m_mapPortList[m_strSelectedPort]);
    Sleep(25000);
    CheckWritedParameters();
}

void CQFuseToolDlg::OnBnClickedMsm7ReadQfuse()
{
    // TODO: Add your control notification handler code here
    //unsigned long reg1 = 0, reg2 = 0;
    //bool bSuccess = false;
    //ReadQFuseState(m_mapPortList[m_strSelectedPort], reg1, reg2, bSuccess);
    //ASSERT(bSuccess);
}

void CQFuseToolDlg::OnBnClickedMsm7WriteQfuse()
{
    // TODO: Add your control notification handler code here
    //unsigned int nChain = 0;
    //unsigned long nReg[5] = {0};
    //CString strchain, strReg0, strReg1, strReg2, strReg3, strReg4;
    //CEdit* pe1 = (CEdit*)GetDlgItem(IDC_EDIT4);
    //pe1->GetWindowText(strchain);
    //nChain = ConvertStringToHex(strchain);
    //CEdit* pe2 = (CEdit*)GetDlgItem(IDC_EDIT5);
    //pe2->GetWindowText(strReg0);
    //nReg[0] = ConvertStringToHex(strReg0);
    //CEdit* pe3 = (CEdit*)GetDlgItem(IDC_EDIT6);
    //pe3->GetWindowText(strReg1);
    //nReg[1] = ConvertStringToHex(strReg1);
    //CEdit* pe4 = (CEdit*)GetDlgItem(IDC_EDIT7);
    //pe4->GetWindowText(strReg2);
    //nReg[2] = ConvertStringToHex(strReg2);
    //CEdit* pe5 = (CEdit*)GetDlgItem(IDC_EDIT8);
    //pe5->GetWindowText(strReg3);
    //nReg[3] = ConvertStringToHex(strReg3);
    //CEdit* pe6 = (CEdit*)GetDlgItem(IDC_EDIT9);
    //pe6->GetWindowText(strReg4);
    //nReg[4] = ConvertStringToHex(strReg4);
    //WriteQFuse(m_mapPortList[m_strSelectedPort], nChain, nReg);
}

void CQFuseToolDlg::OnCbnSelchangeCombo1() {
    CComboBox *pcb = (CComboBox*)GetDlgItem(IDC_COMBO1);
    int nSel = pcb->GetCurSel();
    pcb->GetLBText(nSel, m_strSelectedPort);
}

void CQFuseToolDlg::OnBnClickedChangeFtm()
{
    // Change handset to FTM, Write NV method then reset handset
    OnBnClickedGetPort();
    ChangeFTM(m_mapPortList[m_strSelectedPort]);
    PowerOff(m_mapPortList[m_strSelectedPort]);
}

void CQFuseToolDlg::OnBnClickedChangeOnline()
{
    // Change handset to Online, Write NV method then reset handset
    OnBnClickedGetPort();
    ChangeOnline(m_mapPortList[m_strSelectedPort]);
    PowerOff(m_mapPortList[m_strSelectedPort]);
}

void CQFuseToolDlg::CheckWritedParameters() {
    unsigned long reg1 = 0, reg2 = 0;
    bool bSuccess = false;
    ReadQFuseState(m_mapPortList[m_strSelectedPort], reg1, reg2, bSuccess);
    CString strReadReg1, strReadReg2;
    if(!bSuccess) return;
    strReadReg1.Format(_T("%X"), reg1);
    strReadReg2.Format(_T("%X"), reg2);
    CString strWriteReg1, strWriteReg2;
    CEdit* pe1 = (CEdit*)GetDlgItem(IDC_EDIT2);
    CEdit* pe2 = (CEdit*)GetDlgItem(IDC_EDIT3);
    pe1->GetWindowText(strWriteReg1);
    pe2->GetWindowText(strWriteReg2);

    if(strWriteReg1 != strReadReg1 || strWriteReg2 != strReadReg2) {
        AfxMessageBox(_T("The read out QFuse parameters do not equal the writed parameters..."));
    } else {
        AfxMessageBox(_T("Write successfully..."));
    }
}