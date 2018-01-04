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
//  04/12/2002    H Zong    Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueAudio.h"
#include "ChooseServerDlg.h"
WORD axtoi(char *pch);

/////////////////////////////////////
//
// Handy class for holding Device
// information held in the item list.
//
class CBdInfo
{
public:

    CBdInfo(BD_ADDR bd_addr, BD_NAME bd_name) : m_Scn(0), m_isServer(FALSE), m_Name(bd_name) { memcpy (m_BdAddr, bd_addr, BD_ADDR_LEN); }
    ~CBdInfo() { ; }
    BOOL isBdAddrEqual(BD_ADDR bd_addr) { return (0 == memcmp(m_BdAddr, bd_addr, sizeof(BD_ADDR))); }
    CString DeviceAsString()
    {
        CString s;

        // Default the string to the BD Address only.
        //
        s.Format("[%x:%x:%x:%x:%x:%x]", m_BdAddr[0], m_BdAddr[1], m_BdAddr[2], m_BdAddr[3], m_BdAddr[4], m_BdAddr[5]);

        // If friendly name provided then prepend it.
        //
        if (m_Name.GetLength())
            s = m_Name + " " + s;

        return s;
    }
    CString LabelAsString()
    {
        CString label(DeviceAsString());

        // If the device is a Server then add
        // an indicator.
        //
        if (m_isServer)
            label += "  <- Audio Server";

        return label;
    }

    BD_ADDR m_BdAddr;
    CString m_Name;
    UINT8 m_Scn;
    BOOL m_isServer;
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChooseServerDlg * p_Dlg;

/////////////////////////////////////////////////////////////////////////////
// CChooseServerDlg dialog

CChooseServerDlg::CChooseServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseServerDlg::IDD, pParent)
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
		WaitForSingleObject(m_hEventDiscoveryThreadKilled, INFINITE);
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
    m_ServerList.InsertColumn(0, "Server", LVCFMT_LEFT, 350);

    // Start Device Inquire immediately upon showing
    // dialog.  Inquiry is stopped when user clicks
    // the Discovery button or the Cancel button.
    ///
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

/***************************************************************************
 lab test mechanism to filter inquiry results.  
 Checks for desired server BDA in registry, filters results if found
***************************************************************************/
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
    int     item_count = m_ServerList.GetItemCount();

    for (int x=0; x < item_count; x++)
    {
        CBdInfo* p_Info = (CBdInfo *) m_ServerList.GetItemData(x);

        if (p_Info)
            delete p_Info;
    }
    m_ServerList.DeleteAllItems();
}

void CChooseServerDlg::OnClickServerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    POSITION pos = m_ServerList.GetFirstSelectedItemPosition();

    // Was an item in the list selected?
    //
    if (pos != NULL)
    {
        // An item is selected, enable the 'Discover' button
        // only if it is currently not discovering.
        if (m_pDiscoveryWorkerThread == NULL)
            m_DiscoverButton.EnableWindow( TRUE );

        // Only enable the OK button if the device that was
        // selected is a Server.  This tells us that discovery 
        // has been done on the device and a Audio Service was 
        // found.
        //
        int nItem = m_ServerList.GetNextSelectedItem(pos);
        CBdInfo* p_Info = (CBdInfo *) m_ServerList.GetItemData(nItem);

        if (p_Info->m_isServer)
        {
            m_OkButton.EnableWindow( TRUE );

            // Copy the selected information into member variables.
            // This makes it available when the user clicks OK.
            memcpy(m_BdAddr, p_Info->m_BdAddr, BD_ADDR_LEN);
            m_Scn = p_Info->m_Scn;
        }
        //
        // The selected device does not have a discovered
        // service.  So user cannot pick this server.
        else
            m_OkButton.EnableWindow( FALSE );
    }
    //
    // There is no item (server) selected so user
    // cannot Discover at this point.
    else
        m_DiscoverButton.EnableWindow( FALSE );
	
	*pResult = 0;
}

void CChooseServerDlg::DoAddDevice(BD_ADDR p_bda, BD_NAME bd_name)
{
    CString item_text;
    int     item_count = m_ServerList.GetItemCount();
    BD_ADDR     zeroBda;

    // check for inquiry result filter stored in registry – if filtering, discard non-matches
    memset(zeroBda, 0, BD_ADDR_LEN);
    if(memcmp(m_BdAddrFilter, zeroBda, BD_ADDR_LEN)) 
    {
        if ( memcmp(m_BdAddrFilter, p_bda, sizeof(BD_ADDR)))
            return;
        else
            StopInquiry();
    }

        // If it is a duplicate then don't add it.
        //
        for (int x=0; x < item_count; x++)
        {
            CBdInfo* p_CmpInfo = (CBdInfo *) m_ServerList.GetItemData(x);

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


    CBdInfo* p_Info = new CBdInfo(p_bda, bd_name);

    // Insert the new item, add the BD info as 'item data'
    // and enable the Server List window so user can
    // select a device to discover.
    //
    m_ServerList.InsertItem(item_count, p_Info->LabelAsString());
    m_ServerList.SetItemData(item_count, (LPARAM) p_Info);
    m_ServerList.EnableWindow( TRUE );

    SetDlgItemText(IDC_STATUS_TEXT, "Select a device and click Discover to find Audio service");
}

void CChooseServerDlg::OnDiscoverButton() 
{
    // Stop any inquiry if was started and not stopped yet.
    //
    StopInquiry();

    // Disable the 'Discover' button since you
    // can't do multiple discoveries at the same
    // time.
    //
    m_DiscoverButton.EnableWindow( FALSE );

    // Start thread to Discover Services.
    //
    ResetEvent(m_hEventStopDiscoveryThread);
    m_pDiscoveryWorkerThread = AfxBeginThread(CChooseServerDlg::DiscoverServices, this);
}	

UINT CChooseServerDlg::DiscoverServices(LPVOID pParam) 
{
    CChooseServerDlg* p_Dlg = static_cast<CChooseServerDlg*>(pParam);

    //
    // Get the currently selected device and do discovery
    //
    POSITION pos = p_Dlg->m_ServerList.GetFirstSelectedItemPosition();

    if (pos != NULL)
    {
        int nItem = p_Dlg->m_ServerList.GetNextSelectedItem(pos);
        CBdInfo* p_Info = (CBdInfo *) p_Dlg->m_ServerList.GetItemData(nItem);

        p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Starting discovery on BDA:  " + p_Info->DeviceAsString());

        if ( ! p_Dlg->StartDiscovery(p_Info->m_BdAddr,
                                            p_Dlg->m_pServiceGuid) )
        {
            // Discovery failed to start.  This is usually due to
            // discovery already running.
            //
            p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "ERROR, Start of Discovery Failed for BDA:  " + p_Info->DeviceAsString());
        }
        else
        {
            //
            // Wait until a) user closes dialog window (using Cancel or OK), 
            // b) the OnDiscoveryComplete method is called , or c) we timeout.
            //
            WaitForSingleObject(p_Dlg->m_hEventStopDiscoveryThread, 30000);

            // If the stop event was triggered by the dialog closing then
            // we will skip looking at the services (since discovery was
            // interrupted).
            //
            if (! p_Dlg->m_isDialogClosing)
            {
    		    // Get list of services found
	    	    //CSdpDiscoveryRec SdpRecs[30];

                p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Reading Discovery Records for BDA:  " + p_Info->DeviceAsString());

                // Reset the flag so that even if it was a server
                // it may not be now.
                //
                p_Info->m_isServer = FALSE;

                // Get the service record associated with the GUID.  Note
                // the last argument is the GUID we are searching for.  We
                // could get a list of records instead and parse them ourselves.
                //
                CSdpDiscoveryRec Sdp_Record;

                if ( p_Dlg->ReadDiscoveryRecords(p_Info->m_BdAddr, 1, &Sdp_Record, p_Dlg->m_pServiceGuid) )
                {
                    UINT8			 scn;
                
                    if ( Sdp_Record.FindRFCommScn(&scn) )
                    {
                        p_Info->m_Scn = scn;
                    
                        p_Info->m_isServer = TRUE;

			    	    // User has this server already selected, so
                        // enable the OK button.
                        //
                        p_Dlg->m_OkButton.EnableWindow( TRUE );
                    
                        // Copy the selected information into member variables.
                        // This makes it available when the user clicks OK.
                        //
                        memcpy(p_Dlg->m_BdAddr, p_Info->m_BdAddr, BD_ADDR_LEN);
                        p_Dlg->m_Scn = scn;
                    }
		        }

                // Reset the item label.  If this is
                // a server then it will be indicated.
                //
                p_Dlg->m_ServerList.SetItemText(nItem, 0, p_Info->LabelAsString());

		        if (p_Info->m_isServer)
			        p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Service Found on BDA:  " + p_Info->DeviceAsString());
		        else
			        p_Dlg->SetDlgItemText(IDC_STATUS_TEXT, "Service Not Found on BDA:  " + p_Info->DeviceAsString());
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

    // If user is not closing the dialog and an item
    // in the list is selected, re-enable the Discover button.
    //
    if (! p_Dlg->m_isDialogClosing && p_Dlg->m_ServerList.GetFirstSelectedItemPosition() != NULL)
        p_Dlg->m_DiscoverButton.EnableWindow( TRUE );

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
