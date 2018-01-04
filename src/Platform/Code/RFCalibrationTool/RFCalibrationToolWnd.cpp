// RFCalibrationToolWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RFCalibrationTool.h"
#include "RFCalibrationToolWnd.h"
#include "..\STE\MiddleWare\CallbackTypes.h"

// CRFCalibrationToolWnd

IMPLEMENT_DYNAMIC(CRFCalibrationToolWnd, CFrameWnd)

CRFCalibrationToolWnd::CRFCalibrationToolWnd()
{
	Create(NULL, _T("RFCalibrationWnd"));
}

CRFCalibrationToolWnd::~CRFCalibrationToolWnd()
{
}


BEGIN_MESSAGE_MAP(CRFCalibrationToolWnd, CFrameWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CRFCalibrationToolWnd message handlers
void CRFCalibrationToolWnd::CreateTimer()
{
	m_nTimer = SetTimer(10, 1000, 0);
}

void CRFCalibrationToolWnd::StopTimer()
{
	KillTimer(m_nTimer);
	m_nTimer = 0;
}

void CRFCalibrationToolWnd::OnTimer(UINT_PTR nIDEvent)
{
	std::string sAbortStatus;
	CheckAbortStatus(sAbortStatus);
	if (sAbortStatus == _T("True")) {
		std::string strOutput;
		OutputData(_T("ABORT KEY PRESSED"), _T(" FAIL"), strOutput);
		((CRFCalibrationToolApp*)AfxGetApp())->RFCalTestAbort();
	}
}
