#include "StdAfx.h"
#include "MyMSXML.h"
#include <vector>

using namespace std;
#pragma warning( disable : 4996 )

CMyMSXML::CMyMSXML(void)
{
	m_DomDoc = NULL;
	m_Root = NULL;
	m_bLoaded = FALSE;
	m_sFileName = _T("");
	m_sErrorMsg = _T("");
}

CMyMSXML::~CMyMSXML(void)
{
	if (m_DomDoc != NULL)		m_DomDoc = NULL;
	if (m_Root != NULL)			m_Root = NULL; 
}


INT CMyMSXML::Load(LPCTSTR filePath)
{
	if (filePath == NULL) {
		m_sErrorMsg = _T("ERROR_INVALID_PARAMETER");
		return ERROR_INVALID_PARAMETER;
	}

	// When using an ATL string conversion macro, specify the USES_CONVERSION 
	// macro at the beginning of your function in order to avoid compiler errors
	USES_CONVERSION;

	OleInitialize(NULL);
	if (m_DomDoc == NULL) {
		HRESULT hr = m_DomDoc.CreateInstance(MSXML::CLSID_DOMDocument, NULL, CLSCTX_ALL);//CLSCTX_INPROC_SERVER| CLSCTX_LOCAL_SERVER);

		if (m_DomDoc == NULL){
			m_sErrorMsg = _T("Create IXMLDOMDocument Instance Error");
			return ERROR_CREATE_COM_DOC;
		}
	}

	CComBSTR str = filePath;
	VARIANT_BOOL bResult = m_DomDoc->load(str.m_str);
	str.Empty();	

	if (!bResult) {
		// the xml file format is incorrect		
		MSXML::IXMLDOMParseErrorPtr pParseError = m_DomDoc->GetparseError();
		long dwError = pParseError->GeterrorCode();
		CComBSTR bstrReason = pParseError->Getreason().GetBSTR();

		CString errMsg;
		errMsg.Format(_T("XML Parse Error 0x%x : %s"), dwError, W2T(bstrReason));
		m_sErrorMsg = errMsg;
		bstrReason.Empty();
		str.Empty();
		return ERROR_INVALID_XMLFILE;
	}

	m_bLoaded = TRUE;
	m_sFileName = filePath;
	m_DomDoc->get_documentElement(&m_Root);
	str.Empty();
	OleUninitialize();   

	return ERROR_SUCCESS;
}

INT CMyMSXML::Save(LPCTSTR filePath)
{	
	CString strFilePath = m_sFileName;
	if (filePath != NULL)
		strFilePath = filePath;

	CComBSTR str = strFilePath;
	HRESULT hr = m_DomDoc->save((_variant_t)str);
	if (FAILED(hr))	
		return ERROR_NODE_SAVE_FILE;
	
	return ERROR_SUCCESS;
}

INT CMyMSXML::GetNodeType(XMLNode node)
{
	if (node == NULL){
		m_sErrorMsg = _T("ERROR_INVALID_PARAMETER");
		return ERROR_INVALID_PARAMETER;
	}

	MSXML::DOMNodeType nodeType;

	node->get_nodeType(&nodeType);

	switch(nodeType){
		case MSXML::NODE_ELEMENT:
			return XMLNodeType_Element;

		case MSXML::NODE_TEXT:
			return XMLNodeType_Text;

		case MSXML::NODE_INVALID:
			return XMLNodeType_Invalid;
		case MSXML::NODE_ATTRIBUTE:
			return XMLNodeType_Attribute;
		case MSXML::NODE_CDATA_SECTION:	
		case MSXML::NODE_ENTITY_REFERENCE:	
		case MSXML::NODE_ENTITY:		
		case MSXML::NODE_PROCESSING_INSTRUCTION:		
		case MSXML::NODE_COMMENT:		
		case MSXML::NODE_DOCUMENT:		
		case MSXML::NODE_DOCUMENT_TYPE:		
		case MSXML::NODE_DOCUMENT_FRAGMENT:		
		case MSXML::NODE_NOTATION:
			return XMLNodeType_Others;
		default:
			m_sErrorMsg = _T("ERROR_NODE_TYPE");
			return ERROR_NODE_TYPE;
	}

	return ERROR_SUCCESS;
}

INT CMyMSXML::GetNodeValue(XMLNodeType attribute, XMLNode node, LPTSTR nodeName, size_t inLength)
{
	if (node == NULL){
		m_sErrorMsg = _T("ERROR_INVALID_PARAMETER");
		return ERROR_INVALID_PARAMETER;
	}

	USES_CONVERSION;

	CComBSTR str;

	switch (attribute){
		case XMLNodeType_Element:
		case XMLNodeType_Others:
			node->get_nodeName(&str);
			break;
		case XMLNodeType_Text:
			node->get_text(&str);
			break;
		case XMLNodeType_Attribute:
			node->get_text(&str);
			break;
		default:
			ASSERT(0);
	}

	size_t txtlen = _tcslen(W2T(str));	

	if (inLength == 0){
		// returns the number of characters, not including the terminating 
		// null character
		str.Empty();
		return (int)txtlen;
	}

	if (nodeName == NULL){
		m_sErrorMsg = _T("ERROR_INVALID_PARAMETER");
		return ERROR_INVALID_PARAMETER;
	}

	if (txtlen == 0){
		str.Empty();
		nodeName = _T("");
		return 0;
	}

	size_t wrtlen = 0;

	if (txtlen >= inLength){
		wrtlen = inLength-1;
		_tcsncpy(nodeName, W2T(str), wrtlen);
		nodeName[wrtlen] = _T('\0');
	}
	else{		
		_tcsncpy(nodeName, W2T(str), inLength);
		wrtlen = txtlen;
	}

	str.Empty();

	return (int)wrtlen;
}

CString CMyMSXML::GetNodeValue(XMLNodeType attribute, XMLNode node)
{
	if (node == NULL)
		return _T("");

	USES_CONVERSION;

	CComBSTR str;

	switch (attribute){
		case XMLNodeType_Element:
		case XMLNodeType_Others:
			node->get_nodeName(&str);
			break;
		case XMLNodeType_Text:
			node->get_text(&str);
			break;
		case XMLNodeType_Attribute:
			node->get_text(&str);
			break;
		default:
			ASSERT(0);
	}

	CString retValue;

#ifdef _UNICODE
	retValue = str.m_str;
#else
	size_t szData = WideCharToMultiByte(CP_ACP, 0, str.m_str, -1, NULL, NULL, NULL, NULL);
	char *data = new char[szData];
	memset(data, 0, sizeof(char)*szData);
	WideCharToMultiByte(CP_ACP, 0, str.m_str, -1, data, szData, NULL, NULL);

	retValue = data;
	delete []data;
#endif
	str.Empty();

	return retValue;
}

INT CMyMSXML::GetNodeName(XMLNode node, LPTSTR nodeName, size_t inLength)
{
	return GetNodeValue(XMLNodeType_Element, node, nodeName, inLength);
}

CString CMyMSXML::GetNodeName(XMLNode node)
{
	return GetNodeValue(XMLNodeType_Element, node);
}

INT CMyMSXML::GetNodeText(XMLNode node, LPTSTR nodeText, size_t inLength)
{
	return GetNodeValue(XMLNodeType_Text, node, nodeText, inLength);
}

CString CMyMSXML::GetNodeText(XMLNode node)
{
	return GetNodeValue(XMLNodeType_Text, node);
}

void CMyMSXML::SetNodeText(XMLNode node, LPCTSTR nodeText)
{
	if (node == NULL)
		return;

	CComBSTR str = nodeText;
	node->put_text(str);
	str.Empty();
}

BOOL CMyMSXML::CreateNode(LPCTSTR path, LPCTSTR nodeText)
{
	CString dupStr = path;	
	while (1) {
		int nPre = dupStr.Find(_T('['));
		if (nPre == -1)
			break;

		int nPost = dupStr.Find(_T(']'), nPre);
		ASSERT (nPost != -1);
		if (nPost == -1)
			nPost = dupStr.GetLength()-1;

		for (int i = nPre; i <= nPost; i++)
			dupStr.SetAt(i, _T(' '));
	}

	CString elementName = path;
	CString treePath = path;
	if (dupStr.ReverseFind(_T('/')) != -1) {
		elementName = elementName.Right(dupStr.GetLength()-dupStr.ReverseFind(_T('/'))-1);
		treePath = treePath.Left(dupStr.ReverseFind(_T('/')));
	}
	else
		treePath = _T("");

	// make sure the full path is created already
	XMLNode node = NULL;
	if (treePath != _T("")){	
		CComBSTR str = treePath;
		node = m_DomDoc->selectSingleNode(str.m_str);		
		if (node == NULL){
			CreateNode(treePath);
			node = m_DomDoc->selectSingleNode(str.m_str);
		}
		str.Empty();
	}

	// Get Attributes
	vector<CString> attrName, attrValue;
	if (elementName.ReverseFind(_T('[')) != -1){
		CString strAttrs = elementName.Right(elementName.GetLength()-elementName.Find(_T('['))-1);
		elementName = elementName.Left(elementName.Find(_T('[')));
		strAttrs.Delete(strAttrs.ReverseFind(_T(']')), 1);

		int iStrAttrStart = strAttrs.Find(_T('@'));
		if (iStrAttrStart == -1)
			return ERROR_INVALID_ARG;
		
		while (TRUE){
			int iNextAttrStart = strAttrs.Find(_T('@'), iStrAttrStart+2);
			bool bFinish = (iNextAttrStart == -1);
			
			CString strOneAttr = strAttrs.Mid(iStrAttrStart, strAttrs.GetLength());
			if (!bFinish)
				strOneAttr = strAttrs.Mid(iStrAttrStart, iNextAttrStart-iStrAttrStart);
			CString strAttrName = strOneAttr.Mid(1, strOneAttr.Find(_T('='))-1);
			CString strAttrValue = strOneAttr.Mid(strOneAttr.Find(_T('"'))+1, strOneAttr.ReverseFind(_T('"'))-strOneAttr.Find(_T('"'))-1);

			attrName.push_back(strAttrName);
			attrValue.push_back(strAttrValue);
			if (bFinish)
				break;

			iStrAttrStart = iNextAttrStart;
		}
	}
	
	CComBSTR bstrName = elementName;
	XMLElement newElem = m_DomDoc->createElement(bstrName.m_str);
	bstrName.Empty();	

	if (newElem == NULL)
		return FALSE;

	// set node value
	if (nodeText != NULL){
		CComBSTR str = nodeText;
		newElem->put_text(str);
		str.Empty();
	}

	node->appendChild(newElem);
	if (attrName.size() == 0)
		return TRUE;

	// create attributes
	MSXML::IXMLDOMNamedNodeMapPtr pAttrList = newElem->Getattributes();
	if (NULL == pAttrList)
		return FALSE;
	
	for (UINT i = 0; i < attrName.size(); i++){
		CComBSTR bstrAttrName = attrName.at(i);
		CComBSTR bstrAttrValue = attrValue.at(i);
		MSXML::IXMLDOMAttributePtr pNewAttr = m_DomDoc->createAttribute(bstrAttrName.m_str);		
		pNewAttr->PutnodeValue((_variant_t)bstrAttrValue);
		pAttrList->setNamedItem(pNewAttr);

		bstrAttrName.Empty();
		bstrAttrValue.Empty();
	}

	return TRUE;
}

BOOL CMyMSXML::DeleteNode(LPCTSTR path)
{
	// void CXmltreeDlg::OnTreepopupDelete() 
	CComBSTR str = path;
	XMLNode node = m_DomDoc->selectSingleNode(str.m_str);
	str.Empty();

	if (node == NULL)
		return TRUE; // because the node is really not existing...

	MSXML::DOMNodeType nodeType;
	node->get_nodeType(&nodeType);

	if (MSXML::NODE_DOCUMENT == nodeType ||	MSXML::NODE_DOCUMENT_FRAGMENT == nodeType)
		return FALSE; // IGNORE: Can't delete these items
	else{
		XMLNode pParentNode = node->GetparentNode();
		pParentNode->removeChild(node);
	}

	return TRUE;
}

void CMyMSXML::GetNodeAttrValue(LPCTSTR nodeName, LPCTSTR attrName, LPTSTR attrValue, size_t inLength)
{
	attrValue[0] = _T('\0');

	if (nodeName == NULL || nodeName[0] == _T('\0') || attrName == NULL || attrName[0] == _T('\0') || attrValue == NULL || inLength < 0)
		return;

	CString result = nodeName;
	result = _T("//") + result;

	XMLNodeList nodeList;
	CComBSTR str = result;

	nodeList = m_DomDoc->selectNodes(str.m_str);	
	if (GetNodeListSize(nodeList) == 0){		
		str.Empty();
		return;
	}

	str = attrName;	

	MSXML::IXMLDOMNamedNodeMapPtr attr = NULL;
	XMLNode node = NULL, attrItem = NULL;
	TCHAR strName[1024] = _T("");

	node = GetNodeListItem(nodeList, 0);

	// get the attributes of the node
	attr = node->Getattributes();
	if (attr == NULL){
		str.Empty();
		return;
	}

	// get the certain attribute
	attrItem = attr->getNamedItem(str.m_str);
	if (attrItem == NULL){
		str.Empty();
		return;
	}

	// get the value of the specified attribute
	GetNodeValue(XMLNodeType_Attribute, attrItem, strName, 1024);

	str.Empty();

	_tcsncpy(attrValue, strName, inLength);
}

CString CMyMSXML::GetNodeAttrValue(XMLNode node, LPCTSTR attrName)
{
	if (node == NULL || attrName == NULL)
		return _T("");

	MSXML::IXMLDOMNamedNodeMapPtr attr = NULL;

	// get the attributes of the node
	attr = node->Getattributes();
	if (attr == NULL)
		return _T("");

	// get the certain attribute
	CComBSTR str = attrName;
	XMLNode attrItem = NULL;
	attrItem = attr->getNamedItem(str.m_str);
	if (attrItem == NULL)
		return _T("");

	// get the value of the specified attribute
	CString retValue = GetNodeValue(XMLNodeType_Attribute, attrItem);

	str.Empty();

	return retValue;
}

BOOL CMyMSXML::SetNodeAttrValue(XMLNode node, LPCTSTR attrName, LPCTSTR attrValue)
{
	if (node == NULL || attrName == NULL || attrValue == NULL)
		return FALSE;

	MSXML::IXMLDOMNamedNodeMapPtr attr = NULL;

	// get the attributes of the node
	attr = node->Getattributes();
	if (attr == NULL)
		return FALSE;

	// get the certain attribute
	CComBSTR strAttrName = attrName;
	XMLNode attrItem = NULL;
	attrItem = attr->getNamedItem(strAttrName.m_str);
	if (attrItem == NULL){
		strAttrName.Empty();
		return FALSE;
	}

	// get the value of the specified attribute
	CComBSTR strAttrValue = attrValue;
	attrItem->put_text(strAttrValue.m_str);

	strAttrName.Empty();
	strAttrValue.Empty();

	return TRUE;
}

BOOL CMyMSXML::DeleteNodeAttrs(XMLNode node)
{
	if (node == NULL)
		return FALSE;

	XMLAttrList attrList = GetNodeAttrList(node);
	if (attrList == NULL)
		return TRUE;

	int iListSize = GetAttrListSize(attrList);
	for (int i = iListSize-1; i >= 0; i--) {
		XMLAttr attr = GetAttrListItem(attrList, i);
		CString strAttrName = GetNodeName(attr);
		CComBSTR bstrAttrName = strAttrName;
		attrList->removeNamedItem(bstrAttrName.m_str);
		bstrAttrName.Empty();
	}	
	return TRUE;
}

BOOL CMyMSXML::ModifyNodeAttrs(XMLNode node, UINT iAttrNum, ...)
{
	if (node == NULL)
		return FALSE;

	if (!DeleteNodeAttrs(node))
		return FALSE;

	XMLAttrList attrList = node->Getattributes();

	va_list marker;
	va_start(marker, iAttrNum);	
	for (UINT i = 0; i < iAttrNum; i++){
		CString strAttrName = va_arg(marker, TCHAR*);
		if (strAttrName.IsEmpty()){
			va_end(marker);
			return FALSE;
		}

		CString strAttrValue = va_arg(marker, TCHAR*);
		if (strAttrValue.IsEmpty()){
			va_end(marker);
			return FALSE;
		}		

		CComBSTR bstrAttrName = strAttrName;
		CComBSTR bstrAttrValue = strAttrValue;
		XMLAttr newAttr = m_DomDoc->createAttribute(bstrAttrName.m_str);		
		newAttr->PutnodeValue((_variant_t)bstrAttrValue);
		attrList->setNamedItem(newAttr);

		bstrAttrName.Empty();
		bstrAttrValue.Empty();
	}

	va_end(marker);
	return TRUE;
}

BOOL CMyMSXML::AddNodeAttr(XMLNode node, LPCTSTR attrName, LPCTSTR attrValue)
{
	if (node == NULL || attrName == NULL)
		return FALSE;

	XMLAttrList attrList = node->Getattributes();

	CComBSTR bstrAttrName = attrName;	
	XMLAttr newAttr = m_DomDoc->createAttribute(bstrAttrName.m_str);

	if (attrValue != NULL) {
		CComBSTR bstrAttrValue = attrValue;
		newAttr->PutnodeValue((_variant_t)bstrAttrValue);
		bstrAttrValue.Empty();
	}

	attrList->setNamedItem(newAttr);
	bstrAttrName.Empty();	

	return TRUE;
}

BOOL CMyMSXML::HasChild(XMLNode node)
{
	if (node == NULL)
		return FALSE;

	XMLNodeList nodeList = GetChildNodes(node);

	for (int i = 0; i < nodeList->Getlength(); i++) {
		XMLNode node;
		nodeList->get_item(i, &node);

		MSXML::DOMNodeType nodeType;
		
		node->get_nodeType(&nodeType);
		if (nodeType == NODE_ELEMENT)
			return TRUE;
	}
	
	return FALSE;
}

XMLNodeList CMyMSXML::GetChildNodes(XMLNode node)
{	
	if (node == NULL)
		return NULL;

	XMLNodeList nodeList;	
	node->get_childNodes(&nodeList);
	return nodeList;
}

XMLNodeList CMyMSXML::GetChildNodes(LPCTSTR path)
{
	CComBSTR str = path;
	XMLNode node = m_DomDoc->selectSingleNode(str.m_str);
	str.Empty();

	XMLNodeList nodeList;
	if (node == NULL)
		return nodeList;
	node->get_childNodes(&nodeList);

	return nodeList;
}

XMLNode CMyMSXML::GetFirstChild(XMLNode node)
{
	if (node == NULL)
		return NULL;
	return node->GetfirstChild();
}

XMLAttrList CMyMSXML::GetNodeAttrList(XMLNode node)
{
	if (node == NULL)
		return NULL;

	XMLAttrList attrList = node->Getattributes();
	return attrList;
}

UINT CMyMSXML::GetAttrListSize(XMLAttrList attrList)
{
	if (attrList == NULL)
		return 0;
	
	return attrList->Getlength();
}

XMLAttr CMyMSXML::GetAttrListItem(XMLAttrList attrList, INT idx)
{
	if (attrList == NULL)
		return NULL;

	XMLAttr attr = attrList->Getitem(idx);
	return attr;
}

UINT CMyMSXML::GetNodeListSize(XMLNodeList nodeList)
{
	if (nodeList == NULL)
		return 0;
	return nodeList->Getlength();
}

XMLNode CMyMSXML::GetNodeListItem(XMLNodeList nodeList, UINT idx)
{
	XMLNode node;
	nodeList->get_item(idx, &node);
	return node;
}

XMLNodeList CMyMSXML::Search(LPCTSTR path)
{
	if (path == NULL || m_DomDoc == NULL)
		return NULL;

	XMLNodeList nodeList;
	CComBSTR str = path;

	nodeList = m_DomDoc->selectNodes(str.m_str);
	str.Empty();
	return nodeList;
}

XMLNode CMyMSXML::SearchNode(LPCTSTR path)
{
	if (path == NULL || m_DomDoc == NULL)
		return NULL;

	XMLNodeList nodeList;
	CComBSTR str = path;

	nodeList = m_DomDoc->selectNodes(str.m_str);
	str.Empty();
	
	if (GetNodeListSize(nodeList) >= 1)
		return GetNodeListItem(nodeList, 0); // return the first node in the list
	else
		return NULL;
}

void CMyMSXML::GetFullPath(LPCTSTR nodeName, LPTSTR path, size_t length)
{
	if (nodeName == NULL || path == NULL)
		return;

	CString result = nodeName;
	result = _T("//") + result;

	XMLNodeList nodeList;
	CComBSTR str = result;

	nodeList = m_DomDoc->selectNodes(str.m_str);	
	if (GetNodeListSize(nodeList) == 0){
		path[0] = _T('\0');
		str.Empty();
		return;
	}
	
	str = _T("Caption");
	result = nodeName;

	MSXML::IXMLDOMNamedNodeMapPtr attr = NULL;
	XMLNode node = NULL, nodeCaption = NULL;
	TCHAR strName[1024] = _T("");

	node = GetNodeListItem(nodeList, 0);
	attr = node->Getattributes();
	if (attr == NULL)
		return;

	nodeCaption = attr->getNamedItem(str.m_str);
	if (nodeCaption == NULL)
		return;

	GetNodeValue(XMLNodeType_Attribute, nodeCaption, strName, 512);
	result = strName;
	
	node = node->GetparentNode();

	while (GetNodeType(node) != XMLNodeType_Invalid){
		 attr = node->Getattributes();
		 if (attr == NULL)	
			 break;

		 nodeCaption = attr->getNamedItem(str.m_str);
		 if (nodeCaption == NULL)
			 break;

		 GetNodeValue(XMLNodeType_Attribute, nodeCaption, strName, 512);
		 result = _T(".") + result;
		 result = strName + result;
		 node = node->GetparentNode();
	}

	str.Empty();

	_tcsncpy(path, result, length);

	return;
}