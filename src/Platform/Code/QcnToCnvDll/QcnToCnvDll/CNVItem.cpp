#include "stdafx.h"
#include "CNVItem.h"
#include <assert.h>

CNVItemData::CNVItemData()
:m_dwItemID(0xFFFFFFFF)
,m_dwItemLength(0)
,m_dwBufferLength(NV_BUFFER_LENGTH)
,m_pbItemContent(m_pbItemLocalBuffer)
,m_pbItemLargeBuffer(NULL)
,m_nNbOfElements(0)
,m_index(-1)
,m_active(false)
{
	::ZeroMemory(m_pbItemLocalBuffer,NV_BUFFER_LENGTH);
}

CNVItemData::~CNVItemData()
{
	if ( m_pbItemContent != m_pbItemLocalBuffer )
	{
		if(m_pbItemLargeBuffer)
			delete [] m_pbItemLargeBuffer;
	}
}

CNVItemData::CNVItemData(const CNVItemData& rhs)
:m_dwItemID(0xFFFFFFFF)
,m_dwItemLength(0)
,m_dwBufferLength(NV_BUFFER_LENGTH)
,m_pbItemContent(m_pbItemLocalBuffer)
,m_pbItemLargeBuffer(NULL)
,m_nNbOfElements(0)
,m_index(-1)
,m_active(false)
{
	*this = rhs;
		
}

CNVItemData& CNVItemData::operator=(const CNVItemData& rhs)
{
	m_index = rhs.m_index;
	m_active = rhs.m_active;
	m_dwItemID = rhs.m_dwItemID;
	m_dwItemLength = rhs.m_dwItemLength;
	m_dwBufferLength = rhs.m_dwBufferLength;
	m_nNbOfElements = rhs.m_nNbOfElements;
	if(rhs.m_pbItemLargeBuffer == NULL)
	{
		m_pbItemContent = m_pbItemLocalBuffer;
		memcpy(m_pbItemLocalBuffer,rhs.m_pbItemLocalBuffer,NV_BUFFER_LENGTH);
	}
	else
	{
		m_pbItemLargeBuffer = new(std::nothrow) BYTE[rhs.m_dwBufferLength];
		m_pbItemContent = m_pbItemLargeBuffer;
		memcpy(m_pbItemLargeBuffer,rhs.m_pbItemLargeBuffer,rhs.m_dwBufferLength);
	}
	return *this;
}

bool CNVItemData::operator==(const CNVItemData& rhs) const
{
	return rhs.m_dwItemID == m_dwItemID;
}

// Return value : 
//		ERROR_SUCCESS	--> Complete successfully
//				-1		-->	Heap allocation failure.
int CNVItemData::AddItemContent(BYTE* pbContent)
{
	int nRet = ERROR_SUCCESS;

	_ASSERTE(NULL != pbContent);
	if (m_dwItemLength == m_dwBufferLength)
	{
		// We need to allocate more memory to store the NV content.
		// prepare buffer
		m_pbItemLargeBuffer = new(std::nothrow) BYTE[m_dwBufferLength + NV_BUFFER_LENGTH ];
		if ( NULL == m_pbItemLargeBuffer )
		{
			return -1;
		}
		m_dwBufferLength += NV_BUFFER_LENGTH;
		ZeroMemory(m_pbItemLargeBuffer, m_dwBufferLength);
		CopyMemory(m_pbItemLargeBuffer, m_pbItemContent, m_dwItemLength);
		
		if ( m_pbItemContent != m_pbItemLocalBuffer)
			delete[] m_pbItemContent;

		m_pbItemContent = m_pbItemLargeBuffer;
		m_pbItemLargeBuffer = NULL;
	}
	
	CopyMemory((m_pbItemContent+m_dwItemLength), pbContent, NV_CONTENT_LENGTH);
	m_dwItemLength += NV_CONTENT_LENGTH;
	
	return nRet;
}

int CNVItemData::SetItemContent(BYTE* pbContent)
{
	int nRet = ERROR_SUCCESS;

	_ASSERTE(NULL != pbContent);
	//ZeroMemory(m_pbItemContent, m_dwItemLength);
	ZeroMemory(m_pbItemContent, m_dwItemLength);
	CopyMemory(m_pbItemContent, pbContent, m_dwItemLength);
	return nRet;
}

BYTE* CNVItemData::GetItemContent()
{
	//m_pbItemContent =m_pbItemLocalBuffer;
	return m_pbItemContent;
}

int CNVItemData::GetNVContentLen() const
{
	return m_dwItemLength;
}

void CNVItemData::Reset()
{
	m_dwItemID = 0xFFFFFFFF;
	m_dwItemLength = 0;
	m_dwBufferLength = NV_BUFFER_LENGTH;
	if ( m_pbItemContent != m_pbItemLocalBuffer ){
		delete[] m_pbItemContent;
	}
	m_pbItemContent = m_pbItemLocalBuffer;
	ZeroMemory(m_pbItemLocalBuffer,sizeof(m_pbItemLocalBuffer));
	m_pbItemLargeBuffer = NULL;
}

int CNVItemData::GetItemID() const
{
	return m_dwItemID;
}

void CNVItemData::SetItemID(DWORD dwItemID)
{
	m_dwItemID = dwItemID;
}

int CNVItemData::GetNbOfElements() const
{
	return m_nNbOfElements;
}

void CNVItemData::SetNbOfElements(int NbOfEle)
{
	m_nNbOfElements = NbOfEle;
}

int CNVItemData::GetIndex() const
{
	return m_index;
}

void CNVItemData::SetIndex(int index)
{
	m_index = index;
}

short CNVItemData::IsActive(void) const
{
	return m_active;
}

void CNVItemData::SetActive(short act)
{
	m_active = act;	
}
