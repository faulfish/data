// QisdaQualcommDownloadModule.h : main header file for the QisdaQualcommDownloadModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Download.h"
#include <map>


#pragma warning(push)
#pragma warning(disable:4251)


#ifdef QISDA_QUALCOMM_DOWNLOAD_MODULE_EXPORTS
#define QQDM_API __declspec(dllexport)
#else
#define QQDM_API __declspec(dllimport)
#endif
// CQisdaQualcommDownloadModuleApp
// See QisdaQualcommDownloadModule.cpp for the implementation of this class
//


/************************************************************************/

typedef struct
{
	TSTRING strFilePath;
	IMAGE imgData;
}DLType;

enum ImageCategory
{
	EMPTY,		// for empty file
	HEX,
	PARTITION,
	QCSBLHD,	// 6245
	QCSBL,		// 6245
	OEMSBLHD,	// 6245
	OEMSBL,		// 6245
	AMSSHD,		// 6245
	DBL,		// 6240 //6270
	FSBL,		// 6240 //6270
	OSBL,		// 6240 //6270
	AMSS,		
	CEFS
};
class CFilePathPolicy
{
public:
	CFilePathPolicy(ImageCategory imgCategory):m_imgCategory(imgCategory){}
	CFilePathPolicy(ImageCategory imgCategory, const TSTRING& strFilePath):m_imgCategory(imgCategory)
	{
		m_Data.strFilePath = strFilePath;
	}
	~CFilePathPolicy(){}
public:
	ImageCategory GetImageCategory(){return m_imgCategory;}
	void SetData(const TSTRING& strFilePath)
	{
		m_Data.strFilePath = strFilePath;
	}
	/*CFilePathPolicy& operator=( const CFilePathPolicy& file)
	{
	m_imgCategory = file.m_imgCategory;
	m_Data.strFilePath = file.m_Data.strFilePath;
	return *this;
	}*/

	DLType GetData(){return m_Data;}
	static bool DMSSOpenAndWriteHexFile(CDLFormatCommand* pDL, const DLType& DLData, WORD& wAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout)
	{
		return pDL->DMSSOpenAndWriteHexFile(DLData.strFilePath,wAddress,nRetryTimes,nTimeout);
	}
	static bool StreamingOpenAndWritePartition(CDLFormatCommand* pDL, const DLType& DLData, bool bOverride, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout)
	{
		return pDL->StreamingOpenAndWritePartition(DLData.strFilePath, bOverride, bSpeedUp, nRetryTimes, nTimeout);
	}
	static bool StreamingDLMultiImage(CDLFormatCommand* pDL, BYTE nImageType, const DLType& DLHeader, const DLType& DLData, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout) //Open(Header)+Write(Image)+Close
	{
		return pDL->StreamingDLMultiImage(nImageType, DLHeader.strFilePath, DLData.strFilePath, bSpeedUp, nRetryTimes, nTimeout);
	}
	static bool StreamingDLMultiImage(CDLFormatCommand* pDL, BYTE nImageType, const DLType& DLData, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout) //Open+Write(Image)+Close
	{
		return pDL->StreamingDLMultiImage(nImageType, DLData.strFilePath, bSpeedUp, nRetryTimes, nTimeout);
	}
private:
	ImageCategory m_imgCategory;
	DLType m_Data;
};


class CShareFilePolicy
{
public:
	CShareFilePolicy(ImageCategory imgCategory)
		:m_imgCategory(imgCategory)
	{
		m_Data.imgData.pData = NULL;
		m_Data.imgData.nSize = 0;
	}
	CShareFilePolicy(ImageCategory imgCategory, const BYTE* pData, int nDataSize)
		:m_imgCategory(imgCategory)
	{
		m_Data.imgData.pData = (BYTE*)pData;
		m_Data.imgData.nSize = nDataSize;
	}
	~CShareFilePolicy(){}
public:
	ImageCategory GetImageCategory(){return m_imgCategory;}
	void SetData(const BYTE* pData, int nDataSize)
	{
		m_Data.imgData.pData = (BYTE*)pData;
		m_Data.imgData.nSize = nDataSize;
	}

	/*CShareFilePolicy& operator=( const CShareFilePolicy& file)
	{
	m_imgCategory = file.m_imgCategory;
	m_Data.imgData.pData = (BYTE*)file.m_Data.imgData.pData;
	m_Data.imgData.nSize = file.m_Data.imgData.nSize;
	return *this;
	}*/

	DLType GetData(){return m_Data;}
	static bool DMSSOpenAndWriteHexFile(CDLFormatCommand* pDL, const DLType& DLData, WORD& wAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout)
	{
		return pDL->DMSSOpenAndWriteHexFile(DLData.imgData.pData, DLData.imgData.nSize,wAddress,nRetryTimes,nTimeout);
	}
	static bool StreamingOpenAndWritePartition(CDLFormatCommand* pDL, const DLType& DLData, bool bOverride, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout)
	{
		return pDL->StreamingPartition(DLData.imgData.pData, DLData.imgData.nSize, bOverride, bSpeedUp, nRetryTimes, nTimeout);
	}
	static bool StreamingDLMultiImage(CDLFormatCommand* pDL, BYTE nImageType, const DLType& DLHeader, const DLType& DLData, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout) //Open(Header)+Write(Image)+Close
	{
		return pDL->StreamingDLMultiImage(nImageType, DLHeader.imgData.pData, DLHeader.imgData.nSize, DLData.imgData.pData, DLData.imgData.nSize, bSpeedUp, nRetryTimes, nTimeout);
	}
	static bool StreamingDLMultiImage(CDLFormatCommand* pDL, BYTE nImageType, const DLType& DLData, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout) //Open+Write(Image)+Close
	{
		return pDL->StreamingDLMultiImage(nImageType, DLData.imgData.pData, DLData.imgData.nSize, bSpeedUp, nRetryTimes, nTimeout);
	}

private:
	ImageCategory m_imgCategory;
	DLType m_Data;
};

class CQualcomm6240Policy: private CInterfaceBase
{
public:
	static TSTRING GetPlatfromName()
	{
		return _T("Qualcomm6240");
	}
	static bool BeforeStreamingDL(CDLFormatCommand* pStdDL)
	{
		Sleep(6000);
		CReOpenComport ReopenAfterGo(pStdDL, true);
		return ReopenAfterGo.ReOepnComport();
	}
	static bool AfterStreamingDL(CDLFormatCommand* pStdDL)
	{
		Sleep(10000);
		CReOpenComport ReopenAfterReset(pStdDL);
		return ReopenAfterReset.ReOepnComport();
	}
};

class CQualcomm6245Policy : private CInterfaceBase
{
public:
	static TSTRING GetPlatfromName()
	{
		return _T("Qualcomm6245");
	}
	static bool BeforeStreamingDL(CDLFormatCommand* pStdDL){return true;}
	static bool AfterStreamingDL(CDLFormatCommand* pStdDL)
	{
		Sleep(10000);
		CReOpenComport ReopenAfterReset(pStdDL);
		return ReopenAfterReset.ReOepnComport();
	}
};

template<class CImagePolicy>
class IMAGE_LOAD
{
public:
	IMAGE_LOAD():fHeader(EMPTY), fBody(EMPTY){}
public:
	BYTE nImageType;
	CImagePolicy fHeader;
	CImagePolicy fBody;
};

template<class CImagePolicy>
class CDLParameter
{
public:
	CDLParameter()
		:m_bEmpty(true), m_bOnlyDLDMSS(false), m_bSpeedUp(true), m_bOverridePartition(false), m_fHex(EMPTY),m_fPartition(EMPTY) {}
	CDLParameter(const CImagePolicy& fHex)
		:m_bEmpty(false),m_bOnlyDLDMSS(false), m_bSpeedUp(true), m_bOverridePartition(false), m_fHex(fHex),m_fPartition(EMPTY) {}
	CDLParameter(const CImagePolicy& fHex, const CImagePolicy& fPartition, bool bSpeedUp = true, bool bOverridePartition=false)
		:m_bEmpty(false),m_bOnlyDLDMSS(true), m_bSpeedUp(bSpeedUp), m_bOverridePartition(bOverridePartition),m_fHex(fHex), m_fPartition(fPartition){}
	~CDLParameter(){}
public:
	bool SetDLImage(CImagePolicy& fHeader, CImagePolicy& fData)
	{
		bool bRes = false;
		if(!m_bEmpty)
		{
			if(fHeader.GetImageCategory()==QCSBLHD && fData.GetImageCategory()==QCSBL)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoadHear;
				ImageLoadHear.nImageType = OPEN_MULTI_MODE_QCSBLHDCFG;
				ImageLoadHear.fBody = fHeader;
				m_mapDLImage[QCSBLHD] = ImageLoadHear;

				IMAGE_LOAD<CImagePolicy> ImageLoadBody;
				ImageLoadBody.nImageType = OPEN_MULTI_MODE_QCSBL;
				ImageLoadBody.fBody = fData;
				m_mapDLImage[QCSBL] = ImageLoadBody;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==OEMSBLHD && fData.GetImageCategory()==OEMSBL)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_OEMSBL;
				ImageLoad.fHeader = fHeader;
				ImageLoad.fBody = fData;
				m_mapDLImage[OEMSBLHD] = ImageLoad;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==AMSSHD && fData.GetImageCategory()==AMSS)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_AMSS;
				ImageLoad.fHeader = fHeader;
				ImageLoad.fBody = fData;
				m_mapDLImage[AMSSHD] = ImageLoad;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==DBL || fData.GetImageCategory()==DBL)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_DBL;
				ImageLoad.fBody = fHeader.GetImageCategory()==DBL ? fHeader : fData;
				m_mapDLImage[DBL] = ImageLoad;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==FSBL || fData.GetImageCategory()==FSBL)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_FSBL;
				ImageLoad.fBody = fHeader.GetImageCategory()==FSBL ? fHeader : fData;
				m_mapDLImage[FSBL] = ImageLoad;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==OSBL || fData.GetImageCategory()==OSBL)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_OSBL;
				ImageLoad.fBody = fHeader.GetImageCategory()==OSBL ? fHeader : fData;
				m_mapDLImage[OSBL] = ImageLoad;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==AMSS || fData.GetImageCategory()==AMSS)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_AMSS;
				ImageLoad.fBody = fHeader.GetImageCategory()==AMSS ? fHeader : fData;
				m_mapDLImage[AMSS] = ImageLoad;
				bRes = true;
			}
			else if(fHeader.GetImageCategory()==CEFS || fData.GetImageCategory()==CEFS)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad;
				ImageLoad.nImageType = OPEN_MULTI_MODE_CEFS;
				ImageLoad.fBody = fHeader.GetImageCategory()==CEFS ? fHeader : fData;
				m_mapDLImage[CEFS] = ImageLoad;
				bRes = true;
			}
		}
		return bRes;
	}
public:
	bool IsEmpty() {return m_bEmpty;}
	bool IsOnlyDLDMSS() {return m_bOnlyDLDMSS; }
	bool IsSpeedUp() {return m_bSpeedUp;}
	bool IsOverridePartition() {return m_bOverridePartition;}
	bool GetHex(CImagePolicy& fImage)
	{
		fImage = m_fHex;
		return m_fHex.GetImageCategory() != EMPTY;
	}
	bool GetPartition(CImagePolicy& fImage)
	{
		fImage = m_fPartition;
		return m_fHex.GetImageCategory() != EMPTY;
	}
	bool GetDownloadImages(std::map<ImageCategory, IMAGE_LOAD<CImagePolicy> >& mapDLImage)
	{
		mapDLImage = m_mapDLImage;
		return m_mapDLImage.size()!=0;
	}
private:
	bool m_bEmpty; // for empty constructor usage
	bool m_bOnlyDLDMSS;
	bool m_bSpeedUp;
	bool m_bOverridePartition;
	CImagePolicy m_fHex;
	CImagePolicy m_fPartition;
	std::map< ImageCategory , IMAGE_LOAD<CImagePolicy> > m_mapDLImage;
};


template <class CPlatformPolicy, class CImagePolicy>
class CStandardDownload: public CDLFormatCommand, public CRefObject
{
public:
	CStandardDownload(int nCOMPort)
		:CDLFormatCommand(nCOMPort, CPlatformPolicy::GetPlatfromName().c_str()){}

	virtual ~CStandardDownload(){}

public:
	bool Download(const CDLParameter<CImagePolicy>& parameter)
	{
		bool bRes = false;
		m_Parameter = parameter;
		if(!m_Parameter.IsEmpty() && DMSSDownload())
		{
			if(m_Parameter.IsOnlyDLDMSS())
				bRes = true;
			else
				bRes = StreamingDownload();
		}
		return bRes;
	}
private:
	bool DMSSDownload()
	{
		using namespace PLATFORM_TOOL;
		bool bRes = false;

		WORD dwAddress=0;
		CImagePolicy fHex(HEX);

		if(	m_Parameter.GetHex(fHex) && 
			DMSSNop(20,1000) && 
			DMSSParameter() &&
			CImagePolicy::DMSSOpenAndWriteHexFile(this, fHex.GetData(), dwAddress) &&
			DMSSGo(dwAddress, 0))
		{
			bRes = true;
		}
		return bRes;
	}
	bool StreamingDownload()
	{
		using namespace PLATFORM_TOOL;
		bool bRes = false;

		bRes = CPlatformPolicy::BeforeStreamingDL((CDLFormatCommand*)this);

		std::map<ImageCategory, IMAGE_LOAD<CImagePolicy> > mapDLImage;
		CImagePolicy fPartition(PARTITION);
		if(bRes && 
			m_Parameter.GetPartition(fPartition) &&
			m_Parameter.GetDownloadImages(mapDLImage) && 
			StreamingHello(m_Parameter.IsSpeedUp()) &&
			StreamingSecurity(_T(""), m_Parameter.IsSpeedUp()) && 
			CImagePolicy::StreamingOpenAndWritePartition(this, fPartition.GetData(), m_Parameter.IsOverridePartition(), m_Parameter.IsSpeedUp()))
		{
			bRes = true;
			for(std::map<ImageCategory, IMAGE_LOAD<CImagePolicy> >::iterator iter= mapDLImage.begin(); iter!= mapDLImage.end() && bRes; iter++)
			{
				IMAGE_LOAD<CImagePolicy> ImageLoad = iter->second;
				if(CPlatformPolicy::GetPlatfromName() == _T("Qualcomm6240"))
					bRes = CImagePolicy::StreamingDLMultiImage(this, ImageLoad.nImageType, ImageLoad.fHeader.GetData(), ImageLoad.fBody.GetData(), m_Parameter.IsSpeedUp());
				else if(CPlatformPolicy::GetPlatfromName() == _T("Qualcomm6245"))
					bRes = CImagePolicy::StreamingDLMultiImage(this, ImageLoad.nImageType, ImageLoad.fBody.GetData(), m_Parameter.IsSpeedUp());
			}
			if(bRes)
			{
				bRes = StreamingReset(m_Parameter.IsSpeedUp());
				if(bRes)
				{
					CPlatformPolicy::AfterStreamingDL(this);
				}
			}
		}
		return bRes;
	}
private:
	CDLParameter<CImagePolicy> m_Parameter;
};



typedef CRefObjectPtr< CStandardDownload<CQualcomm6240Policy,CFilePathPolicy> > CFCDL6240Ptr;

/*
Sample:
CFilePathPolicy fHex(HEX, _T("xxhex.mbn"));
CFilePathPolicy fPartition(PARTITION, _T("xxPartition.mbn"));
CDownloadParameter<CFilePathPolicy> dlParameters(fHex, fPartition);

CStandardDownload<CQualcomm6245Policy,CFilePathPolicy> downloadTool(99);
downloadTool.download(dlParameters);
*/
//////////////////////////////////////////////////////////////////////////
/************************************************************************
Class: CDownloadDumpImage
Purpose: For Driver team test
- StreamingWriteMultiImage writes 512 bytes every time 
  (Please change DownloadRequest to StreamingStrWriteDumpReq while writing)
************************************************************************/
class QISDA_DOWNLOAD_CLASS_API CDownloadDumpImage: public CDLFormatCommand
{
public:
	CDownloadDumpImage(int nComPort, const std::string& strPlatform="Qualcomm6245")
		:CDLFormatCommand(nComPort, strPlatform), m_dwStartAddress(0){}
	virtual ~CDownloadDumpImage(){}
public:
	bool SetHeader(const TSTRING& strHeader);
	bool SetBody(const TSTRING& strBody);
	void SetStartAddress(DWORD dwStartAddress);
	bool SetHex(const std::string& strFilePath);
public:
	bool Download( BYTE nImageType);
	bool Download6240(BYTE nImageType);
	bool DMSSDownload(const std::string& strHexFilePath = "" /*"" means to use m_strHEX*/);
	bool StreamingDL( BYTE nImageType, bool bSpeedUp = true);
	bool StreamingDL6240( BYTE nImageType, bool bSpeedUp = true);
	virtual bool StreamingWriteMultiImage(const TSTRING& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout); // return XOR[4]
	bool StreamingWriteMultiImageOrg(const TSTRING& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout); // return XOR[4]
private:
	bool ShouldBeSkiped(BYTE* nBuffer,WORD wReadSize);
private:
	DWORD m_dwStartAddress; // for WriteImage
	TSTRING m_strHeader;
	TSTRING m_strBody;
	std::string m_strHEX;
};

/************************************************************************/

typedef bool(*CallBackInfo)(const char*, int , int);
class CDLCallBack : public QSYNC_LIB::INotify_ANSI
{
public:
	CDLCallBack(bool(*CallBack)(const char*, int , int)):m_CallBack(CallBack){}
	virtual ~CDLCallBack(){}
public:
	virtual void Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == EVENT_DL_PROGRESS)
		{
			AddMsg(((DL_PROGRESS*)nParam)->szMsg, ((DL_PROGRESS*)nParam)->nStep, ((DL_PROGRESS*)nParam)->nProgress);
		}
	}
	bool AddMsg(const char* szMsg, int nStep, int nProgress)
	{
		bool bRes = false;
		if(m_CallBack)
		{
			bRes = true;
			m_CallBack(szMsg, nStep, nProgress);
		}
		ASSERT(bRes);
		return bRes;	
	}
private:
	CallBackInfo m_CallBack;
};
/************************************************************************/
struct DLSetting6245
{
	char szHex[256];
	char szPartition[256];
	char szQcsbl[256];
	char szQcsblhd[256];
	char szOemsbl[256]; 
	char szOemsblhd[256];
	char szAmss[256]; 
	char szAmsshd[256]; 
	char szCefs[256];
};

QISDA_DOWNLOAD_API bool Download6245(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6245& Path);


/************************************************************************/
struct DLSetting6240
{
	char szHex[256];
	char szPartition[256];
	char szDBL[256];
	char szFSBL[256];
	char szOSBL[256];
	char szAmss[256]; 
	char szCefs[256];
};
QISDA_DOWNLOAD_API bool Download6240(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6240& Path);

struct DLSetting6270
{
	char szHex[256];
	char szPartition[256];
	char szDBL[256];
	char szFSBL[256];
	char szOSBL[256];
	char szAmss[256]; 
	char szCefs[256];
};
QISDA_DOWNLOAD_API bool Download6270(long nComPort, bool(*CallBack)(const char*, int , int), const DLSetting6270& Path);

#pragma warning(pop)