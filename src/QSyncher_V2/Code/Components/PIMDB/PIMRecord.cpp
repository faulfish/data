#include "StdAfx.h"
#include "PIMRecord.h"

namespace QSYNC_LIB
{

CPIMRecord::CPIMRecord(const GUID& guid):PIMObjectBase("PIM",guid){}
CPIMRecord::~CPIMRecord(void){}

bool CPIMRecord::HasLineBreak(TSTRING& strLastLine, const TSTRING& strNewLine)
{
	bool bRes = false;

	// Detect QP Line Break
	TCHAR cEnd = _T('');
	TSTRING strTmpForGetCount = strLastLine;
	size_t nSize = strLastLine.size();
	if( nSize > 0 )
	{
		cEnd = strLastLine.at(nSize-1);
	}
	if(cEnd == _T('=') && GetRepeatCount<TSTRING>(PARAMETER_VALUE_QP, strTmpForGetCount) >0)
	{
		strLastLine.erase(nSize-1, 1);
		strLastLine += strNewLine; 
		bRes = true;
	}
	
	// Detect Normal Line Break
	if(!bRes) 
	{
		TSTRING strNewOrgLine = strNewLine;
		if(strNewLine.size() > 0)
		{
			bool bHasLineBreak = false;
			while(strNewOrgLine[0] == _T(' ') || strNewOrgLine[0] == _T('\t'))
			{
				strNewOrgLine = strNewOrgLine.substr(1, strNewOrgLine.size()-1);
				bHasLineBreak = true;
			}
			if(bHasLineBreak)
			{
				strLastLine += strNewOrgLine;
				bRes = true;
			}
		}
	}
	return bRes;
}


bool CPIMRecord::decode( const TSTRING& strRawData )
{
	if(strRawData.empty())
		return false; 
	vector<TSTRING> vStr; 
	vector <CPIMEntity> vEntity;
	TSTRING strObjectName;
	int nIndex =0;
	bool bRes = true;

	Tokenize<TSTRING>(_T("\r\n"), strRawData, vStr, false);
	for(vector<TSTRING>::iterator itervStr = vStr.begin(); itervStr != vStr.end() && bRes; itervStr++)
	{
		TSTRING strFirst, strSecond;
		bool bSplit = Split<TSTRING>( _T(":"),(*itervStr),strFirst, strSecond);
		if(strFirst == _T("BEGIN"))
		{
			int nCount = GetRepeatCount<TSTRING>(_T(";"), strSecond, strObjectName);
			TSTRING strCount;
			if(nCount > 0 && iToTStr<TSTRING>(nCount, strCount))// if object name is more than 1, we'll add .num to the rear			
				StrConnect<TSTRING>(_T("."), strSecond, strCount, false);
			strObjectName = StrConnect<TSTRING>(_T(";"), strObjectName, strSecond, false);
			
			CPIMEntity entity(strSecond, this);
			vEntity.push_back(entity);
			nIndex ++;
		}
		else if(strFirst == _T("END") && GetRepeatCount(strSecond, vEntity[nIndex-1].GetEntityName()) == 1)
			nIndex --;
		else if(nIndex > 0 && strFirst != _T("BEGIN") && strFirst != _T("END"))
		{
			bool bLineBreak =false;			
			if(vEntity[nIndex-1].m_strListTmp.size() >0)
				bLineBreak = HasLineBreak(vEntity[nIndex-1].m_strListTmp.back(), (*itervStr));
			if(!bLineBreak)
				vEntity[nIndex-1].m_strListTmp.push_back((*itervStr));
		}
		else 
		{
			bRes = false;  // wrong format
		}
	}
	if(nIndex != 0){
		assert(nIndex == 0);
		bRes = false;
	}
	for(vector<CPIMEntity>::iterator iterEntity = vEntity.begin(); iterEntity != vEntity.end() && bRes; iterEntity++)
	{
		if(!(*iterEntity).Decode())
			bRes = false;
	}
	if(bRes)
	{
		SetStringData( KEYWORD_OBJECT_NAME, strObjectName.c_str());
		SetLUID(strObjectName);
	}
	return bRes;
}

bool CPIMRecord::encode( TSTRING& strRawData )
{
	bool bEncode = true;
	TSTRING strObjectName, strName;
	if(! GetStringData(KEYWORD_OBJECT_NAME, strObjectName))
		return false;

	list<TSTRING> strRawList;
	vector <CPIMEntity> vEntity;
	size_t nVCardNum = 0;

	vector<TSTRING> strvObject;
	Tokenize<TSTRING>(_T(";"), strObjectName, strvObject, false);
	for(vector<TSTRING>::iterator iter = strvObject.begin(); iter != strvObject.end(); iter++)
	{
		CPIMEntity entity((*iter), this);
		bEncode = entity.Encode();
		if(iter == strvObject.begin())
		{
			strName = (*iter);
			if((*iter) != OBJECT_NAME_VCAL && (*iter) != OBJECT_NAME_VCARD)
				return false;
			else if((*iter) == OBJECT_NAME_VCARD)
				nVCardNum = strvObject.size();
		}
		else
		{
			TSTRING strFirst, strSecond;
			Split<TSTRING>(_T("."), (*iter), strFirst, strSecond);
			entity.m_strListTmp.push_front(_T("BEGIN:")+strFirst);
			entity.m_strListTmp.push_back(_T("END:")+strFirst);	
		}
		vEntity.push_back(entity);
	}

	if(strName == OBJECT_NAME_VCAL || (strName == OBJECT_NAME_VCARD && nVCardNum == 1))
		for(vector<CPIMEntity>::iterator iterEntity = vEntity.begin(); iterEntity != vEntity.end(); iterEntity++)
			InsertEntity((*iterEntity).m_strListTmp, strRawList);
	else if(strName == OBJECT_NAME_VCARD && nVCardNum >1 ) // Case: VCard > 1
		MakeVCardsRawList(vEntity,strRawList);
	else
		bEncode = false;

	if(bEncode)
	{
		strRawList.push_front(_T("BEGIN:")+strName);
		strRawList.push_back(_T("END:")+strName);
		strRawData = StrConnect<TSTRING>(_T("\r\n"), strRawList, false);
	}
	return bEncode;
}

void CPIMRecord::MakeVCardsRawList(vector<CPIMEntity>& vEntity, list<TSTRING>& strRawList )  // for VCard > 1
{
	
	for(list<TSTRING>::iterator iter = vEntity[0].m_strListTmp.begin(); iter != vEntity[0].m_strListTmp.end(); iter++)
	{	
		size_t nVCardNum = vEntity.size();
		strRawList.push_back((*iter));
		if(nVCardNum > 1)
		{	
			if((*iter) == _T("AGENT:"))
			{
				InsertEntity( vEntity[1].m_strListTmp, strRawList );
				vEntity.erase(vEntity.begin()+1);
			}
			// else if((*iter) ==)  //for special case
		}
	}
}
void CPIMRecord::InsertEntity(list<TSTRING>& strListEntity, list<TSTRING>& strRawList )
{
	for(list<TSTRING>::iterator iter = strListEntity.begin(); iter != strListEntity.end(); iter++)
		strRawList.push_back((*iter));
}

void CPIMRecord::SetLUID(TSTRING strObjectName)
{	
	static int nLUID = 0;
	TSTRING strFirst, strSecond, strLUID = _T("");
	Split<TSTRING>(_T(";"), strObjectName, strFirst, strSecond);
	if(strFirst == OBJECT_NAME_VCAL)
		GetStringData("VEVENT:UID", strLUID);
	else if(strFirst == OBJECT_NAME_VCARD)
		GetStringData("VCARD:UID", strLUID);
	if(0 == _ttoi(strLUID.c_str()))
	{
		SetIntegerData( KEYWORD_LUID, ++nLUID );	
	}
	else
	{
		SetIntegerData( KEYWORD_LUID, _ttoi(strLUID.c_str()) );
	}
}

void CPIMRecord::SetStringData( const char * szName, const TCHAR* szData )
{
	using namespace QSYNC_LIB; 
	CommonDataPt sptemData = new CommonData;
	sptemData->SetName(szName);

#ifdef _UNICODE
	sptemData->SetWString(szData); 
#else
	sptemData->SetString(szData); 
#endif
	SetByName(szName, sptemData);
}

void CPIMRecord::SetIntegerData( const char * szName, long nInt)
{
	using namespace QSYNC_LIB; 
	CommonDataPt sptemData = new CommonData;
	sptemData->SetName(szName);
	sptemData->SetInteger(nInt);

	SetByName(szName,sptemData);
}
bool CPIMRecord::GetStringData( const char * szName, TSTRING& strDes )
{
	using namespace QSYNC_LIB; 
	CommonDataPt sptemData = new CommonData;
	size_t npos = -1;
	size_t nGet = GetByName(szName, sptemData);
	bool bRes = false;
	if(nGet != npos)
	{
#ifdef _UNICODE
		const TCHAR* tmp = sptemData->GetWString(); 
		 strDes = tmp;
#else
		strDes = sptemData->GetString(); 
#endif
		bRes = true;
	}
	return bRes;
}
bool CPIMRecord::GetIntegerData( const char * szName, long& nInt )
{
	using namespace QSYNC_LIB; 

	CommonDataPt sptemData = new CommonData;
	size_t npos = -1;
	bool bRes = false;
	size_t nGet = GetByName(szName,sptemData);
	if(nGet != npos)
	{	
		nInt = sptemData->GetInteger();
		bRes = true;
	}
	return bRes;
}


}//namespace QSYNC_LIB