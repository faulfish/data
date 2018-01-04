#include "StdAfx.h"
#include "CommISMS.h"
#include "MobileInterface.h"

CCommISMS::CCommISMS(void)
{
   
}

CCommISMS::~CCommISMS(void)
{

}

HRESULT CCommISMS::GetServiceCenterAddress(CString* pcstrSCA)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->GetServiceCenterAddress(pcstrSCA);
   }
   else
     return eCommErrInterfaceLocked;
}
   
HRESULT CCommISMS::SetPDUMode()
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->SetPDUMode();
   }
   else
     return eCommErrInterfaceLocked;
}   

HRESULT CCommISMS::SetMessageStorage(enumStorage eMsgStorage, MemUsed& used)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->SetMessageStorage(eMsgStorage, used);
   }
   else
     return eCommErrInterfaceLocked;
}
   
HRESULT CCommISMS::QueryMessageSpace(enumStorage eMsgStorage, UINT* puiUsed, UINT* puiTotal)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->QueryMessageSpace(eMsgStorage, puiUsed, puiTotal);
   }
   else
     return eCommErrInterfaceLocked;
}

HRESULT CCommISMS::ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>* vecSM, std::vector<CShortMessage>* vecShortMessage)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->ListMessage(eMsgLoc, pcstrMsgIDList, vecSM, vecShortMessage);
   }
   else
     return eCommErrInterfaceLocked;
}

HRESULT CCommISMS::SendMessage(BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->SendMessage(pMsgData, uiMsgDataSize, puiMsgID);
   }
   else
     return eCommErrInterfaceLocked;
}
   
HRESULT CCommISMS::SendMessageInStorage(UINT uiMsgID, CString cstrAddress)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->SendMessageInStorage(uiMsgID, cstrAddress);
   }
   else
     return eCommErrInterfaceLocked;
}
   
HRESULT CCommISMS::WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->WriteMessage(eMsgLoc, pMsgData, uiMsgDataSize, puiMsgID);
   }
   else
     return eCommErrInterfaceLocked;
}

HRESULT CCommISMS::WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID, char* szTime)
{
	LockMobileInterface lock;

    int    eInterface = -1;
    LPVOID pInterface;
    ISMS   *pISMS;

    // interface is not locked
    if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
    {
        pISMS = (ISMS*)pInterface;
        return pISMS->WriteMessage(eMsgStorage, nFolderID, nTag, pMsgData, uiMsgDataSize, puiMsgID, szTime);
    }
    else
        return eCommErrInterfaceLocked;
}
   
HRESULT CCommISMS::ReadMessage(UINT uiMsgID, BYTE* pMsgData, UINT* puiMsgDataSize)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->ReadMessage(uiMsgID, pMsgData, puiMsgDataSize);
   }
   else
     return eCommErrInterfaceLocked;
}

HRESULT CCommISMS::ReadMessage(UINT uiMsgID, CShortMessage& csm)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;

    // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->ReadMessage(uiMsgID, csm);
   }
   else
     return eCommErrInterfaceLocked;

}

HRESULT CCommISMS::DeleteMessage(UINT uiMsgID)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->DeleteMessage(uiMsgID);
   }
   else
     return eCommErrInterfaceLocked;
}

// get sub folders by folder ID
HRESULT CCommISMS::GetFolderEntries(BOOL bGetRootFolder, UINT nParentFolderID, vector<CMessageFolderInfo> *vEntryList)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->GetFolderEntries(bGetRootFolder, nParentFolderID, vEntryList);
   }
   else
     return eCommErrInterfaceLocked;
}
// create or rename message sub folders by folder ID
HRESULT CCommISMS::WriteFolderEntries(BOOL bCreate, BOOL bCreateRootFolder, UINT nParentFolderID, UINT nRenameFolderID, CString strDiaplayName, UINT &nNewID)
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->WriteFolderEntries(bCreate, bCreateRootFolder, nParentFolderID, nRenameFolderID, strDiaplayName, nNewID);
   }
   else
     return eCommErrInterfaceLocked;
}
// delete sub folders by folder ID
HRESULT CCommISMS::DeleteFolderEntry( UINT nFolderID )
{
	LockMobileInterface lock;

   int    eInterface = -1;
   LPVOID pInterface;
   ISMS   *pISMS;
   
   // interface is not locked
   if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
   {
	 pISMS = (ISMS*)pInterface;
	 return pISMS->DeleteFolderEntry(nFolderID);
   }
   else
     return eCommErrInterfaceLocked;
}

HRESULT CCommISMS::UpdateMessageState(UINT uiMsgID, UINT nState /* = 0 */) 
{
	LockMobileInterface lock;

    int    eInterface = -1;
    LPVOID pInterface;
    ISMS   *pISMS;

    // interface is not locked
    if (GetInterface( eInterface, pInterface ) && eInterface == eISMS) 
    {
        pISMS = (ISMS*)pInterface;
        return pISMS->UpdateMessageState(uiMsgID, nState);
    }
    else
        return eCommErrInterfaceLocked;
}