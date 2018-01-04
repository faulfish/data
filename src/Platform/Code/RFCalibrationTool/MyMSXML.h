#pragma once
#import "MSXML.TLB" named_guids

class CMyMSXML;

#ifdef  _WIN32
typedef CMyMSXML						XMLClass;
typedef MSXML::IXMLDOMDocumentPtr		XMLDoc;
typedef MSXML::IXMLDOMNodePtr			XMLNode;
typedef MSXML::IXMLDOMNodeListPtr		XMLNodeList;
typedef MSXML::IXMLDOMAttributePtr		XMLAttr;
typedef MSXML::IXMLDOMNamedNodeMapPtr	XMLAttrList;
typedef MSXML::IXMLDOMElementPtr		XMLElement;
#endif

typedef enum
{
	XMLNodeType_Invalid,
	XMLNodeType_Element,
	XMLNodeType_Text,
	XMLNodeType_Attribute,
	XMLNodeType_Others
} XMLNodeType;

class CMyMSXML
{
public:
	INT Load(LPCTSTR filePath);
	INT Save(LPCTSTR filePath=NULL);
	BOOL IsLoad(){ return m_bLoaded; }

	// ---------------   Data Access   ---------------------------------
	XMLNode GetRoot(){ return m_Root; }
	CString GetFilePath(){ return m_sFileName; }
	CString GetErrorMsg(){ return m_sErrorMsg; }

	// ---------------   Node Operation   ------------------------------
	INT GetNodeType(XMLNode node);
	INT GetNodeName(XMLNode node, LPTSTR nodeName, size_t inLength);
	INT GetNodeText(XMLNode node, LPTSTR nodeText, size_t inLength);
	CString GetNodeName(XMLNode node);
	CString GetNodeText(XMLNode node);
	void SetNodeText(XMLNode node, LPCTSTR nodeText);

	BOOL HasChild(XMLNode node);
	XMLNode GetFirstChild(XMLNode node);
	
	/* *****************************************************************
	*  Assume that a node: <backlight_intensity_0 SValue="0" Caption="off"></backlight_intensity_0>
	*  If you want to get the value of SValue (=0), then	
	*     [i] nodeName - node name, ex: backlight_intensity_0
	*	  [i] attrName - attribute name, ex: SValue
	*	  [o] attrValue - attribute value
	*	  [i] inLength - size of attrValue
	***************************************************************** */
	void GetNodeAttrValue(LPCTSTR nodeName, LPCTSTR attrName, LPTSTR attrValue, size_t inLength);
	CString GetNodeAttrValue(XMLNode node, LPCTSTR attrName);
	BOOL SetNodeAttrValue(XMLNode node, LPCTSTR attrName, LPCTSTR attrValue);
	BOOL DeleteNodeAttrs(XMLNode node);

	/* *****************************************************************
	*  Delete all the original attributes and add the new ones 
	*   
	*  Ex: ModifyNodeAttrs(node, 2, _T("AttrName1"), _T("AttrValue1"), 
	*                               _T("AttrName2"), _T("AttrValue2"));
	***************************************************************** */
	BOOL ModifyNodeAttrs(XMLNode node, UINT iAttrNum, ...);

	BOOL AddNodeAttr(XMLNode node, LPCTSTR attrName, LPCTSTR attrValue);

	// ------------   Create/Delete node   -----------------------------
	/* *****************************************************************
	*    [i] path - the path of the expected new added node
	*    [i] iAttrNum - the number of attributes of the new added node
	*    [i] ... - pairs of attribute name and attribute value
	*    Ex:
	*      CreateNode(_T("/AA/BB[@Id=\"IDB\" and @Num=\"01\"]/CC"), _T("Value_C"));
	***************************************************************** */
	INT CreateNode(LPCTSTR path, LPCTSTR nodeText=NULL);
	BOOL DeleteNode(LPCTSTR path);

	// ---------------   Node List Operation   -------------------------
	XMLNodeList GetChildNodes(XMLNode node);
	XMLNodeList GetChildNodes(LPCTSTR path);
	UINT GetNodeListSize(XMLNodeList nodeList);
	XMLNode GetNodeListItem(XMLNodeList nodeList, UINT idx);

	// ---------------   Attribute List Operation   --------------------
	XMLAttrList GetNodeAttrList(XMLNode node);
	UINT GetAttrListSize(XMLAttrList attrList);
	XMLAttr GetAttrListItem(XMLAttrList attrList, INT idx);

	// others	
	XMLNodeList Search(LPCTSTR path);
	XMLNode SearchNode(LPCTSTR path);
	void GetFullPath(LPCTSTR nodeName, LPTSTR path, size_t length);

private:	
	BOOL m_bLoaded;
	CString m_sFileName;
	CString m_sErrorMsg;			

	XMLDoc m_DomDoc;
	XMLElement m_Root;

	enum ErrorMessage{
		ERROR_INVALID_ARG			= 1,
		ERROR_INVALID_XMLFILE		= 2,
		ERROR_CREATE_COM_DOC		= 3,
		ERROR_NODE_TYPE				= 4,
		ERROR_NODE_SAVE_FILE		= 5
	};

	INT GetNodeValue(XMLNodeType attribute, XMLNode node, LPTSTR nodeName, size_t inLength);
	CString GetNodeValue(XMLNodeType attribute, XMLNode node);

public:
	CMyMSXML(void);
	virtual ~CMyMSXML(void);
};
