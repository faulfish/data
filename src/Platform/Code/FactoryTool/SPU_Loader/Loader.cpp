#include "StdAfx.h"
#include "Loader.h"
#include <iostream>
#include <io.h>
#include "LoaderExport.h"

bool Loader_CallbackClass::CheckBreak()
{
	return m_bBreak;
}

bool Loader_CallbackClass::WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize)
{
	_tprintf(_T("Please input:"));
	wscanf_s(L"%s", szBuffer, nBufferSize);
	return true;
}

bool Loader_CallbackClass::Print(const TSTRING& strName,const TSTRING& strOutput)
{
	for(size_t i = 0; i < strOutput.length(); i++)
	{
		_tprintf(_T("%c"),(strOutput.c_str())[i]);
	}
	return true;
}


CLoader::CLoader(LPISPU_CALLBACK pCallBack)
:m_pCallBack(pCallBack)
{
}

CLoader::~CLoader(void)
{
}

bool CLoader::LoadHeader()
{
	bool bHeaderFound = false, bHeaderFinished = false;
	CString	instring;
	CString	outstring;

	while(!bHeaderFound)
	{
		BOOL bRes = m_cFile.ReadString(instring);
		if(bRes == FALSE)
			break;
		if(instring == _T("{Header"))
			bHeaderFound = true;
	}


	while( bHeaderFound && !bHeaderFinished)
	{
		m_cFile.ReadString(instring);
		int nIndex = instring.Find(_T("//"), 0);

		if(nIndex != -1)
			outstring = instring.Left(nIndex);
		else
			outstring = instring;

		if(outstring == _T("}"))
			bHeaderFinished = true;
		else if(outstring == _T(""))
			continue;
		else
			m_strHeader.push_back(outstring.GetString());
	}
	
	return true;
}

bool CLoader::LoadDescription()
{
	bool bDescriptionFound = false, bDescriptionFinished = false;
	CString	instring;
	CString	outstring;

	while(!bDescriptionFound)
	{
		BOOL bRes = m_cFile.ReadString(instring);
		if(bRes == FALSE)
			break;
		if(instring == _T("{Description"))
			bDescriptionFound = true;
	}


	while( bDescriptionFound && !bDescriptionFinished)
	{
		m_cFile.ReadString(instring);
		int nIndex = instring.Find(_T("//"), 0);

		if(nIndex != -1)
			outstring = instring.Left(nIndex);
		else
			outstring = instring;

		if(outstring == _T("}"))
			bDescriptionFinished = true;
		else if(outstring == _T(""))
			continue;
		else
			m_strDescription.push_back(outstring.GetString());
	}

	return true;
}

int CLoader::GetCommandCount() const
{
	return m_strCommand.size();
}

bool CLoader::LoadCommand()
{
	bool bCodeFound = false, bCodeFinished = false;
	CString	instring;
	CString	outstring;

	while(!bCodeFound)
	{
		BOOL bRes = m_cFile.ReadString(instring);
		if(bRes == FALSE)
			break;
		if(instring == _T("{Code"))
			bCodeFound = true;
	}


	while( bCodeFound && !bCodeFinished)
	{
		BOOL bRes = m_cFile.ReadString(instring);
		if(bRes == FALSE)
			break;
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
			m_strCommand.push_back(outstring.GetString());
	}

	return true;
}

bool CLoader::LoadScript(const TSTRING& lpszScript)
{
	bool bRet = true;
	if(m_pCallBack == NULL)
		m_pCallBack = new Loader_CallbackClass;

	if(_taccess(lpszScript.c_str(), 0) == -1) //file not exist
	{
		m_pCallBack->Print(_T(""),_T("Script file path error!\n"));
	}
	else
	{
		m_szScript = lpszScript;

		m_cFile.Open(lpszScript.c_str(), CStdioFile::modeRead);

		bRet = LoadHeader();
		if(bRet)
		{
			bRet = LoadDescription();
			if(bRet)
				bRet = LoadCommand();
			else
				m_pCallBack->Print(_T(""),_T("Load Scription Failed!\n"));
		}
		else
			m_pCallBack->Print(_T(""),_T("Load Header Failed!\n"));

		m_cFile.Close();
	}
	return bRet;
}
bool CLoader::SetCallBackFunc(LPISPU_CALLBACK pCallBack)
{
	m_pCallBack = pCallBack;
	return true;
}

SPU_RESULT CLoader::ExecuteCommand()
{
	return LoadEx(m_strCommand, m_pCallBack,m_szScript);
}

void CLoader::GetHeader(TSTRING& strHeader)
{
	for(std::vector<TSTRING>::iterator Iter = m_strHeader.begin();Iter != m_strHeader.end();Iter++)
		strHeader+= *Iter + _T("\r\n");
}

void CLoader::GetDescription(TSTRING& strDescription)
{
	for(std::vector<TSTRING>::iterator Iter = m_strDescription.begin();Iter != m_strDescription.end();Iter++)
		strDescription+= *Iter + _T("\r\n");
}

UINT CLoader::GetUIType(const TSTRING& szScript)
{
	CStdioFile csFile;
	bool bHeaderFound = false, bHeaderFinished = false;
	CString	instring;
	CString	outstring;

	if(_taccess(szScript.c_str(), 0) != 0) //file not exist
	{
		return -1;
	}

	csFile.Open(szScript.c_str(), CStdioFile::modeRead);

	while(!bHeaderFound)
	{
		csFile.ReadString(instring);
		if(instring == _T("{Header"))
			bHeaderFound = true;
	}

	while( bHeaderFound && !bHeaderFinished)
	{
		csFile.ReadString(instring);
		int nIndex = instring.Find(_T(" "), 0);
		outstring = instring.Left(nIndex);
		if(outstring == _T("UI_TYPE"))
		{
			outstring = instring.Mid(nIndex+1);
			break;
		}

		if(instring == _T("}"))
			bHeaderFinished = true;
	}

	csFile.Close();
	if(bHeaderFinished)
		return 0;
	else
		return _ttoi(outstring.GetBuffer(0));
}
