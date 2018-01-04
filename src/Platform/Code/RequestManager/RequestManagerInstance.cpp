#include "StdAfx.h"
#include "RequestManagerInstance.h"

namespace PLATFORM_TOOL
{
	CRequestManagerInstance::CRequestManagerInstance(int nPort,CRequestManager*	pCRequestManager,const std::string& strPlatform)
		:m_nPort(nPort)
		,m_strPlatform(strPlatform)
		,m_pCRequestManager(pCRequestManager)
	{
		if(m_pCRequestManager)
			m_pCRequestManager->AddRef(m_nPort);
	}

	CRequestManagerInstance::~CRequestManagerInstance(void)
	{
		if(m_pCRequestManager)
			m_pCRequestManager->ReleaseRef(m_nPort);
	}
	
	bool CRequestManagerInstance::Open()
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->Open(m_nPort);
		return bRes;
	}

	bool CRequestManagerInstance::Close(bool bKeepPotocolType)
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->Close(m_nPort,bKeepPotocolType);
		return bRes;
	}

	bool CRequestManagerInstance::IsOpen() const
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->IsOpen(m_nPort);
		return bRes;
	}

	bool CRequestManagerInstance::Execuate(CRequestPtr& prREquest,const std::string& strPlatform)
	{
		bool bRes = false;
		if(m_pCRequestManager)
			bRes = m_pCRequestManager->Execuate(m_nPort,prREquest,strPlatform);
		return bRes;
	}

	int CRequestManagerInstance::GetComport() const
	{
		return m_nPort;
	}

	void CRequestManagerInstance::Reset()
	{
		if(m_pCRequestManager)
			m_pCRequestManager->Reset(GetComport());
	}
	void CRequestManagerInstance::ResetMode()
	{
		if(m_pCRequestManager)
			m_pCRequestManager->ResetMode(GetComport());
	}

	PotocolType CRequestManagerInstance::GetPotocolType()
	{
		PotocolType type = NONE;
		if(m_pCRequestManager)
			type = m_pCRequestManager->GetPotocolType(m_nPort);
		return type;
	}

	void CRequestManagerInstance::PopLastLog()
	{
		if(m_pCRequestManager)
			m_pCRequestManager->PopLastLog(m_nPort);
	}
}