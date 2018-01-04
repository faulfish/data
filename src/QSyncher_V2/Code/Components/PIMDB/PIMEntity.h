#ifndef _CPIMENTITY_HPP_
#define _CPIMENTITY_HPP_

#pragma once
#include "..\..\CommonUtil\commondatafile.h"
#include "..\..\Components\PIMDB\PIMObjectBase.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\Components\PIMDB\PIMRecord.h"
#include <vector>
#include <list>
#include <string>

using namespace std;

namespace QSYNC_LIB
{
class CPIMRecord;

 /* 
  * Begin:Entity
  *	PropertyName; ParaName = ParaValue; ... : Value1; Value2;....
  *	...
  *	End:Entity
  */
class CPIMEntity
{
public:
	list <TSTRING> m_strListTmp;

public:
	CPIMEntity(TSTRING EntityName, CPIMRecord* pRecord);
	~CPIMEntity(void);

	TSTRING GetEntityName();
	bool Decode();
	bool Encode();

private:
	TSTRING m_strEntityName;
	CPIMRecord* m_pRecord;
	TSTRING m_strProptyName; // KEYWORD_PROPERTY_NAME

private:
	bool Decode( TSTRING strPropty );
	void DecodeParas( TSTRING strProptyName, list <TSTRING>& strListParas);
	bool DecodeValues( TSTRING strProptyName, vector <TSTRING>& strvValues);
	bool CodecSpecialValue( TSTRING strProptyName, TSTRING& strValue, bool bDecodeOn);

	bool EncodeParas( TSTRING strProptyName, TSTRING& strDes );
	bool EncodeValues( TSTRING strProptyName, TSTRING& strDes );
	
	void SetData( const TSTRING& strPropty, TSTRING strValue );
	void SetValueCount( const TSTRING& strPropty, int nCount );
	bool GetData( const TSTRING& strName, TSTRING& strValue );
	bool GetValueCount( const TSTRING& strPropty, long& nCount );
};

} // namespace QSYNC_LIB

#endif //_CPIMENTITY_HPP_