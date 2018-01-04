#ifndef _CPIMRECORD_HPP_
#define _CPIMRECORD_HPP_

#pragma once
#include "..\..\CommonUtil\commondatafile.h"
#include "..\..\Components\PIMDB\PIMObjectBase.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\CommonUtil\PIMStringUtility.h"
#include "..\..\Components\PIMDB\PIMEntity.h"
#include <vector>
#include <list>
#include <string>
#include <cstringt.h>

using namespace std;

#ifdef		PIMDB_DLL
	#define		PIMDB_DLL_DECLSPEC		__declspec(dllexport)
#else
	#define		PIMDB_DLL_DECLSPEC		__declspec(dllimport)
#endif


namespace QSYNC_LIB
{

#define					KEYWORD_OBJECT_NAME						"Object"
#define					KEYWORD_LUID							"LUID"
#define					KEYWORD_UID								_T("UID")
#define                 OBJECT_NAME_VCAL						_T("VCALENDAR")
#define					OBJECT_NAME_VCARD						_T("VCARD")

#define					KEYWORD_PROPERTY_NAME					_T("PropertyName")
#define					KEYWORD_PARAMETER_NAME					_T("ParaName")
#define					KEYWORD_PROPERTY_COUNT					_T("Count")
#define					KEYWORD_PROPERTY_NULL					_T("NULL")

#define					PARAMETER_NAME_ENCODING					_T("ENCODING")
#define					PARAMETER_VALUE_QP						_T("QUOTED-PRINTABLE")
#define					PARAMETER_VALUE_BASE64					_T("BASE64")
#define					PARAMETER_NAME_CHARSET					_T("CHARSET")
#define					PARAMETER_VALUE_UCS2					_T("UCS2")
#define					PARAMETER_VALUE_UTF8					_T("UTF-8")
#define					PARAMETER_VALUE_ASCII					_T("ASCII")




class CPIMEntity;

class PIMDB_DLL_DECLSPEC CPIMRecord : public PIMObjectBase
{
public:
	CPIMRecord(const GUID& guid);
	virtual ~CPIMRecord(void);
	
	virtual bool encode(void** pVoid,long& nSize){return false;}
	virtual bool decode(void* pVoid,const long& nSize){return false;}
	bool decode( const TSTRING& strRawData );
	bool encode( TSTRING& strRawData );
	
	// for encode
	void MakeVCardsRawList(vector<CPIMEntity>& vEntity, list<TSTRING>& strRawList ); 
	void InsertEntity( list<TSTRING>& strListEntity, list<TSTRING>& strRawList );

	void SetStringData( const char * szName, const TCHAR* szData);
	void SetIntegerData( const char * szName, long nInt );
	bool GetStringData( const char * szName, TSTRING& szData );
	bool GetIntegerData( const char * szName, long& nInt );
private:
	void SetLUID(TSTRING strObjectName);
	bool HasLineBreak(TSTRING& strLastLine, const TSTRING& strNewLine);
};

} // namespace QSYNC_LIB

#endif //_CPIMRECORD_HPP_