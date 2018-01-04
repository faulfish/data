// RSDInterface.h: interface for the CRSDInterface class.
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


#if !defined(AFX_RSDINTERFACE_H__601EA351_0542_4D5A_9902_7106A77ACE86__INCLUDED_)
#define AFX_RSDINTERFACE_H__601EA351_0542_4D5A_9902_7106A77ACE86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <initguid.h>
#include <PSTCommon.h>  // PST APIs header file
#include <Flash_API.h>  // Flash APIs header file

// the GUID that is used to identify this device's interface  //DL mode
DEFINE_GUID(GUID_CLASS_P2K, 0x4E1E1E15, 0x52D7, 0x11D4, 0x85, 0x2D, 0x00, 0x10, 0x5A, 0xA6, 0xF8, 0x0B);

//const CString STR_GUID_MSD = "{53f56307-b6bf-11d0-94f2-00a0c91efb8b}"
DEFINE_GUID(GUID_CLASS_MSD, 0x53f56307, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

//const CString STR_GUID_P2KMDM = "{2C7089AA-2E0E-11D1-B114-00C04FC2AAE4}" // normal mode
DEFINE_GUID(GUID_CLASS_P2KMDM, 0x2C7089AA, 0x2E0E, 0x11D1, 0xB1, 0x14, 0x00, 0xC0, 0x4F, 0xC2, 0xAA, 0xE4);

//const CString STR_GUID_NDI = "{ad498944-762f-11d0-8dcb-00c04fc3358c}"
DEFINE_GUID(GUID_CLASS_NDI, 0xad498944, 0x762f, 0x11d0, 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c);

//const CString STR_GUID_HUB = {f18a0e88-c30c-11d0-8815-00a0c906bed8}
DEFINE_GUID(GUID_CLASS_HUB, 0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);

//const CString STR_ALADDIN_GUID = "{F0F93469-A69A-490e-A830-4ABA-ECEA1AF6}"
DEFINE_GUID(GUID_CLASS_ALADDIN_OBEX, 0xF0F93469, 0xA69A, 0x490e, 0xA8, 0x30, 0x4A, 0xBA, 0xEC, 0xEA, 0x1A, 0xF6);

//const CString STR_ALADDIN_GUID ="{377A488A-350B-4f24-997D-A97B-798F93FA}"
DEFINE_GUID(GUID_CLASS_ALADDIN_OMT, 0x377A488A, 0x350B, 0x4f24, 0x99, 0x7D, 0xA9, 0x7B, 0x79, 0x8F, 0x93, 0xFA);

//const CString STR_MCPC_AT_GUID = "{633DF81D-808E-45db-0x9D06-A9F4DDEB9A4E}"
DEFINE_GUID(GUID_CLASS_MCPC_AT, 0x633DF81D, 0x808E, 0x45db, 0x9D, 0x06, 0xA9, 0xF4, 0xDD, 0xEB, 0x9A, 0x4E);

//const CString STR_MCPC_MDM_GUID = "{0AC4E3E4-7555-4ACD -BDA0 -AD5D4DBBEB88}"
DEFINE_GUID(GUID_CLASS_MCPC_MDM, 0x0AC4E3E4, 0x7555, 0x4ACD, 0xBD, 0xA0, 0xAD, 0x5D, 0x4D, 0xBB, 0xEB, 0x88);

//const CString STR_MCPC_FOMA_COML = "{0x43102F77-BCF2-402f-BA62-C67A-6696-56BC);
DEFINE_GUID(GUID_CLASS_MCCI_FOMA_COML,0x43102F77,  0xBCF2,	0x402f, 0xBA,   0x62,0xC6, 0x7A, 0x66, 0x96, 0x56,0xBC);

// {8DA8AE77-D0ED-4673-8778-0C9DA9F967ED}
DEFINE_GUID(GUID_CLASS_MOTSWCH,0x8da8ae77, 0xd0ed, 0x4673, 0x87, 0x78, 0xc, 0x9d, 0xa9, 0xf9, 0x67, 0xed);

// MTP (media transfer protocol) interface class GUID
DEFINE_GUID(GUID_CLASS_MTP, 0xF33FDC04, 0xD1AC, 0x4E8E, 0x9A, 0x30, 0x19, 0xBB, 0xD4, 0xB1, 0x08, 0xAE);

// Digital Still Camera / Imaging device
DEFINE_GUID(GUID_DEVINTERFACE_IMAGE, 0x6bdd1fc6L, 0x810f, 0x11d0, 0xbe, 0xc7, 0x08, 0x00, 0x2b, 0xe2, 0x09, 0x2f);


typedef EPSTStatus (*FPPSTInitializeAndRegisterEx)(const PSTInitializeStruct&);
typedef EPSTStatus (*FPPSTUninitialize)();
typedef EPSTStatus (*FPPSTNotifyOnDeviceChange)(MotDeviceNotificationStruct*);
typedef EPSTStatus (*FPPSTProcessDevice)(DEVICEID, MotDeviceNotificationStruct*);
typedef EPSTStatus (*FPPSTIgnoreDevice)(DEVICEID, MotDeviceNotificationStruct*);
typedef EPSTStatus (*FPPSTEnableBlankFlashTIROM)(BOOL);
typedef EPSTStatus (*FPPSTRegisterForNotifications)(HWND hWnd);
typedef EPSTStatus (*FPPSTUnregisterForNotifications)(HWND hWnd);


typedef EFlashStatus (*FPUTCInit)(HWND, void*);
typedef EFlashStatus (*FPUTCClose)();
typedef EFlashStatus (*FPFlashDeviceWOptions)(UINT, eActAftFlash, CFlashErr&, va_list, eExtActDuringFlash);


class CRSDInterface  
{
public:  // ctor/dtor
    CRSDInterface();
    virtual ~CRSDInterface();

    BOOL Initialize();
    BOOL UnInitialize();

    //static CString GetPathPrefix(const CString& sSymbolicLink);
    //static CString QueryKey(CString& strKey,const CString& strDevice); //recursive

public:  // register/unregister devices RSD interested
    //void RegisterDevice(const CString& sSymbolicLink, UINT nDeviceId);
    //void UnRegisterDevice(const CString& sSymbolicLink, UINT nDeviceId);

public:  // PST/Flash APIs
    FPPSTInitializeAndRegisterEx	PSTInitializeAndRegisterEx;
    FPPSTUninitialize				PSTUninitialize;
    FPPSTNotifyOnDeviceChange       PSTNotifyOnDeviceChange;
    FPPSTProcessDevice				PSTProcessDevice;
    FPPSTIgnoreDevice				PSTIgnoreDevice;
    FPPSTEnableBlankFlashTIROM		PSTEnableBlankFlashTIROM;
	FPPSTRegisterForNotifications   PSTRegisterForNotifications;
	FPPSTUnregisterForNotifications PSTUnregisterForNotifications;


    FPUTCInit						UTCInit;
    FPUTCClose						UTCClose;
    FPFlashDeviceWOptions			FlashDeviceWOptions;

    //CRSDUSBNotify*  m_pUSBNotify;
protected:
    void ResetMembers();

    BOOL LoadDlls();
    BOOL InitRSD();
    BOOL UnInitRSD();

protected:
    HMODULE         m_hPSTDll;
    HMODULE         m_hFlashDll;


    
};

#endif // !defined(AFX_RSDINTERFACE_H__601EA351_0542_4D5A_9902_7106A77ACE86__INCLUDED_)

