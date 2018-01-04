// Buffer.h: interface for the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFER_H__12BBB9E4_4CDC_410D_8070_C98B084FFAB2__INCLUDED_)
#define AFX_BUFFER_H__12BBB9E4_4CDC_410D_8070_C98B084FFAB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"

class CBuffer : public CRefObject
{
public:
	CBuffer();
    CBuffer( DWORD dwLength, const char* pData );
	CBuffer( size_t dwLength, const char* pData );
	CBuffer( int dwLength, const char* pData );
    CBuffer(SAFEARRAY* buffer);
	virtual ~CBuffer();

    char* GetBufferSetLength( DWORD dwMaxLength );
    void ReleaseBuffer( DWORD dwLength );
    void SetEmpty();
    void PopFront( DWORD dwNoOfBytes );
    void Append( CRefObjectPtr<CBuffer> spBuffer );    

    DWORD GetLength() const;
    const char* GetData() const;
protected:
	void SetDefault(DWORD dwLength, const char* pData);
private:
    DWORD m_dwMaxLength;
    DWORD m_dwLength;
    char* m_pData;
};

typedef CRefObjectPtr<CBuffer> CBufferPtr;

#endif // !defined(AFX_BUFFER_H__12BBB9E4_4CDC_410D_8070_C98B084FFAB2__INCLUDED_)
