/////////////////////////////////////////////////////////////////////////////
//
//  Name        BtIfObexHeaders.h
//  $Header:
//
//  Function    this file contains WIDCOMM BTW SDK class definitions
//
//  Date                 Modification
//  ----------------------------------
//  24Apr2001    JWF   Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _BTIFOBEXHEADERS_H
#define _BTIFOBEXHEADERS_H

#ifndef _BTWLIB
#include "BtIfDefinitions.h"
#include "BtIfClasses.h"
#ifdef WIDCOMMSDK_EXPORTS
#define WIDCOMMSDK __declspec(dllexport)
#else
#define WIDCOMMSDK __declspec(dllimport)
#endif
#else 
#define WIDCOMMSDK
#endif

// Ensure alignment across all builds
//
#pragma pack (1)

class CObexHeaders;

////////////////////////////////////////////////////////////////////////////
// 
// Class for OBEX client side applications
//
class WIDCOMMSDK CObexClient  
{
public:

    ~CObexClient();
    CObexClient();

    BOOL SetSecurityLevel (UINT8 scn, BT_CHAR *p_service_name, UINT8 security_level,GUID* p_service_guid);
    tOBEX_ERRORS Open (UINT8 scn, BD_ADDR bd_addr, CObexHeaders *p_request, UINT16 mtu = OBEX_DEFAULT_MTU);
    tOBEX_ERRORS SetPath (CObexHeaders *p_request, BOOL backup, BOOL create);
    tOBEX_ERRORS Put (CObexHeaders *p_request, BOOL final);
    tOBEX_ERRORS Get (CObexHeaders *p_request, BOOL final);
    tOBEX_ERRORS Abort (CObexHeaders *p_request);
    tOBEX_ERRORS Close (CObexHeaders *p_request);

    virtual void OnOpen(CObexHeaders *p_confirm, UINT16 tx_mtu, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response) = 0;
    virtual void OnClose(CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response) = 0;
    virtual void OnAbort (CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response) {}
    virtual void OnPut (CObexHeaders *p_confirm, tOBEX_ERRORS  code, tOBEX_RESPONSE_CODE response) {}
    virtual void OnGet (CObexHeaders *p_confirm, tOBEX_ERRORS  code, BOOL final, tOBEX_RESPONSE_CODE response) {}
    virtual void OnSetPath (CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response) {}

    //Create Audio Connection
    AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle);

    //Disconnect Audio Connection
    AUDIO_RETURN_CODE RemoveAudioConnection(UINT16 audioHandle);

    //Audio callback functions
    virtual void OnAudioConnected(UINT16 audioHandle){};
    virtual void OnAudioDisconnect(UINT16 audioHandle){};

    tOBEX_ERRORS SetLinkSupervisionTimeOut(UINT16 timeout);
    tOBEX_ERRORS SetWriteTimeOut(UINT16 timeout);            // Added BTW 3.1.1.400, SDK 4.0

    //eSCO functions
    AUDIO_RETURN_CODE RegForEScoEvts (UINT16 audioHandle, tBTM_ESCO_CBACK *p_esco_cback);       // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE SetEScoMode (tBTM_SCO_TYPE sco_mode, tBTM_ESCO_PARAMS *p_parms);          // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE ReadEScoLinkData (UINT16 audioHandle, tBTM_ESCO_DATA *p_Data);            // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE ChangeEScoLinkParms (UINT16 audioHandle, tBTM_CHG_ESCO_PARAMS *p_parms);  // added SDK 5.0, BTW 5.0.1.200
    void EScoConnRsp (UINT16 audioHandle, UINT8 hci_status, tBTM_ESCO_PARAMS *p_parms = NULL);  // added SDK 5.0, BTW 5.0.1.200

    //Returns RFCOMM handle related to that client. Returns NULL if it isn't connected.
    //It is unsupported method for internal use only
    UINT16 GetRfcHandle ();

private:

static CObexClient          *m_p_first_client;
    CObexClient             *m_p_next_client;
    friend class CObexClientFriend;

    tOBEX_CLIENT_HANDLE     m_client_handle;
    tOBEX_CSESSION_HANDLE   m_session_handle;

    tOBEX_HEADERS * GetBinaryPtr(CObexHeaders *p_headers_object);
    BD_ADDR                 m_remote_bd_addr;       // remote server Bluetooth device address
    UINT16                  m_audioHandle;

    UINT8      m_security_index;
};


////////////////////////////////////////////////////////////////////////////
// 
// Class for OBEX server side applications
//
class WIDCOMMSDK CObexServer  
{
public:

    ~CObexServer();
    CObexServer();

    tOBEX_ERRORS Register (UINT8 scn, UINT8 *p_target = NULL);
    tOBEX_ERRORS RegisterBinaryTarget (UINT8 scn, UINT8 *p_target, UINT16 target_len);          // added SDK 4.0, BTW 4.0.1.1400
    tOBEX_ERRORS Unregister();
    tOBEX_ERRORS OpenCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response, UINT16 mtu= OBEX_DEFAULT_MTU);
    tOBEX_ERRORS SetPathCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response);
    tOBEX_ERRORS PutCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response);
    tOBEX_ERRORS PutCreateCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response);
    tOBEX_ERRORS PutDeleteCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response);
    tOBEX_ERRORS GetCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, BOOL final, CObexHeaders * p_response);
    tOBEX_ERRORS AbortCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response);
    tOBEX_ERRORS CloseCnf (tOBEX_ERRORS  obex_errors, tOBEX_RESPONSE_CODE rsp_code, CObexHeaders * p_response);
    BOOL         SwitchRole(MASTER_SLAVE_ROLE new_role);

    //Create Audio Connection
    AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle);
    AUDIO_RETURN_CODE CreateAudioConnection(BOOL bIsClient, UINT16 *audioHandle, BD_ADDR bda);  // added SDK 4.0, BTW 4.0.1.1400

    //Disconnect Audio Connection
    AUDIO_RETURN_CODE RemoveAudioConnection(UINT16 audioHandle);

    //Audio callback functions
    virtual void OnAudioConnected(UINT16 audioHandle){};
    virtual void OnAudioDisconnect(UINT16 audinHandle){};

    void         GetRemoteBDAddr(BD_ADDR_PTR p_bd_addr);

    virtual void OnOpenInd(CObexHeaders *p_request) = 0;
    virtual void OnSetPathInd (CObexHeaders * p_request, BOOL backup, BOOL create) = 0;
    virtual void OnPutInd (CObexHeaders * p_request, BOOL final) = 0;
    virtual void OnPutCreateInd(CObexHeaders * p_request) = 0;
    virtual void OnPutDeleteInd (CObexHeaders * p_request) = 0;
    virtual void OnGetInd (CObexHeaders * p_request, BOOL final) = 0;
    virtual void OnAbortInd (CObexHeaders * p_request) = 0;
    virtual void OnCloseInd(CObexHeaders *p_request) = 0;

    tOBEX_ERRORS SetLinkSupervisionTimeOut(UINT16 timeout);

    //eSCO functions
    AUDIO_RETURN_CODE RegForEScoEvts (UINT16 audioHandle, tBTM_ESCO_CBACK *p_esco_cback);       // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE SetEScoMode (tBTM_SCO_TYPE sco_mode, tBTM_ESCO_PARAMS *p_parms);          // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE ReadEScoLinkData (UINT16 audioHandle, tBTM_ESCO_DATA *p_Data);            // added SDK 5.0, BTW 5.0.1.200
    AUDIO_RETURN_CODE ChangeEScoLinkParms (UINT16 audioHandle, tBTM_CHG_ESCO_PARAMS *p_parms);  // added SDK 5.0, BTW 5.0.1.200
    void EScoConnRsp (UINT16 audioHandle, UINT8 hci_status, tBTM_ESCO_PARAMS *p_parms = NULL);  // added SDK 5.0, BTW 5.0.1.200

private:

static CObexServer          *m_p_first_server;
    CObexServer             *m_p_next_server;
    friend class CObexServerFriend;

    tOBEX_SERVER_HANDLE     m_server_handle;
    tOBEX_SSESSION_HANDLE   m_session_handle;
    BD_ADDR                 m_remote_bd_addr;       // remote client Bluetooth device address
    UINT8                   m_scn;
    UINT8                   *m_p_target;
    UINT16                  m_audioHandle;
    UINT16                  *m_pPendingAudioHandle;
    UINT16                  m_target_len;

    tOBEX_HEADERS * GetBinaryPtr(CObexHeaders *p_headers_object);
    
};

////////////////////////////////////////////////////////////////////////////
// 
// Class for OBEX user defined parameter header
//
class WIDCOMMSDK CObexUserDefined  
{
public:

    ~CObexUserDefined();
    CObexUserDefined();
    BOOL SetHeader (UINT8 id, UINT8 byte);
    BOOL SetHeader (UINT8 id, UINT32 four_byte);
    BOOL SetHeader (UINT8 id, WCHAR * p_text);
    BOOL SetHeader (UINT8 id, UINT8 * p_array, UINT16 length);
    UINT8 GetUserType (UINT8 * p_id);
    BOOL GetByte (UINT8 *p_byte);
    BOOL GetFourByte (UINT32 *p_fourbyte);
    UINT16 GetLength ();
    BOOL GetText (WCHAR *p_text);
    BOOL GetOctets (UINT8 *p_octet_array);

private:
    tOBEX_USER_HDR ud_hdr;

    // This class will not support the compiler-supplied copy constructor or assignment operator, tOBEX_CSESSION_HANDLE session_handle,
    // so these are declared private to prevent inadvertant use by the application.
    CObexUserDefined(const CObexUserDefined & x);
    CObexUserDefined& operator= (const CObexUserDefined & x); 
};

 
////////////////////////////////////////////////////////////////////////////
// 
// Class for OBEX headers object, tOBEX_CSESSION_HANDLE session_handle, a container for all possible OBEX header values
//
class WIDCOMMSDK CObexHeaders  
{
    friend class CObexClient;
    friend class CObexClientFriend;
    friend class CObexServer;
    friend class CObexServerFriend;

public:

    ~CObexHeaders();
    CObexHeaders(); // locally allocate container, tOBEX_CSESSION_HANDLE session_handle, with all null header values

private:

    // These functions are accessable only from friends on the lower OBEX levels
    // They permit efficient use of existing OBEX header structures
    CObexHeaders(tOBEX_HEADERS *p_external_hdrs);   // externally constructed container
    tOBEX_HEADERS * GetBinaryPtr();

    // This class will not support the compiler-supplied copy constructor or assignment operator, tOBEX_CSESSION_HANDLE session_handle,
    // so these are declared private to prevent inadvertant use by the application.
    CObexHeaders(const CObexHeaders & x);
    CObexHeaders& operator= (const CObexHeaders & x); 

public:

    void SetCount(UINT32 count);
    void DeleteCount();
    BOOL GetCount(UINT32 * p_count);

    BOOL SetName (WCHAR * p_array);
    void DeleteName();
    BOOL GetNameLength (UINT32 * p_len_incl_null);
    BOOL GetName (WCHAR * p_array);

    BOOL SetType (UINT8 * p_array, UINT32 length);
    void DeleteType();
    BOOL GetTypeLength (UINT32 * p_length);
    BOOL GetType (UINT8 * p_array);

    void SetLength(UINT32 length);
    void DeleteLength();
    BOOL GetLength(UINT32 * p_length);

    BOOL SetTime(char * p_str_8601);
    void DeleteTime();
    BOOL GetTime(char * p_str_8601);

    BOOL SetDescription (WCHAR * p_array);
    void DeleteDescription();
    BOOL GetDescriptionLength (UINT32 * p_len_incl_null);
    BOOL GetDescription (WCHAR * p_array);

    BOOL AddTarget (UINT8 * p_array, UINT32 length);
    UINT32 GetTargetCnt ();
    BOOL DeleteTarget (UINT16 index);
    BOOL GetTargetLength (UINT32  * p_length, UINT16 index);
    BOOL GetTarget (UINT8 * p_array, UINT16 index);

    BOOL AddHttp (UINT8 * p_array, UINT32 length);
    UINT32 GetHttpCnt ();
    BOOL DeleteHttp (UINT16 index);
    BOOL GetHttpLength (UINT32  * p_length, UINT16 index);
    BOOL GetHttp (UINT8 * p_array, UINT16 index);

    BOOL SetBody (UINT8 * p_array, UINT32 length, BOOL body_end);
    void DeleteBody();
    BOOL GetBodyLength ( UINT32 * p_length);
    BOOL GetBody (UINT8 * p_array, BOOL *p_body_end);

    BOOL SetWho (UINT8 * p_array, UINT32 length);
    void DeleteWho();
    BOOL GetWhoLength ( UINT32 * p_length);
    BOOL GetWho (UINT8 * p_array);

    BOOL AddAppParam (UINT8 tag, UINT8 length, UINT8 * p_array);
    UINT32 GetAppParamCnt ();
    BOOL DeleteAppParam(UINT16 index);
    BOOL GetAppParamLength(UINT8  * p_length, UINT16 index);
    BOOL GetAppParam (UINT8 * p_tag, UINT8 *p_array, UINT16 index);

    BOOL AddAuthChallenge (UINT8 tag, UINT8 length, UINT8 * p_array);
    UINT32 GetAuthChallengeCnt ();
    BOOL DeleteAuthChallenge (UINT16 index);
    BOOL GetAuthChallengeLength (UINT8  * p_length, UINT16 index);
    BOOL GetAuthChallenge (UINT8 * p_tag, UINT8 *p_array, UINT16 index);

    BOOL AddAuthResponse (UINT8 tag, UINT8 length, UINT8 * p_array);
    UINT32 GetAuthResponseCnt ();
    BOOL DeleteAuthResponse (UINT16 index);
    BOOL GetAuthResponseLength (UINT8  * p_length, UINT16 index);
    BOOL GetAuthResponse (UINT8 * p_tag, UINT8 *p_array, UINT16 index);

    BOOL SetObjectClass (UINT8 * p_array, UINT32 length);
    void DeleteObjectClass ();
    BOOL GetObjectClassLength ( UINT32 * p_length);
    BOOL GetObjectClass (UINT8 * p_array);

    BOOL AddUserDefined (CObexUserDefined * p_user_defined);
    UINT32 GetUserDefinedCnt ();
    BOOL DeleteUserDefined (UINT16 index);
    BOOL GetUserDefinedLength (UINT16  * p_length, UINT16 index);
    BOOL GetUserDefined (CObexUserDefined * p_user_defined, UINT16 index);

private:

    void SetDefaults(tOBEX_HEADERS *p_headers);  // all header fields become null
    void FreeInternal(tOBEX_HEADERS *p_headers); // release all memory malloced to this object
    void SetFlag (UINT32 mask);
    void ClearFlag(UINT32 mask);
    BOOL TestFlag(UINT32 mask);
    BOOL locally_allocated;

private:

    tOBEX_HEADERS *p_hdrs;
};



#pragma pack ()

#endif 
