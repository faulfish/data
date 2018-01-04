// AtCodec.cpp: implementation of the CAtCodec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtCodec.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtCodec::CAtCodec() : m_dwCurrentReadCalendarEntryIndex(0), m_bIsCurrentReadVCalendarEmpty(true),
m_dwCurrentWriteCalendarEntryIndex(0), m_dwMessageIndex(0), m_dwMessageReference(0),
m_dwUsedMem1(0), m_dwUsedMem2(0), m_dwUsedMem3(0), m_dwTotalMem1(0), m_dwTotalMem2(0), m_dwTotalMem3(0)
{
}

CAtCodec::~CAtCodec()
{
}

LPCTSTR CAtCodec::opcode2t( Opcode_t eOpcode )
{
    switch( eOpcode )
    {
        //
    case otNone								: return _T("otNone");
    case otRing								: return _T("otRing");
    case otError							: return _T("otError");
    case otCMEError							: return _T("otCMEError");
    case otCMSError							: return _T("otCMSError");
    case otFSError							: return _T("otFSError");
    case otSingleCarriageReturn				: return _T("otSingleCarriageReturn");
    case otSingleLineFeed					: return _T("otLineFeed");
    case otEchoOfCommand				    : return _T("otEchoOfCommand");
    case otResult							: return _T("otResult");
    case otDiscard							: return _T("otDiscard");
    case otRequest						    : return _T("otRequest");
    case otReqAt					        : return _T("otReqAt");
    case otResAt							: return _T("otResAt");
        //
    case otReqGetPhoneType					: return _T("otReqGetPhoneType");
    case otReqGetManufacturer				: return _T("otReqGetManufacturer");
    case otReqGetIMEI						: return _T("otReqGetIMEI");
    case otReqGetDeviceInfoBBClient			: return _T("otReqGetDeviceInfoBBClient");
    case otReqGetSoftwareVersion			: return _T("otReqGetSoftwareVersion");
    case otReqGetSignalQuality			    : return _T("otReqGetSignalQuality");
    case otReqGetBatteryCharge				: return _T("otReqGetBatteryCharge");
    case otReqSwitchToOBEX					: return _T("otReqSwitchToOBEX");
    case otReqSwitchToMux					: return _T("otReqSwitchToMux"); 
    case otReqSwitchToUCS2				    : return _T("otReqSwitchToUCS2"); 
    case otReqSwitchToGSM				    : return _T("otReqSwitchToGSM"); 
        //
    case otReqGetVolumes					: return _T("otReqGetVolumes");
    case otReqGetEntries					: return _T("otReqGetEntries");
    case otReqGetCapacity					: return _T("otReqGetCapacity");
    case otReqGetFile					    : return _T("otReqGetFile");
    case otResGetFile				        : return _T("otResGetFile");
    case otReqPutFile			            : return _T("otReqPutFile");
    case otReqCopyFile		                : return _T("otReqCopyFile");
        // Phonebook Requests
    case otReqGetPBRangesAndLengthsStd		: return _T("otReqGetPBRangesAndLengthsStd");
    case otResGetPBRangesAndLengthsStd		: return _T("otResGetPBRangesAndLengthsStd");
    case otReqReadPhonebookEntriesStd		: return _T("otReqReadPhonebookEntriesStd");
    case otResReadPhonebookEntriesStd		: return _T("otResReadPhonebookEntriesStd");
    case otReqGetSupportedPhonebooksStd		: return _T("otReqGetSupportedPhonebooksStd");
    case otResGetSupportedPhonebooksStd		: return _T("otResGetSupportedPhonebooksStd");
        //================= Steven CC Chen added codes ==================BEGIN//
        // AdddressBook related Requests
    case otReqGetAddrRangesAndCounts        : return _T("otReqGetAddrRangesAndCounts");
    case otReqReadPhonebookEntry			: return _T("otReqReadPhonebookEntry");
    case otReqReadAddressbookEntry          : return _T("otReqReadAddressbookEntry");
    case otReqWriteAddressbookEntryData     : return _T("otReqWriteAddressbookEntryData");
    case otReqGetDeleteSupportedStorageList : return _T("otReqGetDeleteSupportedStorageList");
    case otReqGetListSupportedStorageList   : return _T("otReqGetListSupportedStorageList");
    case otReqListUsedPhonebookEntries      : return _T("otReqListUsedPhonebookEntries");
    case otReqListUsedAddressbookEnties     : return _T("otReqListUsedAddressbookEnties");
    case otReqGetCallergroupIdRange         : return _T("otReqGetCallergroupIdRange");
    case otReqReadCallergroupSetting        : return _T("otReqReadCallergroupSetting");
        // Calendar related Requests
    case otReqGetCalUsedIndexesAndCount     : return _T("otReqGetCalUsedIndexesAndCount");
    case otReqReadCalendarEntry				: return _T("otReqReadCalendarEntry");
    case otReqGetCalDeleteSupportedActions  : return _T("otReqGetCalDeleteSupportedActions");
    case otReqWriteCalendarEntryData		: return _T("otReqWriteCalendarEntryData");
        //================= Steven CC Chen added codes ==================END//
        // Unsolicited Events
    case otUnsolicitedEvent					: return _T("otUnsolicitedEvent");
    case otEventBatteryChargeChanged		: return _T("otEventBatteryChargeChanged");
    case otEventSignalQualityChanged		: return _T("otEventSignalQualityChanged");
    case otEventNetworkRegistrationChanged	: return _T("otEventNetworkRegistrationChanged");
        //
    case otResGetManufacturer			    : return _T("otResGetManufacturer");
    case otResGetIMEI						: return _T("otResGetIMEI");
    case otResGetDeviceInfoBBClient			: return _T("otResGetDeviceInfoBBClient");
    case otResGetSoftwareVersion			: return _T("otResGetSoftwareVersion");
    case otResGetSignalQuality			    : return _T("otResGetSignalQuality");
    case otResGetBatteryCharge				: return _T("otResGetBatteryCharge");
    case otResSwitchToOBEX				    : return _T("otResSwitchToOBEX");
    case otResOk						    : return _T("otOk");
    case otResERROR					        : return _T("otResERROR");
        //
    case otReqListMessagesTi				: return _T("otReqListMessagesTi");
    case otReqListMessagesStd				: return _T("otReqListMessagesStd");
    case otReqWriteMessageToMemoryTi		: return _T("otReqWriteMessageToMemoryTi");
    case otReqWriteMessageToMemoryStd		: return _T("otReqWriteMessageToMemoryStd");
    case otReqPDUDataWriteTi				: return _T("otReqPDUDataWriteTi");
    case otReqPDUDataWriteStd				: return _T("otReqPDUDataWriteStd");
    case otReqDirectSendMessage				: return _T("otReqDirectSendMessage");
    case otReqSendStoredMessageTi			: return _T("otReqSendStoredMessageTi");
    case otReqSendStoredMessageStd			: return _T("otReqSendStoredMessageStd");
    case otReqPDUDataDirectSend				: return _T("otReqPDUDataDirectSend"); 
    case otReqReadMessageTi					: return _T("otReqReadMessageTi");
    case otReqReadMessageStd				: return _T("otReqReadMessageStd");
    case otReqGetServiceCenterAddress		: return _T("otReqGetServiceCenterAddress");
    case otReqPreferredMessageStorage		: return _T("otReqPreferredMessageStorage");
        //
    default				                    : return _T("*** unknown ***");
    }
}

// Create a request for setting flowing control
CAtCodecPtr CAtCodec::CreateReqSetFlowControl(int nFlowControl)
{
    const char* sz = "AT+IFC=2\r";
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

// Create a request for getting phone type
CAtCodecPtr CAtCodec::CreateReqGetPhoneType()
{
    const char* sz = "AT+CGMM\r";
    return CreateAtCodec(otReqGetPhoneType, (DWORD)strlen(sz), sz);
}

// Create a request for getting Manufacturer name
CAtCodecPtr CAtCodec::CreateReqGetManufacturer()
{
    const char* sz = "AT+CGMI\r";
    return CreateAtCodec(otReqGetManufacturer, (DWORD)strlen(sz), sz);
}

// Create a request for getting IMEI
CAtCodecPtr CAtCodec::CreateReqGetIMEI()
{
    const char* sz = "AT+CGSN\r";
    return CreateAtCodec(otReqGetIMEI, (DWORD)strlen(sz), sz);
}

// Create a request for getting software version(load version)
CAtCodecPtr CAtCodec::CreateReqGetSoftwareVersion()
{
    const char* sz = "AT+CGMR\r";
    return CreateAtCodec(otReqGetSoftwareVersion, (DWORD)strlen(sz), sz);
}

// Create a request for switching to UCS2
CAtCodecPtr CAtCodec::CreateReqSwitchToUCS2()
{
    const char* sz = "AT+CSCS=\"UCS2\"\r";
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

// Create a request for switching to UCS2
CAtCodecPtr CAtCodec::CreateReqSwitchToGSM()
{
    const char* sz = "AT+CSCS=\"GSM\"\r";
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

// Create a request for switching to OBEX
// 西門子特有指令
CAtCodecPtr CAtCodec::CreateReqSwitchToOBEX()
{
    const char* sz = "AT^SQWE=3\r";
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

// Create a request for switching to AT
CAtCodecPtr CAtCodec::CreateReqSwitchToAT()
{
    const char* sz = "+++";
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

// Create a request for Echo off, there is no response for input
CAtCodecPtr CAtCodec::CreateEchoOff()
{   
    const char* sz = "ATE0\r";
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
}

// Search inside pBig for pSmall
const char* findbuffer( const char* pBig, DWORD dwBigLength, const char* pSmall, DWORD dwSmallLength )
{
    for( DWORD dw1 = 0; dw1 <= dwBigLength - dwSmallLength; dw1++ )
    {
        DWORD dw2 = 0;
        for( ; dw2 < dwSmallLength; dw2++ )
        {
            if( pBig[dw1+dw2] != pSmall[dw2] )
                break;
        }

        if( dw2 == dwSmallLength )
            return pBig+dw1;
    }

    return NULL;
}

bool CAtCodec::IsCompleteButInvalidResult(Opcode_t eActiveOpcode, DWORD dwPackageSize, LPCSTR pPackageBuffer, CString* pstrDescription )
{
    return false;
}

bool CAtCodec::IsCompleteResult( Opcode_t eActiveOpcode, CBufferPtr spBuffer )
{
    m_dwPackageSize = 0;

    DWORD dwPackageSize = spBuffer ? spBuffer->GetLength() : 0;
    LPCSTR pPackageBuffer = spBuffer ? spBuffer->GetData() : NULL;

    if( dwPackageSize == 0 )
        return false;

    m_eOpcode = otNone;

    if( dwPackageSize >= 2 && !memcmp( pPackageBuffer, "> ", 2 ) )
    {
        m_dwPackageSize = 2;
        m_eOpcode = otPDUPrompt;
    }

    if( dwPackageSize >= 4
        && !memcmp( pPackageBuffer, "RING", 4 ) )
    {
        m_dwPackageSize = 4;
        m_eOpcode = otRing;
    }

    if( dwPackageSize >= 4
        && !memcmp( pPackageBuffer, "BUSY", 4 ) )
    {
        m_dwPackageSize = 4;
        m_eOpcode = otRing;
    }

    if( dwPackageSize >= 3
        && !memcmp( pPackageBuffer, "+++", 3 ) )
    {
        m_dwPackageSize = 3;
        m_eOpcode = otRing;
    }

    if( dwPackageSize >= 10
        && !memcmp( pPackageBuffer, "NO CARRIER", 10 ) )
    {
        m_dwPackageSize = 10;
        m_eOpcode = otRing;
    }

    if( dwPackageSize >= 9
        && !memcmp( pPackageBuffer, "NO ANSWER", 9 ) )
    {
        m_dwPackageSize = 9;
        m_eOpcode = otRing;
    }
    if( dwPackageSize >= 11
        && !memcmp( pPackageBuffer, "NO DIALTONE", 11 ) )
    {
        m_dwPackageSize = 11;
        m_eOpcode = otRing;
    }

    if( m_eOpcode == otNone && dwPackageSize >= 5
        && !memcmp( pPackageBuffer, "ERROR", 5 ) )
    {
        m_dwPackageSize = 5;
        m_eOpcode = otError;
    }

    if( m_eOpcode == otNone && pPackageBuffer[0] == 0x0d )
    {
        m_dwPackageSize = 1;
        m_eOpcode = otSingleCarriageReturn;
    }

    if( m_eOpcode == otNone && pPackageBuffer[0] == 0x0a )
    {
        m_dwPackageSize = 1;
        m_eOpcode = otSingleLineFeed;
    }


    if( m_eOpcode == otNone && dwPackageSize >= 10
        && !memcmp( pPackageBuffer, "+CME ERROR", 10 ) )
    {
        const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
        if( pc != NULL )
        {
            m_eOpcode = otCMEError;
            m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);
        }
    }
    char szCMSError[] = {0x0d, 0x0a, 0x0d, 0x0a, '+', 'C', 'M', 'S', ' ', 'E', 'R', 'R', 'O', 'R'};
    if( (m_eOpcode == otNone || m_eOpcode == otSingleCarriageReturn) && dwPackageSize >= 10
        && (!memcmp( pPackageBuffer, "+CMS ERROR", 10) || !memcmp( pPackageBuffer, szCMSError, 14)))
    {
        const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
        if( pc != NULL )
        {
            m_eOpcode = otCMSError;
            m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);
        }
    }

    if( m_eOpcode == otNone && dwPackageSize >= 8
        && !memcmp( pPackageBuffer, "$FSERROR", 8 ) )
    {
        const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\nERROR\r\n", 8 );
        if( pc != NULL )
        {
            m_eOpcode = otFSError;
            m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 8);
        }
    }

    if( m_eOpcode == otNone && dwPackageSize >= 11
        && !memcmp( pPackageBuffer, "$BQVCARDERR", 11 ) )
    {
        const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
        if( pc != NULL )
        {
            m_eOpcode = otVCardError;
            m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);
        }
    }

    //================= Steven CC Chen added codes ==================BEGIN//
    if( m_eOpcode == otNone && dwPackageSize >= 10
        && !memcmp( pPackageBuffer, "$BQVCALERR", 10 ) )
    {
        const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
        if( pc != NULL )
        {
            m_eOpcode = otVCalendarError;
            m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);
        }
    }
    //================= Steven CC Chen added codes ==================END//

    if( m_eOpcode == otNone && dwPackageSize >= 4 )
    {
        if( !memcmp( pPackageBuffer, "AT", 2 ) )
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                m_eOpcode = otEchoOfCommand;
            }
        }

        else if( memcmp( pPackageBuffer, "+CIEV:", 6 ) == 0 || memcmp( pPackageBuffer, "^SREM:", 6 ) == 0 ) // to check if unsolicited event arrival 
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                m_eOpcode = otUnsolicitedEvent;
            }

        }

        else if( memcmp( pPackageBuffer, "+CREG:", 6 ) == 0 ) // verify if it is an unsolicited event or result
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
            if( pc != NULL )
            {
                DWORD dwPartialPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                pc = findbuffer( pPackageBuffer, dwPartialPackageSize, ",", 1 );
                if( pc == NULL )
                {
                    m_dwPackageSize = dwPartialPackageSize;

                    m_eOpcode = otUnsolicitedEvent;
                }
            }

        }
        else if( memcmp( pPackageBuffer, "^SHFR:", 6 ) == 0 ) // verify if it is an unsolicited event or result
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2 );
            if( pc != NULL )
            {
                DWORD dwPartialPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                pc = findbuffer( pPackageBuffer, dwPartialPackageSize, ",", 1 );
                if( pc == NULL )
                {
                    m_dwPackageSize = dwPartialPackageSize;

                    m_eOpcode = otUnsolicitedEvent;
                }
            }

        }
        else if( memcmp( pPackageBuffer, "+CMTI:", 6) == 0) 
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2);            
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                m_eOpcode = otUnsolicitedEvent;
            }   
        }
        else if( memcmp( pPackageBuffer, "$FSDIR", 6) == 0) 
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "OK\r\n", 4);            
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 4);

                m_eOpcode = otDiscard;
            }   
        }
        else if( memcmp( pPackageBuffer, "$FSRF", 5) == 0) 
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2);            
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                m_eOpcode = otResult;
            }   
        }
        else if( memcmp( pPackageBuffer, "$MGQS", 5) == 0) 
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\r\n", 2);            
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 2);

                m_eOpcode = otResult;
            }   
        }

        if( m_eOpcode == otNone )
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "OK\r\n", 4 );
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer + 4);
                m_eOpcode = otResult;
            }
        }

        if( m_eOpcode == otNone && dwPackageSize >= 12)
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\n+CME ERROR:", 12 );
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer);
                m_eOpcode = otDiscard;
            }
        }

        if( m_eOpcode == otNone && dwPackageSize >= 12)
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\n+CMS ERROR:", 12 );
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer);
                m_eOpcode = otDiscard;
            }
        }

        if( m_eOpcode == otNone && dwPackageSize >= 8)
        {
            const char* pc = findbuffer( pPackageBuffer, dwPackageSize, "\nERROR\r\n", 8 );
            if( pc != NULL )
            {
                m_dwPackageSize = (DWORD)(pc - pPackageBuffer);
                m_eOpcode = otDiscard;
            }
        }

    }

    if( m_eOpcode == otNone )
    {
        return false;
    }
    else
    {
        m_eActiveOpcode = eActiveOpcode;
        m_spPackageBuffer = spBuffer;
        return true;
    }
}

bool CAtCodec::DecodeResult()
{
    USES_CONVERSION;


    if( m_eOpcode != otError && m_eOpcode != otFSError && m_eOpcode != otVCardError && m_eOpcode != otVCalendarError && m_eOpcode != otCMEError && m_eOpcode != otCMSError && m_eOpcode != otResult && m_eOpcode != otUnsolicitedEvent && m_eOpcode != otPDUPrompt )
        return false; // shouldn't occur

    //    LPCTSTR pcf = _T("CAtCodec::DecodeResult()");
    //    Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: eActiveOpcode=%s, dwPackageSize=%ld, pPackageBuffer=<%s>"),
    //        pcf, opcode2t(m_eActiveOpcode), m_dwPackageSize, spbuffer2t2(m_spPackageBuffer,m_dwPackageSize) );

    bool bSuccess = true;
    int dw = 0;
    int nResult  = 0;   

    char* pBuffer = new char [m_dwPackageSize+1];
    memcpy( pBuffer, m_spPackageBuffer->GetData(), m_dwPackageSize );
    pBuffer[m_dwPackageSize] = 0;

    switch( m_eOpcode )
    {
    default:
        bSuccess = false;
        break;
    case otError:
    case otCMEError:
    case otFSError:
    case otCMSError:
        nResult = ParseError(pBuffer, nResult);
        break;
        //================= Steven CC Chen added codes ==================BEGIN//
    case otVCardError:
        ParseVCardError(pBuffer);
        break;
    case otVCalendarError:
        ParseVCalendarError(pBuffer);
        break;
        //================= Steven CC Chen added codes ==================END//
    case otUnsolicitedEvent:
        ParseUnsolicitedEvent(pBuffer, bSuccess, dw);
        break;

    case otPDUPrompt:
        bSuccess = ParsePDUPrompt(bSuccess);
        break;

    case otResult:
        if (m_eActiveOpcode != otReqListMessagesTi && m_eActiveOpcode != otReqGetFile && m_eActiveOpcode != otReqQueryMessageSpace)
        {
            pBuffer[m_dwPackageSize-4] = 0;

            // Remove trailing cr's and lf's
            for( dw = m_dwPackageSize-5; dw >= 0; dw-- )
            {
                if( pBuffer[dw] == 0x0d || pBuffer[dw] == 0x0a )
                    pBuffer[dw] = 0;
                else
                    break;
            }
        }

        switch( m_eActiveOpcode )
        {
        case otRequest:
            m_eOpcode = otResOk;
            break;
        case otReqGetPhoneType:
            m_eOpcode = otResGetPhoneType;
            m_strPhoneType = pBuffer;
            break;

        case otReqGetManufacturer:
            m_eOpcode = otResGetManufacturer;
            m_strManufacturer = pBuffer;
            break;
        case otReqGetIMEI:
            ParseGetIMEI(pBuffer);
            break;
        case otReqGetDeviceInfoBBClient:
            //m_eOpcode = otResGetDeviceInfoBBClient;
            //m_strDeviceInfoBBClient = ExtractParString( pBuffer, "^SGDI:" );
            break;
        case otReqGetSoftwareVersion:
            m_eOpcode = otResGetSoftwareVersion;
            m_strSoftwareVersion = pBuffer;
            break;
        case otReqGetSignalQuality:
            break;
        case otReqGetBatteryCharge:
            break;
        case otReqGetVolumes:
            break;
        case otReqGetEntries:
            {
                int nResult = 0;
                m_strDirEntries = CString(pBuffer);
            }
            break;
        case otReqGetCapacity:
            break;
        case otReqGetFile:
            ParseGetFile(pBuffer);
            break;

            //PhoneBook
        case otReqGetPBRangesAndLengthsStd:
            ParseGetPBRangesAndLengthsStd(pBuffer);
            break;
        case otReqReadPhonebookEntriesStd:
            ParseReadPhonebookEntriesStd(pBuffer);
            break;
        case otReqGetSupportedPhonebooksStd:
            nResult = ParseGetSupportedPhonebooksStd(pBuffer, nResult);
            break;

            // Message
        case otReqWriteMessageToMemoryTi:
        case otReqWriteMessageToMemoryStd:
            break;
        case otReqDirectSendMessage:
            // received <greater_than> <space>
            break;
        case otReqPDUDataDirectSend:
            DecodeDirectSendMessage(pBuffer);
            break;
        case otReqPDUDataWriteTi:
            DecodeWriteMessageToMemoryTi(pBuffer);
            break;
        case otReqPDUDataWriteStd:
            DecodeWriteMessageToMemoryStd(pBuffer);
            break;
        case otReqSendStoredMessageTi:
            DecodeSendStoredMessageTi(pBuffer);            
            break;
        case otReqSendStoredMessageStd:
            DecodeSendStoredMessageStd(pBuffer);            
            break;
        case otReqGetServiceCenterAddress:
            DecodeServiceCenterAddress(pBuffer, nResult);
            break;
        case otReqReadMessageTi:
            DecodeReadMessageTi(pBuffer, m_dwPackageSize);
            break;
        case otReqReadMessageStd:
            DecodeReadMessageStd(pBuffer, m_dwPackageSize);
            break;
        case otReqListMessagesTi:
            DecodeListMessagesTi(pBuffer, m_dwPackageSize);
            break;
        case otReqListMessagesStd:
            DecodeListMessagesStd(pBuffer, m_dwPackageSize);
            break;
        case otReqPreferredMessageStorage:
            DecodePreferredStorage(pBuffer);         
            break;
        case otReqQueryMessageSpace:
            DecodeQueryMessageSpace(pBuffer);
            break;
        case otReqQueryFolderNew:
            DecodeListFolder(pBuffer);
            break;
        case otReqReadMsgNew:
            DecodeReadMessageNew(pBuffer, m_dwPackageSize);
            break;
        case otReqListMsgIDNew:
            DecodeListMessagesNew(pBuffer, m_dwPackageSize);
            break;
        case otReqPDUDataWriteNew:
            DecodeWriteMessageNewResult(pBuffer);
            break;
        case otReqUpdateMessageState:
            DecodeUpdateMessageState(pBuffer);
            break;
            //================= Steven CC Chen added codes ==================BEGIN//
            // AddressBook related AT Command Responses
        case otReqGetAddrRangesAndCounts:
            break;
        case otReqReadPhonebookEntry:
            ParseReadPhonebookEntry(pBuffer);
            break;
        case otReqReadAddressbookEntry:
            break;
        case otReqWriteAddressbookEntryData:
            break;
        case otReqGetDeleteSupportedStorageList:
            break;
        case otReqGetListSupportedStorageList:
            break;
        case otReqListUsedPhonebookEntries:
            ParseListUsedPhonebookEntries(pBuffer);
            break;
        case otReqReadCallergroupSetting:
            ParseReadCallergroupSetting(pBuffer, nResult);
            break;
        case otReqReadCalendarEntry:
            ParseReadCalendarEntry(pBuffer);
            break;
        case otReqWriteCalendarEntryData:
            ParseWriteCalendarEntryData(pBuffer);
            break;
            //================= Steven CC Chen added codes ==================END//

        default:
            //            Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Unexpected: %s"), pcf, A2T(pBuffer) );
            m_eOpcode = otError;
            break;

        } //end of switch (m_eActiveOpcode)
        break;
    }

    if( pBuffer )
        delete pBuffer;
    //    Traceit( TRACEIT_LEAVE, TG_CAtCodec, _T("%s: bSuccess=%s, description=<%s>"), pcf, bool2t(bSuccess), (LPCTSTR)GetTraceString() );
    return bSuccess;
}

void CAtCodec::ParseGetIMEI(char* pBuffer)
{
    char szIMEI[64], szIMEISV[64];
    int nResult = 0;
    nResult = sscanf( pBuffer, "IMEI: %s\r\nIMEISV: %s", szIMEI, szIMEISV );
    m_eOpcode = otResGetIMEI;
    if( nResult == 2 )
    {
        m_strIMEI = CString(szIMEI);
        m_strIMEISV = CString(szIMEISV);
    }
    else
        m_strIMEI = pBuffer;
}

void CAtCodec::ParseUnsolicitedEvent(char* pBuffer, bool& bSuccess, int& dw)
{
    pBuffer[m_dwPackageSize-2] = 0;

    // Remove trailing cr's and lf's
    for( dw = m_dwPackageSize-3; dw >= 0; dw-- )
    {
        if( pBuffer[dw] == 0x0d || pBuffer[dw] == 0x0a )
            pBuffer[dw] = 0;
        else
            break;
    }

    DWORD dwValue = 0;
    DWORD dwId = 0;
    m_eOpcode = GetUnsolicitedEvent( pBuffer, dwValue, dwId );
    //switch(m_eOpcode)
    //{
    //case otEventNetworkRegistrationChanged:
    //    NetworkRegistration_t enuNetworkStatus;
    //    if(ConvertOrdinalToNetworkStatus(dwValue, enuNetworkStatus))
    //        m_enuNetworkStatus = enuNetworkStatus;
    //    else
    //    {
    //        bSuccess = false;
    //        //                        Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid value for NetworkStatus in unsolicited event: %s"), pcf, A2T(pBuffer) );
    //    }
    //    break;
    //case otEventBatteryChargeChanged:
    //    m_lBatteryCharge = dwValue * 20;
    //    break;
    //case otEventSignalQualityChanged:
    //    m_lSignalQuality = dwValue * 20;
    //    break;
    //default:

    bSuccess = false;
    //    Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid unsolicited event: %s"), pcf, A2T(pBuffer) );
    //    break;
    //}

}

int CAtCodec::ParseError(char* pBuffer, int nResult)
{
    m_eCMEExtendedError = errCME_None;
    m_eCMSExtendedError = errCMS_None;

    std::string szBuffer = pBuffer;
    if(szBuffer.find("+CME ERROR:", 0) != std::string::npos)
    {
        nResult = sscanf(pBuffer, "+CME ERROR: %d", &m_eCMEExtendedError);
    }
    else if(szBuffer.find("+CMS ERROR:", 0) != std::string::npos)
    {
        nResult = sscanf(pBuffer, "+CMS ERROR: %d", &m_eCMSExtendedError);
    }
    else
    {
        m_eCMEExtendedError = errCME_Unknown;
        if( m_dwPackageSize > 8 )
        {
            LPCSTR pc = findbuffer(pBuffer, m_dwPackageSize, "$FSERROR", 8);
            if( pc != NULL )
            {
                m_eFSError = errFS_None;
                nResult = sscanf(pc, "$FSERROR: %d", &m_eFSError);
                m_eOpcode = otFSError;
            }
        }
    }
    //nResult = sscanf(pBuffer, "+CME ERROR: %d", &m_eCMEExtendedError);
    //if( nResult == 0 )
    //{
    //    nResult = sscanf(pBuffer, "+CMS ERROR: %d", &m_eCMSExtendedError);
    //    if( nResult == 0)
    //    {
    //        m_eCMEExtendedError = errCME_Unknown;

    //        if( m_dwPackageSize > 8 )
    //        {
    //            LPCSTR pc = findbuffer(pBuffer, m_dwPackageSize, "$FSERROR", 8);
    //            if( pc != NULL )
    //            {
    //                m_eFSError = errFS_None;
    //                nResult = sscanf(pc, "$FSERROR: %d", &m_eFSError);
    //                m_eOpcode = otFSError;
    //            }
    //        }
    //    }
    //}
    return nResult;
}


CString CAtCodec::GetToken(char * pTxt, char StartDelimiter, char EndDelimiter)
{
    if(pTxt == 0)
        return _T("");

    char szBuffer[128];
    int i=0;

    // skip to the StartDelimiter
    while(*pTxt && *pTxt++ != StartDelimiter);

    memset(szBuffer, 0x00, sizeof(szBuffer));

    // get info
    while(*pTxt && *pTxt != EndDelimiter)
    {
        if(i < sizeof(szBuffer) )
            szBuffer[i++] = *pTxt;
        pTxt++;
    }

    CString cstrResult;

    cstrResult.Format(_T("%s"), szBuffer);

    return cstrResult;
}


//CString CAtCodec::ExtractParString( LPCSTR szBuffer, LPCSTR szPrefix )
//{
//    CString strResult;
//    DWORD dwPrefixLen = strlen(szPrefix);
//    const char* pc = NULL; // Points to first '"'
//    const char* pc2 = NULL; // Points to second '"'
//    char* szResult = NULL;
//
//    if( !strncmp(szBuffer,szPrefix,dwPrefixLen) )
//    {
//        pc = strchr( szBuffer + dwPrefixLen, '\"' );
//        if( pc != NULL )
//            pc2 = strchr( pc+1, '\"' );
//
//        if( pc && pc2 )
//        {
//            szResult = new char [pc2-pc];
//            if( pc2-pc > 1 )
//                memcpy( szResult, pc+1, pc2-pc-1 );
//            szResult[pc2-pc-1] = 0;
//            strResult = szResult; // This converts to unicode in unicode compiles
//            delete szResult;
//        }
//    }
//
//    return strResult;
//}


CAtCodec::Opcode_t CAtCodec::GetUnsolicitedEvent( LPCSTR szBuffer, DWORD& dwValue, DWORD& dwId )
{
    LPCTSTR pcf = _T("CAtCodec::GetUnsolicitedEvent");
    //    Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: szBuffer=<%s>"), pcf, (LPCTSTR)CString(szBuffer) );

    Opcode_t enuOpcode = otNone;
    int nValue = 0;

    if( memcmp(szBuffer, "+CIEV:", 6) == 0 )
    {
        int nIndicator = 0;
        int nResult = sscanf( szBuffer, "+CIEV: %d,%d", &nIndicator, &nValue );

        if( nResult == 2 )
        {
            switch( nIndicator )
            {
            case 1: // Battery Charge
                {
                    if( (nValue >= 0) && (nValue <= 5) )
                    {
                        enuOpcode = otEventBatteryChargeChanged;
                        dwValue = nValue;
                    }
                }
                break;

            case 2: // Signal Quality
                {
                    if( (nValue >= 0) && (nValue <= 5) )
                    {
                        enuOpcode = otEventSignalQualityChanged;
                        dwValue = nValue;
                    }
                }
                break;

            case 8: // Call Status TODO if needed
                {
                }
                break;
            }
        }
    }
    else if( memcmp(szBuffer, "+CREG:", 6) == 0 )
    {
        // Network registration event
        int nResult = sscanf( szBuffer, "+CREG: %d", &nValue );

        if( nResult == 1 )
        {
            enuOpcode = otEventNetworkRegistrationChanged;
            dwValue = nValue;
        }
    }

    //    Traceit( TRACEIT_LEAVE, TG_CAtCodec, _T("%s: Opcode=%s, dwValue=%d"), pcf, opcode2t(enuOpcode), dwValue );

    return enuOpcode;
}


/// Conversion methods, mbueno 19.01.05
int CAtCodec::HexStr2Bin (unsigned char *pBin, DWORD dwNum, 
                          unsigned char *pHexStr, DWORD dwSize)
{
    DWORD nIndex, nIndex2;
    char szByte[3];

    if ((dwSize % 2) != 0) 
        return -1;

    if ((dwSize / 2) > dwNum) 
        return -1;

    nIndex2 = 0; szByte[2]=0;
    for (nIndex=0; nIndex < dwSize; nIndex+=2) 
    {
        szByte[0] = pHexStr[nIndex];
        szByte[1] = pHexStr[nIndex+1];

        if (sscanf(szByte, "%02X", (unsigned int *)&pBin[nIndex2++]) != 1) 
            return -1;
    }

    DWORD nChange;
    for(nChange=0; nChange < nIndex2 ; nChange+=2)
    {
        char cTemp = pBin[nChange];

        pBin[nChange] = pBin[nChange+1];

        pBin[nChange+1] = cTemp;

    }

    return nIndex;
}


int CAtCodec::Bin2HexStr(char *pHexStr, DWORD dwSize, unsigned char *pBin, 
                         DWORD dwNum)
{
    DWORD nIndex;
    char szByte[3];
    char *p=pHexStr;

    if (dwNum > (dwSize / 2)) 
        return -1;

    for (nIndex = 0; nIndex < dwNum; nIndex++) 
    {
        if (_snprintf(szByte, 3, "%02X", pBin[nIndex]) != 2) 
            return -1;
        memcpy(p, szByte, 2);
        p+=2;
    }

    p-=(2*nIndex);

    DWORD nChange;
    for(nChange=0; nChange < (dwNum*2) ; nChange+=4)
    {

        char cByte1 = p[nChange];
        char cByte2 = p[nChange+1];

        p[nChange]   = p[nChange+2];
        p[nChange+1] = p[nChange+3];

        p[nChange+2] = cByte1;
        p[nChange+3] = cByte2;

    }

    return nIndex;
}


//bool CAtCodec::ConvertOrdinalToNetworkStatus( long lOrdinal, NetworkRegistration_t &enuNetworkStatus )
//{
//    bool bConverted = true;
//    switch(lOrdinal) 
//    {
//    case 0          : enuNetworkStatus = nrNotCheckedIn_NotSeeking; break;
//    case 1          : enuNetworkStatus = nrCheckedIn;               break;
//    case 2          : enuNetworkStatus = nrNotCheckedIn_Seeking;    break;
//    case 3          : enuNetworkStatus = nrCheckInDenied;           break;
//    case 4          : enuNetworkStatus = nrUnknown;                 break;
//    case 5          : enuNetworkStatus = nrRegistered;              break;
//    default         : bConverted = false;
//    }
//    return bConverted;
//}

bool CAtCodec::CutBeginningCrlf(CString &str)
{
    bool bIsCutOK = true;
    int nCounter = 0;

    while (((str.GetAt(0) == '\r') || (str.GetAt(0) == '\n')) && (nCounter < 5))
    {
        str = str.Right(str.GetLength() - 1);

        nCounter++;
    }

    if (nCounter == 5)
        return false;

    return true;
}

HRESULT CAtCodec::NextToken(const char* szFirstDelimiter, char* szFirstGrid,
                            const char* szSecondDelimiter, char* szSecondGrid,
                            char* pSource, DWORD dwSourceLen,
                            DWORD* pStartPos, DWORD* pEndPos)
{

    HRESULT hRes = S_OK;               

    char* c = pSource;
    *pStartPos = 0;
    *pEndPos = dwSourceLen;

    if (szFirstDelimiter != 0)
    {
        bool bStartDetected = false;
        DWORD dwRemaining = dwSourceLen;
        int nDelimL = (int)strlen(szFirstDelimiter);
        int nPos = 0;

        while((c!=0) && (dwRemaining>(DWORD)nDelimL))
        {
            strncpy(szFirstGrid, c, nDelimL);
            bStartDetected = (strcmp(szFirstGrid, szFirstDelimiter)==0);


            if(bStartDetected)
            {
                c += nDelimL;
                dwRemaining -= nDelimL;
                *pStartPos = nPos+nDelimL;
                break;
            }
            else
            {
                c++;
                dwRemaining--;
            }

            nPos++;
        }

        if(!bStartDetected)
        {
            return E_FAIL;
        }
    }        

    if( szSecondDelimiter !=0 )
    {
        bool bStartDetected = false;
        DWORD dwRemaining = dwSourceLen;
        int nDelimL = (int)strlen(szSecondDelimiter);
        int nPos = 0;

        while((c!=0) && (dwRemaining>(DWORD)nDelimL))
        {
            strncpy(szSecondGrid, c, nDelimL);
            bStartDetected = (strcmp(szSecondGrid, szSecondDelimiter)==0);


            if(bStartDetected)
            {                
                *pEndPos = (*pStartPos)+nPos;
                break;
            }
            else
            {
                c++;
                dwRemaining--;
            }

            nPos++;
        }

        if(!bStartDetected)
        {
            return E_FAIL;
        }
    }

    return S_OK;
}

//================= Steven CC Chen added codes ==================END//



//================= Steven CC Chen added codes ==================BEGIN//
/*-------------------------------------------------------------------*/
/*!
This function extracts each token from the given formatted token list.
[Token List format]:
("Token1","Token2","Token3"...)

@param      CString in_strTokenList      - The source string that contains the designated formatted token list to be parsed
@param      CStringList& out_strlstToken - The reference to the destination string list for storing the parsed tokens
@return     bool                         - The boolean flag indicating the result of parsing is succesful or failure

@author     Steven CC Chen
@date       2006/06/09

// [example]:
// CString strTokenList = _T(" (\"token1\",\"token2 \", \" token3\",\" token4 \",\"  token5\" ,\"\" )");
// CStringList strlstToken;
// if( ExtractTokenFromList(strTokenList, strlstToken) )
// {
//		for( POSITION pos = strlstToken.GetHeadPosition(); pos != NULL; strlstToken.GetNext(pos) )
//		{
//			AfxMessageBox( strlstToken.GetAt(pos) );
//		}
//	}else;
*/
//bool CAtCodec::ExtractTokenFromList(CString in_strTokenList, CStringList& out_strlstToken)
//{
//    CString strTokenListBeginTag("(");
//    CString strTokenListEndTag(")");
//    CString strTokenSegmenter("\"");
//    CString strTokenSeparator(",");
//
//    int nTempIndexOfFirstCharacter = -1, nTempIndexOfFirstCharacter2 = -1;
//    CString strTemp;
//    CString strTokenTemp;
//    bool bIsTokenFound = false;
//
//
//
//    //(To retrieve each token and add to the designated array variable iteratorly:)
//    if( !in_strTokenList.IsEmpty() )
//    {
//        nTempIndexOfFirstCharacter = in_strTokenList.Find(strTokenListBeginTag);
//        if(nTempIndexOfFirstCharacter != -1)
//        {
//            nTempIndexOfFirstCharacter2 = in_strTokenList.Find(strTokenListEndTag, (nTempIndexOfFirstCharacter + strTokenListBeginTag.GetLength()));
//            if(nTempIndexOfFirstCharacter2 != -1)
//            {
//                strTokenTemp = in_strTokenList.Mid( (nTempIndexOfFirstCharacter + strTokenListBeginTag.GetLength()), (nTempIndexOfFirstCharacter2 - (nTempIndexOfFirstCharacter + strTokenListBeginTag.GetLength())) );
//                while( !strTokenTemp.IsEmpty() )
//                {
//                    CString strToken("");
//
//                    nTempIndexOfFirstCharacter = strTokenTemp.Find(strTokenSegmenter);
//                    if(nTempIndexOfFirstCharacter != -1)
//                    {
//                        strTokenTemp.Delete(0, (nTempIndexOfFirstCharacter + strTokenSegmenter.GetLength()));  //(trimmed for the later parsing.)
//
//                        int nTempStartIndex = 0;
//                        while( !strTokenTemp.IsEmpty() )
//                        {
//                            nTempIndexOfFirstCharacter = strTokenTemp.Find(strTokenSegmenter, nTempStartIndex);
//                            if(nTempIndexOfFirstCharacter != -1)
//                            {
//                                bIsTokenFound = true;
//                                break;
//                            }else;
//                        }
//
//                        if( bIsTokenFound )
//                        {
//                            strToken = strTokenTemp.Mid(0, nTempIndexOfFirstCharacter);
//                            strTokenTemp.Delete(0, (nTempIndexOfFirstCharacter + strTokenSegmenter.GetLength()));  //(trimmed for the later parsing.)
//
//                            out_strlstToken.AddTail(strToken);
//                        }else;
//                    }
//                    else
//                    {
//                        //(end the parsing:)
//                        strTokenTemp.Empty();
//                        break;
//                    }
//                }
//            }else;
//        }else;
//    }else;
//
//
//    return bIsTokenFound;
//}


/*-------------------------------------------------------------------*/
/*!
This function extracts each parameter from the given separator-specified (e.g. comma-separated) parameter list.
[Parameter List format]:
Param1,Param2,Param3...

@param      CString in_strParameterList       - The source string that contains the designated separator-specified parameter list to be parsed
@param      CString in_strParameterSeparator  - The character(s) used as the separator between each parameter
@param      CStringList& out_strlstParameter  - The reference to the destination string list for storing the parsed parameters
@param      bool in_bIsEmptyAllowed           - The boolean flag used to indicate whether a parameter with empty content is allowed to be retrieved or not
@return     bool                              - The boolean flag indicating the result of parsing is succesful or failure

@author     Steven CC Chen
@date       2006/06/12

// [example]:
// CString strParameterList = _T("token1,token2 , token3,, , token6 ");  //(Empty allowed.)
// CString strParameterSeparator(",");
// CStringList strlstParameter;
// if( this->ExtractParameterFromList(strParameterList, strParameterSeparator, strlstParameter, true) )
// {
//		for( POSITION pos = strlstParameter.GetHeadPosition(); pos != NULL; strlstParameter.GetNext(pos) )
//		{
//			AfxMessageBox( strlstParameter.GetAt(pos) );
//		}
//	}else;
*/
bool CAtCodec::ExtractParameterFromList(CString in_strParameterList, CString in_strParameterSeparator, CStringList& out_strlstParameter, bool in_bIsEmptyAllowed)
{
    int nTempIndexOfFirstCharacter = -1;
    bool bIsParameterFound = false;


    //(To retrieve each parameter and add to the designated array variable iteratorly:)
    while( !in_strParameterList.IsEmpty() )
    {
        CString strParameter("");

        nTempIndexOfFirstCharacter = in_strParameterList.Find(in_strParameterSeparator);
        if(nTempIndexOfFirstCharacter != -1)
        {
            bIsParameterFound = true;

            strParameter = in_strParameterList.Mid(0, nTempIndexOfFirstCharacter);
            in_strParameterList.Delete(0, (nTempIndexOfFirstCharacter + in_strParameterSeparator.GetLength()));  //(trimmed for the later parsing.)

            if( in_bIsEmptyAllowed )
            {
                out_strlstParameter.AddTail(strParameter);
            }
            else
            {
                if( !strParameter.IsEmpty() )
                {
                    out_strlstParameter.AddTail(strParameter);
                }
                else
                {
                    ;//(skip the current item.)
                }
            }
        }
        else
        {
            if( bIsParameterFound )  //(*If ever found, then the last part of the source string is still considered as a parameter.)
            {
                strParameter = in_strParameterList;
                in_strParameterList.Empty();

                if( in_bIsEmptyAllowed )
                {
                    out_strlstParameter.AddTail(strParameter);
                }
                else
                {
                    if( !strParameter.IsEmpty() )
                    {
                        out_strlstParameter.AddTail(strParameter);
                    }
                    else
                    {
                        ;//(skip the current item.)
                    }
                }

                break;
            }
            else
            {
                //(end the parsing:)
                out_strlstParameter.AddTail(in_strParameterList);
                in_strParameterList.Empty();
                bIsParameterFound = true;
                break;
            }
        }
    }
    return bIsParameterFound;
}
//================= Steven CC Chen added codes ==================END//

//-----------------------------------------------------------------------------
// This function is used for Creating AT Codec. Because every Create Function
// use the same mechanism to return an AT Codec, remove all the same code into
// this function for CreateAtCodec.
// Param1: Op Code is used for what operation codec it is.
// Param2: Buffer length is used for CBuffer size.
// Param3: The original string for commands.
//-----------------------------------------------------------------------------

CRefObjectPtr<CAtCodec> CAtCodec::CreateAtCodec(Opcode_t opCode, DWORD bufferLength, const char* sz) {
    CAtCodec* codec = new CAtCodec();
    codec->m_eActiveOpcode = opCode;
    codec->m_spPackageBuffer = new CBuffer(bufferLength, sz);
    CRefObjectPtr<CAtCodec> retVal(codec);
    return retVal;
}