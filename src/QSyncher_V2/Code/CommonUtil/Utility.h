
#ifndef _UTILITY_H_
#define _UTILITY_H_

#define QWORD __int64
#define LODWORD(a) ((DWORD)(a))
#define HIDWORD(a) ((DWORD)((a)>>32))

enum ERRORCODE_OBEX
{
	ERR_ACCESSDENIED = 0,
	ERR_PAYMENTREQUIRED,
	ERR_NOTFOUND,
	ERR_MEMORY_FULL,
	ERR_UNSPECIFIED
};

inline QWORD Bytes2QWORD( BYTE c0, BYTE c1, BYTE c2, BYTE c3, BYTE c4, BYTE c5, BYTE c6, BYTE c7 )
{
    QWORD qw = QWORD(c0) | (QWORD(c1)<<8) | (QWORD(c2)<<16) | (QWORD(c3)<<24) | (QWORD(c4)<<32) | (QWORD(c5)<<40) | (QWORD(c6)<<48) | (QWORD(c7)<<56); 
    return qw;
}

inline HRESULT MapObexResponseCode( BYTE byCode )
{
    switch( byCode )
    {
    case 0xc1:          return ERR_ACCESSDENIED;
    case 0xc2:          return ERR_PAYMENTREQUIRED;
    case 0xc3:          return ERR_ACCESSDENIED;
    case 0xc4:          return ERR_NOTFOUND;
    case 0xe0:          return ERR_MEMORY_FULL;
    default:            return ERR_UNSPECIFIED;
    }
}

CString dirname( LPCTSTR szPathname );
CString basename( LPCTSTR szPathname );

CString adjustslash( LPCTSTR szPathname );

bool IsValidFilename( LPCTSTR szFilename );

#endif // _UTILITY_H_
