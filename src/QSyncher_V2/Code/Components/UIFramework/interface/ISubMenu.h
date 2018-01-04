///////////////////////////////////////////////////////////
//  ISubMenu.h
//  Implementation of the Interface ISubMenu
//  Created on:      12-Feb-2007 15:59:36
//  Original author: Alan tu
///////////////////////////////////////////////////////////

#if !defined(EA_32097F6D_0B61_4009_AA2A_989972578994__INCLUDED_)
#define EA_32097F6D_0B61_4009_AA2A_989972578994__INCLUDED_

#include <string>

namespace QSYNC_LIB 
{

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	class QPoint
	{
	public:
		int m_nPosX;
		int m_nPosY;
	};

	interface ISubMenu
	{
		virtual int GetMenuItemCount() =0;
		virtual bool GetMenuItem(int m_nIndex, TSTRING& strDisplayID, TSTRING& strNotifyID) =0;
		virtual bool IsEnable(int m_nIndex) {return true;}
		virtual bool IsChecked(int m_nIndex) {return false;}

	};

}
#endif // !defined(EA_32097F6D_0B61_4009_AA2A_989972578994__INCLUDED_)
