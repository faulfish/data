/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
*
*               BenQ QSyncher Project
*
*	   Copyright (C) 2005 BenQ Corp.
*
***************************************************************************
*
*  $Workfile:   DropFiles.cpp  $
*  $Revision:   1.3  $
*  $Date:   Jan 08 2008 15:22:52  $ 
*
***************************************************************************
*
* File Description
* ----------------
* 
*
***************************************************************************
*
* Revision Details
* ----------------
***************************************************************************/
/*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/CommonUti/DropFiles.cpp.-arc  $
   
      Rev 1.3   Jan 08 2008 15:22:52   Alan Tu
   update

Rev 1.2   Nov 20 2007 13:15:52   Alan Tu
update unicode string function

Rev 1.1   Mar 16 2007 19:17:10   Alan Tu
update

Rev 1.0   Mar 16 2007 08:47:22   Alan Tu
Initial revision.

Rev 1.0   Mar 02 2005 18:53:02   Neko Hsieh
Initial revision.
*/

/**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */
#include "stdafx.h"
#include "DropFiles.h"
#include "UnicodeUtility.h"

CDropFiles::CDropFiles():m_hMemData(NULL)
{
}
CDropFiles::~CDropFiles()
{
}
void CDropFiles::AddFile(const CString &sFile)
{
	m_aFiles.Add(sFile);
}
void CDropFiles::CreateBuffer()
{
	using namespace QSYNC_LIB;
	int nBuffSize = sizeof(DROPFILES)+2;	

	for(int i=0;i<m_aFiles.GetSize();i++)
		nBuffSize += (m_aFiles[i].GetLength()+1)*sizeof(TCHAR);

	nBuffSize = (nBuffSize/32 + 1)*32;

	if(m_hMemData)
		::GlobalFree(m_hMemData);

	m_hMemData  =   ::GlobalAlloc(GPTR,nBuffSize);
	LPDROPFILES lpDropFiles   =   (LPDROPFILES)::GlobalLock(m_hMemData);

	::ZeroMemory(lpDropFiles, nBuffSize);

	lpDropFiles->pFiles   =   sizeof(DROPFILES);   
#ifdef _UNICODE
	lpDropFiles->fWide = true;
#else
	lpDropFiles->fWide = false;
#endif
	TCHAR *pszStart   =   (TCHAR*)((LPBYTE)lpDropFiles   +   sizeof(DROPFILES));	
	for(int nIndex = 0;nIndex < m_aFiles.GetSize();nIndex++)
	{
		::_tcscpy(pszStart, m_aFiles[nIndex]);  
		pszStart   =   _tcschr(pszStart,_T('\0'))+   1;
	}

	::GlobalUnlock(m_hMemData);
}

HGLOBAL CDropFiles::GetBuffer() const
{
	return m_hMemData;
}

