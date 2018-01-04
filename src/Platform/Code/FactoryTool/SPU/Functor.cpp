#include "StdAfx.h"
#include "Functor.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include <cassert>

namespace QSYNC_LIB 
{
	//For runthread only
	typedef bool (*pFnType9999)(ISPU_CALLBACK* pISPU_CALLBACK);
	//For run
	typedef bool (*pFnType0000)();
	typedef bool (*pFnType0001)(char* szPara1);
	typedef bool (*pFnType0002)(char* szPara1,char* szPara2);
	typedef bool (*pFnType0003)(char* szPara1,char* szPara2,char* szPara3);
	typedef bool (*pFnType0004)(char* szPara1,char* szPara2,char* szPara3,char* szPara4);
	typedef bool (*pFnType0005)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5);
	typedef bool (*pFnType0006)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6);
	typedef bool (*pFnType0007)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6,char* szPara7);
	typedef bool (*pFnType0008)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6,char* szPara7,char* szPara8);
	typedef bool (*pFnType0009)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6,char* szPara7,char* szPara8,char* szPara9);
	typedef bool (*pFnType0010)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6,char* szPara7,char* szPara8,char* szPara9,char* szPara10);
	typedef bool (*pFnType0011)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6,char* szPara7,char* szPara8,char* szPara9,char* szPara10,char* szPara11);
	typedef bool (*pFnType0012)(char* szPara1,char* szPara2,char* szPara3,char* szPara4,char* szPara5,char* szPara6,char* szPara7,char* szPara8,char* szPara9,char* szPara10,char* szPara11,char* szPara12);

	CFunctor::CFunctor(const TSTRING& strAPIDLL,const TSTRING& strFunction,vector<CFunctorParameter>& vectParameter)
		:m_strAPIDLL(strAPIDLL)
		,m_strFunction(strFunction)
		,m_vectParameter(vectParameter)
		,m_hModule(::LoadLibrary(m_strAPIDLL.c_str()))
		,m_hProc(GetProcAddress(m_hModule,GetAnsi(strFunction).c_str()))
		,m_pISPU_CALLBACK(NULL)
		,m_bUseCallback(false)
	{
		assert(m_hModule);
		assert(m_hProc);
	}

	CFunctor::CFunctor(const TSTRING& strAPIDLL,const TSTRING& strFunction,vector<CFunctorParameter>& vectParameter,ISPU_CALLBACK* pISPU_CALLBACK)
		:m_strAPIDLL(strAPIDLL)
		,m_strFunction(strFunction)
		,m_vectParameter(vectParameter)
		,m_hModule(::LoadLibrary(m_strAPIDLL.c_str()))
		,m_hProc(GetProcAddress(m_hModule,GetAnsi(strFunction).c_str()))
		,m_pISPU_CALLBACK(pISPU_CALLBACK)
		,m_bUseCallback(true)
	{
		assert(m_hModule);
		assert(m_hProc);
	}

	CFunctor::~CFunctor(void)
	{
		if(m_pISPU_CALLBACK && m_bUseCallback)
			m_pISPU_CALLBACK->Print("CFunctor","EndOfCallbackFunction");

		if(m_hModule)
			::FreeLibrary(m_hModule);
	}

	bool  CFunctor::run()
	{
		return operator()();
	}

	bool CFunctor::operator()()
	{
		bool bRes = false;
		if(m_hProc && m_bUseCallback)
		{
			pFnType9999 hProc = (pFnType9999)m_hProc;
			bRes = hProc(m_pISPU_CALLBACK);
		}
		else if(m_hProc)
		{
			switch(m_vectParameter.size())
			{
			case 0:
				{
					pFnType0000 hProc = (pFnType0000)m_hProc;
					bRes = hProc();
				}
				break;
			case 1:
				{
					pFnType0001 hProc = (pFnType0001)m_hProc;
					bRes = hProc(m_vectParameter[0]);
				}
				break;
			case 2:
				{
					pFnType0002 hProc = (pFnType0002)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1]);
				}
				break;
			case 3:
				{
					pFnType0003 hProc = (pFnType0003)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2]);
				}
				break;
			case 4:
				{
					pFnType0004 hProc = (pFnType0004)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3]);
				}
				break;
			case 5:
				{
					pFnType0005 hProc = (pFnType0005)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4]);
				}
				break;
			case 6:
				{
					pFnType0006 hProc = (pFnType0006)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5]);
				}
				break;
			case 7:
				{
					pFnType0007 hProc = (pFnType0007)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5],m_vectParameter[6]);
				}
				break;
			case 8:
				{
					pFnType0008 hProc = (pFnType0008)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5],m_vectParameter[6],m_vectParameter[7]);
				}
				break;
			case 9:
				{
					pFnType0009 hProc = (pFnType0009)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5],m_vectParameter[6],m_vectParameter[7],m_vectParameter[8]);
				}
				break;
			case 10:
				{
					pFnType0010 hProc = (pFnType0010)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5],m_vectParameter[6],m_vectParameter[7],m_vectParameter[8],m_vectParameter[9]);
				}
				break;
			case 11:
				{
					pFnType0011 hProc = (pFnType0011)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5],m_vectParameter[6],m_vectParameter[7],m_vectParameter[8],m_vectParameter[9],m_vectParameter[10]);
				}
				break;
			case 12:
				{
					pFnType0012 hProc = (pFnType0012)m_hProc;
					bRes = hProc(m_vectParameter[0],m_vectParameter[1],m_vectParameter[2],m_vectParameter[3],m_vectParameter[4],m_vectParameter[5],m_vectParameter[6],m_vectParameter[7],m_vectParameter[8],m_vectParameter[9],m_vectParameter[10],m_vectParameter[11]);
				}
				break;
			default:
				break;
			};			
		}
		return bRes;
	}
}