//////////////////////////////////////////////////////////////////////////////////
// 2004/10/11 Embert Tsai
// Add critical section control.
//
/////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <TCHAR.H>
#include <crtdbg.h>
#include <new>
#include "Logger.h"
#include "MemBuffer.h"
#include "SerialIO.h"
#include "CRCMachine.h"

// 20041110 Embert Tsai
// Disable old memory cell function.
/*
CMemCell::CMemCell() : m_pbBuf(NULL), m_dwBufSize(0) {
}

CMemCell::~CMemCell(){
	Release();
}

DWORD CMemCell::GetSize(){
	return m_dwBufSize;
}

int CMemCell::Store(const void *pvSrc, DWORD dwLen){
	LPCTSTR szFuncName = _T("CMemCell::Store()");
	_ASSERTE(pvSrc != NULL);
	_ASSERTE(dwLen > 0 );
	_ASSERTE(m_pbBuf == NULL);
	_ASSERTE(m_dwBufSize == -1);
	if ( dwLen == 0 ){
		return ERROR_CMEMCELL_INVALID_PARAMETER;
	}
	if ( pvSrc == NULL ){
		return ERROR_CMEMCELL_INVALID_PARAMETER;
	}
	try {
		m_pbBuf = new BYTE[dwLen];
		ZeroMemory(m_pbBuf, dwLen);
		CopyMemory(m_pbBuf, pvSrc, dwLen);
		m_dwBufSize = dwLen;
	}catch (std::bad_alloc&){
		_ASSERTE(FALSE);
		LogFunctionInformation(LOG_LEVEL_ERROR, szFuncName, "Allocation memory fails.");
		LogInformation(LOG_LEVEL_ERROR, "The new buffer size is %d", m_dwBufSize);
		return ERROR_CMEMCELL_ALLOC_FAIL;
	}
	return (int)dwLen;
}

int CMemCell::Release(){
	if ( m_pbBuf != NULL){
		delete[] m_pbBuf;
	}
	m_pbBuf = NULL;
	m_dwBufSize = 0;
	return RUN_SUCCESSFULLY;
}

int CMemCell::Retrieve(void *pvDest){
	_ASSERTE(m_pbBuf != NULL);
	_ASSERTE(m_dwBufSize > 0);
	_ASSERTE(pvDest != NULL);
	
	CopyMemory(pvDest, m_pbBuf, m_dwBufSize );
	return m_dwBufSize;
}

void CMemCell::BeSelected(){
	_ASSERTE( m_pbBuf == NULL);
	_ASSERTE( m_dwBufSize == 0 );
	m_dwBufSize = -1;	// This Cell is polluted.
}

CMemBuffer::CMemBuffer()
{
	m_objCellArray = new CMemCell[MEM_ARRAY_SIZE];
}

CMemBuffer::~CMemBuffer(){
	delete[] m_objCellArray;
	m_objCellArray = NULL;
}

int CMemBuffer::AppenBuffer(const void*pvSrc, DWORD dwLen){
	LPCTSTR szFunc = _T("CMemBuffer::AppendBuffer()");
	_ASSERTE(pvSrc != NULL);
	_ASSERTE(dwLen > 0 );
	// Find free cell
	DWORD dwIndex = FindFreeCell(); 
	int nRet = 0;
	nRet = m_objCellArray[dwIndex].Store(pvSrc, dwLen);
	if ( nRet < 0){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("Store memory cell failed."));
		return nRet;
	}
	return dwIndex;
}

CMemCell& CMemBuffer::GetCell(DWORD dwCellIndex){
	_ASSERTE( dwCellIndex < MEM_ARRAY_SIZE);
	return m_objCellArray[dwCellIndex];
}

// 2004/10/11 Change to Thread safe
DWORD CMemBuffer::FindFreeCell(){
	LARGE_INTEGER liIndex; 
	DWORD dwIndex = 0;
	QueryPerformanceCounter(&liIndex);
	dwIndex = liIndex.LowPart & MEM_ARRAY_SIZE_MASK;
	while ( TRUE){

		m_csCellOp.Lock();
		if ( m_objCellArray[dwIndex].GetSize() == 0	){
			m_objCellArray[dwIndex].BeSelected();
			m_csCellOp.Unlock();
			break;
		}
		m_csCellOp.Unlock();

		dwIndex ++;
		if ( dwIndex == MEM_ARRAY_SIZE){
			dwIndex = 0;
		}
	
	}
	return dwIndex;
}

void CMemBuffer::FreeCell(int nIndex){
	_ASSERTE(m_objCellArray[nIndex].GetSize() != 0);
	m_csCellOp.Lock();
	m_objCellArray[nIndex].Release();
	m_csCellOp.Unlock();
}
*/
CRCChecker::CRCChecker(){
	m_pbBuffer = new BYTE[MEM_BUFFER_BLOCK_SIZE];
	ZeroMemory(m_pbBuffer, MEM_BUFFER_BLOCK_SIZE);
	m_dwBufPos = 0;
	// 20050823 Embert Tsai.
	m_nCurState = 0;
	m_bycmd=0;
}

CRCChecker::~CRCChecker(){
	delete[] m_pbBuffer;
	m_pbBuffer = NULL;
	m_dwBufPos = 0;
	m_nCurState = 0;
}

void CRCChecker::SetState(CSerialIO::EState eState){
	m_eState = eState;
}
// Boyan Zhou add 2007/4/10
// for cmd
void CRCChecker::SetCom(BYTE bycmd){
	m_bycmd = bycmd;
}
// end

int CRCChecker::Push(BYTE *pbBuffer,int nLength)
{
	::memcpy_s(m_pbBuffer,nLength,pbBuffer,nLength);
	m_dwBufPos = nLength;
	return m_dwBufPos;
}

// return value
// n: Return buffer
// 0: noraml read
// -1: skip
// -2: invalid header
// -3: CRC error
// -4: undefined error
int CRCChecker::Push(BYTE bData){
	
	//TRACE("CRCChecker::Push: %c\n", bData);
	LPCTSTR szFunc = _T("CRCChecker::Push");
	// m_nCurState
	// 0 Noraml
	// 1 LookAhead
	// 2 7E look ahead
	// -1 Skip
	if ( m_nCurState == 0 ){
		
		if ( m_dwBufPos == 0 && m_eState == CSerialIO::EDL && bData != 0x7E && bData != 0x08 ){
			m_nCurState = -1;
			return -2;
		}
		if ( m_dwBufPos == 0 && m_eState == CSerialIO::EDL && (bData == 0x7E || bData != 0x08) ){
			m_nCurState = 2;
			return 0;
		}
		
		if ( bData == ESCAPE_CHAR){
			m_nCurState = 1;
			return 0;
		}
		WORD wCRC;
		switch( bData )
			{
			// #define ESCAPE_CHAR		0x7D
			// #define END_CHAR			0x7E
			// #define COMPLEMENT_CHAR	0x20
			case END_CHAR:
				if(m_dwBufPos < 2)	// CRC is two byte long, so if length is smaller than 2 bytes, then the buffer is not correct.
				{
						//+ CRCError
					wCRC = 0;
				}
				else
				{
					wCRC = CCRCMachine::CRC16LCalculation((CHAR*)m_pbBuffer, (WORD)(m_dwBufPos * 8));
				}
				if(wCRC != CRC16LOK)
				{
					m_dwBufPos = 0;
					return -3;
				}else {
					return (m_dwBufPos-2);
				}
				break;
			default:
				m_pbBuffer[m_dwBufPos] = bData;
				m_dwBufPos++;
				return 0;
				break;
			}

	}
	
	if ( m_nCurState == 2 ){
		m_pbBuffer[m_dwBufPos] = bData ;
		m_nCurState = 0;
		m_dwBufPos++;
		return 0;
	}

	if ( m_nCurState == -1  ){
		if ( bData != 0x7E){
			return -1;
		}else{
			m_nCurState = 0;
			return -1;
		}
	}

	if ( m_nCurState == 1 ){
		m_pbBuffer[m_dwBufPos] = bData ^ COMPLEMENT_CHAR;
		m_nCurState = 0;
		m_dwBufPos++;
		return 0;
	}
	
	_ASSERTE(FALSE);
	return -4;
}

void CRCChecker::Erase(){
	m_dwBufPos =0;
	m_nCurState = 0;
}

BYTE* CRCChecker::GetBuffer(){
	return m_pbBuffer;
}