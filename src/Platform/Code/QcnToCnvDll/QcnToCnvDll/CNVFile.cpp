#include "StdAfx.h"
#include "CNVFile.h"
#include "Operator.h"

CCNVFile::CCNVFile(void):m_bIsOpened(false)
{
}

CCNVFile::~CCNVFile(void)
{
	Close();
}

int CCNVFile::Open(LPCTSTR szFileName, UINT nFlag)
{
	int nRes = NO_ERROR;
	if(SetFileName(szFileName))
	{
		CFileException fe;
		if (!m_fCNV.Open(GetFileName(),nFlag, &fe))
		{
			CString strError;
			strError.Format(_T("Open file:%s with error code:%d.\n"),GetFileName(),fe.m_cause);
			nRes = CCNV_FILE_ERROR_FILE_NAME;
		}

		if(nRes == NO_ERROR)
		{
			m_bIsOpened = true;
			nRes = WriteHeader();
			if(NO_ERROR != nRes)
				printf("Conversion failed with error: 0x%X", nRes);
		}
	}
	else
	{
		nRes = ERROR_INVALID_DATA;
	}

	return nRes;
}

int CCNVFile::WriteHeader()
{
	ASSERT(m_fCNV.m_hFile != CFile::hFileNull);
	// HEADER //
	uint8 headername[4]= {'C','N','V','*'};
	uint16 headerversion = 1;
	uint16 headerpad = 0xFFFF;

	m_fCNV.SeekToBegin();
	m_fCNV.Write(&headername,4);
	m_fCNV.Write(&headerversion,2);
	m_fCNV.Write(&headerpad,2);

	return 0;
}

int CCNVFile::WriteNVItem(CNVItemData& objNV)
{
	ASSERT(m_fCNV.m_hFile != CFile::hFileNull);

	uint8 *nvi_data;  // buffor for data
	uint8 nvi_pad = 0xFF;   // buffor for padding

	// MULTIPLE INSTANCE 
	uint16 ID;
	uint16 SIZE;
	int ElementsNb;
	char index;

	ID = objNV.GetItemID();
	SIZE = objNV.GetNVContentLen();
	ElementsNb = objNV.GetNbOfElements();
	nvi_data = (uint8*)objNV.GetItemContent();

	if(ElementsNb>0)
		SIZE++;
	if(ID == 2507)
		int m = 0;
	m_fCNV.Write(&ID,2);
	m_fCNV.Write(&SIZE,2);

	if(ElementsNb>0)  // jesli array to dopisuje jeszcze index
	{
		index = (char)objNV.GetIndex();
		m_fCNV.Write(&index,1);
		m_fCNV.Write(nvi_data,SIZE-1); // przesuniecie danych to kolejenj komorki
	}
	else
	{
		m_fCNV.Write(nvi_data,SIZE);
	}

	int pad_size = 4 - (SIZE % 4);

	if(pad_size == 4)
		pad_size = 0;

	for(int p=0;p<pad_size;p++)
	{
		m_fCNV.Write(&nvi_pad,1);
	}

	return NO_ERROR;
}

int CCNVFile::ReWrite(const void* lpBuf, UINT nCount)
{
	m_fCNV.Write(lpBuf,nCount);
	return 0;
}

int CCNVFile::WriteEnd()
{
	ASSERT(m_fCNV.m_hFile != CFile::hFileNull);
	/* END OF FILE */
	uint16 LastNV = 0xFFFF;// (last item)
	uint8 some1 = 0x02;
	uint8 some2 = 0x00;

	m_fCNV.Write(&LastNV,2);
	m_fCNV.Write(&some1,1);
	m_fCNV.Write(&some2,1);

	return 0;
}

int CCNVFile::Close()
{
	if(m_bIsOpened)
	{
		m_fCNV.Close();
		m_bIsOpened = false;
	}
	return 0;
}

int CCNVFile::OpenAndReadHeader(LPCTSTR szFileName, UINT nFlag)
{
	ASSERT(NULL != szFileName);
	CFileException fe;
	if (!m_fCNV.Open(szFileName,nFlag, &fe))
	{
		CString strError;
		strError.Format(_T("Open file:%s with error code:%d.\n"),szFileName,fe.m_cause);
		return CCNV_FILE_ERROR_FILE_NAME;
	}
	ReadHeader();
	return NO_ERROR;

}

int CCNVFile::ReadHeader()
{
	ASSERT(m_fCNV.m_hFile != CFile::hFileNull);
	// HEADER //
	uint8 headername[4]= {0};
	uint16 headerversion;
	uint16 headerpad;

	m_fCNV.SeekToBegin();
	m_fCNV.Read(headername,4);
	m_fCNV.Read(&headerversion,2);
	m_fCNV.Read(&headerpad,2);
	return 0;
}

int CCNVFile::ReadNVItem(CArray<CNVItemData,CNVItemData&>& NVItemArray)
{
	ASSERT(m_fCNV.m_hFile != CFile::hFileNull);
	int nRes = NO_ERROR;
	int num = 0;
	//Begin to read file
	while(true)
	{
		uint16 ID;
		uint16 SIZE = 0x80;
		uint8 nvi_data2[NV_CONTENT_LENGTH] = {0}; 
		CNVItemData objNVItem;
		m_fCNV.Read(&ID,2);
		m_fCNV.Read(&SIZE,2);

		if(SIZE > NV_CONTENT_LENGTH )
		{		
			while(SIZE >= NV_CONTENT_LENGTH)
			{				
				uint8 *pnvdata = new BYTE[NV_CONTENT_LENGTH];
				m_fCNV.Read(pnvdata,NV_CONTENT_LENGTH); //if date length > 128,read 128 byte each time 
				SIZE = SIZE - NV_CONTENT_LENGTH;
				if(m_fCNV.GetPosition() == m_fCNV.GetLength())
					break;
				objNVItem.AddItemContent(pnvdata);
			}
			objNVItem.SetItemID(ID);
			NVItemArray.Add(objNVItem);
			num++;

		}else{			
			m_fCNV.Read(nvi_data2,SIZE);
			if(m_fCNV.GetPosition() == m_fCNV.GetLength())
				break;
			objNVItem.SetItemID(ID);
			objNVItem.AddItemContent(nvi_data2);
			NVItemArray.Add(objNVItem); 
			num++;
		}
	}
	return NO_ERROR;
}