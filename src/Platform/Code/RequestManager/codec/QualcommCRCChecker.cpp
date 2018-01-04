//////////////////////////////////////////////////////////////////////////////////
// 2004/10/11 Embert Tsai
// Add critical section control.
//
/////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <TCHAR.H>
#include <crtdbg.h>
#include <new>
#include "QualcommCRCChecker.h"
#include "CRCMachine.h"

QualcommCRCChecker::QualcommCRCChecker(bool bIsDownloadMode):m_bIsDownloadMode(bIsDownloadMode)
{
	m_pbBuffer = new BYTE[MEM_BUFFER_BLOCK_SIZE];
	ZeroMemory(m_pbBuffer, MEM_BUFFER_BLOCK_SIZE);
	m_dwBufPos = 0;
	// 20050823 Embert Tsai.
	m_nCurState = 0;
	m_bycmd=0;
}

QualcommCRCChecker::~QualcommCRCChecker()
{
	delete[] m_pbBuffer;
	m_pbBuffer = NULL;
	m_dwBufPos = 0;
	m_nCurState = 0;
}

// Boyan Zhou add 2007/4/10
// for cmd
void QualcommCRCChecker::SetCom(BYTE bycmd){
	m_bycmd = bycmd;
}
// end

// return value
// n: Return buffer
// 0: noraml read
// -1: skip
// -2: invalid header
// -3: CRC error
// -4: undefined error
int QualcommCRCChecker::Push(BYTE bData){
	
	//TRACE("QualcommCRCChecker::Push: %c\n", bData);
	LPCTSTR szFunc = _T("QualcommCRCChecker::Push");
	// m_nCurState
	// 0 Noraml
	// 1 LookAhead
	// 2 7E look ahead
	// -1 Skip
	if ( m_nCurState == 0 ){
		
		if ( m_dwBufPos == 0 && m_bIsDownloadMode && bData != 0x7E && bData != 0x08 ){
			m_nCurState = -1;
			return -2;
		}
		if ( m_dwBufPos == 0 && m_bIsDownloadMode && (bData == 0x7E || bData != 0x08) ){
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

void QualcommCRCChecker::Erase(){
	m_dwBufPos =0;
	m_nCurState = 0;
}

BYTE* QualcommCRCChecker::GetBuffer(){
	return m_pbBuffer;
}