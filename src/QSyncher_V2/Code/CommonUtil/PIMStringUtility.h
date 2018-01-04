#ifndef _PIMSTRING_UTILITY_H_
#define _PIMSTRING_UTILITY_H_
#pragma once
#include <string>
#include <list>
#include <vector>
#include <atlenc.h>
#include <cstringt.h>
#include <stdlib.h>
#include <time.h>

namespace QSYNC_LIB 
{

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	using namespace std;
	
	template<typename Str>// typename Str is only for TSTRING and CString
	bool Tokenize(const Str& strSplitter, const Str& strOrg, vector<Str>& strvDes, bool bSpaceOn)
	{
		bool bSuccess = false;
		if(bSpaceOn)
		{
			Str strSrc = strOrg;
			Str strFirst, strSecond;
			while(Split<Str>(strSplitter, strSrc, strFirst, strSecond))
			{
				bSuccess = true;
				strvDes.push_back(strFirst);
				strSrc = strSecond;
			}
			strvDes.push_back(strFirst);
		}
		else
		{
			int nPos = 0;
			TSTRING strSrc = strOrg;
			TSTRING szSplitter = strSplitter;
			CString strToken = _T("");
			CString strCOrg = strSrc.c_str();

			strToken = strCOrg.Tokenize(szSplitter.c_str(), nPos);
			while( strToken != _T("") )
			{
				strvDes.push_back((Str)strToken);
				strToken = strCOrg.Tokenize(szSplitter.c_str(), nPos);
			}
			bSuccess = true;
		}
		return bSuccess;
	}

	template<typename Str>// typename Str is only for TSTRING and CString
	bool Tokenize(const Str& strSplitter, const Str& strOrg, list<Str>& strvDes, bool bSpaceOn)
	{
		bool bSuccess = false;
		if(bSpaceOn)
		{
			Str strSrc = strOrg;
			Str strFirst, strSecond;
			while(Split<Str>(strSplitter, strSrc, strFirst, strSecond))
			{
				bSuccess = true;
				strvDes.push_back(strFirst);
				strSrc = strSecond;
			}
			strvDes.push_back(strFirst);
		}
		else
		{
			int nPos = 0;
			TSTRING strSrc = strOrg;
			TSTRING szSplitter = strSplitter;
			CString strToken = _T("");
			CString strCOrg = strSrc.c_str();

			strToken = strCOrg.Tokenize(szSplitter.c_str(), nPos);
			while( strToken != _T("") )
			{
				strvDes.push_back((Str)strToken);
				strToken = strCOrg.Tokenize(szSplitter.c_str(), nPos);
			}
			bSuccess = true;
		}
		return bSuccess;
	}


	/* 
	 *  Split by finding the first splitter. 
	 *  If there is no splitter, strFirst will be strOrg, and strSecode will be ""  
	 */
	template<typename Str>// typename Str is only for TSTRING and CString
	bool Split(const Str& strSplitter, const Str& strOrg, Str& strFirst, Str& strSecond )
	{
		bool bRes = false;
		TSTRING szOrg = strOrg;
		TSTRING szSplitter = strSplitter;
		basic_string <char>::size_type nLocation = szOrg.find(strSplitter, 0);
		if( nLocation != TSTRING::npos )
		{
			strFirst = szOrg.substr(0, nLocation).c_str();
			strSecond = szOrg.substr(nLocation+szSplitter.size(), szOrg.size()-nLocation-szSplitter.size()).c_str();
			bRes = true;
		}
		else
		{
			strFirst = strOrg;
		}
		return bRes;
	} 


	//Connect String
	template<typename Str>// typename Str is only for TSTRING and CString
	Str StrConnect(const Str& strSplitter, vector<Str>& strvCont, bool bSpaceOn=false)
	{
		TSTRING strReturn = _T("");
		TSTRING szSplitter = strSplitter;
		for(vector<Str>::iterator iterCont = strvCont.begin(); iterCont != strvCont.end(); iterCont++)
		{
			TSTRING strItem = (*iterCont);
			if(strItem.size() != 0 || bSpaceOn)
			{
				strReturn += (*iterCont);
				strReturn += strSplitter;
			}
		}
		strReturn = strReturn.substr(0, strReturn.size()-szSplitter.size());

		return (Str)strReturn.c_str();
	}

	template<typename Str>// typename Str is only for TSTRING and CString
	Str StrConnect( const Str& strSplitter, list<Str>& strvCont, bool bSpaceOn =false)
	{
		TSTRING strReturn = _T("");
		TSTRING szSplitter = strSplitter;
		for(list<Str>::iterator iterCont = strvCont.begin(); iterCont != strvCont.end(); iterCont++)
		{
			TSTRING strItem = (*iterCont);
			if(strItem.size() != 0 || bSpaceOn)
			{
				strReturn += (*iterCont);
				strReturn += strSplitter;
			}
		}
		strReturn = strReturn.substr(0, strReturn.size()-szSplitter.size());

		return (Str)strReturn.c_str();
	}

	template<typename Str>// typename Str is only for TSTRING and CString
	Str StrConnect( const Str& strSplitter, const Str& strFirst, const Str& strSecond, bool bSpaceOn =false)
	{
		TSTRING strReturn = _T("");
		TSTRING szFirst = strFirst;
		TSTRING szSecond = strSecond;

		if(bSpaceOn || (szFirst.size() != 0 && szSecond.size() != 0))
		{
			strReturn += strFirst;
			strReturn += strSplitter;
			strReturn += strSecond;
		}
		else
		{	
			if(szFirst.size() == 0 )
				strReturn = strSecond;
			if(szSecond.size() == 0)
				strReturn = strFirst;
		}
		return (Str)strReturn.c_str();
	}

	template<typename Str>// typename Str is only for TSTRING and CString
	Str StrConnect( const Str& strSplitter, const Str& strFirst, const Str& strSecond, const Str& strThird, bool bSpaceOn =false)
	{
		Str strReturn = StrConnect<Str>( strSplitter, strFirst, strSecond, bSpaceOn );
		strReturn = StrConnect<Str>( strSplitter, strReturn, strThird, bSpaceOn );
		return strReturn;
	}

	
	template<typename Str>// typename Str is only for TSTRING and CString
	int GetRepeatCount( const Str& strKeyword, Str& strOrg )
	{
		int nCount =0;
		Str strFirst, strSecond;
		while(Split<Str>( strKeyword, strOrg, strFirst, strSecond ))
		{
			nCount++;
			strOrg = strSecond;
		}
		return nCount;
	}

	template<typename Str>// typename Str is only for TSTRING and CString
	int GetRepeatCount( const Str& strSplitter, const Str& strKeyword, const Str& strOrg)
	{
		int nCount = 0;
		bool bRes = true;
		vector<Str> strvDes;
		bRes = Tokenize<Str>( strSplitter, strOrg, strvDes, true);
		for(vector<Str>::iterator iter = strvDes.begin(); iter != strvDes.end(); iter++)
		{
			if(GetRepeatCount(strKeyword, (*iter)) >0 )
				++nCount;
		}
		return nCount;
	}
	template<typename Str>// typename Str is only for TSTRING and CString
	int GetRepeatCount( const Str& strSplitter, const Str& strKeyword, const Str& strOrg, bool bRemoveDot)
	{
		int nCount = 0;
		bool bRes = true;
		if(bRemoveDot)
		{
			vector<Str> strvDes;
			bRes = Tokenize<Str>( strSplitter, strOrg, strvDes, true);
			for(vector<Str>::iterator iter = strvDes.begin(); iter != strvDes.end(); iter++)
			{
			
				Str strFirst, strSecond;
				Split<Str>(_T("."),(*iter),strFirst, strSecond);
				if(strKeyword == strFirst)				
					++nCount;
			}
		}
		else
		{
			nCount = GetRepeatCount(strSplitter, strKeyword, strOrg);
		}
		
		return nCount;
	}

	// _ttoi or _tstoi (string to int)
	template<typename Str>// typename Str is only for TSTRING and CString
	bool iToTStr(int nInt, Str& strDes) 
	{
		bool bRes = false;
		TCHAR pDes[50];
		if(pDes)
		{
			_itot_s(nInt, pDes, 50, 10);
			strDes = pDes;
			bRes = true;
		}
		return bRes;
	}

	
	bool StrToTime(const CString& strTime, COleDateTime& oleTime);  //strTime = yyyymmddThhmmssZ
	bool ConvertTime(const COleDateTime& DTOrg, COleDateTime& DTDes, bool bUTCtoLocal);
	int MinusTime(const CString& strFirst, const CString& strSecond);
}

#endif //_PIMSTRING_UTILITY_H_