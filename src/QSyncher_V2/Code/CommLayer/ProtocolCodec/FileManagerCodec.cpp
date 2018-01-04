//-----------------------------------------------------------------------------
// This file is implementation of AtCodec about file manager interfaces.
// If AtCodec is add a new interface, please implement member function in this
// file.
// !!!DO NOT ADD MEMBER FUNCTIONS IN ATCODEC.CPP file!!!
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "AtCodec.h"

CAtCodecPtr CAtCodec::CreateReqGetFile(LPCTSTR szPath, bool bIsDBB)
{
    USES_CONVERSION;

    size_t nAnsiSize = strlen(T2CA(szPath));
    char* sz = new char[nAnsiSize + 30];
    ::ZeroMemory(sz,nAnsiSize + 30);

    sprintf(sz, "AT$FSRF=%d,0,\"%s\"\r", bIsDBB ? 1 : 4, T2CA(szPath));

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqGetFile;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otReqGetFile, (DWORD)strlen(sz), sz);
    delete [] sz;
    
    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqPutFile(LPCTSTR szPath, BOOL bIsDBB, DWORD dwFileSize)
{
    USES_CONVERSION;

	string strAnsi = QSYNC_LIB::widetoansi(szPath,CP_UTF8);

	size_t nAnsiSize = strAnsi.size();
    char* sz = new char[nAnsiSize + 30];
    ::ZeroMemory(sz,nAnsiSize + 30);
	sprintf(sz, "AT$FSWF=%d,0,0,\"%s\",%d\r", bIsDBB ? 1 : 4, strAnsi.c_str(), dwFileSize);

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqPutFile;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otReqPutFile, (DWORD)strlen(sz), sz);
    delete [] sz;

    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqRemoveFile(LPCTSTR szPath, BOOL bIsDBB)
{
    USES_CONVERSION;

    size_t nAnsiSize = strlen(T2CA(szPath));
    char* sz = new char[nAnsiSize + 30];
    ::ZeroMemory(sz,nAnsiSize + 30);

    sprintf(sz, "AT$FSMISC=1,%d,\"%s\"\r", bIsDBB ? 1 : 4, T2CA(szPath));

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    delete [] sz;

    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqGetEntries(LPCTSTR szPath, BOOL bIsDBB , BOOL bIsContinue)
{
    USES_CONVERSION;

    size_t nAnsiSize = strlen(T2CA(szPath));
    char* sz = new char[nAnsiSize + 40];
    ::ZeroMemory(sz,nAnsiSize + 40);


    sprintf(sz, "AT$FSDIR=%d,\"%s\"", bIsDBB ? 15 : 7, T2CA(szPath));
    if(bIsContinue)
        strcat(sz,",\"Continue\"");
    strcat(sz,"\r");

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otReqGetEntries;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otReqGetEntries, (DWORD)strlen(sz), sz);
    delete [] sz;

    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqCreateDir(LPCTSTR szPath, BOOL bIsDBB)
{
    USES_CONVERSION;

    size_t nAnsiSize = strlen(T2CA(szPath));
    char* sz = new char[nAnsiSize + 30];
    ::ZeroMemory(sz,nAnsiSize + 30);

    sprintf(sz, "AT$FSDIR=%d,\"%s\"\r", bIsDBB ? 0 : 3, T2CA(szPath));

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    delete [] sz;

    return spAtCodec;
}

CAtCodecPtr CAtCodec::CreateReqRemoveDir(LPCTSTR szPath, BOOL bIsDBB)
{
    USES_CONVERSION;

    size_t nAnsiSize = strlen(T2CA(szPath));
    char* sz = new char[nAnsiSize + 30];
    ::ZeroMemory(sz,nAnsiSize + 30);

    sprintf(sz, "AT$FSDIR=%d,\"%s\"\r", bIsDBB ? 1 : 4, T2CA(szPath));

    //CAtCodecPtr spAtCodec = new CAtCodec();
    //spAtCodec->m_eActiveOpcode = otRequest;
    //spAtCodec->m_spPackageBuffer = new CBuffer((DWORD)strlen(sz), sz);
    CAtCodecPtr spAtCodec = CreateAtCodec(otRequest, (DWORD)strlen(sz), sz);
    delete [] sz;

    return spAtCodec;
}

void CAtCodec::ParseGetFile(char* pBuffer)
{
    DWORD dwFileType = 0;
    DWORD dwTransMode = 0;
    char szFileName[MAX_MMP_FILENAME_SIZE + 50];
    DWORD dwFileLength = 0;
    int nResult = 0;
    nResult = sscanf( pBuffer, "$FSRF: %d,%d,\"%[^\"]\",%d", &dwFileType, &dwTransMode, szFileName, &dwFileLength );
    if( nResult == 4 )
    {
        m_dwFileLength = dwFileLength;
        m_eOpcode = otResGetFile;
    }
    else
    {
        m_eOpcode = otError;
        //                    Traceit( TRACEIT_ENTER, TG_CAtCodec, _T("%s: Not a valid $FSRF result: %s"), pcf, A2T(pBuffer) );
    }
}