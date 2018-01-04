#include "StdAfx.h"
#include "CommIPhonebook.h"
#include "MobileInterface.h"

CCommIPhonebook::CCommIPhonebook(void)
{
   
}

CCommIPhonebook::~CCommIPhonebook(void)
{

}

void CCommIPhonebook::SetOperationTimeOut(int nTimeOut)
{
}
  
bool CCommIPhonebook::IsPhonebookTypeSupport(ePhonebookType eType)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   IPHONEBOOK *pIPhonebook;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eIPhonebook) 
   {
	 pIPhonebook = (IPHONEBOOK*)pInterface;
	 return pIPhonebook->IsPhonebookTypeSupport(eType);
   }
   else
     return false;
}

bool CCommIPhonebook::GetPhonebookRange(ePhonebookType eType, UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   IPHONEBOOK *pIPhonebook;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eIPhonebook) 
   {
	 pIPhonebook = (IPHONEBOOK*)pInterface;
	 return pIPhonebook->GetPhonebookRange(eType, nMinIdx, nMaxIdx, nMaxCount);
   }
   else
     return false;
}

bool CCommIPhonebook::ListPhonebookIndex(ePhonebookType eType, std::vector<UINT> *vIdxList)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   IPHONEBOOK *pIPhonebook;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eIPhonebook) 
   {
	 pIPhonebook = (IPHONEBOOK*)pInterface;
	 return pIPhonebook->ListPhonebookIndex(eType, vIdxList);
   }
   else
     return false;
}

int CCommIPhonebook::ReadVCardbyIndex(ePhonebookType eType, UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   IPHONEBOOK *pIPhonebook;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eIPhonebook) 
   {
	 pIPhonebook = (IPHONEBOOK*)pInterface;
	 return pIPhonebook->ReadVCardbyIndex(eType, nIdx, cVCardbuf, nBufMaxLen, nVcardLen);
   }
   else
     return eCommErrInterfaceLocked;
}

int CCommIPhonebook::WriteVCardbyIndex(ePhonebookType eType, UINT& nIdx, int& nRetError, char *cVCardbuf, int nBufLen)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   IPHONEBOOK *pIPhonebook;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eIPhonebook) 
   {
	 pIPhonebook = (IPHONEBOOK*)pInterface;
	 return pIPhonebook->WriteVCardbyIndex(eType, nIdx, nRetError, cVCardbuf, nBufLen) != 0 ? eCommRetSuccess : eCommErrInterfaceLocked;
   }
   else
     return eCommErrInterfaceLocked;
}

int CCommIPhonebook::RemoveVCardbyIndex(ePhonebookType eType, UINT nIdx)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   IPHONEBOOK *pIPhonebook;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eIPhonebook) 
   {
	 pIPhonebook = (IPHONEBOOK*)pInterface;
	 return pIPhonebook->RemoveVCardbyIndex(eType, nIdx);
   }
   else
     return eCommErrInterfaceLocked;
}