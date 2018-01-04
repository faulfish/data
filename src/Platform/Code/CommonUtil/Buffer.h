// Buffer.h: interface for the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFER_H__12BBB9E4_4CDC_410D_8070_C98B084FFAB2__INCLUDED_)
#define AFX_BUFFER_H__12BBB9E4_4CDC_410D_8070_C98B084FFAB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"

class CBuffer;
typedef CRefObjectPtr<CBuffer> CBufferPtr;

class CBuffer : public CRefObject
{
public:
	CBuffer()
	{
		m_dwMaxLength = 0;
		m_dwLength = 0;
		m_pData = NULL;
	}

	CBuffer( DWORD dwLength, const char* pData )
	{
		SetDefault(dwLength,pData);
	}

	CBuffer( size_t dwLength, const char* pData )
	{
		SetDefault(static_cast<DWORD>(dwLength),pData);
	}

	CBuffer( int dwLength, const char* pData )
	{
		SetDefault(static_cast<DWORD>(dwLength),pData);
	}
	/*
	CBuffer(SAFEARRAY* sa)
	{
	m_dwMaxLength = 0;
	m_dwLength = 0;
	m_pData = 0;

	VARIANT *data;    
	if(SUCCEEDED(SafeArrayAccessData(sa,(void HUGEP**)&data)))
	{
	long lbound,ubound;
	SafeArrayGetLBound(sa,1,&lbound);
	SafeArrayGetUBound(sa,1,&ubound);
	m_dwLength = ubound-lbound;

	if(m_dwLength > 0)
	{
	m_pData = new char [m_dwLength];
	m_dwMaxLength = m_dwLength;
	memcpy( m_pData, data, m_dwLength );
	}
	SafeArrayUnaccessData(sa);
	}
	} 
	*/
	virtual ~CBuffer()
	{
		if( m_pData )
			delete [] m_pData;
	}

	char* GetBufferSetLength( DWORD dwMaxLength )
	{
		if( m_pData )
			delete [] m_pData;

		m_dwMaxLength = dwMaxLength;
		m_dwLength = 0;
		m_pData = new char [dwMaxLength];

		return m_pData;
	}

	void ReleaseBuffer( DWORD dwLength )
	{
		m_dwLength = dwLength;
	}

	void SetEmpty()
	{
		m_dwLength = 0;
	}

	void PopFront( DWORD dwNoOfBytes )
	{
		if( dwNoOfBytes == 0 )
			return;

		if( dwNoOfBytes >= m_dwLength )
		{
			m_dwLength = 0;
			return;
		}

		memmove( m_pData, m_pData + dwNoOfBytes, m_dwLength - dwNoOfBytes );
		m_dwLength -= dwNoOfBytes;
	}

	void Append( CBufferPtr spBuffer )
	{
		if( spBuffer == NULL || spBuffer->GetLength() == 0 )
			return;

		// Make sure that we have enough room
		if( m_pData == NULL )
		{
			m_dwMaxLength = 0;
			m_dwLength = 0;
		}

		if( m_dwLength + spBuffer->GetLength() > m_dwMaxLength )
		{
			// Realloc the storage
			const DWORD dwReallocSize = 20000;
			DWORD dwRequiredLength = m_dwLength + spBuffer->GetLength() + dwReallocSize;
			char* pNewData = new char[dwRequiredLength];

			if( m_dwLength > 0 )
			{
				memcpy( pNewData, m_pData, m_dwLength );
			}

			if( m_pData )
			{
				delete [] m_pData;
				m_pData = NULL;
			}

			m_pData = pNewData;
			m_dwMaxLength = dwRequiredLength;
		}

		memcpy( m_pData + m_dwLength, spBuffer->GetData(), spBuffer->GetLength() );
		m_dwLength += spBuffer->GetLength();
	}

	DWORD GetLength() const
	{
		return m_dwLength;
	}

	const char* GetData() const
	{
		return m_pData;
	}
protected:
	void SetDefault(DWORD dwLength, const char* pData)
	{
		m_dwMaxLength = dwLength;
		m_dwLength = dwLength;
		m_pData = new char [dwLength];
		memcpy( m_pData, pData, dwLength );
	}
private:
	DWORD m_dwMaxLength;
	DWORD m_dwLength;
	char* m_pData;
};

#endif // !defined(AFX_BUFFER_H__12BBB9E4_4CDC_410D_8070_C98B084FFAB2__INCLUDED_)
