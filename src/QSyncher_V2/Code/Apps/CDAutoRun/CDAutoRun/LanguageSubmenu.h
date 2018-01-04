#pragma once

#include "..\..\..\Components\UIFramework\porting\SubMenuBase.h"
#include "..\..\..\Components\Localization\Ilocalization.h"

namespace QSYNC_LIB 
{
	class LanguageSubmenu : public SubMenuBase
	{	
	public:
		LanguageSubmenu()
		{
			CreateILocalization(&m_pILocalization);

			int nRegionNum = m_pILocalization->GetRegionNum();
			for(int i=0;i<nRegionNum;++i)
			{
				TSTRING strRegionTitle;
				m_pILocalization->GetRegionTitle(i,strRegionTitle);
				TSTRING strRegion;
				m_pILocalization->GetRegion(i,strRegion);

				SubMenuItem Item = {strRegionTitle,strRegion};
				AddItem(Item);
			}
		}
		virtual ~LanguageSubmenu(void){}
		virtual bool IsChecked(int nIndex)
		{
			TSTRING strRegion;
			m_pILocalization->GetCurrentRegion(strRegion);
			TSTRING strDisplayID;
			TSTRING strNotifyID;
			GetMenuItem(nIndex,strDisplayID,strNotifyID);
			return strRegion == strNotifyID;
		}
	private:
		ILocalization*					m_pILocalization;
	};
}
