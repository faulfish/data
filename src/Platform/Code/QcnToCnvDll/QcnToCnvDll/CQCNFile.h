#ifndef CQCNFILE_H
#define CQCNFILE_H

#include <ATLBase.h>
#include <comutil.h>
#include "CNVItemFile.h"
#include <comdef.h>
#include <vector>



typedef CComPtr<IStorage> SIStorage;
class CQCNFile: public CNVItemFile
{
public:

	enum{
		CQCN_FILE_ERROR_OPEN_ROOT			=	0xF0000000 | 0x01,
		CQCN_FILE_ERROR_OPEN_MODEL,
		CQCN_FILE_ERROR_OPEN_CONFIG,
		CQCN_FILE_ERROR_OPEN_NV,
		CQCN_FILE_ERROR_READ_NV,
		CQCN_FILE_ERROR_NV_END,
		CQCN_FILE_ERROR_NOT_SUPPORT,
		CQCN_FILE_ERROR_MAX
	}CQCN_FILE_ERROR_TYPE;

	CQCNFile();
	virtual ~CQCNFile();

	virtual int Open(LPCTSTR szFileName, UINT nFlag);	
	int ReadNVItem(std::vector<CNVItemData>& NVItemArray);
	int OpenAndCreateFile(LPCTSTR szFileName, UINT nFlag);
	int WriteNVItem(CNVItemData& objNVItem);
	virtual int Close();
protected:	
	int OpenHeader();
	int OpenNVStorage();
	int ReadHeader(void **pvHeaderInfo);
	
private:

	CQCNFile(const CQCNFile&);
	CQCNFile& operator=(const CQCNFile&);

	int OpenModel();
	int OpenConfig();
	int GetNVItem(std::vector<CNVItemData>& NVItemArray); //for QCN 2
	bool IsLetterInString(LPCTSTR szCheck);
	CComPtr<IStorage> m_pRoot;      
	CComPtr<IStorage> m_pModelType;
	CComPtr<IStorage> m_pConfig;
	CComPtr<IStorage> m_pNVID;
	CComPtr<IEnumSTATSTG> m_pNVStorage;
	_bstr_t	m_bstrFileName;
	CString m_strModelName;
	WORD	m_wQCNVersion;
private:
	HRESULT hr;		
	IStorage *pStg;	
	IStorage *pSub1;	
	IStorage *pSub2;	
	IStorage *pSub3;	
	IStream *pStm;	
	IStream *pStmFile;	
};

#endif