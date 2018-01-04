// QcnToCnvDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QcnToCnvDll.h"


// Steps To Change From QcnFiles To CnvFiles
QCNTOCNVDLL int QcnAndDUToBRTFunction(const CString& strPathDU,const CString& strPathQCN,const CString& strPathBRT)
{
	COperator Operator;
	Operator.SaveQcnFileAndDUFileToBRTFileFunction_part2(strPathDU,strPathQCN,strPathBRT);
	return 0;	
}

// Steps To Change From QcnFiles To CnvFiles
QCNTOCNVDLL int QcnAndDUToCnvFunction(const CString& strPathDU,const CString& strPathQCN,const CString& strPathCNV)
{
	COperator Operator;
	Operator.SaveQcnFileAndDUFileToCnvFileFunction_part2(strPathDU,strPathQCN,strPathCNV);
	return 0;	
}

QCNTOCNVDLL int QcnToBrtFunction(const CString& strPathQCN,const CString& strPathBRT)
{
	COperator Operator;
	Operator.SaveQcnFileToBrtFileFunction_part2(strPathQCN,strPathBRT);
	return 0;
}

// Steps To Change From QcnFiles To CnvFiles
QCNTOCNVDLL int CnvToQcnFunction(const CString &strPathCNV,const CString &strPathQCN )
{
	COperator Operator;
	Operator.SaveCnvFileToQcnFileFunction(strPathCNV,strPathQCN);
	return 0;

}

// Steps To Change From QcnFiles To CnvFiles
QCNTOCNVDLL int BrtToQcnFunction(const CString &strPathBRT,const CString &strPathQCN )
{
	COperator Operator;
	Operator.SaveBrtFileToQcnFileFunction(strPathBRT,strPathQCN);
	return 0;

}


QCNTOCNVDLL int FileToNVArray(HANDLE hResource,const CString& filePath,const int fileType)
{
	COperator* pQDoperator = (COperator*)hResource;
	pQDoperator->FileToNVArray(filePath,fileType);
	return 0;
}

QCNTOCNVDLL HANDLE  GetOperatorHandle()
{
	COperator* pQDoperator  = new COperator;
	return (HANDLE)pQDoperator;
}

QCNTOCNVDLL void SetCallBackFuncs(HANDLE hResource,LPVOID pDoc, LPVOID pTreeView,LPVOID pTreeView2, LPVOID pEditView, LPVOID pListView)
{
	COperator* pQDoperator = (COperator* )hResource;
	pQDoperator->SetCalBack(pDoc,pTreeView,pTreeView2,pEditView,pListView);
}


QCNTOCNVDLL int GetNVItemObject(HANDLE hResource, int mNVItemID)
{
	COperator* pQDoperator = (COperator* )hResource;
	return pQDoperator->GetNVItem(mNVItemID);
}

QCNTOCNVDLL int SaveNVItemObject(HANDLE hResource,int itemId,BYTE* itemContent)
{
	COperator* pQDoperator = (COperator* )hResource;
	return pQDoperator->SaveNVItemData(itemId,itemContent);
}


QCNTOCNVDLL int SaveNVItemFileChange(HANDLE hResource,CString & filePath,int bFileType,int fileType)
{
	COperator* pQDoperator = (COperator* )hResource;
	return pQDoperator->SaveNVItemFile(filePath,bFileType,fileType);
}

QCNTOCNVDLL int	SortAscByID(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	COperator* pQDoperator = (COperator* )hResource;
	return pQDoperator->SortNVItemAscByID();
}
QCNTOCNVDLL int	SortDesByID(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	COperator* pQDoperator = (COperator* )hResource;
	return pQDoperator->SortNVItemDesByID();
}

QCNTOCNVDLL int	GetMoblieNVItemData(HANDLE hResource,char * com)
{
	ASSERT(hResource != NULL);
	COperator* pQDoperator = (COperator* )hResource;
	return pQDoperator->GetMoblieNVItemData(com);
}
QCNTOCNVDLL int  CloseOperatorHandle(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	COperator* pQDoperator = (COperator* )hResource;
	delete pQDoperator;
	pQDoperator = NULL;
	return 0;
}


QCNTOCNVDLL CNVItemData *  GetMobCNVItem(HANDLE hResource)
{
	ASSERT(hResource != NULL);
	COperator* pQDoperator = (COperator*)hResource;
	return pQDoperator->GetMobCNVItem();
}

QCNTOCNVDLL int GetAllNVFromMobile(HANDLE hResource,char* com,  int nMin, int nMax)
{
	ASSERT(hResource != NULL);
	COperator *pQDoperator = (COperator*)hResource;
	return pQDoperator->ReadAllNVFromMobile(com,nMin, nMax);

}