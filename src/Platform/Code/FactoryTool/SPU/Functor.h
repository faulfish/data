#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\LogPolicy.h"
#include <vector>
#include "ISPU_CALLBACK.h"

namespace QSYNC_LIB 
{
#define SPU_BUFFER_SIZE 2048
#define REGISTER_COUNT 12

	class CFunctorParameter
	{
	public:
		CFunctorParameter(const TSTRING& strValue,const TSTRING& strType,const TSTRING& strOutputName,vector<TSTRING>& strVars)
			:m_strType(strType)
			,m_strValue(strValue)
			,m_strOutputName(strOutputName)
			,m_szBuffer(new char[SPU_BUFFER_SIZE])
			,m_strVars(strVars)
		{
			::ZeroMemory(m_szBuffer,SPU_BUFFER_SIZE);
			string strAnsi = GetAnsi(m_strValue);
			if(strAnsi.size() > SPU_BUFFER_SIZE)
			{
				delete [] m_szBuffer;
				m_szBuffer = new char[strAnsi.size()+1];
				::strcpy_s(m_szBuffer,strAnsi.size()+1,strAnsi.c_str());
			}
			else
				::strcpy_s(m_szBuffer,SPU_BUFFER_SIZE,strAnsi.c_str());
		}
		~CFunctorParameter()
		{
			SetValue(m_strOutputName,GetTSTRING(m_szBuffer),m_strVars);
			delete [] m_szBuffer;
		}
		CFunctorParameter(const CFunctorParameter& rhs)
			:m_strVars(rhs.m_strVars)
		{		
			m_strType.assign(rhs.m_strType.begin(),rhs.m_strType.end());	
			m_strValue.assign(rhs.m_strValue.begin(),rhs.m_strValue.end());	
			m_strOutputName.assign(rhs.m_strOutputName.begin(),rhs.m_strOutputName.end());	
			m_szBuffer = new char[SPU_BUFFER_SIZE];
			string strAnsi = GetAnsi(m_strValue);
			if(strAnsi.size() > SPU_BUFFER_SIZE)
			{
				delete [] m_szBuffer;
				m_szBuffer = new char[strAnsi.size()+1];
				::strcpy_s(m_szBuffer, strAnsi.size()+1, rhs.m_szBuffer);
			}
			else
				::strcpy_s(m_szBuffer,SPU_BUFFER_SIZE,rhs.m_szBuffer);
			*this = rhs;
		}
		CFunctorParameter& operator=(const CFunctorParameter& rhs)
		{
			return *this;
		}
		operator char*()
		{
			return m_szBuffer;
		}
	protected:
		bool  SetValue(const TSTRING& strParameter,const TSTRING& strValue,vector<TSTRING>& strVars)
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
	private:
		TSTRING m_strType;
		TSTRING m_strValue;
		TSTRING m_strOutputName;
		char *m_szBuffer;
		vector<TSTRING>& m_strVars;
	};

	class CFunctor
	{
	public:
		CFunctor(const TSTRING& strAPIDLL,const TSTRING& strFunction,vector<CFunctorParameter>& vectParameter);
		CFunctor(const TSTRING& strAPIDLL,const TSTRING& strFunction,vector<CFunctorParameter>& vectParameter,ISPU_CALLBACK* pISPU_CALLBACK);
		virtual ~CFunctor(void);
		bool operator()();
		bool  run();
	private:
		TSTRING m_strAPIDLL;
		TSTRING m_strFunction;
		vector<CFunctorParameter>& m_vectParameter;
		HMODULE  m_hModule;
		FARPROC m_hProc;
		ISPU_CALLBACK* m_pISPU_CALLBACK;
		bool m_bUseCallback;
	};
}
