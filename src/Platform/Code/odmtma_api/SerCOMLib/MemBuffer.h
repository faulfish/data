#if !defined(MEMBUFFER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_)
#define MEMBUFFER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_

// 2004/09/23 Embert Tsai.
// This class is created for 
#include <windows.h>
#include "SimpleCriticalSection.h"
#include "SerialIO.h"


#define MEM_BUFFER_BLOCK_SIZE	16384
#define WEIRD_BYTE				0xA3
#define MEM_ARRAY_SIZE			2048
#define MEM_ARRAY_SIZE_MASK		(MEM_ARRAY_SIZE-1)

// 20041110 Embert Tsai
// Diable memory cell function.
/*
class CMemCell{
private:
	BYTE	*m_pbBuf;
	DWORD	m_dwBufSize;
public:
    enum ErrorType {
	    ERROR_CMEMCELL_INVALID_PARAMETER	=	ERROR_MASK | 0x20301,	// INVALID Parameters.
	    ERROR_CMEMCELL_ALLOC_FAIL			=	ERROR_MASK | 0x20202,	// Memory Allocation error.
	    ERROR_CMEMCELL_UNKNOWN				=	ERROR_MASK | 0x203FF	// unknown error catched by run-time lib	
	};
	CMemCell();
	~CMemCell();
	DWORD GetSize();
	int Store(const void *pvSrc, DWORD dwLen);
	int Release();
	int Retrieve(void *pvDest);
	BYTE* GetBuffer(){
		return m_pbBuf;
	};
	void BeSelected(); // Mark this cell is selected, but not be polluted.

};

class CMemBuffer{
private:
	CMemCell *m_objCellArray;
	DWORD FindFreeCell();
	CSimpleCriticalSection m_csCellOp;	
public:
    enum ErrorType {
	    ERROR_CMEMBUFFER_ALLOC_FAIL		=	ERROR_MASK | 0x20201,	// CRC error
	    ERROR_CMEMBUFFER_UNKNOWN		=	ERROR_MASK | 0x201FF	// unknown error catched by run-time lib	
	};
	CMemBuffer();
	~CMemBuffer();
	int AppenBuffer(const void *pvSrc, DWORD dwLen);
	CMemCell& GetCell(DWORD dwCellIndex);
	void FreeCell(int nIndex);	
};
*/

class CRCChecker{
private:
	BYTE	*m_pbBuffer;
	DWORD	m_dwBufPos;
	int		m_nCurState;	// 0 Noraml
							// 1 LookAhead
							// 2 7E look ahead
							// -1 Skip
	CSerialIO::EState m_eState;
	BYTE	m_bycmd;

public:

	CRCChecker();
	~CRCChecker();
	int Push(BYTE bData);
	int Push(BYTE *m_pbBuffer,int nLength);
	//DWORD GetBufferSize();
	BYTE* GetBuffer();
	void Erase();
	void SetState(CSerialIO::EState eState);
	void SetCom(BYTE bycmd);
	DWORD GetCurrentBufSize(){
		return m_dwBufPos;
	};

};
#endif