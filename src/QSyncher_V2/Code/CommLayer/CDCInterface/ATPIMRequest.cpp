/*****************************************************************************/
/** @file:		ATPIMRequest.cpp
  This is the file that contains all the functions related to the PIM classes.

  $Author:   Sean Hsu  $
  $Version:		1.0 $
  $Date:   Feb 09 2007 11:25:40  $
  */
/*****************************************************************************/
#include "stdafx.h"
#include "ATPIMRequest.h"
#include "ProtocolManager.h"

//////////////////////////////////////////////////////////////////////////
// switch to GSM														//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		None
  @return		None
  @author		Sean Hsu
  @date			2007/02/09
  */
CRequestATPIMSwitchToGSM::CRequestATPIMSwitchToGSM()
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/02/09
  */
HRESULT CRequestATPIMSwitchToGSM::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqSwitchToGSM();

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/02/09
  */
void CRequestATPIMSwitchToGSM::OnDataArrival(CBufferPtr spBuffer)
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
// switch to UCS2														//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		None
  @return		None
  @author		Sean Hsu
  @date			2007/02/09
  */
CRequestATPIMSwitchToUCS2::CRequestATPIMSwitchToUCS2()
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/02/09
  */
HRESULT CRequestATPIMSwitchToUCS2::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqSwitchToUCS2();

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/02/09
  */
void CRequestATPIMSwitchToUCS2::OnDataArrival(CBufferPtr spBuffer)
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
// list phonebook record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		enumStorage ePIMStorage - the storage to be listed
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMPhonebookList::CRequestATPIMPhonebookList(enumStorage ePIMStorage) : m_ePIMStorage(ePIMStorage)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMPhonebookList::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqListUsedPhonebookEntries(m_ePIMStorage);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMPhonebookList::OnDataArrival(CBufferPtr spBuffer)
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
	{
		m_strPhonebookList = spAtCodec->GetMessageIDList();

		SetEvent(m_hDataArrivalEvent);		
	}
	else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
// read a phonebook record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		enumStorage ePIMStorage - the storage to be read from
  @param		DWORD dwIndex - ID of the phonbook record
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMPhonebookRead::CRequestATPIMPhonebookRead(enumStorage ePIMStorage, DWORD dwIndex) : m_ePIMStorage(ePIMStorage), m_dwIndex(dwIndex)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMPhonebookRead::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	if (QueryCurAtCommandSetFromCfgAndSet())
	{
		if (GetCurAtCmdSet() == AT_CMD_STANDARD)
			m_spATCodec = CAtCodec::CreateReqReadPhonebookEntriesStd(m_dwIndex, m_dwIndex);
		else if (GetCurAtCmdSet() == AT_CMD_BENQ)
			m_spATCodec = CAtCodec::CreateReqReadPhonebookEntry(m_ePIMStorage, m_dwIndex);
		else
			ASSERT(FALSE);
	}
	else
		ASSERT(FALSE);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMPhonebookRead::OnDataArrival(CBufferPtr spBuffer)
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
	{
		if (QueryCurAtCommandSetFromCfgAndSet())
		{
			if (GetCurAtCmdSet() == AT_CMD_STANDARD)
			{
				m_strName = spAtCodec->GetPBEntries()->GetName(0);
				m_strNumber = spAtCodec->GetPBEntries()->GetNumber(0);
			}
			else if (GetCurAtCmdSet() == AT_CMD_BENQ)
				m_strBenqVCard = spAtCodec->GetBenQVCard();
			else
				ASSERT(FALSE);
		}
		else
			ASSERT(FALSE);

		SetEvent(m_hDataArrivalEvent);		
	}
	else
	{
	}
}

/*---------------------------------------------------------------------------*/
/*!
  query current AT command set from configuration file

  @param		None
  @return		BOOL - TRUE if successful; FALSE if failed
  @author		Sean Hsu
  @date			2007/02/09
  */
BOOL CRequestATPIMPhonebookRead::QueryCurAtCommandSetFromCfgAndSet()
{
	BOOL bResult = TRUE;

	m_eCurAtCmdSet = AT_CMD_BENQ;

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// write a phonebook record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		enumStorage ePIMStorage - the storage to be written into
  @param		DWORD dwIndex - ID of the phonbook record
  @param		DWORD dwDataSizeInByte - the size of the data
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMPhonebookWrite::CRequestATPIMPhonebookWrite(enumStorage ePIMStorage, DWORD dwIndex, DWORD dwDataSizeInByte, CString strName, CString strNumber)
: m_ePIMStorage(ePIMStorage), m_dwIndex(dwIndex), m_dwDataSizeInByte(dwDataSizeInByte), m_strName(strName), m_strNumber(strNumber)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMPhonebookWrite::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	if (QueryCurAtCommandSetFromCfgAndSet())
	{
		if (GetCurAtCmdSet() == AT_CMD_STANDARD)
			m_spATCodec = CAtCodec::CreateReqWritePhonebookEntryStd(m_dwIndex, m_strName, m_strNumber);
		else if (GetCurAtCmdSet() == AT_CMD_BENQ)
			m_spATCodec = CAtCodec::CreateReqWritePhonebookEntry(m_ePIMStorage, m_dwIndex, m_dwDataSizeInByte);
		else
			ASSERT(FALSE);
	}
	else
		ASSERT(FALSE);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMPhonebookWrite::OnDataArrival(CBufferPtr spBuffer)
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

/*---------------------------------------------------------------------------*/
/*!
  query current AT command set from configuration file

  @param		None
  @return		BOOL - TRUE if successful; FALSE if failed
  @author		Sean Hsu
  @date			2007/02/09
  */
BOOL CRequestATPIMPhonebookWrite::QueryCurAtCommandSetFromCfgAndSet()
{
	BOOL bResult = TRUE;

	m_eCurAtCmdSet = AT_CMD_BENQ;

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// write a phonebook record	data										//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		LPCTSTR pszVCard - the phonebook record data
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMPhonebookWriteData::CRequestATPIMPhonebookWriteData(LPCTSTR pszVCard) : m_pszVCard(pszVCard)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMPhonebookWriteData::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqWritePhonebookEntryData(m_pszVCard);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMPhonebookWriteData::OnDataArrival(CBufferPtr spBuffer)
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
// delete phonebook record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		enumStorage ePIMStorage - the storage to be deleted
  @param		enumPIMAction ePIMAction - action
  @param		DWORD dwIndex - the index of the phonebook record
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMPhonebookDelete::CRequestATPIMPhonebookDelete(enumStorage ePIMStorage, enumPIMAction ePIMAction, DWORD dwIndex)
: m_ePIMStorage(ePIMStorage), m_ePIMAction(ePIMAction), m_dwIndex(dwIndex)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMPhonebookDelete::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	if (QueryCurAtCommandSetFromCfgAndSet())
	{
		if (GetCurAtCmdSet() == AT_CMD_STANDARD)
			m_spATCodec = CAtCodec::CreateReqDeletePhonebookEntryStd(m_dwIndex);
		else if (GetCurAtCmdSet() == AT_CMD_BENQ)
			m_spATCodec = CAtCodec::CreateReqDeletePhonebookEntry(m_ePIMStorage, m_ePIMAction, m_dwIndex);
		else
			ASSERT(FALSE);
	}
	else
		ASSERT(FALSE);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMPhonebookDelete::OnDataArrival(CBufferPtr spBuffer)
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

/*---------------------------------------------------------------------------*/
/*!
  query current AT command set from configuration file

  @param		None
  @return		BOOL - TRUE if successful; FALSE if failed
  @author		Sean Hsu
  @date			2007/02/09
  */
BOOL CRequestATPIMPhonebookDelete::QueryCurAtCommandSetFromCfgAndSet()
{
	BOOL bResult = TRUE;

	m_eCurAtCmdSet = AT_CMD_BENQ;

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// read a calendar record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		DWORD dwIndex - the index of the phonebook record
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMCalendarRead::CRequestATPIMCalendarRead(DWORD dwIndex) : m_dwIndex(dwIndex)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMCalendarRead::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqReadCalendarEntry(m_dwIndex);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMCalendarRead::OnDataArrival(CBufferPtr spBuffer)
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
	{
		m_strBenqVCalendar = spAtCodec->GetBenQVCalendar();

		SetEvent(m_hDataArrivalEvent);		
	}
	else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
// write a calendar record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		DWORD dwIndex - the index of the phonebook record
  @param		DWORD dwDataSizeInByte - the size of the data
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMCalendarWrite::CRequestATPIMCalendarWrite(DWORD dwIndex, DWORD dwDataSizeInByte) : m_dwIndex(dwIndex), m_dwDataSizeInByte(dwDataSizeInByte)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMCalendarWrite::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqWriteCalendarEntry(m_dwIndex, m_dwDataSizeInByte);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMCalendarWrite::OnDataArrival(CBufferPtr spBuffer)
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
// write a calendar record data											//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		LPCTSTR pszVCalendar - the vCalendar data
  @param		DWORD dwDataSizeInByte - the size of the data
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMCalendarWriteData::CRequestATPIMCalendarWriteData(LPCTSTR pszVCalendar, DWORD dwDataSizeInByte) : m_pszVCalendar(pszVCalendar), m_dwDataSizeInByte(dwDataSizeInByte)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMCalendarWriteData::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqWriteCalendarEntryData(m_pszVCalendar, m_dwDataSizeInByte);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMCalendarWriteData::OnDataArrival(CBufferPtr spBuffer)
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
	{
		m_dwIndex = spAtCodec->GetCurrentWriteCalendarEntryIndex();

		SetEvent(m_hDataArrivalEvent);
	}
	else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
// delete calendar record												//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		enumPIMAction ePIMAction - action
  @param		DWORD dwIndex - the ID of the phonebook record
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMCalendarDelete::CRequestATPIMCalendarDelete(enumPIMAction ePIMAction, DWORD dwIndex) : m_ePIMAction(ePIMAction), m_dwIndex(dwIndex)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMCalendarDelete::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqDeleteCalendarEntry(m_ePIMAction, m_dwIndex);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMCalendarDelete::OnDataArrival(CBufferPtr spBuffer)
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
// read caller group													//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		DWORD dwIndex - the ID of the caller group
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMCallerGroupRead::CRequestATPIMCallerGroupRead(DWORD dwIndex) : m_dwIndex(dwIndex)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMCallerGroupRead::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqReadCallergroupSetting(m_dwIndex);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMCallerGroupRead::OnDataArrival(CBufferPtr spBuffer)
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
	{
		SetEvent(m_hDataArrivalEvent);		
	}
	else
	{
	}
}

//////////////////////////////////////////////////////////////////////////
// write caller group													//
//////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------*/
/*!
  construction

  @param		CallergroupSetting_t tagCallerGroupSetting - the structure that 
				contains the caller group information
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
CRequestATPIMCallerGroupWrite::CRequestATPIMCallerGroupWrite(CallergroupSetting_t tagCallerGroupSetting) : m_tagCallerGroupSetting(tagCallerGroupSetting)
{
	m_hDataArrivalEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bResult = FALSE;
}

/*---------------------------------------------------------------------------*/
/*!
  execute the request

  @param		None
  @return		HRESULT - S_OK if successful; E_FAIL if failed
  @author		Sean Hsu
  @date			2007/01/30
  */
HRESULT CRequestATPIMCallerGroupWrite::ExecuteRequest()
{
	HRESULT hr = S_OK;
	
	::ResetEvent(m_hDataArrivalEvent);

	m_spATCodec = CAtCodec::CreateReqWriteCallergroupSetting(m_tagCallerGroupSetting);

	g_ProtocolManager.ExecuteProtocolRequest(m_spATCodec->GetPackageBuffer(), this);

	hr = WaitForSingleObject(m_hDataArrivalEvent, 3000);

	if (hr == WAIT_OBJECT_0)
	{	
		if (!GetResult())
			hr = E_FAIL;
		else
			hr = S_OK;
	}

	return hr;
}

/*---------------------------------------------------------------------------*/
/*!
  the callback function when data arrived from the handset

  @param		CBufferPtr spBuffer - the smart pointer points to the buffer 
				that contains the data from the handset
  @return		None
  @author		Sean Hsu
  @date			2007/01/30
  */
void CRequestATPIMCallerGroupWrite::OnDataArrival(CBufferPtr spBuffer)
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