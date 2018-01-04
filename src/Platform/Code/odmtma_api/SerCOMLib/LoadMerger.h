// LoadMerger.h: interface for the CLoadMerger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADMERGER_H__C415E9A3_5A60_430C_84E8_78463BE6D66B__INCLUDED_)
#define AFX_LOADMERGER_H__C415E9A3_5A60_430C_84E8_78463BE6D66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DLParameter.h"

class CLoadMerger  
{
public:
	CLoadMerger();
	virtual ~CLoadMerger();
	void SetLoadFileName(LPCTSTR szLoadFileName){
		_ASSERTE(szLoadFileName != NULL);
		m_strLoadFileName = szLoadFileName;
	};
	void SetOutputFileName(LPCTSTR szOutputFileName){
		_ASSERTE(szOutputFileName != NULL);
		m_strOutputFileName = szOutputFileName;
	};
	int Merge();
	int SetDLParameter();
	int EncryptAndSaveLoad(CFile &fLoadFile, CFile& fOutputFile);
	static int DecryptLoad(BYTE *pbBuf, BYTE *pbOut);
	static int DecryptLoad(CFile &fExe, BYTE *pbOut);
	static int CheckLoadCRC(BYTE *pbBuf, DWORD dwLoadSize, HWND hDisplay = NULL);
	static int CheckLoadCRC(CFile &fExe, DWORD dwLoadSize, HWND hDisplay);
	static void SeekToTargetCode(CFile& fExe, DWORD dwLoadSize);
	static void SetEncryptKeyword(BYTE *pbBuf);
	static void CalculateFileCRC(CFile &fSrc, DWORD& dwCRC);
	static void BlowBuf(BYTE *pbSrc, BYTE *pbDes, DWORD dwLen, BYTE* pbKey , BOOL bIsEncryptAction);
	static int CheckExeCRC(CFile &fExe, DWORD dwLoadSize);
private:
	CDLParameter	m_objDLParam;
	CString			m_strLoadFileName;
	CString			m_strOutputFileName;
};

#endif // !defined(AFX_LOADMERGER_H__C415E9A3_5A60_430C_84E8_78463BE6D66B__INCLUDED_)
