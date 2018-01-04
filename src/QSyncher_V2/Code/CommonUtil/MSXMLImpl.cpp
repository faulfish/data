/*********************************************************************************\
  FileName: MSXMLImpl.cpp

  Descript: Implement for A wapper class for Microsoft XML document manager

  Creater: Alan tu

  Create Date:2006/12/15
\*********************************************************************************/

#include "stdafx.h"
#include "MSXMLImpl.h"
#include <comutil.h>
#include "..\CommonUtil\CalculateFunctionTime.h"
#ifdef _UNICODE
#pragma comment(lib, "comsuppw.lib")
#else
#pragma comment(lib, "comsupp.lib")
#endif
#include "msxml2.h"
#pragma comment(lib, "msxml2.lib")

const wstring toWString(const wchar_t *wstr){return wstr;}
const wstring toWString(const char* str){return _com_util::ConvertStringToBSTR(str);}
const wstring toWString(const TSTRING& tstr)
{
	#ifdef _UNICODE
		return tstr;
	#else		
		return _com_util::ConvertStringToBSTR(str.c_str());
	#endif
}

TSTRING toTSTRING(const char* str)
{
	#ifdef _UNICODE
	return _com_util::ConvertStringToBSTR(str);
	#else
		return str;
	#endif
}

TSTRING toTSTRING(BSTR bstr)
{
	#ifdef _UNICODE
		return bstr;	
	#else
		return _com_util::ConvertBSTRToString(bstrValue);
	#endif
}

CComPtr<IXMLDOMDocument> PrepareDOMDocument(CComPtr<IXMLDOMDocument> pIXMLDOMDocument)
{
	CComPtr<IXMLDOMDocument> pRet;
	
	bool bResult = false;	
	try
	{
		HRESULT hr;
		hr=CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_SERVER, 
			IID_IXMLDOMDocument, (LPVOID*)(&pIXMLDOMDocument));
		SUCCEEDED(hr) ? 0 : throw hr;
		pRet = pIXMLDOMDocument;
	}
	catch(...){}	
	return pRet;
}

//interface from XMLLibraryInterface
bool MSXMLLibrary::init()
{
	CoInitialize(NULL);
	return true;
}
bool MSXMLLibrary::uninit()
{
	CoUninitialize();	
	return true;
}

void RecursiveLoad(CComPtr<IXMLDOMNode> pChildNode,XMLElement *pElement,XMLElement *pParentElement)
{
	HRESULT hr;
	BSTR bstrValue;

	//Load current child	
	DOMNodeType type;
	hr=pChildNode->get_nodeType(&type);

	if(type == NODE_ELEMENT)
	{
		pChildNode->get_baseName(&bstrValue);
		pElement->setname(toTSTRING(bstrValue));

		CComPtr<IXMLDOMNamedNodeMap>	pDOMNamedNodeMap;
		hr=pChildNode->get_attributes(&pDOMNamedNodeMap);
		if(SUCCEEDED(hr) && pDOMNamedNodeMap) {
			CComPtr<IXMLDOMNode>	pAttrNode;
			while(SUCCEEDED(pDOMNamedNodeMap->nextNode(&pAttrNode)) && pAttrNode) {
				XMLAttribute attribute;
				HRESULT hr = pAttrNode->get_baseName(&bstrValue);				
				attribute.setname(toTSTRING(bstrValue));
				if(attribute.name().size() == 0)
					attribute.setname(_T("xmlns"));
				pAttrNode->get_text(&bstrValue);
				attribute.setvalue(toTSTRING(bstrValue));				
				pElement->getAllAttribute()->push_back(attribute);				
				pAttrNode.Release();
			};
		}
	}
	else if(type == NODE_TEXT)
	{	
		if(pParentElement) {
			pChildNode->get_text(&bstrValue);
			pParentElement->setvalue(toTSTRING(bstrValue));
		}
	}

	//Rescursive find next child	
	VARIANT_BOOL hasChild;
	hr=pChildNode->hasChildNodes(&hasChild);
	if(SUCCEEDED(hr) && hasChild && type == NODE_ELEMENT) {
		CComPtr<IXMLDOMNodeList> pNodeList;
		if(SUCCEEDED(pChildNode->get_childNodes(&pNodeList)) && pNodeList) {
			CComPtr<IXMLDOMNode> pChildNode;
			while(SUCCEEDED(pNodeList->nextNode(&pChildNode)) && pChildNode) {
				XMLElement newElement;
				pElement->getAllElement()->push_back(newElement);
				XMLAllElement::iterator lastIter;
				for(XMLAllElement::iterator Iter = pElement->getAllElement()->begin();Iter != pElement->getAllElement()->end();Iter++)
					lastIter = Iter;
				RecursiveLoad(pChildNode,&(*lastIter),pElement);
				pChildNode.Release();
			}
		}
	}
}

bool MSXMLLibrary::loadXML(const BSTR bstrXML,XMLMANAGER::XMLDocument* pXMLDoc,const BSTR bstrXSLT)
{
	#ifdef _DEBUG
	CalFunctionTime tic(_T("MSXMLLibrary::loadXML"));
	#endif
	bool bRes = false;

	//prepare DOM document	
	CComPtr<IXMLDOMDocument> pIXMLDOMDocument;
	pIXMLDOMDocument = PrepareDOMDocument(pIXMLDOMDocument);
	
	VARIANT_BOOL varisSuccessful;
	if(bstrXSLT) {
		HRESULT hr;
		VARIANT_BOOL varfalse = VARIANT_FALSE;
		CComPtr<IXMLDOMDocument> pIXMLDOMDocumentData;
		CComPtr<IXMLDOMDocument> pIXMLDOMDocumentXSLT;
		//prepare DOM document		
		{		
			#ifdef _DEBUG
			CalFunctionTime tic(_T("prepare DOM document"));
			#endif
			
			pIXMLDOMDocumentData = PrepareDOMDocument(pIXMLDOMDocumentData);
			pIXMLDOMDocumentData->put_async(varfalse);
			hr = pIXMLDOMDocumentData->loadXML(bstrXML,&varisSuccessful);
			ASSERT(SUCCEEDED(hr) && varisSuccessful == VARIANT_TRUE);
		}
		//prepare DOM document		
		{		
			#ifdef _DEBUG
			CalFunctionTime tic(_T("prepare XSLT document"));
			#endif
			
			pIXMLDOMDocumentXSLT = PrepareDOMDocument(pIXMLDOMDocumentXSLT);
			pIXMLDOMDocumentXSLT->put_async(varfalse);
			hr = pIXMLDOMDocumentXSLT->loadXML(bstrXSLT,&varisSuccessful);
			ASSERT(SUCCEEDED(hr) && varisSuccessful == VARIANT_TRUE);
		}

		//
		BSTR bstrXMLRes;
		{		
			#ifdef _DEBUG
			CalFunctionTime tic(_T("transformNode"));;
			#endif

			pIXMLDOMDocumentData->transformNode(pIXMLDOMDocumentXSLT,&bstrXMLRes);
		}

		{		
			#ifdef _DEBUG
			CalFunctionTime tic(_T("pIXMLDOMDocument->loadXML1"));;
			#endif
			pIXMLDOMDocument->loadXML(bstrXMLRes,&varisSuccessful);
		}
	}
	else {
		HRESULT hr;
		{		
			#ifdef _DEBUG
			CalFunctionTime tic(_T("pIXMLDOMDocument->loadXML2"));;
			#endif
			hr = pIXMLDOMDocument->loadXML(bstrXML,&varisSuccessful);
		}
		ASSERT(SUCCEEDED(hr) && varisSuccessful == VARIANT_TRUE);
	}

	{		
		#ifdef _DEBUG
		CalFunctionTime tic(_T("pIXMLDOMDocument"));;
		#endif
		if(pIXMLDOMDocument) {
			short sResult = FALSE;
			HRESULT hr;
			IXMLDOMElement* pIXMLDOMElement = NULL;
			//Get root element
			hr=pIXMLDOMDocument->get_documentElement(&pIXMLDOMElement);
			if(pIXMLDOMElement && SUCCEEDED(hr)) {
				CComPtr<IXMLDOMNode> pNode(static_cast<IXMLDOMNode*>(pIXMLDOMElement));
				XMLElement newElement;
				pXMLDoc->getAllElement()->push_back(newElement);
				{		
					#ifdef _DEBUG
					CalFunctionTime tic(_T("RecursiveLoad"));;
					#endif
					RecursiveLoad(pNode,&(*(pXMLDoc->getAllElement()->begin())),NULL);
				}
			}			
			bRes = true;
		}
	}
	
	return bRes;
}

bool MSXMLLibrary::load(const TSTRING& strFileName,XMLMANAGER::XMLDocument* pXMLDoc)
{
	bool bRes = false;
	
	//clear
	if(pXMLDoc)
		pXMLDoc->getAllElement()->clear();

	//prepare DOM document	
	CComPtr<IXMLDOMDocument> pIXMLDOMDocument;
	pIXMLDOMDocument = PrepareDOMDocument(pIXMLDOMDocument);

	if(pIXMLDOMDocument) {
		short sResult = FALSE;
		HRESULT hr;
		hr=pIXMLDOMDocument->load(_variant_t(toWString(strFileName).c_str()),&sResult);
		if(SUCCEEDED(hr) && (sResult==VARIANT_TRUE)) {			
			IXMLDOMElement* pIXMLDOMElement = NULL;
			//Get root element
			hr=pIXMLDOMDocument->get_documentElement(&pIXMLDOMElement);
			if(pIXMLDOMElement && SUCCEEDED(hr)) {
				CComPtr<IXMLDOMNode> pNode(static_cast<IXMLDOMNode*>(pIXMLDOMElement));
				XMLElement newElement;
				pXMLDoc->getAllElement()->push_back(newElement);
				RecursiveLoad(pNode,&(*(pXMLDoc->getAllElement()->begin())),NULL);
			}			
			bRes = true;
		}
	}
	
	return bRes;
}

HRESULT WriteElement(XMLMANAGER::XMLElement *pElement,IXMLDOMDocument* pIXMLDOMDocument,IXMLDOMNode *pElementNode)
{
	HRESULT hr = E_FAIL;
	if(pElement && pIXMLDOMDocument && pElementNode)
	{
		hr=pElementNode->put_text((BSTR)toWString(pElement->value()).c_str());
		if(SUCCEEDED(hr))
		{
			for(XMLAllAttribute::iterator Iter = pElement->getAllAttribute()->begin();Iter != pElement->getAllAttribute()->end();Iter++)
			{//write element attribuate name
				HRESULT hr;
				VARIANT vtTemp;

				vtTemp.vt=VT_I2;
				vtTemp.iVal = NODE_ATTRIBUTE;		

				CComPtr<IXMLDOMNode>			pRetNode;
				CComPtr<IXMLDOMNode>			pAttributeNode;
				CComPtr<IXMLDOMNamedNodeMap>	pDOMNamedNodeMap;

				hr=pElementNode->get_attributes(&pDOMNamedNodeMap);
				if(SUCCEEDED(hr)) {
					hr=pIXMLDOMDocument->createNode(vtTemp,(BSTR)toWString(Iter->name()).c_str() , L"",&pAttributeNode);
					if(SUCCEEDED(hr)) {
						hr=pAttributeNode->put_text((BSTR)toWString(Iter->value()).c_str());
						if(SUCCEEDED(hr)) {
							hr=pDOMNamedNodeMap->setNamedItem(pAttributeNode,&pRetNode);
						}
					}
				}
			}
		}
	}

	return hr;
}

HRESULT RecursiveSave(XMLElement* pElement,IXMLDOMDocument* pIXMLDOMDocument,IXMLDOMNode *pElementNode)
{
	HRESULT hr = E_FAIL;
	VARIANT vtTemp;
	vtTemp.vt=VT_I2;
	vtTemp.iVal = NODE_ELEMENT;

	for(XMLAllElement::iterator Iter = pElement->getAllElement()->begin();Iter != pElement->getAllElement()->end();Iter++)
	{
		CComPtr<IXMLDOMNode> pNewElement;
		CComPtr<IXMLDOMNode> pRetElement;
		hr=pIXMLDOMDocument->createNode(vtTemp,(BSTR)toWString(Iter->name()).c_str(),L"",&pNewElement);
		if(pElementNode == NULL)
			hr=pIXMLDOMDocument->appendChild(pNewElement,&pRetElement);
		else
			hr=pElementNode->appendChild(pNewElement,&pRetElement);
		if(SUCCEEDED(hr))
		{
			hr=WriteElement(&(*Iter),pIXMLDOMDocument,pRetElement);
			RecursiveSave(&(*Iter),pIXMLDOMDocument,pRetElement);
		}
	}
	return hr;
}

bool MSXMLLibrary::save(const TSTRING& strFileName,XMLMANAGER::XMLDocument* pXMLDoc)
{
	bool bRes = false;

	HRESULT hr = E_FAIL;
	CComPtr<IXMLDOMDocument> pIXMLDOMDocument;
	pIXMLDOMDocument = PrepareDOMDocument(pIXMLDOMDocument);
	if(pIXMLDOMDocument)
	{
		CComPtr<IXMLDOMNode> pRetElement;
		hr = RecursiveSave(pXMLDoc,pIXMLDOMDocument,pRetElement);
		if(SUCCEEDED(hr))
		{
			BSTR text;
			hr = pIXMLDOMDocument->get_text(&text);
			hr = pIXMLDOMDocument->save(_variant_t((BSTR)toWString(strFileName).c_str()));
		}
		if(SUCCEEDED(hr))
			bRes = true;
	}
	return bRes;
}
