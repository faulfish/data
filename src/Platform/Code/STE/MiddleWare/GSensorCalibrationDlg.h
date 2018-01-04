#pragma once
#include "Resource.h"
#include "..\..\Modules\ODMCoachModule\ODM.h"
#include "CallbackTypes.h"
// CGSensorCalibrationDlg dialog

class CGSensorCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CGSensorCalibrationDlg)

public:
	CGSensorCalibrationDlg(int nPort, CWnd* pParent = NULL);   // standard constructor
	virtual ~CGSensorCalibrationDlg();

// Dialog Data
	enum { IDD = IDD_GSENSORCALIBRATIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
    int m_nPort;
public:
    afx_msg void OnBnClickedOk();
};
