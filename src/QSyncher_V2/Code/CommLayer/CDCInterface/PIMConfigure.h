#pragma once

#ifndef _AT_PIMCONFIGURE_H_
#define _AT_PIMCONFIGURE_H_

#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"
#include "..\MobileInterface\Interface_Include\IPIMConfig.h"
#include "Request.h"
#include "..\..\CommonUtil\NotifyBase.h"

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

CDCINTERFACE_CLASS_DLLEXT IPIMCONFIG* CreatePIMConfigure(const TSTRING& strModalName);
CDCINTERFACE_CLASS_DLLEXT void DeletePIMConfigure();

#ifdef _CDCINTERFACEDLL

#include "RequestFunctionBase.h"
#include "PIMConfigureRequest.h"

class CPIMCONFIGURE : public IPIMCONFIG , public RequestFunctionBase
{
public:
	CPIMCONFIGURE(const TSTRING& strModalName)
		:RequestFunctionBase(INFINITE)
		,m_strModalName(strModalName)
		,m_pConfigure(QSYNC_LIB::GetMobileConFig())
	{
		if(m_pConfigure)
			m_pConfigure->LoadDefaultXML();
	}
	virtual ~CPIMCONFIGURE()
	{
		QSYNC_LIB::ReleaseMobileConFig();
	}
	
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut)
	{
		m_nTimeOut = nTimeOut;
	}
	virtual int  STDMETHODCALLTYPE ListPIMGroup(ePIMType eType, vector<CPIMGroupInfo> *vGroupList)
	{
		CRequestPtr ptRequest = new CListPIMGroup(eType,vGroupList,INFINITE);
		return Execute(ptRequest,INFINITE) > 0;	
	}
	virtual int  STDMETHODCALLTYPE SetPIMGroup(ePIMType eType, int nGroupID, CPIMGroupInfo &setGroup)
	{
		assert(0);
		return 0;
	}
	virtual int  STDMETHODCALLTYPE CreatePIMGroup(CPIMGroupInfo &newGroup, int &nNewID)
	{
		assert(0);
		return 0;
	}
	virtual int  STDMETHODCALLTYPE DeletePIMGroup(int nDeleteID)
	{
		assert(0);
		return 0;
	}

	virtual int		STDMETHODCALLTYPE GetProperty(const TSTRING& strGroup,const TSTRING& strName,TSTRING& strValue)
	{
		strValue.clear();		
		
		if(m_pConfigure)
		{
			using namespace QSYNC_LIB;
			CQSetting oMobSetting;
			if(m_pConfigure->GetMobileSettings(m_strModalName, oMobSetting ))
			{
				CQSetting oSettings;
				if(m_pConfigure->GetSettings( strGroup, oSettings))
				{
					strValue = oSettings.Get(strName);
				}
			}
		}
		return !strValue.empty();
	}

	virtual int		STDMETHODCALLTYPE GetIntegerProperty(const TSTRING& strGroup,const TSTRING& strName,int& nValue)
	{
		bool bRes = false;
		TSTRING strValue;
		bRes = GetProperty(strGroup,strName,strValue) != 0;
		if(bRes)
			nValue = ::_ttoi(strValue.c_str());
		return bRes;
	}
private:
	map<TSTRING,TSTRING>			m_mapProperty;
	TSTRING										m_strModalName;
	QSYNC_LIB::CQMobileConfigure*	m_pConfigure;
};

#endif


#endif //_AT_PIMCONFIGURE_H_