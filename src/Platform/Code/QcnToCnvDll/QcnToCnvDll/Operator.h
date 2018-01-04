#pragma once

#include "CBRTFile.h"
#include "CNVFile.h"
#include "CNVItem.h"
#include <vector>

typedef CArray<CNVItemData,CNVItemData&> NVArray;
typedef void (*FillTreeFunc)(LPVOID pDoc,NVArray & mNVDataBase);
typedef void (*TreeViewFunc)(LPVOID pDoc,std::vector<CNVItemData>& NVItemArray);
typedef void (*EditViewFunc)(LPVOID pDoc,CNVItemData & mCNVItem);
typedef void (*ListViewFunc)(LPVOID pDoc,const char* szInfo,int flag);

class COperator
{
public:

	COperator(void);
	~COperator(void);		
	
	int SaveQcnFileAndDUFileToBRTFileFunction_part2(const CString& strDU,const CString& strQCN,const CString& strBRT);
	// Steps To Change From QcnFiles To CnvFiles
	int SaveQcnFileAndDUFileToCnvFileFunction_part2(const CString& strDU,const CString& strQCN,const CString& strCNV);
	int SaveQcnFileToBrtFileFunction_part2(const CString& strQCN,const CString& strBRT);
	// Steps To Change From CnvFiles To QcnFiles
	int SaveCnvFileToQcnFileFunction(const CString& strCNV,const CString& strQCN);
	// Steps To Change From CnvFiles To QcnFiles
	int SaveBrtFileToQcnFileFunction(const CString& strBRT,const CString& strQCN);
////////////////////////////////////////
	bool FileToNVArray(const CString& strPath,const int fileType);
	void SetCalBack(LPVOID pDoc, LPVOID pTreeView,  LPVOID pTreeView2,LPVOID pEditView, LPVOID pListView);
	int  GetNVItem(int mNVItemID);
	int  SaveNVItemData(int itemId,BYTE* itemContent);
	int  SaveNVItemFile(CString &filePath,int bFileType,int fileType);
	void CallMListViewCalBack(LPVOID pDoc,const char* szInfo,int flag);
	int  SaveNVItemToBrtFile(int bFileType,CString &filePath);
	int  SaveNVItemToCnvFile(int bFileType,CString &filePath);
	int  SaveNVItemToQcnFile(int bFileType,CString &filePath);
	int  SortNVItemAscByID();
	int  SortNVItemDesByID();
	int  GetMoblieNVItemData(LPCTSTR szCOM);

	int ReadAllNVFromMobile(char* com, int nMin, int nMax);
	//int  ReadNVItemData();
	//int  ReadArrayItem();
	CNVItemData * GetMobCNVItem();
////////////////////////////////////////
protected:
	bool SaveQcnFileToNVArray(const CString& strQCNPath,std::vector<CNVItemData>& NVItemArray);
	bool SaveDUFileToNVArray(const CString& strDUPath,std::vector<CNVItemData>& NVItemArray);
	bool FinishTheCNVFile(CCNVFile& objCNV,const CString& CNVFileName);	
	bool FinishTheBRTFile(CBRTFile& objCNV,const CString& BRTFileName);
	int  CreateCRCValue(const CString& CNVFileName);
	unsigned short CountCRC_M(unsigned char* buf, ULONGLONG count);	
	bool GetNVBlock(FILE* hDuckFile, int nNvId, CNVItemData& nvItem);
	bool OutputFile(const CString& strFilePath,BYTE* pContent,int nLBufferLength);
	bool HandleNVFile(FILE* hDuckFile,const CString& strFilePath, bool bCreateFile=true);
	bool CreatePath(std::string &wsPath);
	bool AddToNVArrar(FILE* hDuckFile,int intData,std::vector<CNVItemData>& NVItemArray);

	bool SaveCnvFileToNVArray(const CString& strQCNPath,CArray<CNVItemData,CNVItemData&>& NVItemArray);
	bool SaveBrtFileToNVArray(const CString& strBRTPath,CArray<CNVItemData,CNVItemData&>& NVItemArray);
private:
	std::vector<CNVItemData> m_NVItemArray;
	NVArray		 m_NVItemArrayReverse;
	FillTreeFunc mFillTreeCalBack;
	TreeViewFunc mVecTreeCalBack;
	EditViewFunc mEditViewCalBack;
	ListViewFunc mListViewCalBack;
	LPVOID		 lpDoc;
	CNVItemData	 mNVItem;
	CNVItemData	 tempCNVobj;
	int			 m_FileType;
};
