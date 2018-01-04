// SPU.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SPU.h"
#include <assert.h>
#include <stdlib.h>
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\Win32ThreadUtility.h"
#include "..\..\CommonUtil\LogPolicy.h"
#include "..\..\CommonUtil\QPerformanceTimer.h"
#include "Functor.h"

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    // initialize MFC and print and error on failure
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
        // TODO: change error code to suit your needs
        _tprintf(_T("Fatal Error: MFC initialization failed\n"));
        nRetCode = 1;
    }
    else
    {
        // TODO: code your application's behavior here.
    }

    return nRetCode;
}

using namespace QSYNC_LIB;

CSPU::CSPU(LPISPU_CALLBACK pISPU_CALLBACK)
:m_pISPU_CALLBACK(pISPU_CALLBACK)
,m_strScriptName(_T("SPU_SCRIPT"))
,m_hThread(NULL)
{
}

CSPU::CSPU(LPISPU_CALLBACK pISPU_CALLBACK,const TSTRING& strScriptName)
:m_pISPU_CALLBACK(pISPU_CALLBACK)
,m_strScriptName(strScriptName)
{
}

CSPU::~CSPU(void)
{
}

void CSPU::Init()
{
	//Registers
	int nIP = 0;
	int nLoopIP = 0;
	bool bCompare = false;
	int nLoopCount = 0;

	CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	CString strTemp;
	strTemp.Format(_T("%s_%s.%06d"),m_strScriptName.c_str(),strTime,clock());
	m_strLogScriptName = strTemp;
}

void CSPU::GetToken(const TSTRING& strOriginal,TSTRING& strToken1,TSTRING& strToken2,const TSTRING& strSeperator)
{
	//Avoid " " in the strToken1
	TSTRING::size_type nStartFindIndex = strOriginal.find(_T("\""));
	if(nStartFindIndex != TSTRING::npos && nStartFindIndex == 0)
	{
		TSTRING::size_type nFindEndIndex = strOriginal.find(_T("\""),1);

		TSTRING::size_type nFindIndex = strOriginal.find(strSeperator,nFindEndIndex);
		if(nFindIndex != TSTRING::npos)
		{
			strToken1 = strOriginal.substr(1,nFindEndIndex-1);
			strToken2 = strOriginal.substr(nFindIndex+strSeperator.size());
		}
		else
		{
			strToken1 = strOriginal.substr(1,nFindEndIndex-1);;
			strToken2.clear();
		}
	}
	else
		QSYNC_LIB::GetToken(strOriginal,strToken1,strToken2,strSeperator);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2009/2/12 By alan.Tu
	//Please don't remove " " for strToken2
	//Avoid " " in the strToken2
	/*
	nStartFindIndex = strToken2.find(_T("\""));
	if(nStartFindIndex != TSTRING::npos && nStartFindIndex == 0)
	{
		TSTRING::size_type nFindEndIndex = strToken2.find(_T("\""),1);
		strToken2 = strToken2.substr(1,nFindEndIndex-1);
	}
	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	QSYNC_LIB::TrimLeft(strToken1,_T(": \r\n\t"));
	QSYNC_LIB::TrimRight(strToken1,_T(": \r\n\t"));
	QSYNC_LIB::TrimLeft(strToken2,_T(": \r\n\t"));
	QSYNC_LIB::TrimRight(strToken2,_T(": \r\n\t"));
}

SPU_RESULT CSPU::Run(vector<TSTRING>& strMemory)
{
	SPU_RESULT res = SPU_SUCCESS;
	
	//Registers
	int nIP = 0;
	int nLoopIP = 0;
	bool bCompare = false;
	int nLoopCount = 0;	
	vector<TSTRING> strVars;
	TSTRING strEmpty;
	for(int i=0;i<REGISTER_COUNT;i++)
		strVars.push_back(strEmpty);

	CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
	CString strTemp;
	strTemp.Format(_T("%s_%s.%06d"),m_strScriptName.c_str(),strTime,clock());
	m_strLogScriptName = strTemp;

	while(SPU_SUCCESS == res && nIP < static_cast<int>(strMemory.size()))
	{
		TSTRING strADDR,strParameter;
		GetToken(strMemory[nIP++],strADDR,strParameter,_T(" "));
		TSTRING strInstruction;
		GetToken(strParameter,strInstruction,strParameter,_T(" "));
		MakeLower(strInstruction);
		res = Execuate(strInstruction,strParameter,nIP,nLoopIP,bCompare,nLoopCount,strVars);
	};
	return res;
}

bool  CSPU::SetValue(const TSTRING& strParameter,const TSTRING& strValue,vector<TSTRING>& strVars)
{
	bool bRes = false;
	if(strParameter.find(_T("%var")) == 0)
	{
		TSTRING strInteger = strParameter.substr(4);
		int nIndex = toInt(strInteger);
		if(nIndex < REGISTER_COUNT)
		{
			strVars[nIndex] = strValue;
			bRes = true;
		}
	}
	return bRes;
}

TSTRING  CSPU::GetValue(const TSTRING& strParameter,vector<TSTRING>& strVars)
{
	TSTRING strTemp;
	if(strParameter.find(_T("%var")) == 0)
	{
		TSTRING strInteger = strParameter.substr(4);
		int nIndex = toInt(strInteger);
		if(nIndex < REGISTER_COUNT)
			strTemp = strVars[nIndex];
	}
	else
	{	
		//Avoid " " in the strParameter
		TSTRING::size_type nStartFindIndex = strParameter.find(_T("\""));
		if(nStartFindIndex != TSTRING::npos && nStartFindIndex == 0)
		{
			TSTRING::size_type nFindEndIndex = strParameter.find(_T("\""),1);
			strTemp = strParameter.substr(1,nFindEndIndex-1);
		}
		else
			strTemp = strParameter;
	}

	return strTemp;
}

SPU_RESULT CSPU::Execuate(const TSTRING& strInstruction,const TSTRING& strParameter
	,int& nIP
	,int& nLoopIP
	,bool& bCompare
	,int& nLoopCount
	,vector<TSTRING>& strVars	)
{
	SPU_RESULT res = SPU_FAIL;
	DWORD nStartTime = GetTickCount();

	CString strLog;
	strLog.Format(_T("%s %s"),strInstruction.c_str(),strParameter.c_str());
	Log((LPCTSTR)strLog,m_strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);

	if(strInstruction == _T("sleep"))
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : sleep 5000
		//Sample2 : sleep [var1]
		///////////////////////////////////////////////////////////////////////////////////////
		::Sleep(toInt(GetValue(strParameter,strVars)));
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("assign"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : assign %var1 123456		
		//Sample2 : assign %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strVariable,strValue;
		GetToken(strParameter,strVariable,strValue,_T(" "));
		res = SetValue(strVariable,GetValue(strValue,strVars),strVars) ? SPU_SUCCESS : SPU_FAIL;
	}
	else if(strInstruction == _T("compare"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		bCompare = GetValue(strValue1,strVars) ==  GetValue(strValue2,strVars);

		CString strLog;
		strLog.Format(_T("\"%s\" == \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,m_strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("bigger"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		bCompare = ::atoi(GetValue(strValue1,strVars).c_str()) > ::atoi(GetValue(strValue2,strVars).c_str());

		CString strLog;
		strLog.Format(_T("\"%s\" > \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,m_strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("smaller"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : compare %var1 123456		
		//Sample2 : compare %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strValue1,strValue2;
		GetToken(strParameter,strValue1,strValue2,_T(" "));
		bCompare = ::atoi(GetValue(strValue1,strVars).c_str()) < ::atoi(GetValue(strValue2,strVars).c_str());

		CString strLog;
		strLog.Format(_T("\"%s\" < \"%s\" is %d"),GetValue(strValue1,strVars).c_str(),GetValue(strValue2,strVars).c_str(),bCompare);
		Log((LPCTSTR)strLog,m_strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("print"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : print 123456 NAME
		//Sample2 : print %var2 %var1
		///////////////////////////////////////////////////////////////////////////////////////		
		TSTRING strTarget,strName;
		GetToken(strParameter,strTarget,strName,_T(" "));
		if(m_pISPU_CALLBACK)
		{
			if(m_pISPU_CALLBACK->Print(GetValue(strName,strVars),GetValue(strTarget,strVars)))
				res = SPU_SUCCESS;
		}
	}
	else if(strInstruction == _T("loop"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : loop -1
		//Sample2 : loop 2
		//Sample3 : loop 
		///////////////////////////////////////////////////////////////////////////////////////
		if(strParameter.empty())
		{
			if(nLoopCount != -1)
			{
				nLoopCount--;
				if(nLoopCount > 0)
					nIP = nLoopIP;
			}
			else nIP = nLoopIP;			
		}
		else
		{
			nLoopIP = nIP;
			nLoopCount = toInt(GetValue(strParameter,strVars));
		}
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("waitinput"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : waitinput %var1 Name
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strTarget,strName;
		GetToken(strParameter,strTarget,strName,_T(" "));

		if(m_pISPU_CALLBACK)
		{
			TCHAR szBuffer[SPU_BUFFER_SIZE] = {0};
			assert(GetValue(strTarget,strVars).size() < SPU_BUFFER_SIZE);
			_tcscpy_s(szBuffer,SPU_BUFFER_SIZE,GetValue(strTarget,strVars).c_str());
			if(m_pISPU_CALLBACK->WaitInput(GetValue(strName,strVars),szBuffer,SPU_BUFFER_SIZE))
			{
				SetValue(strTarget,szBuffer,strVars);
				res = SPU_SUCCESS;
			}
		}
	}
	else if(strInstruction == _T("checkbreak"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : checkbreak
		///////////////////////////////////////////////////////////////////////////////////////
		if(m_pISPU_CALLBACK)
			res = m_pISPU_CALLBACK->CheckBreak() ? SPU_BREAK : SPU_SUCCESS;
		else
			res = SPU_SUCCESS;//Without any check
	}
	else if(strInstruction == _T("comparetrue"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : comparetrue 0005
		///////////////////////////////////////////////////////////////////////////////////////
		if(bCompare == true)
			nIP = toInt(strParameter);
		if(nIP == 9999)
			res = SPU_FAIL;
		else
			res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("comparefalse"))
	{
		///////////////////////////////////////////////////////////////////////////////////////		
		//Sample1 : comparefalse 0005
		///////////////////////////////////////////////////////////////////////////////////////
		if(bCompare == false)
			nIP = toInt(strParameter);
		if(nIP == 9999)
			res = SPU_FAIL;
		else
			res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("run"))
	{
		TSTRING strAPIDLL;
		TSTRING strFunction;

		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : run API.dll Function1 %var1 ABC %var3
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strFunctionAndParameter;
		GetToken(strParameter,strAPIDLL,strFunctionAndParameter,_T(" "));

		TSTRING strFunctionParameter;
		GetToken(strFunctionAndParameter,strFunction,strFunctionParameter,_T(" "));

		vector<CFunctorParameter> vectParameter;
		TSTRING strFunctionParameter1,strFunctionParameter2 = strFunctionParameter;
		do
		{			
			GetToken(strFunctionParameter2,strFunctionParameter1,strFunctionParameter2,_T(" "));
			if(!strFunctionParameter1.empty())
			{
				TSTRING strValue,strType;
				GetToken(strFunctionParameter1,strValue,strType,_T("::"));
				CFunctorParameter parameter(GetValue(strValue,strVars),strType,strValue,strVars);
				vectParameter.push_back(parameter);
			}
		}while(!strFunctionParameter2.empty());

		CFunctor command(strAPIDLL,strFunction,vectParameter);
		bCompare = command();
		res = SPU_SUCCESS;

		strLog.Format(_T("Compare flag = %d"),bCompare);
		Log((LPCTSTR)strLog,m_strLogScriptName,_T("9999"),_T("-1"),_T("8"),strVars);
	}
	else if(strInstruction == _T("runscript"))
	{
		TSTRING strScript = strParameter;
		bCompare = SPU_SUCCESS == ExecuateScript(strScript,m_pISPU_CALLBACK);
		res = SPU_SUCCESS;
	}
	else if(strInstruction == _T("runthread"))
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : runthread API.dll Function1
		///////////////////////////////////////////////////////////////////////////////////////
		TSTRING strAPIDLL;
		TSTRING strFunction;

		///////////////////////////////////////////////////////////////////////////////////////
		//Sample1 : run API.dll Function1 %var1 ABC %var3
		///////////////////////////////////////////////////////////////////////////////////////
		GetToken(strParameter,strAPIDLL,strFunction,_T(" "));

		bCompare = SPU_SUCCESS == ExecuateThread(strAPIDLL,strFunction,m_pISPU_CALLBACK);		
		res = SPU_SUCCESS;
	}
	else
	{
		assert(0);
		CString strLog;
		strLog.Format(_T("There is a unknown instruction on your script. \"%s\""),strInstruction);
		Log((LPCTSTR)strLog,m_strLogScriptName,_T("9999"),_T("-1"),_T("1"),strVars);
	}

	// Save log
	//CString cstrParameter = strParameter.c_str();
	//cstrParameter.Replace(_T("\""), _T("\"\""));

	//CString strLog_RunTime;
	//DWORD nRunTime = GetTickCount() - nStartTime;
	//strLog_RunTime.Format(_T("%s,\"%s\",%d"), strInstruction.c_str(), cstrParameter, nRunTime);
	//switch(res)
	//{
	//case SPU_SUCCESS:
	//	strLog_RunTime += _T(",SPU_SUCCESS");
	//	break;
	//case SPU_FAIL:
	//	strLog_RunTime += _T(",SPU_FAIL");
	//	break;
	//case SPU_BREAK:
	//	strLog_RunTime += _T(",SPU_BREAK");
	//	break;
	//default:
	//	strLog_RunTime += _T(",Unknown response");
	//	break;
	//}

	//LogPolicy::Log(strLog_RunTime, _T("SPU_RunTime.csv"), false);
	return res;
}

void CSPU::Log(const TSTRING& strLog,const TSTRING& strName,const TSTRING& strErrorCode,const TSTRING& strMinorErrorCode,const TSTRING& strPriority,vector<TSTRING>& strVars)
{
	vector<CFunctorParameter> vectLogParameter;
	TSTRING strValue,strType;
	strValue = strName;
	CFunctorParameter parameter1(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter1);
	strValue = strErrorCode;
	CFunctorParameter parameter2(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter2);
	strValue = strMinorErrorCode;
	CFunctorParameter parameter3(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter3);
	strValue = strPriority;
	CFunctorParameter parameter4(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter4);
	strValue = strLog;
	CFunctorParameter parameter5(strValue,strType,strValue,strVars);
	vectLogParameter.push_back(parameter5);
#ifdef _DEBUG
	CFunctor Logcommand(_T("ReportCenter_StaticMD.dll"),_T("Log_ForSPU"),vectLogParameter);
#else
	CFunctor Logcommand(_T("ReportCenter_StaticM.dll"),_T("Log_ForSPU"),vectLogParameter);
#endif
	Logcommand();
}

DWORD WINAPI CSPU::ThreadProc( LPVOID lpParam ) 
{
	CSPU *pThis = (CSPU *)lpParam;

	vector<CFunctorParameter> vectParameter;
	CFunctor* pCommand = new CFunctor(pThis->m_strThreadAPIDLL,pThis->m_strThreadFunction,vectParameter,pThis->m_pThreadCallBack); 	
	pThis->m_nThreadRet = pCommand->run() ? SPU_SUCCESS : SPU_FAIL;
	delete pCommand;
	return 0;
}

SPU_RESULT CSPU::ExecuateThread(const TSTRING& strAPIDLL,const TSTRING& strFunction,LPISPU_CALLBACK pCallBack)
{
	DWORD nThreadID = 0;
	m_strThreadAPIDLL= strAPIDLL;
	m_strThreadFunction= strFunction;
	m_pThreadCallBack= pCallBack;
	m_hThread = ::CreateThread(NULL,0,ThreadProc,(LPVOID)this,0,&nThreadID);
	using namespace QSYNC_LIB;
	MessageLoop(&m_hThread,1);
	return m_nThreadRet;
}

SPU_RESULT CSPU::ExecuateScript(const TSTRING& lpszScript,LPISPU_CALLBACK pCallBack)
{
	SPU_RESULT nRet = SPU_SUCCESS;
	vector<TSTRING> strMemory;

	if(_taccess(lpszScript.c_str(), 0) == -1) //file not exist
	{
		if(pCallBack)
			pCallBack->Print(_T(""),_T("Script file path error!\n"));
		nRet = SPU_FAIL;
	}
	else
	{
		CStdioFile cFile;
		if(cFile.Open(lpszScript.c_str(), CStdioFile::modeRead))
		{
			nRet = LoadCommand(cFile,strMemory) ? SPU_SUCCESS : SPU_FAIL;
			if(nRet == SPU_FAIL && pCallBack)
				pCallBack->Print(_T(""),_T("Load Scription Failed!\n"));
			cFile.Close();
		}
		else
		{
			if(pCallBack)
				pCallBack->Print(_T(""),_T("Open Script file error!\n"));
			nRet = SPU_FAIL;
		}
	}

	if(nRet == SPU_SUCCESS)
	{		
		nRet = Run(strMemory);
	}

	return nRet;
}

bool CSPU::LoadCommand(CStdioFile& cFile,vector<TSTRING>& strMemory)
{
	bool bCodeFound = false, bCodeFinished = false;
	CString	instring;
	CString	outstring;

	while(!bCodeFound)
	{
		cFile.ReadString(instring);
		if(instring == _T("{Code"))
			bCodeFound = true;
	}

	while( bCodeFound && !bCodeFinished)
	{
		cFile.ReadString(instring);
		int nIndex = instring.Find(_T("//"), 0);

		if(nIndex != -1)
			outstring = instring.Left(nIndex);
		else
			outstring = instring;

		if(outstring == _T("}"))
			bCodeFinished = true;
		else if(outstring == _T(""))
			continue;
		else
			strMemory.push_back(outstring.GetString());
	}

	return true;
}