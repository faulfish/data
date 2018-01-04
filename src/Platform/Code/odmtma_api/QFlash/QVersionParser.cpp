#include "stdafx.h"
#include "QVersionParser.h"

#include <fstream>
#include <io.h>
using namespace std;
using namespace QSYNC_LIB;

namespace ODMTMA_LIB
{
	CQVersionParser::CQVersionParser(){}
	CQVersionParser::~CQVersionParser(){}

	bool CQVersionParser::IsCodeSigned(const TSTRING& strAmss, const TSTRING& strOemsbl, const TSTRING& strQcsbl)
	{
		bool bRes = false;
		{
			CSingleLock singlelock(&m_CritAMSS, TRUE);
			bRes = (SearchWord(strAmss, _T("QUALCOMM Attestation CA"),4096) >=0);
		}

		if(bRes)
		{
			CSingleLock singlelock(&m_CritOemsbl, TRUE);
			bRes = (SearchWord(strOemsbl, _T("QUALCOMM Attestation CA")) >=0);
		}
		if(bRes)
		{
			CSingleLock singlelock(&m_CritQcsbl, TRUE);
			bRes = (SearchWord(strQcsbl, _T("QUALCOMM Attestation CA")) >=0);
		}
		return bRes;
	}
	bool CQVersionParser::IsCodeSigned(const TSTRING& strAmss, const TSTRING& strOemsbl)
	{
		bool bRes = false;
		{
			CSingleLock singlelock(&m_CritAMSS, TRUE);
			bRes = (SearchWord(strAmss, _T("QUALCOMM Attestation CA"),4096) >=0);
		}
			
		if(bRes)
		{
			CSingleLock singlelock(&m_CritOemsbl, TRUE);
			bRes = (SearchWord(strOemsbl, _T("QUALCOMM Attestation CA")) >=0);
		}
		return bRes;
	}

	int CQVersionParser::GetPhoneCodeVersion(const TSTRING& strAmss)
	{
		int nRet = -1;
		{
			CSingleLock singlelock(&m_CritAMSS, TRUE);
			nRet = GetVersion(strAmss, _T("O'zapft"));
		}
		return nRet;
	}
	int CQVersionParser::GetBootloaderVersion(const TSTRING& strOemsbl)
	{
		int nRet = -1;
		{
			CSingleLock singlelock(&m_CritOemsbl, TRUE);
			nRet = GetVersion(strOemsbl, _T("O'zbplt"));
		}
		return nRet;
	}

	int CQVersionParser::GetVersion(const TSTRING& strFilePath, const TSTRING& strSearchWord)
	{
		int nRet = -1;
		int nLocation = SearchWord(strFilePath, strSearchWord);
		if(nLocation >= 0)
		{
			CFile file(strFilePath.c_str(), CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone);
			int nReadLength = file.GetLength() > (nLocation+100) ? nLocation+100 : file.GetLength();
			int nInterval = nReadLength - nLocation;

			BYTE* pszFileBuf = new BYTE[nReadLength];
			int nLength = file.Read(pszFileBuf, nReadLength);
			BYTE* pszTemp = new BYTE[nInterval];
			memcpy(pszTemp, &(pszFileBuf[nLocation]), nInterval);
			BYTE szVersion[5] = {0};
			int nCount =0, nVersionSize = 0;
			for(int i=0; i< nInterval; i++)
			{
				if(pszTemp[i] == ';')
					nCount++;
				if(nCount == 5)
				{
					szVersion[nVersionSize] = pszTemp[i];
					nVersionSize++;
				}
			}
			szVersion[0] = '0';
			nRet = _ttoi((const char*)szVersion);
			delete [] pszTemp;
			delete [] pszFileBuf;
			file.Close();
		}
		return nRet;
	}


} // namespace ODMTMA_LIB