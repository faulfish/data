#include "StdAfx.h"
#include "CommISyncML.h"

CCommISyncML::CCommISyncML(void)
{
   
}

CCommISyncML::~CCommISyncML(void)
{

}

void CCommISyncML::SetOperationTimeOut(int nTimeOut)
{

}
 
int CCommISyncML::Get(BYTE **ppFileData, DWORD& nSize)
{
   int		eInterface = -1;
   LPVOID	pInterface;
   ISYNCML  *pISyncML;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISyncML) 
   {
	 pISyncML = (ISYNCML*)pInterface;
	 return pISyncML->Get( ppFileData, nSize );
   }
   else
     return eCommErrInterfaceLocked;
}

int CCommISyncML::Put(BYTE *pFileData, DWORD nSize)
{
   int		eInterface = -1;
   LPVOID	pInterface;
   ISYNCML  *pISyncML;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISyncML) 
   {
	 pISyncML = (ISYNCML*)pInterface;
	 return pISyncML->Put( pFileData, nSize );
   }
   else
     return eCommErrInterfaceLocked;
}

int CCommISyncML::Open()
{
   int		eInterface = -1;
   LPVOID	pInterface;
   ISYNCML  *pISyncML;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISyncML) 
   {
	 pISyncML = (ISYNCML*)pInterface;
	 return pISyncML->Open();
   }
   else
     return eCommErrInterfaceLocked;
}

int CCommISyncML::Close()
{
   int		eInterface = -1;
   LPVOID	pInterface;
   ISYNCML  *pISyncML;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISyncML) 
   {
	 pISyncML = (ISYNCML*)pInterface;
	 return pISyncML->Close();
   }
   else
     return eCommErrInterfaceLocked;
}