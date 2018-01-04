#if !defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CMEFileNode__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CMEFileNode__INCLUDED_
#pragma once

#include "CCommonFileNode.h"

namespace QSYNC_LIB
{
	class CMEFileNode :public CCommonFileNode
	{
	public:
		CMEFileNode(CMEFileNode* pParent):CCommonFileNode(pParent)
		{
			SetSeparator(ME_FILE_PATH_SEPARATOR_STR);
		}

		virtual ~CMEFileNode(void)
		{
		}
	};
}//namespace end
#endif
