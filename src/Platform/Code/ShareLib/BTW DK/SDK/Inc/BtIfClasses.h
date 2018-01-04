/////////////////////////////////////////////////////////////////////////////
//
//  Name        BtIfClasses.h
//  $Header:
//
//  Function    this file contains SDK class definitions
//
//  Date                 Modification
//  ----------------------------------
//  12/17/2000    JF   Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _BTIFCLASSES_H
#define _BTIFCLASSES_H


#ifndef _BTWLIB
#ifdef WIDCOMMSDK_EXPORTS
#define WIDCOMMSDK __declspec(dllexport)
#else
#define WIDCOMMSDK __declspec(dllimport)
#endif
#else 
#define WIDCOMMSDK
#endif


#include "com_error.h"
#include "BtIfDefinitions.h"

// Resolve forward references
//
class CSdpDiscoveryRec;


// Ensure alignment across all builds
//
#pragma pack (1)
#define BT_CHAR char
#define BT_CB_LPSTR  WCHAR *  // char used in call back function

class CWBtAPI;
//
// Define status values that are returned in the "OnDeviceStatus" callback
//
#define BT_DEVST_DOWN               0           // Device is present, but down
#define BT_DEVST_UP                 1           // Device is present and UP
#define BT_DEVST_ERROR              2           // Device is in error (maybe being removed)
#define BT_DEVST_UNLOADED           3           // Stack is being unloaded

// Define master/slave role code for SwitchRole function
//
typedef enum
{
    NEW_MASTER,
    NEW_SLAVE
} MASTER_SLAVE_ROLE;

// Define return code for Audio functions
//
typedef enum
{
    AUDIO_SUCCESS,
    AUDIO_UNKNOWN_ADDR,     // if the ACL connection is not up
    AUDIO_BUSY,             // if another SCO being set up to the same BD address
    AUDIO_NO_RESOURCES,     // if the max SCO limit has been reached
    AUDIO_ALREADY_STARTED,  // connection is already up.
    AUDIO_UNKNOWN_ERROR,    // unknown error, or code not supported in this version
    AUDIO_INVALID_PARAM,    // one of the API parameters was invalid                 - added BTW 4.0.1.1400, SDK 4.0
    AUDIO_MISMATCH_ADDR,    // connection attempt for specific bda does not match 
                            // remote addr on connected ACL link                     - added BTW 4.0.1.1400, SDK 4.0
    AUDIO_MODE_UNSUPPORTED, // The controller version is not BT1.2 or later
                            // or does not support eSCO                              - added BTW 5.0.1.200, SDK 5.0
    AUDIO_WRONG_MODE,       // There is no connection with a peer device
                            // or stack reports bad audio handle                     - added BTW 5.0.1.200, SDK 5.0
    AUDIO_INVALID_HANDLE = 0xffff   // The audio handle is invalid
} AUDIO_RETURN_CODE;
  

// Define common return code for new SDK functions that would normally return BOOL
typedef enum
{
    SDK_SUCCESS,            // The call was successful
    SDK_FAIL,               // Unspecified failure
    SDK_NOT_SUPPORTED,      // The API is not supported on the platform BTW stack version
    SDK_BUSY,               // The API cannot complete at this time, but may be retried
    SDK_INVALID_PARAM,      // One of the API parameters was invalid
    SDK_ERR_RESOURCES,      // A necessary resource could not be obtained
    SDK_TIMEOUT,            // The operation timed out before completion
} SDK_RETURN_CODE;                              // Added BTW and SDK 5.0.1.1100

WIDCOMMSDK  void WIDCOMMSDK_ShutDown(void);     // BTW SDK 4.0 - stubs for compatibility -
WIDCOMMSDK  void WIDCOMMSDK_Init(void);         // deprecated and no longer necessary

WIDCOMMSDK  BOOL IsBroadcomStack();             // Added 6.1.0.500 SDK, supported on all stacks

////////////////////////////////////////////////////////////////////////////
// 
// Define a class for interfacing to the stack
//
class WIDCOMMSDK CBtIf
{
public:
    CBtIf();
    virtual ~CBtIf();

    // Application can use this function to start an inquiry.
    BOOL StartInquiry();

    // Application can use this function to stop an inquiry.
    void StopInquiry();

    // Application can use this function to start service discovery
    BOOL StartDiscovery (BD_ADDR p_bda, GUID *p_service_guid);

    // Define return code for Bond function
    //
    typedef enum
    {
        SUCCESS,
        BAD_PARAMETER,
        NO_BT_SERVER,
        ALREADY_BONDED,     // maintained for compatibility, BTW stack allows rebonding
        FAIL,
        REPEATED_ATTEMPTS   // pairing has failed repeatedly, and further attempts will
                            // continue to return this code until after the device security
                            // timeout                  - added BTW 5.0.1.700, SDK 5.0
    } BOND_RETURN_CODE;

    typedef enum
    {
        REM_DEV_INFO_SUCCESS,   // success response
        REM_DEV_INFO_EOF,       // no more devices found
        REM_DEV_INFO_ERROR,     // can not find exsiting entry for bda provided as input
        REM_DEV_INFO_MEM_ERROR  // out of memory
    } REM_DEV_INFO_RETURN_CODE;

    typedef struct
    {
        BD_ADDR     bda;
        DEV_CLASS   dev_class;
        BD_NAME     bd_name;
        BOOL        b_paired;
        BOOL        b_connected;
    } REM_DEV_INFO;

    typedef struct
    {
        BD_ADDR      bd_addr;
        UINT8        hci_version;
        UINT16       hci_revision;
        UINT8        lmp_version;
        UINT16       manufacturer;
        UINT16       lmp_sub_version;
    } DEV_VER_INFO;

    typedef enum
    {
        LINK_MODE_NORMAL,
        LINK_MODE_HOLD,
        LINK_MODE_SNIFF,
        LINK_MODE_PARK
    } LINK_MODE;
    
    /////////////////////////////////////////////////
    // Begin BondEx support - added SDK 6.1.0.1503
    /////////////////////////////////////////////////
    typedef enum
    {
        BOND_EVT_PIN_CODE_REQ,  // legacy pin code authentication
        BOND_EVT_CONFIRM_REQ,   // numeric compare authentication
        BOND_EVT_PASSKEY_REQ    // passkey authentication
    } eBOND_CB_EVENT;

    typedef void (tBond_CB) (eBOND_CB_EVENT eEvent, void * user_data, UINT32 event_data);

    BOND_RETURN_CODE BondEx(BD_ADDR bda, tBond_CB *BondCb, void * user_data) ;

    typedef enum
    {
        BOND_CONFIRM_ALLOW,     // numeric compare, allow authentication
        BOND_CONFIRM_DISALLOW,  // numeric compare, disallow authentication
        BOND_PIN_ALLOW,         // legacy pin code sent
        BOND_PIN_DISALLOW       // legacy pin code sent, disallow authentication
    } eBOND_REPLY;  
    
    void BondReply(eBOND_REPLY reply, UINT32 nPinLength=0, UCHAR * szPin=NULL);
    /////////////////////////////////////////////////
    // End BondEx support - added SDK 6.1.0.1503
    /////////////////////////////////////////////////

    BOND_RETURN_CODE Bond(BD_ADDR bda, BT_CHAR* pin_code); // deprecated, does not support SSP, use BondEx()

    // query if a device is bonded
    BOOL BondQuery(BD_ADDR bda);

    // Remove Bonding
    BOOL UnBond(BD_ADDR bda);

    // Create AudioConnection
    AUDIO_RETURN_CODE CreateAudioConnection(BD_ADDR bda, BOOL bIsClient, UINT16 *audioHandle);

    // Disconnect AudioConnection
    static AUDIO_RETURN_CODE RemoveAudioConnection(UINT16 audioHandle);

    // audio callback functions
    virtual void OnAudioConnected(UINT16 audioHandle){};
    virtual void OnAudioDisconnect(UINT16 audioHandle){};

    // Application can use this function to get list of services on the remote device
    int ReadDiscoveryRecords (BD_ADDR p_bda, int max_size, CSdpDiscoveryRec *p_list, GUID *p_guid_filter = NULL);

    typedef enum {
        DISCOVERY_RESULT_SUCCESS,
        DISCOVERY_RESULT_CONNECT_ERR,           // Could not connect to remote device 
        DISCOVERY_RESULT_CONNECT_REJ,           // Remote device rejected the connection 
        DISCOVERY_RESULT_SECURITY,              // Security failed 
        DISCOVERY_RESULT_BAD_RECORD,            // Remote Service Record Error 
        DISCOVERY_RESULT_OTHER_ERROR            // Other error
    } DISCOVERY_RESULT;

    // application can use this function from within the OnDiscoveryComplete callback
    // to find out the discovery results
    CBtIf::DISCOVERY_RESULT GetLastDiscoveryResult(BD_ADDR p_bda, UINT16 *p_num_recs);

    // server should call this method to switch role to master if
    // they want to accept multiple connections
    static BOOL SwitchRole(BD_ADDR bd_addr, MASTER_SLAVE_ROLE new_role);

    virtual void OnInquiryComplete (BOOL success, short num_responses) {}// {}
    virtual void OnDeviceResponded (BD_ADDR bda, DEV_CLASS devClass, BD_NAME bdName, BOOL bConnected) {} // = 0;
    virtual void OnDiscoveryComplete () {}// = 0;
    virtual void OnDiscoveryComplete (UINT16 nRecs, long lResultCode) {}// = 0;

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }

    // allow to send vendor specific HCI_cmd
    typedef enum {
        SENDVENDOR_HCICMD_SUCCESS,
        SENDVENDOR_HCICMD_FAILURE,
        SENDVENDOR_HCICMD_UNKOWN_PLATFORM,   // Platform is unkown, (ie. Not Win2k, Win9x)
        SENDVENDOR_HCICMD_NO_SUPPORTED,      // The HCI command being sent is not
                                             // supported.
        SENDVENDOR_HCICMD_BTRKNL_NOT_OPENED,
        SENDVENDOR_HCICMD_BTKRNL_FAILURE,    // Unable to open or talk to btkrnl.sys
        SENDVENDOR_HCICMD_DEVICE_FAILURE,    // Unable to open or talk to the device.
        SENDVENDOR_HCICMD_NO_RESOURCE,       // memalloc failed
        SENDVENDOR_HCICMD_BUFFER_TOO_BIG,    // input buffer is lager than max_command for HCI command  - added SDK 5.0, BTW 5.0.1.702
        SENDVENDOR_HCICMD_BUFFER_TOO_SMALL,  // out buffer is too small to hold return data             - added SDK 5.0, BTW 5.0.1.702
        SENDVENDOR_HCICMD_INVALID_PARAM      // one of the parameters is invalid                        - added SDK 5.0, BTW 5.0.1.702
    } SENDVENDOR_HCICMD_RETURN_CODE;
    
    SENDVENDOR_HCICMD_RETURN_CODE SendVendorSpecific_HCICMD(UINT16 OpCode, UINT8 *InParam, UINT8 InParamLen,
                                                            UINT8 * pOutBuff, UINT8 outBuffLen);

    // sets the public variable m_BdAddr
    BOOL GetLocalDeviceInfo();
    BOOL GetLocalDeviceName(BD_NAME *BdName);
    BOOL SetLocalDeviceName(BD_NAME BdName);        // deactivated SDK 6.1
    
    typedef void (tAudio_CB) (UINT16);
    static AUDIO_RETURN_CODE CreateAudioConnection(BD_ADDR bda, BOOL bIsClient, UINT16 *audioHandle,
                                                   tAudio_CB *p_conn_cb, tAudio_CB *p_disc_cb);
     // eSCO functions
    static AUDIO_RETURN_CODE RegForEScoEvts (UINT16 audioHandle, tBTM_ESCO_CBACK *p_esco_cback);        // added SDK 5.0, BTW 5.0.1.200
    static AUDIO_RETURN_CODE SetEScoMode (tBTM_SCO_TYPE sco_mode, tBTM_ESCO_PARAMS *p_parms);           // added SDK 5.0, BTW 5.0.1.200
    static AUDIO_RETURN_CODE ReadEScoLinkData (UINT16 audioHandle, tBTM_ESCO_DATA *p_data);             // added SDK 5.0, BTW 5.0.1.200
    static AUDIO_RETURN_CODE ChangeEScoLinkParms (UINT16 audioHandle, tBTM_CHG_ESCO_PARAMS *p_parms);   // added SDK 5.0, BTW 5.0.1.200
    static void EScoConnRsp (UINT16 audioHandle, UINT8 hci_status, tBTM_ESCO_PARAMS *p_parms = NULL);   // added SDK 5.0, BTW 5.0.1.200
    
    static BOOL GetBTWVersionInfo(BT_CHAR *p_version_info, int nSize);
    static BOOL GetDKVersionInfo(BT_CHAR *p_version_info, int nSize);

    static BOOL SetLinkSupervisionTimeOut(BD_ADDR BdAddr, UINT16 timeout);
    static BOOL SetSniffMode (BD_ADDR bd_Addr);                                 // added SDK 4.0, BTW 4.0.1.1000
    static BOOL CancelSniffMode (BD_ADDR bd_Addr);                              // added SDK 4.0, BTW 4.0.1.1000
    static BOOL ReadLinkMode(BD_ADDR bd_Addr, UINT8 *pMode);                    // added SDK 5.0, BTW 5.0.1.900
    BOOL GetLocalServiceName(BT_CHAR *p_ServiceName, int bBuffLen);
    BOOL GetNextLocalServiceName(BT_CHAR *p_ServiceName, int bBuffLen);

    REM_DEV_INFO_RETURN_CODE GetRemoteDeviceInfo(DEV_CLASS dev_class_filter, REM_DEV_INFO *p_rem_dev_info);
    REM_DEV_INFO_RETURN_CODE GetNextRemoteDeviceInfo(REM_DEV_INFO *p_rem_dev_info);
    BOOL CreateCOMPortAssociation(BD_ADDR bda, GUID *p_guid, LPCSTR szServiceName, USHORT mtu, BYTE SecurityID,
                             BYTE SecurityLevel, USHORT uuid, USHORT *p_com_port);
    BOOL RemoveCOMPortAssociation(USHORT com_port);
    BOOL ReadCOMPortAssociation(tBT_REM_ASSOC_REC *pBuffList, DWORD dwBuffSize, DWORD *pdwRequiredSize);

    BOOL Add_Printer(LPCSTR PortName, BD_ADDR bd_Addr);
    BOOL Remove_Printer(BD_ADDR bd_Addr);
    BOOL IsDeviceReady();                                                     // added BTW 3.0.0.300, SDK 3.0.1.901
    BOOL GetLocalDeviceVersionInfo(DEV_VER_INFO *p_Dev_Ver_Info);             // added BTW 3.0.0.1300, SDK 3.0.1.901
    BOOL GetRemoteDeviceVersionInfo(BD_ADDR bd_addr_remote, 
                                    DEV_VER_INFO *p_Dev_Ver_Info);            // added BTW 3.0.0.1400, SDK 3.0.1.901
    BOOL IsStackServerUp();                                                   // added BTW 3.0.0.1300, SDK 3.0.1.901
    BOOL GetConnectionStats (BD_ADDR bd_Addr, tBT_CONN_STATS *p_conn_stats);  // added BTW 3.0.0.1500, SDK 3.0.1.901
    BOOL IsClientEnabled(UINT32 uClientID);   // unsupported method, for internal use only

    typedef void (tLINK_STATUS_CB)(BD_ADDR remote_bda, BOOL bLinkUp);
    BOOL IsRemoteDeviceConnected(BD_ADDR bd_addr_remote);                                // added BTW 5.0.1.300, SDK 5.0
    BOOL RegisterForLinkStatusChanges(tLINK_STATUS_CB *p_link_status_cb, BD_ADDR bda);   // added BTW 5.0.1.300, SDK 5.0

    SDK_RETURN_CODE IsRemoteDevicePresent(BD_ADDR bd_addr_remote);      // added BTW and SDK 5.0.1.1000


private:
    CWBtAPI                 *m_pBtApi;
    

public:
    typedef enum {
        DEVST_DOWN,     // Device is present, but down
        DEVST_UP,       // Device is present and UP
        DEVST_ERROR,    // Device is in error (maybe being removed)
        DEVST_UNLOADED, // Stack is being unloaded
        DEVST_RELOADED  // Stack reloaded after being unloaded
    } STACK_STATUS;

    virtual void OnStackStatusChange(CBtIf::STACK_STATUS new_status) {}

    // standard GUID values for common Bluetooth service classes
    static const GUID guid_SERVCLASS_SERVICE_DISCOVERY_SERVER;
    static const GUID guid_SERVCLASS_BROWSE_GROUP_DESCRIPTOR;
    static const GUID guid_SERVCLASS_PUBLIC_BROWSE_GROUP;
    static const GUID guid_SERVCLASS_SERIAL_PORT;    
    static const GUID guid_SERVCLASS_LAN_ACCESS_USING_PPP;
    static const GUID guid_SERVCLASS_PANU;
    static const GUID guid_SERVCLASS_NAP;
    static const GUID guid_SERVCLASS_GN;
    static const GUID guid_SERVCLASS_DIALUP_NETWORKING;
    static const GUID guid_SERVCLASS_IRMC_SYNC;
    static const GUID guid_SERVCLASS_OBEX_OBJECT_PUSH;
    static const GUID guid_SERVCLASS_OBEX_FILE_TRANSFER;
    static const GUID guid_SERVCLASS_IRMC_SYNC_COMMAND;
    static const GUID guid_SERVCLASS_HEADSET;
    static const GUID guid_SERVCLASS_CORDLESS_TELEPHONY;
    static const GUID guid_SERVCLASS_INTERCOM;
    static const GUID guid_SERVCLASS_FAX;
    static const GUID guid_SERVCLASS_HEADSET_AUDIO_GATEWAY;
    static const GUID guid_SERVCLASS_PNP_INFORMATION;
    static const GUID guid_SERVCLASS_GENERIC_NETWORKING;
    static const GUID guid_SERVCLASS_GENERIC_FILETRANSFER;
    static const GUID guid_SERVCLASS_GENERIC_AUDIO;
    static const GUID guid_SERVCLASS_GENERIC_TELEPHONY;
    static const GUID guid_SERVCLASS_BPP_PRINTING;
    static const GUID guid_SERVCLASS_HCRP_PRINTING;
    static const GUID guid_SERVCLASS_SPP_PRINTING;
    static const GUID guid_SERVCLASS_HUMAN_INTERFACE;
    static const GUID guid_SERVCLASS_AUDIO_SINK;
                      

    BD_ADDR m_BdAddr;   // Bluetooth address of local device
    BD_NAME m_BdName;

    long    m_DeviceState;
        
protected:
    void SetOnDeviceStatusCallback();
    BD_ADDR                 m_DiscoveryBdAddr;
    UINT16                  m_DiscoveryNumRecs;
    DISCOVERY_RESULT        m_DiscoveryResult;
   
private:
    BOOL    m_bInquiryActive;
    BOOL    m_bDiscoveryActive;

    BOOL    m_tempRegistry;
    GUID    m_TempRegistryGUID;
    void    DeleteTempRegistry();
    BOOL    AddTempRegistry(const GUID *p_GUID);
    UINT16  m_audioHandle;
    HANDLE  m_hKrnl;
    UINT32  m_KernelDriverVersion;
    WBtRc   m_LastWBtRc;    // Contains the last code returned from WBtApi
    static BOOL    GetDLLVersion(LPCSTR sDLLFileName, LPSTR lpVersionInfo);
    BT_CHAR m_szKeyName[MAX_PATH];
    DEV_CLASS m_dev_class_filter;
    BOOL    GetRemoteDeviceName(BD_ADDR remote_bda, BD_NAME *remote_name, DEV_CLASS *devClass);

    BOOL    m_bStackServerConnected;
    BOOL    ConnectToStackServer();
    void    DeleteWbtApi();
    HANDLE  m_hMutex;

    tLINK_STATUS_CB *m_pLinkStatusCb;
    BD_ADDR m_link_status_bda_filter;
    BYTE* m_devAddr;
    DWORD m_devAddrCount;

    HANDLE m_hDevNotify;
    HWND m_hDevNotifyWnd;
    BOOL PP_AclRegisterForChanges(BOOL bRegister);

    friend class CBtIfFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CBtIf(const CBtIf & x);
    CBtIf& operator= (const CBtIf & x);
};


////////////////////////////////////////////////////////////////////////////
//
// Define a class to control an L2CAP interface (for a specific PSM)
//
class WIDCOMMSDK CL2CapIf  
{
public:
    CL2CapIf();
    ~CL2CapIf();

    // Server should call this method without any parameter
    // to assign a new PSM value, or with a PSM value if it
    // is using a fixed PSM. Client should call this method
    // with PSM found from service discovery
    //
    // Due to a problem discovered in the Microsoft stack implementation
    // of this function, a new version has been added, AssignPsmValueEx,
    // which requires specification of the desired client/server role for
    // the PSM.  The original method is now deprecated, and if still used,
    // its behavior now assumes client/server role based on if psm = 0 (client
    // assumed) or non-zero (server assumed), for Microsoft stack deployments
    //
    /* Deprecated */ BOOL    AssignPsmValue (GUID *p_guid, UINT16 psm = 0);      
    BOOL    AssignPsmValueEx (GUID *p_guid, UINT16 psm, BOOL is_server);      // Added 6.1.0.1502 SDK, supported on all stacks

    // Both client and server sides should call this function
    // to register a PSM with L2CAP, once the PSM value is known
    //
    BOOL    Register ();

    // Both client and server sides should call this function
    // to de-register a PSM from L2CAP, when application is exiting
    //
    void    Deregister ();

    // Both client and server MUST call this function to set
    // the security level for connections on the assigned PSM.
    //
    BOOL SetSecurityLevel (BT_CHAR *p_service_name, UINT8 security_level, BOOL is_server);

    // Returns the PSM value currently in use.
    //
    inline UINT16 GetPsm() { return m_psm; }

private:
    UINT16      m_psm;
    BOOL        m_is_registered;
    CL2CapIf   *m_p_next_psm;
    GUID        m_service_guid;
    UINT8       m_security_index;
    UINT8       m_security_level;
    BT_CHAR     m_p_service_name[BT_MAX_SERVICE_NAME_LEN+1];
    
    friend class CL2CapConn;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CL2CapIf(const CL2CapIf & x);
    CL2CapIf& operator= (const CL2CapIf & x); 
};



////////////////////////////////////////////////////////////////////////////
//
// Define a class to control the L2CAP connections (both client and server
//
class WIDCOMMSDK  CL2CapConn
{
public:

    // Construction/destruction
    //
    CL2CapConn ();
    virtual ~CL2CapConn();

    // Public variables
    //
    BOOL        m_isCongested;
    UINT16      m_RemoteMtu;
    BD_ADDR     m_RemoteBdAddr;

    // Server should call this method to listen for an incoming
    // connection. Client should not call this method.
    //
    BOOL    Listen (CL2CapIf *p_if);

    // Server should call this method to switch role to master if
    // it wants to accept multiple connections
    //
    BOOL    SwitchRole(MASTER_SLAVE_ROLE new_role);

    BOOL    SetLinkSupervisionTimeOut(UINT16 timeout);

    // Create AudioConnection
    AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle);
    AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle, BD_ADDR bda);  // added BTW 4.0.1.1400, SDK 4.0

    // Disconnect AudioConnection
    AUDIO_RETURN_CODE RemoveAudioConnection(UINT16 audioHandle);

    // audio callback functions
    virtual void OnAudioConnected(UINT16 audioHandle){};
    virtual void OnAudioDisconnect(UINT16 audioHandle){};

    // eSCO functions
    AUDIO_RETURN_CODE RegForEScoEvts (UINT16 audioHandle, tBTM_ESCO_CBACK *p_esco_cback);       // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE SetEScoMode (tBTM_SCO_TYPE sco_mode, tBTM_ESCO_PARAMS *p_parms);          // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE ReadEScoLinkData (UINT16 audioHandle, tBTM_ESCO_DATA *p_Data);            // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE ChangeEScoLinkParms (UINT16 audioHandle, tBTM_CHG_ESCO_PARAMS *p_parms);  // added SDK 5.0, BTW 5.0.1.200
    void EScoConnRsp (UINT16 audioHandle, UINT8 hci_status, tBTM_ESCO_PARAMS *p_parms = NULL);  // added SDK 5.0, BTW 5.0.1.200

    // Server should call this method to accept an incoming
    // connection, after he is notified of that connection.
    // If anything other than the default MTU is desired,
    // it should be passed as a parameter.
    //
    BOOL    Accept (UINT16 desired_mtu = L2CAP_DEFAULT_MTU);

    // Server should call this method to reject an incoming
    // connection, after he is notified of that connection.
    //
    BOOL    Reject (UINT16 reason);

    // Client should call thi smethod to create a connection
    // to a remote device. If anything other than the default 
    // MTU is desired, it should be passed as a parameter
    //
    BOOL    Connect (CL2CapIf *p_if, BD_ADDR p_bd_addr, UINT16 desired_mtu = L2CAP_DEFAULT_MTU);

    // Client or server may call this function to reconfigure
    // an existing connection.
    //
    BOOL    Reconfigure (tL2CAP_CONFIG_INFO *p_cfg);

    // Client or server may call this function to disconnect
    // an existing connection.
    //
    void    Disconnect (void);

    // Client or server may call this function to send data to
    // an existing connection.
    //
    BOOL    Write (void *p_data, UINT16 length, UINT16 *p_len_written);

    // Get Current Connection Statistics
    //
    BOOL GetConnectionStats (tBT_CONN_STATS *p_conn_stats);

    // Server may provide a function to handle incoming connection
    // notifications. Client should not.
    //
    virtual void OnIncomingConnection ();

    // Client may provide a function to handle connection pending
    // notifications.
    //
    virtual void OnConnectPendingReceived (void) {}

    // Client and server may provide a method to be notified
    // when a connection is established.
    //
    virtual void OnConnected() {}

    // Client and server may provide a method to be notified
    // when data is received from the remote side.
    //
    virtual void OnDataReceived (void *p_data, UINT16 length) {}

    // Client and server may provide a method to be notified
    // when a connection becomes congested or uncongested.
    //
    virtual void OnCongestionStatus (BOOL is_congested) {}

    // Client and server may provide a method to be notified
    // when a connection is disconnected.
    //
    virtual void OnRemoteDisconnected (UINT16 reason) {}

    // Returns L2CAP chanel ID (private member)
    // It is unsupported method for internal use only
    UINT16 GetCid(){return m_cid;}

private:
    void           SetIdle();

#define CL2CAP_STATE_IDLE           0
#define CL2CAP_STATE_LISTENING      1
#define CL2CAP_STATE_WAITING_ACCEPT 2
#define CL2CAP_STATE_CONNECTING     3
#define CL2CAP_STATE_CONFIG         4
#define CL2CAP_STATE_CONNECTED      5

    int                 m_state;

    UINT16              m_psm;
    UINT16              m_desired_mtu;
    UINT8               m_cfg_flags;
    UINT16              m_cid;
    UINT8               m_id;

    static CL2CapConn   *m_p_first_conn;
    CL2CapConn          *m_p_next_conn;
    UINT16              m_audioHandle;
    UINT16              *m_pPendingAudioHandle;
    CL2CapIf            *m_p_if;
    
    friend class CL2CapFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CL2CapConn(const CL2CapConn & x);
    CL2CapConn& operator= (const CL2CapConn & x); 
};



////////////////////////////////////////////////////////////////////////////
//
// Define a class to create and manage SDP service records
//
class WIDCOMMSDK CSdpService
{
public:
    CSdpService();
    virtual ~CSdpService();

    // This function adds a service class ID list to a service record. The service class ID
    // list should be the first attribute added for a service record.
    //
    SDP_RETURN_CODE AddServiceClassIdList (int num_guids, GUID *p_service_guids);

    // This function adds a name field to a service record.
    //
    SDP_RETURN_CODE AddServiceName (BT_CHAR *p_service_name);// CE defs

    // This function adds a profile descriptor list to a service record.
    //
    SDP_RETURN_CODE AddProfileDescriptorList (GUID *p_profile_guid, UINT16 version);

    // This function adds an L2CAP protocol descriptor list to a service record.
    //
    SDP_RETURN_CODE AddL2CapProtocolDescriptor (UINT16 psm);

    // This function adds an RFCOMM protocol descriptor list to a service record.
    //
    SDP_RETURN_CODE AddRFCommProtocolDescriptor (UINT8 scn);

    // This function adds a generic protocol descriptor list to a service record.
    // It should be only needed if the specific RFCOMM and L2CAP functions above
    // do not suffice.
    //
    SDP_RETURN_CODE AddProtocolList (int num_elem, tSDP_PROTOCOL_ELEM *p_elem_list);

    // This function adds the additional sequence of generic protocol descriptor lists to a service record.
    // It should be only needed if the specific RFCOMM and L2CAP functions above
    // do not suffice.
    //
    SDP_RETURN_CODE AddAdditionProtoLists (int num_list_elem, tSDP_PROTO_LIST_ELEM *p_proto_list);

    // This function adds a language base to a service record.
    //
    SDP_RETURN_CODE AddLanguageBaseAttrIDList (UINT16 lang, UINT16 char_enc, UINT16 base_id);

    // This function makes a service record public browsable.
    //
    SDP_RETURN_CODE MakePublicBrowseable (void);

    // This function sets the 'service availability' field of a service record.
    //
    SDP_RETURN_CODE SetAvailability (UINT8 availability);

    // This function adds an attribute to a service record. It is intended
    // to be used to add some other attribute not covered by the existing
    // functions. Note that the parameter should be in Big Endian order.
    //
    SDP_RETURN_CODE AddAttribute (UINT16 attr_id, UINT8 attr_type, UINT32 attr_len, UINT8 *p_val);

    // This function deletes an attribute from a service record.
    //
    SDP_RETURN_CODE DeleteAttribute (UINT16 attr_id);

    // This functions add a list (sequence) for the 'supported formats' attribute
    //
    SDP_RETURN_CODE AddSupportedFormatsList(UINT8 num_formats, UINT8 pDataType[],
                                            UINT8 pDataTypeLength[], UINT8 *pDataTypeValue[]);

    SDP_RETURN_CODE CommitRecord();             // added SDK 6.1, BTW PP 6.0.1.2300

private:
    UINT32  m_sdp_record_handle;
    BOOL    m_bRecordSet;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CSdpService(const CSdpService & x);
    CSdpService& operator= (const CSdpService & x); 
};


class WIDCOMMSDK CSdpDiscoveryRec
{
public:

    CSdpDiscoveryRec();
    ~CSdpDiscoveryRec();

    BT_CHAR m_service_name[BT_MAX_SERVICE_NAME_LEN + 1];

    GUID    m_service_guid;

    BOOL    FindRFCommScn (UINT8 *pScn);

    BOOL    FindL2CapPsm (UINT16 *pPsm);

    BOOL    FindProtocolListElem (UINT16 layer_uuid, tSDP_PROTOCOL_ELEM *p_elem);

    BOOL    FindAdditionalProtocolListElem (UINT16 layer_uuid, tSDP_PROTOCOL_ELEM *p_elem);

    BOOL    FindProfileVersion (GUID *p_profile_guid, UINT16 *p_version);

    BOOL    FindAttribute (UINT16 attr_id, SDP_DISC_ATTTR_VAL *p_val);

private:

    void    *m_p_rec;
    UINT8   *m_p_free_mem;
    UINT16  m_mem_free;

    void    Create(void *p);
    UINT8   *AddDiscoveryAttr (UINT8 *pp, int len, void *p_parent, int nest_level);

    friend class CBtIf;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CSdpDiscoveryRec(const CSdpDiscoveryRec & x);
    CSdpDiscoveryRec& operator= (const CSdpDiscoveryRec & x); 
};


////////////////////////////////////////////////////////////////////////////
//
// Define a class to control the RfComm interface
//
class WIDCOMMSDK CRfCommIf  
{
public:
    CRfCommIf();
    ~CRfCommIf();

    // Server should call this method without any parameter
    // to assign a new SCN value, or with a SCN value if it
    // is using a fixed SCN. Client should call this method
    // with SCN found from service discovery
    //
    // UPDATED BTW 3.0.0.400, SDK 3.0.1.901:
    //  szServiceName parameter added to allow differentiation between 2 services using
    //  the same GUID
    //
    BOOL AssignScnValue (GUID *p_service_guid, UINT8 scn = 0, LPCSTR szServiceName = "");

    // Returns the SCN value currently in use.
    //
    inline UINT8 GetScn() { return m_scn; }

    // Both client and server MUST call this function to set
    // the security level for connections on the assigned SCN.
    //
    BOOL SetSecurityLevel (BT_CHAR *p_service_name, UINT8 security_level, BOOL is_server);

private:
    UINT8      m_scn;
    BOOL       m_scnWasAllocated;
    GUID       m_service_guid;
    UINT8      m_security_index;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CRfCommIf(const CRfCommIf & x);
    CRfCommIf& operator= (const CRfCommIf & x); 
};



///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the RFCOMM connections (both client and server)
//
class WIDCOMMSDK  CRfCommPort
{
public:

    // Construction/destruction
    //
    CRfCommPort ();
    virtual ~CRfCommPort();
//
// Define return code for RFCOMM port functions
//
typedef enum
{
    SUCCESS,
    UNKNOWN_ERROR,
    ALREADY_OPENED,         // Client tried to open port to existing DLCI/BD_ADDR
    NOT_OPENED,             // Function called before conn opened, or after closed
    HANDLE_ERROR,           // Use of a bad connection handle within SDK
    LINE_ERR,               // Line error
    START_FAILED,           // Connection attempt failed
    PAR_NEG_FAILED,         // Parameter negotiation failed, currently only MTU
    PORT_NEG_FAILED,        // Port negotiation failed
    PEER_CONNECTION_FAILED, // Connection ended by remote side
    PEER_TIMEOUT,           
    INVALID_PARAMETER,
    NOT_SUPPORTED           // not supported on Profile Pack version - added SDK 6.1
} PORT_RETURN_CODE;


    // Open the RFComm serial port as a server (i.e. listen for
    // remote side to connect to us).
    //
   PORT_RETURN_CODE  OpenServer (UINT8 scn, UINT16 desired_mtu = RFCOMM_DEFAULT_MTU);

    // Open the RFComm serial port as a client (i.e. initiate
    // the connection).
    //
   PORT_RETURN_CODE  OpenClient (UINT8 scn, BD_ADDR RemoteBdAddr, UINT16 desired_mtu = RFCOMM_DEFAULT_MTU);

   // Close the RFComm serial port
   //
   PORT_RETURN_CODE  Close(void);

   // Check if connection is up, and if so to whom
   //
   BOOL IsConnected (BD_ADDR *p_remote_bdaddr);

   // Set port flow control - application level, not low level
   //
   PORT_RETURN_CODE SetFlowEnabled (BOOL enabled);

   // Set control leads see BtIfDefinitions.h
   //
   PORT_RETURN_CODE SetModemSignal (UINT8 signal);

   // Get control lead status
   //
   PORT_RETURN_CODE GetModemStatus (UINT8 *p_signal);

   // Send an error to the peer
   //
   PORT_RETURN_CODE SendError (UINT8 errors);

   // Purge transmit queue
   //
   PORT_RETURN_CODE Purge (UINT8 purge_flags);

   // Write data
   //
   PORT_RETURN_CODE Write (void *p_data, UINT16 len_to_write, UINT16 *p_len_written);

   // Get Current Connection Statistics
   //
   PORT_RETURN_CODE GetConnectionStats (tBT_CONN_STATS *p_conn_stats);

   // App may provide these functions
   //
   virtual void OnDataReceived (void *p_data, UINT16 len) {}
   virtual void OnEventReceived (UINT32 event_code) {}
   virtual void OnFlowEnabled (BOOL enabled) {}                 // never implemented, stub maintained for compatibility

   BOOL SwitchRole(MASTER_SLAVE_ROLE new_role);

   // Create AudioConnection
   AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle);
   AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle, BD_ADDR bda);   // added BTW 4.0.1.1400, SDK 4.0

   // Disconnect AudioConnection
   AUDIO_RETURN_CODE RemoveAudioConnection(UINT16 audioHandle);

   // audio callback functions
   virtual void OnAudioConnected(UINT16 audioHandle){};
   virtual void OnAudioDisconnect(UINT16 aidioHandle){};

   // eSCO functions
   AUDIO_RETURN_CODE RegForEScoEvts (UINT16 audioHandle, tBTM_ESCO_CBACK *p_esco_cback);        // added SDK 5.0, BTW 5.0.1.200
   AUDIO_RETURN_CODE SetEScoMode (tBTM_SCO_TYPE sco_mode, tBTM_ESCO_PARAMS *p_parms);           // added SDK 5.0, BTW 5.0.1.200
   AUDIO_RETURN_CODE ReadEScoLinkData (UINT16 audioHandle, tBTM_ESCO_DATA *p_Data);             // added SDK 5.0, BTW 5.0.1.200
   AUDIO_RETURN_CODE ChangeEScoLinkParms (UINT16 audioHandle, tBTM_CHG_ESCO_PARAMS *p_parms);   // added SDK 5.0, BTW 5.0.1.200
   void EScoConnRsp (UINT16 audioHandle, UINT8 hci_status, tBTM_ESCO_PARAMS *p_parms = NULL);   // added SDK 5.0, BTW 5.0.1.200

   PORT_RETURN_CODE SetLinkSupervisionTimeOut(UINT16 timeout);

private:

   UINT8               m_scn;
   BOOL                m_is_server;
   UINT16              m_RemoteMtu;

   UINT16              m_desired_mtu;

   static CRfCommPort  *m_p_first_port;
   CRfCommPort         *m_p_next_port;

   UINT16              m_handle;
   BOOL                m_is_connected;
   BD_ADDR             m_RemoteBdAddr;
   UINT16              m_audioHandle;
   UINT16              m_handle_saved;
   UINT16              *m_pPendingAudioHandle;
   friend class CRfCommFriend;

   // This class will not support the compiler-supplied copy constructor or assignment operator,
   // so these are declared private to prevent inadvertent use by the application.
   CRfCommPort(const CRfCommPort & x);
   CRfCommPort& operator= (const CRfCommPort & x); 
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the FTP client sessions
//
class WIDCOMMSDK  CFtpClient
{

public:

//
// Define return code for FTP Client functions
//
typedef enum
{
    SUCCESS,                // Operation initiated without error
    OUT_OF_MEMORY,          // Not enough memory to initiate operation
    SECURITY_ERROR,         // Error implementing requested security level
    FTP_RETURN_ERROR,       // FTP-specific error
    NO_BT_SERVER,           // cannot access the local Bluetooth COM server
    FILE_NOT_FOUND,         // file/dirctory does not found
    FILE_SHARING,           // file sharing violation
    ALREADY_CONNECTED,      // Only one connection at a time supported for each instantiated CFtpClient object
    NOT_OPENED,             // Connection must be opened before requesting this operation
    INVALID_PARAMETER,      // One or more of function parameters are not valid
    UNKNOWN_RETURN_ERROR,   // Any condition other than the above
    LICENSE_ERROR           // license error
} FTP_RETURN_CODE;



//
// Define return code for FTP response functions
//
typedef enum
{
    COMPLETED,              // operation completed without error
    BAD_ADDR,               // bad BD_ADDR
    FILE_EXISTS,            // file already exists
    BAD_STATE,              // could not handle request in present state
    BAD_REQUEST,            // invalid request
    NOT_FOUND,              // no such file
    NO_SERVICE,             // could not find the specified FTP server
    DISCONNECT,             // connection lost
    READ,                   // read error
    WRITE,                  // write error
    OBEX_AUTHEN,            // OBEX Authentication required
    DENIED,                 // request could not be honored
    DATA_NOT_SUPPORTED,     // server does not support the requested data
    CONNECT,                // error establishing connection
    PERMISSIONS,            // incorrect file or service permissions
    NOT_INITIALIZED,        // not initialized
    PARAM,                  // invalid parameter
    RESOURCES,              // out of file system resources (handles, disk space, etc)
    SHARING,                // sharing violation
    UNKNOWN_RESULT_ERROR    // Any condition other than the above
} FTP_RESULT_CODE;

// FTP
enum FtpFolder
{
    FTP_ROOT_FOLDER = 1,
    FTP_PARENT_FOLDER,
    FTP_SUBFOLDER
};
typedef enum FtpFolder tFtpFolder;


public:

    // Construction/destruction
    //
    CFtpClient ();
    virtual ~CFtpClient();

    FTP_RETURN_CODE OpenConnection (BD_ADDR bdAddr, CSdpDiscoveryRec & sdp_rec);
    FTP_RETURN_CODE CloseConnection();
    FTP_RETURN_CODE PutFile(WCHAR * localFileName);
    FTP_RETURN_CODE GetFile(WCHAR * remoteFileName,  WCHAR * localFolder);
    FTP_RETURN_CODE FolderListing();
    FTP_RETURN_CODE ChangeFolder(WCHAR * szFolder);
    FTP_RETURN_CODE DeleteFile(WCHAR * szFile) { return _DeleteFile(szFile); }      // BtwDeleteFile == DeleteFile, added Btw version to 
    FTP_RETURN_CODE BtwDeleteFile(WCHAR * szFile) { return _DeleteFile(szFile); }   // avoid issue with UNICODE and standard lib DeleteFileA/W
    FTP_RETURN_CODE Abort();
    FTP_RETURN_CODE Parent();
    FTP_RETURN_CODE Root();
    FTP_RETURN_CODE CreateEmpty(WCHAR * szFile);
    FTP_RETURN_CODE CreateFolder(WCHAR * szFolder);
    void SetSecurity(BOOL authentication, BOOL encryption);

   // Application may provide these functions to facilitate managing the connection
    virtual void OnOpenResponse(FTP_RESULT_CODE result_code) {}
    virtual void OnCloseResponse(FTP_RESULT_CODE result_code){}
    

// windows
    virtual void OnProgress(FTP_RESULT_CODE result_code, WCHAR * name, long current, long total) {}
    virtual void OnPutResponse(FTP_RESULT_CODE result_code, WCHAR * name) {}
    virtual void OnGetResponse(FTP_RESULT_CODE result_code, WCHAR * name) {}
    virtual void OnCreateResponse(FTP_RESULT_CODE result_code, WCHAR * name) {}
    virtual void OnDeleteResponse(FTP_RESULT_CODE result_code, WCHAR * name){}
    
    virtual void OnChangeFolderResponse(FTP_RESULT_CODE result_code, tFtpFolder folder_type, WCHAR * szFolder) {}
    virtual void OnFolderListingResponse(FTP_RESULT_CODE result_code, tFTP_FILE_ENTRY * listing, long entries) {}
    virtual void OnXmlFolderListingResponse(FTP_RESULT_CODE rc, WCHAR * pfolder_listing, long folder_length ){}
    virtual void OnAbortResponse(FTP_RESULT_CODE result_code) {}

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }


private:
    long        m_FtpHandle;
    CWBtAPI     *m_pBtApi;
    BOOL        m_authentication_requested_by_app;
    BOOL        m_encryption_requested_by_app;
    BOOL        m_authentication_saved;
    BOOL        m_encryption_saved;
    void        SaveSecurity();
    void        RestoreSecurity();
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    HANDLE      m_hMutex;

    friend class CFtpClientFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CFtpClient(const CFtpClient & x);
    CFtpClient& operator= (const CFtpClient & x); 
    FTP_RETURN_CODE _DeleteFile(WCHAR * szFile);

};



///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the OPP client sessions
//
class COppClient_Impl;

class WIDCOMMSDK  COppClient
{
public:

//
// Define return code for OPP Client functions
//
typedef enum
{
    OPP_CLIENT_SUCCESS, // Operation initiated without error
    OUT_OF_MEMORY,      // Not enough memory to initiate operation
    SECURITY_ERROR,     // Error implementing requested security level
    OPP_ERROR,          // OPP-specific error
    OPP_NO_BT_SERVER,   // Cannot access the local Bluetooth COM server
    ABORT_INVALID,      // Abort not valid, no operation is in progress
    INVALID_PARAMETER,  // One or more of function parameters are not valid
    UNKNOWN_ERROR       // Any condition other than the above
} OPP_RETURN_CODE;

//
// Define return code for OPP response functions
//
typedef enum
{
    COMPLETED,              // operation completed without error
    BAD_ADDR,               // bad BD_ADDR
    BAD_STATE,              // could not handle request in present state
    BAD_REQUEST,            // invalid request
    NOT_FOUND,              // no such file
    NO_SERVICE,             // could not find the specified FTP server
    DISCONNECT,             // connection lost
    READ,                   // read error
    WRITE,                  // write error
    OBEX_AUTH,              // OBEX Authentication required
    DENIED,                 // request could not be honored
    DATA_NOT_SUPPORTED,     // server does not support the requested data
    CONNECT,                // error establishing connection
    NOT_INITIALIZED,        // not initialized
    PARAM,                  // bad parameter
    BAD_INBOX,              // inbox is not valid
    BAD_NAME,               // bad name for object
    PERMISSIONS,            // prohibited by file permissions
    SHARING,                // file is shared
    RESOURCES,              // file system resource limit reached - may be file handles, disk space, etc.
    FILE_EXISTS,            // is closedfile alto attempt to perform function after connectionready exists
    UNKNOWN_RESULT_ERROR    // Any condition other than the above
} OPP_RESULT_CODE;

//
// Define return code for OPP response functions
// (NOTE: these values match the ones defined in \middleware\opp\oppapp.h)
//
typedef enum
{
    OPP_PUT_TRANS =             1,
    OPP_GET_TRANS =             2,
    OPP_EXCHANGE_PUT_TRANS =    3,
    OPP_EXCHANGE_GET_TRANS =    4,
    OPP_ABORT_TRANS =           5

} OPP_TRANSACTION_CODE;

public:

    // Construction/destruction
    //
    COppClient ();
    virtual ~COppClient();

    OPP_RETURN_CODE Push(BD_ADDR bda, WCHAR * pszPathName, CSdpDiscoveryRec & sdp_rec);
    OPP_RETURN_CODE Pull(BD_ADDR bda, WCHAR * pszPathName, CSdpDiscoveryRec & sdp_rec);
    OPP_RETURN_CODE Exchange(BD_ADDR bda, WCHAR * pszName, WCHAR * pszFolder, CSdpDiscoveryRec & sdp_rec);
    OPP_RETURN_CODE Abort();
    void SetSecurity(BOOL authentication, BOOL encryption);

    virtual void OnAbortResponse (OPP_RESULT_CODE result_code) {}
    virtual void OnProgress(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * string, long current, long total) {}
    virtual void OnPushResponse(OPP_RESULT_CODE result_code,  BD_ADDR bda, WCHAR * string) {}
    virtual void OnPullResponse(OPP_RESULT_CODE result_code , BD_ADDR bda, WCHAR * string) {}
    virtual void OnExchangeResponse(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * string) {}
    virtual void OnExchangeResponse(OPP_RESULT_CODE result_code, BD_ADDR bda, WCHAR * string, OPP_TRANSACTION_CODE transaction_code) {}

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }
  
private:
    long        m_OppHandle;
    CWBtAPI     *m_pBtApi;
    BOOL        m_authentication_requested_by_app;
    BOOL        m_encryption_requested_by_app;
    BOOL        m_authentication_saved;
    BOOL        m_encryption_saved;
    void        SaveSecurity();
    void        RestoreSecurity();
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    GUID        m_guid;
    BD_ADDR     m_addr;
    HANDLE      m_hMutex;
    friend class COppClientFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    COppClient(const COppClient & x);
    COppClient& operator= (const COppClient & x); 
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the LAP client sessions
//
class WIDCOMMSDK  CLapClient
{
public:

//
// Define return code for LAP Client functions
//
typedef enum
{
    SUCCESS,            // Operation initiated without error
    NO_BT_SERVER,       // COM server could not be started
    ALREADY_CONNECTED,  // attempt to connect before previous connection closed
    NOT_CONNECTED,      // attempt to close unopened connection
    NOT_ENOUGH_MEMORY,  // local processor could not allocate memory for open
    INVALID_PARAMETER,  // One or more of function parameters are not valid
    UNKNOWN_ERROR,      // Any condition other than the above
    LICENSE_ERROR,      // license error
    NOT_SUPPORTED       // not supported on Profile Pack version - added SDK 6.1
} LAP_RETURN_CODE;

//
// Define connection states
//
typedef enum
{
    LAP_CONNECTED,      // port now connected
    LAP_DISCONNECTED    // port now disconnected
} LAP_STATE_CODE;


public:

    // Construction/destruction
    //
    CLapClient ();
    virtual ~CLapClient();

    LAP_RETURN_CODE CreateConnection(BD_ADDR bda, GUID guid, CSdpDiscoveryRec & sdp_rec);
    LAP_RETURN_CODE CreateConnection(BD_ADDR bda, CSdpDiscoveryRec & sdp_rec);
    LAP_RETURN_CODE RemoveConnection();
    void SetSecurity(BOOL authentication, BOOL encryption);

    virtual void OnStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, LAP_STATE_CODE state) = 0;

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }
    LAP_RETURN_CODE GetConnectionStats (tBT_CONN_STATS *p_conn_stats);

private:
    short       m_ComPort;
    CWBtAPI     *m_pBtApi;
    BOOL        m_authentication_requested_by_app;
    BOOL        m_encryption_requested_by_app;
    BOOL        m_authentication_saved;
    BOOL        m_encryption_saved;
    void        SaveSecurity();
    void        RestoreSecurity();
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    BD_ADDR     m_BdAddr;   // Bluetooth address of remote BT to connect tos
    GUID        m_Guid;
    HANDLE      m_hMutex;
    
    friend class CLapClientFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CLapClient(const CLapClient & x);
    CLapClient& operator= (const CLapClient & x); 
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the DUN client sessions
//
class WIDCOMMSDK  CDunClient
{
public:

//
// Define return code for DUN Client functions
//
typedef enum
{
    SUCCESS,            // Operation initiated without error
    NO_BT_SERVER,       // COM server could not be started
    ALREADY_CONNECTED,  // attempt to connect before previous connection closed
    NOT_CONNECTED,      // attempt to close unopened connection
    NOT_ENOUGH_MEMORY,  // local processor could not allocate memory for open
    INVALID_PARAMETER,  // One or more of function parameters are not valid
    UNKNOWN_ERROR,      // Any condition other than the above
    LICENSE_ERROR,      // invalid license
    NOT_SUPPORTED       // not supported in Profile Pack version - added SDK 6.1
} DUN_RETURN_CODE;

//
// Define connection states
//
typedef enum
{
    DUN_CONNECTED,      // port now connected
    DUN_DISCONNECTED    // port now disconnected
} DUN_STATE_CODE;


public:

    // Construction/destruction
    //
    CDunClient ();
    virtual ~CDunClient();

    DUN_RETURN_CODE CreateConnection(BD_ADDR bda, CSdpDiscoveryRec & sdp_rec);
    DUN_RETURN_CODE RemoveConnection();
    void SetSecurity(BOOL authentication, BOOL encryption);

    virtual void OnStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, DUN_STATE_CODE state) = 0;

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }
    DUN_RETURN_CODE GetConnectionStats (tBT_CONN_STATS *p_conn_stats);

private:
    short       m_ComPort;
    CWBtAPI     *m_pBtApi;
    BOOL        m_authentication_requested_by_app;
    BOOL        m_encryption_requested_by_app;
    BOOL        m_authentication_saved;
    BOOL        m_encryption_saved;
    void        SaveSecurity();
    void        RestoreSecurity();
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    BD_ADDR     m_BdAddr;
    HANDLE      m_hMutex;


    friend class CDunClientFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CDunClient(const CDunClient & x);
    CDunClient& operator= (const CDunClient & x); 
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the SPP client sessions
//
class WIDCOMMSDK  CSppClient
{
public:

//
// Define return code for SPP Client functions
//
typedef enum
{
    SUCCESS,             // Operation initiated without error
    NO_BT_SERVER,        // COM server could not be started
    ALREADY_CONNECTED,   // attempt to connect before previous connection closed
    NOT_CONNECTED,       // attempt to close unopened connection
    NOT_ENOUGH_MEMORY,   // local processor could not allocate memory for open
    INVALID_PARAMETER,   // One or more of function parameters are not valid
    UNKNOWN_ERROR,       // Any condition other than the above
    NO_EMPTY_PORT,       // no empty port
    LICENSE_ERROR,       // license error
    NOT_SUPPORTED        // not supported in Profile Pack version - added SDK 6.1
} SPP_CLIENT_RETURN_CODE;


public:

    // Construction/destruction
    //
    CSppClient ();
    virtual ~CSppClient();

    SPP_CLIENT_RETURN_CODE CreateConnection(BD_ADDR bda, BT_CHAR *szServiceName);
    SPP_CLIENT_RETURN_CODE RemoveConnection();

    virtual void OnClientStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, SPP_STATE_CODE state) = 0;

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }
    SPP_CLIENT_RETURN_CODE GetConnectionStats (tBT_CONN_STATS *p_conn_stats);
    SPP_CLIENT_RETURN_CODE CreateCOMPort(short *com_port);

private:
    short       m_ComPort;
    CWBtAPI     *m_pBtApi;
    BOOL        m_connected;
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    BD_ADDR     m_BdAddr;
    HANDLE      m_hMutex;
    friend class CSppClientFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CSppClient(const CSppClient & x);
    CSppClient& operator= (const CSppClient & x); 
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the SPP server sessions
//
class WIDCOMMSDK  CSppServer
{
public:

//
// Define return code for SPP Server functions
//
typedef enum
{
    SUCCESS,             // Operation initiated without error
    NO_BT_SERVER,        // COM server could not be started
    ALREADY_CONNECTED,   // attempt to connect before previous connection closed
    NOT_CONNECTED,       // attempt to close unopened connection
    NOT_ENOUGH_MEMORY,   // local processor could not allocate memory for open
    NOT_SUPPORTED,       // requested service not available locally
    UNKNOWN_ERROR,       // Any condition other than the above
    NO_EMPTY_PORT,       // no empty port
    INVALID_PARAMETER,   // One or more of function parameters are not valid
    LICENSE_ERROR        // license error
} SPP_SERVER_RETURN_CODE;

private:

typedef struct {
    short   comPort;
    BOOL    authentication;
    BOOL    authorization;
    BOOL    encryption;
    BOOL    automatic;  // TRUE if this service is automatically started by stack server
    BT_CHAR serviceName[BT_MAX_SERVICE_NAME_LEN];
} SPP_LOCAL_SERVICE;

#define MAX_LOCAL_SERVICES  5

public:

    // Construction/destruction
    //
    CSppServer ();
    virtual ~CSppServer();

    SPP_SERVER_RETURN_CODE CreateConnection(BT_CHAR * szServiceName);
    SPP_SERVER_RETURN_CODE RemoveConnection();

    virtual void OnServerStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, SPP_STATE_CODE state) = 0;

    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }
    SPP_SERVER_RETURN_CODE GetConnectionStats (tBT_CONN_STATS *p_conn_stats);
    SPP_SERVER_RETURN_CODE CreateCOMPort(short *com_port);

private:
    short       m_comPort;
    CWBtAPI     *m_pBtApi;
    BOOL        m_authentication;           // future
    BOOL        m_authorization;            // future
    BOOL        m_encryption;               // future
    BOOL        m_startup;
    BT_CHAR     m_serviceName[BT_MAX_SERVICE_NAME_LEN];
    BOOL        serviceAvailable(LPCSTR szServiceName);
    BOOL        m_connected;
    BOOL        m_automatic;
    short       m_cntLocalServices;
    SPP_LOCAL_SERVICE m_localServiceList[MAX_LOCAL_SERVICES];
    
    void SppLocalServices(SPP_LOCAL_SERVICE *pLocalServiceList, short maxLocalServices, short *pCntLocalServices);
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi 
    BD_ADDR     m_BdAddr;
    HANDLE      m_hMutex;

    friend class CSppServerFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CSppServer(const CSppServer & x);
    CSppServer& operator= (const CSppServer & x); 
};



////////////////////////////////////////////////////////////////////////////
// The CPrintClient class
//
#define BPSF_TYPE    0x01   // Mask value

struct BTPRINTSTRUCT
{
    DWORD    dwSize;     // Must be sizeof(BTPRINTSTRUCT)
    UINT     mask;       // 0 or BPSF_TYPE
    LPCSTR   pszType;    // Data type
};


class CPrintInternal;
class WIDCOMMSDK CPrintClient
{
public:
 
    // Define the profiles supported by the Printing SDK
    //
    typedef enum
    {
        PRINT_PROFILE_BPP,
        PRINT_PROFILE_HCRP,
        PRINT_PROFILE_SPP

    } ePRINT_PROFILE;

    // Define the current state of the Printing SDK
    //
    typedef enum
    {
        PRINT_STATE_IDLE,
        PRINT_STATE_CONNECTING,     
        PRINT_STATE_PRINTING,
        PRINT_STATE_FLOW_CONTROLLED,
        PRINT_STATE_DISCONNECTING, 
        PRINT_STATE_DONE

    } ePRINT_STATE;

    // Define error codes returned by the Printing SDK
    //
    typedef enum
    {
        // Generic to all profiles
        //
        PRINT_RC_OK,
        PRINT_RC_FILE_PRINTED_OK,
        PRINT_RC_FILE_NOT_FOUND,
        PRINT_RC_FILE_READ_ERROR,
        PRINT_RC_ALREADY_PRINTING,
        PRINT_RC_UNKNOWN_PROFILE,
        PRINT_RC_SERVICE_NOT_FOUND,
        PRINT_RC_SECURITY_ERROR,
        PRINT_RC_CONNECT_ERROR,
        PRINT_RC_WRITE_ERROR,
        PRINT_RC_REMOTE_DISCONNECTED,
        PRINT_RC_INVALID_PARAM,


        // BPP Specific errors
        //
        PRINT_RC_BPP_SCN_NOT_FOUND,
        PRINT_RC_BPP_SCN_NOT_ASSIGNED,
        PRINT_RC_BPP_OBEX_ABORTED,
        PRINT_RC_BPP_OBEX_MISMATCH,

        // HCRP Specific errors
        //
        PRINT_RC_HCRP_CTL_PSM_NOT_FOUND,
        PRINT_RC_HCRP_DATA_PSM_NOT_FOUND,

        // SPP Specific errors
        //
        PRINT_RC_SPP_SCN_NOT_FOUND,

    } ePRINT_ERROR;

public:
    CPrintClient();
    ~CPrintClient();

    ePRINT_ERROR        Start (BD_ADDR pBDA, ePRINT_PROFILE eProfile, LPCSTR pszFile, 
                               BTPRINTSTRUCT * pBtPrintStruct = NULL);

    void                Cancel();
    ePRINT_STATE        GetState();
    UINT                GetBytesSent();
    UINT                GetBytesTotal();
    ePRINT_ERROR        GetLastError(BT_CHAR **pDescr);

    virtual void OnStateChange (ePRINT_STATE NewState) { };
    static CPrintInternal *pIp;
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the headphone client sessions
//
// New Class added in SDK 5.0.  
// Supported in BTW stacks version 4.0 and greater
class WIDCOMMSDK  CHeadphoneClient
{
public:

    //
    // Define return code for Headphone Client functions
    //
    typedef enum
    {
        SUCCESS,             // Operation initiated without error
        NO_BT_SERVER,        // COM server could not be started
        ALREADY_CONNECTED,   // attempt to connect before previous connection closed
        NOT_CONNECTED,       // attempt to close unopened connection
        NOT_ENOUGH_MEMORY,   // local processor could not allocate memory for open
        INVALID_PARAMETER,   // One or more of function parameters are not valid
        UNKNOWN_ERROR,       // Any condition other than those here
        LICENSE_ERROR,       // invalid license
        DEVICE_BUSY,         // Device busy                             - added SDK 5.0.1.902
        SERVICE_NOT_FOUND,   // no SDP record found                     - added SDK 5.0.1.902
        BTM_WRONG_MODE       // local device is disabled or not up      - added SDK 5.0.1.902

    } HEADPHONE_RETURN_CODE;

    //
    // Define connection states for AV. Ref. from a2d_api.h
    //
    typedef enum
    {
        HEADPHONE_CONNECTED = 0,                // device is connected
        HEADPHONE_LOCAL_DISCONNECT = 10,        // connection closed by local device
        HEADPHONE_REMOTE_DISCONNECT = 11,       // connection closed by remote device
        HEADPHONE_DEVICE_NOT_AUTHORIZED = 15,   // device not authorized
        HEADPHONE_NO_STREAM_FOUND =  13,        // no audio stream found                 - added SDK 5.0.1.902
        HEADPHONE_REMOTE_SUSPENDED = 16,        // remote device suspended the stream    - added SDK 5.0.1.902
        HEADPHONE_INCOMING_STREAM  = 17,        // incoming stream                       - added SDK 5.0.1.902
        HEADPHONE_STREAMING      =   18,        // streaming data                        - added SDK 5.0.1.902
        HEADPHONE_STOPPED        =   19         // streaming stopped                     - added SDK 5.0.1.902
    } HEADPHONE_STATUS;


    typedef void (*tOnHAGConnectionStatusChangedCallback)
        (void *userData, BD_ADDR bda, DEV_CLASS dev_class, BD_NAME bd_name, long lHandle, long lStatus);

public:

    // Construction/destruction
    //
    CHeadphoneClient ();
    virtual ~CHeadphoneClient();

    void SetSecurity(BOOL authentication, BOOL encryption);
    void SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }

    HEADPHONE_RETURN_CODE ConnectHeadphone(BD_ADDR bda, LPCSTR szServiceName = "");
    HEADPHONE_RETURN_CODE DisconnectHeadphone(long hHandle);
    HEADPHONE_RETURN_CODE GetConnectionStats (tBT_CONN_STATS *p_conn_stats);
    HEADPHONE_RETURN_CODE RegStatusChangeCB (tOnHAGConnectionStatusChangedCallback pOnHAGStatus,
                                             void *userData);
    
private:
    CWBtAPI     *m_pBtApi;
    BOOL        m_authentication_requested_by_app;
    BOOL        m_encryption_requested_by_app;
    BOOL        m_authentication_saved;
    BOOL        m_encryption_saved;
    void        SaveSecurity();
    void        RestoreSecurity();
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    BD_ADDR     m_BdAddr;
    HANDLE      m_hMutex;


    friend class CHeadphoneClientFriend;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    CHeadphoneClient(const CHeadphoneClient & x);
    CHeadphoneClient& operator= (const CHeadphoneClient & x); 
};

///////////////////////////////////////////////////////////////////////////////////////
// Define a class to control the OPPMultiPush sessions
//

// BEGIN - added SDK 6.1, BTW 6.1.0.1300
typedef void (*tOnOppMultiPushCB)(long lOPPHandle,BD_ADDR bda,LPCWSTR pszName,long lError);
typedef void (*tOnOppMultiOpenCB)(long lError);
typedef void (*tOnOppMultiCloseCB)(long lError);
class COppClient_Impl;

class WIDCOMMSDK COppMultiPush
{
public:

//
// Define return code for OPPMultiPush functions
//
//
// Define return code for OPP Client functions
//
//#define OPP_MPUSH_RETURN_CODE COppClient::OPP_RETURN_CODE

public:

    // Construction/destruction
    //
    COppMultiPush ();
    virtual ~COppMultiPush();
    
    void  SetExtendedError(WBtRc code) { m_LastWBtRc = code; }
    WBtRc GetExtendedError() const { return m_LastWBtRc; }
  
    COppClient::OPP_RETURN_CODE MultiPush(WCHAR * pszPathName);
    COppClient::OPP_RETURN_CODE OpenOppConnection (BD_ADDR bdAddr, CSdpDiscoveryRec & sdp_rec);
    COppClient::OPP_RETURN_CODE CloseOppConnection();
    COppClient::OPP_RETURN_CODE RegOppMultiPushCB(tOnOppMultiPushCB  pOnOppMultiPushCB);
    COppClient::OPP_RETURN_CODE RegOppMultiOpenCB(tOnOppMultiOpenCB  pOnOppMultiOpenCB);
    COppClient::OPP_RETURN_CODE RegOppMultiCloseCB(tOnOppMultiCloseCB  pOnOMultippCloseCB);
  
private:
    BOOL        m_authentication_requested_by_app;
    BOOL        m_encryption_requested_by_app;
    BOOL        m_authentication_saved;
    BOOL        m_encryption_saved;
    void        SaveSecurity();
    void        RestoreSecurity();
    long        m_OppHandle;
    CWBtAPI     *m_pBtApi;
    WBtRc       m_LastWBtRc;    // Contains the last code returned by WBtApi
    GUID        m_guid;
    BD_ADDR     m_addr;
    HANDLE      m_hMutex;

    // This class will not support the compiler-supplied copy constructor or assignment operator,
    // so these are declared private to prevent inadvertent use by the application.
    COppMultiPush(const COppMultiPush & x);
    COppMultiPush& operator= (const COppMultiPush & x); 
    friend class COppMultiPushFriend;
};
// END   - added SDK 6.1, BTW 6.1.0.1300


#pragma pack ()


#endif // !defined(AFX_WIDCOMMSDK_H__1F5ED990_6FC6_4B0D_882C_8D7C98C16A06__INCLUDED_)
