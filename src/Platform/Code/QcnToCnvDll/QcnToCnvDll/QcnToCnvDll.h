// QcnToCnvDll.h : main header file for the QcnToCnvDll DLL
//

#include "Operator.h"



#define QCNTOCNVDLL extern "C" _declspec(dllexport) 

typedef CArray<CNVItemData,CNVItemData&> NVArray;

QCNTOCNVDLL int QcnAndDUToBRTFunction(const CString& strPathDU,const CString& strPathQCN,const CString& strPathBRT);
QCNTOCNVDLL int QcnAndDUToCnvFunction(const CString& strPathDU,const CString& strPathQCN,const CString& strPathCNV);
QCNTOCNVDLL int QcnToBrtFunction(const CString& strPathQCN,const CString& strPathBRT);



QCNTOCNVDLL int CnvToQcnFunction(const CString &strPathCNV,const CString &strPathQCN );
QCNTOCNVDLL int BrtToQcnFunction(const CString &strPathBRT,const CString &strPathQCN );

///////////////////////////////

QCNTOCNVDLL int		FileToNVArray(HANDLE hResource,const CString &strPath,const int fileType);
QCNTOCNVDLL HANDLE  GetOperatorHandle();
QCNTOCNVDLL void	SetCallBackFuncs(HANDLE hResource,LPVOID pDoc, LPVOID pTreeView,LPVOID pTreeView2, LPVOID pEditView, LPVOID pListView);
QCNTOCNVDLL int		GetNVItemObject(HANDLE hResource, int mNVItemID);
QCNTOCNVDLL int		SaveNVItemObject(HANDLE hResource,int itemId,BYTE* itemContent);
QCNTOCNVDLL int		SaveNVItemFileChange(HANDLE hResource,CString & filePath,int bFileType,int fileType);
QCNTOCNVDLL int		SortAscByID(HANDLE hResource);
QCNTOCNVDLL int		SortDesByID(HANDLE hResource);
QCNTOCNVDLL int		GetMoblieNVItemData(HANDLE hResource,char * com);
QCNTOCNVDLL CNVItemData*GetMobCNVItem(HANDLE hResource);
QCNTOCNVDLL int		CloseOperatorHandle(HANDLE hResource);
QCNTOCNVDLL int		GetAllNVFromMobile(HANDLE hResource, char* com, int nMin, int nMax);