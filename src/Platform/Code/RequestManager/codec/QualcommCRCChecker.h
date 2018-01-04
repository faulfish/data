#if !defined(MEMBUFFER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_)
#define MEMBUFFER_H__3D46E606_32E3_47C4_9053_68ECEEF82D74__INCLUDED_

// 2004/09/23 Embert Tsai.
// This class is created for 
#include <windows.h>
#include "Logger.h"

#define MEM_BUFFER_BLOCK_SIZE	16384
#define WEIRD_BYTE				0xA3
#define MEM_ARRAY_SIZE			2048
#define MEM_ARRAY_SIZE_MASK		(MEM_ARRAY_SIZE-1)

#define ESCAPE_CHAR			0x7D
#define END_CHAR			0x7E
#define COMPLEMENT_CHAR		0x20
#define START_CHAR			0x7E

class QualcommCRCChecker
{
private:
	BYTE	*m_pbBuffer;
	DWORD	m_dwBufPos;
	int		m_nCurState;	// 0 Noraml
							// 1 LookAhead
							// 2 7E look ahead
							// -1 Skip
	BYTE	m_bycmd;
	bool m_bIsDownloadMode;

public:

	QualcommCRCChecker(bool bIsDownloadMode);
	~QualcommCRCChecker();
	int Push(BYTE bData);
	//DWORD GetBufferSize();
	BYTE* GetBuffer();
	void Erase();
	void SetCom(BYTE bycmd);
	DWORD GetCurrentBufSize(){
		return m_dwBufPos;
	};

};
#endif