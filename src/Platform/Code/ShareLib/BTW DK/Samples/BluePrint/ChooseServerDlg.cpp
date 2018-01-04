/////////////////////////////////////////////////////////////////////////////
//
//  Name        ChooseServerDlg.cpp
//  $Header:
//
//  Function    implementation of the CChooseServerDlg class.
//              
//
//  Date                         Modification
//  -----------------------------------------
//  08/30/2002    H Zong     Create
//
//  Copyright (c) 2000 - 2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BluePrint.h"
#include "ChooseServerDlg.h"
#include "PrintingDlg.h"

static CString BdAddrToString(BD_ADDR bd_addr);
CString BdNameToString(BD_NAME bd_name);
CString CreateTempFile();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChooseServerDlg * p_Dlg;
WORD axtoi(char *pch);

/////////////////////////////////////////////////////////////////////////////
// CChooseServerDlg dialog

CChooseServerDlg::CChooseServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseServerDlg::IDD, pParent),
    m_num_devices(0),
    m_num_services_per_device(0)
{
	//{{AFX_DATA_INIT(CChooseServerDlg)
	//}}AFX_DATA_INIT

    // Use the following events for thread notification.
  	m_isDialogClosing = FALSE;
    m_bDiscActive = FALSE;
    p_Dlg = this;
}

CChooseServerDlg::~CChooseServerDlg()
{
	m_isDialogClosing = TRUE;

}

void CChooseServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseServerDlg)
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerList);
	DDX_Control(pDX, IDC_PRINT, m_OkButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseServerDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseServerDlg)
	ON_NOTIFY(NM_CLICK, IDC_SERVER_LIST, OnClickServerList)
	ON_BN_CLICKED(IDC_PRINT, OnPrintButton)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseServerDlg message handlers

BOOL CChooseServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    // Create a column in the Server list to hold
    // the device name and address.
    //
    m_ServerList.InsertColumn(0, "Services", LVCFMT_LEFT, 1000);

    // Start Device Inquire immediately upon showing
    // dialog.  Inquiry is stopped when user clicks
    // the Connect button or the Cancel button.
    ///
    StopInquiry();
    Sleep(100);

    if ( ! StartInquiry() )
        SetDlgItemText(IDC_STATUS_TEXT, "ERROR - Unable to start device inquiry.");

    // optional BDA filter for lab testing with large number of devices responding to inquiry
    GetBDAFilterFromReg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChooseServerDlg::string2BDAddr(BD_ADDR bdAddr, char *lpbdAddrString)
{
	if ( strlen(lpbdAddrString) != (BD_ADDR_LEN * 3 - 1))
		return FALSE;

	for (int i = 0; i<BD_ADDR_LEN; i++)
		bdAddr[i] = (char)axtoi(&lpbdAddrString[i *3]);
	return TRUE;

}

/*************************************************************
lab test mechanism to filter inquiry results.  
Checks for desired server BDA in registry, filters results if found
*************************************************************/
void CChooseServerDlg::GetBDAFilterFromReg()
{
    HKEY   hKey;
    DWORD  dwResult;
		
    memset(m_BdAddrFilter, 0, BD_ADDR_LEN);

    if ((dwResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Widcomm\\BtConfig\\Debug",
                    0, KEY_READ|KEY_WOW64_64KEY, &hKey)) == ERROR_SUCCESS)
    {
        TCHAR szFilter[256];
        DWORD dwBytes = 256;
        if (ERROR_SUCCESS == RegQueryValueEx (hKey, "SDKSampleTargetDevice", NULL, NULL, (LPBYTE)&szFilter, &dwBytes))
        {
            string2BDAddr(m_BdAddrFilter, szFilter);
        }
        RegCloseKey(hKey);
    }

}

void CChooseServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();

    // User may have clicked 'Cancel' button after starting inquiry.  
    // Need to stop any inquiry if was started and not stopped yet.
    //
    StopInquiry();

	//
    // Need to get rid of the item data.
    //
    int     item_count = m_num_devices;

    for (int x=0; x < item_count; x++)
    {
        CBdInfo* p_Info = m_pDevInfo[x];

        if (p_Info)
            delete p_Info;
    }
    m_ServerList.DeleteAllItems();
}

void CChooseServerDlg::OnClickServerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	StopInquiry();
    POSITION pos = m_ServerList.GetFirstSelectedItemPosition();

	// Was an item in the list selected?
    //
    if (pos != NULL)
    {
        int nItem = m_ServerList.GetNextSelectedItem(pos);
        CBdInfo* p_Info = (CBdInfo *) m_ServerList.GetItemData(nItem);
		memcpy(m_BdAddr, p_Info->m_BdAddr, BD_ADDR_LEN);
		memcpy(m_BdName, p_Info->m_Name, BD_NAME_LEN);
		if ( TRUE == DiscoverSvc())
			m_OkButton.EnableWindow( TRUE );
		else
			m_OkButton.EnableWindow( FALSE );
    }
    
	*pResult = 0;
}

void CChooseServerDlg::DoAddDevice(BD_ADDR p_bda, BD_NAME bd_name, DEV_CLASS dev_class)
{
    CString item_text;
    int     item_count = m_num_devices;

    BYTE major_dev_class = 0;
    BYTE minor_dev_class = 0;
    major_dev_class = dev_class[1] & MAJOR_DEV_CLASS_MASK;
    minor_dev_class = dev_class[2] & MINOR_DEV_CLASS_MASK;
    BYTE service_class = dev_class[0];  // don't care about masks in high 3 bits of dev_class[1]

    SetDlgItemText(IDC_STATUS_TEXT, "Inquiry found " + BdNameToString(bd_name));

    //if it is printer, add it to list box
    if (major_dev_class != MAJOR_DEV_CLASS_IMAGING || minor_dev_class != MINOR_DEV_CLASS_IMAGING_PRINTER)
        return;
#if 0
    // if you want to include devices with the rendering bit
    // it will allow multi-function devices (e.g. PC servers) to appear, but
    // also gets a lot of unwanted stuff due to AV devices also setting rendering bit
    if ((major_dev_class != MAJOR_DEV_CLASS_IMAGING || minor_dev_class != MINOR_DEV_CLASS_IMAGING_PRINTER)
        && !(service_class & SERV_CLASS_RENDERING))
        return;
#endif

    // check for inquiry result filter stored in registry – if filtering, discard non-matches
    BD_ADDR     zeroBda;
    memset(zeroBda, 0, BD_ADDR_LEN);
    if(memcmp(m_BdAddrFilter, zeroBda, BD_ADDR_LEN)) 
    {
        if ( memcmp(m_BdAddrFilter, p_bda, sizeof(BD_ADDR)))
            return;
        else
        {
            StopInquiry();
        }
    }

    // If the bda is a duplicate then don't add it.
    // update it
    for (int x=0; x < m_num_devices; x++)
    {
        CBdInfo* p_CmpInfo = (CBdInfo *) m_pDevInfo[x];

        // Is it a duplicate?
        if ( p_CmpInfo->isBdAddrEqual(p_bda))
        {
            // Yes! If we don't have a name
            // yet and this time we were given
            // one, update the info and reset the
            // item text.
            if (p_CmpInfo->m_Name.GetLength() == 0)
            {
                p_CmpInfo->m_Name = bd_name;
                m_ServerList.SetItemText(x, 0, p_CmpInfo->LabelAsString());
            }

            return;
        }
    }

    if (m_num_devices >= MAX_SERVER_DEVICES - 1)
    {
        SetDlgItemText(IDC_STATUS_TEXT, "SERVER TABLE FULL " + BdAddrToString(p_bda) + " NOT ADDED");
        StopInquiry();
        Sleep(500);
    }
    else
    {
        //new ones, add it
        CBdInfo* p_Info = new CBdInfo(p_bda, bd_name);
        m_pDevInfo[m_num_devices] = p_Info;
        m_num_devices++;

        // Insert the new item, add the BD info as 'item data'
        // and enable the Server List window so user can
        // select a device to discover.
        //
        m_ServerList.InsertItem(item_count, p_Info->LabelAsString());
        m_ServerList.SetItemData(item_count, (LPARAM) p_Info);
        m_ServerList.EnableWindow( TRUE );
    }
    
}

///////////////////////////////////////////////////////////////////////////////////////////////
// methods to replace virtual methods in base class CBtIf 
//

void CChooseServerDlg::OnDeviceResponded (BD_ADDR bda, DEV_CLASS devClass, BD_NAME bdName, BOOL bConnected)
{
    

    // Add the device address and name (if any)
    // to the Server List.  It is OK to pass
    // duplicates.  This method will ignore them.
    //

    p_Dlg->DoAddDevice(bda, bdName, devClass);
}


void CChooseServerDlg::OnInquiryComplete (BOOL success, short num_responses)
{
	SetDlgItemText(IDC_STATUS_TEXT, "Inquiry Complete.");
	return;
    
}


BOOL CChooseServerDlg :: DiscoverSvc ()
{
	// Discover the printing service (this takes a few seconds) and get a service channel number (scn).
	CWaitCursor			wc;
    CSdpDiscoveryRec    sdr;
	CButton*            pbButton = NULL;

	SetDlgItemText(IDC_STATUS_TEXT, "Start Discovery...");
	UpdateWindow();
	m_bDiscActive = TRUE;
	GUID bpp_service_guid  = CBtIf::guid_SERVCLASS_BPP_PRINTING;
	
    if (!StartDiscovery(m_BdAddr, NULL))
    {
		TCHAR	szMsg[256];

		_stprintf(szMsg, ("%S\n\nUnable to initiate service discovery."), m_BdName);
		SetDlgItemText(IDC_STATUS_TEXT, szMsg);
		
        m_bDiscActive = FALSE;
        return (FALSE);
    }

	int	iLoop = 0;

    // Wait up to 60 seconds for service discovery to complete
    while (m_bDiscActive && ++iLoop < 600)
        Sleep(100);

	if (m_bDiscActive)
	{
		TCHAR	szMsg[256];

		_stprintf(szMsg, _T("%S\n\nError discovering services on the selected device."), m_BdName);
		AfxMessageBox(szMsg);
		return FALSE;
	}


    // check to see the which profiles the printer server suuports:
    // PRINT_PROFILE_HCRP, PRINT_PROFILE_SPP, and PRINT_PROFILE_BPP
    // 
    // First, we'll see what the printer supports - for every profile
    // it supports, enable the checkbox.  For each profile, disable the 
    // control if not supported.  The user can then select/deselect from the
    // available profiles.  If multiple checkboxes left enabled, the preferred
    // order will be HCRP, SPP, and BPP.

    pbButton = (CButton*)GetDlgItem(IDC_BPP);
    if (ReadDiscoveryRecords(m_BdAddr, 1, &sdr, &bpp_service_guid))
    {
        pbButton->EnableWindow(TRUE);
        SetDlgItemText(IDC_STATUS_TEXT, "support PRINT_PROFILE_BPP");
        pbButton->SetCheck(TRUE);
        m_ProfileToUse = CPrintClient::PRINT_PROFILE_BPP;
    }
    else
    {
        pbButton->SetCheck(FALSE);
        pbButton->EnableWindow(FALSE);
    }

    pbButton = (CButton*)GetDlgItem(IDC_SPP);
    if (ReadDiscoveryRecords(m_BdAddr, 1, &sdr, (GUID *)(&CBtIf::guid_SERVCLASS_SPP_PRINTING)))
    {
        pbButton->EnableWindow(TRUE);
        SetDlgItemText(IDC_STATUS_TEXT, "support PRINT_PROFILE_SPP");
        pbButton->SetCheck(TRUE);
        m_ProfileToUse = CPrintClient::PRINT_PROFILE_SPP;
    }
    else
    {
        pbButton->SetCheck(FALSE);
        pbButton->EnableWindow(FALSE);
    }

    pbButton = (CButton*)GetDlgItem(IDC_HCRP);
    if (ReadDiscoveryRecords(m_BdAddr, 1, &sdr, (GUID *)(&CBtIf::guid_SERVCLASS_HCRP_PRINTING)))
    {
        pbButton->EnableWindow(TRUE);
        SetDlgItemText(IDC_STATUS_TEXT, "support PRINT_PROFILE_HCRP");
        pbButton->SetCheck(TRUE);
        m_ProfileToUse = CPrintClient::PRINT_PROFILE_HCRP;
    }
    else
    {
        pbButton->SetCheck(FALSE);
        pbButton->EnableWindow(FALSE);
    }

    // any not checked & enabled at this point are not supported by the printer

    return TRUE;
}

void CChooseServerDlg::OnDiscoveryComplete()
{
        m_bDiscActive = FALSE;
		SetDlgItemText(IDC_STATUS_TEXT, "Select Print to Print");
}

void CChooseServerDlg::OnPrintButton() 
{
		CString szFileToPrint = CreateTempFile();

        // let's use the checkboxes
        // Go in order, if HCRP is checked, use it first, then SPP, then BPP
        CButton *pbButton = pbButton = (CButton*)GetDlgItem(IDC_HCRP);
        if(pbButton->GetCheck())
            m_ProfileToUse = CPrintClient::PRINT_PROFILE_HCRP;
        else
        {
            pbButton = pbButton = (CButton*)GetDlgItem(IDC_SPP);
            if(pbButton->GetCheck())
                m_ProfileToUse = CPrintClient::PRINT_PROFILE_SPP;
            else
            {
                pbButton = pbButton = (CButton*)GetDlgItem(IDC_BPP);
                if(pbButton->GetCheck())
                    m_ProfileToUse = CPrintClient::PRINT_PROFILE_BPP;
            }    
        }

		CPrintingDlg Dlg(m_ProfileToUse, m_BdAddr, szFileToPrint);
		Dlg.DoModal();
}	

///////////////////////////////////////
////////// Utility Functions //////////
///////////////////////////////////////

// Return a CString based on a
//  a BD Address (BD_ADDR type).
//
CString BdAddrToString(BD_ADDR bd_addr)
{
    CString s;

    s.Format("%x:%x:%x:%x:%x:%x",
             bd_addr[0],
             bd_addr[1],
             bd_addr[2],
             bd_addr[3],
             bd_addr[4],
             bd_addr[5]);

    return s;
}

CString BdNameToString(BD_NAME bd_name)
{
    CString s;

    s.Format(_T("%s"), bd_name);
             

    return s;
}

CString CreateTempFile()
{
CString s;
char szTempName[MAX_PATH];
DWORD dwBytes;
HANDLE hTempFile;

	GetTempPath(MAX_PATH, szTempName);
	strcat(szTempName, "Widcomm.txt");
	hTempFile = CreateFile(szTempName, GENERIC_WRITE, 
				0, (LPSECURITY_ATTRIBUTES)NULL, 
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if(hTempFile != INVALID_HANDLE_VALUE)
	{
		char szBuffer[MAX_PATH];
		
		for (int i = 1; i < 6; i++)
		{
			sprintf(szBuffer, "%d line test print DK\r\n", i);
			WriteFile( hTempFile, szBuffer,
					lstrlen(szBuffer), &dwBytes,NULL);
		}

		//write form feed
		szBuffer[0] = 0x0c;
		WriteFile( hTempFile, szBuffer, 1, &dwBytes,NULL);
	    CloseHandle(hTempFile); 
	}

	s.Format(_T("%s"), szTempName);
	return s;
}
#define CHAR_0 '0'
#define CHAR_CAP_A 'A'
#define CHAR_CAP_F 'F'
#define CHAR_A 'a'
#define CHAR_F 'f'

WORD axtoi(char *pch)
{
register char ch;
register WORD n = 0;

	while((ch = *pch++) != 0)
	{
		if (isdigit(ch))
			ch -= CHAR_0;
		else if (ch >= CHAR_CAP_A && ch <= CHAR_CAP_F)
			ch += (char)(10 - CHAR_CAP_A);
		else if (ch >= CHAR_A && ch <= CHAR_F)
			ch += (char)(10 - CHAR_A);
		else 
            break; //ch = (char)0;
		n = 16 * n + ch;
     }
	return n;
}
