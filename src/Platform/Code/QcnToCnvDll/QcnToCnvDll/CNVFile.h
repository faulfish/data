#pragma once
#include "cnvitemfile.h"
#include "CNVItemFile.h"


typedef unsigned char uint8;
typedef unsigned short uint16;

class CCNVFile : public CNVItemFile
{
public:
	enum{
		CCNV_FILE_ERROR_FILE_NAME,
		CCNV_FILE_ERROR_FILE_OPERATOR,
		CCNV_FILE_ERROR_MAX
	}CCNV_FILE_ERROR_TYPE;

	CCNVFile(void);
	virtual ~CCNVFile(void);
	virtual int Open(LPCTSTR szFileName, UINT nFlag);	
private:
	int WriteHeader();
	int ReWrite(const void* lpBuf, UINT nCount);
public:
	int WriteNVItem(CNVItemData&);
	int WriteEnd();
	virtual int Close(void);
public:
	int OpenAndReadHeader(LPCTSTR szFileName, UINT nFlag);
	int ReadHeader();
	int ReadNVItem(CArray<CNVItemData,CNVItemData&>& NVItemArray);

public:
	CFile m_fCNV;
	bool m_bIsOpened;
};
