// AtCodec.h: interface for the CAtCodec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATCODEC_H__868D35E0_6422_4AF3_9A5E_1AC03E626F87__INCLUDED_)
#define AFX_ATCODEC_H__868D35E0_6422_4AF3_9A5E_1AC03E626F87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buffer.h"
#include "Global_Def.h"
#include "PBEntries.h"
#include <string>
#include <vector>
#include "../../Components/SMSServer/MessageStructure.h"
#include "../../CommonUtil/UnicodeUtility.h"


#define MAX_DBB_FILENAME_SIZE 100
#define MAX_MMP_FILENAME_SIZE 250
#define MAX_FILENAME_SIZE (MAX_MMP_FILENAME_SIZE > MAX_DBB_FILENAME_SIZE) ? MAX_MMP_FILENAME_SIZE : MAX_DBB_FILENAME_SIZE

/**
 *  @class 	    CAtCodec
 *  @brief	    This codec is used to generate the AT Command and to parse the corresponding AT Command Response.
 *  @author	    Saffin Laio et al.
 *  @date	    2006/05/09
 */

class CAtCodec: public CRefObject
{
public:
    CAtCodec();
    virtual ~CAtCodec();

public:
    enum Opcode_t
    {
        otNone,
        otRing,
        otError,
        otCMEError,
        otCMSError,
		otFSError,
		otVCardError,
		//================= Steven CC Chen added codes ==================BEGIN//
		otVCalendarError,
		//================= Steven CC Chen added codes ==================END//
        otSingleCarriageReturn,
        otSingleLineFeed,
        otEchoOfCommand,
        otResult,                   // Some result, not yet decoded
        otRequest,                  // Some request, not specified
        otPDUPrompt,
        otDiscard,
        otReqAt,                    // AT
        otReqGetPhoneType,
        otReqSwitchToMux,
        otReqGetManufacturer,
        otReqGetIMEI,
        otReqGetSoftwareVersion,
        otReqGetDeviceInfoBBClient,
        otReqGetSignalQuality,
        otReqGetBatteryCharge,
        otReqSwitchToRCCP,
        otReqSwitchToOBEX,
		otReqSwitchToUCS2,
		otReqSwitchToGSM,
        otResAt,
        otReqGetVolumes,
        otReqGetEntries,
        otReqGetCapacity,
        otReqGetFile,
        otResGetFile,
        otReqPutFile,
        otReqCopyFile,
		otUnsolicitedEvent,
        otEventBatteryChargeChanged,
        otEventSignalQualityChanged,
        otEventNetworkRegistrationChanged,
		// PhoneBook
		otReqGetPBRangesAndLengthsStd,
		otResGetPBRangesAndLengthsStd,
		otReqReadPhonebookEntriesStd, 
		otResReadPhonebookEntriesStd,
		otReqGetSupportedPhonebooksStd,
		otResGetSupportedPhonebooksStd,
		//================= Steven CC Chen added codes ==================BEGIN//
		// AdddressBook related Requests
		otReqGetAddrRangesAndCounts,
		otReqReadPhonebookEntry,
		otReqReadAddressbookEntry,
		otReqWriteAddressbookEntryData,
		otReqGetDeleteSupportedStorageList,
		otReqGetListSupportedStorageList,
		otReqListUsedPhonebookEntries,
		otReqListUsedAddressbookEnties,
		otReqGetCallergroupIdRange,
		otReqReadCallergroupSetting,
		// Calendar related Requests
		otReqGetCalUsedIndexesAndCount,
		otReqReadCalendarEntry,
		otReqGetCalDeleteSupportedActions,
		otReqWriteCalendarEntryData,
		//================= Steven CC Chen added codes ==================END//
		// Messages
		otReqWriteMessageToMemoryTi,
		otReqWriteMessageToMemoryStd,
		otReqPDUDataWriteTi,
		otReqPDUDataWriteStd,
		otReqDirectSendMessage,
		otReqSendStoredMessageTi,
		otReqSendStoredMessageStd,
		otReqPDUDataDirectSend,
		otReqGetServiceCenterAddress,
		otReqReadMessageTi,
		otReqReadMessageStd,
		otReqListMessagesTi,
		otReqListMessagesStd,
		otReqPreferredMessageStorage,
		otReqQueryMessageSpace,
        //============ Eddy Guo add the code for NEW_BENQ_COMMAND ========//
        otReqQueryFolderNew,                ///< AT$BQMGFL
        otReqListMsgIDNew,                  ///< AT$BQMGL
        otReqReadMsgNew,                    ///< AT$BQMGR
        otReqWriteMessageToMemoryNew,       ///< AT$BQMGR
        otReqPDUDataWriteNew,               ///< Write PDU into memory
        otReqDeleteMsgNew,                  ///< AT$BQMGD
        otReqUpdateMessageState,
		//
		otResGetPhoneType,
		otResGetManufacturer,
		otResGetIMEI,
		otResGetDeviceInfoBBClient,
		otResGetSoftwareVersion,
		otResGetSignalQuality,
		otResGetBatteryCharge,
		otResSwitchToOBEX,
		otResOk,
		otResERROR
    };

    static LPCTSTR opcode2t( Opcode_t eOpcode );

    CString GetTraceString() const;

	enum CME_Error_t
	{
        errCME_None                         = -1,
		//
		errCME_PhoneFailure                 = 0,
        errCME_NoConnection                 = 1,
        errCME_PhoneAdapterLinkReserved     = 2,
        errCME_OperationNotAllowed          = 3,
        errCME_OperationNotSupported        = 4,
        errCME_PHSIM_PINRequired            = 5,
        errCME_PHFSIM_PINRequired           = 6,
        errCME_PHFSIM_PUKRequired           = 7,
        //                  
        errCME_SIM_NotInserted              = 10,
        errCME_SIM_PINRequired              = 11,
        errCME_SIM_PUKRequired              = 12,
        errCME_SIMFailure                   = 13,
        errCME_SIMBusy                      = 14,
        errCME_SIMWrong                     = 15,
        errCME_IncorrectPassword            = 16,
        errCME_SIM_PIN2Required             = 17,
        errCME_SIM_PUK2Required             = 18,
        //                              
        errCME_MemoryFull                   = 20,
        errCME_InvalidIndex                 = 21,
        errCME_NotFound                     = 22,
        errCME_MemoryFailure                = 23,
        errCME_TextStringTooLong            = 24,
        errCME_InvalidCharactersInTextString= 25,
        errCME_DialStringTooLong            = 26,
        errCME_InvalidCharactersInDialString= 27,
		//
        errCME_NoNetworkService             = 30,
        errCME_NetworkTimeout               = 31,
        errCME_NetworkNotAllowed_EmergencyCallsOnly = 32,
		//
        errCME_NetworkPersonalization_PINRequired           = 40,
        errCME_NetworkPersonalization_PUKRequired           = 41,
        errCME_NetworkSubsetPersonalization_PINRequired     = 42,
        errCME_NetworkSubsetPersonalization_PUKRequired     = 43,
        errCME_ServiceProviderPersonalization_PINRequired   = 44,
        errCME_ServiceProviderPersonalization_PUKRequired   = 45,
        errCME_CorporatePersonalization_PINRequired         = 46,
        errCME_CorporatePersonalization_PUKRequired         = 47,
		//
        errCME_Unknown                      = 100,
        //                              
        errCME_IllegalMS                    = 103,
        errCME_IllegalME                    = 106,
        errCME_GPRSServicesNotAllowed       = 107,
        errCME_PLMNNotAllowed               = 111,
        errCME_LocationAreaNotAllowed       = 112,
        errCME_RoamingNotAllowedInThisArea  = 113,
        errCME_ServiceOptionNotSupported    = 132,
        errCME_RequestedServiceOptionNotSubscribed = 133,
        errCME_ServiceOptionTemporarilyOutOfOrder  = 134,
		//
        errCME_UnspecifiedGPRSError         = 148,
        errCME_PDPAuthenticationFailure     = 149,
        errCME_InvalidMobileClass           = 150,
		//
        errCME_OperationTemporarilyNotAllowed = 256,
        errCME_CallBarred                   = 257,
        errCME_PhoneIsBusy                  = 258,
        errCME_UserAbort                    = 259,
        errCME_InvalidDialString            = 260,
        errCME_SSNotExecuted                = 261,
        errCME_SIMBlocked                   = 262,
        errCME_SuplementaryServiceRejected  = 263,
    };

    enum CMS_Error_t
	{
        errCMS_None                             = -1,
		//
        errCMS_UnassignedNumber                 = 1,
        errCMS_OperatorBarring                  = 8,
        errCMS_CallBarred                       = 10,
        errCMS_ShortMessageTransferRejected     = 21,
        errCMS_DestinationOutOfService          = 27,
        errCMS_UnidentifiedSubscriber           = 28,
        errCMS_FacilityRejected                 = 29,
        errCMS_UnknownSubscriber                = 30,
        errCMS_NetworkOutOfOrder                = 38,
        errCMS_TemporaryFailure                 = 41,
        errCMS_Congestion                       = 42,
        errCMS_ResourcesUnavailable             = 47,
        errCMS_RequestedFacilityNotSubscribed   = 50,
        errCMS_RequestedFacilityNotImplemented  = 69,
        errCMS_InvalidReferenceValue            = 81,
        errCMS_InvalidMessage                   = 95,
        errCMS_InvalidMandatoryInformation      = 96,
        errCMS_MessageTypeNotImplemented        = 97,
        errCMS_MessageNotCompatible             = 98,
        errCMS_InformationElementNotImplemented = 99,
        errCMS_ProtocolError                    = 111,
        errCMS_Interworking                     = 127,
        errCMS_TelematicInterworkingNotSupported= 128,
        errCMS_ShortMessageType0NotSupported    = 129,
        errCMS_CannotReplaceShortMessage        = 130,
        errCMS_UnspecifiedTPPIDError            = 143,
        errCMS_DataCodingNotSupported           = 144,
        errCMS_MessageClassNotSupported         = 145,
        errCMS_UnspecifiedTPDCSError            = 159,
        errCMS_CommandCannotBeExecuted          = 160,
        errCMS_CommandUnsupported               = 161,
        errCMS_UnspecifiedTPCommandError        = 175,
        errCMS_TPDUNotSupported                 = 176,
        errCMS_SCBusy                           = 192,
        errCMS_NoSCSubscription                 = 193,
        errCMS_SCSystemFailure                  = 194,
        errCMS_InvalidSMEAddress                = 195,
        errCMS_DestinationSMEBarred             = 196,
        errCMS_SMRejected_DuplicateSM           = 197,
        errCMS_TPVPF_NotSupported               = 198,
        errCMS_TPVP_NotSupported                = 199,
        errCMS_SIMSMSStorageFull                = 208,
        errCMS_NoSMSStoragecapability           = 209,
        errCMS_ErrorInMS                        = 210,
        errCMS_MemoryCapacityExceeded           = 211,
        errCMS_SIMApplicationToolkitBusy        = 212,
        errCMS_SIMDataDownloadError             = 213,
        errCMS_UnspecifiedErrorCause            = 255,
        errCMS_MEFailure                        = 300,
        errCMS_SMSServiceOfMEReserved           = 301,
        errCMS_OperationNotAllowed              = 302,
        errCMS_OperationNotSupported            = 303,
        errCMS_InvalidPDUModeParameter          = 304,
        errCMS_InvalidTextModeParameter         = 305,
        errCMS_SIMNotInserted                   = 310,
        errCMS_SIMPINRequired                   = 311,
        errCMS_PH_SIMPINRequired                = 312,
        errCMS_SIMFailure                       = 313,
        errCMS_SIMBusy                          = 314,
        errCMS_SIMWrong                         = 315,
        errCMS_SIMPUKRequired                   = 316,
        errCMS_SIMPIN2Required                  = 317,
        errCMS_SIMPUK2Required                  = 318,
        errCMS_MemoryFailure                    = 320,
        errCMS_InvalidMemoryIndex               = 321,
        errCMS_MemoryFull                       = 322,
        errCMS_SMSCAddressUnknown               = 330,
        errCMS_NoNetworkService                 = 331,
        errCMS_NetworkTimeout                   = 332,
        errCMS_NoCNMAACKExpected                = 340,
        errCMS_UnknownError                     = 500,
        errCMS_UserAbort                        = 512,
    };

	enum FS_Error_t
	{
		errFS_None = 0,
		//
		errFS_InitFail               = -1,     // file system fails to initialize.
		errFS_InitAgain              = -2,     // file system has been initialized.
		errFS_FileNotFound           = -3,     // specified file does not exist.
		errFS_ChecksumError          = -4,     // checksum of data is wrong.
		errFS_BufferSizeTooLarge     = -5,     // size of given buffer is larger.
		errFS_BufferSizeTooSmall     = -6,     // size of given buffer is smaller.
		errFS_SpaceNotEnough         = -7,     // free flash space is not enough.
		errFS_IllegalParameter       = -8,     // unknown parameters.
		errFS_RecordNotFound         = -9,     // specified record does not exist.
		errFS_TypeNotMatch           = -10,    // can not handle this file type.
		errFS_OpenTooMany            = -11,    // open too many files/directories.
		errFS_AlreadyExist           = -12,    // file/directory already exists.
		errFS_DirectoryNotFound      = -13,    // directory doesn't exist.
		errFS_NextItemNotFound       = -14,    // no next item.
		errFS_WriteError             = -15,    // flash program error.
		errFS_EraseError             = -16,	   // flash erase error.
		errFS_DeleteSplitError       = -17,	   // delete split file error.
		errFS_SegmentNotFound        = -18,	   // one or some split file`s segment not found.
		errFS_TooManyObjects         = -19,	   // too many objects in the directory.
		errFS_DirectoryNotEmpty      = -20,	   // directory not empty.
		errFS_PartitionSpaceChanged  = -21,	   // result of Close_An_Opened_Segment.
		errFS_AlreadyOpened          = -22,	   // object already be opened.
		errFS_WholeFileTooLarge      = -23,	   // whole file too large (> 60KB).
		errFS_Error                  = -32767, // unknown error.
		errFS_Timeout                = -32768, // timeout.																				
	};

    // Encoding
public:
    static CRefObjectPtr<CAtCodec> CreateReqSetFlowControl(int nFlowControl);
    static CRefObjectPtr<CAtCodec> CreateReqGetPhoneType();
    static CRefObjectPtr<CAtCodec> CreateReqGetManufacturer();
    static CRefObjectPtr<CAtCodec> CreateReqGetIMEI();
    //static CRefObjectPtr<CAtCodec> CreateReqGetDeviceInfoBBClient();
    static CRefObjectPtr<CAtCodec> CreateReqGetSoftwareVersion();
    //
    //static CRefObjectPtr<CAtCodec> CreateReqGetSignalQuality();
    //static CRefObjectPtr<CAtCodec> CreateReqGetBatteryCharge();
    //static CRefObjectPtr<CAtCodec> CreateReqSwitchToRCCP();
    static CRefObjectPtr<CAtCodec> CreateReqSwitchToOBEX();
    static CRefObjectPtr<CAtCodec> CreateReqSwitchToAT();
    //static CBufferPtr CreateReqSwitchToMux();
    //static CRefObjectPtr<CAtCodec> CreateReqEnableCMEE();
    static CRefObjectPtr<CAtCodec> CreateReqSwitchToUCS2();
    static CRefObjectPtr<CAtCodec> CreateReqSwitchToGSM();
    //
    //static CRefObjectPtr<CAtCodec> CreateReqCIND();
    //static CRefObjectPtr<CAtCodec> CreateReqCMER();
    //static CRefObjectPtr<CAtCodec> CreateReqCNMI();
    //static CRefObjectPtr<CAtCodec> CreateReqEnableCREG();
    //static CRefObjectPtr<CAtCodec> CreateReqEnableSHFR();
    static CRefObjectPtr<CAtCodec> CreateEchoOff();

    //-------------------------------------------------------------------------
    // File Manager
    // Implementation in FileManagerCodec.cpp
    //-------------------------------------------------------------------------
    //static CRefObjectPtr<CAtCodec> CreateReqGetVolumes();
    //static CRefObjectPtr<CAtCodec> CreateReqGetCapacity( LPCTSTR szDriver, bool bIsDBB );
    static CRefObjectPtr<CAtCodec> CreateReqGetFile(LPCTSTR szPath, bool bIsDBB);            
    static CRefObjectPtr<CAtCodec> CreateReqPutFile(LPCTSTR szPath, BOOL bIsDBB, DWORD dwFileSize);
    static CRefObjectPtr<CAtCodec> CreateReqRemoveFile(LPCTSTR szPath, BOOL bIsDBB);
    static CRefObjectPtr<CAtCodec> CreateReqGetEntries(LPCTSTR szPath, BOOL bIsDBB, BOOL bIsContinue = FALSE);                       
    static CRefObjectPtr<CAtCodec> CreateReqCreateDir(LPCTSTR szPath, BOOL bIsDBB);                          
    static CRefObjectPtr<CAtCodec> CreateReqRemoveDir(LPCTSTR szPath, BOOL bIsDBB);
    //static CRefObjectPtr<CAtCodec> CreateReqMoveDir( LPCTSTR szOldPath, LPCTSTR szNewPath, bool bIsDBB );
    //static CRefObjectPtr<CAtCodec> CreateReqMoveFile( LPCTSTR szOldPath, LPCTSTR szNewPath, bool bIsDBB );
    //static CRefObjectPtr<CAtCodec> CreateReqCopyFile( LPCTSTR szPathSource, bool bIsDBBSource, LPCTSTR szPathTarget, bool bIsDBBTarget );

    //-------------------------------------------------------------------------
    // PhoneBook ATCodec
    // Implementation in PhoneBookCodec.cpp
    //-------------------------------------------------------------------------
    static CRefObjectPtr<CAtCodec> CreateReqGetPBRangesAndLengthsStd();
    static CRefObjectPtr<CAtCodec> CreateReqWritePhonebookEntryStd( long lIndex, LPCTSTR szName, LPCTSTR szNumber );
    static CRefObjectPtr<CAtCodec> CreateReqReadPhonebookEntriesStd( long lStartPos, long lEndPos );
    static CRefObjectPtr<CAtCodec> CreateReqDeletePhonebookEntryStd( long lIndex );
    static CRefObjectPtr<CAtCodec> CreateReqGetSupportedPhonebooksStd();
    // AddressBook ATCodec
    //static CRefObjectPtr<CAtCodec> CreateReqGetAddrRangesAndCounts( void );
    static CRefObjectPtr<CAtCodec> CreateReqReadPhonebookEntry(enumStorage ePIMStorage, DWORD dwIndex);
    static CRefObjectPtr<CAtCodec> CreateReqWritePhonebookEntry(enumStorage ePIMStorage, DWORD dwIndex, DWORD dwDataSizeInByte);
    static CRefObjectPtr<CAtCodec> CreateReqWritePhonebookEntryData(LPCTSTR pszVCard);
    static CRefObjectPtr<CAtCodec> CreateReqDeletePhonebookEntry(enumStorage ePIMStorage, enumPIMAction ePIMAction, DWORD dwIndex);
    //static CRefObjectPtr<CAtCodec> CreateReqReadAddressbookEntry( DWORD dwIndex );
    //static CRefObjectPtr<CAtCodec> CreateReqWriteAddressbookEntry( DWORD dwIndex, DWORD dwDataSizeInByte );
    //static CRefObjectPtr<CAtCodec> CreateReqWriteAddressbookEntryData( const char* pszVCard, DWORD dwDataSizeInByte );
    //static CRefObjectPtr<CAtCodec> CreateReqGetDeleteSupportedStorageList( void );
    //static CRefObjectPtr<CAtCodec> CreateReqDeletePhonebookEntry( DWORD dwIndex );
    //static CRefObjectPtr<CAtCodec> CreateReqDeleteAddressbookEntry( DWORD dwIndex );
    //static CRefObjectPtr<CAtCodec> CreateReqGetListSupportedStorageList( void );
    static CRefObjectPtr<CAtCodec> CreateReqListUsedPhonebookEntries(enumStorage ePIMStorage);
    //static CRefObjectPtr<CAtCodec> CreateReqListUsedAddressbookEnties( void );
    //static CRefObjectPtr<CAtCodec> CreateReqGetCallergroupIdRange( void );
    static CRefObjectPtr<CAtCodec> CreateReqReadCallergroupSetting(DWORD dwCallergroupId);
    static CRefObjectPtr<CAtCodec> CreateReqWriteCallergroupSetting(CallergroupSetting_t tagCallergroupSetting);

    //-------------------------------------------------------------------------
    // Calendar ATCodec
    // Implementation in CalendarCodec.cpp
    //-------------------------------------------------------------------------
    //static CRefObjectPtr<CAtCodec> CreateReqGetCalUsedIndexesAndCount( void );
    static CRefObjectPtr<CAtCodec> CreateReqReadCalendarEntry(DWORD dwIndex);
    static CRefObjectPtr<CAtCodec> CreateReqWriteCalendarEntry(DWORD dwIndex, DWORD dwDataSizeInByte);
    static CRefObjectPtr<CAtCodec> CreateReqWriteCalendarEntryData(LPCTSTR pszVCalendar, DWORD dwDataSizeInByte);
    //static CRefObjectPtr<CAtCodec> CreateReqGetCalDeleteSupportedActions( void );
    static CRefObjectPtr<CAtCodec> CreateReqDeleteCalendarEntry(enumPIMAction ePIMAction, DWORD dwIndex);
    //static CRefObjectPtr<CAtCodec> CreateReqDeleteCalendarAllEntries( DWORD dwActionIdDeleteAll );

    //-------------------------------------------------------------------------
    // Message
    // Implementation in MessageCodec.cpp
    //-------------------------------------------------------------------------
    static CRefObjectPtr<CAtCodec> CreateReqSetPDUMode(int nMode = 0);
    static CRefObjectPtr<CAtCodec> CreateReqListMessagesTi(enumMsgLoc eLocation);
    static CRefObjectPtr<CAtCodec> CreateReqListMessagesStd(int nStat);
    static CRefObjectPtr<CAtCodec> CreateReqPreferredMessageStorage(enumStorage eMsgStorage);
    static CRefObjectPtr<CAtCodec> CreateReqWriteMessageToMemoryTi(enumMsgLoc eMsgLoc, int nDataLen);
    static CRefObjectPtr<CAtCodec> CreateReqWriteMessageToMemoryStd(int nLoc, BYTE* pMsgData, int nDataLen);
    static CRefObjectPtr<CAtCodec> CreateReqDirectSendMessage(BYTE* pMsgData, int nDataLen);
    static CRefObjectPtr<CAtCodec> CreateReqSendStoredMessageTi(DWORD dwIndex, LPCTSTR pszAddress);
    static CRefObjectPtr<CAtCodec> CreateReqSendStoredMessageStd(DWORD dwIndex, LPCTSTR pszAddress);
    static CRefObjectPtr<CAtCodec> CreateReqMessagePDUTransferWriteMessageToMemoryTi(BYTE* pMsgData, int nDataLen);
    static CRefObjectPtr<CAtCodec> CreateReqMessagePDUTransferWriteMessageToMemoryStd(BYTE* pMsgData, int nDataLen);
    static CRefObjectPtr<CAtCodec> CreateReqMessagePDUTransferDirectSendMessage(BYTE* pMsgData, int nDataLen);
    static CRefObjectPtr<CAtCodec> CreateReqDeleteMessageTi(DWORD dwIndex);
    static CRefObjectPtr<CAtCodec> CreateReqDeleteMessageStd(DWORD dwIndex);
    static CRefObjectPtr<CAtCodec> CreateReqReadMessageTi(DWORD dwIndex);
    static CRefObjectPtr<CAtCodec> CreateReqReadMessageStd(DWORD dwIndex);
    //	static CRefObjectPtr<CAtCodec> CreateReqSetRead(DWORD index);
    static CRefObjectPtr<CAtCodec> CreateReqGetServiceCenterAddress();
    static CRefObjectPtr<CAtCodec> CreateReqQueryMessageSpace(enumStorage eMsgStorage);
    // New AT Command 2007.10.23 Added by Eddy Guo
    // AT$BQMGFL
    static CRefObjectPtr<CAtCodec> CreateReqListFolder(UINT nFolderID);
    // AT$BQMGR
    static CRefObjectPtr<CAtCodec> CreateReqReadMessageNew(DWORD dwIndex);
    // AT$BQMGL
    static CRefObjectPtr<CAtCodec> CreateReqListMessagesNew(DWORD nFolderID);
    // AT$BQMGD
    static CRefObjectPtr<CAtCodec> CreateReqDeleteMessagesNew(DWORD nMsgID);
    // AT$BQMGW
    static CRefObjectPtr<CAtCodec> CreateReqWriteMessagesNew(enumStorage eMsgStorage, UINT FolderID, UINT nTag, UINT nMessageLength);
    // AT$BQMGU
    static CRefObjectPtr<CAtCodec> CreateReqUpdateMessageState(DWORD dwIndex, UINT nState);
    static CRefObjectPtr<CAtCodec> CreateReqMessagePDUTransferWriteMessageToMemoryNew(BYTE* pMsgData, int nDataLen);
    //-------------------------------------------------------------------------
    // Password
    // Implementation in MessageCodec.cpp
    //-------------------------------------------------------------------------
    static CRefObjectPtr<CAtCodec> CreateReqCheckPassword(LPCTSTR pszPassword);
    //

    //-------------------------------------------------------------------------
    // Message
    // Implementation in MessageCodec.cpp
    //-------------------------------------------------------------------------
    void DecodeListMessagesTi(char* pBuffer, DWORD dwBufferLength);
    void DecodeListMessagesStd(char* pBuffer, DWORD dwBufferLength);
    void DecodeWriteMessageToMemoryTi(char* pBuffer);
    void DecodeWriteMessageToMemoryStd(char* pBuffer);
    void DecodeDirectSendMessage(char* pBuffer);
    void DecodeSendStoredMessageTi(char* pBuffer);
    void DecodeSendStoredMessageStd(char* pBuffer);
    void DecodeReadMessageTi(char* pBuffer, DWORD dwBufferLength);
    void DecodeReadMessageStd(char* pBuffer, DWORD dwBufferLength);
    void DecodeGetServiceCenterAddress(char* pBuffer);
    void DecodePreferredStorage(char* pBuffer);
    void DecodeQueryMessageSpace(char* pBuffer);
    void DecodeServiceCenterAddress(char* pBuffer, int& nResult);
    void DecodeListFolder(char* pBuffer);
    void DecodeReadMessageNew(char* pBuffer, DWORD dwBufferLength);
    void DecodeListMessagesNew(char* pBuffer, DWORD dwBufferLength);
    void DecodeWriteMessageNewResult(char* pBuffer);
    void DecodeUpdateMessageState(char* pBuffer);

    bool IsCompleteResult(Opcode_t eActiveOpcode, CBufferPtr spBuffer);
    bool DecodeResult();

    // Get Service Center Address
    CString GetSCA(){ return m_strSCA; }
    // Get Message ID List
    CString GetMessageIDList(){ return m_strMessageIDList; }
    // Get Message List (Used for CMGL)
    void GetMessageList(std::vector<SM>& vecSM) { vecSM = this->m_vecSM; }
    void GetShortMessage(std::vector<CShortMessage>& vecShortMessage) { vecShortMessage = this->m_vecShortMessage; }
    void GetFolderList(std::vector<CMessageFolderInfo>& vecFolder) { vecFolder = this->m_vecFolder;}
    void SetReadMessage(CShortMessage& csm) { this->m_shortMessage = csm;}
    void GetReadMessage(CShortMessage& csm) { csm = this->m_shortMessage;}
    // Get Message Index
    DWORD GetMsgID(){ return m_dwMessageIndex; }    
    // Get Message Size
    CString GetTime() { return m_strTime; }
    DWORD GetMsgSize() const { return m_dwMsgSize; }
    // Get Message Buffer
    CBufferPtr GetMsgBuffer() const { return m_spMsgBuffer; }
    // Get Used count for Standard
    void GetSetMessageStorageResult(MemUsed& used) {
        used.m_dwTotalMem1 = m_dwTotalMem1;
        used.m_dwTotalMem2 = m_dwTotalMem2;
        used.m_dwTotalMem3 = m_dwTotalMem3;
        used.m_dwUsedMem1  = m_dwUsedMem1;
        used.m_dwUsedMem2  = m_dwUsedMem2;
        used.m_dwUsedMem3  = m_dwUsedMem3;
    }
    // Get total used count
    DWORD GetUsedTi() const { return m_dwUsedTi; };
    // Get total message count
    DWORD GetTotalTi() const { return m_dwTotalTi; };
    //
    Opcode_t GetOpcode() const { return m_eOpcode; }
    Opcode_t GetActiveOpcode() const { return m_eActiveOpcode; }
    CBufferPtr GetPackageBuffer() const { return m_spPackageBuffer; }
    CME_Error_t GetExtendedCMEErrorCode() const { return m_eCMEExtendedError; }
    CMS_Error_t GetExtendedCMSErrorCode() const { return m_eCMSExtendedError; }
    FS_Error_t GetExtendedFSErrorCode() const { return m_eFSError; } 
    //
    LPCTSTR GetPhoneType() const { return m_strPhoneType; }
    LPCTSTR GetManufacturer() const { return m_strManufacturer; }
    LPCTSTR GetIMEI() const { return m_strIMEI; }
    LPCTSTR GetDeviceInfoBBClient() const { return m_strDeviceInfoBBClient; }
    LPCTSTR GetSoftwareVersion() const { return m_strSoftwareVersion; }
    long GetSignalQuality() const { return m_nSignalQuality; }
    long GetBatteryCharge() const { return m_nBatteryCharge; }
    //	NetworkRegistration_t GetNetworkStatus() const { return m_enuNetworkStatus; }
    //
    LPCTSTR GetVolumes() const { return m_strVolumes; }
    LPCTSTR GetDirEntries() const { return m_strDirEntries; }
    DWORD GetTotalCapacity() const { return m_dwTotalCapacity; }
    DWORD GetFreeCapacity() const { return m_dwFreeCapacity; }
    DWORD GetFileLength() const { return m_dwFileLength; }
    //
    DWORD GetPackageSize() const { return m_dwPackageSize; }
    SAFEARRAY* GetSafeArrayData() const { return m_psaData; }
    bool GetEnabled() const { return m_bEnabled; }
    // PhoneBook
    long GetIndexRangeMin () const {return m_nIndexRangeMin;}
    long GetIndexRangeMax () const {return m_nIndexRangeMax;}
    long GetAllowedLengthNumber () const {return m_nAllowedLengthNumber;}
    long GetAllowedLengthName () const {return m_nAllowedLengthName;}
    LPCTSTR GetSupportedPhonebooks() const { return m_strSupportedPhonebooks; }
    CPBEntriesPtr GetPBEntries ()
    {
        CPBEntriesPtr pPBEntries(m_pPBEntries); 
        m_pPBEntries = NULL; 
        return pPBEntries; 
    }

	DWORD GetCurrentReadPhonebookEntryIndex () const {
		return this->m_dwCurrentReadPhonebookEntryIndex;
	}
	BOOL IsCurrentReadVCardEmpty () {
		return this->m_bIsCurrentReadVCardEmpty;
	}
	CString GetBenQVCard () const {
		return this->m_strBenQVCard;
	}

	BOOL GetCurrentReadCallergroup (CallergroupSetting_t& tagCallergroupSetting) {\
		tagCallergroupSetting.nID = this->m_tagCurrentReadCallergroup.nID;\
		strcpy(tagCallergroupSetting.szName, this->m_tagCurrentReadCallergroup.szName);\
		tagCallergroupSetting.dwRingtoneIdType = this->m_tagCurrentReadCallergroup.dwRingtoneIdType;\
		strcpy(tagCallergroupSetting.szRingtoneIdPath, this->m_tagCurrentReadCallergroup.szRingtoneIdPath);\
		tagCallergroupSetting.dwImageIdType = this->m_tagCurrentReadCallergroup.dwImageIdType;\
		strcpy(tagCallergroupSetting.szImageIdPath, this->m_tagCurrentReadCallergroup.szImageIdPath);\
		return TRUE;\
	}
	// Calendar

	DWORD GetCurrentReadCalendarEntryIndex () const {\
		return this->m_dwCurrentReadCalendarEntryIndex;\
	}
	BOOL IsCurrentReadVCalendarEmpty () {\
		return this->m_bIsCurrentReadVCalendarEmpty;\
	}
	CString GetBenQVCalendar () const {\
		return this->m_strBenQVCalendar;\
	}

	DWORD GetCurrentWriteCalendarEntryIndex () const {\
		return this->m_dwCurrentWriteCalendarEntryIndex;\
	}
	//================= Steven CC Chen added codes ==================END//
    // Conversion
    //bool ConvertOrdinalToNetworkStatus( long lOrdinal, NetworkRegistration_t &enuNetworkStatus );

private:
    CString GetToken(char * pTxt, char StartDelimiter, char EndDelimiter);
	HRESULT NextToken(const char* szFirstDelimiter, char* szFirstGrid,
                      const char* szSecondDelimiter, char* szSecondGrid,
                      char* source, DWORD dwSourceLen,
                      DWORD* pStartPos, DWORD* pEndPos);
    bool IsCompleteButInvalidResult(  Opcode_t eActiveOpcode, DWORD dwPackageSize, LPCSTR pPackageBuffer, CString* pstrDescription );
    CString ExtractParString( LPCSTR szBuffer, LPCSTR szPrefix );
    Opcode_t GetUnsolicitedEvent( LPCSTR szBuffer, DWORD& dwValue, DWORD& dwId);
	bool CutBeginningCrlf(CString &str);
    int HexStr2Bin (unsigned char *pBin, DWORD dwNum, unsigned char *pHexStr, DWORD dwSize);
    static int Bin2HexStr(char *pHexStr, DWORD dwSize, unsigned char *pBin, DWORD dwNum);
    //int PDUStr2Bin (unsigned char *pBin, DWORD dwNum, unsigned char *pHexStr, DWORD dwSize);
    //================= Steven CC Chen added codes ==================BEGIN//
    //bool ExtractTokenFromList (CString in_strTokenList, CStringList& out_strlstToken);
    bool ExtractParameterFromList (CString in_strParameterList, CString in_strParameterSeparator, CStringList& out_strlstParameter, bool in_bIsEmptyAllowed = false);
    //================= Steven CC Chen added codes ==================END//


    //-------------------------------------------------------------------------
    // Refactoring function. If there is error occuring, remove this function
    // and replacing the codes into the function call position will be fine.
    //-------------------------------------------------------------------------
    int ParseError(char* pBuffer, int nResult);
    void ParseVCardError(char* pBuffer);
    void ParseVCalendarError(char* pBuffer);
    void ParseUnsolicitedEvent(char* pBuffer, bool& bSuccess, int& dw);
    bool ParsePDUPrompt(bool bSuccess);
    void ParseGetIMEI(char* pBuffer);
    void ParseGetFile(char* pBuffer);
    void ParseGetPBRangesAndLengthsStd(char* pBuffer);
    void ParseReadPhonebookEntriesStd(char* pBuffer);
    int ParseGetSupportedPhonebooksStd(char* pBuffer, int nResult);
    void ParseWriteCalendarEntryData(char* pBuffer);
    void ParseReadPhonebookEntry(char* pBuffer);
    void ParseListUsedPhonebookEntries(char* pBuffer);
    void ParseReadCallergroupSetting(char* pBuffer, int nResult);
    void ParseReadCalendarEntry(char* pBuffer);

    static CRefObjectPtr<CAtCodec> CreateAtCodec(Opcode_t opCode, DWORD bufferLength, const char* sz);

public:

// The member variables hold decode results. They are not used during encoding.
private:
    Opcode_t m_eOpcode;
    CME_Error_t m_eCMEExtendedError;
    CMS_Error_t m_eCMSExtendedError;
	FS_Error_t m_eFSError;
    CString m_strPhoneType;
    CString m_strManufacturer;
    CString m_strIMEI;
	CString m_strIMEISV;
    CString m_strDeviceInfoBBClient;
    CString m_strSoftwareVersion;
    long m_nSignalQuality;
    long m_nBatteryCharge;
    //	NetworkRegistration_t m_enuNetworkStatus;
	// PhoneBook
	long m_nIndexRangeMin;
	long m_nIndexRangeMax;      
	long m_nAllowedLengthNumber;
	long m_nAllowedLengthName;
	CPBEntriesPtr m_pPBEntries;
	CString m_strSupportedPhonebooks;
	//================= Steven CC Chen added codes ==================BEGIN//
	// AddressBook
	DWORD m_dwPhonebookAvailableEntryCount;
    DWORD m_dwPhonebookUsedEntryCount;
    DWORD m_dwAddressbookAvailableEntryCount;
    DWORD m_dwAddressbookUsedEntryCount;
	DWORD m_dwCurrentReadPhonebookEntryIndex;
    DWORD m_dwCurrentReadAddressbookEntryIndex;
    bool m_bIsCurrentReadVCardEmpty;
    CString m_strBenQVCard;
    DWORD m_dwCurrentWriteAddressbookEntryIndex;
    CStringList m_strlstDeleteSupportedStorages;
    CStringList m_strlstListSupportedStorages;
    CStringList m_strlstUsedPhonebookEntryIndexes;
    CStringList m_strlstUsedAddressbookEntryIndexes;
    DWORD m_dwCallergroupIdMin;
    DWORD m_dwCallergroupIdMax;
    CallergroupSetting_t m_tagCurrentReadCallergroup;
	// Calendar
    DWORD m_dwCalendarAvailableEntryCount;
    DWORD m_dwCalendarUsedEntryCount;
    DWORD m_dwCurrentReadCalendarEntryIndex;
    bool m_bIsCurrentReadVCalendarEmpty;
    CString m_strBenQVCalendar;
    CStringList m_strlstCalDeleteSupportedActions;
	DWORD m_dwCalActionIdDelete;
	DWORD m_dwCalActionIdDeleteAll;
	bool m_bIsVCalendarDeleteAllSupported;
	CStringList m_strlstUsedCalendarEntryIndexes;
	DWORD m_dwCurrentWriteCalendarEntryIndex;
    //================= Steven CC Chen added codes ==================END//
	SAFEARRAY* m_psaData;
    bool m_bEnabled;
	//
    // Intermediate (hold information between IsCompleteResult() and DecodeResult()
    DWORD m_dwPackageSize;
    CBufferPtr m_spPackageBuffer;
    Opcode_t m_eActiveOpcode;
	//
    DWORD m_dwVolumes;
    CString m_strVolumes;
    CString m_strDirEntries;
    DWORD m_dwTotalCapacity;
    DWORD m_dwFreeCapacity;
    DWORD m_dwFileLength;
	//
	//Messages
//	TSMSCollection m_aSMSCollection;
	CString m_strMessageIDList; ///< return message id list
    std::vector<SM> m_vecSM;    ///< return total SM
    std::vector<CShortMessage> m_vecShortMessage;   ///< AT+CMGL return this vector
    std::vector<CMessageFolderInfo> m_vecFolder;    ///< AT$BQMGFL return this vector
    CShortMessage m_shortMessage;   ///< AT$BQMGR return this message
//	CSMS m_SMS;    
	//
	DWORD m_dwUsedMem1;    
    DWORD m_dwUsedMem2;    
    DWORD m_dwUsedMem3;
    DWORD m_dwTotalMem1;
    DWORD m_dwTotalMem2;
    DWORD m_dwTotalMem3;
	DWORD m_dwUsedTi;
    DWORD m_dwTotalTi;  
	CString m_strSCA;          
	CString m_strToSCA;
    CString m_strTime;
	DWORD m_dwMessageIndex;
	DWORD m_dwMessageReference;
	DWORD m_dwMsgSize;
    CBufferPtr m_spMsgBuffer;
	DWORD m_dwStatus;
	int m_nMsgCount;

};

typedef CRefObjectPtr<CAtCodec> CAtCodecPtr;

typedef enum tag_AuMa_Purpose
{
    E_AUMA_PURPOSE_MINITEM=-1, //DO NOT USE THIS AS A PURPOSE !!! ONLY FOR RANGE CHECKING !!!
    E_AUMA_PURPOSE_MTC=0, ///<Ringtone triggered by a MTC
    E_AUMA_PURPOSE_DISTRIBUTIONLIST_MTC, ///<Ringtone triggered by a MTC for distribution list
    E_AUMA_PURPOSE_VOICECALL, ///<Voice call / voice channel
    E_AUMA_PURPOSE_PTT, ///<Ringtone triggered by PTT
    E_AUMA_PURPOSE_LOW_POWER, ///<Low-power sound
    E_AUMA_PURPOSE_SMS, ///<SMS notification
    E_AUMA_PURPOSE_DISTRIBUTIONLIST_SMS, ///<Ringtone triggered by a SMS for distribution list
    E_AUMA_PURPOSE_MMS, ///<MMS notification
    E_AUMA_PURPOSE_CBS, ///<CBS notification
    E_AUMA_PURPOSE_IMPS, ///<IMPS notification
    E_AUMA_PURPOSE_EMAIL, ///<IMPS notification
    E_AUMA_PURPOSE_VOICEMEMO_RECORD, ///<Record a voice Memo
    E_AUMA_PURPOSE_APPLICATION_RECORD, ///<Used for Ringtone record or record from Java Applications
    E_AUMA_PURPOSE_ALARMCLOCK, ///<Alarm clock
    E_AUMA_PURPOSE_ORGANIZER_ALARM, ///<All kinds of organizer alarms
    E_AUMA_PURPOSE_VR, ///<Voice Recognition sounds
    E_AUMA_PURPOSE_APPLICATION_PLAYBACK, ///<Other application sounds
    E_AUMA_PURPOSE_APPLICATION_BROWSER, ///<application sound used by browser
    E_AUMA_PURPOSE_APPLICATION_VOLUME, ///<application sound used by apbas for volume
    E_AUMA_PURPOSE_APPLICATION_AVATAR, ///<application sound used by avatar
    E_AUMA_PURPOSE_APPLICATION_IDC, ///<application sound used by Idle call
    E_AUMA_PURPOSE_APPLICATION_VOICEAL, ///<application sound used by VoiceAL
    E_AUMA_PURPOSE_APPLICATION_MPLAYER, ///<application sound used by Media player
    E_AUMA_PURPOSE_APPLICATION_JAVA_GEN, ///<application sound used by Java in general
    E_AUMA_PURPOSE_APPLICATION_JAVA_GAMES, ///<application sound used by Java for games
    E_AUMA_PURPOSE_APPLICATION_MESSAGES, ///<application sound used for embedded sound in messages
    E_AUMA_PURPOSE_PTT_SIGNAL, ///<application sound used by PTT for signaling
    E_AUMA_PURPOSE_PTT_TALK, ///<application sound used by PTT talk burst
    E_AUMA_PURPOSE_FAX_DATA, ///<All Info Tones
    E_AUMA_PURPOSE_FAX_ALERT, ///<All Key Tones
    E_AUMA_PURPOSE_STARTUP, ///<application sound used by SoundAE for startup
    E_AUMA_PURPOSE_SHUTDOWN, ///<application sound used by SoundAE for shutdown
    E_AUMA_PURPOSE_CAMERA_CLICK, ///<Camera Click sounds
    E_AUMA_PURPOSE_FMRADIO, ///<FM Radio
    E_AUMA_PURPOSE_SLIDER, ///<Slider sound
    E_AUMA_PURPOSE_SLIDER_OPEN, ///<Slider sound for Open
    E_AUMA_PURPOSE_SLIDER_CLOSE, ///<Slider sound for Close
    E_AUMA_PURPOSE_SERVICE_TONES, ///<All Service Tones
    E_AUMA_PURPOSE_INFO_TONES, ///<All Info Tones
    E_AUMA_PURPOSE_KEY_TONES, ///<All Key Tones
    E_AUMA_PURPOSE_CALLWAITING_TONES, ///<Call Waiting Tones
    E_AUMA_PURPOSE_CONNECT_TONE, ///<Connect Tone
    E_AUMA_PURPOSE_LOCAL_RINGING_TONE, ///<Local Ringing Tones
    E_AUMA_PURPOSE_LIGHT_MISSED_EVENTS , ///<Missed events used for light
    E_AUMA_PURPOSE_LIGHT_NET_SEARCH, ///<net search used for light
    E_AUMA_PURPOSE_LIGHT_MEDIA_PLAYER, ///<Media player used for light
    E_AUMA_PURPOSE_MAXITEM //DO NOT USE THIS AS A PURPOSE !!! ONLY FOR RANGE CHECKING !!!
} E_AuMa_Purpose;


typedef enum SAE_Event_definitions
{
    E_SAE_EVENT_DUMMY_MIN = INT_MIN,   ///< Dummy value to force the size of this enum to the size of int, like ANSI-C. 
    E_SAE_EVENT_MTC_START_MARKER = 0,  ///< Dummy value to mark the starting of MTC events 
    E_SAE_EVENT_MTC_LINE1 = E_SAE_EVENT_MTC_START_MARKER, ///< Call on line 1 
    E_SAE_EVENT_MTC_LINE2,             ///< Call on line 2 
    E_SAE_EVENT_MTC_PB,                ///< Phone/Addressbook call 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE1,   ///< Call adressbook group 1 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE2,   ///< Call adressbook group 2 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE3,   ///< Call adressbook group 3 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE4,   ///< Call adressbook group 4 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE5,   ///< Call adressbook group 5 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE6,   ///< Call adressbook group 6 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE7,   ///< Call adressbook group 7 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE8,   ///< Call adressbook group 8 
    E_SAE_EVENT_MTC_AB_DISTINCTIVE9,   ///< Call adressbook group 9 
    E_SAE_EVENT_RING_ONLY,             ///< MTC with sound only    
    E_SAE_EVENT_SILENT_MTC,            ///< MTC effect for vibra and light 
    E_SAE_EVENT_MTC_END_MARKER = E_SAE_EVENT_SILENT_MTC, ///< Dummy value to mark the ending of MTC events 
    E_SAE_EVENT_MMS,                   ///< Incoming MMS 
    E_SAE_EVENT_SMS,                   ///< Incoming SMS 
    E_SAE_EVENT_CBS,                   ///< Incoming CBS 
    E_SAE_EVENT_ALARM_CLOCK,           ///< Expired Alarm Clock 
    E_SAE_EVENT_ORG_ALARM,             ///< Expired Organizer Alarm 
    E_SAE_EVENT_POC_RINGER,            ///< Push to talk alarm 
    E_SAE_EVENT_IMPS,                  ///< Chat Client , IMPS
    E_SAE_EVENT_EMAIL,                 ///< Incoming Email 
    E_SAE_EVENT_DISTRIBUTION,          ///< Distribution List
    E_SAE_EVENT_STARTUP,               ///< Mobile startup event 
    E_SAE_EVENT_SHUTDOWN,              ///< Mobile shutdown event 
    E_SAE_EVENT_FAX_DATA,              ///< Fax Data 
    E_SAE_EVENT_FAX_ALERT,             ///< Fax Alert 
    E_SAE_EVENT_SLIDER_OPEN,           ///< Slider open (if supported) 
    E_SAE_EVENT_SLIDER_CLOSE,          ///< Slider close (if supported) 
    E_SAE_EVENT_CAMERACLICK,           ///< Cameraclick 
    E_SAE_EVENT_VIDEOCLICK,            ///< Videoclick 
    E_SAE_EVENT_LOWBAT,                ///< LowBat event 
    E_SAE_EVENT_COUNTDOWN_EXPIRED,     ///< Countdown expired event 
    E_SAE_EVENT_MISSED_EVENTS,         ///< When any missed event is indicated in idle mode
    E_SAE_EVENT_NET_SEARCH,            ///< When device is searching for a network 
    E_SAE_EVENT_MEDIAPLAYER,           ///< When Media Player plays sound 
    E_SAE_EVENT_CALL_WAITING,          ///< Used for Call waiting as an separate event
    E_SAE_EVENT_WAP_PUSH_AUDIO,        ///< WAP PUSH event only audio 
    E_SAE_EVENT_WAP_PUSH_EFFECT,       ///< WAP PUSH event only effect 
    E_SAE_EVENT_SMS_RING_ONLY,         ///< SMS ringing only 
    E_SAE_EVENT_LOWBAT_INDICATION,     ///< LowBat Indication, comes as an warning tone 
    E_SAE_EVENT_UNKNOWN,               ///< Unknown event 
    E_SAE_EVENT_DUMMY_MAX = INT_MAX    ///< Dummy value to force the size of this enum to size of int, like ANSI-C. 
} E_SAE_Event;

typedef enum HMI_Graph_definitions
{
    // - Animations/Images (Graphics), which are available by setup menu (partly not) -------------------------
    eRegGraph_Start = 100,
    eRegGraph_Wallpaper,                    ///< Index 101: Wallpaper
    eRegGraph_Logo,                         ///< Index 102: Provider-Logo
    eRegGraph_Startup,                      ///< Index 103: Mobile startup animation
    eRegGraph_Shutdown,                     ///< Index 104: Mobile shutdown animation
    eRegGraph_NetSearch,                    ///< Index 105: Net search animation
    eRegGraph_Preview,                      ///< Index 106: Preview animation
    eRegGraph_InsertSim,                    ///< Index 107: Insert Sim animation
    eRegGraph_Wait,                         ///< Index 108: Wait animation
    eRegGraph_PinInput,                     ///< Index 109: Pin input animation
    eRegGraph_Screensaver,                  ///< Index 110: Screensaver
    eRegGraph_ScreensaverType,              ///< Index 111: Screensaver type
    eRegGraph_Distinctive_Face,             ///< Index 112: Calling face no Group
    eRegGraph_Distinctive1_Face,            ///< Index 113: Calling face Group 1
    eRegGraph_Distinctive2_Face,            ///< Index 114: Calling face Group 2
    eRegGraph_Distinctive3_Face,            ///< Index 115: Calling face Group 3
    eRegGraph_Distinctive4_Face,            ///< Index 116: Calling face Group 4
    eRegGraph_Distinctive5_Face,            ///< Index 117: Calling face Group 5
    eRegGraph_Distinctive6_Face,            ///< Index 118: Calling face Group 6
    eRegGraph_Distinctive7_Face,            ///< Index 119: Calling face Group 7
    eRegGraph_Distinctive8_Face,            ///< Index 120: Calling face Group 8
    eRegGraph_Distinctive9_Face,            ///< Index 121: Calling face Group 9
    eRegGraph_Distinctive_Video,            ///< Index 122: Calling video no Group
    eRegGraph_Distinctive1_Video,           ///< Index 123: Calling video Group 1
    eRegGraph_Distinctive2_Video,           ///< Index 124: Calling video Group 2
    eRegGraph_Distinctive3_Video,           ///< Index 125: Calling video Group 3
    eRegGraph_Distinctive4_Video,           ///< Index 126: Calling video Group 4
    eRegGraph_Distinctive5_Video,           ///< Index 127: Calling video Group 5
    eRegGraph_Distinctive6_Video,           ///< Index 128: Calling video Group 6
    eRegGraph_Distinctive7_Video,           ///< Index 129: Calling video Group 7
    eRegGraph_Distinctive8_Video,           ///< Index 130: Calling video Group 8
    eRegGraph_Distinctive9_Video,           ///< Index 131: Calling video Group 9
    eRegGraph_End,                          ///< Index 132
} E_HMI_Graph;

typedef enum HMI_Audio_definitions
{
    // - Sounds (Audio), which are available by setup menu ----------------------------------------------------
    eRegAudio_Start = 200,
    eRegAudio_Calls,                        ///< Index 201: Ring tone Calls
    eRegAudio_Distinctive,                  ///< Index 202: Ring tone Calls no Group
    eRegAudio_Distinctive1,                 ///< Index 203: Ring tone Calls Group 1
    eRegAudio_Distinctive2,                 ///< Index 204: Ring tone Calls Group 2
    eRegAudio_Distinctive3,                 ///< Index 205: Ring tone Calls Group 3
    eRegAudio_Distinctive4,                 ///< Index 206: Ring tone Calls Group 4
    eRegAudio_Distinctive5,                 ///< Index 207: Ring tone Calls Group 5
    eRegAudio_Distinctive6,                 ///< Index 208: Ring tone Calls Group 6
    eRegAudio_Distinctive7,                 ///< Index 209: Ring tone Calls Group 7
    eRegAudio_Distinctive8,                 ///< Index 210: Ring tone Calls Group 8
    eRegAudio_Distinctive9,                 ///< Index 211: Ring tone Calls Group 9
    eRegAudio_Appointments,                 ///< Index 212: Ring tone Appointments
    eRegAudio_Messages,                     ///< Index 213: Ring tone Messages
    eRegAudio_AlarmClock,                   ///< Index 214: Ring tone Alarm Clock
    eRegAudio_InstantMessaging,             ///< Index 215: Ring tone Instant Messaging
    eRegAudio_Games,                        ///< Index 216: Ring tone Games
    eRegAudio_Startup,                      ///< Index 217: Ring tone mobile startup
    eRegAudio_Shutdown,                     ///< Index 218: Ring tone mobile shutdown
    eRegAudio_OpenSlider,                   ///< Index 219: Ring tone Open Slider
    eRegAudio_CloseSlider,                  ///< Index 220: Ring tone Close Slider
    eRegAudio_CallLine1,                    ///< Index 221: Ring tone unknown incoming call (for primary number)
    eRegAudio_CallLine2,                    ///< Index 222: Ring tone unknown incoming call for secondary number (E-Plus only)
    eRegAudio_PTTRingTone,                  ///< Index 223: Ring tone Push to talk Over Cellular
    eRegAudio_OpenClamshell,                ///< Index 224: Ring tone Open Clamshell
    eRegAudio_CloseClamshell,               ///< Index 225: Ring tone Close Clamshell
    eRegAudio_End,                          ///< Index 226
} E_HMI_Audio;

#endif // !defined(AFX_ATCODEC_H__868D35E0_6422_4AF3_9A5E_1AC03E626F87__INCLUDED_)
