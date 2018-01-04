/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *							BenQ QSyncher Project
 *
 *						Copyright (c) 2005 BENQ Corp.,
 *
 ***************************************************************************
 *
 *  $Workfile:   DropFiles.h  $
 *  $Revision:   1.2  $
 *  $Creater:   Keven Yang $
 *  $Date:   Jan 08 2008 15:22:52  $ 
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 ***************************************************************************/
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/CommonUti/DropFiles.h.-arc  $
 * 
 *    Rev 1.2   Jan 08 2008 15:22:52   Alan Tu
 * update
 * 
 *    Rev 1.1   Mar 16 2007 19:17:10   Alan Tu
 * update
 * 
 *    Rev 1.0   Mar 16 2007 08:47:22   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Mar 02 2005 18:53:02   Neko Hsieh
 * Initial revision.
  */ 
 /**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */
#ifndef BS_CDropFiles_H
#define BS_CDropFiles_H
#include <shlobj.h>
#include <afxcoll.h>
class CDropFiles
{
	public:
		CDropFiles();
		~CDropFiles();

		void	AddFile(const CString &sFile);
		void	CreateBuffer();
		HGLOBAL	GetBuffer() const;
	private:
		CStringArray m_aFiles;
		HGLOBAL m_hMemData;
};
#endif