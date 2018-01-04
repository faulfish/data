#pragma once
#include "afx.h"
#include "..\..\CommonUtil\Buffer.h"

class CHexFile :public CFile
{
private:
	UINT m_nlength;
	UINT m_npostion;
	BYTE m_address[2];
	
public:
	CHexFile(void);
	~CHexFile(void);
	BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException * pError= NULL);
	UINT Read(char * lpBuf , UINT nCount);
	WORD GetAddress();
	WORD GetHexAddrss(void);
	static BYTE TwoAsciToByte(char * pcassci);
};


class CHexDataParser
{
public:
	CHexDataParser(BYTE** pData, int nDataSize, WORD& wStartAddress);
	virtual ~CHexDataParser();
	
	DWORD GetHexAddrss();
	UINT Read(char* lpBuf, UINT nRequestCount, bool& bFailinRead);
private:
	bool ReadALine(CBufferPtr& ptBuffer);
	bool ParseALine(CBufferPtr& ptBuffer);

private:
	DWORD m_dwAddress;
	CBufferPtr m_ptLeftData;
	bool m_bEndOfData; 
	int m_nTotalDataSize;
	int m_nLeftSize;
	BYTE* m_pData;  // as File Pointer
};
