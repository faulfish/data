// FolderListingCodec.cpp: implementation of the CFolderListingCodec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FolderListingCodec.h"

#include "msxml4.tlh"
//#import "msxml4.dll"  named_guids raw_interfaces_only
//using namespace MSXML2;
//#include "msxml2.h"

CComBSTR bstrObexDtd =
L"<!-- DTD for the OBEX Folder-Listing Object -->"
L"<!ELEMENT folder-listing ( folder | file | parent-folder )* >"
L"<!ATTLIST folder-listing version CDATA \"1.0\">"
L"<!ELEMENT file (#PCDATA)>"
L"<!ATTLIST file name CDATA #REQUIRED>"
L"<!ATTLIST file size CDATA #IMPLIED>"
L"<!ATTLIST file type CDATA #IMPLIED>"
L"<!ATTLIST file modified CDATA #IMPLIED>"
L"<!ATTLIST file created CDATA #IMPLIED>"
L"<!ATTLIST file accessed CDATA #IMPLIED>"
L"<!ATTLIST file user-perm NMTOKEN #IMPLIED>"
L"<!ATTLIST file group-perm NMTOKEN #IMPLIED>"
L"<!ATTLIST file other-perm NMTOKEN #IMPLIED>"
L"<!ATTLIST file group CDATA #IMPLIED>"
L"<!ATTLIST file owner CDATA #IMPLIED>"
L"<!ATTLIST file xml:lang NMTOKEN #IMPLIED>"
L"<!ELEMENT folder (#PCDATA) >"
L"<!ATTLIST folder name CDATA #REQUIRED>"
L"<!ATTLIST folder size CDATA #IMPLIED>"
L"<!ATTLIST folder modified CDATA #IMPLIED>"
L"<!ATTLIST folder created CDATA #IMPLIED>"
L"<!ATTLIST folder accessed CDATA #IMPLIED>"
L"Object Exchange Protocol Version 1.3"
L"73"
L"<!ATTLIST folder user-perm NMTOKEN #IMPLIED>"
L"<!ATTLIST folder group-perm NMTOKEN #IMPLIED>"
L"<!ATTLIST folder other-perm NMTOKEN #IMPLIED>"
L"<!ATTLIST folder group CDATA #IMPLIED>"
L"<!ATTLIST folder owner CDATA #IMPLIED>"
L"<!ATTLIST folder xml:lang NMTOKEN #IMPLIED>"
L"<!ELEMENT parent-folder EMPTY>";



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFolderListingCodec::CFolderListingCodec()
{
	::CoInitialize(NULL);
}

CFolderListingCodec::~CFolderListingCodec()
{
	::CoUninitialize();
}

bool CFolderListingCodec::DecodeBuffer( CBufferPtr spBuffer )
{
	HRESULT hr = S_OK;
	bool bSuccess = true;
	const size_t MAX_UTF8_CHARACTERLENGTH = 3;

	if( spBuffer != NULL )
	{
		CEntry entry;
		DWORD dwLength = spBuffer ? spBuffer->GetLength() : 0;
		const char* pcBuffer = spBuffer ? spBuffer->GetData() : "";                          

		std::string strTempXML;        
		{
			char strDtd[] = "<!DOCTYPE folder-listing SYSTEM \"obex-folder-listing.dtd\">";
			size_t dwDtdLength = strlen(strDtd);
			size_t dwRead = 0;
			size_t dwWrite = 0;

			for( dwRead = 0, dwWrite = 0; dwRead < dwLength; dwRead++, dwWrite++ )
			{
				if( dwRead + dwDtdLength < dwLength && !memcmp(pcBuffer+dwRead,strDtd,dwDtdLength) )             
				{
					dwRead += dwDtdLength;
					if( dwRead >= dwLength )
						break;
				}
				strTempXML += pcBuffer[dwRead];
			}            
		}

		MakeListingXMLCompliant(&strTempXML);

		size_t dwStringLength = strTempXML.length()+1;

		CComPtr<IXMLDOMDocument> spXMLDOMDocument;
		CComBSTR bstrName;        
		CComBSTR bstrXML( static_cast<int>(dwStringLength) + 2 );

		MultiByteToWideChar( CP_UTF8, 0, strTempXML.c_str(), static_cast<int>(strTempXML.length())+1, bstrXML, static_cast<int>(dwStringLength)+1 );

		VARIANT_BOOL bSuccessful = false;

		hr = spXMLDOMDocument.CoCreateInstance( MSXML2::CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER );

		bSuccessful = SUCCEEDED(hr);			

		if(bSuccessful)
		{
			hr = spXMLDOMDocument->put_async( VARIANT_FALSE );        
			hr = spXMLDOMDocument->loadXML( bstrXML, &bSuccessful );
		}
		else
		{
			ASSERT(0);//lack MSXML2 library
		}

		if( bSuccessful )
		{
			CComPtr<IXMLDOMNode> spNode;
			CComPtr<IXMLDOMNode> spNodeNext;
			CComPtr<IXMLDOMNode> spNodeName;
			CComPtr<IXMLDOMNode> spNodeFolderListing;
			CComPtr<IXMLDOMNode> spNodeEntry;
			CComVariant varName;


			hr = spXMLDOMDocument->get_firstChild(&spNode);

			while(spNode)
			{
				bstrName.Empty();
				hr = spNode->get_nodeName( &bstrName );
				if( bstrName == CComBSTR(L"folder-listing") )
				{
					spNodeFolderListing = spNode;
					break;
				}

				spNodeNext = NULL;
				hr = spNode->get_nextSibling( &spNodeNext );
				spNode = spNodeNext;
			}

			if( spNodeFolderListing )
			{
				for( hr = spNodeFolderListing->get_firstChild(&spNodeEntry); spNodeEntry; )
				{
					// Get the type (folder or file)
					entry.reset();
					bstrName.Empty();
					hr = spNodeEntry->get_nodeName( &bstrName );

					if( bstrName )
					{
						if( bstrName == CComBSTR("folder") )
							entry.m_bAttributeDirectory = true;
						else // ( bstrName == CComBSTR("file") )
							entry.m_bAttributeDirectory = false;
					}

					// Get the name of the entry
					CComPtr<IXMLDOMNamedNodeMap> spAttributes = NULL;
					hr = spNodeEntry->get_attributes( &spAttributes );

					entry.m_dateCreationTime = COleDateTime();
					entry.m_dateLastAccessTime = COleDateTime();
					entry.m_dateLastWriteTime = COleDateTime();

					if( spAttributes )
					{
						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("name"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_BSTR) == S_OK )
						{
							entry.m_strName = varName.bstrVal;
						}
						else
							continue;

						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("size"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_I4) == S_OK )
						{
							entry.m_dwFileSize = varName.lVal;
						}
						else
						{
							entry.m_dwFileSize = 0;
						}

						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("user-perm"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_BSTR) == S_OK )
						{
							CString strUserPerm = varName.bstrVal;
							entry.m_bAttributeHidden = (strUserPerm.Find(_T("R")) >= 0) ? false : true;
							entry.m_bAttributeReadonly = (strUserPerm.Find(_T("W")) >= 0) ? false : true;
							entry.m_bAttributeSystem = (strUserPerm.Find(_T("D")) >= 0) ? false : true;
						}

						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("group-perm"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_BSTR) == S_OK )
						{
							CString strGroupPerm = varName.bstrVal;
							entry.m_bAttributeArchive = (strGroupPerm.Find(_T("R")) >= 0) ? true : false;
						}


						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("created"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_BSTR) == S_OK )
						{
							CString strCreated = varName.bstrVal;
							int nYear = 0;
							int nMonth = 0;
							int nDayOfMonth = 0;
							int nHour = 0;
							int nMinute = 0;
							int nSecond = 0;

							if(!strCreated.IsEmpty())
							{
								_stscanf_s( strCreated,_T("%4d%02d%02dT%02d%02d%02d"), &nYear, &nMonth, &nDayOfMonth, &nHour,&nMinute, &nSecond );								
								entry.m_dateCreationTime.SetDateTime( nYear, nMonth, nDayOfMonth, nHour, nMinute, nSecond );
							}
						}

						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("modified"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_BSTR) == S_OK )
						{
							CString strDateTime = varName.bstrVal;
							int nYear = 0;
							int nMonth = 0;
							int nDayOfMonth = 0;
							int nHour = 0;
							int nMinute = 0;
							int nSecond = 0;

							if(!strDateTime.IsEmpty())
							{
								_stscanf_s( strDateTime, _T("%4d%02d%02dT%02d%02d%02d"), &nYear, &nMonth, &nDayOfMonth, &nHour,&nMinute, &nSecond );
								entry.m_dateLastWriteTime.SetDateTime( nYear, nMonth, nDayOfMonth, nHour, nMinute, nSecond );
							}
						}

						spNodeName = NULL;
						varName.Clear();
						hr = spAttributes->getNamedItem( CComBSTR("accessed"), &spNodeName );
						if( spNodeName )
						{
							hr = spNodeName->get_nodeValue( &varName );
						}
						if( varName.ChangeType(VT_BSTR) == S_OK )
						{
							CString strDateTime = varName.bstrVal;
							int nYear = 0;
							int nMonth = 0;
							int nDayOfMonth = 0;
							int nHour = 0;
							int nMinute = 0;
							int nSecond = 0;

							if(!strDateTime.IsEmpty())
							{
								_stscanf_s( strDateTime, _T("%4d%02d%02dT%02d%02d%02d"), &nYear, &nMonth, &nDayOfMonth, &nHour,&nMinute, &nSecond );
								entry.m_dateLastAccessTime.SetDateTime( nYear, nMonth, nDayOfMonth, nHour, nMinute, nSecond );
							}
						}
					}

					m_listEntry.push_back( entry );

					spNode = NULL;
					hr = spNodeEntry->get_nextSibling(&spNode);
					spNodeEntry = spNode;
				}
			}
		}
		else
		{
			CComPtr<IXMLDOMParseError> spXMLDOMParseError;
			CComBSTR bstrErrorReason;

			hr = spXMLDOMDocument->get_parseError( &spXMLDOMParseError );

			if( SUCCEEDED(hr) && spXMLDOMParseError )
			{
				hr = spXMLDOMParseError->get_reason( &bstrErrorReason );
			}
		}
	}
	else
		bSuccess = false;

	return bSuccess;
}

HRESULT CFolderListingCodec::MakeListingXMLCompliant(std::string* strListing)
{    
	size_t pos  = strListing->find('&');

	while(pos!=std::string::npos)
	{
		//ATLTRACE("\n");ATLTRACE(strListing->c_str()); ATLTRACE("\n");
		std::string newStr = strListing->substr(0, pos);
		newStr += "&amp;";
		newStr += strListing->substr(pos+1);
		*strListing = newStr;

		//ATLTRACE("\n");ATLTRACE(strListing->c_str()); ATLTRACE("\n");
		pos = strListing->find('&',pos+1);

	}
	return S_OK;
}

bool CFolderListingCodec::ContainsFile( LPCTSTR szFilename )
{
	bool bFound = false;
	std::vector<CEntry>::iterator it;

	for( it = m_listEntry.begin(); it != m_listEntry.end(); it++ )
	{
		if( it->m_strName.CompareNoCase(szFilename) == 0 )
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}


