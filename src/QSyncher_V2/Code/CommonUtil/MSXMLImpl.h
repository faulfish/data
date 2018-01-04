/*********************************************************************************\
  FileName: MSXMLImpl.h

  Descript: A wapper class for Microsoft XML document manager

  Creater: Alan tu

  Create Date:2006/12/15
\*********************************************************************************/

#if !defined(_INC_MSXMLIMPL_)
#define _INC_MSXMLIMPL_

#include "XMLManager.h"

using namespace XMLMANAGER;
struct MSXMLLibrary : public XMLLibraryInterface
{
	virtual bool init();
	virtual bool uninit();
	virtual bool loadXML(const BSTR bstrXML,XMLMANAGER::XMLDocument* pXMLDoc,const BSTR bstrXSLT = NULL);
	virtual bool load(const TSTRING& strFileName,XMLMANAGER::XMLDocument* pXMLDoc);
	virtual bool save(const TSTRING& strFileName,XMLMANAGER::XMLDocument* pXMLDoc);
};

#endif //!defined(_INC_MSXMLIMPL_)