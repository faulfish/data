
#include "stdafx.h"
#include <comutil.h>
#include "wbxmlSyncml.h"
#include "msxml2.h"
#pragma comment(lib, "msxml2.lib")
#include "..\..\CommonUtil\unicodeutility.h"

//
static unsigned char*		wbxmlBuffer = 0;
static int					wbxmlBufferSize = 0;
//
//	WBXML
//
//
//	Code page for Device info
//
const TMap	tDevInf[] =
{
	_T("CTCap"),0x05,
	_T("CTType"),0x06,
	_T("DataStore"),0x07,
	_T("DataType"),0x08,
	_T("DevID"),0x09,
	_T("DevInf"),0x0A,
	_T("DevTyp"),0x0B,
	_T("DisplayName"),0x0C,
	_T("DSMem"),0x0D,
	_T("Ext"),0x0E,
	_T("FwV"),0x0F,
	_T("HwV"),0x10,
	_T("Man"),0x11,
	_T("MaxGUIDSize"),0x12,
	_T("MaxID"),0x13,
	_T("MaxMem"),0x14,
	_T("Mod"),0x15,
	_T("OEM"),0x16,
	_T("ParamName"),0x17,
	_T("PropName"),0x18,
	_T("Rx"),0x19,
	_T("Rx-Pref"),0x1A,
	_T("SharedMem"),0x1B,
	_T("Size"),0x1C,
	_T("SourceRef"),0x1D,
	_T("SwV"),0x1E,
	_T("SyncCap"),0x1F,
	_T("SyncType"),0x20,
	_T("Tx"),0x21,
	_T("Tx-Pref"),0x22,
	_T("ValEnum"),0x23,
	_T("VerCT"),0x24,
	_T("VerDTD"),0x25,
	_T("Xnam"),0x26,
	_T("Xval"),0x27,
	_T("UTC"),0x28,
	_T("SupportNumberOfChanges"),0x29,
	_T("SupportLargeObjs"),0x2A,
	_T(""),-1
};
//
//	code page for Meta info
//
const TMap	tMetInf[] =
{
	_T("Anchor"),0x05,
	_T("EMI"),0x06,
	_T("Format"),0x07,
	_T("FreeID"),0x08,
	_T("FreeMem"),0x09,
	_T("Last"),0x0A,
	_T("Mark"),0x0B,
	_T("MaxMsgSize"),0x0C,
	_T("MaxObjSize"),0x15,
	_T("Mem"),0x0D,
	_T("MetInf"),0x0E,
	_T("Next"),0x0F,
	_T("NextNonce"),0x10,
	_T("SharedMem"),0x11,
	_T("Size"),0x12,
	_T("Type"),0x13,
	_T("Version"),0x14,
	_T(""),-1
};
//
//	Main code page for SyncML
//
const TMap	tSyncML[] =
{
	_T("Add"),0x05,
	_T("Alert"),0x06,
	_T("Archive"),0x07,
	_T("Atomic"),0x08,
	_T("Chal"),0x09,
	_T("Cmd"),0x0A,
	_T("CmdID"),0x0B,
	_T("CmdRef"),0x0C,
	_T("Copy"),0x0D,
	_T("Cred"),0x0E,
	_T("Data"),0x0F,
	_T("Delete"),0x10,
	_T("Exec"),0x11,
	_T("Final"),0x12,
	_T("Get"),0x13,
	_T("Item"),0x14,
	_T("Lang"),0x15,
	_T("LocName"),0x16,
	_T("LocURI"),0x17,
	_T("Map"),0x18,
	_T("MapItem"),0x19,
	_T("Meta"),0x1A,
	_T("MsgID"),0x1B,
	_T("MsgRef"),0x1C,
	_T("NoResp"),0x1D,
	_T("NoResults"),0x1E,
	_T("Put"),0x1F,
	_T("Replace"),0x20,
	_T("RespURI"),0x21,
	_T("Results"),0x22,
	_T("Search"),0x23,
	_T("Sequence"),0x24,
	_T("SessionID"),0x25,
	_T("SftDel"),0x26,
	_T("Source"),0x27,
	_T("SourceRef"),0x28,
	_T("Status"),0x29,
	_T("Sync"),0x2A,
	_T("SyncBody"),0x2B,
	_T("SyncHdr"),0x2C,
	_T("SyncML"),0x2D,
	_T("Target"),0x2E,
	_T("TargetRef"),0x2F,
	_T("XXXXXX"),0x30,				//	Reserved for future use. 
	_T("VerDTD"),0x31,
	_T("VerProto"),0x32,
	_T("NumberOfChanges"),0x33,
	_T("MoreData"),0x34,
	_T(""),-1
};
//
//	List of code pages for SyncML
//
const TMap* codePages[] =
{
	tSyncML,
	tMetInf,
	tDevInf,
	0
};
const TCHAR* codePageNames[] =
{
	_T("syncml:SYNCML1.1"),
	_T("syncml:metinf"),
	_T("syncml:devinf"),
	0
};
static const TMap*		currentCodePage = codePages[0];

//
//////////////////////////////////////////////////////////////
//
//	Function :		ShowErrorMessage
//	Description :	
//		show error message by HRESULT
//	Author :		Alan Tu
//	Return :
//		void	-
//	Parameters :
//		hr
//			[IN ]	value to result
//		strCaption
//			[IN ]	Message dialog's caption
//		strMsgFMT
//			[IN ]	Message dialog's message format like _T("Error 0x%08x")
//	Note :
//
//////////////////////////////////////////////////////////////
//
static void ShowErrorMessage(HRESULT hr,const CString& strCaption,const CString& strMsgFMT)
{
	if( hr != 0 )
	{
		TCHAR*	buffer = 0;
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			0,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),		// Default language
			(LPTSTR)&buffer,
			0,
			0);
		if( buffer )
		{
			::MessageBox(0,buffer,strCaption,MB_OK | MB_ICONERROR);
			LocalFree(buffer);
		}
		else
		{
			TCHAR message[256] = {0};
			_stprintf_s(message,sizeof(TCHAR)*256,strMsgFMT,hr);
			::MessageBox(0,message,strCaption,MB_OK | MB_ICONERROR);
		}
	}
}
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
static void makeMBInt(int value, BYTE*& wbxml,int& freeSpace,bool first)
{
	int			shifted = value >> 7;

	if( shifted != 0 )
		makeMBInt(shifted,wbxml,freeSpace,false);
	*wbxml++ = (value & 0x7F) | (first ? 0 : 0x80);
	freeSpace--;
}		//	end of makeMBInt
//
//////////////////////////////////////////////////////////////
//
//	Function :		checkSpace
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		bool	-
//	Parameters :
//		buffer
//			[IN/OUT]	
//		freeSpace
//			[IN/OUT]	
//		needed
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static bool checkSpace(unsigned char*& buffer,int& freeSpace,int needed)
{
	if( needed < freeSpace )
		return true;
	//
	int					used = wbxmlBufferSize - freeSpace;
	int					newBufferSize = wbxmlBufferSize * 2;

	while( needed >= (newBufferSize - used) )
	{
		newBufferSize *= 2;
	}
	unsigned char*		more = new unsigned char[newBufferSize];

	if( !more )
		return false;
	memcpy(more,wbxmlBuffer,used);
	delete [] wbxmlBuffer;
	buffer = &more[used];		//	same offset in new buffer space
	wbxmlBuffer = more;
	freeSpace = newBufferSize - used;
	wbxmlBufferSize = newBufferSize;
	return true;
}		//	end of checkSpace
//
//////////////////////////////////////////////////////////////
//
//	Function :		reportError
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		HRESULT	-
//	Parameters :
//		pXMLError
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static HRESULT reportError(IXMLDOMParseError*	pXMLError)
{
	long		line;
	long		linePos;
	LONG		errorCode;
	BSTR		pBReason;

	if(	!FAILED(pXMLError->get_line(&line))	&&
		!FAILED(pXMLError->get_linepos(&linePos)) &&
		!FAILED(pXMLError->get_errorCode(&errorCode)) &&
		!FAILED(pXMLError->get_reason(&pBReason)))
	{

		_bstr_t		msg(pBReason);
		TCHAR		buffer[128] = {0};

		if( line > 0 )
		{
			_stprintf_s(buffer,sizeof(TCHAR)*128,_T("\nError on line %d, position %d in XML"),line,linePos);
			msg += _bstr_t(buffer);
		}
		::MessageBox(0,(const TCHAR*)msg,_T("Make WBXML"),MB_OK | MB_ICONERROR);
	}
	SysFreeString(pBReason);
	return E_FAIL;
}		//	end of reportError
//
//////////////////////////////////////////////////////////////
//
//	Function :		checkLoad
//	Description :	
//		Check load results
//	Author :		Bill Holohan
//	Return :
//		HRESULT	-
//	Parameters :
//		pDoc
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static HRESULT checkLoad(CComPtr<IXMLDOMDocument> pDoc)
{
	//
	//	And since we don't have the VARIANT_BOOL from load we
	//	need to check the parse Error errorCode property to see
	//	if everything went OK.
	//
	CComPtr<IXMLDOMParseError>	pXMLError = NULL;
	LONG						errorCode = E_FAIL;
	if(	SUCCEEDED(pDoc->get_parseError(&pXMLError)) && SUCCEEDED(pXMLError->get_errorCode(&errorCode)))
	{
		if( errorCode != 0 )
			reportError(pXMLError);
	}
	return errorCode;
}		//	end of checkLoad
//
///////////////////////////////////////////////////////////
//
//	loadDocumentSync
//		Load an XML Document from the specified string synchronously.
//
//	parameters	:
//
//	return		:
//
///////////////////////////////////////////////////////////
//
static HRESULT loadDocumentSync(CComPtr<IXMLDOMDocument> pDoc,BSTR xml)
{
	VARIANT_BOOL					vb;
	HRESULT							hr;

	hr = pDoc->put_async(VARIANT_FALSE);
	//
	//	Load xml document from the given string
	//
	if( SUCCEEDED(hr) )
		hr = pDoc->loadXML(xml, &vb);
	hr = checkLoad(pDoc);
	return hr;
}		//	end of loadDocumentSync
//
//////////////////////////////////////////////////////////////
//
//	Function :		walkTree
//	Description :	
//		Walk all the Elements in a loaded XML document.
//	Author :		Bill Holohan
//	Return :
//		HRESULT	-
//	Parameters :
//		node
//			[IN ]	
//		inDTD
//			[IN ]	
//		appendHere
//			[IN/OUT]	
//		freeSpace
//			[IN/OUT]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static HRESULT walkTree(CComPtr<IXMLDOMNode> node,bool inDTD,unsigned char*& appendHere,int& freeSpace)
{
	static int				level = 0;
	CComPtr<IXMLDOMNode>	pChild;	
	BSTR					nodeName;
	BSTR					namespaceURI;
	DOMNodeType				nodeType;
	bool					hasContent = false;
	bool					hasAttributes = false;
	int						codePage = 0;
	HRESULT					hr = S_OK;

	node->get_nodeType(&nodeType);
	node->get_nodeName(&nodeName);
	//
	//	detect change of current code page (aka namespace)
	//
	node->get_namespaceURI(&namespaceURI);

	if(namespaceURI != 0)
	{
		while( codePageNames[codePage] != 0 )
		{
			if( _bstr_t(codePageNames[codePage]) == _bstr_t(namespaceURI) )
			{
				if( codePages[codePage] != currentCodePage )
				{
					currentCodePage = codePages[codePage];
					checkSpace(appendHere,freeSpace,2);
					*appendHere++ = SWITCH_PAGE;			//	switch page
					*appendHere++ = codePage;
					freeSpace -= 2;
				}
				break;
			}
			codePage++;
		}
	}
	//
	const TMap*	entry = currentCodePage;
	int			tagOffset = appendHere - wbxmlBuffer;

	if( nodeType == NODE_DOCUMENT_TYPE )
	{		//	start processing DTD
		inDTD = true;
	}
	if( inDTD )
	{
	}
	else
	{
		//
		//	add entry
		//
		CComPtr<IXMLDOMNamedNodeMap>	attrs;

		if( nodeType == NODE_ELEMENT )
		{
			while( _bstr_t(entry->m_xml) != _bstr_t(nodeName) )
			{
				entry++;
				if( entry->m_xml == _T("") )
					break;
			}
			_ASSERTE(entry->m_wbxml != -1);
			if( entry->m_wbxml == -1 )
				return E_FAIL;
			checkSpace(appendHere,freeSpace,1);
			*appendHere++ = (unsigned char)entry->m_wbxml;
			freeSpace--;
		}
		/*inner*/
		else if( nodeType == NODE_CDATA_SECTION )
		{
			VARIANT			value;

			node->get_nodeValue(&value);
			_ASSERTE(value.vt == VT_BSTR);
			int		len = _bstr_t(value.bstrVal).length();
			//****
			HRESULT						hr = S_OK;
			CComPtr<IXMLDOMDocument>	pDoc = NULL;
			CComPtr<IXMLDOMNode>		pNode = NULL;
			//
			//	Create an empty XML document
			//
			hr = CoCreateInstance(CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&pDoc);
			if( !FAILED(hr) )
			{
				hr = loadDocumentSync(pDoc,value.bstrVal);
				//
				//	Now walk the loaded XML document
				//
				if( hr == 0 )					//	loadDocumentSync can return 'normal' win32 errors
				{
					hr = pDoc->QueryInterface(IID_IXMLDOMNode,(void**)&pNode);
					if( !FAILED(hr) )
						walkTree(pNode,false,appendHere,freeSpace);
				}
			}
			//
			//	clean up
			//
			ShowErrorMessage(hr,_T("Make WBXML"),_T("readXML: Error code 0x%08x accessing XML"));
			if( FAILED(hr) )
				return hr;
			//****
		}
		/*inner*/
		else if( nodeType == NODE_COMMENT )
		{	//	discard comments
		}
		else if(nodeType == NODE_TEXT)
		{	//	terminal value
			VARIANT			value;

			node->get_nodeValue(&value);
			_ASSERTE(value.vt == VT_BSTR);
			int	len = _bstr_t(value.bstrVal).length();
			int nOrgLen = len;
			for( int c = 0; c < len; c++ )
			{
				if(c > 0 && value.bstrVal[c] == '\n')
				{
					if(value.bstrVal[c-1] != '\r')
					{
						len++;
					}
				}
			}

			checkSpace(appendHere,freeSpace,len + 4 + 1);
			*appendHere++ = OPAQUE;
			freeSpace--;
			makeMBInt(len,appendHere,freeSpace,true);
			for( int c = 0; c < nOrgLen; c++ )
			{
				if(c > 0 && value.bstrVal[c] == '\n')
					if(value.bstrVal[c-1] != '\r')
						*appendHere++ = '\r';
				*appendHere++ = (char)value.bstrVal[c];
			}
			freeSpace -= len;
			hasContent = true;
		}
		else if(  nodeType == NODE_PROCESSING_INSTRUCTION )
		{	//	terminal value
			//	e.g. <?xml version="1.0"/>
			// PI attrStart attrValue END
		}
		else
		{
			//
			//	get attributes
			//
			if(	SUCCEEDED(node->get_attributes(&attrs))	&& attrs != NULL)
			{
				IXMLDOMNode*			childAttr;

				attrs->nextNode(&childAttr);
				while( childAttr )
				{
					BSTR			name;
					VARIANT			var;

					var.vt = VT_NULL;
					childAttr->get_nodeName(&name);
					childAttr->get_nodeValue(&var);
					_ASSERTE(var.vt == VT_BSTR);
					if( _bstr_t(name) == _bstr_t("xmlns") )
					{	//	don't output xmlns attribute
					}
					else
					{
						hasAttributes = true;
						//
						int		len = _bstr_t(name).length();

						checkSpace(appendHere,freeSpace,len + 4 + 1);
						*appendHere++ = OPAQUE;
						freeSpace--;
						makeMBInt(len,appendHere,freeSpace,true);
						for( int c = 0; c < len; c++ )
						{
							*appendHere++ = (char)name[c];
						}
						freeSpace -= len;
						//
						len = _bstr_t(var.bstrVal).length();
						checkSpace(appendHere,freeSpace,len + 4 + 1);
						*appendHere++ = OPAQUE;
						freeSpace--;
						makeMBInt(len,appendHere,freeSpace,true);
						for( int v = 0; v < len; v++ )
						{
							*appendHere++ = (char)var.bstrVal[v];
						}
						freeSpace -= len;
						checkSpace(appendHere,freeSpace,1);
						*appendHere++ = (char)END;				//	end of attributes
						freeSpace--;
					}
					::SysFreeString(name);
					VariantClear(&var);
					childAttr->Release();
					attrs->nextNode(&childAttr);
				}
			}
		}
	}	//	not in DTD
	SysFreeString(nodeName);

	if( !pChild )
		node->get_firstChild(&pChild);
	while( pChild )
	{
		hasContent = true;
		hr = walkTree(pChild,inDTD,appendHere,freeSpace);
		if( FAILED(hr) )
			return hr;
		CComPtr<IXMLDOMNode> pNext;
		pChild->get_nextSibling(&pNext);
		pChild.Release();
		pChild = pNext;
	}
	if( nodeType == NODE_ELEMENT )
	{
		if( hasAttributes )
			wbxmlBuffer[tagOffset] |= cHasAttributes;
		if( hasContent )
			wbxmlBuffer[tagOffset] |= cHasContent;
		if( hasAttributes || hasContent )
		{
			checkSpace(appendHere,freeSpace,1);
			*appendHere++ = (unsigned char)END;
			freeSpace--;
		}
	}
	return S_OK;
}		//	end of walkTree
//
///////////////////////////////////////////////////////////
//
//	readXML
//
//	parameters	:
//
//	return		:
//
///////////////////////////////////////////////////////////
//
bool makeWbxml(BSTR xml,char** pwbxml,int& wbxmlLength)
{
	HRESULT						hr = S_OK;
	CComPtr<IXMLDOMDocument>	pDoc = NULL;
	CComPtr<IXMLDOMNode>		pNode = NULL;
	int							freeSpace = 0;
	unsigned char*				appendHere = 0;
	
	//
	//	Create an empty XML document
	//
	hr = CoCreateInstance(CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&pDoc);
	if( !FAILED(hr) )
	{
		hr = loadDocumentSync(pDoc,xml);
		//
		//	Now walk the loaded XML document saving the data in deviceList
		//
		if( hr == S_OK )	//	loadDocumentSync can return 'normal' win32 errors
		{
			hr = pDoc->QueryInterface(IID_IXMLDOMNode,(void**)&pNode);
			if( !FAILED(hr) )
			{
				wbxmlBufferSize = 100;
				wbxmlBuffer = new unsigned char[wbxmlBufferSize];
				memset(wbxmlBuffer,0,wbxmlBufferSize);
				appendHere = wbxmlBuffer;
				//
				//	Add standard stuff
				//
				*appendHere++ = 0x02;		//	version 1.2
				*appendHere++ = 0x00;
				*appendHere++ = 0x00;
				*appendHere++ = 0x6a;		//	charset UTF-8
				*appendHere++ = 0x1e;		//	string table length

				#ifdef _UNICODE
				QSYNC_LIB::UnicodeToAnsiBuffer(syncml11,(char*)appendHere,wbxmlBufferSize-6);
				#else
				strcpy((char*)appendHere,syncml11);
				#endif

				appendHere += _tcslen(syncml11) + 1;
				freeSpace = wbxmlBufferSize - (appendHere - wbxmlBuffer);
				//
				//	walk the tree
				//
				walkTree(pNode,false,appendHere,freeSpace);
			}
		}
	}
	//
	//	clean up
	//
	ShowErrorMessage(hr,_T("Make WBXML"),_T("readXML: Error code 0x%08x accessing XML"));

	bool bRes = SUCCEEDED(hr);
	if( bRes )
	{
		*pwbxml = (char*)wbxmlBuffer;
		wbxmlLength = wbxmlBufferSize - freeSpace;
	}
	return true;
}		//	end of makeWbxml

//
//////////////////////////////////////////////////////////////
//
//	Function :		getDataFromSyncML
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		BSTR	-
//	Parameters :
//		xml
//			[IN ]	
//		xslt
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
BSTR getDataFromSyncML(BSTR xml,BSTR xslt)
{
	HRESULT							hr = S_OK;
	CComPtr<IXMLDOMDocument>		xmlDoc = NULL;
	CComPtr<IXMLDOMDocument>		xsltDoc = NULL;
	BSTR							xmlOut;
	//
	//	Create the XML document
	//
	hr = CoCreateInstance(CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&xmlDoc);
	if( SUCCEEDED(hr) )
		hr = loadDocumentSync(xmlDoc,xml);
	//
	//	Create the XSLT document
	//
	if( SUCCEEDED(hr) )
		hr = CoCreateInstance(CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&xsltDoc);
	if( SUCCEEDED(hr) )
		hr = loadDocumentSync(xsltDoc,xslt);
	if( SUCCEEDED(hr) )
		hr = xmlDoc->transformNode(xsltDoc,&xmlOut);

	ShowErrorMessage(hr,_T("getDataFromSyncML"),_T("Error code 0x%08x for XML"));
	return FAILED(hr) ? 0 : xmlOut;
}		//	end of getDataFromSyncML
//
//////////////////////////////////////////////////////////////
//
//	Function :		extractCtCap
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		HRESULT	-
//	Parameters :
//		node
//			[IN ]	
//		infoLog
//			[IN/OUT]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
static HRESULT extractCtCap(CComPtr<IXMLDOMNode> node,BSTR& infoLog)
{
	CComPtr<IXMLDOMNode>	pChild;
	BSTR					nodeName;
	DOMNodeType				nodeType;
	_bstr_t					findType(_T("text/x-vcard"));

	node->get_nodeType(&nodeType);
	node->get_nodeName(&nodeName);
	if( nodeType == NODE_DOCUMENT_TYPE )
	{		//	start processing DTD
		return S_OK;
	}
	//
	//	reached CTCap?
	//
	if( nodeType == NODE_ELEMENT &&	_bstr_t(nodeName) == _bstr_t(_T("CTCap")))
	{
		int				telFields = 0;
		_bstr_t			infoLogData;
		_bstr_t			paramName;
		_bstr_t			propName;
		_bstr_t			valEnum;
		_bstr_t			entry(_T(""));
		_bstr_t			newline(_T("\n"));
		bool			startProp = true;
		bool			gotValEnum = false;
		bool			collect = false;
		bool			previousIsPropName = false;

		node->get_firstChild(&pChild);
		while( pChild )
		{
			BSTR		text;

			SysFreeString(nodeName);
			pChild->get_nodeType(&nodeType);
			pChild->get_nodeName(&nodeName);
			//
			_bstr_t		name(nodeName);

			pChild->get_text(&text);
			if( name == _bstr_t(_T("CTType")) )
			{
				if( collect )
					break;			//	got the CTType we were looking for so stop
				collect = findType == _bstr_t(text);
			}
			else if( collect ) 
			{
				bool			isPropName = false;

				if( name == _bstr_t(_T("PropName")) )
				{
					if( entry.length() != 0 )
					{
						if( !gotValEnum )
							entry += _T(":");
						infoLogData += entry;
						infoLogData += newline;
						entry = _T("");
					}
					startProp = true;
					gotValEnum = false;
					isPropName = true;
					propName = text;
					entry = text;
				}
				else if( name == _bstr_t(_T("ParamName")) )
				{
					if( propName == _bstr_t(_T("TEL")) )
					{
						if( telFields == 0 )
						{
							infoLogData += _T("X-TEL-FIELDS:%d");
							infoLogData += newline;
						}
						telFields++;
					}
					paramName = text;
					if( !startProp )
					{
						if( !gotValEnum )
							entry += _T(":");
						infoLogData += entry;
						infoLogData += newline;
						entry = propName;
					}
					gotValEnum = false;
					entry += _bstr_t(_T(";"));
					startProp = false;
					entry += _bstr_t(_T("TYPE="));
					entry += paramName;
				}
				else if( name == _bstr_t(_T("ValEnum")) )
				{
					gotValEnum = true;
					valEnum = text;
					if( previousIsPropName )
						entry += _T(":");
					entry += _T("=");
					entry += valEnum;
				}
				else
				{
				}
				previousIsPropName = isPropName;
			}
			CComPtr<IXMLDOMNode> pNext;
			pChild->get_nextSibling(&pNext);
			pChild.Release();
			pChild = pNext;
			SysFreeString(text);
		}
		if( entry.length() != 0 )
		{
			if( !gotValEnum )
				entry += _T(":");
			infoLogData += entry;
			infoLogData += newline;
			entry = _T("");
		}
		if( infoLogData.length() != 0 )
		{
			infoLog = SysAllocStringLen(0,infoLogData.length()+10);		//	allow for telFields expanding to 10 digits!
			swprintf((wchar_t*)infoLog,(const wchar_t*)infoLogData,telFields);
		}
		return S_OK;
	}
	SysFreeString(nodeName);

	HRESULT	hr = E_FAIL;
	if( !pChild )
		node->get_firstChild(&pChild);
	while( pChild )
	{
		hr = extractCtCap(pChild,infoLog);
		if( FAILED(hr) || infoLog != 0 )
			break;
		CComPtr<IXMLDOMNode> pNext;
		pChild->get_nextSibling(&pNext);
		pChild.Release();
		pChild = pNext;
	}
	return hr;
}		//	end of extractCtCap
//
//////////////////////////////////////////////////////////////
//
//	Function :		makeInfoLogFromDevInf
//	Description :	
//	Author :		Bill Holohan
//	Return :
//		BSTR	-
//	Parameters :
//		xml
//			[IN ]	
//	Note :
//
//////////////////////////////////////////////////////////////
//
BSTR makeInfoLogFromDevInf(BSTR xml)
{
	HRESULT						hr = S_OK;
	CComPtr<IXMLDOMDocument>	pDoc = NULL;
	CComPtr<IXMLDOMNode>		pNode = NULL;
	BSTR						infoLog = 0;
	//
	//	Create an empty XML document
	//
	hr = CoCreateInstance(CLSID_DOMDocument,NULL,CLSCTX_INPROC_SERVER,IID_IXMLDOMDocument,(void**)&pDoc);
	if( !FAILED(hr) )
	{
		hr = loadDocumentSync(pDoc,xml);
		//
		//	Now walk the loaded XML document saving the data
		//
		if( hr == 0 )					//	loadDocumentSync can return 'normal' win32 errors
		{
			hr = pDoc->QueryInterface(IID_IXMLDOMNode,(void**)&pNode);
			if( SUCCEEDED(hr) )
				extractCtCap(pNode,infoLog);
		}
	}
	//
	//	clean up
	//
	ShowErrorMessage(hr,_T("makeInfoLogFromDevInf"),_T("Error code 0x%08x for XML"));
	return !FAILED(hr) ? infoLog : 0;
}		//	end of makeInfoLogFromDevInf
