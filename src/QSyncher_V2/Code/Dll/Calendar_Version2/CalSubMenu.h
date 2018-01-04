#ifndef _CCALSUBMENU_HPP_
#define _CCALSUBMENU_HPP_
#endif
#pragma once

#include "..\..\Components\UIFramework\interface\ISubMenu.h"
#include "..\..\Components\Localization\ilocalization.h"

class CalSubMenu : public QSYNC_LIB::ISubMenu
{	
public:
	CalSubMenu():m_pILocalization(NULL), nMenuItemCount(0)
	{
		CreateILocalization(&m_pILocalization);
	}
	void SetMenuItemCount(int nCount){nMenuItemCount = nCount;}
	virtual int GetMenuItemCount(){return nMenuItemCount;}
	virtual bool GetMenuItem(int nIndex, TSTRING& strDisplayID, TSTRING& strNotifyID)
	{
		bool bRes = false;
		ASSERT(m_pILocalization);
		if(m_pILocalization)
		{
			if(nIndex == 0)
			{
				strDisplayID = m_pILocalization->GetString(_T("IDS_Insert"));
				strNotifyID = _T("TNotify_APPOINTMENTAdd");
				bRes = true;
			}
			else if(nIndex == 1 && nMenuItemCount == 3)
			{
				strDisplayID = m_pILocalization->GetString(_T("IDS_Update"));
				strNotifyID = _T("TNotify_APPOINTMENTEdit");
				bRes = true;
			}
			else if(nIndex == 2 || (nIndex == 1 && nMenuItemCount == 2))
			{
				strDisplayID = m_pILocalization->GetString(_T("IDS_Delete"));
				strNotifyID = _T("TNotify_APPOINTMENTDelete");
				bRes = true;
			}
		}
		return bRes;
	}
private:
	QSYNC_LIB::ILocalization* m_pILocalization;	
	int nMenuItemCount;
};