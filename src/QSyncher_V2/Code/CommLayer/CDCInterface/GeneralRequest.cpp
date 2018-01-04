/*****************************************************************************/
/** @file:		GeneralRequest.cpp
  This is the file that contains all the functions related to the general
  classes.

  $Author:   Eddy Guo  $
  $Version:		1.0 $
  $Date:   Feb 19 2008 16:12:28  $
  */
/*****************************************************************************/
#include "stdafx.h"
#include "GeneralRequest.h"
#include "MessageRequest.h"

//////////////////////////////////////////////////////////////////////////
// set port																//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		CString cstrPort - the COM port to be set
  @return		None
  @author		Eddy Guo
  @date			2007/01/30
  */
CRequestSetPort::CRequestSetPort(CString cstrPort,int nBaudRate) : m_cstrPort(cstrPort),m_nBaudRate(nBaudRate)
{
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/01/30
  */
HRESULT CRequestSetPort::ExecuteRequest()
{
	HRESULT hr = S_OK;

	hr = g_ProtocolManager.SetPort(m_cstrPort,m_nBaudRate);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// reset prot															//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		None
  @return		None
  @author		Eddy Guo
  @date			2007/01/30
  */
CRequestResetPort::CRequestResetPort()
{
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/01/30
  */
HRESULT CRequestResetPort::ExecuteRequest()
{
	HRESULT hr = S_OK;

	hr = g_ProtocolManager.ResetPort();

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// check password														//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		CString cstrPassword - the password to be check
  @return		None
  @author		Eddy Guo
  @date			2007/01/30
  */
CRequestCheckPassword::CRequestCheckPassword(CString cstrPassword) : m_cstrPassword(cstrPassword)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/01/30
  */
HRESULT CRequestCheckPassword::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqCheckPassword(m_cstrPassword);

    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);

}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Eddy Guo
  @date			2007/01/30
  */
void CRequestCheckPassword::OnDataArrival(CBufferPtr spBuffer)
{
	CAtCodecPtr spAtCodec = new CAtCodec();

	CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;

	if (m_spATCodec)
		enuActiveOpcode = m_spATCodec->GetActiveOpcode();

	m_ptBufferReceiver->Append(spBuffer);

	while (spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver))
	{
		if (spAtCodec->GetOpcode() == CAtCodec::otResult)
		{
			m_bResult = TRUE;

			spAtCodec->DecodeResult();
		}

		m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
	}

	if (m_bResult)
		SetEvent(m_hDataArrivalEvent);
	else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
// echo off																//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		None
  @return		None
  @author		Eddy Guo
  @date			2007/01/30
  */
CRequestEchoOff::CRequestEchoOff()
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Eddy Guo
  @date			2007/01/30
  */
HRESULT CRequestEchoOff::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateEchoOff();

    return ::ExecuteRequestT(m_spATCodec, this, m_hDataArrivalEvent);

}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Eddy Guo
  @date			2007/01/30
  */
void CRequestEchoOff::OnDataArrival(CBufferPtr spBuffer)
{
	CAtCodecPtr spAtCodec = new CAtCodec();

	CAtCodec::Opcode_t enuActiveOpcode = CAtCodec::otReqAt;

	if (m_spATCodec)
		enuActiveOpcode = m_spATCodec->GetActiveOpcode();

	m_ptBufferReceiver->Append(spBuffer);

	while (spAtCodec->IsCompleteResult(enuActiveOpcode, m_ptBufferReceiver))
	{
		if (spAtCodec->GetOpcode() == CAtCodec::otResult)
		{
			m_bResult = TRUE;

			spAtCodec->DecodeResult();
		}

		m_ptBufferReceiver->PopFront(spAtCodec->GetPackageSize());
	}

	if (m_bResult = TRUE)
		SetEvent(m_hDataArrivalEvent);
	else
	    ASSERT(false);
}

//////////////////////////////////////////////////////////////////////////
// Check on line or off line                                            //
//////////////////////////////////////////////////////////////////////////

CRequestCheckConnectivity::CRequestCheckConnectivity(bool& bIsOnline) :
m_hDataArrivalEvent(::CreateEvent(NULL, FALSE, FALSE, NULL)),
m_bResult(FALSE),
m_pbIsOnline(&bIsOnline)
{
    char* sz = new char[30];
    ::ZeroMemory(sz, 30);
    sprintf(sz, "AT+CFUN?\r");
    m_ptBufferSend = new CBuffer(strlen(sz), sz);
    delete [] sz;
}

const bool CRequestCheckConnectivity::DoDataArrival(CBufferPtr& spBuffer) {
    if(IsOK(spBuffer))
    {
        spBuffer->ReleaseBuffer(spBuffer->GetLength() - 8);
        spBuffer->PopFront(8);
        char szBuffer[256] = {0};
        strncpy(szBuffer,spBuffer->GetData(),spBuffer->GetLength());
        *m_pbIsOnline = atoi(szBuffer) == 1;
    }
    else
    {
        *m_pbIsOnline = false;
    }
    return true;
}
