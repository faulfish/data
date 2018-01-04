#ifndef _DEFINE_PHONEBOOK_SUBMENU_
#define _DEFINE_PHONEBOOK_SUBMENU_

#pragma once

#include "..\..\Components\UIFramework\porting\SubMenuBase.h"

const LPCTSTR		EVENT_PHONEBOOK_SUBMENU_INSERT	= _T("TNotify_PB_SubMenu_Insert");
const LPCTSTR		EVENT_PHONEBOOK_SUBMENU_DELETE	= _T("TNotify_PB_SubMenu_Delete");
const LPCTSTR		EVENT_PHONEBOOK_SUBMENU_EDIT	= _T("TNotify_PB_SubMenu_Edit");

class CPhonebookSubMenu : public SubMenuBase
{	
public:
	CPhonebookSubMenu()
	{	
		SubMenuItem Item1 = {_T("IDS_PHONEBOOK_INSERT"),EVENT_PHONEBOOK_SUBMENU_INSERT};
		AddItem(Item1);
		SubMenuItem Item2 = {_T("IDS_PHONEBOOK_DELETE"),EVENT_PHONEBOOK_SUBMENU_DELETE};
		AddItem(Item2);
		SubMenuItem Item3 = {_T("IDS_PHONEBOOK_EDIT"),EVENT_PHONEBOOK_SUBMENU_EDIT};
		AddItem(Item3);
	}
	virtual ~CPhonebookSubMenu(){}
};

#endif //#define _DEFINE_PHONEBOOK_SUBMENU_