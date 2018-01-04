/*****************************************************************************/
/** @file:		ATPIMRequest.h
  This is the header file that contains all the PIM related classes.

  $Author:   Sean Hsu  $
  $Version:		1.0 $
  $Date:   Feb 09 2007 11:25:42  $
  */
/*****************************************************************************/
#ifndef __ATPIMREQUEST_H__
#define __ATPIMREQUEST_H__

#include "PotocolRequestBase.h"
#include "Global_Def.h"

/*! @class		CATPIMBaseRequest
 *  @brief		the base class of PIM request with AT command
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CATPIMBaseRequest: public CPotocolRequestBase
{	
public:
	CATPIMBaseRequest() {;}
	virtual ~CATPIMBaseRequest() {;}
	virtual const enumServiceType GetServiceType() const { return SERVICE_AT_CMD; }
	virtual const enumProtocolState GetProtocolType() const { return STATE_AT_CMD; }

private:
	const bool	DoOpen() { return TRUE; }
	const bool	DoClose() { return TRUE; }
	const bool	DoRequest(bool bResursive = false)	{ return TRUE; }
	void	DoSetOpen(const bool bIsOpen) {;}
	const bool	DoIsOpen() { return TRUE; }
	const bool	DoCanClose() { return TRUE; }
	const bool	DoDataArrival(CBufferPtr& spBuffer) { return TRUE; }
	const bool  DoIsFinishPackage(CBufferPtr& spBuffer)  { return TRUE; }
};

/*! @class		CRequestATPIMSwitchToGSM
 *  @brief		switch to GSM mode
 *  @author		Sean Hsu
 *  @date		2007/02/09
 */
class CRequestATPIMSwitchToGSM : public CATPIMBaseRequest
{
public:
	CRequestATPIMSwitchToGSM();
	~CRequestATPIMSwitchToGSM() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
};

typedef CRefObjectPtr<CRequestATPIMSwitchToGSM> CRequestATPIMSwitchToGSMPtr;

/*! @class		CRequestATPIMSwitchToUCS2
 *  @brief		switch to UCS2 mode
 *  @author		Sean Hsu
 *  @date		2007/02/09
 */
class CRequestATPIMSwitchToUCS2 : public CATPIMBaseRequest
{
public:
	CRequestATPIMSwitchToUCS2();
	~CRequestATPIMSwitchToUCS2() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
};

typedef CRefObjectPtr<CRequestATPIMSwitchToUCS2> CRequestATPIMSwitchToUCS2Ptr;

/*! @class		CRequestATPIMPhonebookList
 *  @brief		list phone book record in a specified storage
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMPhonebookList : public CATPIMBaseRequest
{
public:
	CRequestATPIMPhonebookList(enumStorage ePIMStorage);
	~CRequestATPIMPhonebookList() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }
	CString GetPhonebookIDList() { return m_strPhonebookList; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	enumStorage m_ePIMStorage;
	CString m_strPhonebookList;
};

typedef CRefObjectPtr<CRequestATPIMPhonebookList> CRequestATPIMPhonebookListPtr;

/*! @class		CRequestATPIMPhonebookRead
 *  @brief		read a phonebook record in a specified storage
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMPhonebookRead : public CATPIMBaseRequest
{
public:
	CRequestATPIMPhonebookRead(enumStorage ePIMStorage, DWORD dwIndex);
	~CRequestATPIMPhonebookRead() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }
	CString GetPhonebookRecord() { return m_strBenqVCard; }
	CString GetPhonebookRecordName() { return m_strName; }
	CString GetPhonebookRecordNumber() { return m_strNumber; }

private:
	BOOL QueryCurAtCommandSetFromCfgAndSet();
	enumAtCmdSet GetCurAtCmdSet() { return m_eCurAtCmdSet; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	enumAtCmdSet m_eCurAtCmdSet;
	enumStorage m_ePIMStorage;
	DWORD m_dwIndex;
	CString m_strBenqVCard;
	CString m_strName;
	CString m_strNumber;
};

typedef CRefObjectPtr<CRequestATPIMPhonebookRead> CRequestATPIMPhonebookReadPtr;

/*! @class		CRequestATPIMPhonebookWrite
 *  @brief		write a phonebook record in a specified storage
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMPhonebookWrite : public CATPIMBaseRequest
{
public:
	CRequestATPIMPhonebookWrite(enumStorage ePIMStorage, DWORD dwIndex, DWORD dwDataSizeInByte, CString strName, CString strNumber);
	~CRequestATPIMPhonebookWrite() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	BOOL QueryCurAtCommandSetFromCfgAndSet();
	enumAtCmdSet GetCurAtCmdSet() { return m_eCurAtCmdSet; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	enumAtCmdSet m_eCurAtCmdSet;
	enumStorage m_ePIMStorage;
	DWORD m_dwIndex;
	DWORD m_dwDataSizeInByte;
	CString m_strName;
	CString m_strNumber;
};

typedef CRefObjectPtr<CRequestATPIMPhonebookWrite> CRequestATPIMPhonebookWritePtr;

/*! @class		CRequestATPIMPhonebookWriteData
 *  @brief		write a phonebook record data
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMPhonebookWriteData : public CATPIMBaseRequest
{
public:
	CRequestATPIMPhonebookWriteData(LPCTSTR pszVCard);
	~CRequestATPIMPhonebookWriteData() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	LPCTSTR m_pszVCard;
};

typedef CRefObjectPtr<CRequestATPIMPhonebookWriteData> CRequestATPIMPhonebookWriteDataPtr;

/*! @class		CRequestATPIMPhonebookDelete
 *  @brief		delete a phonebook record in a specified storage
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMPhonebookDelete : public CATPIMBaseRequest
{
public:
	CRequestATPIMPhonebookDelete(enumStorage ePIMStorage, enumPIMAction ePIMAction, DWORD dwIndex);
	~CRequestATPIMPhonebookDelete() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	BOOL QueryCurAtCommandSetFromCfgAndSet();
	enumAtCmdSet GetCurAtCmdSet() { return m_eCurAtCmdSet; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	enumAtCmdSet m_eCurAtCmdSet;
	enumStorage m_ePIMStorage;
	enumPIMAction m_ePIMAction;
	DWORD m_dwIndex;
};

typedef CRefObjectPtr<CRequestATPIMPhonebookDelete> CRequestATPIMPhonebookDeletePtr;

/*! @class		CRequestATPIMCalendarRead
 *  @brief		read a calendar record
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMCalendarRead : public CATPIMBaseRequest
{
public:
	CRequestATPIMCalendarRead(DWORD dwIndex);
	~CRequestATPIMCalendarRead() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }
	CString GetCalendarRecord() { return m_strBenqVCalendar; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	DWORD m_dwIndex;
	CString m_strBenqVCalendar;
};

typedef CRefObjectPtr<CRequestATPIMCalendarRead> CRequestATPIMCalendarReadPtr;

/*! @class		CRequestATPIMCalendarWrite
 *  @brief		write a calendar record
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMCalendarWrite : public CATPIMBaseRequest
{
public:
	CRequestATPIMCalendarWrite(DWORD dwIndex, DWORD dwDataSizeInByte);
	~CRequestATPIMCalendarWrite() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	DWORD m_dwIndex;
	DWORD m_dwDataSizeInByte;
};

typedef CRefObjectPtr<CRequestATPIMCalendarWrite> CRequestATPIMCalendarWritePtr;

/*! @class		CRequestATPIMCalendarWrite
 *  @brief		write a calendar record data
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMCalendarWriteData : public CATPIMBaseRequest
{
public:
	CRequestATPIMCalendarWriteData(LPCTSTR pszVCalendar, DWORD dwDataSizeInByte);
	~CRequestATPIMCalendarWriteData() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }
	DWORD GetIndex() { return m_dwIndex; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	LPCTSTR m_pszVCalendar;
	DWORD m_dwDataSizeInByte;
	DWORD m_dwIndex;
};

typedef CRefObjectPtr<CRequestATPIMCalendarWriteData> CRequestATPIMCalendarWriteDataPtr;

/*! @class		CRequestATPIMCalendarDelete
 *  @brief		delete a calendar record
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMCalendarDelete : public CATPIMBaseRequest
{
public:
	CRequestATPIMCalendarDelete(enumPIMAction ePIMAction, DWORD dwIndex);
	~CRequestATPIMCalendarDelete() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	enumPIMAction m_ePIMAction;
	DWORD m_dwIndex;
};

typedef CRefObjectPtr<CRequestATPIMCalendarDelete> CRequestATPIMCalendarDeletePtr;

/*! @class		CRequestATPIMCallerGroupRead
 *  @brief		read a caller group record
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMCallerGroupRead : public CATPIMBaseRequest
{
public:
	CRequestATPIMCallerGroupRead(DWORD dwIndex);
	~CRequestATPIMCallerGroupRead() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	DWORD m_dwIndex;
};

typedef CRefObjectPtr<CRequestATPIMCallerGroupRead> CRequestATPIMCallerGroupReadPtr;

/*! @class		CRequestATPIMCallerGroupWrite
 *  @brief		write a caller group record
 *  @author		Sean Hsu
 *  @date		2007/01/30
 */
class CRequestATPIMCallerGroupWrite : public CATPIMBaseRequest
{
public:
	CRequestATPIMCallerGroupWrite(CallergroupSetting_t tagCallerGroupSetting);
	~CRequestATPIMCallerGroupWrite() {;}
	HRESULT ExecuteRequest();
	enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	void OnDataArrival(CBufferPtr spBuffer);
	void OnWriteCompleted() {;}

	BOOL GetResult() { return m_bResult; }

private:
	CAtCodecPtr m_spATCodec;
	HANDLE m_hDataArrivalEvent;
	BOOL m_bResult;
	CallergroupSetting_t m_tagCallerGroupSetting;
};

typedef CRefObjectPtr<CRequestATPIMCallerGroupWrite> CRequestATPIMCallerGroupWritePtr;

#endif // #ifndef __ATPIMREQUEST_H__