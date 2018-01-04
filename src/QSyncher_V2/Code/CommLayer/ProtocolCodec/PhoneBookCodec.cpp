//-----------------------------------------------------------------------------
// This file is implementation of AtCodec about phone book interfaces.
// If AtCodec is add a new interface, please implement member function in this
// file.
// !!!DO NOT ADD MEMBER FUNCTIONS IN ATCODEC.CPP file!!!
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "AtCodec.h"

// PhoneBook
CAtCodecPtr CAtCodec::CreateReqGetPBRangesAndLengthsStd()
{         
    const char* sz = "AT+CPBR=?\r";

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqGetPBRangesAndLengthsStd;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otReqGetPBRangesAndLengthsStd, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqWritePhonebookEntryStd( long lIndex, LPCTSTR szName, LPCTSTR szNumber )
{
    long lInternational = _tcsstr(szNumber, _T("+")) == NULL ? 129 : 145;

    //if( _tcsstr(szNumber, _T("+")) == NULL )
    //    lInternational = 129;
    //else
    //    lInternational = 145;

    USES_CONVERSION;
    DWORD dwSize = (DWORD)_tcslen(szName)*4 + 30;
    char *szTempBuffer = new char[ dwSize ];
    char *szBuffer = new char[ dwSize + 128 ];

    ZeroMemory(szTempBuffer, dwSize);
    ZeroMemory(szBuffer, dwSize + 128);

    if (_tcslen(szName))
        Bin2HexStr(szTempBuffer, dwSize, (unsigned char *)T2CW (szName), (DWORD)_tcslen(szName) * 2);

    if (lIndex > 0 ) 
        sprintf(szBuffer, "AT+CPBW=%d,\"%s\",%d,\"%s\"\r", lIndex, T2CA(szNumber), lInternational, szTempBuffer); 
    else
        sprintf(szBuffer, "AT+CPBW=,\"%s\",%d,\"%s\"\r", T2CA(szNumber), lInternational, szTempBuffer); 

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(szBuffer), szBuffer);
    CAtCodecPtr spAtCodec = CreateAtCodec(otRequest, (DWORD)strlen(szBuffer), szBuffer);
    delete [] szTempBuffer;
    delete [] szBuffer;

    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqReadPhonebookEntriesStd( long lStartPos, long lEndPos )
{         
    char sz[30] = {0};
    sprintf(sz, "AT+CPBR=%d,%d\r", lStartPos, lEndPos );

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqReadPhonebookEntriesStd;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otReqReadPhonebookEntriesStd, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqDeletePhonebookEntryStd( long lIndex )
{
    char sz[30] = {0};
    sprintf(sz, "AT+CPBW=%d\r", lIndex);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqGetSupportedPhonebooksStd()
{
    const char* sz = "AT^SPBS=?\r";

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqGetSupportedPhonebooksStd;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz );
    return CreateAtCodec(otReqGetSupportedPhonebooksStd, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqReadPhonebookEntry(enumStorage ePIMStorage, DWORD dwIndex)
{
    char sz[128];

    if (ePIMStorage == STORAGE_SIM)
        sprintf(sz, "AT$BQVCARDR=\"SM\",%d\r", dwIndex);
    else
        sprintf(sz, "AT$BQVCARDR=\"ME\",%d\r", dwIndex);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqReadPhonebookEntry;  //(this AT command needs a specific AT command response rather than just an "OK" back.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otReqReadPhonebookEntry, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqWritePhonebookEntry(enumStorage ePIMStorage, DWORD dwIndex, DWORD dwDataSizeInByte)
{
    char sz[128];

    if (ePIMStorage == STORAGE_SIM)
        sprintf(sz, "AT$BQVCARDW=\"SM\",%d,%d\r", dwIndex, dwDataSizeInByte);
    else
        sprintf(sz, "AT$BQVCARDW=\"ME\",%d,%d\r", dwIndex, dwDataSizeInByte);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqWritePhonebookEntryData(LPCTSTR pszVCard)
{
    USES_CONVERSION;

    char* psz = new char[_tcslen(pszVCard) + 32];
    sprintf(psz, "%s", T2CA(pszVCard));

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(psz), psz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otRequest, (DWORD)strlen(psz), psz);
    delete [] psz;

    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqDeletePhonebookEntry(enumStorage ePIMStorage, enumPIMAction ePIMAction, DWORD dwIndex)
{
    char sz[128];

    if (ePIMStorage == STORAGE_SIM)
    {
        if (ePIMAction == DELETE_ONE)
            sprintf(sz, "AT$BQVCARDD=\"SM\",0,%d\r", dwIndex);
        else
            sprintf(sz, "AT$BQVCARDD=\"SM\",1\r");
    }
    else
    {
        if (ePIMAction == DELETE_ONE)
            sprintf(sz, "AT$BQVCARDD=\"ME\",0,%d\r", dwIndex);
        else
            sprintf(sz, "AT$BQVCARDD=\"ME\",1\r");
    }

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqListUsedPhonebookEntries(enumStorage ePIMStorage)
{
    char sz[128];

    if (ePIMStorage == STORAGE_SIM)
        sprintf(sz, "AT$BQVCARDL=\"SM\"\r");
    else
        sprintf(sz, "AT$BQVCARDL=\"ME\"\r");

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqListUsedPhonebookEntries;  //(this AT command needs a specific AT command response rather than just an "OK" back.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otReqListUsedPhonebookEntries, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqReadCallergroupSetting(DWORD dwCallergroupId)
{
    char sz[128];
    sprintf(sz, "AT$BQCGR=%d\r", dwCallergroupId);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqReadCallergroupSetting;  //(this AT command needs a specific AT command response rather than just an "OK" back.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otReqReadCallergroupSetting, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqWriteCallergroupSetting(CallergroupSetting_t tagCallergroupSetting)
{
    USES_CONVERSION;

    char* psz = new char[strlen(tagCallergroupSetting.szName) + strlen(tagCallergroupSetting.szRingtoneIdPath) + strlen(tagCallergroupSetting.szImageIdPath) + 128];
    sprintf(psz, "AT$BQCGW=%d,\"%s\",%d,\"%s\",%d,\"%s\"\r", tagCallergroupSetting.nID, T2CA((LPCTSTR)tagCallergroupSetting.szName), tagCallergroupSetting.dwRingtoneIdType, T2CA((LPCTSTR)tagCallergroupSetting.szRingtoneIdPath), tagCallergroupSetting.dwImageIdType, T2CA((LPCTSTR)tagCallergroupSetting.szImageIdPath));

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(psz), psz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otRequest, (DWORD)strlen(psz), psz);
    delete [] psz;

    return spAtCodec;
}

void CAtCodec::ParseReadCallergroupSetting(char* pBuffer, int nResult)
{
    //(*ps. The trailing "\r\n...\r\nOK\r\n" in the response data have been removed at the beginning of the "case otResult" code block already.)

    //(for temporarily storing the parsed parameter respectively:)
    DWORD dwCallergroupId, dwRingtoneIdType, dwImageIdType;
    char szCallergroupName[256], szRingtoneIdPath[1024], szImageIdPath[1024];
    ZeroMemory(szCallergroupName, sizeof(szCallergroupName));
    ZeroMemory(szRingtoneIdPath, sizeof(szRingtoneIdPath));
    ZeroMemory(szImageIdPath, sizeof(szImageIdPath));

    nResult = sscanf(pBuffer, "$BQCGR:%d,\"%[^\"]\",%d,\"%[^\"]\",%d,\"%[^\"]\"", &dwCallergroupId, szCallergroupName, &dwRingtoneIdType, szRingtoneIdPath, &dwImageIdType, szImageIdPath);

    if(nResult >= 2)
    {
        m_eOpcode = otReqReadCallergroupSetting;

        //(to store all the info of the current read CallerGroup setting:)
        this->m_tagCurrentReadCallergroup.nID = dwCallergroupId;
        strcpy(this->m_tagCurrentReadCallergroup.szName, szCallergroupName);
        this->m_tagCurrentReadCallergroup.dwRingtoneIdType = dwRingtoneIdType;
        strcpy(this->m_tagCurrentReadCallergroup.szRingtoneIdPath, szRingtoneIdPath);
        this->m_tagCurrentReadCallergroup.dwImageIdType = dwImageIdType;
        strcpy(this->m_tagCurrentReadCallergroup.szImageIdPath, szImageIdPath);
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid <AT$BQCGR=idx> response, or an error occur: %s"), pcf, A2T(pBuffer));
    }
}

void CAtCodec::ParseListUsedPhonebookEntries(char* pBuffer)
{
    USES_CONVERSION;
    //(*ps. The trailing "\r\n...\r\nOK\r\n" in the response data have been removed at the beginning of the "case otResult" code block already.)

    char* pUsedIndexList = new char[m_dwPackageSize+1];  //(for temporarily storing the received used Phonebook entry list.)
    char* pUsedStorage = new char[3];
    ZeroMemory(pUsedIndexList, m_dwPackageSize);
    ZeroMemory(pUsedStorage, 3);
    pUsedIndexList[m_dwPackageSize] = 0;
    pUsedStorage[2] = 0;
    int nResult = 0;

    nResult = sscanf(pBuffer, "$BQVCARDL:%s,%s", pUsedStorage, pUsedIndexList);
    if(nResult == 1)
    {
        //(To extract each item from the list:)
        if( this->ExtractParameterFromList(A2CT(pUsedIndexList),CString(","),this->m_strlstUsedPhonebookEntryIndexes,false) )
        {
            m_eOpcode = otReqListUsedPhonebookEntries;
        }
        else
        {
            m_eOpcode = otError;
            //						Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: No any index found in the <AT$BQVCARDL=\"SM\"> response: %s"), pcf, A2T(pBuffer));

            //(to reset the member variable that stores these information:)
            if( !this->m_strlstUsedPhonebookEntryIndexes.IsEmpty() )
            {
                this->m_strlstUsedPhonebookEntryIndexes.RemoveAll();
            }else;
        }
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid <AT$BQVCARDL=\"SM\"> response, or an error occurs: %s"), pcf, A2T(pBuffer));
    }
    delete [] pUsedIndexList;
    delete [] pUsedStorage;
}

void CAtCodec::ParseReadPhonebookEntry(char* pBuffer)
{
    USES_CONVERSION;
    //(*ps. The trailing "\r\n...\r\nOK\r\n" in the response data have been removed at the beginning of the "case otResult" code block already.)

    DWORD n1, n2 = 0;  //(for temporarily storing the parsed parameter respectively.)
    int nResult = 0, sep1 = 0, sep2 = 0;

    nResult = sscanf(pBuffer, "$BQVCARDR:%d,%d\r\n", &n1, &n2);
    if(nResult == 2)
    {
        this->m_dwCurrentReadPhonebookEntryIndex = n1;  //(to store the index of the current read Phonebook entry.)

        if( (n2 > 0 && n2 <= m_dwPackageSize) )
        {
            CString strBuffer( A2T(pBuffer) );
            sep1 = strBuffer.Find(_T("\r\nBEGIN:VCARD")) + 2;
            sep2 = strBuffer.Find(_T("\r\nEND:VCARD")) + 11;
            if( sep1>0 && ((sep2-sep1)==n2-2) ) //2 means \r\n
            {
                this->m_strBenQVCard = strBuffer.Mid(sep1,sep2-sep1) + _T("\r\n"); //the last \r\n had been extract and need to add it
                m_eOpcode = otReqReadPhonebookEntry;
                this->m_bIsCurrentReadVCardEmpty = false;
            }
            else
            {
                m_eOpcode = otError;
                //							Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: vCard Format Error: %s"), pcf, A2T(pBuffer));
                this->m_bIsCurrentReadVCardEmpty = true;
                this->m_strBenQVCard = "";  //(to reset the vCard content to be empty due to invalid or not-supported (too big) vCard size.)
            }
        }
        else if( n2 == 0 )
        {
            m_eOpcode = otReqReadPhonebookEntry;

            this->m_bIsCurrentReadVCardEmpty = true;
            this->m_strBenQVCard = "";
        }
        else
        {
            m_eOpcode = otError;
            //						Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not an acceptable-sized vCard: %s"), pcf, A2T(pBuffer));
            this->m_bIsCurrentReadVCardEmpty = true;
            this->m_strBenQVCard = "";  //(to reset the vCard content to be empty due to invalid or not-supported (too big) vCard size.)
        }
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid <AT$BQVCARDR=\"SM\",idx> response, or an error occurs: %s"), pcf, A2T(pBuffer));
    }
}

int CAtCodec::ParseGetSupportedPhonebooksStd(char* pBuffer, int nResult)
{
    char szSupportedPhonebooks[200] = "";

    nResult = sscanf( pBuffer, "^SPBS: (%[^)])", szSupportedPhonebooks);
    if( nResult == 1 )
    {
        m_eOpcode = otResGetSupportedPhonebooksStd;
        m_strSupportedPhonebooks = szSupportedPhonebooks;
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid result: %s"), pcf, A2T(pBuffer) );
    }
    return nResult;
}
void CAtCodec::ParseReadPhonebookEntriesStd(char* pBuffer)
{
    char *pLine = pBuffer;

    m_pPBEntries = new CPBEntries();

    m_eOpcode = otResReadPhonebookEntriesStd;

    long lIndex = 0;
    char szNumber[50] = "";
    DWORD dwType = 0;
    unsigned char szContactName[1024] = "";

    int nResult = 0;

    nResult = sscanf( pLine, "+CPBR: %d,\"%[^\"]\",%d,\"%[^\"]\"", &lIndex, szNumber, &dwType, szContactName );

    while( nResult > 0 )
    {
        if( nResult >= 3 )
        {
            USES_CONVERSION;
            wchar_t szBinName[512]=L"";
            nResult = HexStr2Bin((unsigned char *)szBinName,sizeof(szBinName),szContactName,(DWORD)strlen((char *)szContactName));
            if  (nResult != -1)
            {
                m_pPBEntries->Insert(lIndex, W2CT(szBinName), A2CT(szNumber));
            }

        }
        else
        {
            //						Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a read phonebook entries result: %s"), pcf, A2T(pLine) );
        }

        pLine = strstr(pLine, "\x0a");
        if(pLine != NULL)
        {
            strcpy((char*)szContactName, "");
            strcpy(szNumber, "");
            pLine++;
            nResult = sscanf( pLine, "+CPBR: %d,\"%[^\"]\",%d,\"%[^\"]\"", &lIndex, szNumber, &dwType, szContactName );
        }
        else
            nResult = 0;

    }
}

void CAtCodec::ParseGetPBRangesAndLengthsStd(char* pBuffer)
{
    long  nIndexRangeMin = 0;
    long  nIndexRangeMax = 0;
    long  nAllowedLengthNumber = 0;
    long  nAllowedLengthName   = 0;
    int nResult  = 0;   
    nResult = sscanf( pBuffer, "+CPBR: (%d-%d),%d,%d", &nIndexRangeMin,
        &nIndexRangeMax, &nAllowedLengthNumber, &nAllowedLengthName);
    if( nResult == 4 )
    {
        m_eOpcode = otResGetPBRangesAndLengthsStd;

        m_nIndexRangeMin       = nIndexRangeMin;
        m_nIndexRangeMax       = nIndexRangeMax;
        m_nAllowedLengthNumber = nAllowedLengthNumber;
        m_nAllowedLengthName   = nAllowedLengthName;
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid result: %s"), pcf, A2T(pBuffer) );
    }
}

void CAtCodec::ParseVCardError(char* pBuffer)
{
    //int nErrcode = 0;
    long nErrcode = 0;
    sscanf(pBuffer, "$BQVCARDERR:%d", &nErrcode);
    //m_eOpcode = otReqListUsedPhonebookEntries;
    if(nErrcode == 2)
    {
        m_eOpcode = m_eActiveOpcode;
        this->m_strlstUsedPhonebookEntryIndexes.RemoveAll();
    }
    else
    {
        m_eOpcode = otError;
    }
}