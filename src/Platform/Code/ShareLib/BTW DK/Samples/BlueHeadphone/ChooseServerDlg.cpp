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
//
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueHeadphone.h"
#include "ChooseServerDlg.h"

static CString BdAddrToString(BD_ADDR bd_addr);

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
    m_num_services_per_device(0),
    m_start_discovery(FALSE),
	m_discovery_in_progress(FALSE),
    m_user_cancel_discovery(FALSE)
{
	//{{AFX_DATA_INIT(CChooseServerDlg)
	//}}AFX_DATA_INIT

    // Use the following events for thread notification.
  	m_hEventStopDiscoveryThread = CreateEvent(NULL, FALSE, FALSE, NULL);   // auto reset, initially reset
	m_hEventDiscoveryThreadKilled = CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset
    m_isDialogClosing = FALSE;
    m_pDiscoveryWorkerThread = NULL;
    p_Dlg = this;
}

CChooseServerDlg::~CChooseServerDlg()
{
	// The dialog owns the worker thread.
	// The destructor is responsible for killing the active worker
	// thread.

	// It's a good idea to wait for the worker thread to notify via a
	// "Thread done" event that it has killed itself. Otherwise, in the case
	// where the app is terminating, is possible (even if unlikely) that it
	// will detect a memory leak of the CWinThread object before the
	// CWinThread object has had a chance to auto-delete itself.

	DWORD dwExitCode;
	if (m_pDiscoveryWorkerThread != NULL &&
		GetExitCodeThread(m_pDiscoveryWorkerThread->m_hThread, &dwExitCode) &&
		dwExitCode == STILL_ACTIVE)
	{
        // Set flag to indicate the dialog is closing.  This is
        // used to distinguish between stopping Discovery due to
        // closing the dialog versus Discovery actually completing.
        //
        m_isDialogClosing = TRUE;

		// Kill the worker thread by setting the "Stop Discovery" 
        // thread" event.  Then wait until notified that the thread
        // was actually killed.
		SetEvent(m_hEventStopDiscoveryThread);
		WaitForSingleObject(m_hEventDiscoveryThreadKilled, 3000);
	}

  	CloseHandle(m_hEventStopDiscoveryThread);
	CloseHandle(m_hEventDiscoveryThreadKilled);
}

void CChooseServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseServerDlg)
	DDX_Control(pDX, IDC_DISCOVER_BUTTON, m_DiscoverButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseServerDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseServerDlg)
	ON_NOTIFY(NM_CLICK, IDC_SERVER_LIST, OnClickServerList)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DISCOVER_BUTTON, OnDiscoverButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseServerDlg message handlers

BOOL CChooseServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_timer_id = SetTimer (1, 500, NULL);

    // Create a column in the Server list to hold
    // the device name and address.
    //
    m_ServerList.InsertColumn(0, "Services", LVCFMT_LEFT, 1000);

    // Start Device Inquire immediately upon showing
    // dialog.  Inquiry is stopped when user clicks
    // the Connect button or the Cancel button.
    ///
    StopInquiry();
    Sleep(1000);

    if ( ! StartInquiry() )
        SetDlgItemText(IDC_STATUS_TEXT, "ERROR - Unable to start device inquiry.");
    else
        SetDlgItemText(IDC_STATUS_TEXT, "Started device inquiry.");

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

/************************************************
lab test mechanism to filter inquiry results.
Checks for desired server BDA in registry, filters results if found
************************************************/
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

	// wind down any discovery in progress
	SetEvent(m_hEventStopDiscoveryThread);
	WaitForSingleObject(m_hEventDiscoveryThreadKilled, 3000);
	
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
    POSITION pos = m_ServerList.GetFirstSelectedItemPosition();

	if ((m_discovery_in_progress) && (!m_user_cancel_discovery))
	{
	    SetDlgItemText(IDC_STATUS_TEXT, "Discovery still active, click again to cancel remaining");
        m_user_cancel_discovery = TRUE;
		*pResult = 0;
		return;
	}
    else if(m_discovery_in_progress && m_user_cancel_discovery)
    {
        // wind down any discovery in progress
        m_isDialogClosing = TRUE;
	    SetDlgItemText(IDC_STATUS_TEXT, "Stopping discovery cycle ... ");
        SetEvent(m_hEventStopDiscoveryThread);
        WaitForSingleObject(m_hEventDiscoveryThreadKilled, 3000);
	    SetDlgItemText(IDC_STATUS_TEXT, "Discovery stopped ");
    }

    m_user_cancel_discovery = FALSE;
	
    // Was an item in the list selected?
    //
    if (pos != NULL)
    {

        int nItem = m_ServerList.GetNextSelectedItem(pos);
        CBdInfo* p_Info = (CBdInfo *) m_ServerList.GetItemData(nItem);

        m_OkButton.EnableWindow( TRUE );

        // Copy the selected information into member variables.
        // This makes it available when the user clicks OK.
        memcpy(m_BdAddr, p_Info->m_BdAddr, BD_ADDR_LEN);
        memcpy(m_serviceName, p_Info->m_serviceName, sizeof(m_serviceName));
        m_Name = p_Info->m_Name;    // friendly name
        m_Scn = p_Info->m_Scn;
    }
    //
    // There is no item (server) selected so user
    // cannot Ok at this point.
    else
        m_OkButton.EnableWindow( FALSE );

	*pResult = 0;
}

void CChooseServerDlg::DoAddDevice(BD_ADDR p_bda, BD_NAME bd_name)
{
    CString item_text;
    int     item_count = m_num_devices;
    BD_ADDR     zeroBda;
     
    // check for inquiry result filter stored in registry – if filtering, discard non-matches
    memset(zeroBda, 0, BD_ADDR_LEN);
    if(memcmp(m_BdAddrFilter, zeroBda, BD_ADDR_LEN)) 
    {
        if ( memcmp(m_BdAddrFilter, p_bda, sizeof(BD_ADDR)))
            return;
        else
        {
            StopInquiry();
            OnInquiryComplete(TRUE, 1);
        }
    }

    // If the bda is a duplicate then don't add it.
    //
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

	// add server device if there is room
	if (m_num_devices >= MAX_SERVER_DEVICES - 1)
	{
		SetDlgItemText(IDC_STATUS_TEXT, "SERVER TABLE FULL " + BdAddrToString(p_bda) + " NOT ADDED");
        StopInquiry();
        OnInquiryComplete(TRUE, m_num_devices);
	}
	else
	{
		// room is available
		CBdInfo* p_Info = new CBdInfo(p_bda, bd_name);

		// add this device to the list
		m_pDevInfo[m_num_devices] = p_Info;
		m_num_devices++;
		SetDlgItemText(IDC_STATUS_TEXT, "Inquiry found " + p_Info->DeviceAsString());
		Sleep(200);
	}
    
}

void CChooseServerDlg::OnDiscoverButton() 
{
	// This procedure is really activated by the end of inquiry
    // Stop any inquiry if was started and not stopped yet.
    //
    StopInquiry();

    // Start thread to Discover Services.
    //
    ResetEvent(m_hEventStopDiscoveryThread);
    m_pDiscoveryWorkerThread = AfxBeginThread(CChooseServerDlg::DiscoverServices, this);
}	

// For each device found by inquiry (m_num_devices in m_pDevInfo), discover all services
// with our GUID. For each match , add the service name to the CBdInfo object and make a 
// new server list entry (m_ServerList) in the dialog box.
//
UINT CChooseServerDlg::DiscoverServices(LPVOID pParam) 
{
    CChooseServerDlg* p_Dlg = static_cast<CChooseServerDlg*>(pParam);
    int ii;
    int     item_count = 0;     // number of items in the list of services in dialog box

    for (ii = 0; ii < p_Dlg->m_num_devices; ii++)
    {
        CBdInfo* p_Info = p_Dlg->m_pDevInfo[ii];

        if (p_Dlg->m_isDialogClosing)
			continue;

		if (ii != 0)
			Sleep(3000);	// L2CAP needs at least 1 sec to clear previous discovery connection

        if (p_Dlg->m_isDialogClosing)
			continue;

        p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Starting discovery on BDA:  " + p_Info->DeviceAsString());

        if ( ! p_Dlg->StartDiscovery(p_Info->m_BdAddr,
                                            p_Dlg->m_pServiceGuid) )
        {
            // Discovery failed to start.  This is usually due to
            // discovery already running.
            //
            if (p_Dlg->m_isDialogClosing)
			    continue;
            p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "ERROR, Start of Discovery Failed for BDA:  " + p_Info->DeviceAsString());
            continue;
        }
        else
        {
            //
            // Wait until a) user closes dialog window (using Cancel or OK), 
            // b) the OnDiscoveryComplete method is called , or c) we timeout.
            //
            WaitForSingleObject(p_Dlg->m_hEventStopDiscoveryThread, 60000);

            // If the stop event was triggered by the dialog closing then
            // we will skip looking at the services (since discovery was
            // interrupted).
            //
            if (! p_Dlg->m_isDialogClosing)
            {
                int services_found = 0;

                p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Reading Discovery Records for BDA:  " + p_Info->DeviceAsString());

                // Get a list of service records associated with the GUID.  Note
                // the last argument is the GUID we are searching for. 
                //
                UINT16 total_services = 0;
                if(CBtIf::DISCOVERY_RESULT_SUCCESS == p_Dlg->GetLastDiscoveryResult(p_Info->m_BdAddr, &total_services) && total_services)
                {
                    services_found = p_Dlg->ReadDiscoveryRecords(p_Info->m_BdAddr, MAX_SERVICES_PER_DEVICE, p_Dlg->m_si, p_Dlg->m_pServiceGuid);

                    if (services_found == 0)
                    {
                        // try the Headset UUID - older BTW stacks translated Audio Sink records to Headset, so if the Audio Sink Discovery
                        // returns no records, can check for Headset record - the ConnectHeadphone API will work
                        GUID tmp_service_guid = CBtIf::guid_SERVCLASS_HEADSET; 
                        services_found = p_Dlg->ReadDiscoveryRecords(p_Info->m_BdAddr, MAX_SERVICES_PER_DEVICE, p_Dlg->m_si, &tmp_service_guid);
                    }
                }

                if(services_found > 0)
                {
                    UINT8			 scn;
                
                    int jj;
                    for (jj=0; jj < services_found; jj++)
                    {
                        // all services for the GUID should have a unique name
                        // add the service name to the p_info data
                        p_Dlg->m_si[jj].FindRFCommScn(&scn);
                        p_Info->m_Scn = scn;
                        if(*p_Dlg->m_pServiceGuid == guid_SERVCLASS_AUDIO_SINK)
                        {
                              strcpy(p_Info->m_serviceName, "Stereo Audio");
                        }
                        else
                            strcpy(p_Info->m_serviceName, p_Dlg->m_si[jj].m_service_name);
                        p_Dlg->m_ServerList.InsertItem(item_count, p_Info->LabelAsString());
                        p_Dlg->m_ServerList.SetItemData(item_count, (LPARAM) p_Info);
                        p_Dlg->m_ServerList.EnableWindow( TRUE );
                        item_count++;
                    }
                    
		        }

				if (! p_Dlg->m_isDialogClosing)
				{
					if (services_found)
						p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Service(s) Found on BDA:  " + p_Info->DeviceAsString());
					else
						p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "No Services Found on BDA:  " + p_Info->DeviceAsString());
				}
			}
            //
            // Discovery was stopped because dialog is closing.  Set the event
            // to say we are done.
            //
            else
            {
                SetEvent(p_Dlg->m_hEventDiscoveryThreadKilled);
            }
        }
    }

    if (item_count)
    {
        CString msg;
        msg.Format("%d services available from all devices", item_count);
        p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, msg);
    }
    else
    {
        p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "No Services Found on any devices");
        //        Sleep(3000);
    }

    p_Dlg->m_discovery_in_progress = FALSE;

	// Reset the pointer to the worker thread to indicate
    // that discovery is finished.
    // 
    p_Dlg->m_pDiscoveryWorkerThread = NULL;

    // Thread completed successfully.
    //
    return 0;
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
    p_Dlg->DoAddDevice(bda, bdName);
}

void CChooseServerDlg::OnDiscoveryComplete ()
{
    //
    // Discovery is now complete, so set event to
    // unblock the Discovery worker thread.
    //
    SetEvent(p_Dlg->m_hEventStopDiscoveryThread);
}

void CChooseServerDlg::OnInquiryComplete (BOOL success, short num_responses)
{
    CString msg;
	if (m_start_discovery || m_discovery_in_progress)
	{
    msg.Format("Redundant Inquiry Complete for %d devices", num_responses);
    p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, msg);
	}
	else
	{
    msg.Format("Starting service discovery cycle for %d devices", num_responses);
    p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, msg);
	Sleep (3000);
    m_start_discovery = TRUE;   // timer will start discovery
	}
}


void CChooseServerDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

    if (m_start_discovery)
    {
		m_start_discovery = FALSE;
		m_discovery_in_progress = TRUE;
        OnDiscoverButton();
    }
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
