#include "stdafx.h"
#include "CBRTFile.h"

int CBRTFile::Open(LPCTSTR szFileName, UINT nFlag){
	
	_ASSERTE(NULL != szFileName);
	CFileException fe;
	if (!m_fBRT.Open(szFileName,nFlag, &fe)){
		CString strError;
		strError.Format(_T("Open file:%s with error code:%d.\n"),szFileName,fe.m_cause);
		return CBRT_FILE_ERROR_FILE_NAME;
	}
	DWORD dwNVCount = 0;
	WriteHeader(&dwNVCount);
	return NO_ERROR;
}

int CBRTFile::WriteHeader(const void *pvHeaderInfo){
	_ASSERTE (m_fBRT.m_hFile != CFile::hFileNull);

	m_fBRT.SeekToBegin();
	m_fBRT.Write(pvHeaderInfo,sizeof(BRT_FILE_HEADER));
	return NO_ERROR;
}

int CBRTFile::WriteNVItem(CNVItemData& objNV){
	_ASSERTE (m_fBRT.m_hFile != CFile::hFileNull);
	// Write NVID
	DWORD	dwNVID = objNV.GetItemID();
	DWORD	dwContentLen = objNV.GetNVContentLen();
	m_fBRT.Write(&dwNVID,sizeof(DWORD));
	m_fBRT.Write(&dwContentLen,sizeof(DWORD));
	m_fBRT.Write(objNV.GetItemContent(),dwContentLen);
	return NO_ERROR;
}

int CBRTFile::Close(){
	m_fBRT.Close();
	return NO_ERROR;
}

int CBRTFile::OpenAndReadHeader(LPCTSTR szFileName, UINT nFlag){

	_ASSERTE(NULL != szFileName);
	CFileException fe;
	if (!m_fBRT.Open(szFileName,nFlag, &fe)){
		CString strError;
		strError.Format(_T("Open file:%s with error code:%d.\n"),szFileName,fe.m_cause);
		return CBRT_FILE_ERROR_FILE_NAME;
	}
	ReadHeader();
	return NO_ERROR;
}

int CBRTFile::ReadHeader()
{
	_ASSERTE (m_fBRT.m_hFile != CFile::hFileNull);

	DWORD pvHeaderInfo;
	m_fBRT.SeekToBegin();
	m_fBRT.Read(&pvHeaderInfo,sizeof(BRT_FILE_HEADER));
	return NO_ERROR;
}

int CBRTFile::ReadNVItem(CArray<CNVItemData,CNVItemData&>& NVItemArray){
	_ASSERTE (m_fBRT.m_hFile != CFile::hFileNull);

	int num = 0;
	DWORD dwNVID;
	DWORD dwContentLen;
	BYTE m_pbItemContent[0x80] = {0};


	while(true)
	{
		CNVItemData objNVItem;
		m_fBRT.Read(&dwNVID,sizeof(DWORD));
		m_fBRT.Read(&dwContentLen,sizeof(DWORD));
		
		if(dwContentLen > 0x80)
		{
			while(dwContentLen >= 0x80)
			{
				BYTE *pbItemContent = new BYTE[0x80];
				m_fBRT.Read(pbItemContent,0x80);
				dwContentLen = dwContentLen -0x80;
				objNVItem.AddItemContent(pbItemContent);
			}
			objNVItem.SetItemID(dwNVID);
			NVItemArray.Add(objNVItem);
			num++;
			if(m_fBRT.GetPosition() == m_fBRT.GetLength())
				break;
		}else{
			m_fBRT.Read(m_pbItemContent,dwContentLen);
			objNVItem.SetItemID(dwNVID);
			objNVItem.AddItemContent(m_pbItemContent);
			NVItemArray.Add(objNVItem); 
			num++;
			if(m_fBRT.GetPosition() == m_fBRT.GetLength())
				break;
		}
	}
	return NO_ERROR;
}