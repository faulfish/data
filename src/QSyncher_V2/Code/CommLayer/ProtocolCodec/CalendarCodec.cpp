//-----------------------------------------------------------------------------
// This file is implementation of AtCodec about calendar interfaces.
// If AtCodec is add a new interface, please implement member function in this
// file.
// !!!DO NOT ADD MEMBER FUNCTIONS IN ATCODEC.CPP file!!!
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "AtCodec.h"

CAtCodecPtr CAtCodec::CreateReqReadCalendarEntry(DWORD dwIndex)
{
    char sz[128];
    sprintf(sz, "AT$BQVCALR=%d\r", dwIndex);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqReadCalendarEntry;  //(this AT command needs a specific AT command response rather than just an "OK" back.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otReqReadCalendarEntry, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqWriteCalendarEntry(DWORD dwIndex, DWORD dwDataSizeInByte)
{
    char sz[128];
    sprintf(sz, "AT$BQVCALW=%d,%d\r", dwIndex, dwDataSizeInByte);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqWriteCalendarEntryData(LPCTSTR pszVCalendar, DWORD dwDataSizeInByte)
{
    //USES_CONVERSION;

    //char* psz = new char[_tcslen(pszVCalendar) + 32];
    //sprintf(psz, "%s", T2CA(pszVCalendar));

    //CAtCodecPtr spAtCodec = new CAtCodec();

    //spAtCodec->m_eActiveOpcode = otReqWriteCalendarEntryData;  //(this AT command needs a specific AT command response rather than just an "OK" back.)
    ////spAtCodec->m_spPackageBuffer = new CBuffer(strlen(psz), psz);
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)dwDataSizeInByte, (const char*)pszVCalendar);

    //delete [] psz;
    return CreateAtCodec(otRequest, (DWORD)dwDataSizeInByte, (const char*)pszVCalendar);
    //return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqDeleteCalendarEntry(enumPIMAction ePIMAction, DWORD dwIndex)
{
    char sz[128];

    if (ePIMAction == DELETE_ONE)
        sprintf(sz, "AT$BQVCALD=0,%d\r", dwIndex);
    else
        sprintf(sz, "AT$BQVCALD=1\r");

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    return CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    //return spAtCodec;
}

/*-------------------------------------------------------------------*/
/*!
This method creates the AT command for querying the supported actions by which Calendar entries can be deleted.

@return     CAtCodecPtr                 - The smart pointer to the CAtCodec class that generates this AT command

@author     Steven CC Chen
@date       2006/07/19
*/
/*CAtCodecPtr CAtCodec::CreateReqGetCalDeleteSupportedActions()
{
const char* psz = "AT$BQVCALD=?\r";

CAtCodecPtr spAtCodec = new CAtCodec();
spAtCodec->m_eActiveOpcode = otReqGetCalDeleteSupportedActions;  //(this AT command needs a specific AT command response rather than just an "OK" back.)
spAtCodec->m_spPackageBuffer = new CBuffer(strlen(psz), psz);

return spAtCodec;
}
*/

/*-------------------------------------------------------------------*/
/*!
This method creates the AT command for deleting all the existed entries in Calendar.

@return     CAtCodecPtr                 - The smart pointer to the CAtCodec class that generates this AT command

@author     Steven CC Chen
@date       2006/07/19
*/
/*CAtCodecPtr CAtCodec::CreateReqDeleteCalendarAllEntries(DWORD dwActionIdDeleteAll)
{
char sz[128];
sprintf(sz, "AT$BQVCALD=%d\r", dwActionIdDeleteAll);

CAtCodecPtr spAtCodec = new CAtCodec();
spAtCodec->m_eActiveOpcode = otRequest;  //(this AT command needs just a general "OK" response.)
spAtCodec->m_spPackageBuffer = new CBuffer(strlen(sz), sz);

return spAtCodec;
}
*/

void CAtCodec::ParseReadCalendarEntry(char* pBuffer)
{
    USES_CONVERSION;
    //(*ps. The trailing "\r\n...\r\nOK\r\n" in the response data have been removed at the beginning of the "case otResult" code block already.)

    DWORD n1, n2 = 0;  //(for temporarily storing the parsed parameter respectively.)
    int nResult = 0, sep1 = 0, sep2 = 0;

    nResult = sscanf(pBuffer, "$BQVCALR:%d,%d\r\n", &n1, &n2);
    if(nResult == 2)
    {
        this->m_dwCurrentReadCalendarEntryIndex = n1;  //(to store the index of the current read Calendar entry.)

        if( (n2 > 0 && n2 <= m_dwPackageSize) )
        {
            CString strBuffer( A2T(pBuffer) );
            sep1 = strBuffer.Find(_T("\r\nBEGIN:VCALENDAR")) + 2;
            sep2 = strBuffer.Find(_T("\r\nEND:VCALENDAR")) + 15;
            if ( sep1>0 && ((sep2-sep1)==n2-2) ) //2 means \r\n
            {
                m_eOpcode = otReqReadCalendarEntry;
                this->m_bIsCurrentReadVCalendarEmpty = false;

                //to store the vCalendar raw data of the current read Calendar entry:
                this->m_strBenQVCalendar = strBuffer.Mid(sep1,sep2-sep1) + _T("\r\n"); //the last \r\n had been extract and need to add it
            }
            else
            {
                m_eOpcode = otError;
                //							Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: vCalendar Format Error: %s"), pcf, A2T(pBuffer));
                this->m_bIsCurrentReadVCalendarEmpty = true;
                this->m_strBenQVCalendar = "";  //(to reset the vCard content to be empty due to invalid or not-supported (too big) vCard size.)
            }
        }
        else
        {
            m_eOpcode = otError;
            //						Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not an acceptable-sized vCalendar: %s"), pcf, A2T(pBuffer));
            this->m_bIsCurrentReadVCalendarEmpty = true;
            this->m_strBenQVCalendar = "";  //(to reset the vCalendar content to be empty due to invalid or not-supported (too big) vCalendar size.)
        }
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid <AT$BQVCALR=,idx> response, or an error occurs: %s"), pcf, A2T(pBuffer));
    }
}

void CAtCodec::ParseWriteCalendarEntryData(char* pBuffer)
{
    //(*ps. The trailing "\r\n...\r\nOK\r\n" in the response data have been removed at the beginning of the "case otResult" code block already.)

    DWORD nIndex;  //(for temporarily storing the parsed parameters.)
    int nResult = 0;

    nResult = sscanf(pBuffer, "$BQVCALW:%d", &nIndex);
    if(nResult == 1)
    {
        m_eOpcode = otReqWriteCalendarEntryData;

        this->m_dwCurrentWriteCalendarEntryIndex = nIndex;  //(to store the (destination) index of the current written Calendar entry.)
    }
    else
    {
        m_eOpcode = otError;
        //					Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid <AT$BQVCALW> Data Writing response, or an error occurs: %s"), pcf, A2T(pBuffer));
    }
}

void CAtCodec::ParseVCalendarError(char* pBuffer)
{
    //int nErrcode = 0;
    long nErrcode = 0;
    sscanf(pBuffer, "$BQVCALERR:%d", &nErrcode);
    //m_eOpcode = otReqReadCalendarEntry;
    if(nErrcode == 1)
    {
        m_eOpcode = m_eActiveOpcode;
        this->m_bIsCurrentReadVCalendarEmpty = true;
        this->m_strBenQVCalendar = "";
    }
    else
    {
        m_eOpcode = otError;
    }
}