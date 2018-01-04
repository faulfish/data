/*$Log:

*/

///////////////////////////////////////////////////////////
//  IQTreeDataSource.h
//  Implementation of the Class ITreeDataSource
//  Created on:      09-Feb-2007 09:46:52
///////////////////////////////////////////////////////////

#if !defined(EA_8444963F_7090_48e4_97BD_3937ABA0ECA0__INCLUDED_)
#define EA_8444963F_7090_48e4_97BD_3937ABA0ECA0__INCLUDED_


#include "..\..\..\CommonUtil\NotifyBase.h"
#include "IQTreeUI.h"
#include "../Interface/QControlNotify.h"


using namespace QSYNC_LIB;

interface IQTreeDataSource
{
	virtual bool      CreateNode(CQTreeNode* pNode) = 0;
	virtual bool		DeleteNode(CQTreeNode* pNode) = 0;
	virtual CQTreeNode* GetRoot() = 0;
	virtual bool		ExpandNode(CQTreeNode* pNode) = 0;
	virtual bool		UpdateNode(CQTreeNode* pNode) = 0;
	virtual bool		Register(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
	virtual bool		Register(INotify* pINotify, const TSTRING& strEvent) = 0;
	virtual bool		Unregister(INotify* pINotify, const TSTRING& strEvent) = 0;
	virtual bool		Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
	virtual void		RefreshClear() = 0;

};
#endif // !defined(EA_8444963F_7090_48e4_97BD_3937ABA0ECA0__INCLUDED_)
