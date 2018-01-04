/*********************************************************************/
/** @file:	IQTreeUI.h

Defines the IQTreeUI interface.

$Original Author:    YC Liang  $
$Version:	1.0 $
$Date:   Apr 01 2009 14:17:24  $
*/
/*********************************************************************/

#if !defined(EA_DAD34693_43B2_44e0_A65A_19ED5D59E109__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_19ED5D59E109__INCLUDED_

#include "..\..\..\CommonUtil\NotifyBase.h"
#include ".\QControlNotify.h"
#include <vector>
#include "..\CommUIExportAPI.h"

namespace QSYNC_LIB
{

	class COMMONUI_API CQTreeNode
	{
	public:	
		CQTreeNode(CQTreeNode *pParent):
		  m_bFocus(false),
			  m_bSelected(false),
			  m_bChecked(false),
			  m_bExpanded(false),
			  m_pParent(NULL),
			  m_nSelectedImageID(0),
			  m_nUnselectedImageID(0),
			  m_bHasChild(false),
			  m_strSeparator(_T("/"))
		  {
			  SetParent(pParent);
			  if(pParent)
				  pParent->AddSubNode(this);
		  }
		  virtual ~CQTreeNode()
		  {	
			  if(!IsRoot())
			  {
				  for(std::vector<CQTreeNode*>::iterator Iter = m_pParent->GetSubNode().begin();Iter != m_pParent->GetSubNode().end();Iter++)
				  {
					  if(this == *Iter)
					  {
						  m_pParent->GetSubNode().erase(Iter);
						  break;
					  }
				  }		
			  }
		  }

		  CQTreeNode* GetChild(int nIndex) const
		  {
			  CQTreeNode* pRet = NULL;
			  if(nIndex >= 0 && nIndex < GetChildSize())
				  pRet = m_vct_pChilds[nIndex];
			  return pRet;
		  }

		  int GetChildSize() const
		  {
			  return static_cast<int>(m_vct_pChilds.size());
		  }

		  bool IsRoot() const
		  {
			  return m_pParent == NULL;
		  }

		  bool ItemHasChildren(void) const
		  {
			  return m_vct_pChilds.size() > 0;
		  }

		  const TSTRING& GetName() const
		  {
			  return m_strName;
		  }

		  void SetName(const TCHAR* strName)
		  {
			  assert(strName);
			  if(strName)
				m_strName = strName;
		  }

		  const TSTRING& GetDisplayName() const
		  {
			  return m_strDisplayText;
		  }

		  void SetDisplayName(const TCHAR* strDisplayName)
		  {
			  assert(strDisplayName);
			  if(strDisplayName)
				m_strDisplayText = strDisplayName;
		  }

		  const LPARAM& GetLPARAM() const
		  {
			  return m_pData;
		  }

		  void SetLPARAM(LPARAM pData)
		  {
			  m_pData = pData;
		  }

		  CQTreeNode* GetParent() const
		  {
			  return m_pParent;
		  }

		  const CQTreeNode* GetRoot() const
		  {
			  const CQTreeNode* pTemp = this;
			  while(pTemp->GetParent() != NULL)
				  pTemp = pTemp->GetParent();
			  return pTemp;
		  }

		  const CQTreeNode* FindChildByName(const TSTRING& strName) const
		  {
			  //Use this function must avoid the duplicate name issue
			  //We allow duplicate name in the tree and always find the first node that it can find
			  const CQTreeNode* pRetNode = NULL;
			  for(std::vector<CQTreeNode*>::const_iterator Iter = m_vct_pChilds.begin();Iter != m_vct_pChilds.end();Iter++)
			  {
				  if((*Iter)->GetName() == strName)
				  {
					  pRetNode = *Iter;
					  break;
				  }
			  }
			  return pRetNode;
		  }

		  TSTRING GetFullPath() const
		  {
			  TSTRING strRet;
			  if(IsRoot())
				  strRet = GetName();
			  else
			  {
				  const CQTreeNode* pParentNode = GetParent();
				  if(pParentNode)
				  {
					  TSTRING strParent = pParentNode->GetFullPath() == GetSeparator().c_str() ? _T("") : pParentNode->GetFullPath();
					  strRet =  strParent + GetSeparator().c_str() + GetName();
				  }
			  }
			  return strRet;
		  }

		  const CQTreeNode* GetTreeNodeByFullPath(const TSTRING& strFullPath)
		  {//Sample /Pictures/Logo/abc.jpg			  
			  const CQTreeNode* pTempNode = NULL;

			  TSTRING strTempPath = strFullPath;
			  while(!strTempPath.empty())
			  {
				  TSTRING strFolder;
				  TSTRING::size_type nIndex = strTempPath.find(m_strSeparator.c_str());
				  
				  if(nIndex == TSTRING::npos)
				  {
					  if(pTempNode == NULL)
						  pTempNode = GetRoot();
					  else
					  {
						  TSTRING strName = strTempPath;
						  strTempPath.clear();
						  pTempNode = pTempNode->FindChildByName(strName);
					  }
				  }
				  else if(nIndex == 0)
				  {//Is root
					  pTempNode = GetRoot();
					  strTempPath = strTempPath.substr(nIndex + 1);
				  }
				  else if(nIndex > 0)
				  {
					  if(pTempNode == NULL)
					  {
						  pTempNode = GetRoot();
					  }
					  else
					  {
						  TSTRING strName = strTempPath.substr(0,nIndex);
						  pTempNode = pTempNode->FindChildByName(strName);
						  assert(pTempNode);
					  }					  
					  strTempPath = strTempPath.substr(nIndex+1);
				  }
				  else assert(0);
			  };

			  return pTempNode;
		  }

		  bool operator==(const CQTreeNode& prm1) const 
		  {
			  bool bRes = false;
			  if( m_pParent		 == prm1.m_pParent		  &&
				  m_strName		 == prm1.m_strName		  &&
				  m_strDisplayText == prm1.m_strDisplayText &&
				  m_vct_pChilds	 == prm1.m_vct_pChilds		)
			  {
				  bRes = true;
			  }
			  return bRes;
		  }

		  bool operator<(const CQTreeNode& prm1) const 
		  {
			  return false;
		  }

		  const TSTRING& GetSeparator() const
		  {
			  return m_strSeparator;
		  }

		  void  SetSeparator(const TSTRING& strSeparator)
		  {
			  m_strSeparator = strSeparator;
		  }

		  bool				m_bFocus;
		  bool				m_bSelected;
		  bool				m_bChecked;
		  bool				m_bHasChild;
		  bool				m_bExpanded;		
		  int				m_nUnselectedImageID;
		  int				m_nSelectedImageID;

		  std::wstring		m_strImagePath;//BMP, ICON, GIF, JPEG, Exif, PNG, TIFF, WMF, and EMF.
		  std::wstring		m_strSelectedImagePath;//BMP, ICON, GIF, JPEG, Exif, PNG, TIFF, WMF, and EMF.

	private:
		void AddSubNode(const CQTreeNode* pNode)
		{
			if(pNode != NULL)
				m_vct_pChilds.push_back(const_cast<CQTreeNode*>(pNode));
		}
		void SetParent(const CQTreeNode* pNode)
		{
			m_pParent = const_cast<CQTreeNode*>(pNode);
		}		
		std::vector<CQTreeNode*>& GetSubNode()
		{
			return m_vct_pChilds;
		}
	private:
		TSTRING						m_strName;		
		std::vector<CQTreeNode*>	m_vct_pChilds;
		TSTRING						m_strDisplayText;
		LPARAM						m_pData;
		CQTreeNode*					m_pParent;
		TSTRING						m_strSeparator;
	};

	class CTreeEditParam
	{
	public:
		CTreeEditParam():m_bEditable(false),m_bFinishEdit(false),m_pNode(NULL){}
		void SetEditable(bool bEditable){m_bEditable = bEditable;}
		bool GetEditable() const {return m_bEditable;}		
		void SetEditString(const TSTRING& strEditString){m_strEditString = strEditString;}		
		const TSTRING& GetEditString() const{return m_strEditString;}
		void SetFinishEdit(bool bFinishEdit){m_bFinishEdit = bFinishEdit;}
		bool GetFinishEdit() const {return m_bFinishEdit;}
		void SetNode(CQTreeNode* pNode){m_pNode = pNode;}
		CQTreeNode* GetNode() const {return m_pNode;}
	private:
		CQTreeNode*	m_pNode;
		bool		m_bEditable;
		bool		m_bFinishEdit;
		TSTRING		m_strEditString;
	};

	interface IQTreeUI
	{
		// Tree Node Manipulation
		virtual bool CreateNode(CQTreeNode* pParentNode, CQTreeNode* pNode) = 0;
		virtual bool DeleteNode(CQTreeNode* pNode) = 0;
		virtual bool ExpandNode(CQTreeNode* pNode, bool bExpandFlag) = 0;
		virtual CQTreeNode* GetRoot() = 0;
		virtual CQTreeNode* GetCurrentNode() = 0;

		virtual bool UpdateNode(CQTreeNode* pNode) = 0;
		// Event Notification
		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
		virtual bool Register(INotify* pINotify, const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pINotify, const TSTRING& strEvent) = 0;
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
		virtual void RefreshClear() = 0;
	};

}

#endif // !defined(EA_DAD34693_43B2_44e0_A65A_19ED5D59E109__INCLUDED_)
