/*********************************************************************************\
  FileName: XMLManager.h

  Descript: A wapper class for XML document manager

  Creater: Alan tu
\*********************************************************************************/

#if !defined(_INC_XMLMANAGER_)
#define _INC_XMLMANAGER_

#include <vector>
#include <stdlib.h>
#include "PIMStringUtility.h"

namespace XMLMANAGER
{
	using namespace std;
	/*
	
	XML Manager architecture

	XMLDocument(::XMLElement::XMLNode)
			|__________Element(::XMLNode)
			|               |
			|				|__________Element
			|               |
			|				|__________Attribute(::XMLNode)
			|               |
			|				|__________Attribute
			|
			|__________Attribute
			|
			|__________Attribute
			|
			|__________Element
			|
			|__________Element
			|
			|__________Element
			                 |
							 |__________Attribute
							 
	*/

	class XMLAttribute;	
	class XMLElement;
	class XMLDocument;

	typedef vector<XMLElement> XMLAllElement;
	typedef vector<XMLAttribute> XMLAllAttribute;

	class XMLNode
	{
	public:
		XMLNode(){}
		XMLNode(const TSTRING& strName){m_strName = strName;}
		const TSTRING& name() const {return m_strName;}
		const TSTRING& value() const {return m_strValue;}
		void setname(const TSTRING& strName){m_strName = strName;}
		void setvalue(const TSTRING& strValue){m_strValue = strValue;}
		void setNameAndValue(const TSTRING& strName, const TSTRING& strValue)
		{ m_strName = strName;m_strValue = strValue;}
	private:
		TSTRING m_strName;
		TSTRING m_strValue;
	};

	class XMLAttribute : public XMLNode
	{
	public:
		XMLAttribute(){}
		XMLAttribute(const TSTRING& strName):XMLNode(strName){}
		//Not use now...
		enum TYPE
		{
			TYPE_STRING,
			TYPE_NUMBER,
			TYPE_DATETIME
		};
	private:
		TYPE m_Type;
	};
	
	class XMLElement : public XMLNode
	{
	public:
		XMLElement(){}
		XMLElement(const TSTRING& strName):XMLNode(strName){}
		const size_t getAttributeCount() const {return m_Attributes.size();}
		XMLAllElement* getAllElement(){return &m_Elements;}
		const XMLAllElement* getAllElement() const {return &m_Elements;}
		XMLAllAttribute* getAllAttribute(){return &m_Attributes;}
		bool ReplaceAttr(const TSTRING& strName,const TSTRING& strValue)
		{
			bool bRes = false;
			for(XMLAllAttribute::iterator Iter = m_Attributes.begin();Iter != m_Attributes.end();Iter++)
			{
				if(_tcscmp(Iter->name().c_str(),strName.c_str()) == 0)
				{
					Iter->setvalue(strValue);
					bRes = true;
					break;
				}
			}
			if(!bRes)
				bRes = addAttribute(strName, strValue);
			return bRes;
		}
		bool ReplaceAttr(const TSTRING& strName,int nValue)
		{
			bool bRes = false;
			TSTRING strValue;
			QSYNC_LIB::iToTStr<TSTRING>(nValue, strValue);
			ReplaceAttr(strName, strValue);
			return bRes;
		}
		bool getAttr(const TSTRING& strName,TSTRING& strValue)
		{
			bool bRes = false;
			strValue = _T("");
			for(XMLAllAttribute::iterator Iter = m_Attributes.begin();Iter != m_Attributes.end();Iter++)
			{
				if(_tcscmp(Iter->name().c_str(),strName.c_str()) == 0)
				{
					strValue = Iter->value();
					bRes = true;
					break;
				}
			}
			return bRes;
		}
		bool getAttr(const TSTRING& strName,TSTRING& strValue) const
		{
			bool bRes = false;
			strValue = _T("");
			for(XMLAllAttribute::const_iterator Iter = m_Attributes.begin();Iter != m_Attributes.end();Iter++)
			{
				if(_tcscmp(Iter->name().c_str(),strName.c_str()) == 0)
				{
					strValue = Iter->value();
					bRes = true;
					break;
				}
			}
			return bRes;
		}
		int getAttr(const TSTRING& strName) const
		{
			int nRes = -1;
			TSTRING strValue;
			for(XMLAllAttribute::const_iterator Iter = m_Attributes.begin();Iter != m_Attributes.end();Iter++)
			{
				if(_tcscmp(Iter->name().c_str(),strName.c_str()) == 0)
				{
					strValue = Iter->value();
					nRes = _ttoi(strValue.c_str());
					break;
				}
			}
			return nRes;
		}
		bool addAttribute(const TSTRING& strName, const TSTRING& strValue)
		{
			bool bRes = true;
			XMLAttribute attr;
			attr.setNameAndValue(strName, strValue);
			m_Attributes.push_back(attr);
			return bRes;
		}
		bool addAttribute(const TSTRING& strName, int nValue)
		{
			bool bRes = true;
			TSTRING strValue;
			bRes = QSYNC_LIB::iToTStr<TSTRING>(nValue, strValue);
			XMLAttribute attr;
			attr.setNameAndValue(strName, strValue);
			m_Attributes.push_back(attr);
			return bRes;
		}

		bool addAttribute(const TSTRING& strName, double nValue)
		{
			bool bRes = true;
			TSTRING strValue;
			
			TCHAR cpValue[50];
			_stprintf_s(cpValue, 50, _T("%f"), nValue);

			strValue = cpValue;

			XMLAttribute attr;
			attr.setNameAndValue(strName, strValue);
			m_Attributes.push_back(attr);
			return bRes;
		}
	private:
		XMLAllAttribute m_Attributes;
		XMLAllElement m_Elements;
	};

	struct XMLLibraryInterface
	{//for fit third party library
		virtual bool init(){return true;}//not essential
		virtual bool uninit(){return true;}//not essential
		virtual bool load(const TSTRING& strFileName,XMLMANAGER::XMLDocument* pXMLDoc) = 0;
		virtual bool loadXML(const BSTR bstrXML,XMLMANAGER::XMLDocument* pXMLDoc,const BSTR bstrXSLT = NULL) = 0;
		virtual bool save(const TSTRING& strFileName,XMLMANAGER::XMLDocument* pXMLDoc) = 0;
	};
	
	class XMLDocument : public XMLElement
	{
	public:
		XMLDocument(){}
		XMLDocument(const TSTRING& strName):XMLElement(strName){}
	public:
		XMLDocument(XMLLibraryInterface *pXMLLibraryInterface):m_pXMLLibraryInterface(pXMLLibraryInterface){m_bXMLLibraryReady = m_pXMLLibraryInterface->init();}
		virtual ~XMLDocument(){m_pXMLLibraryInterface->uninit();}
		bool loadXML(const BSTR bstrXML,const BSTR bstrXSLT = NULL){return m_bXMLLibraryReady && m_pXMLLibraryInterface->loadXML(bstrXML,this,bstrXSLT);}
		bool load(const TSTRING& strFileName){return m_bXMLLibraryReady && m_pXMLLibraryInterface->load(strFileName,this);}
		bool save(const TSTRING& strFileName){return m_bXMLLibraryReady && m_pXMLLibraryInterface->save(strFileName,this);}
	private:
		bool m_bXMLLibraryReady;
		XMLLibraryInterface *m_pXMLLibraryInterface;
	};
}

#endif // !defined(_INC_XMLMANAGER_)