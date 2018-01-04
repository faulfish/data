#include "StdAfx.h"
#include "Operator.h"
#include "CQCNFile.h"
#include "CNVFile.h"
#include "CNVFile.h"
#include "CodeCrc.h"
#include <assert.h>
#include "CBRTFile.h"
#include "..\..\Modules\QisdaNVModule\NVItem.h"

COperator::COperator(void)
:mListViewCalBack(NULL)
,mEditViewCalBack(NULL)
,mVecTreeCalBack(NULL)
,mFillTreeCalBack(NULL)
{
	
}

COperator::~COperator(void)
{

}

void COperator:: SetCalBack(LPVOID pDoc, LPVOID pTreeView,LPVOID pTreeView2, LPVOID pEditView, LPVOID pListView)
{
	_ASSERTE(pDoc != NULL);
	_ASSERTE(pTreeView != NULL);
	_ASSERTE(pTreeView2 != NULL);
	_ASSERTE(pEditView != NULL);
	_ASSERTE(pListView != NULL);
	mFillTreeCalBack = (FillTreeFunc)pTreeView;
	mVecTreeCalBack  = (TreeViewFunc)pTreeView2;
	mEditViewCalBack = (EditViewFunc)pEditView;
	mListViewCalBack = (ListViewFunc)pListView;
	lpDoc = pDoc;
}

int COperator:: GetNVItem(int mNVItemID)
{
	switch(m_FileType)
	{
	case BRT:
	case CNV:
		for(int i = 0;i<m_NVItemArrayReverse.GetCount();i++)
		{
			if(m_NVItemArrayReverse[i].GetItemID()==mNVItemID)
			{
				mNVItem = m_NVItemArrayReverse[i];
				mEditViewCalBack(lpDoc,mNVItem);
				return 0;
			}
		}
		break;
	case DU:
	case QCN:
		for(int i = 0;i<m_NVItemArray.size();i++)
		{
			if(m_NVItemArray[i].GetItemID()==mNVItemID)
			{
				mNVItem = m_NVItemArray[i];
				mEditViewCalBack(lpDoc,mNVItem);
				return 0;
			}
		}
		break;
	default:
		for(int i = 0;i<m_NVItemArrayReverse.GetCount();i++)
		{
			if(m_NVItemArrayReverse[i].GetItemID()==mNVItemID)
			{
				mNVItem = m_NVItemArrayReverse[i];
				mEditViewCalBack(lpDoc,mNVItem);
				return 0;
			}
		}
		break;
	}
	return -1;
}

int COperator:: SaveNVItemData(int itemId,BYTE* itemContent)
{
	switch(m_FileType)
	{
	case BRT:
	case CNV:
		for(int i = 0;i<m_NVItemArrayReverse.GetCount();i++)
		{
			if(m_NVItemArrayReverse[i].GetItemID()==itemId)
			{
				CNVItemData mNVItemC;
				mNVItemC = m_NVItemArrayReverse[i];
				mNVItemC.SetItemContent(itemContent);
				m_NVItemArrayReverse.RemoveAt(i);
				m_NVItemArrayReverse.InsertAt(i,mNVItemC);
				return 0;
			}
		}
		break;
	case DU:
	case QCN:
		for(int i = 0;i<m_NVItemArray.size();i++)
		{
			if(m_NVItemArray[i].GetItemID()==itemId)
			{
				m_NVItemArray[i].SetItemContent(itemContent);
				return 0;
			}
		}
		break;
	default:
		for(int i = 0;i<m_NVItemArrayReverse.GetCount();i++)
		{
			if(m_NVItemArrayReverse[i].GetItemID()==itemId)
			{
				CNVItemData mNVItemC;
				mNVItemC = m_NVItemArrayReverse[i];
				mNVItemC.SetItemContent(itemContent);
				m_NVItemArrayReverse.RemoveAt(i);
				m_NVItemArrayReverse.InsertAt(i,mNVItemC);
				return 0;
			}
		}
		break;

	}
	return -1;
}

int COperator:: SortNVItemAscByID()
{
	switch(m_FileType)
	{
	case DU:
	case QCN:
		for(int cmpnum = m_NVItemArray.size()-1; cmpnum != 0; --cmpnum)
		{
			for (int i = 0; i != cmpnum; ++i)
			{
				if (m_NVItemArray[i].GetItemID() > m_NVItemArray[i+1].GetItemID())
				{
					CNVItemData temp;
					temp = m_NVItemArray[i];
					m_NVItemArray[i] = m_NVItemArray[i+1];
					m_NVItemArray[i+1] = temp;
				}
			}
		}
		mVecTreeCalBack(lpDoc,m_NVItemArray);
		return 0;
		break;
	case BRT:
	case CNV:
	default:
		for(int j = m_NVItemArrayReverse.GetSize()-1; j != 0; --j)
		{
			for (int i = 0; i != j; ++i)
			{
				if (m_NVItemArrayReverse[i].GetItemID() > m_NVItemArrayReverse[i+1].GetItemID())
				{
					CNVItemData temp;
					temp = m_NVItemArrayReverse[i];
					m_NVItemArrayReverse[i] = m_NVItemArrayReverse[i+1];
					m_NVItemArrayReverse[i+1] = temp;
				}
			}
		}
		mFillTreeCalBack(lpDoc,m_NVItemArrayReverse);
		return 0;
		break;	}
	return -1;
}

int COperator:: SortNVItemDesByID()
{
	switch(m_FileType)
	{
	case DU:
	case QCN:
		for(int cmpnum = m_NVItemArray.size()-1; cmpnum != 0; --cmpnum)
		{
			for (int i = 0; i != cmpnum; ++i)
			{
				if (m_NVItemArray[i].GetItemID() < m_NVItemArray[i+1].GetItemID())
				{
					CNVItemData temp;
					temp = m_NVItemArray[i];
					m_NVItemArray[i] = m_NVItemArray[i+1];
					m_NVItemArray[i+1] = temp;
				}
			}
		}
		mVecTreeCalBack(lpDoc,m_NVItemArray);
		return 0;
		break;
	case BRT:
	case CNV:
	default:
		for(int j = m_NVItemArrayReverse.GetSize()-1; j != 0; --j)
		{
			for (int i = 0; i != j; ++i)
			{
				if (m_NVItemArrayReverse[i].GetItemID() < m_NVItemArrayReverse[i+1].GetItemID())
				{
					CNVItemData temp;
					temp = m_NVItemArrayReverse[i];
					m_NVItemArrayReverse[i] = m_NVItemArrayReverse[i+1];
					m_NVItemArrayReverse[i+1] = temp;
				}
			}
		}
		mFillTreeCalBack(lpDoc,m_NVItemArrayReverse);
		return 0;
		break;
	}
	return -1;
}

int COperator:: SaveNVItemFile(CString &filePath,int bFileType,int fileType)
{
	int bRes = 0;
	switch(fileType)
	{
	case BRT:
		bRes = SaveNVItemToBrtFile(bFileType,filePath);
		break;
	case CNV:
		bRes = SaveNVItemToCnvFile(bFileType,filePath);
		break;
	case DU:
		//bRes = SaveNVItemToDuFile(filePath);
		break;
	case QCN:
		bRes = SaveNVItemToQcnFile(bFileType,filePath);
		break;
	}
	return bRes;
}
int COperator:: SaveNVItemToBrtFile(int bFileType,CString &filePath)
{
	int nRet = NO_ERROR;
	CBRTFile objBRT;
	nRet = objBRT.Open(filePath,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		DWORD nNVItemCount = 0;
		if(bFileType==QCN){
		  for(std::vector<CNVItemData>::iterator Iter = m_NVItemArray.begin();Iter != m_NVItemArray.end();Iter++)
			{
				objBRT.WriteNVItem(*Iter);
				nNVItemCount++;
			}

		}else if(bFileType==BRT||bFileType==CNV|| bFileType==RAWDATA){
			for(int i = 0;i < m_NVItemArrayReverse.GetSize();i++)
			{
				CNVItemData objNVItem =m_NVItemArrayReverse.ElementAt(i);
				objBRT.WriteNVItem(objNVItem);
				nNVItemCount++;
			}
		}
		nRet = objBRT.WriteHeader(&nNVItemCount);
		if ( NO_ERROR != nRet){
			printf("Conversion failed with error: 0x%X", nRet);
		}
		objBRT.Close();
	}
	return nRet;
}
int COperator:: SaveNVItemToCnvFile(int bFileType,CString &filePath)
{
	int nRet = NO_ERROR;

	CCNVFile objCNV;
	nRet = objCNV.Open(filePath,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		if(bFileType==QCN){
			for(std::vector<CNVItemData>::iterator Iter = m_NVItemArray.begin();Iter != m_NVItemArray.end();Iter++)
			{
				objCNV.WriteNVItem(*Iter);
			}

		}else if(bFileType==BRT||bFileType==CNV || bFileType==RAWDATA){
			for(int i = 0;i < m_NVItemArrayReverse.GetSize();i++)
			{
				CNVItemData objNVItem =m_NVItemArrayReverse.ElementAt(i);
				objCNV.WriteNVItem(objNVItem);
			}
		}
		FinishTheCNVFile(objCNV,filePath);
	}
	return nRet;
}
int COperator:: SaveNVItemToQcnFile(int bFileType,CString &filePath)
{
	int nRet = NO_ERROR;

	CQCNFile objQCN;
	nRet = objQCN.OpenAndCreateFile(filePath,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		if(bFileType==QCN){
			for(std::vector<CNVItemData>::iterator Iter = m_NVItemArray.begin();Iter != m_NVItemArray.end();Iter++)
			{
				objQCN.WriteNVItem(*Iter);
			}	
		}else if(bFileType==BRT||bFileType==CNV|| bFileType==RAWDATA){
			for(int i = 0;i < m_NVItemArrayReverse.GetSize();i++)
			{
				CNVItemData objNVItem =m_NVItemArrayReverse.ElementAt(i);
				objQCN.WriteNVItem(objNVItem);
			}
		}
	}
	return nRet;
}

bool COperator:: FileToNVArray(const CString &filePath,const int fileType)
{
	bool bRes = false;
	m_FileType = fileType;
	CallMListViewCalBack(lpDoc,_T("Start to read File..."),0);
	switch(m_FileType)
	{
	case BRT:
		bRes = SaveBrtFileToNVArray(filePath,m_NVItemArrayReverse);
		mFillTreeCalBack(lpDoc,m_NVItemArrayReverse);
		break;
	case CNV:
		bRes =  SaveCnvFileToNVArray(filePath,m_NVItemArrayReverse);
		mFillTreeCalBack(lpDoc,m_NVItemArrayReverse);
		break;
	case DU:
		bRes = SaveDUFileToNVArray(filePath,m_NVItemArray);
		mVecTreeCalBack(lpDoc,m_NVItemArray);
		break;
	case QCN:
		bRes = SaveQcnFileToNVArray(filePath,m_NVItemArray);
		mVecTreeCalBack(lpDoc,m_NVItemArray);
		break;
	}
	if(bRes)
		CallMListViewCalBack(lpDoc,_T("Read File Success!"),0);
	return bRes;
}

bool COperator::SaveQcnFileToNVArray(const CString& strQCNPath,std::vector<CNVItemData>& NVItemArray)
{
	bool bRes = false;

	CQCNFile objQCN;
	CallMListViewCalBack(lpDoc,_T("Start Open File..."),0);
	int nRet = objQCN.Open((LPCTSTR)strQCNPath,CFile::modeRead);
	if ( NO_ERROR != nRet)
	{
		CallMListViewCalBack(lpDoc,_T("Open File Error!"),-1);
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		CallMListViewCalBack(lpDoc,_T("Open File Success!"),0);
		nRet = objQCN.ReadNVItem(NVItemArray);
		//while(true)
		//{
		//	CNVItem objNVItem;
		//	nRet = objQCN.ReadNVItem(&objNVItem);
		//	if (NO_ERROR == nRet)
		//		NVItemArray.push_back(objNVItem);
		//	else
		//		break;
		//};
		if(nRet == NO_ERROR)
			bRes = true;
	}
	
	return bRes;
}

bool COperator::FinishTheBRTFile(CBRTFile& objBRT,const CString& BRTFileName)
{
	objBRT.Close();
	return NO_ERROR;
}

bool COperator::FinishTheCNVFile(CCNVFile& objCNV,const CString& CNVFileName)
{
	int nRet = objCNV.WriteEnd();
	if(NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		objCNV.Close();
		CreateCRCValue(CNVFileName);
		printf("Conversion complete successfully.\n");
	}

	return nRet == NO_ERROR;
}

// Steps To Change From QcnFiles To CnvFiles
int COperator::SaveQcnFileAndDUFileToCnvFileFunction_part2(const CString& strDU,const CString& strQCN,const CString& strCNV)
{
	int nRet = NO_ERROR;

	if(::_taccess(strDU,0) != -1)
		SaveDUFileToNVArray(strDU,m_NVItemArray);
	if(::_taccess(strQCN,0) != -1)
		SaveQcnFileToNVArray(strQCN,m_NVItemArray);	

	CCNVFile objCNV;
	nRet = objCNV.Open(strCNV,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		//1 Write NV to CNV file
		DWORD nNVItemCount = 0;
		for(std::vector<CNVItemData>::iterator Iter = m_NVItemArray.begin();Iter != m_NVItemArray.end();Iter++)
		{
			objCNV.WriteNVItem(*Iter);
			nNVItemCount++;
		}
		//2 
		FinishTheCNVFile(objCNV,strCNV);		
	}

	return nRet;
}

// Steps To Change From QcnFiles To CnvFiles
int COperator::SaveQcnFileAndDUFileToBRTFileFunction_part2(const CString& strDU,const CString& strQCN,const CString& strBRT)
{
	int nRet = NO_ERROR;

	if(::_taccess(strDU,0) != -1)
		SaveDUFileToNVArray(strDU,m_NVItemArray);
	if(::_taccess(strQCN,0) != -1)
		SaveQcnFileToNVArray(strQCN,m_NVItemArray);	

	CBRTFile objBRT;
	nRet = objBRT.Open(strBRT,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		//1 Write NV to CNV file
		DWORD nNVItemCount = 0;
		for(std::vector<CNVItemData>::iterator Iter = m_NVItemArray.begin();Iter != m_NVItemArray.end();Iter++)
		{
			objBRT.WriteNVItem(*Iter);
			nNVItemCount++;
		}
		//2 
		nRet = objBRT.WriteHeader(&nNVItemCount);
		FinishTheBRTFile(objBRT,strBRT);
	}

	return nRet;
}

bool COperator::GetNVBlock(FILE* hDuckFile, int nNvId, CNVItemData& nvItem)
{
	bool bRes = false;

	nvItem.SetItemID(nNvId);

	int nDataSize = 0;
	fread(&nDataSize,sizeof(int),1,hDuckFile);          // reading value of data's size

	BYTE buffer[NV_CONTENT_LENGTH] = {0};

	assert(nDataSize <= 128);	
	if(nDataSize <= 128)
	{
		::fread(buffer,nDataSize,1,hDuckFile);                   // reading data
		nvItem.AddItemContent(buffer);
		bRes = true;
	}
	else
		bRes = false;

	return bRes;
}

bool COperator::CreatePath(std::string &wsPath)
{
	DWORD attr;
	int pos;
	bool result = true;

	// Check for trailing slash:
	pos = wsPath.find_last_of('\\');
	if (wsPath.length() == pos + 1)  // last character is "\"
	{
		wsPath.resize(pos);
	}

	// Look for existing object:
	attr = GetFileAttributes(wsPath.c_str());
	if (0xFFFFFFFF == attr)  // doesn't exist yet - create it!
	{
		pos = wsPath.find_last_of('\\');
		if (0 < pos)
		{
			// Create parent dirs:
			result = CreatePath(wsPath.substr(0, pos));
		}
		// Create node:
		result = result && CreateDirectory(wsPath.c_str(), NULL);
	}
	else if (FILE_ATTRIBUTE_DIRECTORY != attr)
	{  // object already exists, but is not a dir
		//SetLastError(ERROR_FILE_EXISTS);
		//result = false;
	}

	return result;
}

bool COperator::OutputFile(const CString& strFilePath,BYTE* pContent,int nLBufferLength)
{
	bool bRes = false;
	CString strLocalFile = PathFindFileName(strFilePath);
	CString strPath = strFilePath.Left(strFilePath.GetLength() - strLocalFile.GetAllocLength());
	strPath.Replace('/','\\');
	std::string sPath = strPath;
	if(CreatePath(sPath))
	{
		CFile file;
		BOOL nRet = file.Open(strPath+strLocalFile,CFile::modeCreate|CFile::modeWrite);
		if(nRet)
		{
			file.Write(pContent,nLBufferLength);
			file.Close();
			bRes = true;
		}
	}
	else
	{
		assert(0);//Need to implement
	}
	return bRes;
}

bool COperator::HandleNVFile(FILE* hDuckFile,const CString& strFilePath, bool bCreateFile)
{
	//NV file content
	int nDataSize = 0;
	::fread(&nDataSize,sizeof(int),1,hDuckFile);          // reading value of data's size
	if(nDataSize > 0)
	{
		BYTE* pBuffer = new BYTE[nDataSize];
		memset(pBuffer, 0, nDataSize);
		::fread(pBuffer,nDataSize,1,hDuckFile);                   // reading data
		if (bCreateFile)
			OutputFile(strFilePath,pBuffer,nDataSize);
		delete [] pBuffer;
	}

	return true;
}

#include <algorithm>

bool  COperator::AddToNVArrar(FILE* hDuckFile,int intData,std::vector<CNVItemData>& NVItemArray)
{
	CNVItemData nvItem;
	GetNVBlock(hDuckFile,intData,nvItem);

	std::vector<CNVItemData>::iterator Iter = std::find(NVItemArray.begin(),NVItemArray.end(),nvItem);
	if(Iter != NVItemArray.end()){
#ifdef CHECK_CONFLICT_LIST
		TRACE("%d\n", Iter->GetItemID());
#endif
		*Iter = nvItem;
	}
	else
		NVItemArray.push_back(nvItem);
	return true;
}

bool COperator::SaveDUFileToNVArray(const CString& strDUPath,std::vector<CNVItemData>& NVItemArray)
{
	bool bRes = false;

	CString strDefaultPath = strDUPath;
	PathRemoveFileSpec(strDefaultPath.GetBuffer(strDefaultPath.GetLength()));
	strDefaultPath.ReleaseBuffer();
	PathAddBackslash(strDefaultPath.GetBuffer(strDefaultPath.GetLength() + 2));
	strDefaultPath.ReleaseBuffer();
	CallMListViewCalBack(lpDoc,"Start Open File...",0);
	FILE* hDuckFile = fopen(strDUPath,"rb");  // opening Duck File for reading only
	if(hDuckFile)  
	{	
		CallMListViewCalBack(lpDoc,"Open File Success!",0);
		int intData=0;
		while(fread(&intData,sizeof(int),1,hDuckFile))      //while not end of file; reading value of NV_ID
		{	 
			if (intData != -1)
			{ //It's NV item
				AddToNVArrar(hDuckFile,intData,NVItemArray);
			} 
			else
			{//It's NV file or Array
				::fread(&intData,sizeof(int),1,hDuckFile);  // read size of FileName
				char* filePath = new char[intData+1]; 
				memset(filePath, 0,(intData+1));
				fread(filePath,intData,1,hDuckFile);    // read File name

				CString nameOfFile;
				CString nameOfNVFile;
				nameOfFile.SetString(filePath);      // change char* to CString
				nameOfNVFile.SetString(filePath);
				delete [] filePath;

				int nTokenCount = 0;
				if(nameOfFile.Tokenize(_T("#"),nTokenCount) == "ARRAY")
				{//It's NV array
					CString strNVId = nameOfFile.Tokenize("#",nTokenCount);
					intData = ::_ttoi(strNVId);
					CString strNVIndex = nameOfFile.Tokenize("#",nTokenCount);

					AddToNVArrar(hDuckFile, intData, NVItemArray);
				}
				else
				{//It's NV file
					CString strLocalFile = PathFindFileName(nameOfNVFile);
					CString strPath = nameOfNVFile.Left(nameOfNVFile.GetLength() - strLocalFile.GetAllocLength());
					
					//NV file content
					HandleNVFile(hDuckFile,strDefaultPath+strPath+strLocalFile);

					//ATT file name
					CString strATTPath = strLocalFile;
					::PathRenameExtension(strATTPath.GetBuffer(strATTPath.GetLength() + 8),_T(".ATT"));
					strATTPath.ReleaseBuffer();
					
					//ATT file content block
					HandleNVFile(hDuckFile,strDefaultPath+strPath+strATTPath, false);
				}
			}
		}

		::fclose(hDuckFile);
		bRes = true;
	}else
	{
		CallMListViewCalBack(lpDoc,"Open File Error!",-1);
	} 
	
	return bRes;
}

int COperator::CreateCRCValue(const CString& CNVFileName)
{	
	CString FilePath = CNVFileName;

	CFile file;	
	BOOL bRes = file.Open(FilePath,CFile::modeRead);
	assert(bRes == TRUE);
	
	if(bRes)
	{
		file.Seek(0,CFile::end);
		ULONGLONG filePosition = file.GetPosition();
		file.Seek(0,CFile::begin);

		byte *buf_ptr = new byte [static_cast<size_t>(filePosition)]; 
		byte bajt;
		ULONGLONG offset = 0;	

		do
		{
			file.Read(&bajt,1);
			buf_ptr[offset] = bajt;
			offset++;
		}
		while(file.GetPosition()<filePosition);

		file.Close();

		word ResultCRC;
		uint16 CRC;

		ResultCRC = CountCRC_M(buf_ptr,filePosition);
		delete [] buf_ptr;

		uint8 *wskResult = reinterpret_cast<uint8*>(&ResultCRC);
		uint8 *wskCRC = reinterpret_cast<uint8*>(&CRC);

		wskCRC[0] = wskResult[1];
		wskCRC[1] = wskResult[0];

		bRes = file.Open(FilePath,CFile::modeWrite);	
		if(bRes)
		{
			file.Seek(0,CFile::end);
			filePosition = file.GetPosition();
			file.Write(&CRC,2);

			file.Close();
		}
	}
	return 1;
}

unsigned short COperator::CountCRC_M(unsigned char* buf, ULONGLONG count)
{
	word partCount = 1024;
	word crc16 = (word) CRC_16_STEP_SEED;

   ULONGLONG offset = 0;

	do
	{
		if(count < partCount)
			partCount = static_cast<word>(count);
		
		crc16 = crc_16_step(crc16,&buf[offset],partCount);
		count -=partCount;
		offset+=partCount;
	}
	while(count>0);

	return crc16;
}

int COperator::SaveQcnFileToBrtFileFunction_part2(const CString& strQCN,const CString& strBRT)
{
	int nRet = NO_ERROR;
	SaveQcnFileToNVArray(strQCN,m_NVItemArray);
	CBRTFile objBRT;
	nRet = objBRT.Open(strBRT,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		DWORD nNVItemCount = 0;
		for(std::vector<CNVItemData>::iterator Iter = m_NVItemArray.begin();Iter != m_NVItemArray.end();Iter++)
		{
			objBRT.WriteNVItem(*Iter);
			nNVItemCount++;
		}	
		nRet = objBRT.WriteHeader(&nNVItemCount);
		if ( NO_ERROR != nRet){
			printf("Conversion failed with error: 0x%X", nRet);
		}
		objBRT.Close();
	}
	return nRet;

}

// Steps To Change From CnvFiles To QcnFiles
int COperator::SaveCnvFileToQcnFileFunction(const CString& strCNV,const CString& strQCN)
{
	int nRet = NO_ERROR;

	SaveCnvFileToNVArray(strCNV,m_NVItemArrayReverse);	

	CQCNFile objQCN;
	nRet = objQCN.OpenAndCreateFile(strQCN,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		DWORD nItemCount = m_NVItemArrayReverse.GetSize();
		for(int i = 0;i < nItemCount;i++)
		{
			CNVItemData objNVItem =m_NVItemArrayReverse.ElementAt(i);
			objQCN.WriteNVItem(objNVItem);
		}
	}
	return nRet;
}

bool COperator::SaveCnvFileToNVArray(const CString& strCNVPath,CArray<CNVItemData,CNVItemData&>& NVItemArray)
{
	bool bRes = false;

	CCNVFile objCNV;
	CallMListViewCalBack(lpDoc,"Start Open File...",0);
	int nRet = objCNV.OpenAndReadHeader((LPCTSTR)strCNVPath,CFile::modeRead);
	if ( NO_ERROR != nRet)
	{
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		nRet = objCNV.ReadNVItem(NVItemArray);

		if(nRet == NO_ERROR)
			bRes = true;
	}
	objCNV.Close();
	return bRes;
}

// Steps To Change From BrtFiles To QcnFiles
int COperator::SaveBrtFileToQcnFileFunction(const CString& strBRT,const CString& strQCN)
{
	int nRet = NO_ERROR;
	SaveBrtFileToNVArray(strBRT,m_NVItemArrayReverse);	

	CQCNFile objQCN;
	nRet = objQCN.OpenAndCreateFile(strQCN,CFile::modeCreate|CFile::modeWrite);
	if ( NO_ERROR != nRet)
	{
		CallMListViewCalBack(lpDoc,"Open File Error!",-1);
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		CallMListViewCalBack(lpDoc,"Open File Success!",0);
		DWORD nItemCount = m_NVItemArrayReverse.GetSize();
		for(int i = 0;i < nItemCount;i++)
		{
			CNVItemData objNVItem =m_NVItemArrayReverse.ElementAt(i);
			objQCN.WriteNVItem(objNVItem);
		}
	}
	return nRet;
}

bool COperator::SaveBrtFileToNVArray(const CString& strBRTPath,CArray<CNVItemData,CNVItemData&>& NVItemArray)
{
	bool bRes = false;

	CBRTFile objBRT;
	CallMListViewCalBack(lpDoc,"Start Open File...",0);
	int nRet = objBRT.OpenAndReadHeader((LPCTSTR)strBRTPath,CFile::modeRead);
	
	if ( NO_ERROR != nRet)
	{
		CallMListViewCalBack(lpDoc,"Open File Error!",-1);
		printf("Conversion failed with error: 0x%X", nRet);
	}
	else
	{
		CallMListViewCalBack(lpDoc,"Open File Success!",0);
		nRet = objBRT.ReadNVItem(NVItemArray);

		if(nRet == NO_ERROR)
			bRes = true;
	}
	objBRT.Close();
	return bRes;
}


void COperator::CallMListViewCalBack(LPVOID pDoc,const char* szInfo,int flag)
{
	if(mListViewCalBack!=NULL)
		mListViewCalBack(lpDoc,szInfo,flag);
}

//////////////////////////////////////////////////////////////////////////////
///////////////////Get NVItem Date by Id from moblie//////////////////////////
//////////////////////////////////////////////////////////////////////////////
int COperator::GetMoblieNVItemData(LPCTSTR com)
{
	_ASSERTE(com != NULL);
	if(strcmp(com, _T("")) == 0)
	{
		CallMListViewCalBack(lpDoc,_T("Please Select ComPort.."), -1);
		return -1;
	}
	CString strCOMNum;
	strCOMNum = com;
	int nIndex =strCOMNum.ReverseFind(_T('M'));
	strCOMNum = strCOMNum.Mid(nIndex+1, strCOMNum.GetLength()-nIndex-1);
	int m_nPortID = _ttoi(strCOMNum);
    CNVItem nv(m_nPortID);
	WORD wNVid = mNVItem.GetItemID();
	tempCNVobj.SetItemID(mNVItem.GetItemID());
	DIAG_NVITEM_PACKET_F_type NVItemReadResp = {0};
	bool nRet = nv.ReadNVItem(wNVid,NVItemReadResp);
	CString strInfo = _T("");
	strInfo.Format(_T("Get mobile NVItemDate (ID=%d)"), wNVid );
	if(nRet && NVItemReadResp.nv_stat==0)
	{
		CallMListViewCalBack(lpDoc,strInfo+_T(" Success!!"), 0);
		tempCNVobj.SetNVContentLen(128);
		tempCNVobj.SetItemContent(NVItemReadResp.item_data);
		return 0;
	}else if(!nRet)
	{
		CallMListViewCalBack(lpDoc,strInfo+_T(", ERROR!!"), -1);
		return -1;
	}
	return -1;
}

CNVItemData * COperator::GetMobCNVItem()
{
	return &tempCNVobj;
}

int COperator::ReadAllNVFromMobile(char* com, int nMin, int nMax)
{
	
	CString szCom = com;
	if(szCom == _T(""))
	{
		CallMListViewCalBack(lpDoc,_T("Please Select ComPort.."), -1);
		return -1;
	}
	szCom = szCom.Mid(3);
	int m_nPortID = _ttoi(szCom);
	CNVItem nv(m_nPortID);
	CString strInfo = _T("");
	BYTE DefaultContent[218] = {0};

	for(int i = nMin; i < nMax+1; i++)
	{
		tempCNVobj.SetItemID(i);
		tempCNVobj.SetNVContentLen(128);


		DIAG_NVITEM_PACKET_F_type NVItemReadResp = {0};
		bool nRet = nv.ReadNVItem(i,NVItemReadResp);
		if(nRet && NVItemReadResp.nv_stat==0)
		{
			strInfo.Format(_T("Read NV Item(%d) Success!!"), i );
			CallMListViewCalBack(lpDoc,strInfo, 0);
			tempCNVobj.SetItemContent(NVItemReadResp.item_data);
		}
		else
		{
			strInfo.Format(_T("Read NV Item(%d) Failed!! error code: %d"), i , NVItemReadResp.nv_stat);
			CallMListViewCalBack(lpDoc,strInfo, -1);
			tempCNVobj.SetItemContent(DefaultContent);
		}

		m_NVItemArrayReverse.Add(tempCNVobj);
	}
	mFillTreeCalBack(lpDoc,m_NVItemArrayReverse);
	return 0;
}