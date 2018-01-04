#ifndef CBRTFILE_H
#define CBRTFILE_H
#include "CNVItemFile.h"
typedef DWORD BRT_FILE_HEADER;
class CBRTFile: public CNVItemFile{
public:
	enum{
		CBRT_FILE_ERROR_FILE_NAME,
		CBRT_FILE_ERROR_FILE_OPERATOR,
		CBRT_FILE_ERROR_MAX
	}CBRT_FILE_ERROR_TYPE;
	CBRTFile(){
	};
	virtual ~CBRTFile(){
	}
	virtual int Open(LPCTSTR szFileName, UINT nFlag);
	virtual int WriteHeader(const void *pvHeaderInfo);
	virtual int WriteNVItem(CNVItemData&);
	virtual int Close();
public:
	int OpenAndReadHeader(LPCTSTR szFileName, UINT nFlag);
	int ReadHeader();
	int ReadNVItem(CArray<CNVItemData,CNVItemData&>& NVItemArray);
private:
	CFile m_fBRT;
};
#endif