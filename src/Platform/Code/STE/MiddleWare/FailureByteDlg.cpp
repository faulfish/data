// FailureByte.cpp : implementation file
//

#include "stdafx.h"
#include "MiddleWare.h"
#include "FailureByteDlg.h"

IMPLEMENT_DYNAMIC(CFailureByteDlg, CDialog)

CFailureByteDlg::CFailureByteDlg(int nPort, int nType, CWnd* pParent /*=NULL*/)
	: CDialog(CFailureByteDlg::IDD, pParent)
	,m_nPort(nPort), m_pOdm(NULL), m_nType(nType), m_FailureCount(0)
{
    CreateODMInstance();
}

CFailureByteDlg::~CFailureByteDlg()
{
    if(m_pOdm)
        delete m_pOdm;
    m_pOdm = NULL;
}

void CFailureByteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFailureByteDlg, CDialog)
	ON_BN_CLICKED(IDC_Add, &CFailureByteDlg::OnBnClickedAdd)
END_MESSAGE_MAP()

BOOL CFailureByteDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	ReadFailureByte();
	
	return TRUE;
}

void CFailureByteDlg::CreateODMInstance() {
    if(m_pOdm == NULL)
        m_pOdm = new CODM(m_nPort);
}

char CFailureByteDlg::ReadFailureByte()
{
	char cFailureByte = -1;
    bool bRes = false;
    switch(m_nType) {
        case 0:
            bRes = m_pOdm->GetMotoXPRSData(m_MotoXprsData);
            cFailureByte = m_MotoXprsData.cFailByteId;
            break;
        case 1:
            bRes = m_pOdm->GetMotoXPRSData(m_MotoXprsData_Lavernock);
            cFailureByte = m_MotoXprsData_Lavernock.cFailByteId;
            break;
    }

	if(bRes)
	{
		CString strFormat;
        if(cFailureByte == 0)
            strFormat = _T("Current failure byte is 0");
        else if(cFailureByte == 1)
            strFormat = _T("Current failure byte is 01");
        else if(cFailureByte == 2)
            strFormat = _T("Current failure byte is 11");
		CStatic * pStatic =  (CStatic *)GetDlgItem(IDC_DISPLAY);
		if(pStatic)
			pStatic->SetWindowText(strFormat);
	}

	return m_FailureCount = cFailureByte;
}

void CFailureByteDlg::OnBnClickedAdd()
{	
	bool bRes = false;
	
	if(m_FailureCount >= 0 && m_FailureCount < 2)
	{
        bRes = m_pOdm->WriteFailureByte(m_FailureCount+1, m_nType);
    }


    ReadFailureByte();
}
