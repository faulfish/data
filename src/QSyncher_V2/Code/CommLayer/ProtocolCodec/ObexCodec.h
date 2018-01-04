// ObexCodec.h: interface for the CObexCodec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBEXCODEC_H__E282A9D3_F480_4AD8_8213_C74AEBF2A44C__INCLUDED_)
#define AFX_OBEXCODEC_H__E282A9D3_F480_4AD8_8213_C74AEBF2A44C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "RefObject.h"
#include "Buffer.h"
#include "Utility.h"
#include <map>
#include <string>

class CObexCodec : public CRefObject
{
public:
    enum Opcode_t
    {
        oUnknown = 0,
        oreqPut,
        oreqPutFirst,
        oreqGet,
        oreqConnect,
        oreqDisconnect,
        oreqGetFolderListing,
        oreqSetPath,
        oreqMoveFile,
        oreqGetCapacity,
        oreqAbort,
        oresSuccess,
        oresContinue,
        oresGet,
        oresGetContinue,
        oresSuccessConnect,
        oresGetCapacity,
        oresFailure,
        oreqIrmcPutFile,
        oresIrmcPutFile,
        oreqFormat,
        oresFormat,
        oreqGetRecursiveFolderSize,
        oreqGetMaximumFilenameLength,
    };
public:
	CObexCodec( Opcode_t enuOpcode = oUnknown );
	virtual ~CObexCodec();
    void Reset();

    static LPCTSTR opcode2t( Opcode_t eOpcode );

// Encoding functions
public:
	static CBufferPtr CreateReqOBEXConnect(std::string strTarget);
	static CBufferPtr CreateReqOBEXConnect(CBufferPtr ptTarget);
    static CBufferPtr CreateRequestPut( CBufferPtr spContentBuffer );
	static CBufferPtr CreateReqOBEXConnectSyncML();
    static CBufferPtr CreateReqOBEXConnectFileSystem();
    static CBufferPtr CreateReqOBEXConnectIrmc();
    static CBufferPtr CreateReqOBEXDisconnect( DWORD dwOBEXConnId );
	static CBufferPtr CreateReqOBEXGet( DWORD dwOBEXConnId, std::string strType );
    static CBufferPtr CreateReqOBEXGetFolderListing( DWORD dwOBEXConnId );
    static CBufferPtr CreateReqOBEXGetFileFirst( DWORD dwOBEXConnId, LPCTSTR szFilename );
    static CBufferPtr CreateReqOBEXGetFileNext( DWORD dwOBEXConnId );
    static CBufferPtr CreateReqOBEXPutFileFirst( DWORD dwOBEXConnId, LPCTSTR szFilename, DWORD dwFileSize,
         CBufferPtr spBuffer, bool bLastPackage, DWORD dwMaxPackageSize, DWORD& dwDataSize );
    static CBufferPtr CreateReqOBEXPutFileNext( DWORD dwOBEXConnId, CBufferPtr spBuffer, bool bLastPackage,
        DWORD dwMaxPackageSize, DWORD& dwDataSize );
    static CBufferPtr CreateReqOBEXSetAttributes( DWORD dwOBEXConnId, LPCTSTR szFilename, bool bHidden,
        bool bReadonly, bool bSystem, bool bArchiv, bool bDirectory );
    static CBufferPtr CreateReqOBEXMoveFile( DWORD dwOBEXConnId, LPCTSTR szFilename, LPCTSTR szNewFilename );
    static CBufferPtr CreateReqOBEXDeleteFile( DWORD dwOBEXConnId, LPCTSTR szFilename );
    static CBufferPtr CreateReqOBEXSetPath( DWORD dwOBEXConnId, LPCTSTR szPath, bool bCreateDir = false );
    static CBufferPtr CreateReqOBEXAbort( DWORD dwOBEXConnId );
    static CBufferPtr CreateReqOBEXCopy( DWORD dwOBEXConnId, LPCTSTR szFilename, LPCTSTR szNewFilename );
    static CBufferPtr CreateReqOBEXCreateStdDirs( DWORD dwOBEXConnId, LPCTSTR szDrive );
    static CBufferPtr CreateReqOBEXGetCapacity( DWORD dwOBEXConnId, LPCTSTR szDrive, bool bFreeSpace );
    static CBufferPtr CreateReqIrmcPutFile(  DWORD dwOBEXConnId, LPCTSTR szFilename, CBufferPtr spBuffer,
        DWORD dwMaxExpectedChangeCounter, DWORD dwMaxObexPackageSize,  DWORD& dwDataSize );
    static CBufferPtr CreateReqIrmcPutFileNext( DWORD dwOBEXConnId, CBufferPtr spBuffer,
        DWORD dwMaxPackageLength, DWORD& dwDataSize );

    static CRefObjectPtr<CObexCodec> CreateReqFormat( DWORD dwOBEXConnId, LPCTSTR szDrive );
    static CRefObjectPtr<CObexCodec> CreateReqFormatContinue( DWORD dwOBEXConnId );
    static CRefObjectPtr<CObexCodec> CreateReqGetRecursiveFolderSize( DWORD dwOBEXConnId, LPCTSTR szPathname );
    static CRefObjectPtr<CObexCodec> CreateReqGetMaximumFilenameLength( DWORD dwOBEXConnId, LPCTSTR szDrive );

// Decoding functions
public:
    bool IsCompletePackage( CBufferPtr spBuffer );
    bool DecodePackage( CBufferPtr spBuffer, Opcode_t enuActiveOpcode );

    DWORD GetPackageSize() const { return m_dwPackageSize; }
    CBufferPtr GetPackageBuffer() const { return m_spPackageBuffer; }
    Opcode_t GetOpcode() const { return m_eOpcode; }
    CBufferPtr GetBody() const { return m_spBodyBuffer; }
    DWORD GetConnId() const { return m_dwConnId; }
    HRESULT GetFailureCause() const { return m_hrFailureCause; }
    DWORD GetFileSize() const { return m_dwFileSize; }
    DWORD GetMaxObexPackageSize() const { return m_dwMaxObexPackageSize; }
    DWORD GetAppParamDWORD( DWORD dwIndex ) const;
    DWORD GetAppParamBinaryDWORD( DWORD dwIndex ) const;
    QWORD GetAppParamBinaryQWORD( DWORD dwIndex ) const;
    CString GetAppParamString( DWORD dwIndex ) const;
    BYTE GetResponseCode() const { return m_byResponseCode; }

private:
    DWORD m_dwPackageSize;
    CBufferPtr m_spPackageBuffer;
    Opcode_t m_eOpcode;
    CBufferPtr m_spBodyBuffer;
    DWORD m_dwConnId;
    HRESULT m_hrFailureCause;
    BYTE m_byResponseCode;
    DWORD m_dwFileSize;
    DWORD m_dwMaxObexPackageSize;
    std::map<DWORD,CBufferPtr> m_mapApplicationParameters; // <dwTag,spContent>
};


typedef CRefObjectPtr<CObexCodec> CObexCodecPtr;


#endif // !defined(AFX_OBEXCODEC_H__E282A9D3_F480_4AD8_8213_C74AEBF2A44C__INCLUDED_)
