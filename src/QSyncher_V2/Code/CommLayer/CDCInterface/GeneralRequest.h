/*****************************************************************************/
/** @file:		GeneralRequest.h
  This is the header file that contains all the general classes.

  $Author:   Alan Tu  $
  $Version:		1.0 $
  $Date:   Mar 24 2008 10:37:36  $
  */
/*****************************************************************************/
#ifndef __GENERALREQUEST_H__
#define __GENERALREQUEST_H__

#include "ATFSRequest.h"

class CATCOMMANDBaseRequest: public CATFSBaseRequest
{	
public:
	CATCOMMANDBaseRequest(DWORD dwTimeout = 5000):CATFSBaseRequest(dwTimeout) {;}
	virtual ~CATCOMMANDBaseRequest() {;}
};

/*! @class		CRequestSetPort
 *  @brief		set port
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestSetPort : public CRequest
{
public:
	CRequestSetPort(CString cstrPort,int nBaudRate = 115200);
	virtual ~CRequestSetPort() {;}
	virtual const enumServiceType GetServiceType() const { return SERVICE_CONNECTION; }
	virtual const enumProtocolState GetProtocolType() const { return STATE_AT_CMD; }
	HRESULT ExecuteRequest();
	virtual enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	virtual enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }

private:
	CString m_cstrPort;
	int m_nBaudRate;
};

typedef CRefObjectPtr<CRequestSetPort> CRequestSetPortPtr;

/*! @class		CRequestResetPort
 *  @brief		close port
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestResetPort : public CRequest
{
public:
	CRequestResetPort();
	virtual ~CRequestResetPort() {;}
	virtual const enumServiceType GetServiceType() const { return SERVICE_CONNECTION; }
	virtual const enumProtocolState GetProtocolType() const { return STATE_AT_CMD; }
	virtual enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	virtual enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	HRESULT ExecuteRequest();
};

typedef CRefObjectPtr<CRequestResetPort> CRequestResetPortPtr;

#include "MessageRequest.h"
class CRequestGetIMEI: public CATCOMMANDBaseRequest
{
public:
	CRequestGetIMEI(TSTRING& strIMEI,bool& bSucceed,DWORD dwTimeout)
		:CATCOMMANDBaseRequest(dwTimeout)
		,m_strIMEI(strIMEI)
		,m_bSucceed(bSucceed)
	{
		char* sz = new char[30];
		::ZeroMemory(sz,30);
		sprintf(sz, "AT+CGSN\r");
		m_ptBufferSend = new CBuffer(strlen(sz),sz);
		delete [] sz;
	}
	virtual ~CRequestGetIMEI() {;}	
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/* Without IMEI version
		000082: Bulk or Interrupt Transfer (UP), 19.12.2007 11:42:45.3312843 +0.0156362 Pipe Handle: 0x8648886c Send 0x8 bytes to the device:
		41 54 2B 43 47 53 4E 0D
		AT+CGSN.
		000083: Bulk or Interrupt Transfer (UP), 19.12.2007 11:42:45.3312843 +0.0 Pipe Handle: 0x8648884c Get 0x1e bytes from the device:
		0D 0A 2B 43 4D 45 20 45 52 52 4F 52 3A 20 6D 65
		6D 6F 72 79 20 66 61 69 6C 75 72 65 0D 0A
		..+CME ERROR: me
		mory failure..
		*/
		/* Normal version
		000228: Bulk or Interrupt Transfer (UP), 19.12.2007 11:47:36.8487577 +29.2193110
		Pipe Handle: 0x8995085c (Endpoint Address: 0x2)
		Send 0x8 bytes to the device:
		41 54 2B 43 47 53 4E 0D                           AT+CGSN.
		000229: Bulk or Interrupt Transfer (UP), 19.12.2007 11:47:36.8487577 +0.0
		Pipe Handle: 0x8995083c (Endpoint Address: 0x82)
		Get 0x19 bytes from the device:
		0D 0A 33 35 36 33 35 33 30 31 30 30 30 30 38 39   ..35635301000089
		34 0D 0A 0D 0A 4F 4B 0D 0A                        4....OK..
		*/
		if(IsOK(spBuffer))
		{
			spBuffer->ReleaseBuffer(spBuffer->GetLength() - 8);
			spBuffer->PopFront(2);
			char szBuffer[256] = {0};
			strncpy(szBuffer,spBuffer->GetData(),spBuffer->GetLength());			
			m_strIMEI = GetTSTRING(szBuffer);
			m_bSucceed = true;
		}
		else
			m_bSucceed = false;
		return true;
	}
private:	
	bool&				m_bSucceed;
	TSTRING&		m_strIMEI;
};

class CRequestGetSoftwareVer: public CATCOMMANDBaseRequest
{
public:
	CRequestGetSoftwareVer(CString* pstrSoftwareVer):m_pstrSoftwareVer(pstrSoftwareVer)
	{
		char* sz = new char[30];
		::ZeroMemory(sz,30);
		sprintf(sz, "AT+CGMR\r");
		m_ptBufferSend = new CBuffer(strlen(sz),sz);
		delete [] sz;
	}
	virtual ~CRequestGetSoftwareVer() {;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		if(IsOK(spBuffer))
		{
			spBuffer->ReleaseBuffer(spBuffer->GetLength() - 8);
			spBuffer->PopFront(2);
			char szBuffer[256] = {0};
			strncpy(szBuffer,spBuffer->GetData(),spBuffer->GetLength());			
			*m_pstrSoftwareVer = szBuffer;
		}
		return true;
	}
private:
	CString *m_pstrSoftwareVer;
};

class CRequestGetManufacturer: public CATCOMMANDBaseRequest
{
public:
	CRequestGetManufacturer(CString* pcstrManufacturer):m_pcstrManufacturer(pcstrManufacturer)
	{
		char* sz = new char[30];
		::ZeroMemory(sz,30);
		sprintf(sz, "AT+CGMI\r");
		m_ptBufferSend = new CBuffer(strlen(sz),sz);
		delete [] sz;
	}
	virtual ~CRequestGetManufacturer() {;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		if(IsOK(spBuffer))
		{
			spBuffer->ReleaseBuffer(spBuffer->GetLength() - 8);
			spBuffer->PopFront(2);
			char szBuffer[256] = {0};
			strncpy(szBuffer,spBuffer->GetData(),spBuffer->GetLength());			
			*m_pcstrManufacturer = szBuffer;
		}
		return true;
	}
private:
	CString* m_pcstrManufacturer;
};

class CRequestGetPhoneModel : public CATCOMMANDBaseRequest 
{
public:	
	CRequestGetPhoneModel(CString* pcstrModel):m_pcstrModel(pcstrModel)
	{
		char* sz = new char[30];
		::ZeroMemory(sz,30);
		sprintf(sz, "AT+CGMM\r");
		m_ptBufferSend = new CBuffer(strlen(sz),sz);
		delete [] sz;
	}
	virtual ~CRequestGetPhoneModel() {}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		if(IsOK(spBuffer))
		{
			spBuffer->ReleaseBuffer(spBuffer->GetLength() - 8);
			spBuffer->PopFront(2);
			char szBuffer[256] = {0};
			strncpy(szBuffer,spBuffer->GetData(),spBuffer->GetLength());			
			*m_pcstrModel = szBuffer;
		}
		return true;
	}

private:
	CString* m_pcstrModel;
};

class CRequestGetFDN: public CATCOMMANDBaseRequest
{
public:
	CRequestGetFDN(bool& bIsFDNMode):m_bIsFDNMode(bIsFDNMode)
	{
		/*
		        AT+CLCK="FD",2  ==> query status of FDN  mode
                +CLCK:0  ==> unlock
                +CLCK:1 ==> lock
		*/
		char* sz = new char[30];
		::ZeroMemory(sz,30);
		sprintf(sz, "AT+CLCK=\"FD\",2\r");
		m_ptBufferSend = new CBuffer(strlen(sz),sz);
		delete [] sz;
	}
	virtual ~CRequestGetFDN() {;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*
		000137: Bulk or Interrupt Transfer (UP), 14.02.2008 14:42:17.0838153 +4.0791951
		Pipe Handle: 0x892c3194 (Endpoint Address: 0x2)
		Send 0xf bytes to the device:
		41 54 2B 43 4C 43 4B 3D 22 46 44 22 2C 32 0D      AT+CLCK="FD",2.
		000138: Bulk or Interrupt Transfer (UP), 14.02.2008 14:42:17.0838153 +0.0
		Pipe Handle: 0x892c3174 (Endpoint Address: 0x82)
		Get 0x12 bytes from the device:
		0D 0A 2B 43 4C 43 4B 3A 20 30 0D 0A 0D 0A 4F 4B   ..+CLCK: 0....OK
		0D 0A                                             ..
		*/
		if(IsOK(spBuffer))
		{
			spBuffer->ReleaseBuffer(spBuffer->GetLength() - 8);
			spBuffer->PopFront(8);
			char szBuffer[256] = {0};
			strncpy(szBuffer,spBuffer->GetData(),spBuffer->GetLength());
			m_bIsFDNMode = atoi(szBuffer) == 1;
		}
		else
		{
			m_bIsFDNMode = false;
		}
		return true;
	}
private:
	bool& m_bIsFDNMode;
};

class CRequestCheckPassword: public CATCOMMANDBaseRequest
{
public:
	CRequestCheckPassword(CString cstrPassword);
	virtual ~CRequestCheckPassword() {;}
	HRESULT ExecuteRequest();
	void OnDataArrival(CBufferPtr spBuffer);

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	CString m_cstrPassword;
};

typedef CRefObjectPtr<CRequestCheckPassword> CRequestCheckPasswordPtr;

/*! @class		CRequestEchoOff
 *  @brief		echo off
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CRequestEchoOff: public CATCOMMANDBaseRequest
{
public:
	CRequestEchoOff();
	virtual ~CRequestEchoOff() {;}
	HRESULT ExecuteRequest();
	void OnDataArrival(CBufferPtr spBuffer);

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
};

typedef CRefObjectPtr<CRequestEchoOff> CRequestEchoOffPtr;

class CRequestCheckConnectivity: public CATCOMMANDBaseRequest
{
public:
    CRequestCheckConnectivity(bool& bIsOnline);
    virtual ~CRequestCheckConnectivity() {;}
    const bool DoDataArrival(CBufferPtr& spBuffer);
    BOOL GetResult() { return m_bResult; }
    bool IsOnline() {return *m_pbIsOnline; }
private:
    CAtCodecPtr m_spATCodec;
    HANDLE m_hDataArrivalEvent;
    BOOL m_bResult;
    bool *m_pbIsOnline;
};

typedef CRefObjectPtr<CRequestCheckConnectivity> CRequestCheckConnectivityPtr;

template <class ATTypePolicy>
class CATRequestBase : public CPotocolRequestBase
{
public:
	virtual const enumServiceType	GetServiceType() const			{return ATTypePolicy::GetServiceType(); }
	virtual const enumProtocolState GetProtocolType() const			{return ATTypePolicy::GetProtocolType(); }
	virtual const bool				DoCanClose()					{return true;}
	virtual const bool				DoIsOpen()						{return true;}
	virtual void					DoSetOpen(const bool bIsOpen)	{}	

protected:
	CATRequestBase():CPotocolRequestBase(){}
	virtual ~CATRequestBase(){}
public:
	virtual const bool DoOpen(){return true;}
	virtual const bool DoClose(){return true;}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		return IsCompleteATPackage(spBuffer);
	}
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		return true;
	}
	virtual int GetReceiverResult(const CBufferPtr& ptReceiver)
	{
		int nRet = -1;
		if(ptReceiver->GetLength() > 0)
		{
			BYTE* pBuffer = (BYTE*)ptReceiver->GetData();
			nRet = pBuffer[0];
		}
		return nRet;
	}
protected:
private:
};

class SERVICE_AT_FS_POLICY
{
public:
	static enumServiceType GetServiceType() {return SERVICE_SWITCH_POTOCOL;}
	static enumProtocolState GetProtocolType() { return STATE_OBEX; }
};

class SERVICE_AT_CMD_POLICY
{
public:
	static enumServiceType GetServiceType() {return SERVICE_SWITCH_POTOCOL;}
	static enumProtocolState GetProtocolType() { return STATE_AT_CMD; }
};
/*! @class		CRequestSwitchOBEXToAT
 *  @brief		switch the handset from OBEX mode to AT command mode
 *  @author		Alan Tu
 *  @date		2007/03/21
 */
class CRequestSwitchOBEXToAT : public CATRequestBase<SERVICE_AT_FS_POLICY>
{
public:
	CRequestSwitchOBEXToAT()
	{
		CAtCodecPtr spData = CAtCodec::CreateReqSwitchToAT();
		m_ptBufferSend = spData->GetPackageBuffer();
	}
	virtual ~CRequestSwitchOBEXToAT() {;}
	
	virtual const bool DoRequest(bool bResursive = false)
	{	
		m_ptBufferReceiver  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes)
		{
			Sleep(1000);
			Fire(_T("Received"));
		}		
		return bRes;
	}
};

/*! @class		CRequestSwitchATToOBEX
 *  @brief		switch the handset from AT command mode to OBEX mode
 *  @author		Alan Tu
 *  @date		2007/03/21
 */
class CRequestSwitchATToOBEX : public CATRequestBase<SERVICE_AT_CMD_POLICY>
{
public:
	CRequestSwitchATToOBEX()
	{
		//m_strParas.push_back("AT^SQWE=0\r");
		m_strParas.push_back("AT^SQWE=3\r");

		m_ptBufferSend = new CBuffer(m_strParas.begin()->size(),m_strParas.begin()->c_str());
		m_strParas.pop_front();
	}
	virtual ~CRequestSwitchATToOBEX() {;}
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		//for AT^SQWE=0 only
		if(m_strParas.size() == 1)
			Sleep(1000);

		BYTE* pBuffer = (BYTE*)m_ptBufferReceiver->GetData();
		if(m_strParas.size() > 0 && m_ptBufferReceiver->GetLength() > 0)
		{
			m_ptBufferReceiver = new CBuffer;
			m_ptBufferSend = new CBuffer(m_strParas.begin()->size(),m_strParas.begin()->c_str());
			m_strParas.pop_front();
			DoRequest();
		}		
		return true;
	}
	virtual const bool DoRequest(bool bResursive = false)
	{	
		m_ptBufferReceiver  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes)
		{
			WaitReceiver();
			while(DoDataArrival(GetReceiver()) == false);
			Fire(_T("Received"));
		}		
		return bRes;
	}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		return IsCompleteATPackage(spBuffer);
	}
private:
	std::list<std::string> m_strParas;
};

class CRequestSwitchDriver : public CATRequestBase<SERVICE_AT_CMD_POLICY>
{
public:
	CRequestSwitchDriver(char nDrive)
	{
		const char szTemp0[] = {0x61,0x74,0x5E,0x73,0x71,0x77,0x65,0x3D,0x31,0x0D};
		Input(szTemp0);
		const char szTemp1[] = {0x02,0x01,0x03,0x14};
		Input(szTemp1);
		const char szTemp2[] = {0x05,0x02,0x07,0x63,0x01};
		Input(szTemp2);
		const char szTemp3[] = {0x05,0x04,0x01,0x63,0x02,0x52,0x01};
		Input(szTemp3);
		unsigned char szTemp4[] = {0x06,0x0B,0x0D,0xF0,0x41,0x54,0x5E,0x53,0x51,0x57,0x45,0x3D,0x32,0x0D};
		Input((char*)szTemp4);
		m_ptBufferSend = *m_strParas.begin();
		m_strParas.pop_front();
	}
	virtual ~CRequestSwitchDriver() {;}
	void Input(const char* pBuffer)
	{
		CBufferPtr ptBuffer = new CBuffer(sizeof(pBuffer)/sizeof(const char),pBuffer);
		m_strParas.push_back(ptBuffer);
	}
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		Sleep(1000);
		BYTE* pBuffer = (BYTE*)m_ptBufferReceiver->GetData();
		if(m_strParas.size() > 0 && m_ptBufferReceiver->GetLength() > 0)
		{
			m_ptBufferReceiver = new CBuffer;
			m_ptBufferSend = *m_strParas.begin();
			m_strParas.pop_front();
			DoRequest();
		}		
		return true;
	}
	virtual const bool DoRequest(bool bResursive = false)
	{	
		m_ptBufferReceiver  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes)
		{
			WaitReceiver();
			while(DoDataArrival(GetReceiver()) == false);
			Fire(_T("Received"));
		}		
		return bRes;
	}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		return true;//IsCompleteATPackage(spBuffer);
	}
private:
	std::list<CBufferPtr> m_strParas;
};

typedef CRefObjectPtr<CRequestSwitchOBEXToAT> CRequestSwitchOBEXToATPtr;
typedef CRefObjectPtr<CRequestSwitchATToOBEX> CRequestSwitchATToOBEXPtr;

#endif // #ifndef __GENERALREQUEST_H__
