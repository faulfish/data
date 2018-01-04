#pragma once

#include "..\..\Components\UIFramework\porting\SubMenuBase.h"

class CFileSubMenu : public SubMenuBase
{	
public:
	CFileSubMenu():m_pFileNode(NULL)
	{	
		SubMenuItem Item1 = {_T("IDS_FileMgr_Tree_InsertFolder"),EVENT_FILE_SUBMENU_INSERT_NODE};
		AddItem(Item1);
		SubMenuItem Item2 = {_T("IDS_FileMgr_Tree_DeleteFolder"),EVENT_FILE_SUBMENU_DELETE_NODE};
		AddItem(Item2);
	}
	virtual ~CFileSubMenu(void){}
	virtual bool IsEnable(int m_nIndex) 
	{
		bool bRes = true;
		if(m_nIndex == 1)
		{
			if(m_pFileNode && m_pFileNode->IsRoot())
				bRes = false;
			else
				bRes = !(m_nFileAttr & FILE_ATTRIBUTE_READONLY);
		}
		return bRes;
	}
	void SetFolderNode(CCommonFileNode *pFileNode)
	{
		m_pFileNode = pFileNode;
		if(m_pFileNode)
			m_nFileAttr = m_pFileNode->GetFileAttr();
	}
	CCommonFileNode* GetFolderNode() const {return m_pFileNode;}
private:
	unsigned int m_nFileAttr;
	CCommonFileNode* m_pFileNode;
};