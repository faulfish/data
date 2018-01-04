#include "StdAfx.h"
#include "QMobileConfigure.h"
#include "QConfigXMLParser.h"

namespace QSYNC_LIB
{

	CQMobileConfigure *g_pSingleton = NULL;

	QCONFIGURE_CLASS_DLLEXT CQMobileConfigure* GetMobileConFig()
	{
		if(g_pSingleton == NULL)
			g_pSingleton = new CQMobileConfigure();
		return g_pSingleton;
	}

	QCONFIGURE_CLASS_DLLEXT void ReleaseMobileConFig()
	{
		if(g_pSingleton)
		{
			delete g_pSingleton;
			g_pSingleton = NULL;
		}
	}


	CQMobileConfigure::CQMobileConfigure(void):m_bLoaded(false)
	{
		m_pConfigParser = new CQConfigXMLParser();
#ifdef _DEBUG
#ifdef _UNICODE
		m_strDeafultPath = _T("..\\Debug\\Configuration\\QConfigure_UNICODE.xml");
#else
		m_strDeafultPath = _T("..\\Debug\\Configuration\\QConfigure_ANSI.xml");
#endif
#else
#ifdef _UNICODE
		m_strDeafultPath = _T("Configuration\\QConfigure_UNICODE.xml");
#else
		m_strDeafultPath = _T("Configuration\\QConfigure_ANSI.xml");
#endif
#endif
	}

	CQMobileConfigure::~CQMobileConfigure(void)		
	{
		if(m_pConfigParser)
		{
			delete m_pConfigParser;
			m_pConfigParser = NULL;
		}
		m_bLoaded = false;
	}

	bool CQMobileConfigure::LoadDefaultXML()
	{
		return LoadQConfigXML( m_strDeafultPath );
	}

	bool CQMobileConfigure::LoadQConfigXML( const TSTRING& strQConfigXMLPath )
	{
		bool  bResult = m_bLoaded;
		if(m_bLoaded == false)
		{
			CFile qConfigFile;			

			bResult = TRUE == qConfigFile.Open( strQConfigXMLPath.c_str(), CFile::modeRead);
			ULARGE_INTEGER MAX_CONFIG_FSIZE;
			MAX_CONFIG_FSIZE.QuadPart = qConfigFile.GetLength();

			ASSERT(bResult);
			if ( bResult && MAX_CONFIG_FSIZE.LowPart > 0)
			{
				TCHAR* cBuffer = new TCHAR[MAX_CONFIG_FSIZE.LowPart];
				::ZeroMemory(cBuffer,MAX_CONFIG_FSIZE.LowPart*sizeof(TCHAR));

				if (qConfigFile.Read( cBuffer, MAX_CONFIG_FSIZE.LowPart) > 0)
				{
					TCHAR* ctBuffer = cBuffer;
#ifdef _UNICODE
					ctBuffer += 1;//For avoid of header 0xFF 0xFE
#endif
					bResult = m_pConfigParser->LoadQConfigXML( ctBuffer );
					if(bResult)
						m_bLoaded = true;
				}
				qConfigFile.Close();

				delete [] cBuffer;
			}
		}
		return bResult; 
	}

	bool CQMobileConfigure::GetMobileSettings( const TSTRING& strMobID, CQSetting& oMobSetting )
	{
		return m_pConfigParser->GetMobileSettings( strMobID, oMobSetting ); 
	}

	bool CQMobileConfigure::GetSettings( const TSTRING& strAppSetting, CQSetting& oSettings )
	{
		return m_pConfigParser->GetSettings( strAppSetting, oSettings ); 
	}

}