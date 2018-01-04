// GSensorCalibrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiddleWare.h"
#include "GSensorCalibrationDlg.h"
#

// CGSensorCalibrationDlg dialog

IMPLEMENT_DYNAMIC(CGSensorCalibrationDlg, CDialog)

CGSensorCalibrationDlg::CGSensorCalibrationDlg(int nPort, CWnd* pParent /*=NULL*/)
	: CDialog(CGSensorCalibrationDlg::IDD, pParent)
    ,m_nPort(nPort)
{

}

CGSensorCalibrationDlg::~CGSensorCalibrationDlg()
{
}

void CGSensorCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGSensorCalibrationDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CGSensorCalibrationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGSensorCalibrationDlg message handlers

void CGSensorCalibrationDlg::OnBnClickedOk()
{
    CODM odm(m_nPort);
    bool bRes = false;
    int nRet = 0;
    bool bRet = odm.GSensorCalibration(bRes, nRet);
    //CString strMessage = bRet ? _T("Calibration OK!!") : _T("Calibration Failed!!");
    TSTRING strOutput;
    OutputPrompt(_T("GSensor calibration"), bRes ? _T("Calibration Success!!") : _T("Calibration Failed!!"), _T("OK"), strOutput);
    //AfxMessageBox(bRet ? _T("Calibration OK!!") : _T("Calibration Failed!!"));
}
