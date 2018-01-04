#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include <afx.h>
#include "../../FactoryTool/spu/SPUExport.h"

/****************************************************************************/
//Loader default callback function
/****************************************************************************/
class Loader_CallbackClass : public ISPU_CALLBACK
{
public:
	Loader_CallbackClass(void):m_bBreak(false){}
	virtual ~Loader_CallbackClass(void)	{}
	virtual bool CheckBreak();
	virtual bool WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize);
	virtual bool Print(const TSTRING& strName,const TSTRING& strOutput);
private:
	bool	m_bBreak;
};

class CLoader
{
public:
	CLoader(LPISPU_CALLBACK pCallBack);
public:
	~CLoader(void);

public:
	bool LoadScript(const TSTRING& lpszScript);
	bool SetCallBackFunc(LPISPU_CALLBACK pCallBack);
	UINT GetUIType(const TSTRING& szScript);
	SPU_RESULT ExecuteCommand();
	void GetHeader(TSTRING& strHeader);
	void GetDescription(TSTRING& strDescription);
	int GetCommandCount() const;

private:
	bool LoadHeader();
	bool LoadDescription();
	bool LoadCommand();
	TSTRING		m_szScript;
	LPISPU_CALLBACK		m_pCallBack;
	std::vector<TSTRING> m_strDescription;
	std::vector<TSTRING>	m_strCommand;
	std::vector<TSTRING> m_strHeader;
	CStdioFile	m_cFile;
};

