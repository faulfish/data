
#include "stdafx.h"
#include "wbxmlSyncml.h"
#include <comutil.h>
#include "..\..\CommonUtil\unicodeutility.h"

#if !defined(ASSERT)
#if !defined(ATLASSERT)
#define	ASSERT(p) (p)
#else
#define	ASSERT ATLASSERT
#endif
#endif


//
//////////////////////////////////////////////////////////////
//
//	Function :		getMBInt
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		int	-
//	Parameters :
//		wbxml
//			[IN ]	
//		wbxmlLength
//			[IN ]	
//		index
//			[OUT]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static int getMBInt(const BYTE* wbxml,int wbxmlLength,int& index)
{
	int			mbInt = 0;
	const BYTE*	next = &wbxml[index];

	while( index < wbxmlLength )
	{
		mbInt <<= 7;
		mbInt += *next & 0x7F;
		index++;
		if( (*next & 0x80) == 0 )
			break;
		next++;
	}
	return mbInt;
}		//	end of getMBInt
//
//////////////////////////////////////////////////////////////
//
//	Function :		makeMBInt
//	Description :	
//		write the MB encoding of the supplied int value to the
//		wbxml starting at the current length value. Update the
//		length value.
//	Author :		Bill Holohan
//	Return :
//		void	-
//	Parameters :
//		value
//			[IN ]	value to be encoded
//		wbxml
//			[IN ]	pointer to total wbxml buffer
//		wbxmlLength
//			[IN ]	offset to start writing within wbxml
//			[OUT]	offset to byte after the MB int value
//	Note :
//
//////////////////////////////////////////////////////////////
//
void makeMBInt(int value, BYTE* wbxml,int& wbxmlLength,bool first)
{
	int			shifted = value >> 7;
	if( shifted != 0 )
		makeMBInt(shifted,wbxml,wbxmlLength,false);
	wbxml[wbxmlLength++] = (value & 0x7F) | (first ? 0 : 0x80);
}		//	end of makeMBInt
//
//////////////////////////////////////////////////////////////
//
//	Function :		addXmlLine
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		void	-
//	Parameters :
//		firstLine
//			[IN/OUT]	
//		lastLine
//			[IN/OUT]	
//		level
//			[IN ]	
//		codePage
//			[IN ]	
//		endTag
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static void addXmlLine(TXmlLine*& firstLine,TXmlLine*& lastLine,int level,int codePage,bool endTag)
{
	if( lastLine == 0 )
	{
		firstLine =	lastLine = new TXmlLine(level,codePage,endTag);
	}
	else
	{
		lastLine->m_next = new TXmlLine(level,codePage,endTag);
		lastLine = lastLine->m_next;
	}
}	//	end of addXmlLine

void TransferUTF8toAnsi(char *szTemp)
{
	if(::strlen(szTemp) > 0)
	{
		char *pIndex = szTemp;
		while(*pIndex != 0)
		{
			int nUTF8Bytes = 0;
			unsigned cFlag = 0x80;
			unsigned cTest = pIndex[0];
			while(cTest & cFlag)
			{
				nUTF8Bytes++;
				cFlag >>= 1;
			};

			//1. Get UTF8 bytes
			if(nUTF8Bytes > 0)
			{
				char* szUTF8Buffer = new char[nUTF8Bytes+1];
				if(szUTF8Buffer)
				{
					::memset(szUTF8Buffer,0,nUTF8Bytes+1);
					::memcpy(szUTF8Buffer,pIndex,nUTF8Bytes);

					//2. Transfer UTF8 to UCS2
					int nWideLen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szUTF8Buffer, -1, NULL, 0);
					wchar_t* wszBuffer = new wchar_t[nWideLen+1];
					if(wszBuffer)
					{
						::ZeroMemory(wszBuffer,(nWideLen+1)*sizeof(wchar_t));
						::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szUTF8Buffer, -1, wszBuffer, nWideLen);

						//3. Transfer UCS2 to ANSI
						int nAnsiLen = ::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, NULL, 0, NULL, NULL);
						char* szBuffer = new char[nAnsiLen+1];					
						if(szBuffer)
						{
							::memset(szBuffer,0,nAnsiLen+1);
							::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, szBuffer, nAnsiLen, NULL, NULL);

							//4. insert to original string
							int nAnsiBytes = static_cast<int>(::strlen(szBuffer)*sizeof(char));
							if(nAnsiBytes <= nUTF8Bytes)
							{
								char* pLastIndex = pIndex + nUTF8Bytes;
								memcpy(pIndex,szBuffer,nAnsiBytes);
								pIndex += nAnsiBytes;							
								memcpy(pIndex,pLastIndex,strlen(pLastIndex));
								pIndex--;
							}
							else ASSERT(0);
							delete [] szBuffer;
						}

						delete [] wszBuffer;
					}

					delete [] szUTF8Buffer;
				}
			}
			
			pIndex++;
		};
	}
}

//
//////////////////////////////////////////////////////////////
//
//	Function :		addXmlData
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		void	-
//	Parameters :
//		data
//			[IN ]	
//		length
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static void addXmlData(TXmlLine* lastLine,const TCHAR* data)
{
	if(lastLine)
		lastLine->m_strData += data;
}

static void addXmlData(TXmlLine* lastLine,const BYTE* data,int length = 0)
{
	using namespace QSYNC_LIB;
	char* szTemp = new char[length+1];
	memset(szTemp,0,length+1);
	memcpy(szTemp,data,length);
#ifdef _UNICODE
	TransferUTF8toAnsi(szTemp);
	wchar_t* wszTemp = new wchar_t[length+1];
	memset(wszTemp,0,(length+1)*sizeof(wchar_t));
	AnsiToUnicodeBuffer(szTemp,wszTemp,(length+1)*sizeof(wchar_t));
	addXmlData(lastLine,wszTemp);
	delete [] wszTemp;
#else
	addXmlData(lastLine,szTemp);
#endif
	delete [] szTemp;
}		//	end of addXmlData

//
//////////////////////////////////////////////////////////////
//
//	Function :		wbxmlToXml
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		TCHAR*	-
//	Parameters :
//		wbxml
//			[IN ]	
//		wbxmlLength
//			[IN ]	
//		xmlLength
//			[OUT]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
const CString wbxmlToXml(const BYTE* wbxml,DWORD wbxmlLength,DWORD& xmlLength)
{
	if( wbxmlLength == 0 )
		return 0;
	if( wbxml[wbxmlLength-1] == 0x0a 		//	error in example file
		||
		wbxml[wbxmlLength-1] == 0x00		//	error in SE/K700 wbxml (from Server)?
		)
	{
		wbxmlLength--;
	}
	if( wbxml[0] == 0x00 )		//	error in SE/K700 wbxml (from Client)?
	{
		wbxmlLength--;
		wbxml++;
	}
	int			versionMajor = (wbxml[0] >> 4) + 1;
	int			versionMinor = wbxml[0] & 0x0F;
	int			publicId = -1;
	int			publicIdIndex = -1;
	const BYTE*	publicIdString = 0;
	const BYTE*	stringTable;
	int			stringTableLength = 0;
	int			characterSet;
	int			index;
	int			level = 0;
	int			maxLevel = 0;
	TMapStack*	stack = 0;
	TXmlLine*	firstLine = 0;
	TXmlLine*	lastLine = 0;

	firstLine = 0;
	lastLine = 0;
	//
	//	publicId
	//
	if( wbxml[1] == 0 )
	{	//	string
		index = 2;
		publicIdIndex = getMBInt(wbxml,wbxmlLength,index);
	}
	else
	{	//	mb_u_int32
		index = 1;
		publicId = getMBInt(wbxml,wbxmlLength,index);
	}
	//
	//	charset
	//
	characterSet = getMBInt(wbxml,wbxmlLength,index);
	//
	//	string table
	//
	stringTableLength = getMBInt(wbxml,wbxmlLength,index);
	stringTable = &wbxml[index];
	index += stringTableLength;
	//
	//	set public id string, if present
	//
	if( publicId == -1 )
	{
		publicIdString = &stringTable[publicIdIndex];
	}
	else
	{
		switch( publicId )
		{
		case 4051:
			publicIdString = (const BYTE*)syncml11;
			publicId = -1;
			break;
		case 4052:
			publicIdString = (const BYTE*)devinf11;
			publicId = -1;
			break;
		}
	}
	//
	//	body
	//
	int			tagCodePageNo = 0;
	int			defaultCodePage = 0;

	if( publicId == -1 )
	{
		if( _tcsnicmp((const TCHAR*)publicIdString,devinf10,_tcslen(devinf10)) == 0
			||
			_tcsnicmp((const TCHAR*)publicIdString,devinf11,_tcslen(devinf11)) == 0
			)
		{
			defaultCodePage =
				tagCodePageNo = 2;
		}
		else if( _tcsnicmp((const TCHAR*)publicIdString,metinf10,_tcslen(devinf10)) == 0
			||
			_tcsnicmp((const TCHAR*)publicIdString,metinf11,_tcslen(devinf11)) == 0
			)
		{
			defaultCodePage =
				tagCodePageNo = 1;
		}
	}
	const TMap*	tagCodePage = codePages[tagCodePageNo];
	bool		inAttributes = false;
	bool		inAttributesWithContent = false;
	bool		attributeName = true;
	bool		bSwitchCodePage = false;

	while( index < (int)wbxmlLength )
	{
		unsigned char	tag = wbxml[index++];
		int				length;

		switch( tag )
		{
		case SWITCH_PAGE:			//	SWITCH_PAGE
			tagCodePageNo = wbxml[index];
			ASSERT(tagCodePageNo >=0 && tagCodePageNo < 4);
			tagCodePage = codePages[tagCodePageNo];
			index++;
			bSwitchCodePage = true;
			break;
		case END:		//	END -  Indicates the end of an attribute list or the end of an element.
			{
				if( inAttributes )
				{
					inAttributes = false;
					if( inAttributesWithContent )
						addXmlData(lastLine,_T(">"));
					else
						addXmlData(lastLine,_T("/>"));
					attributeName = true;
				}
				else
				{
					level--;
					addXmlData(lastLine,_T("</"));
					addXmlData(lastLine,stack->m_entry->m_xml);
					addXmlData(lastLine,_T(">"));
					addXmlLine(firstLine,lastLine,level-1,tagCodePageNo,true);
					//
					//	pop from stack
					//
					TMapStack*		temp = stack->m_next;

					delete stack;
					stack = temp;
				}
			}
			break;
		case ENTITY:	//	ENTITY -  A character entity. Followed by a mb_u_int32 encoding the character entity number.
			break;
		case STR_I:		//	STR_I -  Inline string. Followed by a termstr.
			{
				using namespace QSYNC_LIB;
				const char*		string = (const char*)&wbxml[index];
				size_t			length = strlen(string);

				wchar_t* szTemp = new wchar_t[length+1];
				AnsiToUnicodeBuffer(string,szTemp,(static_cast<int>(length)+1)*sizeof(wchar_t));

				if( _tcsncmp(MIME_TYPE_WBXML,szTemp,length) == 0 )
					addXmlData(lastLine,MIME_TYPE_XML);
				else if( _tcsncmp(MIME_TYPE_WBXML_DEVINF,szTemp,length) == 0 )
					addXmlData(lastLine,MIME_TYPE_XML_DEVINF);
				else
					addXmlData(lastLine,szTemp);
				delete [] szTemp;
				index += static_cast<int>(length) + 1;
			}
			break;
		case LITERAL_C:		//	LITERAL_C -  An unknown tag posessing content but no attributes.
		case LITERAL_A:		//	LITERAL_A -  An unknown tag posessing attributes but no content.
		case LITERAL_AC:	//	LITERAL_AC -  An unknown tag posessing both attributes and content.
			ASSERT(!inAttributes);
		case LITERAL:		//	LITERAL -  An unknown attribute name, or unknown tag posessing no attributes or content.Followed by a mb_u_int32 that encodes an offset into the string table.
			{
				int				stringPos = getMBInt(wbxml,wbxmlLength,index);
				const TCHAR*	tagName = (const TCHAR*)&stringTable[stringPos];

				if( inAttributes )
				{
					if( attributeName )
					{
						addXmlData(lastLine,_T(" "));
						addXmlData(lastLine,tagName);
						addXmlData(lastLine,_T("='"));
					}
					else
					{
						addXmlData(lastLine,tagName);
						addXmlData(lastLine,_T("'"));
					}
					attributeName = !attributeName;
				}
				else
					addXmlData(lastLine,tagName);
			}
			break;
		case EXT_I_0:	//	EXT_I_0 -  Inline string document-type-specific extension token. Token is followed by a termstr.
			ASSERT(FALSE);
			break;
		case EXT_I_1:	//	EXT_I_1 -  Inline string document-type-specific extension token. Token is followed by a termstr.
			ASSERT(FALSE);
			break;
		case EXT_I_2:	//	EXT_I_2 -  Inline string document-type-specific extension token. Token is followed by a termstr.
			ASSERT(FALSE);
			break;
		case PI:		//	PI -  Processing instruction.
			ASSERT(FALSE);
			break;
		case EXT_T_0:	//	EXT_T_0 -  Inline integer document-type-specific extension token. Token is followed by a mb_u_int32.
			ASSERT(FALSE);
			break;
		case EXT_T_1:	//	EXT_T_1 -  Inline integer document-type-specific extension token. Token is followed by a mb_u_int32.
			ASSERT(FALSE);
			break;
		case EXT_T_2:	//	EXT_T_2 -  Inline integer document-type-specific extension token. Token is followed by a mb_u_int32.
			ASSERT(FALSE);
			break;
		case STR_T:		//	STR_T -  String table reference. Followed by a mb_u_int32 encoding a byte offset from the beginning of the string table.
			ASSERT(FALSE);
			break;
		case EXT_0:		//	EXT_0 -  Single -byte document-type-specific extension token.
			ASSERT(FALSE);
			break;
		case EXT_1:		//	EXT_1 -  Single -byte document-type-specific extension token.
			ASSERT(FALSE);
			break;
		case EXT_2:		//	EXT_2 -  Single -byte document-type-specific extension token.
			ASSERT(FALSE);
			break;
		case OPAQUE:		//	OPAQUE -  Opaque document-type-specific data.
			length = getMBInt(wbxml,wbxmlLength,index);
			if( length != 0 )
			{
				/*inner*/
				if( wbxml[index] < 0x1f )		//****
				{
					DWORD	xmlLength;
					CString strTemp = wbxmlToXml(&wbxml[index],length,xmlLength);
					addXmlData(lastLine,_T("<![CDATA["));
					if( strTemp.IsEmpty() )
						addXmlData(lastLine,_T("****ERROR****"));
					else
						addXmlData(lastLine,strTemp);
					addXmlData(lastLine,_T("]]>"));
				}
				else
					addXmlData(lastLine,&wbxml[index],length);
				/*inner*/
				/*inner
				bool		printable = true;
				const BYTE*	nextChar = &wbxml[index];
				int			todo = length;

				while( printable && todo )
				{
				todo--;
				printable = isprint((int)*nextChar) != 0 || isspace((int)*nextChar) != 0;
				nextChar++;
				}
				if( !printable )
				addXmlData(lastLine,(BYTE*)_T("<![CDATA["));
				addXmlData(lastLine,&wbxml[index],length);
				if( !printable )
				addXmlData(lastLine,(BYTE*)_T("]]>"));
				inner*/
			}
			index += length;
			break;
		default:
			{
				bool			hasAttributes = (tag & 0x80) != 0;
				bool			hasContent = (tag & 0x40) != 0;
				const TMap*		mapElement = tagCodePage;
				TMapStack*		stackItem;

				tag &= 0x3F;
				while(	mapElement->m_wbxml != tag && mapElement->m_wbxml != -1	)
					mapElement++;
				ASSERT(mapElement->m_wbxml != -1);
				if( mapElement->m_wbxml == -1 )
					return 0;					//*****	get out
				addXmlLine(firstLine,lastLine,level,tagCodePageNo,false);
				addXmlData(lastLine,_T("<"));
				addXmlData(lastLine,mapElement->m_xml);
				if( hasAttributes )
				{
					inAttributes = true;
					inAttributesWithContent = hasContent;
				}
				if(bSwitchCodePage)
				{
					addXmlData(lastLine,_T(" xmlns='"));
					addXmlData(lastLine,codePageNames[tagCodePageNo]);
					addXmlData(lastLine,_T("'"));
					bSwitchCodePage = false;
				}
				if( hasContent )
				{
					stackItem = new TMapStack;
					stackItem->m_entry = mapElement;
					stackItem->m_next = stack;
					stack = stackItem;
					level++;
					if( level > maxLevel )
						maxLevel = level;
					if( !inAttributes )
					{
						addXmlData(lastLine,_T(">"));
						attributeName = true;
					}
				}
				else
				{
					attributeName = true;
					addXmlData(lastLine,_T("/>"));
					addXmlLine(firstLine,lastLine,level-1,tagCodePageNo,false);
				}
			}
		}	//	end of switch tag
	}	//	end of while index < wbxmlLength

	//
	//	copy data to string
	//
	CString strXML;
	xmlLength = 0;
	
	lastLine = firstLine;
	if( _tcsstr(lastLine->m_strData,_T("xmlns")) == 0 )
	{
		CString strAttr;
		strAttr.Format(_T(" xmlns='%s'>"),codePageNames[lastLine->m_codePage]);
		lastLine->m_strData.Replace(_T(">"),strAttr);
	}
	while( lastLine )
	{
		for( int l = 0; l < lastLine->m_indent; l++ )
			strXML += _T("\t");
		strXML += lastLine->m_strData;
		strXML += _T("\r\n");
		lastLine = lastLine->m_next;
	}
	//
	//	tidy up
	//
	lastLine = firstLine;
	while( lastLine )
	{
		TXmlLine*	temp = lastLine->m_next;
		delete lastLine;
		lastLine = temp;
	}
	xmlLength = strXML.GetLength();
	return strXML;
}		//	end of wbxmlToXml

//
//////////////////////////////////////////////////////////////
//
//	Function :		xmlToWbxml
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		unsigned char*	-
//	Parameters :
//		xml
//			[IN ]	
//		xmlLength
//			[IN ]	
//		wbxml
//			[IN/OUT]	
//		wbxmlLength
//			[IN/OUT]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
bool xmlToWbxml(const TCHAR* xml,unsigned long xmlLength,char** pwbxml,int& wbxmlLength)
{
	bool bRes = false;	

	_bstr_t	xmlBstr(xml);
	wbxmlLength = 0;
	*pwbxml = 0;

	CoInitialize(NULL);
	bool makeWbxml(BSTR xml,char** pwbxml,int& wbxmlLength);
	bRes = makeWbxml(xmlBstr,pwbxml,wbxmlLength);
	CoUninitialize();
	return bRes;
}		//	end of xmlToWbxml

//
//////////////////////////////////////////////////////////////
//
//	Function :		codePageForXmlns
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		int	-
//	Parameters :
//		xmlns
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
int codePageForXmlns(const TCHAR* xmlns)
{
	int nRet = -1;
	if( _tcsnicmp(xmlns,devinf10,_tcslen(devinf10)) == 0 ||
		_tcsnicmp(xmlns,devinf11,_tcslen(devinf11)) == 0 ||
		_tcsnicmp(xmlns,devinfXmlsn,_tcslen(devinfXmlsn)) == 0)
		nRet = 2;
	else if( _tcsnicmp(xmlns,metinf10,_tcslen(devinf10)) == 0 ||
		_tcsnicmp(xmlns,metinf11,_tcslen(devinf11)) == 0 ||
		_tcsnicmp(xmlns,metinfXmlsn,_tcslen(metinfXmlsn)) == 0)
		nRet = 1;
	else if( _tcsnicmp(xmlns,syncml10,_tcslen(syncml10)) == 0 ||
		_tcsnicmp(xmlns,syncml11,_tcslen(syncml11)) == 0 ||
		_tcsnicmp(xmlns,syncmlXmlsn,_tcslen(syncmlXmlsn)) == 0)
		nRet = 0;
	return nRet;
}		//	end of codePageForXmlns
//
//////////////////////////////////////////////////////////////
//
//	Function :		nameToHex
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		int	-
//	Parameters :
//		name
//			[IN ]	
//		codePage
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
int nameToHex(const TCHAR* name,int codePage)
{
	int nRet = -1;
	const TMap* codePagePtr = codePages[codePage];
	if( codePagePtr != 0 )
	{
		while( codePagePtr->m_wbxml != -1 )
		{
			if( _tcscmp(name,codePagePtr->m_xml) == 0 )
			{
				nRet = codePagePtr->m_wbxml;
				break;
			}
			codePagePtr++;
		}
	}
	return nRet;
}		//	end of nameToHex

XptStatus XmlToWbxml(const TCHAR* xml,int xmlLength,char** pwbxml,int* wbxmlLength)
{
	return xmlToWbxml(xml,(DWORD)xmlLength,pwbxml,*wbxmlLength) ? XPT_STATUS_SUCCESS : XPT_STATUS_FAILED;
}
XptStatus WbxmlToXml(const BYTE* wbxml,int wbxmlLength,CString& xml,DWORD& xmlLength)
{
	xml = wbxmlToXml(wbxml,(DWORD)wbxmlLength,xmlLength);
	return xml.IsEmpty() ? XPT_STATUS_FAILED : XPT_STATUS_SUCCESS;
}
