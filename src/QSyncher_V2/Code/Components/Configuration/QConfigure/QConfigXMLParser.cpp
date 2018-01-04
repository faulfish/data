#include "stdafx.h"
#include "QConfigXMLParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace QSYNC_LIB;

CQConfigXMLParser::CQConfigXMLParser(void)
:m_qXMLdoc(&m_qXMLlib),
m_isConfigLoad(false)
{
}

CQConfigXMLParser::~CQConfigXMLParser(void)
{
}

bool CQConfigXMLParser::LoadQConfigXML(const BSTR strQConfigXML)
{
	m_qXMLdoc.getAllElement()->clear();
	m_isConfigLoad = m_qXMLdoc.loadXML(strQConfigXML);

	size_t nElementSize;

	nElementSize = m_qXMLdoc.getAllElement()->size();

	if ( m_isConfigLoad && nElementSize == 1 ) 
	{
		XMLAllElement AllElement = m_qXMLdoc.getAllElement()[0];
		m_eleRoot = AllElement[0];

		AllElement = m_eleRoot.getAllElement()[0];

		for( size_t i=0; i<m_eleRoot.getAllElement()->size(); i++ )
			m_Elements.insert(std::pair<TSTRING,XMLMANAGER::XMLElement>(AllElement[i].name(),AllElement[i]));

	}
	else m_isConfigLoad = false;

	return m_isConfigLoad;
}

bool CQConfigXMLParser::GetMobileSettings( const TSTRING& strMobID, CQSetting& oMobSetting )
{
	/*
	strMobID is _T("E81")
	This content of m_Elements[_T("MobilePhones")]
		<MobilePhones>
			<MobilePhone ID="E81" Description="BenQ E81">
	     		<Settings ChildID = "" 					ID="Applications" Description="Support Applications"   Value="AppDefault"/>
	     		<Settings ChildID = "" 					ID="Devices"      Description="Device Setting"         Value="CDCDev"      DevType="Only"/>	     
	     		<Settings ChildID = "Devices" 	ID="Interfaces"   Description="Interface Setting"      Value="MIXInf"/>
	     		<Settings ChildID = "Devices" 	ID="PIMDevices"   Description="PIM Devices Setting"    Value="ATGROUP"/>
			</MobilePhone>
		</MobilePhones>
	*/
	bool bResult = true;
	bool bMobileFound = false;

	if( m_isConfigLoad == false )
		bResult = false;

	if(bResult)
	{
		XMLAllElement   AllElement = m_Elements[_T("MobilePhones")].getAllElement()[0];
		XMLElement      SettingElement;
		XMLAttribute    SettingAttribute;

		for (UINT i=0; i<AllElement.size(); i++)
		{
			XMLElement MobileElement = AllElement[i];

			if ( MobileElement.name() == _T("MobilePhone") )
			{
				XMLAllAttribute AllAttribute = MobileElement.getAllAttribute()[0];

				TSTRING strName,strValue,strChild;
				if(GetAttribuate(AllAttribute,strName,strValue,strChild))
				{
					if(strName == strMobID)
					{
						bMobileFound = true;
						XMLAllElement   SettingsElement = MobileElement.getAllElement()[0];
						for(UINT j=0; j<SettingsElement.size(); j++)
						{
							XMLElement SettingElement = SettingsElement[j];
							XMLAllAttribute attrs = SettingElement.getAllAttribute()[0];

							TSTRING strName,strValue,strChild;
							if(GetAttribuate(attrs,strName,strValue,strChild))
								m_oMobSetting.Add(strName,strValue,strChild);
						}
						oMobSetting = m_oMobSetting;
						break;
					}
				}				
			}
		}
	}//end of if bResult

	return (bResult && bMobileFound);
}

bool CQConfigXMLParser::InternalGetSettings(XMLMANAGER::XMLElement &rElement,const TSTRING& strAppSetting, CQSetting& oSettings )
{
	/*
	This content of rElement
	<Applications>
       <AppDefault>
           <Settings		ID="Phonebook" 			Value="PhonebookV2" 		Description="Phonebook Application" />
           <Settings 		ID="Message" 			Value="MessageV2" 			Description="Message Application"/>
           <Settings 		ID="Calendar" 			Value="CalendarV2" 			Description="Calendar Application"/>
           <Settings 		ID="PIMSync" 			Value="OutlookSync" 		Description="PIMSync Application"/>
           <Settings 		ID="FileManager" 		Value="FileManagerV2" 		Description="FileManager Application"/>
           <Settings 		ID="BackupRestore" 		Value="BackupRestoreV2"		Description="BackupRestore Application"/>
           <Settings 		ID="Options" 			Value="Options" 			Description="Options Application"/>
       </AppDefault>
	</Applications>
	*/
	bool bRes = false;
	TSTRING strValue = m_oMobSetting.Get(strAppSetting);//strAppSetting is _T("Applications") for this sample
	//strValue maybe is _T("AppDefault") for this sample

	XMLAllElement   SpecialElement = rElement.getAllElement()[0];
	for(UINT i=0; i<SpecialElement.size(); i++)
	{
		XMLElement SubSettingElement = SpecialElement[i];
		if(SubSettingElement.name() == strValue)
		{
			bRes = true;
			GetElementSettings(SubSettingElement,oSettings);
			break;
		}
	}
	return bRes;
}

bool CQConfigXMLParser::GetSettings( const TSTRING& strAppSetting, CQSetting& oSettings )
{
	bool bRes = false;
	TSTRING strChild = m_oMobSetting.GetChild(strAppSetting);	

	TSTRING	strIndex = strChild.empty() ? strAppSetting : strChild;
	TSTRING strValue = m_oMobSetting.Get(strIndex);

	XMLAllElement   SpecialElement = m_Elements[strIndex].getAllElement()[0];
	for(UINT j=0; j<SpecialElement.size(); j++)
	{
		XMLElement SettingElement = SpecialElement[j];	

		if(SettingElement.name() == strValue)
		{
			if(strChild.empty())
			{
				GetElementSettings(SettingElement,oSettings);
				bRes = true;
			}
			else
			{
				XMLAllElement   SubSpecialElement = SettingElement.getAllElement()[0];
				for(UINT k=0; k<SubSpecialElement.size(); k++)
				{
					XMLElement SubSettingElement = SubSpecialElement[k];
					if(SubSettingElement.name() == strAppSetting)
					{						
						bRes = InternalGetSettings(SubSettingElement,strAppSetting,oSettings);
						break;
					}
				}
			}
			break;
		}
	}
	
	return bRes;
}

bool CQConfigXMLParser::GetAttribuate(XMLMANAGER::XMLAllAttribute &rAttribute,TSTRING& strName,TSTRING& strValue,TSTRING& strChild)
{
	/*
	This content of rAttribute
	<Settings		ID="Phonebook" 			Value="PhonebookV2" 		Description="Phonebook Application" />
	*/
	bool bFineName = false;
	for (UINT n=0; n<rAttribute.size(); n++)
	{
		XMLAttribute SettingAttribute = rAttribute[n]; 

		if ( SettingAttribute.name() == _T("ID"))
		{
			bFineName = true;
			strName = SettingAttribute.value();
		}	
		else if ( SettingAttribute.name() == _T("Value"))
		{
			strValue = SettingAttribute.value();
		}	
		else if ( SettingAttribute.name() == _T("ChildID"))
		{
			strChild = SettingAttribute.value();
		}
	}
	return bFineName;
}

void CQConfigXMLParser::GetElementSettings( XMLMANAGER::XMLElement &rElement, CQSetting& oSettings )
{	
	/*
	This content of rElement
       <AppDefault>
           <Settings		ID="Phonebook" 			Value="PhonebookV2" 		Description="Phonebook Application" />
           <Settings 		ID="Message" 			Value="MessageV2" 			Description="Message Application"/>
           <Settings 		ID="Calendar" 			Value="CalendarV2" 			Description="Calendar Application"/>
           <Settings 		ID="PIMSync" 			Value="OutlookSync" 		Description="PIMSync Application"/>
           <Settings 		ID="FileManager" 		Value="FileManagerV2" 		Description="FileManager Application"/>
           <Settings 		ID="BackupRestore" 		Value="BackupRestoreV2"		Description="BackupRestore Application"/>
           <Settings 		ID="Options" 			Value="Options" 			Description="Options Application"/>
       </AppDefault>
	*/

	XMLAllElement   AllElement = rElement.getAllElement()[0];
	for (UINT m=0; m < AllElement.size(); m++) //Loop Settings
	{
		XMLElement SettingElement = AllElement[m];
		if(SettingElement.name() == _T("Settings"))
		{
			XMLAllAttribute AllAttribute = SettingElement.getAllAttribute()[0];

			TSTRING strName,strValue,strChild;
			if(GetAttribuate(AllAttribute,strName,strValue,strChild))
				oSettings.Add(strName,strValue,strChild);
		}

	}//end of loop Setting
}
