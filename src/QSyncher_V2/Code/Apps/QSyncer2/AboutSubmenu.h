#pragma once

#include "..\..\Components\UIFramework\porting\SubMenuBase.h"

namespace QSYNC_LIB 
{
	class AboutBoxSubmenu : public SubMenuBase
	{	
	public:
		AboutBoxSubmenu()
		{	
			SubMenuItem Item1 = {_T("IDS_HELP"),_T("Help")};
			AddItem(Item1);
			SubMenuItem Item2 = {_T("IDS_ABOUT"),_T("About")};
			AddItem(Item2);
		}
		virtual ~AboutBoxSubmenu(void){}
	};
}
