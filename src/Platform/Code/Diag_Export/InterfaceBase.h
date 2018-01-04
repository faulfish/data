#pragma once
#include "interfacebase.h"
#include "..\RequestManager\RequestManagerExport.h"

#pragma warning(push)
#pragma warning(disable:4273)

class CInterfaceBase
{
protected:
	class TemporaryCloseComport
	{
	public:
		TemporaryCloseComport(CInterfaceBase* pThis):m_ptIRequestManager(pThis->m_ptIRequestManager)
		{
			if(m_ptIRequestManager)
				m_ptIRequestManager->Close();
		}
		~TemporaryCloseComport()
		{		
			if(m_ptIRequestManager)
				m_ptIRequestManager->Open();
		}
	private:
		PLATFORM_TOOL::CIRequestManagerPtr& m_ptIRequestManager;
};
public:
	virtual bool EnableDiagEvent(bool bEnable);
protected:
	CInterfaceBase(int nCOMPort);
	virtual ~CInterfaceBase();

	virtual bool Execuate(PLATFORM_TOOL::CRequestPtr& ptRequest);
	virtual bool IsConnected() const;	

	//Utility
	int DigitToHexBcd(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize);
	int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize);
	unsigned short ShortEndianTransfer(unsigned short usValue);
	unsigned int IntEndianTransfer(unsigned int usValue);
private:
	PLATFORM_TOOL::CIRequestManagerPtr m_ptIRequestManager;
	int																		m_nCOMPort;
	bool																		m_bIsConnected;
};
