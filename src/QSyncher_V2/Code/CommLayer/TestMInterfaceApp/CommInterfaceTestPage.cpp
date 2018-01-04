// CommInterfaceTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "CommInterfaceTestPage.h"
#include "../MobileInterface/Interface_Include/ICommMobile.h"

const CString DevTypeName[] = {
	_T("CDC Serial Device"),
    _T("Bluetooth Device"),
    _T("Mass Storage Device"),
    _T("Empty Device")
};

IMPLEMENT_DYNAMIC(CCommInterfaceTestPage, CDialog)

CCommInterfaceTestPage::CCommInterfaceTestPage(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CCommInterfaceTestPage::IDD, pParent)
{

}

CCommInterfaceTestPage::~CCommInterfaceTestPage()
{
}

void CCommInterfaceTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_InfoList);
}


BEGIN_MESSAGE_MAP(CCommInterfaceTestPage, CDialog)
END_MESSAGE_MAP()


// CCommInterfaceTestPage message handlers

void CCommInterfaceTestPage::UpdateInfoList()
{
   TCHAR strModel[MAX_RET_STRING_LENGTH], 
	     strIMEI[MAX_RET_STRING_LENGTH], 
		 strDisplayName[MAX_RET_STRING_LENGTH],
	     strPort[MAX_RET_STRING_LENGTH],
		 strSoftware[MAX_RET_STRING_LENGTH];
   
   int index =0;
   CString strInfo;
   CommTransDevice eTransDevice;
   int res;

   CommGetMobileInfo(theAppName, strModel, strIMEI, strDisplayName);
   CommGetConnectPort( eTransDevice, strPort );
   res = CommGetMobileSoftwareVer( strSoftware );
   
   m_InfoList.ResetContent();
   m_InfoList.InsertString(index++,_T("Mobile Interface Information"));
   strInfo.Format(_T("Model Name: %s"), strModel);
   m_InfoList.InsertString(index++,strInfo);
   strInfo.Format(_T("IMEI: %s"), strIMEI);
   m_InfoList.InsertString(index++,strInfo);
   strInfo.Format(_T("Display Name: %s"), strDisplayName);
   m_InfoList.InsertString(index++,strInfo);
   strInfo.Format(_T("Device Type: %s"), DevTypeName[eTransDevice]);
   m_InfoList.InsertString(index++,strInfo);
   strInfo.Format(_T("Connect Port: %s"), strPort);
   m_InfoList.InsertString(index++,strInfo);
   strInfo.Format(_T("Device Software Version: %s"), (res==eCommRetSuccess?strSoftware:_T("")));
   m_InfoList.InsertString(index++,strInfo);
}