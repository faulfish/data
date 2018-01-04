// Buffer.cpp: implementation of the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Buffer.h"

// ***************************************************************************************
// CBuffer
// ***************************************************************************************

CBuffer::CBuffer()
{
    m_dwMaxLength = 0;
    m_dwLength = 0;
    m_pData = NULL;
}

CBuffer::CBuffer( DWORD dwLength, const char* pData )
{
	SetDefault(dwLength,pData);
}

CBuffer::CBuffer( size_t dwLength, const char* pData )
{
	SetDefault(static_cast<DWORD>(dwLength),pData);
}

CBuffer::CBuffer( int dwLength, const char* pData )
{
	SetDefault(static_cast<DWORD>(dwLength),pData);
}

void CBuffer::SetDefault(DWORD dwLength, const char* pData)
{
    m_dwMaxLength = dwLength;
    m_dwLength = dwLength;
    m_pData = new char [dwLength];
    memcpy( m_pData, pData, dwLength );
}

CBuffer::CBuffer(SAFEARRAY* sa)
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

CBuffer::~CBuffer()
{
    if( m_pData )
        delete [] m_pData;
}

char* CBuffer::GetBufferSetLength( DWORD dwMaxLength )
{
    if( m_pData )
        delete [] m_pData;

    m_dwMaxLength = dwMaxLength;
    m_dwLength = 0;
    m_pData = new char [dwMaxLength];

    return m_pData;
}

void CBuffer::ReleaseBuffer( DWORD dwLength )
{
    m_dwLength = dwLength;
}

void CBuffer::SetEmpty()
{
    m_dwLength = 0;
}

void CBuffer::PopFront( DWORD dwNoOfBytes )
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

void CBuffer::Append( CBufferPtr spBuffer )
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

DWORD CBuffer::GetLength() const
{
    return m_dwLength;
}

const char* CBuffer::GetData() const
{
    return m_pData;
}