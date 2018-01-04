/*********************************************************************/
/** @file:	CommonUIParameter.h
  
  Defines the CommonUI Parameter.
  
  $Original Author:    YC Liang  $
  $Version:	1.0 $
  $Date:   Apr 01 2009 14:17:22  $
  */
/*********************************************************************/

/*$Log:



*/




#if !defined(EA_DAD34693_43B2_44e0_A65A_CommonUIParameter__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_CommonUIParameter__INCLUDED_

/*
#include "..\..\..\CommonUtil\NotifyBase.h"
#include ".\QControlNotify.h"
#include <vector>
#include "..\CommUIExportAPI.h"
*/



namespace QSYNC_LIB{

template <class Type>
class FireUIParam {
public:
	CPoint	m_Point;
    int		m_nRow;
    UINT	m_nFlags;
    Type*	m_pData;
	FireUIParam(void):m_Point(NULL),m_nRow(-1),m_nFlags(0),m_pData(NULL){}
};



//} fUIp;






}

#endif