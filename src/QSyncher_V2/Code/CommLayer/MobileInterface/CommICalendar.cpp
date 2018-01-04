#include "StdAfx.h"
#include "CommICalendar.h"
#include "MobileInterface.h"

CCommICalendar::CCommICalendar(void)
{   
}

CCommICalendar::~CCommICalendar(void)
{
}

bool CCommICalendar::SetOperationTimeOut(int nTimeOut)
{
	return false;
}

bool CCommICalendar::GetRange(UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   ICALENDAR *pICalendar;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eICalendar) 
   {
	 pICalendar = (ICALENDAR*)pInterface;
	 return pICalendar->GetRange(nMinIdx, nMaxIdx, nMaxCount);
   }
   else
     return false;
}

bool CCommICalendar::ListIndex(std::vector<UINT> *vIdxList)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   ICALENDAR *pICalendar;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eICalendar) 
   {
	 pICalendar = (ICALENDAR*)pInterface;
	 return pICalendar->ListIndex(vIdxList);
   }
   else
     return false;
}

bool CCommICalendar::ReadByIndex(UINT nIdx, char *szbuf, int nBufMaxLen, int &nVcardLen)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   ICALENDAR *pICalendar;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eICalendar) 
   {
	 pICalendar = (ICALENDAR*)pInterface;
	 return pICalendar->ReadByIndex(nIdx, szbuf, nBufMaxLen, nVcardLen);
   }
   else
     return false;
}

bool CCommICalendar::WriteByIndex(UINT& nIdx, char *szbuf, int nBufLen)
{  
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   ICALENDAR *pICalendar;
    
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eICalendar) 
   {
	 pICalendar = (ICALENDAR*)pInterface;
	 return pICalendar->WriteByIndex(nIdx, szbuf, nBufLen);
   }
   else
     return false;
}

bool CCommICalendar::RemoveByIndex(UINT nIdx)
{
	LockMobileInterface lock;

   int		  eInterface = -1;
   LPVOID     pInterface;
   ICALENDAR *pICalendar;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eICalendar) 
   {
	 pICalendar = (ICALENDAR*)pInterface;
	 return pICalendar->RemoveByIndex( nIdx);
   }
   else
     return false;
}