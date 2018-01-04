// SecurityDlg.cpp : implementation file
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
#include "BlueTime.h"
#include "SecurityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CSecurityDlg dialog


CSecurityDlg::CSecurityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecurityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecurityDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

        
    m_security_level_in = BTM_SEC_NONE;
	m_security_level_out = BTM_SEC_NONE;
	m_isServer = FALSE;

}


void CSecurityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecurityDlg)
	DDX_Control(pDX, IDC_SECURITY_LIST, m_security_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecurityDlg, CDialog)
	//{{AFX_MSG_MAP(CSecurityDlg)
	ON_LBN_SELCHANGE(IDC_SECURITY_LIST, OnSelchangeSecurityList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecurityDlg message handlers

BOOL CSecurityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// set up display for this client/server choice
	if (m_isServer)
	{
		m_internal_list.Add(CSecurityOption("No Security", BTM_SEC_NONE, FALSE));
		m_internal_list.Add(CSecurityOption("Authorize Incoming", BTM_SEC_IN_AUTHORIZE, FALSE));
		m_internal_list.Add(CSecurityOption("Authenticate Incoming", BTM_SEC_IN_AUTHENTICATE, FALSE));
		m_internal_list.Add(CSecurityOption("Encrypt Incoming", BTM_SEC_IN_ENCRYPT, FALSE));

		// clear any previous client only flags
		m_security_level_in &= ~(BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT); 
	}
	else
	{
		m_internal_list.Add(CSecurityOption("No Security", BTM_SEC_NONE, FALSE));
		m_internal_list.Add(CSecurityOption("Authenticate Outgoing", BTM_SEC_OUT_AUTHENTICATE, FALSE));
		m_internal_list.Add(CSecurityOption("Encrypt Outgoing", BTM_SEC_OUT_ENCRYPT, FALSE));

		// clear any previous server only flags
		m_security_level_in &= ~(BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT); 
	}


    if (m_security_level_in == BTM_SEC_NONE)
        m_security_level_out = BTM_SEC_NONE;
    else
        // no security overrides
        m_security_level_out = m_security_level_in;

    // present security options in list window
    for (int index = 0; index < m_internal_list.GetSize(); index++)
    {
        m_security_list.InsertString(index, m_internal_list[index].m_display_name);
        m_security_list.SetItemData(index, m_internal_list[index].m_flag);
        // special for no security
        if (index == 0 && m_security_level_out == BTM_SEC_NONE)
            m_internal_list[index].m_selected = TRUE;
        else
            m_internal_list[index].m_selected =
                m_security_level_out & m_internal_list[index].m_flag;
        m_security_list.SetSel(index, m_internal_list[index].m_selected);
    }

    m_security_list.EnableWindow( TRUE );

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSecurityDlg::OnSelchangeSecurityList() 
{
	// TODO: Add your control notification handler code here
    UINT8 new_sels = 0;
    UINT8 prev_sels = 0;
    UINT8   new_level = 0;
    int index;

    // this event triggers for each select/deselect in the list box
    // so only one flag is changed

    // build masks of prev sels and new sels (ignore no security entry)
    for (index = 1; index < m_internal_list.GetSize(); index++)
    {
        if (m_internal_list[index].m_selected)
            prev_sels |= m_internal_list[index].m_flag;
        if (m_security_list.GetSel(index))
            new_sels |= m_internal_list[index].m_flag;
    }

	// enforce requirements that encryption requires the corresponding authentication
	// if incoming encryption turned on, then incoming authentication must also be on
	if (new_sels & BTM_SEC_IN_ENCRYPT && !(prev_sels & BTM_SEC_IN_ENCRYPT))
		new_sels |= BTM_SEC_IN_AUTHENTICATE;

	// if incoming authenticaton turned off then incoming encryption must be off
	if (!(new_sels & BTM_SEC_IN_AUTHENTICATE) && (prev_sels & BTM_SEC_IN_AUTHENTICATE))
		new_sels &= ~(BTM_SEC_IN_ENCRYPT | BTM_SEC_IN_AUTHENTICATE);

	// if outgoing encryption turned on, then outgoing authentication must also be on
	if (new_sels & BTM_SEC_OUT_ENCRYPT && !(prev_sels & BTM_SEC_OUT_ENCRYPT))
		new_sels |= BTM_SEC_OUT_AUTHENTICATE;

	// if outgoing authenticaton turned off then outgoing encryption must be off
	if (!(new_sels & BTM_SEC_OUT_AUTHENTICATE) && (prev_sels & BTM_SEC_OUT_AUTHENTICATE))
		new_sels &= ~(BTM_SEC_OUT_ENCRYPT | BTM_SEC_OUT_AUTHENTICATE);


    // the 'no security' option is special
    if (!m_internal_list[0].m_selected && m_security_list.GetSel(0)
        ||
        new_sels == 0   // all real options deselected
        )
    {
        // the no security option went from non selected to selected
        m_internal_list[0].m_selected = TRUE;
        new_sels = 0;           // clear all other options
        m_security_level_out = BTM_SEC_NONE;
    }
    else
    {
        m_internal_list[0].m_selected = FALSE;
        m_security_level_out = new_sels;
    }


    // set the other flags in the internal array
    for (index = 1; index < m_internal_list.GetSize(); index++)
    {
        int flag = m_internal_list[index].m_flag;
        m_internal_list[index].m_selected = new_sels & m_internal_list[index].m_flag;
    }

    // replace the level avail to the caller
    m_security_level_out = new_sels;

    PresentNewList ();
}

void CSecurityDlg::PresentNewList()
{
    // set all 'selected' choices in display list from the internal array
    for (int index = 0; index < m_internal_list.GetSize(); index++)
    {
        if (m_internal_list[index].m_selected)
            m_security_list.SetSel(index, TRUE);
        else
            m_security_list.SetSel(index, FALSE);
    }
}


