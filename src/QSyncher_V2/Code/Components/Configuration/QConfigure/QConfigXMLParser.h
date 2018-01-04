#ifndef _QSYNC_QCONFIGXMLPARSER_H_
#define _QSYNC_QCONFIGXMLPARSER_H_

#pragma once

#include "QMobileConfigure.h"
#include "..\..\..\CommonUtil\XMLManager.h"
#include "..\..\..\CommonUtil\MSXMLImpl.h"

namespace QSYNC_LIB
{

	class CQConfigXMLParser
	{
	public:
		CQConfigXMLParser(void);
		virtual ~CQConfigXMLParser(void);

		bool    LoadQConfigXML(const BSTR strQConfigXML );
		bool    GetMobileSettings( const TSTRING& strMobID, CQSetting& oMobSetting );
		bool    GetSettings( const TSTRING& strAppSetting, CQSetting& oSettings );

	private:		
		CQSetting									m_oMobSetting;
		MSXMLLibrary								m_qXMLlib;
		XMLMANAGER::XMLDocument						m_qXMLdoc;
		bool										m_isConfigLoad;

		XMLMANAGER::XMLElement						m_eleRoot;
		std::map<TSTRING,XMLMANAGER::XMLElement>	m_Elements;

		bool	InternalGetSettings(XMLMANAGER::XMLElement &rElement,const TSTRING& strAppSetting, CQSetting& oSettings );
		void    GetElementSettings( XMLMANAGER::XMLElement &rElement, CQSetting& oSettings );
		bool	GetAttribuate( XMLMANAGER::XMLAllAttribute &rAttribute, TSTRING& strName,TSTRING& strValue,TSTRING& strChild);
	};

}
#endif