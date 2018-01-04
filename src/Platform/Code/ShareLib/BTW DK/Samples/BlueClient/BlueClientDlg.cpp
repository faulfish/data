// BlueClientDlg.cpp : implementation file
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
#include "stdlib.h"
#include "BlueClient.h"
#include "BlueClientDlg.h"
#include "btversinfo.h"

// application wide definitions
//
#define	SZ_OBJECT_EXCHANGE	"Object Exchange"
#define	SZ_FILE_TRANSFER	"File Transfer"

// SECURITY DEFAULT SETTINGS
static bool FtpAuthentication = FALSE;
static bool FtpEncryption = FALSE;
static bool OppAuthentication = FALSE;
static bool OppEncryption = FALSE;

#define	FTP_ENABLED
static bool bAutomaticDiscoveryChanged = FALSE;
static WCHAR szInboxDirectory[] = L"Not usded"; //use the dir saved in registry
static WCHAR szDefaultBusinessCard[MAX_PATH];
static WCHAR szFTPInbox[MAX_PATH];
static WCHAR szFTPRoot[MAX_PATH];

#define	KEY_GENERAL	_T("General")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WM_CONFIG_CHANGED       (WM_USER + 0xA1)

///////////////////////////////////////////////////////////////////

CBlueClientDlg * p_Dlg;
WORD axtoi(char *pch);
///////////////////////////////////////////////////////////////////////////////////
// methods to replace virtual methods in base class CBtIf 
//

void CBlueClientDlg::OnDeviceResponded (BD_ADDR p_bda, DEV_CLASS dev_class, BD_NAME bd_name, BOOL bConnected)
{
    CDeviceInfo *pDeviceInfo;
	CString sbText;

    // If window was closed, ignore trailing responses
    if (!m_hWnd)
        return;

    // check for inquiry result filter stored in registry – if filtering, discard non-matches
    BD_ADDR     zeroBda;
    memset(zeroBda, 0, BD_ADDR_LEN);
    if(memcmp(m_BdAddrFilter, zeroBda, BD_ADDR_LEN)) 
    {
        if ( memcmp(m_BdAddrFilter, p_bda, sizeof(BD_ADDR)))
            return;
        else
            StopInquiry();
    }


    // Check if this is update for known device
    for (pDeviceInfo = p_Dlg->pFirstDeviceInfo; pDeviceInfo; pDeviceInfo = pDeviceInfo->pNext)
    {
        if (!memcmp (pDeviceInfo->bd_addr, p_bda, BD_ADDR_LEN))
            break;
    }

    BOOL new_item = FALSE;

    if (!pDeviceInfo)
    {
        new_item = TRUE;
		
        pDeviceInfo = new CDeviceInfo (p_bda, dev_class, bd_name);
        if (!p_Dlg->pFirstDeviceInfo)
            p_Dlg->pFirstDeviceInfo = pDeviceInfo;
        else
        {	CDeviceInfo *p;
            for ( p = p_Dlg->pFirstDeviceInfo; p->pNext; p = p->pNext)
                ;
            p->pNext = pDeviceInfo;
        }
    }

    CString s = (CString)bd_name;

    if ((strlen ((char *)bd_name) > 1) && (bd_name[0] > ' '))
	{
		if (strlen((char *)pDeviceInfo->bd_name)==0)
		{
			//display the name found on the status bar
			sbText.Format("Device name update: %s",s);
			p_Dlg->m_statusBar.SetWindowText(sbText);
		}

		strcpy((char *)(pDeviceInfo->bd_name),(const char *)bd_name);
        s.Format ("%s", bd_name);

	}
    else
        s.Format ("%02X:%02X:%02X:%02X:%02X:%02X", p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    if (bConnected)
        s += _T(" (Connected)");

	if (new_item)
	{
		// display the newly discovered device on the status bar
		sbText.Format("New device found:  %s",s);
		p_Dlg->m_statusBar.SetWindowText(sbText);
	}

    int id = IMG_DEVICE_BT_UNKNOWN;

    BYTE service         = dev_class[0];
    BYTE major_dev_class = dev_class[1];
    BYTE minor_dev_class = dev_class[2] & ~0x03;

    switch (major_dev_class)
    {
    case MAJOR_DEV_CLASS_COMPUTER:
        switch (minor_dev_class)
        {
        case MINOR_DEV_CLASS_COMP_LAPTOP:
            id = IMG_DEVICE_LAPTOP;
            break;
        case MINOR_DEV_CLASS_COMP_HANDHELD:
        case MINOR_DEV_CLASS_COMP_PALM:
            id = IMG_DEVICE_PDA;
            break;
        default:
            id = IMG_DEVICE_DESKTOP;
            break;
        }
        break;
    case MAJOR_DEV_CLASS_PHONE:
        switch (minor_dev_class)
        {
        case MINOR_DEV_CLASS_PHONE_MODEM:
            id = IMG_DEVICE_MODEM;
            break;
        default:
            id = IMG_DEVICE_CELL_PHONE;
            break;
        }
        break;
    case MAJOR_DEV_CLASS_LAN_ACCESS:
        id = IMG_DEVICE_LAN;
        break;
    }

    if (new_item)
    {
        pDeviceInfo->m_item = p_Dlg->m_tree.InsertItem(s, id, id, p_Dlg->m_hTreeItemRoot);
        p_Dlg->m_tree.SetItemData(pDeviceInfo->m_item, (DWORD_PTR)pDeviceInfo);

        p_Dlg->m_tree.Expand(p_Dlg->m_hTreeItemRoot, TVE_EXPAND);
    }
    else
    {
        p_Dlg->m_tree.SetItemImage(pDeviceInfo->m_item, id, id);
        p_Dlg->m_tree.SetItemText(pDeviceInfo->m_item, s);
    }

    p_Dlg->m_tree.SortChildren(p_Dlg->m_hTreeItemRoot);
}

void CBlueClientDlg::OnInquiryComplete (BOOL success, short num_responses)
{
		CString s;
		s.Format("Inquiry Complete -  %s, %d devices found",
            success ?  "success"  : "failed", num_responses);
		p_Dlg->m_statusBar.SetWindowText(s);
		p_Dlg->SetWaitCursor(FALSE);

}

void CBlueClientDlg::OnDiscoveryComplete (UINT16 nRecs, long lResultCode)
{
	CDeviceInfo *pDeviceInfo = NULL;
    if (p_Dlg->m_hWnd)
    {
		//this stuff is all to update the ftp/opp icons displayed below the device
		{
			
			for (pDeviceInfo = CBlueClientDlg::pFirstDeviceInfo; pDeviceInfo; pDeviceInfo = pDeviceInfo->pNext)
			{
				if (!memcmp (pDeviceInfo->bd_addr, p_Dlg->m_pDeviceSelected->bd_addr, BD_ADDR_LEN))
					break;
			}

			if (pDeviceInfo)
			{

				//start from the just below the root...
				HTREEITEM hItem = p_Dlg->m_tree.GetRootItem();
				if (hItem)
				{
					CString s;

					//loop through the devices...
					HTREEITEM hTempItem = p_Dlg->m_tree.GetChildItem(hItem);
					while(hTempItem)
					{
						s = p_Dlg->m_tree.GetItemText(hTempItem);
			            CDeviceInfo *pTreeDeviceInfo = (CDeviceInfo *)p_Dlg->m_tree.GetItemData(hTempItem);
                        
						//the right device needs the bdname and bdaddr to match
						if (0==strncmp((char *)pDeviceInfo->bd_name,(LPCTSTR)s,strlen((char *)pDeviceInfo->bd_name)) &&
							0==memcmp(p_Dlg->m_pDeviceSelected->bd_addr,pDeviceInfo->bd_addr,sizeof(BD_ADDR_LEN)))
						{
							hItem = p_Dlg->m_tree.GetChildItem(hTempItem);
							while(hItem)
							{
								hTempItem = p_Dlg->m_tree.GetNextSiblingItem(hItem);
								s = p_Dlg->m_tree.GetItemText(hItem);
								if ((0 != strcmp(SZ_FILE_TRANSFER,s)) && (0 != strcmp(SZ_OBJECT_EXCHANGE,s)))
									break;
								p_Dlg->m_tree.DeleteItem(hItem);
								hItem = hTempItem;
							}
							break;
						}
						else
						{
							hTempItem = p_Dlg->m_tree.GetNextSiblingItem(hTempItem);
						}
					}
				}

                CSdpDiscoveryRec  si[CBlueClientDlg::MAX_SERVICES]; // just for display purposes

				GetAvailableServices (pDeviceInfo->bd_addr, &pDeviceInfo->m_nServices, CBlueClientDlg::MAX_SERVICES, si);
				for (int i = 0; i < pDeviceInfo->m_nServices; i++)
				{
                    if (si[i].m_service_guid == guid_SERVCLASS_OBEX_FILE_TRANSFER)
                    {
						HTREEITEM item = p_Dlg->m_tree.InsertItem(SZ_FILE_TRANSFER, IMG_BT_FOLDER, IMG_BT_FOLDER_OPEN, pDeviceInfo->m_item);
						p_Dlg->m_tree.SetItemData(item, (DWORD)IMG_BT_FOLDER);
						
						// Insert stub to show that there can be files
						p_Dlg->m_tree.InsertItem(_T("Getting the file list..."), item);
					}
					
                    else if (si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
                    {
						HTREEITEM item = p_Dlg->m_tree.InsertItem(SZ_OBJECT_EXCHANGE, IMG_BT_VCARD, IMG_BT_VCARD, pDeviceInfo->m_item);
						p_Dlg->m_tree.SetItemData(item, (DWORD)IMG_BT_VCARD);
					}
				}
			}
		}

		CString s;
		s.Format("Discovery... done, %d services found",pDeviceInfo->m_nServices);
		p_Dlg->m_statusBar.SetWindowText(s);
		p_Dlg->SetWaitCursor(FALSE);
	}
}


///////////////////////////////////////////////////////////////////////////////////
// methods to replace virtual methods in base class CFtpClient 
//

void CBlueClientDlg::OnOpenResponse(CFtpClient::FTP_RESULT_CODE result_code)
{
	CString s;

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("ClientOpen... FAILED, error=%d",result_code);
        p_Dlg->SetFTPState(FTP_ERROR);
	}
	else
	{
		s.Format("ClientOpen... was successful.");
		p_Dlg->SetFTPState(FTP_OPEN_EVENT);
	}

	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(2000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnCloseResponse(CFtpClient::FTP_RESULT_CODE result_code)
{
	CString s;

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("ClientClose... FAILED, error=%d",result_code);
		p_Dlg->SetFTPState(FTP_ERROR);
	}
	else
	{
		s.Format("ClientClose... was successful.");
		p_Dlg->SetFTPState(FTP_CLOSE_EVENT);
	}

	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(2000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnPutResponse(CFtpClient::FTP_RESULT_CODE result_code, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("PutFile... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
	}
	else
	{
		s.Format("PutFile... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

	p_Dlg->SetFTPState(FTP_PUT_EVENT);
	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnGetResponse(CFtpClient::FTP_RESULT_CODE result_code, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("GetFile... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
	}
	else
	{
		s.Format("GetFile... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

	p_Dlg->SetFTPState(FTP_GET_EVENT);
	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnDeleteResponse(CFtpClient::FTP_RESULT_CODE result_code, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null


	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("DeleteFile... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
        Sleep(3000);	//this is only to give folks a chance to read the message
	}
	else
	{
		s.Format("DeleteFile... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

    p_Dlg->SetFTPState(FTP_DELETE_EVENT);	
	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnAbortResponse(CFtpClient::FTP_RESULT_CODE result_code)
{
	CString s;

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("Abort... FAILED,  error=%d", result_code);
	}
	else
	{
		s.Format("Abort... was successful.");
	}
	p_Dlg->SetFTPState(FTP_DELETE_EVENT);
	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	// give folks a chance to read the message
}

void CBlueClientDlg::OnProgress(CFtpClient::FTP_RESULT_CODE result_code, WCHAR * name, long current, long total)
{
	CString s;

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("FTP OnProgress event: FAILED, %d of %d bytes transferred, error=%d",
				current,total, result_code);
		p_Dlg->SetFTPState(FTP_ERROR);
	}
	else
	{
		s.Format("FTP OnProgress event: %d of %d bytes transferred",current,total);
		p_Dlg->SetFTPState(FTP_PROGRESS_EVENT);
	}
	p_Dlg->m_statusBar.SetWindowText(s);
}

void CBlueClientDlg::OnFolderListingResponse(CFtpClient::FTP_RESULT_CODE result_code, tFTP_FILE_ENTRY * listing, long entries)
{
    // Stuff the listing entries into the client tree
	CString sbText;
    tFTP_FILE_ENTRY ent;
    tFTP_FILE_ENTRY *p     = listing;
    int             len    = entries;
	int             nItems = 0;

	if (result_code == CFtpClient::COMPLETED)
	{
		HTREEITEM m_hRclickItem = p_Dlg->m_hTreeItem;
		CString s = p_Dlg->m_tree.GetItemText(m_hRclickItem);
		
		// if the currently selected item is not the one with the folder icon, step through
		// until we get to it.
		if (p_Dlg->m_tree.GetItemData(m_hRclickItem) != IMG_BT_FOLDER)
		{
			m_hRclickItem = p_Dlg->m_tree.GetChildItem(m_hRclickItem);
			while (m_hRclickItem && p_Dlg->m_tree.GetItemData(m_hRclickItem) != IMG_BT_FOLDER)
				m_hRclickItem = p_Dlg->m_tree.GetNextSiblingItem(m_hRclickItem);
		}
		
		if (m_hRclickItem==NULL)
			return;
		
		// delete what was in the folder list before we repopulate it
		HTREEITEM hTempItem;
		HTREEITEM hItem = p_Dlg->m_tree.GetChildItem(m_hRclickItem);
		while(hItem)
		{
			hTempItem = p_Dlg->m_tree.GetNextSiblingItem(hItem);
			s = p_Dlg->m_tree.GetItemText(hItem);
			p_Dlg->m_tree.DeleteItem(hItem);
			hItem = hTempItem;
		}
		
		// each time get_file_info() returns TRUE the ent struct contains a valid file or folder entry
        for (int ii = 0; ii < len; ii++)
		{
			HTREEITEM hNewItem;
			BOOL bSuccess;
			ent = p[ii];
			nItems++;
			char * p_ent_local = (char *) malloc(2*wcslen(ent.name)+2);
			wcstombs (p_ent_local, ent.name, wcslen(ent.name)+1);	// include terminating null

			if (ent.is_folder)
			{
				//att XXX SPECIAL CASE - to signify this is a subfolder we get an entry with
				//    the is_folder bit set but a zero-length filename
				if (0==ent.name[0])
				{
					hNewItem = p_Dlg->m_tree.InsertItem("Up one folder",IMG_BT_FOLDER_UP,IMG_BT_FOLDER_UP,m_hRclickItem);
					bSuccess = p_Dlg->m_tree.SetItemData(hNewItem,(DWORD)IMG_BT_REMOTE_FOLDER);
				}
				else
				{
					hNewItem = p_Dlg->m_tree.InsertItem(p_ent_local,IMG_BT_REMOTE_FOLDER,IMG_BT_REMOTE_FOLDER,m_hRclickItem);
					bSuccess = p_Dlg->m_tree.SetItemData(hNewItem,(DWORD)IMG_BT_REMOTE_FOLDER);
				}
			}
			else
			{
				hNewItem = p_Dlg->m_tree.InsertItem(p_ent_local,IMG_BT_REMOTE_DOCUMENT,IMG_BT_REMOTE_DOCUMENT,m_hRclickItem);
				bSuccess = p_Dlg->m_tree.SetItemData(hNewItem,IMG_BT_REMOTE_DOCUMENT);
			}

			free (p_ent_local);
		}
		
		//add a stub if there were no files or folders in the directory so that the '+' sign shows
		//up next to the ftp folder
		if (nItems==0)
		{
			// Insert stub to show that there can be files
			p_Dlg->m_tree.InsertItem(_T("(The folder is empty)"), m_hRclickItem);
		}
		
		p_Dlg->m_tree.Expand(m_hRclickItem,TVE_EXPAND);
		
		p_Dlg->SetFTPState(FTP_FOLDER_LIST_EVENT);

		sbText.Format("FTP_FolderListing... was successful");
	}
	// some sort of error
	else
	{
		sbText.Format("FTP_FolderListing... FAILED, error=<%d>",result_code);
	}
	p_Dlg->m_statusBar.SetWindowText(sbText);
	p_Dlg->SetWaitCursor(FALSE);
	Sleep(3000);	// give folks a chance to read the message
}

void CBlueClientDlg::OnXmlFolderListingResponse(CFtpClient::FTP_RESULT_CODE rc, WCHAR * pfolder_listing, long folder_length )
{
}

void CBlueClientDlg::OnChangeFolderResponse(CFtpClient::FTP_RESULT_CODE result_code, CFtpClient::tFtpFolder folder_type, WCHAR * folder_name)
{
	CString sbText;
	char * p_loc_name = (char *) malloc(2*wcslen(folder_name)+2);
	wcstombs (p_loc_name, folder_name, wcslen(folder_name)+1);	// include terminating null

	if (result_code != CFtpClient::COMPLETED)
	{
		CString s;
		CString szFolderType;
		switch( folder_type )
		{
        case CFtpClient::FTP_ROOT_FOLDER:	szFolderType = "Root folder";			break;
		case CFtpClient::FTP_PARENT_FOLDER:	szFolderType = "Parent folder";			break;
		case CFtpClient::FTP_SUBFOLDER:		szFolderType = "subfolder";				break;
		default:				szFolderType = "Unknown folder type";	break;
		}
		//sbText.Format("FTP ChangeFolder... FAILED, folderType=%s,error=%ld", folder_type, result_code);
        sbText.Format("FTP ChangeFolder... FAILED");
		p_Dlg->SetFTPState(FTP_IDLE);
	}
	else
	{
		sbText.Format("FTP ChangeFolder... successfully changed to <%s>", p_loc_name);
		p_Dlg->SetFTPState(FTP_CHANGE_FOLDER_EVENT);
	}
	free (p_loc_name);

	p_Dlg->m_statusBar.SetWindowText(sbText);
	p_Dlg->SetWaitCursor(FALSE);
	Sleep(3000);	// give folks a chance to read the message
}

void CBlueClientDlg::OnCreateResponse(CFtpClient::FTP_RESULT_CODE result_code, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null

	if (result_code != CFtpClient::COMPLETED)
	{
		s.Format("CreateFile... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
	}
	else
	{
		s.Format("CreateFile... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

	p_Dlg->SetFTPState(FTP_CREATE_EVENT);
	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}

////////////////////////////////////////////////////////////////////////////////////////////
// methods to replace virtual methods in base class COppClient 
//

void CBlueClientDlg::OnProgress(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * string, long current, long total)
{
	CString s;

	if (result_code != COppClient::COMPLETED)
		s.Format("OPP OnProgress event: FAILED, %d of %d bytes transferred, error=%d",
				current,total, result_code);
	else
		s.Format("OPP OnProgress event: %d of %d bytes transferred",current,total);

    p_Dlg->m_statusBar.SetWindowText(s);

}

void CBlueClientDlg::OnPushResponse(OPP_RESULT_CODE result_code,  BD_ADDR bda, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null

	if (result_code != COppClient::COMPLETED)
	{
		s.Format("OPP Push... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
	}
	else
	{
		s.Format("OPP Push... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message

}

void CBlueClientDlg::OnPullResponse(OPP_RESULT_CODE result_code , BD_ADDR bda, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null

	if (result_code != COppClient::COMPLETED)
	{
		s.Format("OPP Pull... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
	}
	else
	{
		s.Format("OPP Pull... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnExchangeResponse(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * name)
{
	CString s;
	char * p_loc_name = (char *) malloc(2*wcslen(name)+2);
	wcstombs (p_loc_name, name, wcslen(name)+1);	// include terminating null

	if (result_code != COppClient::COMPLETED)
	{
		s.Format("OPP Exchange... FAILED.  Filename:<%s>, error=%d", p_loc_name, result_code);
	}
	else
	{
		s.Format("OPP Exchange... was successful.  Filename:<%s>",p_loc_name);
	}
	free (p_loc_name);

	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}

void CBlueClientDlg::OnAbortResponse (OPP_RESULT_CODE result_code)
{
	CString s;

	if (result_code != COppClient::COMPLETED)
	{
		s.Format("Abort... FAILED,  error=%d", result_code);
	}
	else
	{
		s.Format("Abort... was successful.");
	}
	p_Dlg->m_statusBar.SetWindowText(s);
	Sleep(3000);	//this is only to give folks a chance to read the message
}


class CSaveDir
{
public:
	CSaveDir()
	{
		GetCurrentDirectory(MAX_PATH,m_buf);
	}
	virtual ~CSaveDir()
	{
		SetCurrentDirectory(m_buf);
	}
private:
	char m_buf[MAX_PATH];
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
    CString m_strVersion;
	CString m_strCopyRight;


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_strVersion = _T("BlueClient Version: ");
    m_strVersion += _T(BTWPRODUCTVERSIONSTR);
    m_strCopyRight = _T(BTWLEGALCOPYRIGHTSTR);
    SetDlgItemText(IDC_COPYRIGHT, m_strCopyRight);
    SetDlgItemText(IDC_VERSION, m_strVersion);
    return true;
}


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueClientDlg dialog

CDeviceInfo* CBlueClientDlg::pFirstDeviceInfo = NULL;

CBlueClientDlg::CBlueClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlueClientDlg::IDD, pParent),

    m_bInquiryComplete(FALSE),
    m_pDeviceSelected(NULL),
	m_bWaitCursor(FALSE),
	m_hTreeItem(0),
	m_hFTP(0),
	m_si_num_services(0),
	m_num_services(0)
{
	//{{AFX_DATA_INIT(CBlueClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetFTPState(FTP_IDLE);

    p_Dlg = this;
}

CBlueClientDlg::~CBlueClientDlg()
{

    if (pFirstDeviceInfo)
    {
        CDeviceInfo *pNext;
        for (CDeviceInfo *p = pFirstDeviceInfo; p; p = pNext)
        {
            pNext = p->pNext;
            delete p;
        }

        pFirstDeviceInfo = NULL;
    }
}


void CBlueClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlueClientDlg)
	DDX_Control(pDX, IDC_CLIENT_TREE, m_tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlueClientDlg, CDialog)
	//{{AFX_MSG_MAP(CBlueClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_CLIENT_TREE, OnDblclkClientTree)
	ON_NOTIFY(NM_RCLICK, IDC_CLIENT_TREE, OnRclickClientTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_CLIENT_TREE, OnItemexpandingClientTree)
    ON_COMMAND(ID_PROPERTIES, OnProperties)
    ON_COMMAND(ID_REFRESH, OnRefresh)
    ON_COMMAND(ID_REFRESH_NEIGHBORS, OnRefreshNeighbors) 
    ON_COMMAND(ID_REFRESH_FOLDERLISTING, OnRefreshFolderListing)
    ON_COMMAND(ID_FTP_CHANGE_REMOTE_DIRECTORY, OnFTPChangeRemoteDirectory) 
    ON_COMMAND(ID_FTP_DELETE_REMOTE_DIRECTORY, OnFTPDeleteRemoteDirectory) 
    ON_COMMAND(ID_FTP_GET_FILE, OnFTPGetFile) 
    ON_COMMAND(ID_FTP_PUT_FILE, OnFTPPutFile) 
    ON_COMMAND(ID_FTP_DELETE_FILE, OnFTPDeleteFile) 
    ON_COMMAND(ID_FTP_REMOTE_ROOT, OnFTPRoot) 
	ON_COMMAND(ID_FTP_ABORT, OnFTPAbort)
	ON_COMMAND(ID_FTP_REMOTE_CREATE_FOLDER, OnFTPCreateFolder)
	ON_COMMAND(ID_FTP_REMOTE_CREATE_FILE, OnFTPCreateFile)
    ON_COMMAND(ID_PULL_VCARD, OnPullVcard)
    ON_COMMAND(ID_EXCHANGE_VCARD, OnExchangeVcard)
    ON_COMMAND(ID_PUSH_VCARD, OnPushVcard)
    ON_COMMAND(ID_PUSH_VCAL, OnPushVcal)
    ON_COMMAND(ID_PUSH_VNOTE, OnPushVnote)
    ON_COMMAND(ID_PUSH_VMESSAGE, OnPushVmessage)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueClientDlg message handlers

BOOL CBlueClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_timer_id = SetTimer (1, 500, NULL);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    CString s;
	
    m_ImgList.Create( IDB_BT_DEVICE_IMAGE, 16, 1, RGB(255, 255, 255));
	
    m_tree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

    m_tree.SetImageList(&m_ImgList, TVSIL_NORMAL);

    m_hTreeItemRoot = m_tree.InsertItem(_T("SERVERS in Neighborhood"), IMG_BT_NEIGHBORHOOD, IMG_BT_NEIGHBORHOOD);
    m_tree.InsertItem(_T("Servers in Neighborhood"), IMG_BT_NEIGHBORHOOD, IMG_BT_NEIGHBORHOOD, m_hTreeItemRoot);

	// Create status bar at the bottom of the dialog window
	if (m_statusBar.Create(this))
	{
		static UINT auIDStatusBar[] = {
				ID_SEPARATOR,
				IDS_INDICATOR_TIME
		};
		m_statusBar.SetIndicators(auIDStatusBar, 2);

		// Make a sunken or recessed border around the first pane
		m_statusBar.SetPaneInfo(0, m_statusBar.GetItemID(0),SBPS_STRETCH, NULL );
	}

	ResizeControlsToFit();	// resize the tree control and status bar to fit

    // set default security values
    CFtpClient::SetSecurity(FtpAuthentication, FtpEncryption);
    COppClient::SetSecurity(OppAuthentication, OppEncryption);


    StopInquiry();
    Sleep(100);

    if ( ! StartInquiry() )
     	m_statusBar.SetWindowText("ERROR - Unable to start initial device inquiry.");
    else
	    m_statusBar.SetWindowText("Start Initial Inquiry");


    // optional BDA filter for lab testing with large number of devices responding to inquiry
    GetBDAFilterFromReg();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CBlueClientDlg::string2BDAddr(BD_ADDR bdAddr, char *lpbdAddrString)
{
	if ( strlen(lpbdAddrString) != (BD_ADDR_LEN * 3 - 1))
		return FALSE;

	for (int i = 0; i<BD_ADDR_LEN; i++)
		bdAddr[i] = (char)axtoi(&lpbdAddrString[i *3]);
	return TRUE;

}


/***********************************************************
lab test mechanism to filter inquiry results.  
Checks for desired server BDA in registry, filters results if found
***********************************************************/
void CBlueClientDlg::GetBDAFilterFromReg()
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


void CBlueClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlueClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBlueClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBlueClientDlg::OnDblclkClientTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CDeviceInfo *pDeviceInfo = NULL;
	CString s;
    HTREEITEM hItem = m_tree.GetDropHilightItem();
    if (!hItem)
        hItem = m_tree.GetSelectedItem();

	if (!hItem)
	{
		*pResult = 0;
		return;
	}

	switch(m_tree.GetItemData(hItem))
	{
	case IMG_BT_REMOTE_FOLDER:
		s = m_tree.GetItemText(hItem);
		pDeviceInfo = (CDeviceInfo *)m_tree.GetItemData (
									 m_tree.GetParentItem(
									 m_tree.GetParentItem(hItem)));
		if (pDeviceInfo)
		{
			SetFTPState(FTP_BUSY);
			if (0==strcmp((LPCTSTR)s,"Up one folder"))
				Parent();
			else
			{
				WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(s)+2);
				mbstowcs(p_s_wide, s, strlen(s)+1);
				ChangeFolder(p_s_wide);
				free (p_s_wide);
			}
		}
		break;
	case IMG_BT_REMOTE_DOCUMENT:
		s = m_tree.GetItemText(hItem);
		pDeviceInfo = (CDeviceInfo *)m_tree.GetItemData (
									 m_tree.GetParentItem(
									 m_tree.GetParentItem(hItem)));
		if (pDeviceInfo)
		{
			// make sure user has selected an inbox directory
			if (wcslen(szFTPInbox) == 0)
				FTPInboxBrowse(); 

			// if user chooses not to select, just ignore request
			if (wcslen(szFTPInbox) == 0)
			{
				*pResult = 0;
				return;
			}

			WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(s)+2);
			mbstowcs(p_s_wide, s, strlen(s)+1);
			SetFTPState(FTP_BUSY);
			GetFile( p_s_wide,szFTPInbox);
			free (p_s_wide);
			CString msg_s;
			msg_s.Format("GET File %s", s);
			m_statusBar.SetWindowText(msg_s);
		}
		break;
	default:
		break;
	}

	
	*pResult = 0;
}

void CBlueClientDlg::OnRclickClientTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

    HTREEITEM m_hRclickItem = m_tree.GetDropHilightItem();
    if (!m_hRclickItem)
        m_hRclickItem = m_tree.GetSelectedItem();

	m_szTreeFileName = m_tree.GetItemText(m_hRclickItem);

    POINT pt;
    GetCursorPos (&pt);

    HMENU hMenu;

    if (m_hRclickItem == m_hTreeItemRoot)
    {
        // Menu for the Bluetooth Neighborhood 
        hMenu = CreatePopupMenu ();
        AppendMenu (hMenu, MF_STRING, ID_REFRESH_NEIGHBORS, "&Refresh");
        // allow FTP abort if operation in progress
        switch(GetFTPState())
        {
        case FTP_BUSY:
        case FTP_OPENING:
        case FTP_OPEN_EVENT:
        case FTP_GET_EVENT:
        case FTP_PUT_EVENT:
        case FTP_DELETE_EVENT:
        case FTP_PROGRESS_EVENT:
        case FTP_FOLDER_LIST_EVENT:
        case FTP_CHANGE_FOLDER_EVENT:
        case FTP_CREATE_EVENT:
            AppendMenu (hMenu, MF_STRING, ID_FTP_ABORT,   "&Abort current operation...");
            break;

        case FTP_IDLE:
        case FTP_CLOSE_EVENT:
        case FTP_ABORT_EVENT:
        case FTP_CLOSING:
        case FTP_ERROR:
        default:
            break;
        }

        TrackPopupMenu (hMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);

        DestroyMenu (hMenu);
    }

    else if (m_hTreeItemRoot == m_tree.GetParentItem(m_hRclickItem))
    {
        // Menu for the Bluetooth Device
        m_pDeviceSelected = (CDeviceInfo *)m_tree.GetItemData(m_hRclickItem);

        hMenu = CreatePopupMenu ();

        GetAvailableServices(m_pDeviceSelected->bd_addr, &m_num_services, MAX_SERVICES, m_si);

        AppendMenu (hMenu, MF_STRING, ID_REFRESH, "&Refresh");
        AppendMenu (hMenu, MF_SEPARATOR, -1, NULL);
        AppendMenu (hMenu, MF_STRING, ID_PROPERTIES, "&Properties");

        TrackPopupMenu (hMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);

        DestroyMenu (hMenu);
    }
    else if (m_tree.GetItemData(m_hRclickItem) == IMG_BT_VCARD)
    {
        // Menu for the Bluetooth Device
        m_pDeviceSelected = (CDeviceInfo *)m_tree.GetItemData(m_tree.GetParentItem(m_hRclickItem));

        GetAvailableServices(m_pDeviceSelected->bd_addr, &m_num_services, MAX_SERVICES, m_si);

        hMenu = CreatePopupMenu ();

        AppendMenu (hMenu, MF_STRING | MF_HILITE, ID_PULL_VCARD, "&Pull Business Cards");
        AppendMenu (hMenu, MF_STRING, ID_EXCHANGE_VCARD, "&Exchange Business Cards");
        AppendMenu (hMenu, MF_SEPARATOR, -1, NULL);
        AppendMenu (hMenu, MF_STRING, ID_PUSH_VCARD, "Push &Business Cards");
        AppendMenu (hMenu, MF_STRING, ID_PUSH_VCAL, "Push &Calendar Item");
        AppendMenu (hMenu, MF_STRING, ID_PUSH_VNOTE, "Push &Note");
        AppendMenu (hMenu, MF_STRING, ID_PUSH_VMESSAGE, "Push &Message");

        TrackPopupMenu (hMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);

        DestroyMenu (hMenu);
    }
	else if (m_tree.GetItemData(m_hRclickItem) == IMG_BT_REMOTE_FOLDER)
	{

#ifdef DEBUG
		HTREEITEM hTemp = m_tree.GetParentItem(m_hRclickItem);
		CString s = m_tree.GetItemText(hTemp);
		hTemp = m_tree.GetParentItem(hTemp);
		s = m_tree.GetItemText(hTemp);
		hTemp = m_tree.GetParentItem(hTemp);
		s = m_tree.GetItemText(hTemp);
#endif	//DEBUG

        m_pDeviceSelected = (CDeviceInfo *)m_tree.GetItemData(
												m_tree.GetParentItem(
													m_tree.GetParentItem(m_hRclickItem)));

        GetAvailableServices(m_pDeviceSelected->bd_addr, &m_num_services, MAX_SERVICES, m_si);

        hMenu = CreatePopupMenu ();
        AppendMenu (hMenu, MF_STRING, ID_FTP_CHANGE_REMOTE_DIRECTORY, "&Change to this directory");
        AppendMenu (hMenu, MF_STRING, ID_FTP_DELETE_REMOTE_DIRECTORY, "&Delete this directory");
        TrackPopupMenu (hMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);
        DestroyMenu (hMenu);
	}
	else if (m_tree.GetItemData(m_hRclickItem) == IMG_BT_REMOTE_DOCUMENT)
	{
        m_pDeviceSelected = (CDeviceInfo *)m_tree.GetItemData(
												m_tree.GetParentItem(
													m_tree.GetParentItem(m_hRclickItem)));
        
        GetAvailableServices(m_pDeviceSelected->bd_addr, &m_num_services, MAX_SERVICES, m_si);

        hMenu = CreatePopupMenu ();
        AppendMenu (hMenu, MF_STRING, ID_FTP_GET_FILE, "&Get this file");
        AppendMenu (hMenu, MF_STRING, ID_FTP_DELETE_FILE, "&Delete this file");
        TrackPopupMenu (hMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);
        DestroyMenu (hMenu);
	}
	else if (m_tree.GetItemData(m_hRclickItem) == IMG_BT_FOLDER)
	{
		//set this for the (possible) folderlisting if the user selects "Refresh" below
        m_pDeviceSelected = (CDeviceInfo *)m_tree.GetItemData(m_tree.GetParentItem(m_hRclickItem));

        GetAvailableServices(m_pDeviceSelected->bd_addr, &m_num_services, MAX_SERVICES, m_si);

        hMenu = CreatePopupMenu ();
        AppendMenu (hMenu, MF_STRING, ID_REFRESH_FOLDERLISTING,    "&Refresh");
        AppendMenu (hMenu, MF_STRING, ID_FTP_PUT_FILE,             "&Put a file...");
        AppendMenu (hMenu, MF_STRING, ID_FTP_REMOTE_ROOT,          "Change to the &Root Folder");
        AppendMenu (hMenu, MF_STRING, ID_FTP_REMOTE_CREATE_FOLDER, "Create a new &Folder...");
        AppendMenu (hMenu, MF_STRING, ID_FTP_REMOTE_CREATE_FILE,   "Create a new F&ile...");

        // allow FTP abort if operation in progress
        if (GetFTPState() == FTP_BUSY)
            AppendMenu (hMenu, MF_STRING, ID_FTP_ABORT,   "&Abort current operation...");

        TrackPopupMenu (hMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);
        DestroyMenu (hMenu);

		if (m_pDeviceSelected->m_hFtp && GetFTPState() != FTP_BUSY)
		{
			m_hTreeItem = m_hRclickItem;
			m_ftp_rc = FolderListing();
            if (m_ftp_rc == CFtpClient::SUCCESS)
				SetFTPState(FTP_BUSY);
			else 
				SetFTPState(FTP_ERROR);
		}
		else
		{
			GetAvailableServices(m_pDeviceSelected->bd_addr, &m_pDeviceSelected->m_nServices, MAX_SERVICES, m_si);
			for (int i = 0; i < m_pDeviceSelected->m_nServices; i++)
			{
                if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_FILE_TRANSFER)
				{
					m_hTreeItem = m_hRclickItem;

					m_ftp_rc = OpenConnection(m_pDeviceSelected->bd_addr, m_si[i]);

                    if (m_ftp_rc == CFtpClient::SUCCESS)
                    {
					    SetFTPState(FTP_OPENING);
                        m_pDeviceSelected->m_hFtp = 1; // flag that the FTP conn is OK
                    }
					else
                    {
						SetFTPState(FTP_IDLE);
                        m_pDeviceSelected->m_hFtp = 1; // flag that the FTP conn failed
                    }
					m_hFTP = m_pDeviceSelected->m_hFtp;
					break;
				}
			}
		}
	}
    
	*pResult = 0;
}

void CBlueClientDlg::OnItemexpandingClientTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

    HTREEITEM hItem = pNMTreeView->itemNew.hItem;
    CWaitCursor cursor;
	int nImage1, nImage2;

	m_tree.GetItemImage(hItem,nImage1,nImage2);

	if ((hItem == m_hTreeItemRoot) && !m_bInquiryComplete)
    {
        HTREEITEM stub = m_tree.GetChildItem(hItem);
        m_tree.DeleteItem (stub);

        m_bInquiryComplete = TRUE;
    }
	else if (m_tree.GetItemData(hItem) == IMG_BT_FOLDER)
    {
		CDeviceInfo *pDeviceInfo = (CDeviceInfo *)m_tree.GetItemData (m_tree.GetParentItem(hItem));
		if (pDeviceInfo)
		{
			GetAvailableServices(pDeviceInfo->bd_addr, &pDeviceInfo->m_nServices, MAX_SERVICES, m_si);
			for (int i = 0; i < pDeviceInfo->m_nServices; i++)
			{
                if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_FILE_TRANSFER)
				{
					if (pNMTreeView->action == 2 /*expanding*/)
					{
						if (pDeviceInfo->m_hFtp)
							break;
						
						m_hTreeItem = hItem;
					    m_ftp_rc = OpenConnection(m_pDeviceSelected->bd_addr, m_si[i]);

                        if (m_ftp_rc == CFtpClient::SUCCESS)
                        {
					        SetFTPState(FTP_OPENING);
                            m_pDeviceSelected->m_hFtp = 1;  // flag that FTP connection OK
                        }
					    else
                        {
						    SetFTPState(FTP_IDLE);
                            m_pDeviceSelected->m_hFtp = 0; //  flag that the FTP conn failed
                        }
					    m_hFTP = m_pDeviceSelected->m_hFtp;
						break;
					}
					else if (pNMTreeView->action == 1 /*shrinking*/)
					{
						if (pDeviceInfo->m_hFtp)
						{

							HTREEITEM stub = m_tree.GetChildItem(hItem);
							HTREEITEM tempItem;
							while( stub )
							{
								tempItem = stub;
								stub = m_tree.GetNextSiblingItem(stub);
								m_tree.DeleteItem (tempItem);	//delete the stub used to create the '+' next to the item
							}
							// Insert one stub to show that there can be files
							m_tree.InsertItem(_T("Getting the file list..."),hItem);
							m_tree.Expand(hItem, TVE_COLLAPSE);

							SetFTPState(FTP_CLOSING);
							CloseConnection();
							pDeviceInfo->m_hFtp = 0;
						}
					}
					break;
				}
			}
		}
    }
	// else if it's a device
	else if (nImage1 < IMG_BT_NEIGHBORHOOD)
	{
		if (pNMTreeView->action == 1 /*shrinking*/)
		{
			CDeviceInfo *pDeviceInfo = (CDeviceInfo *)m_tree.GetItemData(hItem);
			if (pDeviceInfo && pDeviceInfo->m_hFtp)
			{
				// delete what was in the folder list before we repopulate it
				HTREEITEM hFolder = NULL;
				HTREEITEM hTempItem;

				//start with the 1st item below the device, and look for the folder
				HTREEITEM hCurItem = m_tree.GetChildItem(hItem);
				m_tree.GetItemImage(hCurItem,nImage1,nImage2);
				while (hCurItem && nImage1 != IMG_BT_FOLDER)
				{
					hCurItem = m_tree.GetNextSiblingItem(hCurItem);
					if (hCurItem)
						m_tree.GetItemImage(hCurItem,nImage1,nImage2);
				}

				//if we found the folder, delete all of the file and folder names in it
				if (hCurItem && nImage1==IMG_BT_FOLDER)
				{
					hFolder = hCurItem;
					hCurItem = m_tree.GetChildItem(hCurItem);	//start with the 1st item below the folder

					while(hCurItem)
					{
						hTempItem = m_tree.GetNextSiblingItem(hCurItem);
						m_tree.DeleteItem(hCurItem);
						hCurItem = hTempItem;
					}
					//insert a stub and collapse it so the user sees the '+' sign
					m_tree.InsertItem(_T("Getting the file list..."), hFolder);
					m_tree.Expand(hFolder,TVE_COLLAPSE);
				}

				SetFTPState(FTP_CLOSING);
				CloseConnection();
				pDeviceInfo->m_hFtp = 0;
			}
		}
	}

	*pResult = 0;
}

/////////////////// START OF DYNAMIC MENU COMMAND HANDLERS ////////////////////////////////


void CBlueClientDlg::OnFTPChangeRemoteDirectory()
{
	{
		if (0==strcmp(m_szTreeFileName,"Up one folder"))
			Parent();
		else
			{
				WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(m_szTreeFileName)+2);
				mbstowcs(p_s_wide, m_szTreeFileName, strlen(m_szTreeFileName)+1);
				ChangeFolder(p_s_wide);
				free (p_s_wide);
			}
	}
}

void CBlueClientDlg::OnFTPDeleteRemoteDirectory()
{
	{
		SetFTPState(FTP_BUSY);
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(m_szTreeFileName)+2);
		mbstowcs(p_s_wide, m_szTreeFileName, strlen(m_szTreeFileName)+1);
		DeleteFile(p_s_wide);
		free (p_s_wide);
	}
}

void CBlueClientDlg::OnFTPGetFile()
{
    // make sure user has selected an inbox directory
    FTPInboxBrowse(); 

    // if user chooses not to select, just ignore request
    if (wcslen(szFTPInbox) == 0)
        return;

	if (GetFTPState()==FTP_IDLE)
	{
		SetFTPState(FTP_BUSY);
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(m_szTreeFileName)+2);
		mbstowcs(p_s_wide, m_szTreeFileName, strlen(m_szTreeFileName)+1);
		GetFile( p_s_wide,szFTPInbox);
		free (p_s_wide);
		CString msg_s;
        msg_s.Format("GET File %s", m_szTreeFileName);
        m_statusBar.SetWindowText(msg_s);
	}
}

void CBlueClientDlg::OnFTPPutFile()
{
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name

    CWnd *mainWnd = AfxGetMainWnd();

    // Initialize OPENFILENAME
    memset(szFile,0,sizeof(szFile));
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = mainWnd->m_hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All (*.*)\0*.*\0\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 
    if (::GetOpenFileName(&ofn)==TRUE) 
    {
	    SetFTPState(FTP_BUSY);
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(ofn.lpstrFile)+2);
		mbstowcs(p_s_wide, ofn.lpstrFile, strlen(ofn.lpstrFile)+1);
	    PutFile(p_s_wide);
		free (p_s_wide);
    }
}

void CBlueClientDlg::OnFTPDeleteFile()
{
	{
		SetFTPState(FTP_BUSY);
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(m_szTreeFileName)+2);
		mbstowcs(p_s_wide, m_szTreeFileName, strlen(m_szTreeFileName)+1);
		DeleteFile(p_s_wide);
		free (p_s_wide);
	}
}

void CBlueClientDlg::OnFTPRoot()
{
    SetFTPState(FTP_BUSY);
	Root();
}

void CBlueClientDlg::OnFTPAbort()
{
    CFtpClient::Abort();
}

void CBlueClientDlg::OnFTPCreateFolder()
{
	INT_PTR nRet;
	FTPCreate dlg;
	dlg.m_Name = "";
	dlg.m_bCreateFile = FALSE;
	nRet = dlg.DoModal();
	if (nRet == IDOK && strlen(dlg.m_Name)>0)
	{
		SetFTPState(FTP_BUSY);
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(dlg.m_Name)+2);
		mbstowcs(p_s_wide, dlg.m_Name, strlen(dlg.m_Name)+1);
		CreateFolder(p_s_wide);
		free (p_s_wide);
	}
}

void CBlueClientDlg::OnFTPCreateFile()
{
	INT_PTR nRet;
	FTPCreate dlg;
	dlg.m_Name = "";
	dlg.m_bCreateFile = TRUE;
	nRet = dlg.DoModal();
	if (nRet == IDOK && strlen(dlg.m_Name)>0)
	{
		SetFTPState(FTP_BUSY);
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(dlg.m_Name)+2);
		mbstowcs(p_s_wide, dlg.m_Name, strlen(dlg.m_Name)+1);
		CreateEmpty(p_s_wide);
		free (p_s_wide);
	}
}

void CBlueClientDlg::OnProperties() 
{
    if (!CBtDeviceProps::m_bActive)
        CBtDeviceProps::m_pDlg = NULL;

    if (!CBtDeviceProps::m_pDlg)
    {
        CBtDeviceProps::m_pDlg = new CBtDeviceProps (m_pDeviceSelected, m_num_services, m_si);
        CBtDeviceProps::m_pDlg->Create ();
    }
    else
    {
        CBtDeviceProps::m_pDlg->SetForegroundWindow ();
    }
}

void CBlueClientDlg::OnRefresh() 
{
    CString msg_s;
    BD_ADDR *p_bd_addr;
    p_bd_addr = &m_pDeviceSelected->bd_addr;
    CString bd_addr_str;
    bd_addr_str.Format("[%x:%x:%x:%x:%x:%x]",
        m_pDeviceSelected->bd_addr[0], m_pDeviceSelected->bd_addr[1],
        m_pDeviceSelected->bd_addr[2], m_pDeviceSelected->bd_addr[3],
        m_pDeviceSelected->bd_addr[4], m_pDeviceSelected->bd_addr[5]);

    if ( ! StartDiscovery(m_pDeviceSelected->bd_addr, NULL) )
    {
        msg_s.Format("Discovery Failed for BDA:  %s", bd_addr_str);
        m_statusBar.SetWindowText(msg_s);
    }
    else
    {
        msg_s.Format("Discovery Started for BDA:  %s", bd_addr_str);
        m_statusBar.SetWindowText(msg_s);
    }
}


void CBlueClientDlg::OnRefreshNeighbors() 
{
    StopInquiry();
    Sleep(100);

    if ( ! StartInquiry() )
     	m_statusBar.SetWindowText("ERROR - Unable to start device inquiry for refresh.");
    else
        m_statusBar.SetWindowText("Start Refresh Inquiry");	
}

// this function is called when the user selects right-clicks on a folder and selects
// "Refresh" from the popup menu
void CBlueClientDlg::OnRefreshFolderListing()
{
	int nImage1, nImage2;
    HTREEITEM m_hRclickItem = m_tree.GetDropHilightItem();
    if (!m_hRclickItem)
        m_hRclickItem = m_tree.GetSelectedItem();

	if (!m_hRclickItem)
		return;
	
	m_tree.GetItemImage(m_hRclickItem,nImage1,nImage2);
	CString s = m_tree.GetItemText(m_hRclickItem);
	if (nImage1 == IMG_DEVICE_DESKTOP)
	{
		CDeviceInfo *pDeviceInfo = (CDeviceInfo *)m_tree.GetItemData(m_tree.GetParentItem(m_hRclickItem));
		if (pDeviceInfo && pDeviceInfo->m_hFtp)
		{
			FolderListing();
			SetFTPState(FTP_BUSY);
		}
	}
}

void CBlueClientDlg::OnPullVcard()
{
    // make sure user has selected an inbox directory
    if (wcslen(szInboxDirectory) == 0)
        InboxBrowse(); 

    // if user chooses not to select, just ignore request
    if (wcslen(szInboxDirectory) == 0)
        return;

    for (int i = 0; i < m_pDeviceSelected->m_nServices; i++)
    {
        if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
        {
            Pull(m_pDeviceSelected->bd_addr, szInboxDirectory, m_si[i]);
            break;
        }
    }
}

void CBlueClientDlg::OnExchangeVcard()
{
    // make sure user has selected an inbox directory
    if (wcslen(szInboxDirectory) == 0)
        InboxBrowse(); 

    // if user chooses not to select, just ignore request
    if (wcslen(szInboxDirectory) == 0)
        return;

    // make sure user has selected a default business card file
    if (wcslen(szDefaultBusinessCard) == 0)
        DefaultBusinessCardBrowse(); 

    // if user chooses not to select, just ignore request
    if (wcslen(szDefaultBusinessCard) == 0)
        return;

    for (int i = 0; i < m_pDeviceSelected->m_nServices; i++)
    {
        if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
        {
            Exchange(m_pDeviceSelected->bd_addr, szDefaultBusinessCard, szInboxDirectory, m_si[i]);
            break;
        }
    }
}

void CBlueClientDlg::OnPushVcard()
{
    // make sure user has selected a default business card file
    if (wcslen(szDefaultBusinessCard) == 0)
        DefaultBusinessCardBrowse(); 

    // if user chooses not to select, just ignore request
    if (wcslen(szDefaultBusinessCard) == 0)
        return;

    for (int i = 0; i < m_pDeviceSelected->m_nServices; i++)
    {
        if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
        {
			Push(m_pDeviceSelected->bd_addr, szDefaultBusinessCard, m_si[i]);
			break;
        }
    }
}

void CBlueClientDlg::OnPushVcal()
{
    CSdpDiscoveryRec *pSi;
	int i = 0;
    for ( i; i < m_pDeviceSelected->m_nServices; i++)
    {
        if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
        {
            pSi = &m_si[i];
            break;
        }
    }

    if (i == m_pDeviceSelected->m_nServices)
        return;

    CSaveDir sv;

    OPENFILENAME ofn;
    char         chFileName[MAX_PATH];

    memset (&ofn, 0, sizeof (ofn));
    memset (chFileName, 0, MAX_PATH);

    ofn.Flags |= (OFN_FILEMUSTEXIST);
    ofn.lStructSize     = sizeof (ofn);
    ofn.lpstrFilter     = _T("Calendar 1.0 Items (*.vcs)\0*.vcs\0Calendar 2.0 Items (*.ics)\0*.ics\0\0");
    ofn.lpstrFile       = chFileName;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrTitle      = _T("Calendar File Name");

    if (::GetOpenFileName (&ofn))
    {
        if (!ofn.nFileExtension)
            _tcscat (chFileName, _T(".vcs"));
		{
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(chFileName)+2);
		mbstowcs(p_s_wide, chFileName, strlen(chFileName)+1);
		Push(m_pDeviceSelected->bd_addr, p_s_wide, m_si[i]);
		free (p_s_wide);
		}
    }
}


void CBlueClientDlg::OnPushVnote()
{
    CSdpDiscoveryRec *pSi;
	int i = 0;
    for ( i; i < m_pDeviceSelected->m_nServices; i++)
    {
        if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
        {
            pSi = &m_si[i];
            break;
        }
    }

    if (i == m_pDeviceSelected->m_nServices)
        return;

    CSaveDir     sv;

    OPENFILENAME ofn;
    char         chFileName[MAX_PATH];

    memset (&ofn, 0, sizeof (ofn));
    memset (chFileName, 0, MAX_PATH);

    ofn.Flags |= (OFN_FILEMUSTEXIST);
    ofn.lStructSize     = sizeof (ofn);
    ofn.lpstrFilter     = _T("Notes (*.vnt)\0*.vnt\0\0");
    ofn.lpstrFile       = chFileName;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrTitle      = _T("Note File Name");

    if (::GetOpenFileName (&ofn))
    {
        if (!ofn.nFileExtension)
            _tcscat (chFileName, _T(".vnt"));
		{
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(chFileName)+2);
		mbstowcs(p_s_wide, chFileName, strlen(chFileName)+1);
		Push(m_pDeviceSelected->bd_addr, p_s_wide, m_si[i]);
		free (p_s_wide);
		}
    }
}

void CBlueClientDlg::OnPushVmessage()
{
    CSdpDiscoveryRec *pSi;
	int i = 0;
    for ( i; i < m_pDeviceSelected->m_nServices; i++)
    {
        if (m_si[i].m_service_guid == guid_SERVCLASS_OBEX_OBJECT_PUSH)
        {
            pSi = &m_si[i];
            break;
        }
    }

    if (i == m_pDeviceSelected->m_nServices)
        return;

    CSaveDir     sv;

    OPENFILENAME ofn;
    char         chFileName[MAX_PATH];

    memset (&ofn, 0, sizeof (ofn));
    memset (chFileName, 0, MAX_PATH);

    ofn.Flags |= (OFN_FILEMUSTEXIST);
    ofn.lStructSize     = sizeof (ofn);
    ofn.lpstrFilter     = _T("Messages (*.vmg)\0*.vmg\0\0");
    ofn.lpstrFile       = chFileName;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrTitle      = _T("Message File Name");

    if (::GetOpenFileName (&ofn))
    {
        if (!ofn.nFileExtension)
            _tcscat (chFileName, _T(".vmg"));
		{
		WCHAR *p_s_wide = (WCHAR *)malloc (2*strlen(chFileName)+2);
		mbstowcs(p_s_wide, chFileName, strlen(chFileName)+1);
		Push(m_pDeviceSelected->bd_addr, p_s_wide, m_si[i]);
		free (p_s_wide);
		}
    }
}

///////////////////////////// END OF COMMAND HANDLERS ////////////////////////////////////


CBlueClientDlg::tFTPState CBlueClientDlg::GetFTPState()
{
	return(m_FTPState);
}

void CBlueClientDlg::SetFTPState(tFTPState state)
{
	char *s;
	switch(state)
	{
	case FTP_IDLE:					s = "FTP_IDLE";					break;
	case FTP_BUSY:					s = "FTP_BUSY";					break;
	case FTP_OPENING:				s = "FTP_OPENING";				break;
	case FTP_OPEN_EVENT:			s = "FTP_OPEN_EVENT";			break;
	case FTP_CLOSE_EVENT:			s = "FTP_CLOSE_EVENT";			break;
	case FTP_GET_EVENT:				s = "FTP_GET_EVENT";			break;
	case FTP_PUT_EVENT:				s = "FTP_PUT_EVENT";			break;
	case FTP_DELETE_EVENT:			s = "FTP_DELETE_EVENT";			break;
	case FTP_CREATE_EVENT:			s = "FTP_CREATE_EVENT";			break;
	case FTP_ABORT_EVENT:			s = "FTP_ABORT_EVENT";			break;
	case FTP_PROGRESS_EVENT:		s = "FTP_PROGRESS_EVENT";		break;
	case FTP_FOLDER_LIST_EVENT:		s = "FTP_FOLDER_LIST_EVENT";	break;
	case FTP_CHANGE_FOLDER_EVENT:	s = "FTP_CHANGE_FOLDER_EVENT";	break;
	case FTP_CLOSING:				s = "FTP_CLOSING";				break;
	case FTP_ERROR:					s = "FTP_ERROR";				break;
	default:						s = "unknown state!";			break;
	}
	TRACE1("setting FTPState to <%s>\n",s);

	m_FTPState = state;
}

CDeviceInfo::CDeviceInfo(BD_ADDR bda, DEV_CLASS dc, BD_NAME bdn)
{
    memcpy (bd_addr, bda, BD_ADDR_LEN);
    memcpy (dev_class, dc, DEV_CLASS_LEN);
    memcpy (bd_name, bdn, BD_NAME_LEN);

    pNext       = NULL;
	m_item      = NULL;
	m_nServices = 0;
	m_hFtp      = 0;
	m_sSPPPort  = 0;
	m_sLAPPort  = 0;
	m_sDUNPort  = 0;
	m_sFAXPort  = 0;
}

// resize the tree control and status bar to fit
void CBlueClientDlg::ResizeControlsToFit()
{

	if (m_tree.m_hWnd)
	{
		RECT treeRect,dlgClientRect;
		CRect myCRect;
		m_tree.GetClientRect(&treeRect);
		GetClientRect(&dlgClientRect);
		myCRect = dlgClientRect;
		myCRect.NormalizeRect();
#define BORDERWIDTH	2
		myCRect.bottom -= myCRect.top + BORDERWIDTH + BORDERWIDTH;
		myCRect.top     = BORDERWIDTH;
		myCRect.right  -= myCRect.left + BORDERWIDTH + BORDERWIDTH;
		myCRect.left    = BORDERWIDTH;

		// figure out how much space the status bar takes up
		CRect rcClientStart;
		CRect rcClientNow;
		GetClientRect(rcClientStart);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,
						0, reposQuery, rcClientNow);

        // resize the tree window so it doesn't overlap the graphic at top.
		myCRect.top += 100;

		// resize the tree window so it doesn't overlap the status bar.
		myCRect.bottom -= (rcClientStart.bottom - rcClientNow.bottom);
		m_tree.MoveWindow((LPRECT)myCRect,TRUE);

		// Now redraw the status bar at the bottom of the screen.
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	}
}

LRESULT CBlueClientDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// eat mouse clicks if we have a wait cursor enabled
	if (m_bWaitCursor)
	{
		//don't let the child windows set the cursor
		if (message == WM_SETCURSOR)
			return(TRUE);

#ifndef WM_XBUTTONDBLCLK	//support xbuttons
#define WM_XBUTTONDBLCLK                0x020D
#endif
		if (message >= WM_LBUTTONDOWN && message <= WM_XBUTTONDBLCLK)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return(0L);
		}

		//exit from wait cursor mode if the user presses <ESC>
		if (message == WM_CHAR && wParam==VK_ESCAPE)
		{
			SetWaitCursor(FALSE);
		}
	}
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CBlueClientDlg::SetWaitCursor( bool bEnable )
{
	static HCURSOR hSavedCursor = NULL;

	m_bWaitCursor = bEnable;
	if (m_bWaitCursor == TRUE)
	{
		SetCapture();
		hSavedCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
	}
	else if (hSavedCursor)
	{
		SetCursor(hSavedCursor);
		ReleaseCapture();
	}
}

/////////////////////////////////////////////////////////////////////////////
// FTPCreate dialog

FTPCreate::FTPCreate(CWnd* pParent /*=NULL*/)
	: CDialog(FTPCreate::IDD, pParent)
{
	//{{AFX_DATA_INIT(FTPCreate)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void FTPCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FTPCreate)
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDV_MaxChars(pDX, m_Name, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(FTPCreate, CDialog)
	//{{AFX_MSG_MAP(FTPCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBlueClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CBlueClientDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CBlueClientDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_hFTP != 0)
	{
		switch(GetFTPState())
		{
		case FTP_IDLE:
		case FTP_BUSY:
		case FTP_CLOSING:
			break;

		case FTP_OPEN_EVENT:
		case FTP_PUT_EVENT:
		case FTP_DELETE_EVENT:
		case FTP_CREATE_EVENT:
		case FTP_CHANGE_FOLDER_EVENT:
			SetFTPState(FTP_BUSY);
			FolderListing();
			break;

		case FTP_PROGRESS_EVENT:
			break;

		case FTP_GET_EVENT:
		case FTP_ABORT_EVENT:
		case FTP_FOLDER_LIST_EVENT:
			SetFTPState(FTP_IDLE);
			break;

		case FTP_CLOSE_EVENT:	//
		case FTP_ERROR:	// when an open callback returns an error, you can't call
						// FTPRemoveConnection(), just update client state
			m_hFTP = 0;
			SetFTPState(FTP_IDLE);
			break;
		}
	}

	{
		CTime t = CTime::GetCurrentTime();
		char  szTime[6];
		int   nHour = t.GetHour();
		int   nMinute = t.GetMinute();
		static int   nLastMinute = 0;

		if (nMinute != nLastMinute)
		{
			// Base hours on 12 instead of 24
			if (nHour > 12)
				nHour -= 12;
			
			wsprintf(szTime, _T("%02i:%02i"), nHour, nMinute);
			
			// Now set the text of the pane
			m_statusBar.SetPaneText(m_statusBar.CommandToIndex(IDS_INDICATOR_TIME),
				LPCSTR(szTime));
			
			nLastMinute = nMinute;
		}
	}
}

BOOL FTPCreate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBlueClientDlg::GetAvailableServices (BD_ADDR p_bd_addr, int *p_nServices, int max_services, CSdpDiscoveryRec * p_si)
{
    CString err_s;
    CString bd_addr_str;
    bd_addr_str.Format("[%x:%x:%x:%x:%x:%x]", p_bd_addr[0], p_bd_addr[1], p_bd_addr[2],
                                              p_bd_addr[3], p_bd_addr[4], p_bd_addr[5]);

	// delete internal allocation for any entries already in the current list

    *p_nServices =
        ReadDiscoveryRecords(p_bd_addr, max_services, p_si); // no GUID - get all
}

/////////////////////////////////////////////////////////////////////////////
// Browsers 
//

void CBlueClientDlg::FtpRootBrowse() 
{
	BROWSEINFO		bi;
	LPCITEMIDLIST	pidl = NULL;
	char			szDirectory[MAX_PATH];

	memset( &bi, 0, sizeof( BROWSEINFO ) );

	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidl;
	bi.pszDisplayName = szDirectory;
	bi.lpszTitle = "Select FTP Root Directory";
	bi.ulFlags = BIF_RETURNONLYFSDIRS + BIF_DONTGOBELOWDOMAIN;

	pidl = ::SHBrowseForFolder( &bi ); 

	char loc_string[MAX_PATH];
	::SHGetPathFromIDList( pidl, loc_string );
	mbstowcs(szFTPInbox, &loc_string[0], strlen(loc_string)+1);
}

void CBlueClientDlg::DefaultBusinessCardBrowse() 
{
    OPENFILENAME ofn;
    char         chFileName[MAX_PATH];

    memset (&ofn, 0, sizeof (ofn));
    memset (chFileName, 0, MAX_PATH);

    ofn.Flags |= (OFN_FILEMUSTEXIST);
    ofn.lStructSize     = sizeof (ofn);
    ofn.lpstrFilter     = _T("Business Card Files (*.vcd;*.vcf)\0*.vcd;*.vcf\0\0");
    ofn.lpstrFile       = chFileName;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrTitle      = _T("Default Business Card Name");

    if (::GetOpenFileName (&ofn))
    {
        mbstowcs(szDefaultBusinessCard, ofn.lpstrFile, strlen (ofn.lpstrFile)+1);
    }
}

void CBlueClientDlg::InboxBrowse() 
{
	BROWSEINFO		bi;
	LPCITEMIDLIST	pidl = NULL;
	char			szDirectory[MAX_PATH];

	memset( &bi, 0, sizeof( BROWSEINFO ) );

	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidl;
	bi.pszDisplayName = szDirectory;
	bi.lpszTitle = "Select PIM Item Transfer Folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS + BIF_DONTGOBELOWDOMAIN;

	pidl = ::SHBrowseForFolder( &bi ); 

	char loc_string[MAX_PATH];
	::SHGetPathFromIDList( pidl, loc_string );
	mbstowcs(szInboxDirectory, &loc_string[0], strlen(loc_string)+1);
}

void CBlueClientDlg::FTPInboxBrowse() 
{
	BROWSEINFO		bi;
	LPCITEMIDLIST	pidl = NULL;
	char			szDirectory[MAX_PATH];

	memset( &bi, 0, sizeof( BROWSEINFO ) );

	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = pidl;
	bi.pszDisplayName = szDirectory;
	bi.lpszTitle = "Select Local Folder for FTP Get";
	bi.ulFlags = BIF_RETURNONLYFSDIRS + BIF_DONTGOBELOWDOMAIN;

	pidl = ::SHBrowseForFolder( &bi ); 

	char loc_string[MAX_PATH];
	::SHGetPathFromIDList( pidl, loc_string );
	mbstowcs(szFTPInbox, &loc_string[0], strlen(loc_string)+1);
}

// End of browsers

/////////////////////////////////////////////////////////////////////////////
// CBtDeviceProps dialog

BOOL CBtDeviceProps::m_bActive = FALSE;
CBtDeviceProps *CBtDeviceProps::m_pDlg = NULL;

CBtDeviceProps::CBtDeviceProps(CDeviceInfo *pDevice, UINT num_services, CSdpDiscoveryRec *pSI, CWnd* pParent /*=NULL*/)
	: CModelessDlg(CBtDeviceProps::IDD, pParent), 
      m_devInfo(pDevice->bd_addr, pDevice->dev_class, pDevice->bd_name)
{
	//{{AFX_DATA_INIT(CBtDeviceProps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_num_services = num_services;
    p_si = pSI;
}

CBtDeviceProps::~CBtDeviceProps()
{
}


void CBtDeviceProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBtDeviceProps)
	DDX_Control(pDX, IDC_LIST_SERVICE, m_lbService);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBtDeviceProps, CDialog)
	//{{AFX_MSG_MAP(CBtDeviceProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtDeviceProps message handlers

BOOL CBlueClientDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}


BOOL CBtDeviceProps::OnInitDialog() 
{
	CModelessDlg::OnInitDialog();
    CString s;
    s.Format ("%02X:%02X:%02X:%02X:%02X:%02X", 
        m_devInfo.bd_addr[0], m_devInfo.bd_addr[1], m_devInfo.bd_addr[2], 
        m_devInfo.bd_addr[3], m_devInfo.bd_addr[4], m_devInfo.bd_addr[5]);
    SetDlgItemText (IDC_BD_ADDR, s);
    SetDlgItemText (IDC_BD_NAME, (LPCTSTR)m_devInfo.bd_name);

    int rgTabStops[] = {100, 160, 0};
    m_lbService.SetTabStops(2, rgTabStops);
    for (UINT i = 0; i < m_num_services; i++)
    {
        s.Format("%s\t%s:%lx (uuid)", p_si[i].m_service_name, "", p_si[i].m_service_guid.Data1);

        m_lbService.AddString(s);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CBlueClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    if (pFirstDeviceInfo)
    {
        CDeviceInfo *pNext;
        for ( CDeviceInfo *p = pFirstDeviceInfo; p; p = pNext)
        {
            pNext = p->pNext;
            delete p;
        }

        pFirstDeviceInfo = NULL;
    }	
}


bool CBlueClientDlg ::GetFtpDefaultRoot(LPTSTR szRootDir, DWORD dwSize)
{
HKEY	hBaseKey = HKEY_CURRENT_USER; 
HKEY	hKey = NULL;
DWORD	dwIndex = 0;

TCHAR	szKeyPath[MAX_PATH];
bool	bRet = FALSE;
TCHAR szKeyName[MAX_PATH];

    _tcscpy(szKeyPath, _T("Software\\WIDCOMM\\BTConfig\\Services"));

	if (RegOpenKeyEx (hBaseKey, szKeyPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return FALSE;
	    
	while (1)
	{
		long lRet = 0;
		DWORD	cbKey = MAX_PATH;
		if (RegEnumKeyEx (hKey, dwIndex, szKeyName, &cbKey, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
			break;

		HKEY hKey1 = 0;

        if (RegOpenKeyEx (hKey, szKeyName, 0, KEY_READ, &hKey1) == ERROR_SUCCESS)
        {
			DWORD dwBytes;
            DWORD dwUUID = 0;

                dwBytes = sizeof (DWORD);
                RegQueryValueEx (hKey1, "UUID", NULL, NULL, (LPBYTE)&dwUUID, &dwBytes);
				if (!memcmp (&dwUUID, &guid_SERVCLASS_OBEX_FILE_TRANSFER, dwBytes))
				{
					RegQueryValueEx (hKey1, "root", NULL, NULL, (LPBYTE)szRootDir, &dwSize);
					RegCloseKey (hKey1);
                    bRet = TRUE;
					break;
                }
               RegCloseKey (hKey1);
         }
         dwIndex++;
	};

	if (hKey != NULL)
		RegCloseKey (hKey);

	return bRet;
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
