// RSDUSBNotify.h: interface for the CRSDUSBNotify class.
//
//////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------

                            Motorola Confidential Proprietary
                       Advanced Technology and Software Operations
                    (c) Copyright Motorola 2001-2008, All Rights Reserved
   

 Revision History:
                       Modification     Tracking
 Author                   Date          Number     Description of Changes
-------------------   ------------    ----------   ----------------------------------------
 Zhiyong Yang          10/13/2008     LIBqq78510   Initial Creation

--------------------------------------------------------------------------------------------------*/


#if !defined(AFX_RSDUSBNOTIFY_H__6AE2296D_FD36_43B7_8420_ADF137A116BF__INCLUDED_)
#define AFX_RSDUSBNOTIFY_H__6AE2296D_FD36_43B7_8420_ADF137A116BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxmt.h>
#include <afxtempl.h>
#include <dbt.h>


#define ENUMUSBKEY "SYSTEM\\CurrentControlSet\\Enum\\USB"
#define MAX_VALUE_NAME 16383


class CRSDInterface;

class CRSDUSBNotify : public CWnd  
{

public:
    CRSDUSBNotify(CRSDInterface* pRSDInterface);
    virtual ~CRSDUSBNotify();

    void RegisterDevice(const CString& sPathPrefix, UINT nDeviceId);
    void UnRegisterDevice(const CString& sPathPrefix, UINT nDeviceId);
    //void GetUSBInterfaceList(const GUID*  pDeviceClsName, CStringArray &interfaceList);

    DEV_BROADCAST_DEVICEINTERFACE m_DeviceInterface;


	CString GetPathPrefix(const CString& sSymbolicLink);
	CString QueryKey(CString& strKey,const CString& strDevice); //recursive

private:
	BOOL RegisterNotification();
	BOOL UnRegisterNotification();

protected:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUSBNOTIFYApp)
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CUSBNOTIFYApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG

    afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);

    DECLARE_MESSAGE_MAP()

protected:
    CRSDInterface* m_pRSDInterface;
	
    HDEVNOTIFY m_hNotifyDev;

    CCriticalSection m_csDeviceMap;
    CMap<CString, LPCTSTR, UINT, UINT> m_mapDevice;   // record the devices RSD interested

private:
	CCriticalSection m_csQueryRegistry;
	BOOL m_bFound;      
	CString m_strHubName;
};

#endif // !defined(AFX_RSDUSBNOTIFY_H__6AE2296D_FD36_43B7_8420_ADF137A116BF__INCLUDED_)

