// QisdaQualcommDownloadModule.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QisdaQualcommDownloadModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

bool CDownloadDumpImage::Download(BYTE nImageType)
{
	bool bRes = false;
	if(DMSSDownload() 
	   && StreamingDL(nImageType, true))
	{
		bRes = true;
	}
	return bRes;
}

bool CDownloadDumpImage::DMSSDownload(const std::string& strHexFilePath)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6240NopRetryTimes,g_n6240NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( (strHexFilePath.empty() ? m_strHEX : strHexFilePath), dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}
bool CDownloadDumpImage::Download6240(BYTE nImageType)
{
	bool bRes = false;
	if(DMSSDownload() 
		&& StreamingDL6240(nImageType, true))
	{
		bRes = true;
	}
	return bRes;
}
bool CDownloadDumpImage::StreamingDL(BYTE nImageType, bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	Sleep(8000);

	if(StreamingHello(bSpeedUp) &&
		StreamingSecurity(_T(""), bSpeedUp) && 
	//	StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingWriteMultiImage(m_strBody, bSpeedUp) &&
		StreamingReset(bSpeedUp))
	{
		Sleep(10000);
		CReOpenComport ReopenAfterReset(this);
		bRes = ReopenAfterReset.ReOepnComport();
	}	
	return bRes;
}
bool CDownloadDumpImage::StreamingDL6240( BYTE nImageType, bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = ReopenAfterGo.ReOepnComport(g_n6240ReOpenAfterGoSleepTime, false);
	AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	ASSERT(bReconnect);

	if(StreamingHello(bSpeedUp) &&
		StreamingSecurity(_T(""), bSpeedUp) && 
		//	StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingWriteMultiImageOrg(m_strBody, bSpeedUp) && // no open and close
		StreamingReset(bSpeedUp))
	{
		
		CReOpenComport ReopenAfterReset(this);
		bReconnect = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime_Path, false);
		ASSERT(bReconnect);
		AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
		SetLastError(DL_ERROR_ReOpenComport, bReconnect);
	
	}	
	return bRes;
}
bool CDownloadDumpImage::StreamingWriteMultiImage(const TSTRING& strImagePath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingWriteMultiImage", StreamingProtocol, 0);
	CFile cfImage;
	CFileException pError;
	if(cfImage.Open(strImagePath.c_str(),CFile::shareDenyNone | CFile::modeRead,&pError) == TRUE)
	{
		int nImageSize = cfImage.GetLength();
		Struct_XOR XOR={0};
		int nProgressCount = 0;
		if(nImageSize > 0)
		{
			DWORD dwAddress = m_dwStartAddress;
			WORD wReadSize = 0;
			do {
				BYTE nTemp[gc_nDumpImagePageSize+32] = {0};
				wReadSize = cfImage.Read(nTemp,gc_nDumpImagePageSize);
				if(!ShouldBeSkiped(nTemp, wReadSize))
				{
					if(bSpeedUp)
						bRes = WriteDumpImage512(nTemp, wReadSize, dwAddress, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
					else
						bRes = false;
				}
				else
					bRes = true;

				dwAddress += 1;
				Sleep(0); // for multi-download

				nProgressCount++;
				//AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*nProgressCount/(nImageSize/gc_nDumpImagePageSize+1));
			}while( wReadSize == gc_nDumpImagePageSize && bRes );
		}
		cfImage.Close();
	}
	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	return bRes;
}

//************************************
// Method:    StreamingWriteMultiImageOrg
// FullName:  CDownloadDumpImage::StreamingWriteMultiImageOrg
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: const TSTRING & strImagePath
// Parameter: bool bSpeedUp
// Parameter: int nRetryTimes
// Parameter: int nTimeout
// Same as CDLCompoundCommand::StreamingWriteMultiImage, 
// the difference is DWORD dwAddress = m_dwStartAddress; and skip 0xbb, 0xcc
//************************************
bool CDownloadDumpImage::StreamingWriteMultiImageOrg(const TSTRING& strImagePath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingWriteMultiImage", StreamingProtocol, 0);
	CFile cfImage;
	CFileException pError;
	if(cfImage.Open(strImagePath.c_str(),CFile::shareDenyNone | CFile::modeRead,&pError) == TRUE)
	{
		int nImageSize = cfImage.GetLength();
		Struct_XOR XOR={0};
		int nProgressCount = 0;
		if(nImageSize > 0)
		{
			DWORD dwAddress = m_dwStartAddress;
			WORD wReadSize = 0;
			do {
				BYTE nTemp[PAGE_SIZE+32] = {0};
				wReadSize = cfImage.Read(nTemp,PAGE_SIZE);
				if(!ShouldBeSkiped(nTemp, wReadSize))
				{
					if(bSpeedUp)
						bRes = WriteDumpImage(nTemp, wReadSize, dwAddress, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
					else
						bRes = false;
				}
				else
					bRes = true;

				dwAddress += 1;
				Sleep(0); // for multi-download

				nProgressCount++;
				//AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*nProgressCount/(nImageSize/gc_nDumpImagePageSize+1));
			}while( wReadSize == PAGE_SIZE && bRes );
		}
		cfImage.Close();
	}
	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	return bRes;
}

bool CDownloadDumpImage::ShouldBeSkiped(BYTE* nBuffer,WORD wReadSize)
{
	bool bSkip = false;
	if(nBuffer)
	{
		int nScanbSize = 0;
		int nScancSize = 0;
		for(int n=0; n < wReadSize; n++)
		{
			if(*(nBuffer +n) == 0xbb)
				nScanbSize++;
			else if(*(nBuffer +n) == 0xcc)
				nScancSize++;
			else
				break;
		}
		bSkip = nScanbSize == wReadSize;
		if(!bSkip)
			bSkip = nScancSize == wReadSize;
		ASSERT(bSkip==false);
	}
	return bSkip;
}

void CDownloadDumpImage::SetStartAddress(DWORD dwStartAddress)
{
	m_dwStartAddress = dwStartAddress;
}

bool CDownloadDumpImage::SetHeader(const TSTRING& strHeader)
{
	bool bRes = false;
	if(::_taccess(strHeader.c_str(), 0) == 0)
	{
		m_strHeader = strHeader;
		bRes = true;
	}
	return bRes;
}
bool CDownloadDumpImage::SetBody(const TSTRING& strBody)
{
	bool bRes = false;
	if(::_taccess(strBody.c_str(), 0) == 0)
	{
		m_strBody = strBody;
		bRes = true;
	}
	return bRes;
}

bool CDownloadDumpImage::SetHex(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strHEX = strFilePath;
		bRes = true;
	}
	return bRes;
}
/************************************************************************/
bool Download6245(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6245& Path)
{
	bool bRes = false;
	CDLCallBack DLCallback(CallBack);
	CDownload6245 dl(nComPort);
	dl.Register(&DLCallback, EVENT_DL_PROGRESS);
	//Path Setting
	if(dl.SetHex(Path.szHex) && dl.SetPartition(Path.szPartition))
	{
		dl.SetQCSBLHD(Path.szQcsblhd);
		dl.SetQCSBL(Path.szQcsbl);
		dl.SetOEMSBLHD(Path.szOemsblhd);
		dl.SetOEMSBL(Path.szOemsbl);
		dl.SetAMSSHD(Path.szAmsshd);
		dl.SetAMSS(Path.szAmss);
		dl.SetCEFS(Path.szCefs);
		// Download
		bRes = dl.Download(true);
	}
	else
	{
		CallBack("Path Setting Error.", 1, 0);
	}
	return bRes;
}
/************************************************************************/
bool Download6240(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6240& Path)
{
	bool bRes = false;
	CDLCallBack DLCallback(CallBack);
	CDownload6240 dl(nComPort);
	dl.Register(&DLCallback, EVENT_DL_PROGRESS);
	//Path Setting
	if(dl.SetHex(Path.szHex) && dl.SetPartition(Path.szPartition))
	{
		dl.SetDBL(Path.szDBL);
		dl.SetFSBL(Path.szFSBL);
		dl.SetOSBL(Path.szOSBL);
		dl.SetAMSS(Path.szAmss);
		dl.SetCEFS(Path.szCefs);
		// Download
		bRes = dl.Download(true);
	}
	else
	{
		CallBack("Path Setting Error.", 1, 0);
	}
	return bRes;
}
/************************************************************************/
bool Download6270(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6270& Path)
{
	bool bRes = false;
	CDLCallBack DLCallback(CallBack);
	CDownload6270 dl(nComPort);
	dl.Register(&DLCallback, EVENT_DL_PROGRESS);
	//Path Setting
	if(dl.SetHex(Path.szHex) && dl.SetPartition(Path.szPartition))
	{
		dl.SetDBL(Path.szDBL);
		dl.SetFSBL(Path.szFSBL);
		dl.SetOSBL(Path.szOSBL);
		dl.SetAMSS(Path.szAmss);
		dl.SetCEFS(Path.szCefs);
		// Download
		bRes = dl.Download(true);
	}
	else
	{
		CallBack("Path Setting Error.", 1, 0);
	}
	return bRes;
}