#pragma once

#include "..\..\Components\UIFramework\porting\SubMenuBase.h"

class CMEListSubMenu : public QSYNC_LIB::SubMenuBase
{	
public:
	CMEListSubMenu():m_nFileSelectedCount(0)
	{
		SubMenuItem DownloadItem = {_T("IDS_FileMgr_Tree_Download"),EVENT_ME_LIST_SUBMENU_DOWNLOAD_NODE};
		AddItem(DownloadItem);
		SubMenuItem DeleteItem = {_T("IDS_Delete"),EVENT_ME_LIST_SUBMENU_DELETE_NODE};
		AddItem(DeleteItem);
		SubMenuItem RenameItem = {_T("IDS_FileMgr_List_Rename"),EVENT_ME_LIST_SUBMENU_RENAME_NODE};
		AddItem(RenameItem);
	}
    virtual ~CMEListSubMenu(void){};
	void SetFileSelectedCount(int nFileSelectedCount){m_nFileSelectedCount = nFileSelectedCount;}
	virtual bool IsEnable(int nIndex) 
	{
		bool bRes = true;
		if(nIndex == 2 && m_nFileSelectedCount != 1)//the RENAME item
			bRes = false;
		return bRes;
	}
private:
	int m_nFileSelectedCount;
};