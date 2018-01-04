// ObexCodec.cpp: implementation of the CObexCodec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObexCodec.h"
#include <cassert>

#pragma warning(disable:4267)
#pragma warning(disable:4996)

#define MaxPackageLength 4096

void copyunicode( WCHAR* out, WCHAR* in )
{
    for( ; *in; in++,out++ )
    {
        *out = MAKEWORD(HIBYTE(*in),LOBYTE(*in));
    }

    *out = 0;
}

void ncopyunicode( WCHAR* out, WCHAR* in, size_t n )
{
    for( ; n > 0; in++,out++,n-- )
    {
        *out = MAKEWORD(HIBYTE(*in),LOBYTE(*in));
    }
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObexCodec::CObexCodec( Opcode_t enuOpcode )
{
    Reset();

    m_eOpcode = enuOpcode;
}

CObexCodec::~CObexCodec()
{
}

void CObexCodec::Reset()
{
    m_dwPackageSize = 0;
    m_eOpcode = oUnknown;
    m_spBodyBuffer = NULL;
    m_dwConnId = 0;
    m_hrFailureCause = S_OK;
    m_dwFileSize = 0;
    m_dwMaxObexPackageSize = 0;

    m_mapApplicationParameters.clear();
}


LPCTSTR CObexCodec::opcode2t( Opcode_t eOpcode )
{
    switch( eOpcode )
    {
    case oUnknown                           : return _T("oUnknown");
    case oreqPut                            : return _T("oreqPut");
    case oreqGet                            : return _T("oreqGet");
    case oreqConnect                        : return _T("oreqConnect");
    case oreqGetFolderListing               : return _T("oreqGetFolderListing");
    case oreqSetPath                        : return _T("oreqSetPath");
    case oreqGetCapacity                    : return _T("oreqGetCapacity");
    case oreqAbort                          : return _T("oreqAbort");
    case oresSuccess                        : return _T("oresSuccess");
    case oresContinue                       : return _T("oresContinue");
    case oresGet                            : return _T("oresGet");
    case oresGetContinue                    : return _T("oresGetContinue");
    case oresSuccessConnect                 : return _T("oresSuccessConnect");
    case oresGetCapacity                    : return _T("oresGetCapacity");
    case oresFailure                        : return _T("oresFailure");
    case oreqIrmcPutFile                    : return _T("oreqIrmcPutFile");
    case oresIrmcPutFile                    : return _T("oresIrmcPutFile");
    case oreqFormat                         : return _T("oreqFormat");
    case oresFormat                         : return _T("oresFormat");
    case oreqGetRecursiveFolderSize         : return _T("oreqGetRecursiveFolderSize");
    case oreqGetMaximumFilenameLength       : return _T("oreqGetMaximumFilenameLength");
    default                                 : return _T("*** unknown ***");
    }
}

BYTE Opcode2Binary( CObexCodec::Opcode_t eOpcode, bool bFinal )
{
    BYTE cOpcode = 0;

    switch( eOpcode )
    {
    case CObexCodec::oreqPut:
        cOpcode = 0x02;
        break;
    case CObexCodec::oresSuccess:
        cOpcode = 0x20;
        break;
    }

    if( bFinal )
        cOpcode |= 0x80;

    return cOpcode;
}

bool IsValidOpcode( CObexCodec::Opcode_t eOpcode, bool bFinal )
{
    switch( eOpcode )
    {
    case CObexCodec::oreqPut:
    case CObexCodec::oresSuccess:
        return true;
    default:
        return false;
    }
}


// Encoding functions

CBufferPtr CObexCodec::CreateRequestPut( CBufferPtr spBuffer )
{
    DWORD dwContentSize = spBuffer ? spBuffer->GetLength() : 0;     // Size of the content to put
    DWORD dwPackageSize = dwContentSize + 6;                        // Size of the complete OBEX package
    LPCSTR pSourceData = spBuffer ? spBuffer->GetData() : "";

    CBufferPtr spPackageBuffer = new CBuffer;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );

    pPackageData[0] = (char)0x82;                       // Final 0x80 + Put 0x02
    pPackageData[1] = HIBYTE((WORD)dwPackageSize);
    pPackageData[2] = LOBYTE((WORD)dwPackageSize);

    pPackageData[3] = (char)0x49;                       // EndOfBody
    pPackageData[4] = HIBYTE((WORD)(dwContentSize+3));  // the +3 because of length and tag prefix
    pPackageData[5] = LOBYTE((WORD)(dwContentSize+3));  // dto

    if( dwContentSize > 0 )
        memcpy( pPackageData + 6, pSourceData, dwContentSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXConnect(std::string strTarget)
{
	CBufferPtr ptTarget = new CBuffer(strTarget.size(),strTarget.c_str());
	return CreateReqOBEXConnect(ptTarget);
}

CBufferPtr CObexCodec::CreateReqOBEXConnect(CBufferPtr ptTarget)
{	
    CBufferPtr spPackageBuffer = new CBuffer;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( MaxPackageLength );	
	DWORD dw = 0;

    pPackageData[dw++] = (char)0x80;                       // CONNECT
    pPackageData[dw++] = HIBYTE((WORD)0);
    pPackageData[dw++] = LOBYTE((WORD)0);
    pPackageData[dw++] = (char)0x10;                       // OBEX version 1.0
    pPackageData[dw++] = (char)0x00;                       // Flags, reserved, must be 0

	DWORD dwMaxPackageSize = 0x4006;
    pPackageData[dw++] = HIBYTE((WORD)dwMaxPackageSize);   // Max size of OBEX packages
    pPackageData[dw++] = LOBYTE((WORD)dwMaxPackageSize);
	
	DWORD dwTargetSize = ptTarget->GetLength();
	if(dwTargetSize > 0)
	{		
		pPackageData[dw++] = (char)0x46;                       // Target header
		pPackageData[dw++] = HIBYTE((WORD)dwTargetSize+3);
		pPackageData[dw++] = LOBYTE((WORD)dwTargetSize+3);
		memcpy( pPackageData+10, ptTarget->GetData(), dwTargetSize );
		dw += dwTargetSize;
	}		

	pPackageData[1] = HIBYTE((WORD)dw);
    pPackageData[2] = LOBYTE((WORD)dw);
    spPackageBuffer->ReleaseBuffer(dw);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXConnectSyncML()
{
	unsigned char pcTarget[] = {'S','Y','N','C','M','L','-','S','Y','N','C'};
	CBufferPtr spBuffer = new CBuffer(sizeof(pcTarget),(const char *)pcTarget);
	return CreateReqOBEXConnect(spBuffer);
}

CBufferPtr CObexCodec::CreateReqOBEXConnectFileSystem()
{
	unsigned char pcTarget[] = { 0x6b, 0x01, 0xcb, 0x31, 0x41, 0x06, 0x11, 0xd4, 0x9a, 0x77, 0x00, 0x50, 0xda, 0x3f, 0x47, 0x1f };
	CBufferPtr spBuffer = new CBuffer(sizeof(pcTarget),(const char *)pcTarget);
	return CreateReqOBEXConnect(spBuffer);
}

CBufferPtr CObexCodec::CreateReqOBEXConnectIrmc()
{
	unsigned char pcTarget[] = {'I','R','M','C','-','S','Y','N','C'};
	CBufferPtr spBuffer = new CBuffer(sizeof(pcTarget),(const char *)pcTarget);
	return CreateReqOBEXConnect(spBuffer);
}

CBufferPtr CObexCodec::CreateReqOBEXDisconnect( DWORD dwOBEXConnId )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( MaxPackageLength );
	DWORD dw = 0;

    pPackageData[dw++] = (char)0x81;                       // Disconnect
    pPackageData[dw++] = (char)0x00;
    pPackageData[dw++] = (char)0x00;

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
	pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

	pPackageData[1] = HIBYTE((WORD)dw);
    pPackageData[2] = LOBYTE((WORD)dw);
    spPackageBuffer->ReleaseBuffer(dw);
    return spPackageBuffer;
}


CBufferPtr CObexCodec::CreateReqOBEXGetFileFirst( DWORD dwOBEXConnId, LPCTSTR szFilename )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwNameSize = 2 * _tcslen(szFilename) + 2;          // 2(Unicode)*strlen + 2(Unicode term0)
    DWORD dwPackageSize = 3 + 5 + 3 + dwNameSize;               // 3(Opcode+PkgLen) + 5(connid) + 3(NameTag+NameLen) + swNameSize
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    CComBSTR bstrFilename = szFilename;
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x83;                       // GET
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
    pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));


    pPackageData[dw++] = (char)0x01;                       // Name header
    pPackageData[dw++] = HIBYTE((WORD)dwNameSize+3);
    pPackageData[dw++] = LOBYTE((WORD)dwNameSize+3);

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)bstrFilename, dwNameSize/2 );
    dw += dwNameSize;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXGetFileNext( DWORD dwOBEXConnId )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwPackageSize = 3 + 5;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
	DWORD dw = 0;

    pPackageData[dw++] = (char)0x83;                       // GET
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);
	
    pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
    pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXSetPath( DWORD dwOBEXConnId, LPCTSTR szPath,bool bCreateDir )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwNameSize = szPath ? (2 * _tcslen(szPath) + 2) : 0;             // 2(Unicode)*strlen + 2(Unicode term0)
	DWORD dwNameHeaderSize = 3;
    DWORD dwPackageSize = 3 + 2 + dwNameHeaderSize + dwNameSize;           // 3(Opcode+PkgLen) + 2(Flags+Constants) + 3(NameTag+NameLen) + swNameSize
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    CComBSTR bstrPath = szPath;
	DWORD dw = 0;

    pPackageData[dw++] = (char)0x85;                       // SetPath
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);
    if( bCreateDir )
        pPackageData[dw++] = (char)0x00;                   // Flags (bit0(not set):..\, bit1(not set):create non-existent folders)
    else
        pPackageData[dw++] = (char)0x02;                   // Flags (bit0(not set):..\, bit1(set):don't create non-existent folders)
    pPackageData[dw++] = (char)0x00;                       // Constants (reserved, must be 0)

	if(dwNameHeaderSize > 0)
	{
		pPackageData[dw++] = (char)0x01;                       // Name header
		pPackageData[dw++] = HIBYTE((WORD)dwNameSize+3);
		pPackageData[dw++] = LOBYTE((WORD)dwNameSize+3);
	}

    if( szPath != NULL )
        ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)bstrPath, dwNameSize/2 );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXGet( DWORD dwOBEXConnId, std::string strType )
{
    CBufferPtr spPackageBuffer = new CBuffer;
	LPCSTR szType = strType.c_str();
    DWORD dwTypeSize = strlen(szType) + 1;              // +1: include terminating 0
	DWORD dwTypeSizeHeader = dwTypeSize > 1 ? 3 : 0;
    DWORD dwPackageSize = 3 + 5 + dwTypeSizeHeader + dwTypeSize;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
	DWORD dw = 0;

    pPackageData[dw++] = (char)0x83;                       // GET
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
    pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

	if(dwTypeSizeHeader)
	{
		pPackageData[dw++] = (char)0x42;                       // Type header
		pPackageData[dw++] = HIBYTE((WORD)dwTypeSize+3);
		pPackageData[dw++] = LOBYTE((WORD)dwTypeSize+3);
		memcpy( pPackageData+dw, szType, dwTypeSize );
	}

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXGetFolderListing( DWORD dwOBEXConnId )
{
	return CreateReqOBEXGet(dwOBEXConnId,"x-obex/folder-listing");
}

CBufferPtr CObexCodec::CreateReqOBEXAbort( DWORD dwOBEXConnId )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwPackageSize = 8;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
	DWORD dw = 0;

    pPackageData[dw++] = (char)0xFF;                       // Abort
    pPackageData[dw++] = (char)0x00;
    pPackageData[dw++] = (char)0x08;

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
    pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));
	
    dwPackageSize = dw;
	pPackageData[1] = HIBYTE((WORD)dwPackageSize);
    pPackageData[2] = LOBYTE((WORD)dwPackageSize);

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXPutFileFirst( DWORD dwOBEXConnId, LPCTSTR szFilename, DWORD dwFileSize,
    CBufferPtr spBuffer, bool bLastPackage, DWORD dwMaxPackageLength, DWORD& dwDataSize )
{
	CBufferPtr spPackageBuffer = new CBuffer;
	LPSTR pPackageData = spPackageBuffer->GetBufferSetLength(dwMaxPackageLength);
    DWORD dwPackageSize = 0 ;    
    DWORD dw = 0;

    pPackageData[dw++] = bLastPackage ? (char)0x82 : (char)0x02;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
	pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

	DWORD dwNameSize = 2 * _tcslen(szFilename) + 2;          // 2(Unicode)*strlen + 2(Unicode term0)
    pPackageData[dw++] = (char)0x01;                       // Name header
    pPackageData[dw++] = HIBYTE((WORD)dwNameSize+3);
    pPackageData[dw++] = LOBYTE((WORD)dwNameSize+3);
    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szFilename), dwNameSize/2 );
    dw += dwNameSize;

    pPackageData[dw++] = (char) 0xC3;                   // Size header
    pPackageData[dw++] = HIBYTE(HIWORD(dwFileSize));
    pPackageData[dw++] = LOBYTE(HIWORD(dwFileSize));
    pPackageData[dw++] = HIBYTE(LOWORD(dwFileSize));
    pPackageData[dw++] = LOBYTE(LOWORD(dwFileSize));

	DWORD dwTimeSize = strlen("20040101T000000");
    pPackageData[dw++] = (char) 0x44;                   // Time header (creation time)
    pPackageData[dw++] = HIBYTE(LOWORD(dwTimeSize+3));
    pPackageData[dw++] = LOBYTE(LOWORD(dwTimeSize+3));
    memcpy( pPackageData+dw, "20040101T000000", dwTimeSize);
    dw += dwTimeSize;

	dwDataSize = 0;
	if( spBuffer->GetLength() + dw + 3 <= dwMaxPackageLength )
    {
        dwDataSize = spBuffer->GetLength();
    }
    else if( dwMaxPackageLength > dw )
    {
        dwDataSize = dwMaxPackageLength - (dw + 3);
    }
    else
    {
        assert(0);// Should not occur
        dwDataSize = spBuffer->GetLength();
    }

	pPackageData[dw++] = bLastPackage ? (char)0x49 : (char)0x48;    // EndOfBody resp Body header    
	pPackageData[dw++] = HIBYTE(LOWORD(dwDataSize+3));
    pPackageData[dw++] = LOBYTE(LOWORD(dwDataSize+3));
    memcpy( pPackageData+dw, spBuffer->GetData(), dwDataSize );
    dw += dwDataSize;

    dwPackageSize = dw;
	pPackageData[1] = HIBYTE((WORD)dwPackageSize);
    pPackageData[2] = LOBYTE((WORD)dwPackageSize);

	assert(dwMaxPackageLength >= dwPackageSize);
    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXPutFileNext( DWORD dwOBEXConnId, CBufferPtr spBuffer, bool bLastPackage,
    DWORD dwMaxPackageLength, DWORD& dwDataSize )
{
	CBufferPtr spPackageBuffer = new CBuffer;
	LPSTR pPackageData = spPackageBuffer->GetBufferSetLength(dwMaxPackageLength);
    DWORD dwPackageSize = 0 ;    
    DWORD dw = 0;

    pPackageData[dw++] = bLastPackage ? (char)0x82 : (char)0x02;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
	pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

	dwDataSize = 0;
	if( spBuffer->GetLength() + dw + 3 <= dwMaxPackageLength )
    {
        dwDataSize = spBuffer->GetLength();
    }
    else if( dwMaxPackageLength > dw )
    {
        dwDataSize = dwMaxPackageLength - (dw + 3);
    }
    else
    {
        assert(0);// Should not occur
        dwDataSize = spBuffer->GetLength();
    }

    pPackageData[dw++] = bLastPackage ? (char)0x49 : (char)0x48;    // EndOfBody resp Body header
    pPackageData[dw++] = HIBYTE(LOWORD(dwDataSize+3));
    pPackageData[dw++] = LOBYTE(LOWORD(dwDataSize+3));
    memcpy( pPackageData+dw, spBuffer->GetData(), dwDataSize );
    dw += dwDataSize;

    dwPackageSize = dw;
	pPackageData[1] = HIBYTE((WORD)dwPackageSize);
    pPackageData[2] = LOBYTE((WORD)dwPackageSize);

	assert(dwMaxPackageLength >= dwPackageSize);
    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXSetAttributes( DWORD dwOBEXConnId, LPCTSTR szFilename, bool bHidden,
    bool bReadonly, bool bSystem, bool bArchiv, bool bDirectory)
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwFilenameSize = 2 * _tcslen(szFilename);
    char szAttrib[100] = "";
    sprintf( szAttrib, "\"%s%s%s\"\"%s%s\"",
        bHidden ? "" : "R",
        bReadonly ? "" : "W",
        bSystem ? "" : "D",
        bArchiv ? "R" : "",
        bDirectory ? "W" : "" );
    DWORD dwAttribSize = strlen(szAttrib);
    DWORD dwPackageSize = 3 + 3 + dwFilenameSize + 3 + 2 + dwAttribSize;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x82;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0x01;                       // Name header
    pPackageData[dw++] = HIBYTE((WORD)dwFilenameSize+3);
    pPackageData[dw++] = LOBYTE((WORD)dwFilenameSize+3);

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szFilename), dwFilenameSize/2 );
    dw += dwFilenameSize;

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(3+2+dwAttribSize));
    pPackageData[dw++] = LOBYTE(LOWORD(3+2+dwAttribSize));

    pPackageData[dw++] = (char)0x38;                                // '8': SetAttributes tag
    pPackageData[dw++] = (char)dwAttribSize;                        // Length of value

    memcpy( pPackageData+dw, szAttrib, dwAttribSize );
    dw += dwAttribSize;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXMoveFile( DWORD dwOBEXConnId, LPCTSTR szFilename, LPCTSTR szNewFilename )
{
	CBufferPtr spPackageBuffer = new CBuffer;
	LPSTR pPackageData = spPackageBuffer->GetBufferSetLength(1024);
    DWORD dwPackageSize = 0 ;    
    DWORD dw = 0;

    DWORD dwFilenameSize = 2 * _tcslen(szFilename);
    DWORD dwNewFilenameSize = 2 * _tcslen(szNewFilename);
    DWORD dwApplSize = 3 + (2 + 4) + (2 + dwFilenameSize) + (2 + dwNewFilenameSize);

    pPackageData[dw++] = (char)0x82;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
    pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(dwApplSize));
    pPackageData[dw++] = LOBYTE(LOWORD(dwApplSize));

    pPackageData[dw++] = (char)0x34;                                // '4': what-to-do header
    pPackageData[dw++] = (char)0x04;                                // Length of value

    memcpy( pPackageData+dw, "move", 4 );
    dw += 4;

    pPackageData[dw++] = (char)0x35;                                // '5': source file name
    pPackageData[dw++] = (char)dwFilenameSize;

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szFilename), dwFilenameSize/2 );
    dw += dwFilenameSize;

    pPackageData[dw++] = (char)0x36;                                // '6': target file name
    pPackageData[dw++] = (char)dwNewFilenameSize;

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szNewFilename), dwNewFilenameSize/2 );
    dw += dwNewFilenameSize;

    dwPackageSize = dw;
	pPackageData[1] = HIBYTE((WORD)dw);
    pPackageData[2] = LOBYTE((WORD)dw);

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXCopy( DWORD dwOBEXConnId, LPCTSTR szFilename, LPCTSTR szNewFilename )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwFilenameSize = 2 * _tcslen(szFilename) + 2;
    DWORD dwNewFilenameSize = 2 * _tcslen(szNewFilename) + 2;
    DWORD dwApplSize = 3 + (2 + 4) + (2 + dwFilenameSize) + (2 + dwNewFilenameSize);
    DWORD dwPackageSize = 3 + dwApplSize;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x82;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(dwApplSize));
    pPackageData[dw++] = LOBYTE(LOWORD(dwApplSize));

    pPackageData[dw++] = (char)0x34;                                // '4': what-to-do header
    pPackageData[dw++] = (char)0x04;                                // Length of value

    memcpy( pPackageData+dw, "copy", 4 );
    dw += 4;

    pPackageData[dw++] = (char)0x35;                                // '5': source file name
    pPackageData[dw++] = (char)dwFilenameSize;

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szFilename), dwFilenameSize/2 );
    dw += dwFilenameSize;

    pPackageData[dw++] = (char)0x36;                                // '6': target file name
    pPackageData[dw++] = (char)dwNewFilenameSize;

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szNewFilename), dwNewFilenameSize/2 );
    dw += dwNewFilenameSize;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}


CBufferPtr CObexCodec::CreateReqOBEXDeleteFile( DWORD dwOBEXConnId, LPCTSTR szFilename )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwFilenameSize = 2 * _tcslen(szFilename) + 2;
    DWORD dwPackageSize = 3 + 5 + 3 + dwFilenameSize;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x82;                        // PUT
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
	pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

    pPackageData[dw++] = (char)0x01;                        // Name header
    pPackageData[dw++] = HIBYTE((WORD)dwFilenameSize+3);
    pPackageData[dw++] = LOBYTE((WORD)dwFilenameSize+3);

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szFilename), dwFilenameSize/2 );
    dw += dwFilenameSize;

    // No body header means: delete

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXCreateStdDirs( DWORD dwOBEXConnId, LPCTSTR szDrive )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwLength4 = 2 * _tcslen(szDrive) + 2;
    DWORD dwLength3 = dwLength4 + 1;
    DWORD dwPackageSize = 3 + 5 +3 + 2 + dwLength3;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

	// Put	Length1	App-Header	Length2	CreateStdDir	Length3	Length4 data
	// 82	00 00	4c			00 00	33				00		00		.... ... ... ... 
	// |-------------------- lenght of header = 9 --------------|       |--- length of data is variable --...--|
	//               |------- lenght of parameter = 6 ----------|
	//
	// Sample protocol: 
	// 82 00 19 4c 00 16 33 11 10 00 5c 00 43 00 6f 00 6e 00 66 00 69 00 67 00 00 // data: \config
    // at this moment length3 was defined by Martin Maurer as "one + length4"

    pPackageData[dw++] = (char)0x82;                                // PUT final
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;								// ConnectionId (HeaderTyp c0: 4-byte quantity)
	pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(3+2+dwLength3));
    pPackageData[dw++] = LOBYTE(LOWORD(3+2+dwLength3));

    pPackageData[dw++] = (char)0x33;                                // '3': CreateStdDirs tag
    pPackageData[dw++] = (char)dwLength3;                           // Length of value

    pPackageData[dw++] = (char)dwLength4;                           // Additional length field
    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szDrive), dwLength4/2 );
    dw += dwLength4;
	
    dwPackageSize = dw;
	pPackageData[1] = HIBYTE((WORD)dwPackageSize);
    pPackageData[2] = LOBYTE((WORD)dwPackageSize);

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqOBEXGetCapacity( DWORD dwOBEXConnId, LPCTSTR szDrive, bool bFreeSpace )
{	
	CBufferPtr spPackageBuffer = new CBuffer;
	LPSTR pPackageData = spPackageBuffer->GetBufferSetLength(MaxPackageLength);
    DWORD dwPackageSize = 0 ;    
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x83;                                // GET final
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

	pPackageData[dw++] = (char)0xcb;								// ConnectionId (HeaderTyp c0: 4-byte quantity)
	pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
	pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
	pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

	DWORD dwFileSize = szDrive ? 2 * _tcslen(szDrive) + 2 : 0;
    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(3+2+2+dwFileSize));
    pPackageData[dw++] = LOBYTE(LOWORD(3+2+2+dwFileSize));
    pPackageData[dw++] = (char)0x32;                                // Capacity tag
    pPackageData[dw++] = (char)LOBYTE(2 + dwFileSize);				// Length of value
    if( bFreeSpace )
        pPackageData[dw++] = (char)0x02;                            // Free space
    else
        pPackageData[dw++] = (char)0x01;                            // Total space	
    pPackageData[dw++] = (char)dwFileSize;                          // Additional length field
	if(NULL != szDrive)
		ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szDrive), dwFileSize/2 );
    dw += dwFileSize;

    dwPackageSize = dw;
	pPackageData[1] = HIBYTE((WORD)dwPackageSize);
    pPackageData[2] = LOBYTE((WORD)dwPackageSize);

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}




CBufferPtr CObexCodec::CreateReqIrmcPutFile(  DWORD dwOBEXConnId, LPCTSTR szFilename, CBufferPtr spBuffer,
    DWORD dwMaxExpectedChangeCounter, DWORD dwMaxObexPackageSize,  DWORD& dwDataSize )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    char szMaxExpectedChangeCounter[20] = "";
    sprintf( szMaxExpectedChangeCounter, "%ld", dwMaxExpectedChangeCounter );
    DWORD dwLengthMaxExpectedChangeCounter = strlen(szMaxExpectedChangeCounter);
    DWORD dwApplSize = 3 + 2 + dwLengthMaxExpectedChangeCounter;
    DWORD dwNameSize = 2 * _tcslen(szFilename) + 2;          // 2(Unicode)*strlen + 2(Unicode term0)
    DWORD dwPackageSize = 3 + 5 + 3 + dwNameSize + dwApplSize;            // 3(Opcode+PkgLen) + 5(connid) + 3(NameTag+NameLen) + swNameSize
    bool bLastPackage = false;

    if( spBuffer ) // spBuffer == NULL means: delete
    {
        if( (spBuffer->GetLength() + dwPackageSize  + 3) <= dwMaxObexPackageSize)
        {
           dwDataSize = spBuffer->GetLength();
           bLastPackage = true;
        }
        else if( dwMaxObexPackageSize > dwPackageSize + 3 )
        {
            dwDataSize = dwMaxObexPackageSize - (dwPackageSize + 3);
        }
        dwPackageSize += dwDataSize + 3;
    }
    else
    {
        bLastPackage = true;
    }

    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    CComBSTR bstrFilename = szFilename;
    DWORD dw = 0;

    pPackageData[dw++] = bLastPackage ? (char)0x82 : (char)0x02;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0xcb;                       // ConnectionId (HeaderTyp c0: 4-byte quantity)
    pPackageData[dw++] = HIBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(HIWORD(dwOBEXConnId));
    pPackageData[dw++] = HIBYTE(LOWORD(dwOBEXConnId));
    pPackageData[dw++] = LOBYTE(LOWORD(dwOBEXConnId));

    pPackageData[dw++] = (char)0x01;                       // Name header
    pPackageData[dw++] = HIBYTE((WORD)dwNameSize+3);
    pPackageData[dw++] = LOBYTE((WORD)dwNameSize+3);

    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)bstrFilename, dwNameSize/2 );
    dw += dwNameSize;

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(dwApplSize));
    pPackageData[dw++] = LOBYTE(LOWORD(dwApplSize));

    pPackageData[dw++] = (char)0x11;                                // Irmc MaxExpectedChangeCounter
    pPackageData[dw++] = (char)dwLengthMaxExpectedChangeCounter;    // Length of value

    memcpy( pPackageData+dw, szMaxExpectedChangeCounter, dwLengthMaxExpectedChangeCounter );
    dw += dwLengthMaxExpectedChangeCounter;

    if( spBuffer )
    {
        pPackageData[dw++] = bLastPackage ? (char)0x49 : (char)0x48;    // EndOfBody resp Body header
        pPackageData[dw++] = HIBYTE(LOWORD(dwDataSize+3));
        pPackageData[dw++] = LOBYTE(LOWORD(dwDataSize+3));

        memcpy( pPackageData+dw, spBuffer->GetData(), dwDataSize );
        dw += dwDataSize;
    }

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CBufferPtr CObexCodec::CreateReqIrmcPutFileNext( DWORD dwOBEXConnId, CBufferPtr spBuffer,
    DWORD dwMaxPackageLength, DWORD& dwDataSize )
{
    CBufferPtr spPackageBuffer = new CBuffer;
    DWORD dwExtraSize = 3 + 3;
    bool bLastPackage = false;
    if( spBuffer->GetLength() + dwExtraSize <= dwMaxPackageLength )
    {
        dwDataSize = spBuffer->GetLength();
        bLastPackage = true;
    }
    else if( dwMaxPackageLength > dwExtraSize )
    {
        dwDataSize = dwMaxPackageLength - dwExtraSize;
    }
    else
    {
        // Should not occur
        dwDataSize = spBuffer->GetLength();
    }

    DWORD dwPackageSize = dwExtraSize + dwDataSize;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = bLastPackage ? (char)0x82 : (char)0x02;
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = bLastPackage ? (char)0x49 : (char)0x48;    // EndOfBody resp Body header
    pPackageData[dw++] = HIBYTE(LOWORD(dwDataSize+3));
    pPackageData[dw++] = LOBYTE(LOWORD(dwDataSize+3));

    memcpy( pPackageData+dw, spBuffer->GetData(), dwDataSize );
    dw += dwDataSize;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);
    return spPackageBuffer;
}

CObexCodecPtr CObexCodec::CreateReqFormat( DWORD dwOBEXConnId, LPCTSTR szDrive )
{
    CObexCodecPtr spObexCodec = new CObexCodec( oreqFormat );
    CBufferPtr spPackageBuffer = new CBuffer;
    spObexCodec->m_spPackageBuffer = spPackageBuffer;

    DWORD dwLength4 = 2 * _tcslen(szDrive) + 2;
    DWORD dwLength3 = 1 + dwLength4;
    DWORD dwLength2 = 3 + 2 + dwLength3;
    DWORD dwPackageSize = 3 + dwLength2;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x82;                                // PUT final
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(dwLength2));
    pPackageData[dw++] = LOBYTE(LOWORD(dwLength2));

    pPackageData[dw++] = (char)0x31;                                // '1': Format tag
    pPackageData[dw++] = (char)dwLength3;                           // Length of value

    pPackageData[dw++] = (char)dwLength4;                           // Length of value
    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szDrive), dwLength4/2 );
    dw += dwLength4;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);

    return spObexCodec;
}

CObexCodecPtr CObexCodec::CreateReqFormatContinue( DWORD dwOBEXConnId )
{
    CObexCodecPtr spObexCodec = new CObexCodec( oreqFormat );
    CBufferPtr spPackageBuffer = new CBuffer;
    spObexCodec->m_spPackageBuffer = spPackageBuffer;

    DWORD dwPackageSize = 3;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x82;                                // PUT final
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);

    return spObexCodec;
}

CObexCodecPtr CObexCodec::CreateReqGetRecursiveFolderSize( DWORD dwOBEXConnId, LPCTSTR szPathname )
{
    CObexCodecPtr spObexCodec = new CObexCodec( oreqGetRecursiveFolderSize );
    CBufferPtr spPackageBuffer = new CBuffer;
    spObexCodec->m_spPackageBuffer = spPackageBuffer;

    DWORD dwLength4 = 2 * _tcslen(szPathname) + 2;
    DWORD dwLength3 = 1 + dwLength4;
    DWORD dwLength2 = 3 + 2 + dwLength3;
    DWORD dwPackageSize = 3 + dwLength2;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x83;                                // GET final
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(dwLength2));
    pPackageData[dw++] = LOBYTE(LOWORD(dwLength2));

    pPackageData[dw++] = (char)0x73;                                // 's': GetRecursiveFolderSize tag
    pPackageData[dw++] = (char)dwLength3;                           // Length of value

    pPackageData[dw++] = (char)dwLength4;                           // Length of value
    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szPathname), dwLength4/2 );
    dw += dwLength4;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);

    return spObexCodec;
}

CObexCodecPtr CObexCodec::CreateReqGetMaximumFilenameLength( DWORD dwOBEXConnId, LPCTSTR szDrive )
{
    CObexCodecPtr spObexCodec = new CObexCodec( oreqGetMaximumFilenameLength );
    CBufferPtr spPackageBuffer = new CBuffer;
    spObexCodec->m_spPackageBuffer = spPackageBuffer;

    DWORD dwLength4 = 2 * _tcslen(szDrive) + 2;
    DWORD dwLength3 = 1 + dwLength4;
    DWORD dwLength2 = 3 + 2 + dwLength3;
    DWORD dwPackageSize = 3 + dwLength2;
    LPSTR pPackageData = spPackageBuffer->GetBufferSetLength( dwPackageSize );
    DWORD dw = 0;

    pPackageData[dw++] = (char)0x83;                                // GET final
    pPackageData[dw++] = HIBYTE((WORD)dwPackageSize);
    pPackageData[dw++] = LOBYTE((WORD)dwPackageSize);

    pPackageData[dw++] = (char)0x4c;                                // Application parameters header
    pPackageData[dw++] = HIBYTE(LOWORD(dwLength2));
    pPackageData[dw++] = LOBYTE(LOWORD(dwLength2));

    pPackageData[dw++] = (char)0x66;                                // 'f': GetMaximumFilenameLength tag
    pPackageData[dw++] = (char)dwLength3;                           // Length of value

    pPackageData[dw++] = (char)dwLength4;                           // Length of value
    ncopyunicode( (WCHAR*)(pPackageData+dw), (BSTR)CComBSTR(szDrive), dwLength4/2 );
    dw += dwLength4;

    ASSERT( dw == dwPackageSize );

    spPackageBuffer->ReleaseBuffer(dwPackageSize);

    return spObexCodec;
}

bool CObexCodec::IsCompletePackage( CBufferPtr spBuffer )
{
    DWORD dwDataSize = spBuffer->GetLength();
    LPCSTR pData = spBuffer->GetData();

    if( dwDataSize < 3 )
        return false;

    m_dwPackageSize = MAKEWORD(pData[2],pData[1]);
    if( dwDataSize < m_dwPackageSize )
        return false;

    // OBEX packages are at least 3 bytes long. Prevent endless loops on damaged OBEX data
    if( m_dwPackageSize < 3 )
        m_dwPackageSize = 3;

    return true;
}

bool CObexCodec::DecodePackage( CBufferPtr spBuffer, Opcode_t enuActiveOpcode )
{
    LPCTSTR pcf = _T("CObexCodec::DecodePackage()");
    DWORD dwAllDataSize = spBuffer->GetLength();
    LPCSTR pData = spBuffer->GetData();

    //Traceit( TRACEIT_ENTER, TG_CObexCodec, _T("%s: dwDataSize=%ld, pData=%s"), pcf, dwAllDataSize, buffer2hex(dwAllDataSize,pData) );
    bool bSuccess = true;

    Reset();

    if( !IsCompletePackage( spBuffer ) )
    {
        //Traceit( TRACEIT_INFO, TG_CObexCodec, _T("%s: Package incomplete"), pcf );
        bSuccess = false;
    }
    else
    {
        // Do the decoding here

        DWORD dwDataSize = m_dwPackageSize; // This is the retrieved length and may be smaller than spBuffer

        DWORD dw = 3;   // Index to the pData array. Initially points behind the package length field

        m_byResponseCode = pData[0];
        
        switch( enuActiveOpcode )
        {
        case oreqGet:
        case oreqGetFolderListing:
            switch( (char)pData[0] )
            {
            case (char)0x20:
            case (char)0xa0:
                m_eOpcode = oresGet;
                break;
            case (char)0x90:
                m_eOpcode = oresGetContinue;
                break;
            default:
                m_eOpcode = oresFailure;
                m_hrFailureCause = MapObexResponseCode(pData[0]);
                break;
            }
            break;

        case oreqPut:
        case oreqPutFirst:
            switch( (char)pData[0] )
            {
            case (char)0x20:
            case (char)0xa0:
                m_eOpcode = oresSuccess;
                break;
            case (char)0x90:
                m_eOpcode = oresContinue;
                break;
            default:  
                m_eOpcode = oresFailure;  
                m_hrFailureCause = MapObexResponseCode(pData[0]);
                break;
            }
            break;

        case oreqConnect:
            switch( (char)pData[0] )
            {
            case (char)0xa0:
                if( dwDataSize >= 7 )
                {
                    m_eOpcode = oresSuccessConnect;
                    m_dwMaxObexPackageSize = MAKEWORD( pData[6], pData[5] ) - 3;
                }
                break;
            default:
                m_eOpcode = oresFailure;
                m_hrFailureCause = MapObexResponseCode(pData[0]);
                break;
            }

            if( dw + 4 > dwDataSize )
            {
                //Traceit( TRACEIT_ERROR, TG_CObexCodec, _T("%s: Connect response must be at least 7 bytes long"), pcf );
                bSuccess = false;
            }
            else
            {
                dw += 4;
            }

            break;

        case oreqIrmcPutFile:
            switch( (char)pData[0] )
            {
            case (char)0xa0:
                m_eOpcode = oresIrmcPutFile;
                break;
            case (char)0x90:
                m_eOpcode = oresContinue;
                break;
            default:
                m_eOpcode = oresFailure;
                m_hrFailureCause = MapObexResponseCode(pData[0]);
                break;
            }
            break;

        case oreqGetCapacity:
            switch( (char)pData[0] )
            {
            case (char)0xa0:
                m_eOpcode = oresGetCapacity;
                break;
            default:
                m_eOpcode = oresFailure;
                m_hrFailureCause = MapObexResponseCode(pData[0]);
                break;
            }
            break;

        case oreqFormat:
            switch( (char)pData[0] )
            {
            case (char)0xa0:
                m_eOpcode = oresFormat;
                break;
            case (char)0x90:
                m_eOpcode = oresContinue;
                break;
            default:
                m_eOpcode = oresFailure;
                m_hrFailureCause = MapObexResponseCode(pData[0]);
                break;
            }
            break;

        default:
            switch( (char)pData[0] )
            {
                case (char)0x20:    
                case (char)0xa0:
                    m_eOpcode = oresSuccess; break;
                case (char)0x90:
                    m_eOpcode = oresContinue; break;
                default:
                    m_eOpcode = oresFailure;
                    m_hrFailureCause = MapObexResponseCode(pData[0]);
                    break;
            }
            break;
        }

//        m_bFinalPackage = ((pData[0]>>7) & 0x01) ? true : false; // High order bit of opcode is final bit

        DWORD dwHeaderSize = 0;
        DWORD dwContentSize = 0;

        for( ; dw < dwDataSize && bSuccess; )
        {
            switch( pData[dw] )
            {
            case (char)0x48: // Body
            case (char)0x49: // EndOfBody
                if( dw + 3 > dwDataSize )
                {
                    //Traceit( TRACEIT_ERROR, TG_CObexCodec, _T("%s: EndOfBody header must be at least 3 bytes long"), pcf );
                    bSuccess = false;
                }
                else
                {
                    dwHeaderSize = MAKEWORD(pData[dw+2],pData[dw+1]);
                    dwContentSize = dwHeaderSize - 3;

                    if( dw + dwHeaderSize > dwDataSize )
                    {
                        //Traceit( TRACEIT_INFO, TG_CObexCodec, _T("%s: EndOfBody header contains less data than specified"), pcf );
                        bSuccess = false;
                    }
                    else
                    {
                        m_spBodyBuffer = new CBuffer;
                        LPSTR pBodyData = m_spBodyBuffer->GetBufferSetLength( dwContentSize );
                        memcpy( pBodyData, pData + dw + 3, dwContentSize );
                        m_spBodyBuffer->ReleaseBuffer( dwContentSize );
                    }
                }
                break;

            case (char)0x4C: // Application
                if( dw + 3 > dwDataSize )
                {
                    //Traceit( TRACEIT_ERROR, TG_CObexCodec, _T("%s: Header must be at least 3 bytes long"), pcf );
                    bSuccess = false;
                }
                else
                {
                    dwHeaderSize = MAKEWORD(pData[dw+2],pData[dw+1]);
                    dwContentSize = dwHeaderSize - 3;

                    if( dw + dwHeaderSize > dwDataSize )
                    {
                        //Traceit( TRACEIT_INFO, TG_CObexCodec, _T("%s: Header contains less data than specified"), pcf );
                        bSuccess = false;
                    }
                    else
                    {
                        DWORD dw2 = 0;
                        DWORD dwTag = 0;
                        DWORD dwLength = 0;
                        for( dw2 = dw+3; dw2 < dw + dwHeaderSize; dw2 += 2+dwLength )
                        {
                            dwTag = pData[dw2];
                            dwLength = pData[dw2+1];
                            if( dw2 + 2 + dwLength > dwDataSize )
                            {
                                //Traceit( TRACEIT_INFO, TG_CObexCodec, _T("%s: Parameter length info error"), pcf );
                                bSuccess = false;
                                break;
                            }
                            else
                            {
                                CBufferPtr spBuffer = new CBuffer( dwLength, pData+dw2+2 );
                                m_mapApplicationParameters[dwTag] = spBuffer;
                            }
                        }
                    }
                }
                break;

            case (char)0xc3: // Length
                if( dw + 5 > dwDataSize )
                {
                    //Traceit( TRACEIT_ERROR, TG_CObexCodec, _T("%s: Length header must be 5 bytes long"), pcf );
                    bSuccess = false;
                }
                else
                {
                    m_dwFileSize = MAKELONG( MAKEWORD(pData[dw+4],pData[dw+3]), MAKEWORD(pData[dw+2],pData[dw+1]) );
                    dwHeaderSize = 5;
                }
                break;

            case (char)0xcb: // ConnectionId
                if( dw + 5 > dwDataSize )
                {
                    //Traceit( TRACEIT_ERROR, TG_CObexCodec, _T("%s: ConnectionId header must be 5 bytes long"), pcf );
                    bSuccess = false;
                }
                else
                {
                    m_dwConnId = MAKELONG( MAKEWORD(pData[dw+4],pData[dw+3]), MAKEWORD(pData[dw+2],pData[dw+1]) );
                    dwHeaderSize = 5;
                }
                break;

            default: // Overread unknown header
                switch( (pData[dw] >> 6) & 0x03 ) // Upper two bits
                {
                case 0x00: // null terminated unicode text, prefixed with two bytes length
                case 0x01: // byte sequence prefixed wit two bytes length
                    if( dw + 3 > dwDataSize )
                    {
                        //Traceit( TRACEIT_INFO, TG_CObexCodec, _T("%s: Overread unknown header: header must be 3 bytes long"), pcf );
                        bSuccess = false;
                    }
                    else
                        dwHeaderSize = MAKEWORD(pData[dw+2],pData[dw+1]);
                    break;

                case 0x02: // 1 byte quantity
                    dwContentSize = 1;
                    dwHeaderSize = dwContentSize + 1;
                    break;
                case 0x03: // 4 byte quantity
                    dwContentSize = 4;
                    dwHeaderSize = dwContentSize + 1;
                    break;
                default:
                    bSuccess = false;
                    break;
                }
                break;
            }

            if( bSuccess )
            {
                if( dw + dwHeaderSize > dwDataSize )
                {
                    //Traceit( TRACEIT_INFO, TG_CObexCodec, _T("%s: Position to next header: package too short"), pcf );
                    bSuccess = false;
                }
                else
                    dw += dwHeaderSize;
            }
        }
    }

    //Traceit( TRACEIT_LEAVE, TG_CObexCodec, _T("CObexCodec::DecodePackage() bSuccess=%s, Opcode=%s"), bool2t(bSuccess), opcode2t(m_eOpcode) );

    return bSuccess;
}


DWORD CObexCodec::GetAppParamDWORD( DWORD dwIndex ) const
{
    DWORD dwResult = 0;
    std::map<DWORD,CBufferPtr>::const_iterator iter = m_mapApplicationParameters.find(dwIndex);
    if( iter != m_mapApplicationParameters.end() )
    {
        CBufferPtr spBuffer = iter->second;
        DWORD dwLength = spBuffer->GetLength();
        if( dwLength <= 10 ) // max DWORD is 4294967295 (10 digits)
        {
            char szResult[11] = "";
            memcpy( szResult, spBuffer->GetData(), dwLength );
            szResult[dwLength] = 0;

            sscanf( szResult, "%ld", &dwResult );
        }
    }
    return dwResult;
}

DWORD CObexCodec::GetAppParamBinaryDWORD( DWORD dwIndex ) const
{
    DWORD dwResult = (DWORD) GetAppParamBinaryQWORD(dwIndex);
    return dwResult;
}

QWORD CObexCodec::GetAppParamBinaryQWORD( DWORD dwIndex ) const
{
    QWORD qwResult = 0;
    std::map<DWORD,CBufferPtr>::const_iterator iter = m_mapApplicationParameters.find(dwIndex);
    if( iter != m_mapApplicationParameters.end() )
    {
        CBufferPtr spBuffer = iter->second;
        DWORD dwLength = spBuffer->GetLength();
        LPCSTR pcData = spBuffer->GetData();

        switch( dwLength )
        {
        case 1:
            qwResult = Bytes2QWORD( pcData[0], 0, 0, 0, 0, 0, 0, 0 );
            break;
        case 2:
            qwResult = Bytes2QWORD( pcData[1], pcData[0], 0, 0, 0, 0, 0, 0 );
            break;
        case 4:
            qwResult = Bytes2QWORD( pcData[3], pcData[2], pcData[1], pcData[0], 0, 0, 0, 0 );
            break;
        case 8:
            qwResult = Bytes2QWORD( pcData[7], pcData[6], pcData[5], pcData[4], pcData[3], pcData[2], pcData[1], pcData[0] );
            break;
        }
    }
    return qwResult;
}

CString CObexCodec::GetAppParamString( DWORD dwIndex ) const
{
    CString strResult;
    std::map<DWORD,CBufferPtr>::const_iterator iter = m_mapApplicationParameters.find(dwIndex);
    if( iter != m_mapApplicationParameters.end() )
    {
        CBufferPtr spBuffer = iter->second;
        DWORD dwLength = spBuffer->GetLength();
        strResult = CString(spBuffer->GetData(),dwLength);
    }
    return strResult;
}



