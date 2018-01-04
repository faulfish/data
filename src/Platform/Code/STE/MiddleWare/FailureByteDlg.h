#pragma once
#include "resource.h"
#include "..\..\Modules\ODMCoachModule\ODM.h"


typedef enum _PLATFORM {
    PLATFORM_6240,
    PLATFORM_6245,
    PLATFORM_6270,
    PLATFORM_UNKNOWN
} PLATFORM;

class CFailureByteDlg : public CDialog
{
	DECLARE_DYNAMIC(CFailureByteDlg)

public:
	CFailureByteDlg(int nPort, int nType, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFailureByteDlg();

// Dialog Data
	enum { IDD = IDD_FailureByte };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	char ReadFailureByte();

private:
    void CreateODMInstance();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	int m_nPort;
	opxprofile_data m_MotoXprsData;
    opxprofile_data_Lavernock m_MotoXprsData_Lavernock;
    opxprofile_data_Melos m_MotoXprsData_Melos;
private:
    CODM* m_pOdm;
    int m_nType;
    int m_FailureCount;
};
