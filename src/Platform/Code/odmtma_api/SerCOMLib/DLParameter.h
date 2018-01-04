// DLParameter.h: interface for the CDLParameter class.
//	20050722 Embert Tsai.
//	Download tools for end user.

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLPARAMETER_H__8CBBE8B9_E68B_43DA_A904_01602744AA68__INCLUDED_)
#define AFX_DLPARAMETER_H__8CBBE8B9_E68B_43DA_A904_01602744AA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DL_PARAMETER_BLOCK_SIZE 512

struct SDLParameter{
	char	szLoadFileName[32];
	char	szLoadVersion[16];
	WORD	wModelID;
	DWORD	dwLoadSize;
	DWORD	dwFlashSize;
	DWORD	dwNVOffset;
	DWORD	dwNVImgSize;
};

class CDLParameter  
{
private:
	// Member variable.
	CString		m_strLoadFileName;
	CString		m_strVersion;
	DWORD		m_dwModelID;
	DWORD		m_dwLoadSize;
	DWORD		m_dwFlashSize;
	DWORD		m_dwNVOffset;
	DWORD		m_dwNVImgSize;
	DWORD		m_dwWriteBlockSize;
	// Member fuction.
	void SetEncryptKeyword(BYTE *pbBuf);
public:
	CDLParameter();
	virtual ~CDLParameter();
	int		ReadParameterFromBuf(LPVOID lpBuf);
	int		ReadParameterFromFile(CFile &fExe);
	int		WriteToFile(CFile &f);
	void	SetLoadFileName(LPCTSTR szLoadFileName){
		_ASSERTE(szLoadFileName != NULL);
		m_strLoadFileName = szLoadFileName;
	};
	LPCTSTR GetLoadFileName(){
		return m_strLoadFileName;
	};
	void SetVersion(LPCTSTR szVersion){
		_ASSERTE(szVersion != NULL);
	};
	LPCTSTR GetVersion(){
		return m_strVersion;
	};
	void SetModelID(DWORD dwModelID){
		_ASSERTE(dwModelID > 0);
		m_dwModelID = dwModelID;
	};
	DWORD GetModelID(){
		return m_dwModelID;
	};
	void SetLoadSize(DWORD dwLoadSize){
		_ASSERTE(dwLoadSize > 0);
		m_dwLoadSize = dwLoadSize;
	};
	DWORD GetLoadSize(){
		return m_dwLoadSize;
	};
	void SetFlashSize(DWORD dwFlashSize){
		_ASSERTE(dwFlashSize > 0);
		m_dwFlashSize = dwFlashSize;
	};
	DWORD GetFlashSize(){
		return m_dwFlashSize;
	};
	void SetNVOffset(DWORD dwNVOffset){
		_ASSERTE(dwNVOffset > 0);
		m_dwNVOffset = dwNVOffset;
	};
	DWORD GetNVOffset(){
		return m_dwNVOffset;
	};
	void SetNVImgSize(DWORD dwNVImgSize){
		_ASSERTE(dwNVImgSize > 0);
		m_dwNVImgSize = dwNVImgSize;
	};
	DWORD GetNVImgSize(){
		return m_dwNVImgSize;
	};
	void SetWriteBlockSize(DWORD dwWriteBlockSize){
		_ASSERTE(dwWriteBlockSize > 0);
		m_dwWriteBlockSize = dwWriteBlockSize;
	};
	DWORD GetWriteBlockSize(){
		return m_dwWriteBlockSize;
	};

};

#endif // !defined(AFX_DLPARAMETER_H__8CBBE8B9_E68B_43DA_A904_01602744AA68__INCLUDED_)
