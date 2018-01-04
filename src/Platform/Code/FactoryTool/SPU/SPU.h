#pragma once

#include <vector>
#include <string>
#include "ISPU_CALLBACK.h"

#define REGISTER_COUNT 12
using namespace std;

class CSPU : public IValue
{
public:
	CSPU(LPISPU_CALLBACK pISPU_CALLBACK);
	CSPU(LPISPU_CALLBACK pISPU_CALLBACK,const TSTRING& strScriptName);
	virtual ~CSPU(void);
	SPU_RESULT Run(vector<TSTRING>& strMemory);
protected:
	SPU_RESULT Execuate(const TSTRING& strInstruction,const TSTRING& strParameter
	,int& nIP
	,int& nLoopIP
	,bool& bCompare
	,int& nLoopCount
	,vector<TSTRING>& strVars);
	TSTRING  GetValue(const TSTRING& strParameter,vector<TSTRING>& strVars);
	virtual bool  SetValue(const TSTRING& strParameter,const TSTRING& strValue,vector<TSTRING>& strVars);
	void Init();
	void GetToken(const TSTRING& strOriginal,TSTRING& strToken1,TSTRING& strToken2,const TSTRING& strSeperator);
	void Log(const TSTRING& strLog,const TSTRING& strName,const TSTRING& strErrorCode,const TSTRING& strMinorErrorCode,const TSTRING& strPriority,vector<TSTRING>& strVars);

	//Internal
	static DWORD WINAPI ThreadProc( LPVOID lpParam ) ;
	SPU_RESULT ExecuateThread(const TSTRING& strAPIDLL,const TSTRING& strFunction,LPISPU_CALLBACK pCallBack);
	SPU_RESULT ExecuateScript(const TSTRING& lpszScript,LPISPU_CALLBACK pCallBack);
	bool LoadCommand(CStdioFile& cFile,vector<TSTRING>& strMemory);
private:
	LPISPU_CALLBACK m_pISPU_CALLBACK;

	//Log
	TSTRING m_strScriptName;
	TSTRING m_strLogScriptName;

	//Thread
	HANDLE m_hThread;
	TSTRING m_strThreadAPIDLL;
	TSTRING m_strThreadFunction;
	LPISPU_CALLBACK m_pThreadCallBack;
	SPU_RESULT m_nThreadRet;
};
