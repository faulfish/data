#include "stdafx.h"
#include "CQCNFile.h"

#define FILE_NAME_MAX_LEN		1024
#define NV_CONTENT_FIXED_LEN	128

CQCNFile::CQCNFile()
:m_pRoot(NULL)
,m_pModelType(NULL)
,m_pConfig(NULL)
,m_pNVStorage(NULL)
{
	::OleInitialize(NULL);
	pStg = NULL;	
	pSub1 = NULL;	
	pSub2 = NULL;	
	pSub3 = NULL;	
	pStm = NULL;	
	pStmFile = NULL;
}

CQCNFile::~CQCNFile()
{
	Close();
	::OleUninitialize();
}

int CQCNFile::Open(LPCTSTR szFileName, UINT nFlag)
{
	int nRes = NO_ERROR;
	if(SetFileName(szFileName))
	{
		if ( nFlag & CFile::modeCreate || nFlag & CFile::modeWrite)
		{
			nRes = CQCN_FILE_ERROR_NOT_SUPPORT;
		}
		else
		{
			DWORD grfMode = nFlag & CFile::modeWrite ? STGM_READWRITE : STGM_READ;
			m_bstrFileName = GetFileName();
			HRESULT hr = ::StgOpenStorage((LPCWSTR) m_bstrFileName,NULL,grfMode|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,NULL,0, &m_pRoot);
			if(hr != S_OK)
				nRes = CNV_ITEM_FILE_ERROR_FILE_OPERATOR;
		}
	}
	else
	{
		nRes = CNV_ITEM_FILE_ERROR_FILE_NAME;
	}

	if(nRes == NO_ERROR)
	{		
		nRes = OpenHeader();
		if(NO_ERROR != nRes)
			printf("Conversion failed with error: 0x%X", nRes);
	}

	if(nRes == NO_ERROR)
	{		
		nRes = OpenNVStorage();
		if(NO_ERROR != nRes)
			printf("Conversion failed with error: 0x%X", nRes);
	}

	if(nRes == NO_ERROR) //Get qcn version
	{
		_bstr_t versionfilename = _T("File_Version");
		CComPtr<IStream> pStream;
		HRESULT hr = m_pRoot->OpenStream((LPCWSTR)versionfilename, NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, &pStream);
		if(hr != S_OK || pStream == NULL)
			nRes = CNV_ITEM_FILE_ERROR_FILE_OPERATOR;
		if(nRes == NO_ERROR)
		{
			ULONG ulen;
			pStream->Read(&m_wQCNVersion, sizeof(WORD), &ulen);
		}
	}

	return nRes;
}

int CQCNFile::OpenHeader()
{
	int nRet = NO_ERROR;

	nRet = OpenModel();
	if ( nRet == NO_ERROR )
		nRet = OpenConfig();

	return nRet;
}

int CQCNFile::OpenModel()
{
	// Make sure that root is open
	if ( NULL == m_pRoot)
		return CQCN_FILE_ERROR_OPEN_MODEL;

	HRESULT hr = S_OK;
	CComPtr<IEnumSTATSTG> lpEnum = NULL;
	STATSTG sstg = {0};

	hr = m_pRoot->EnumElements(0, NULL, 0, &lpEnum);

	if(hr != S_OK)
	{
		lpEnum = NULL; 
		return CQCN_FILE_ERROR_OPEN_MODEL;
	}

	hr = lpEnum->Next(1, &sstg, NULL);
	if(hr != S_OK)
	{
		lpEnum = NULL; 
		return CQCN_FILE_ERROR_OPEN_MODEL;
	}

	// Open model storage
	_bstr_t bstrModelName;
	if(sstg.type == STGTY_STORAGE)  
		bstrModelName = sstg.pwcsName; 

	lpEnum = NULL; 

	hr = m_pRoot->OpenStorage((LPCWSTR)bstrModelName,NULL,STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
		NULL,0,&m_pModelType);

	if ( S_OK != hr ){
		return CQCN_FILE_ERROR_OPEN_MODEL;
	}
	return NO_ERROR;

}

int CQCNFile::OpenConfig()
{
	// Make sure that config is opened.
	
	if ( NULL == m_pModelType)
		return CQCN_FILE_ERROR_OPEN_MODEL;
	
	HRESULT hr = S_OK;
	CComPtr<IEnumSTATSTG> lpEnum = NULL;
	STATSTG sstg = {0};
	_bstr_t bstrConfigName;

	hr = m_pModelType->EnumElements(0, NULL, 0, &lpEnum); 

    if(hr != S_OK)
	{
		lpEnum = NULL; 
		return CQCN_FILE_ERROR_OPEN_CONFIG;
	}

	hr = lpEnum->Next(1, &sstg, NULL);
	if(hr != S_OK)
	{
		lpEnum = NULL; 
		return CQCN_FILE_ERROR_OPEN_CONFIG;
	}

	if(sstg.type == STGTY_STORAGE)  
		bstrConfigName = sstg.pwcsName; 

	lpEnum = NULL;

	hr = m_pModelType->OpenStorage((LPCWSTR)bstrConfigName,NULL,STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
		NULL,0,&m_pConfig);

	if ( S_OK != hr){
		return CQCN_FILE_ERROR_OPEN_CONFIG;
	}

	return NO_ERROR;

}

int CQCNFile::GetNVItem(std::vector<CNVItemData>& NVItemArray) // for QCN 2
{
	HRESULT hr = S_OK;
	int nRes = NO_ERROR;

	CComPtr<IStorage>	pNV;
	CComPtr<IStream> pNVContent = NULL;

	if(nRes == NO_ERROR)
	{
		_bstr_t bstrNVID = _T("NV_NUMBERED_ITEMS");
		hr = m_pConfig->OpenStorage((LPCWSTR) bstrNVID ,NULL,STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,NULL,0,&pNV);
		if ( S_OK != hr || pNV == NULL){
			nRes = CQCN_FILE_ERROR_READ_NV;
		}
	}
	if(nRes == NO_ERROR)
	{
		_bstr_t bstrIndex = _T("NV_ITEM_ARRAY");
		hr = pNV->OpenStream((LPCWSTR)bstrIndex,NULL,STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pNVContent);
		if(hr != S_OK || pNVContent == NULL)
			nRes = CQCN_FILE_ERROR_READ_NV;
	}

	WORD wLastID = 0;
	WORD wTotalSize = 0;
	short sTemp = 0;
	ULONG ulen = 0;
	if(nRes == NO_ERROR)
	{
		hr = pNVContent->Read(&wTotalSize, sizeof(WORD), &ulen);
		if(hr != S_OK)
			nRes = CQCN_FILE_ERROR_READ_NV;
	}
	if(nRes == NO_ERROR)
	{
		hr = pNVContent->Read(&sTemp, sizeof(short), &ulen);
		if(hr != S_OK)
			nRes = CQCN_FILE_ERROR_READ_NV;
	}
	if(nRes == NO_ERROR)
	{
		hr = pNVContent->Read(&wLastID, sizeof(WORD), &ulen);
		if(hr != S_OK)
			nRes = CQCN_FILE_ERROR_READ_NV;
	}
	while(nRes == NO_ERROR)
	{
		WORD wItemId = 0;
		short sItemIndex = 0 ;
		BYTE pbData[NV_CONTENT_LENGTH] = {0};
		CNVItemData objNVItem;

		if(nRes == NO_ERROR)
		{
			hr = pNVContent->Read(&sItemIndex, sizeof(short), &ulen);
			if(hr != S_OK)
				nRes = CQCN_FILE_ERROR_READ_NV;
		}
		if(nRes == NO_ERROR)
		{
			hr = pNVContent->Read(pbData,wTotalSize - 8 ,&ulen);
			if(hr != S_OK)
				nRes = CQCN_FILE_ERROR_READ_NV;
		}

		if(nRes == NO_ERROR)
		{
			objNVItem.SetItemID(wLastID);
			objNVItem.AddItemContent(pbData);

			hr = pNVContent->Read(&wTotalSize, sizeof(WORD), &ulen);
			if(hr != S_OK)
				nRes = CQCN_FILE_ERROR_READ_NV;
			if((nRes == NO_ERROR) && (ulen < sizeof(WORD)))
			{
				NVItemArray.push_back(objNVItem);
				nRes = CQCN_FILE_ERROR_NV_END;
			}
		}
		if(nRes == NO_ERROR)
		{
			hr = pNVContent->Read(&sTemp, sizeof(short), &ulen);
			if(hr != S_OK)
				nRes = CQCN_FILE_ERROR_READ_NV;
		}
		if(nRes == NO_ERROR)
		{
			hr = pNVContent->Read(&wItemId, sizeof(WORD), &ulen);
			if(hr != S_OK)
				nRes = CQCN_FILE_ERROR_READ_NV;
		}
		while((wItemId == wLastID) && (nRes == NO_ERROR))
		{
			if(wItemId == 2507)
				int m =0;
			if(nRes == NO_ERROR)
			{
				hr = pNVContent->Read(&sItemIndex, sizeof(short), &ulen);
				if(hr != S_OK)
					nRes = CQCN_FILE_ERROR_READ_NV;
			}
			if(nRes == NO_ERROR)
			{
				hr = pNVContent->Read(pbData,wTotalSize - 8 ,&ulen);
				if(hr != S_OK)
					nRes = CQCN_FILE_ERROR_READ_NV;
			}
			if(nRes == NO_ERROR)
			{
				objNVItem.AddItemContent(pbData);

				hr = pNVContent->Read(&wTotalSize, sizeof(WORD), &ulen);
				if(hr != S_OK)
					nRes = CQCN_FILE_ERROR_READ_NV;

				if((nRes == NO_ERROR) && (ulen < sizeof(WORD)))
				{
					NVItemArray.push_back(objNVItem);
					nRes = CQCN_FILE_ERROR_NV_END;
				}
			}
			if(nRes == NO_ERROR)
			{
				hr = pNVContent->Read(&sTemp, sizeof(short), &ulen);
				if(hr != S_OK)
					nRes = CQCN_FILE_ERROR_READ_NV;
			}
			if(nRes == NO_ERROR)
			{
				hr = pNVContent->Read(&wItemId, sizeof(WORD), &ulen);
				if(hr != S_OK)
					nRes = CQCN_FILE_ERROR_READ_NV;
			}
		}

		if(nRes == NO_ERROR)
		{
			NVItemArray.push_back(objNVItem);
			wLastID = wItemId;
		}
	}

	return nRes;	
}
int CQCNFile::ReadNVItem(std::vector<CNVItemData>& NVItemArray){
	
	_ASSERTE ( NULL != m_pNVStorage);
	int nRes = NO_ERROR;
	if(m_wQCNVersion == 1)
	{
		HRESULT hr = S_OK;
		STATSTG sstg = {0};
		_bstr_t bstrNVID;

		

		while(nRes == NO_ERROR)
		{
			CComPtr<IStorage>	pNV;
			hr = m_pNVStorage->Next(1, &sstg, NULL);
			if(hr != S_OK)
			{
				nRes = CQCN_FILE_ERROR_READ_NV;
				return nRes;
			}

			if(/*(nRes == NO_ERROR) &&*/ sstg.type == STGTY_STORAGE)
			{
				bstrNVID = sstg.pwcsName;

				if(!IsLetterInString(static_cast<LPCTSTR>(bstrNVID)) )
				{
					// Read NV Content
					hr = m_pConfig->OpenStorage((LPCWSTR) bstrNVID ,NULL,STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,
						NULL,0,&pNV);
					if ( S_OK != hr){
						pNV = NULL;
						m_pNVStorage = NULL;
						nRes = CQCN_FILE_ERROR_READ_NV;
						return nRes;;
					}

					// Read NV Index
					CComPtr<IEnumSTATSTG> pNVIndex = NULL;

					hr = pNV->EnumElements(0, NULL, 0, &pNVIndex); 
					if(hr != S_OK)
					{
						pNV = NULL;
						m_pNVStorage = NULL;
						pNVIndex = NULL;
						nRes = CQCN_FILE_ERROR_READ_NV;
						return nRes;;
					}

					CNVItemData objNVItem;
					objNVItem.SetItemID((DWORD)_ttoi(static_cast<LPCTSTR>(bstrNVID)));
					if(objNVItem.GetItemID() == 2507)
						int m = 0;
					while(true)
					{	
						_bstr_t bstrIndex = _T("");
						hr = pNVIndex->Next(1, &sstg, NULL);
						if(hr != S_OK)
						{
							NVItemArray.push_back(objNVItem);
							break;
						}

						if(sstg.type == STGTY_STREAM)  
							bstrIndex = sstg.pwcsName;

						CComPtr<IStream> pNVContent = NULL;
						hr = pNV->OpenStream((LPCWSTR)bstrIndex,NULL,STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pNVContent);

						WORD wItemId = 0;

						short sItemIndex = 0 ;
						short sTotalSize = 0;
						ULONG ulen = 0;
						BYTE pbData[NV_CONTENT_LENGTH] = {0};

						pNVContent->Read(&wItemId, sizeof(WORD), &ulen);
						pNVContent->Read(&sItemIndex, sizeof(short), &ulen);
						pNVContent->Read(&sTotalSize, sizeof(short), &ulen);
						pNVContent->Read(pbData,NV_CONTENT_LENGTH,&ulen);

						//objNVItem.SetItemID(wItemId);
						objNVItem.AddItemContent(pbData);
						//NVItemArray.push_back(objNVItem);
						pNVContent = NULL;
					}
				}
				else
				{
					m_pNVStorage = NULL;
					nRes = CQCN_FILE_ERROR_NV_END;
					return nRes;
				}
			}else{
				m_pNVStorage = NULL;
				nRes = CQCN_FILE_ERROR_NV_END;
				return nRes;
			}
		}
		return NO_ERROR;
	}
	else if(m_wQCNVersion == 2)
		return GetNVItem(NVItemArray);
}


int CQCNFile::Close()
{
	m_pRoot = NULL;      
	m_pModelType = NULL;
	m_pConfig = NULL;
	m_pNVStorage = NULL;

	/*pStg = NULL;	
	pSub1 = NULL;	
	pSub2 = NULL;	
	pSub3 = NULL;	
	pStm = NULL;	
	pStmFile = NULL;*/

	if( pStg )	pStg->Release();
	if( pSub1 )	pSub1->Release();
	if( pSub2 )	pSub2->Release();
	if( pSub3 )	pSub3->Release();
	if( pStm )	pStm->Release();
	if(pStmFile) pStmFile->Release();
	return NO_ERROR;
}

int CQCNFile::OpenNVStorage()
{

	_ASSERTE ( NULL != m_pConfig);
	
	HRESULT hr = CQCN_FILE_ERROR_OPEN_NV;
	STATSTG sstg = {0};

	if(m_pConfig)
		hr = m_pConfig->EnumElements(0, NULL, 0, &m_pNVStorage);

	if(hr != S_OK)
		m_pNVStorage = NULL; 

	return NO_ERROR;

}

bool CQCNFile::IsLetterInString(LPCTSTR szCheck)
{
	bool bRes = false;
	_ASSERTE ( NULL != szCheck);
	
	if(szCheck)
	{
		size_t nLen = _tcslen(szCheck);

		for ( DWORD i = 0 ; i < nLen; i++)
		{
			if (! _istdigit(szCheck[i]) )
			{
				bRes = true;
				break;
			}
		}
	}

	return bRes;
}

int CQCNFile::OpenAndCreateFile(LPCTSTR szFileName, UINT nFlag)
{
	WCHAR cstrFolderPath[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,FALSE,szFileName,
		-1, cstrFolderPath,MAX_PATH);
	hr = ::StgCreateDocfile(	
		cstrFolderPath,	
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,	
		0,		
		&pStg);		
	ASSERT( SUCCEEDED(hr) );	
	
	hr = pStg->CreateStorage(	
		L"00000186",	
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pSub1);		
	ASSERT( SUCCEEDED(hr) );
	hr = pSub1->CreateStorage(	
		L"default",	
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pSub2);		
	ASSERT( SUCCEEDED(hr) );
	hr = pSub2->CreateStorage(	
		L"NV_NUMBERED_ITEMS",	
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pSub3);		
	ASSERT( SUCCEEDED(hr) );
	hr = pSub3->CreateStream(	
		L"NV_ITEM_ARRAY",		
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pStm);		
	ASSERT( SUCCEEDED(hr) );
	hr = pStg->CreateStream(	
		L"File_Version",		
		STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0,0,
		&pStmFile);		
	ASSERT( SUCCEEDED(hr) );
	WORD m_wQCNVersion = 0x2;
	hr = pStmFile->Write(		
		&m_wQCNVersion,		
		sizeof(WORD),		
		NULL);		
	ASSERT( SUCCEEDED(hr) );	
	return NO_ERROR;
}

int CQCNFile::WriteNVItem(CNVItemData& objNVItem)
{
	WORD wLastID = 0;
	WORD wTotalSize = 0x88;
	int nvlength = objNVItem.GetNVContentLen();
	short sTemp = 0x01;
	wLastID = objNVItem.GetItemID();
	short sItemIndex = 0 ;
	BYTE *pbData = objNVItem.GetItemContent();
		
	while(nvlength >= 128)
		{
			hr = pStm->Write(		
				&wTotalSize,		
				sizeof(WORD),		
				NULL);		
			ASSERT( SUCCEEDED(hr) );
			hr = pStm->Write(		
				&sTemp,		
				sizeof(short),		
				NULL);		
			ASSERT( SUCCEEDED(hr) );
			hr = pStm->Write(		
				&wLastID,		
				sizeof(WORD),		
				NULL);		
			ASSERT( SUCCEEDED(hr) );
			hr = pStm->Write(		
				&sItemIndex,		
				sizeof(short),		
				NULL);		
			ASSERT( SUCCEEDED(hr) );
			hr = pStm->Write(		
				pbData,		
				0x80,		
				NULL);		
			ASSERT( SUCCEEDED(hr) );
			pbData = pbData + 0x80;
			nvlength = nvlength - 0x80;
			sItemIndex++;

		}
	return 0;

}
