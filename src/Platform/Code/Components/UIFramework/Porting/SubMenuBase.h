#ifndef _SUB_MENU_BASE_H_
#define _SUB_MENU_BASE_H_

#pragma once

#include "..\interface\ISubMenu.h"
#include "..\..\Localization\ilocalization.h"
#include <vector>

namespace QSYNC_LIB 
{
	struct SubMenuItem
	{
		TSTRING m_strDisplayID;
		TSTRING strNotifyID;
	};

	class SubMenuBase : public ISubMenu
	{	
	public:
		SubMenuBase():m_pILocalization(NULL)
		{
			CreateILocalization(&m_pILocalization);
		}
		virtual ~SubMenuBase(void){};

		virtual int GetMenuItemCount()
		{
			return static_cast<int>(m_SubMenumItems.size());
		}
		virtual bool GetMenuItem(int nIndex, TSTRING& strDisplayID, TSTRING& strNotifyID)
		{
			bool bRes = false;
			ASSERT(m_pILocalization);
			if(m_pILocalization)
			{
				strDisplayID = m_pILocalization->GetString(m_SubMenumItems[nIndex].m_strDisplayID);
				strNotifyID = m_SubMenumItems[nIndex].strNotifyID.c_str();
				bRes = true;
			}
			return bRes;
		}
		virtual bool IsEnable(int m_nIndex)
		{
			return true;
		}
		virtual bool IsChecked(int m_nIndex)
		{
			return false;
		}
		void RemoveItem(const TSTRING& strNotifyID)
		{
			for(std::vector<SubMenuItem>::iterator Iter = m_SubMenumItems.begin();Iter != m_SubMenumItems.end();Iter++)
			{
				if(Iter->strNotifyID == strNotifyID)
				{
					m_SubMenumItems.erase(Iter);
					break;
				}
			}
		}
		int AddItem(const SubMenuItem& menuItem , int nIndex = -1)
		{
			int nNewIndex = -1;
			std::vector<SubMenuItem>::iterator Iter = m_SubMenumItems.begin();
			if(nIndex == -1)
			{
				m_SubMenumItems.push_back(menuItem);
				nNewIndex = static_cast<int>(m_SubMenumItems.size() - 1);
			}
			else
			{
				advance(Iter,nIndex);
				m_SubMenumItems.insert(Iter, menuItem);
				nNewIndex = nIndex;
			}
			return nNewIndex;
		}
	protected:
		ILocalization* m_pILocalization;	
		std::vector<SubMenuItem> m_SubMenumItems;
	};
}
#endif //_SUB_MENU_BASE_H_