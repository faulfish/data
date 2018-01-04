#include "StdAfx.h"
#include "PIMEntity.h"

namespace QSYNC_LIB
{
CPIMEntity::CPIMEntity(TSTRING strEntityName, CPIMRecord* pRecord): m_strEntityName(strEntityName), m_pRecord(pRecord)
{}

CPIMEntity::~CPIMEntity(void)
{
}

TSTRING CPIMEntity::GetEntityName()
{
	return m_strEntityName;
}

bool CPIMEntity::Decode()
{
	bool bRes = true;
	for(list <TSTRING>::iterator iter = m_strListTmp.begin(); iter != m_strListTmp.end() && bRes; iter++)
	{
		if(! Decode((*iter)))
			bRes = false;
	}
	if(bRes)
		SetData(StrConnect<TSTRING>(_T(":"), m_strEntityName, KEYWORD_PROPERTY_NAME, false), m_strProptyName);
	return bRes;
}
bool CPIMEntity::Decode( TSTRING strPropty )
{
	TSTRING strFirst, strSecond, strCount;
	list<TSTRING> strListParas;
	vector<TSTRING> strvValues;

	if(!Split<TSTRING>(_T(":"), strPropty, strFirst, strSecond))
		return false;
	Tokenize<TSTRING>(_T(";"), strFirst, strListParas, false);
	Tokenize<TSTRING>(_T(";"), strSecond, strvValues, true);

	TSTRING strProptyName = strListParas.front();
	strListParas.pop_front();
	int nCount = GetRepeatCount<TSTRING>(_T(";"), strProptyName, m_strProptyName,true);
	// if property name is more than 1, we'll add .num to the rear, but the first one isn't included ".num"
	if(nCount > 0 && iToTStr<TSTRING>(nCount ,strCount))
	{
		strProptyName = StrConnect<TSTRING>(_T("."), strProptyName, strCount, false);
	}
	//Parse Parameters
	if(strListParas.size() > 0 )
	{	
		DecodeParas(strProptyName, strListParas);
	}
	//Parse Values
	if(strvValues.size() > 0)
	{
		if(!DecodeValues(strProptyName, strvValues))
			return false;	
	}
	else // no value
	{	
		SetValueCount(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, KEYWORD_PROPERTY_COUNT, false), 0);
	}

	m_strProptyName = StrConnect<TSTRING>(_T(";"), m_strProptyName, strProptyName, false);
	return true;
}
void CPIMEntity::DecodeParas( TSTRING strProptyName, list<TSTRING>& strListParas)
{
	TSTRING strFirst, strSecond, strNULL, strParaName;
	
	for(list<TSTRING>::iterator iter = strListParas.begin(); iter != strListParas.end(); iter++)
	{
		if(Split<TSTRING>(_T("="), (*iter), strFirst, strSecond))
		{
			SetData( StrConnect<TSTRING>(_T(":"), m_strEntityName, strProptyName, strFirst, false), strSecond);
			strParaName = StrConnect<TSTRING>(_T(";"), strParaName, strFirst, false);
		}
		else
			strNULL = StrConnect<TSTRING>(_T(";"), strNULL, strFirst, false);
	}
	if( strNULL.size() >0 )
	{
		SetData(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, KEYWORD_PROPERTY_NULL , false), strNULL);
		strParaName = StrConnect<TSTRING>(_T(";"), strParaName,KEYWORD_PROPERTY_NULL, false);
	}
	if( strParaName.size() >0 )
		SetData(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, KEYWORD_PARAMETER_NAME , false), strParaName);
}

bool CPIMEntity::DecodeValues( TSTRING strProptyName, vector<TSTRING>& strvValues)
{
	if(strProptyName == _T("LOCATION")  && strvValues.size() > 1 ) // Special Case for LOCATION
	{
		TSTRING strValue = StrConnect<TSTRING>(_T(";"), strvValues, true);
		strvValues.clear();
		strvValues.push_back(strValue);
	}

	size_t nSize = strvValues.size();
	TSTRING strCount, strValue;
	bool bDecode = true;
	if(nSize == 1)
	{
		strValue = strvValues[0];
		if(CodecSpecialValue(strProptyName, strValue, true))
		{
			SetData( StrConnect<TSTRING>(_T(":"), m_strEntityName, strProptyName, false), strValue);
		}
		else
		{
			bDecode = false;
		}
	}
	else if(nSize > 1) // Set Object:Propty:Count 
	{   
		SetValueCount(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, KEYWORD_PROPERTY_COUNT, false),(int) nSize);
		int nCount = 0;
		for(vector<TSTRING>::iterator iter = strvValues.begin(); iter != strvValues.end(); iter++)
		{
			iToTStr<TSTRING>(nCount, strCount);
			if(CodecSpecialValue(strProptyName, (*iter), true))
			{
				SetData(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, strCount, false), (*iter));
			}
			nCount++;
		}
	}
	return bDecode;
}



bool CPIMEntity::CodecSpecialValue( TSTRING strProptyName, TSTRING& strValue, bool bDecodeOn)
{
	TSTRING strParaEncoding, strParaCharset = _T("");
	bool bEncode = true;
	bool bEncoding = GetData( StrConnect<TSTRING>(_T(":"),  m_strEntityName, strProptyName, PARAMETER_NAME_ENCODING, false), strParaEncoding);
	bool bCharset = GetData( StrConnect<TSTRING>(_T(":"),  m_strEntityName, strProptyName, PARAMETER_NAME_CHARSET, false), strParaCharset);
	
	if(bEncoding && strParaEncoding == PARAMETER_VALUE_QP)
	{	
		if(bCharset && strParaCharset == PARAMETER_VALUE_UTF8)
			bEncode = bDecodeOn ? QPDecoder(strValue, strValue, CP_UTF8) : QPEncoder(strValue, strValue, CP_UTF8);
		else
			bEncode = bDecodeOn ?  QPDecoder(strValue, strValue, CP_ACP) : QPEncoder(strValue, strValue, CP_ACP); 
	}
	else if(bEncoding && strParaEncoding == PARAMETER_VALUE_BASE64)
	{	
		if(bCharset && strParaCharset == PARAMETER_VALUE_UTF8)
			bEncode = bDecodeOn ? Base64Decoder(strValue, strValue, CP_UTF8) : Base64Encoder(strValue, strValue, CP_UTF8);
		else
			bEncode = bDecodeOn ? Base64Decoder(strValue, strValue, CP_ACP) : Base64Encoder(strValue, strValue, CP_ACP); 
	}
	
	else if(!bEncoding && bCharset && strParaCharset == PARAMETER_VALUE_UTF8)
	{   // It has already been decoded during Sync period 
		/*
		if(bDecodeOn)
		{
			const CStringA strTmp = WideCharToChar(strValue.c_str(), CP_UTF8);
			strValue = CharToWideChar(strTmp, CP_ACP);
		}
		else
		{
			const CStringA strTmp = WideCharToChar(strValue.c_str(), CP_ACP);
			strValue = CharToWideChar(strTmp, CP_UTF8);
		}*/
	}
	
	return bEncode;
}

bool CPIMEntity::Encode()
{
	bool bEncode = true;
	TSTRING strProptyName, strPropty, strFirst, strSecond;
	if(! GetData(StrConnect<TSTRING>(_T(":"), m_strEntityName, KEYWORD_PROPERTY_NAME, false), strProptyName))
		return false;
	
	vector<TSTRING> strvProptyName;
	Tokenize<TSTRING>(_T(";"), strProptyName, strvProptyName, false);
	for(vector<TSTRING>::iterator iter = strvProptyName.begin(); iter != strvProptyName.end() && bEncode; iter++)
	{
		TSTRING strPara, strValue;
		//TRACE(_T("property name : %s\n"),iter->c_str());
		bEncode = EncodeParas( (*iter), strPara);
		assert(bEncode);
		if(bEncode)
			bEncode = EncodeValues( (*iter), strValue);
		assert(bEncode);
		if(!bEncode)
			break;
		Split<TSTRING>(_T("."), (*iter), strFirst, strSecond);
		strPropty = StrConnect<TSTRING>(_T(";"), strFirst, strPara, false);
		strPropty = StrConnect<TSTRING>(_T(":"), strPropty, strValue, true);
		m_strListTmp.push_back(strPropty);
	}
	return bEncode;
}
bool CPIMEntity::EncodeParas( TSTRING strProptyName, TSTRING& strDes )
{
	TSTRING strParaName, strParaValue, strPara, strParas, strFirst, strSecond;
	bool bEncode = true;
	if(! GetData(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, KEYWORD_PARAMETER_NAME, false), strParaName))
		return bEncode;

	vector<TSTRING> strvParaName;
	Tokenize<TSTRING>(_T(";"), strParaName, strvParaName, false);
	for(vector<TSTRING>::iterator iter = strvParaName.begin(); iter != strvParaName.end() && bEncode; iter++)
	{
		if((*iter) != KEYWORD_PROPERTY_NULL)
		{
			bEncode = GetData( StrConnect<TSTRING>(_T(":"),  m_strEntityName, strProptyName, (*iter), false), strParaValue);
			Split<TSTRING>(_T("."), (*iter), strFirst, strSecond);
			strPara = StrConnect<TSTRING>(_T("="), strFirst, strParaValue, false);
			strParas = StrConnect<TSTRING>(_T(";"), strParas, strPara, false);
		}
		else
		{
			bEncode = GetData( StrConnect<TSTRING>(_T(":"),  m_strEntityName, strProptyName, (*iter), false), strParaValue);
			strParas = StrConnect<TSTRING>(_T(";"), strParas, strParaValue, false);
		}
	}
	strDes = strParas;
	return bEncode;
}
bool CPIMEntity::EncodeValues( TSTRING strProptyName, TSTRING& strDes )
{
	bool bEncode = true;
	TSTRING strCount;
	long nValueCount=0;
	if(GetValueCount(StrConnect<TSTRING>(_T(":"), m_strEntityName, strProptyName, KEYWORD_PROPERTY_COUNT, false), nValueCount)) 
	{ // nValueCount > 1
		if(nValueCount >0)
		{
			vector<TSTRING> strvValue;
			for(int i = 0; i < nValueCount; i++)
			{
				TSTRING strValue;
				iToTStr<TSTRING>(i, strCount);
				if(GetData(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, strCount, false), strValue))
					bEncode = CodecSpecialValue(strProptyName, strValue, false);
				strvValue.push_back(strValue);
			}
			strDes = StrConnect<TSTRING>(_T(";"), strvValue, true);
		}
	}
	else
	{
		if(strProptyName == KEYWORD_UID)
		{
			long nUID=0;
			bEncode = m_pRecord->GetIntegerData(KEYWORD_LUID, nUID);
			if(bEncode)
				bEncode = iToTStr<TSTRING>((int)nUID,strDes);
		}
		else
		{
			bEncode = GetData(StrConnect<TSTRING>(_T(":"),m_strEntityName, strProptyName, false), strDes);
			if(bEncode)
				bEncode = CodecSpecialValue(strProptyName, strDes, false);
		}
	}
	return bEncode;
}
	
void CPIMEntity::SetData( const TSTRING& strPropty, TSTRING strValue )
{
	CStringA strProptyA = UnicodetoASCII(strPropty.c_str());
	m_pRecord->SetStringData( (LPCSTR)strProptyA, strValue.c_str());
}
void CPIMEntity::SetValueCount( const TSTRING& strPropty, int nCount )
{
	CStringA strProptyA = UnicodetoASCII(strPropty.c_str());
	m_pRecord->SetIntegerData((LPCSTR)strProptyA, (long)nCount);
}
bool CPIMEntity::GetValueCount( const TSTRING& strPropty , long& nCount)
{
	CStringA strProptyA = UnicodetoASCII(strPropty.c_str());
	return m_pRecord->GetIntegerData((LPCSTR)strProptyA, nCount);
}
bool CPIMEntity::GetData( const TSTRING& strName, TSTRING& strValue )
{
	CStringA strProptyA = UnicodetoASCII(strName.c_str());
	return m_pRecord->GetStringData((LPCSTR)strProptyA, strValue);
}


}//namespace QSYNC_LIB