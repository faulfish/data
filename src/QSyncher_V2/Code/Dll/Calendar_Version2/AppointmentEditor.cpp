// AppointmentEditor.cpp : implementation file
//

#include "stdafx.h"
#include "AppointmentEditor.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"

namespace QSYNC_LIB
{
	// CAppointmentEditor dialog

	IMPLEMENT_DYNAMIC(CAppointmentEditor, Win32DoubleBufferQDialogWidge)

		CAppointmentEditor* g_pThis = NULL;

	CAppointmentEditor::CAppointmentEditor(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(IDD_APPOINTMENTEDITOR_DIALOG, pParent)
		, m_szSubject(_T(""))
		, m_szLocation(_T(""))
		, m_szAttendees(_T(""))
		, m_szDescription(_T(""))
		, m_bIsNew(true)
		, m_DTInterval(0)
		, m_pIPIMConfig(NULL)
		, m_pLayout(NULL)
		,m_pBtnCancel(NULL)
		,m_pBtnOK(NULL)
		,m_pWinCloseButton(NULL)
	{
		CreateILocalization(&m_pILocale);
		g_pThis = this;
		GetDeviceFinder()->Register(this);
	}

	CAppointmentEditor::~CAppointmentEditor()
	{
		m_pIPIMConfig = NULL;
		GetDeviceFinder()->Unregister(this);
	}

	void CAppointmentEditor::Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == DEVICE_DISCONNECT)
		{
			m_pIPIMConfig = NULL;
		}
		else if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pBtnOK->GetID() == nParam)
				OnBtnClickOK();
			else if(m_pBtnCancel->GetID() == nParam || m_pWinCloseButton->GetID() == nParam)
				FnWinClose();
		}
	}

	void CAppointmentEditor::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_APPOINTMENTEDITOR_EDIT_SUBJECT, m_szSubject);
		DDX_Text(pDX, IDC_APPOINTMENTEDITOR_EDIT_LOCATION, m_szLocation);
		DDX_Text(pDX, IDC_APPOINTMENTEDITOR_EDIT_ATTENDEES, m_szAttendees);
		DDX_Text(pDX, IDC_APPOINTMENTEDITOR_EDIT_DESCRIPTION, m_szDescription);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_SUBJECT, m_dlgSubject);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_LOCATION, m_dlgLocation);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_ATTENDEES, m_dlgAttendee);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_DTSTART, m_dlgStartDT);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_DTEND, m_dlgEndDT);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_RECURRENCE, m_dlgRecur);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_AALARM, m_dlgAlarm);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_DESCRIPTION, m_dlgDescript);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_STATIC_CATEGORIES, m_dlgCategories);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_EDIT_SUBJECT, m_ECtrlSubject);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_EDIT_LOCATION, m_ECtrlLocation);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_EDIT_ATTENDEES, m_ECtrlAttendee);
		DDX_Control(pDX, IDC_APPOINTMENTEDITOR_EDIT_DESCRIPTION, m_ECtrlDescription);
	}


	BEGIN_MESSAGE_MAP(CAppointmentEditor, CQDialog)
		ON_BN_CLICKED(IDCANCEL, &CAppointmentEditor::OnBnClickedCancel)
		ON_BN_CLICKED(IDOK, &CAppointmentEditor::OnBnClickedOk)
		ON_NOTIFY(DTN_DATETIMECHANGE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, &CAppointmentEditor::OnBtnChangeTStart)
		ON_NOTIFY(DTN_DATETIMECHANGE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME, &CAppointmentEditor::OnBtnChangeTEnd)
		ON_NOTIFY(DTN_CLOSEUP, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, &CAppointmentEditor::OnDtnCloseupDStart)
		ON_NOTIFY(DTN_CLOSEUP, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, &CAppointmentEditor::OnDtnCloseupDEnd)
		ON_WM_SIZING()
	END_MESSAGE_MAP()


	// CAppointmentEditor message handlers
	CString CAppointmentEditor::GetString(const TSTRING& strID)
	{
		return m_pILocale->GetString(strID).c_str();
	}

	bool CAppointmentEditor::GetXMLString(const TSTRING& strSettingsID, vector<CString>& vCString)
	{
		vCString.clear();
		bool bRes = false;
		if(!m_pIPIMConfig)
			m_pIPIMConfig= (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("Phonebook"), eIPIMConfigure);
		if(m_pIPIMConfig)
		{
			TSTRING strValue;
			m_pIPIMConfig->GetProperty(_T("CalenderConfig"),strSettingsID, strValue);
			vector<TSTRING> vstrValue;
			if(!strValue.empty() && Tokenize<TSTRING>(_T("|"), strValue, vstrValue, false))
			{
				for(vector<TSTRING>::iterator iter = vstrValue.begin(); iter!= vstrValue.end(); ++iter)
				{
					vCString.push_back(m_pILocale->GetString((*iter)).c_str());
				}
				bRes = true;
			}
		}
		return bRes;
	}


	BOOL CAppointmentEditor::OnInitDialog()
	{
		CQDialog::OnInitDialog();

		m_dlgSubject.SetWindowText(GetString(_T("IDS_Cal_Subject")));
		m_dlgLocation.SetWindowText(GetString(_T("IDS_Cal_Location")));
		m_dlgAttendee.SetWindowText(GetString(_T("IDS_Cal_Attende")));
		m_dlgStartDT.SetWindowText(GetString(_T("IDS_Cal_StartDT")));
		m_dlgEndDT.SetWindowText(GetString(_T("IDS_Cal_EndDT")));
		m_dlgRecur.SetWindowText(GetString(_T("IDS_Cal_Recur")));
		m_dlgAlarm.SetWindowText(GetString(_T("IDS_Cal_Alarm")));
		m_dlgDescript.SetWindowText(GetString(_T("IDS_Cal_Descript")));
		m_dlgCategories.SetWindowText(GetString(_T("IDS_Cal_Categories")));

		//fill vector
		GetXMLString(_T("Alarm"),	m_vStrAlarm);
		GetXMLString(_T("Categories"), m_vStrCate);
		GetXMLString(_T("Recurrence"), m_vStrRecur);

		m_pAlarmComBoCtrl = (CComboBox*) GetDlgItem(IDC_APPOINTMENTEDITOR_COMBO_AALARMTIME);
		for(vector<CString>::iterator iter = m_vStrAlarm.begin(); iter != m_vStrAlarm.end(); iter++)
			m_pAlarmComBoCtrl->AddString((*iter));
		m_pAlarmComBoCtrl->SetCurSel(0); 

		m_pRecurrenceComBoCtrl = (CComboBox*) GetDlgItem(IDC_APPOINTMENTEDITOR_COMBO_RECURRENCE);
		for(vector<CString>::iterator iter = m_vStrRecur.begin(); iter != m_vStrRecur.end(); iter++)
			m_pRecurrenceComBoCtrl->AddString((*iter));
		m_pRecurrenceComBoCtrl->SetCurSel(0);	

		m_pCategoriesComBoCtrl = (CComboBox*) GetDlgItem(IDC_APPOINTMENTEDITOR_COMBO_CATEGORIES);
		for(vector<CString>::iterator iter = m_vStrCate.begin(); iter != m_vStrCate.end(); iter++)
			m_pCategoriesComBoCtrl->AddString((*iter));
		m_pCategoriesComBoCtrl->SetCurSel(0);

		((CDateTimeCtrl*)GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE))->SetFormat(_T("yyyy/MM/dd"));
		((CDateTimeCtrl*)GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE))->SetFormat(_T("yyyy/MM/dd"));
		((CDateTimeCtrl*)GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME))->SetFormat(_T("tt hh:mm:ss"));
		((CDateTimeCtrl*)GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME))->SetFormat(_T("tt hh:mm:ss"));
		((CDateTimeCtrl*)GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE))->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));
		((CDateTimeCtrl*)GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE))->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));

		BOOL BRet = TRUE;
		if(!m_bIsNew) 
			BRet = SetEditBox();	
		else if(m_bIsNew)
		{
			BRet = SetNewPIMObj() ? TRUE : FALSE;
			ClearEditBox();
		}
		SetXMLUI();
		SetUILimitation();
		UpdateData(FALSE);
		return BRet;
	}
	void CAppointmentEditor::SetXMLUI()
	{
		SetBackground();
		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		Widge<HDC>* pWidget = pLayoutManager->Load(_T("Cal_AppointmentEditorID"));
		m_pWinCloseButton = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
		m_pBtnCancel = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_1"));
		m_pBtnOK = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_2"));
		SetLayout(pWidget);
		delete pLayoutManager;
	}
	void CAppointmentEditor::SetUILimitation()
	{// Calendar only needs to restrict the number of words.
		IPIMObjPt PIMObjPt; // Empty PIMObj (cause we won't use it)
		CVCalCtrl VCalCtrl(PIMObjPt);

		SetLimit(&m_ECtrlSubject, &VCalCtrl, _T(KEYWORD_SUBJECT));
		SetLimit(&m_ECtrlLocation, &VCalCtrl, _T(KEYWORD_LOCATION));
		SetLimit(&m_ECtrlDescription, &VCalCtrl, _T(KEYWORD_DESCRIPTION));
		SetLimit(&m_ECtrlAttendee, &VCalCtrl, _T(KEYWORD_ATTENDEE));
		SetDateRange(_T(KEYWORD_DTSTART), IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE);
		SetDateRange(_T(KEYWORD_DTEND), IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE);
	}
	void CAppointmentEditor::SetLimit(CQEditCtrl* pEditCtrl, CVCalCtrl* pVCalCtrl, const TSTRING& strKey)
	{
		int nLimit = pVCalCtrl->GetLimitation(strKey);
		if(nLimit != -1)
			pEditCtrl->SetTextLength(nLimit);
	}
	void CAppointmentEditor::OnDtnCloseupDStart(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

		COleDateTime oleDTStart, oleDTEnd;
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, oleDTStart);
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);

		if(oleDTStart < COleDateTime::GetCurrentTime())
		{
			ShowMessage(GetSafeHwnd(),GetString(_T("IDS_Appointment_MsgCurrentTime")),GetString(_T("IDS_CALENDAR_FUNCTION")));
			SetCurrentTimeCtrl();
			GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, oleDTStart);
			GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);
		}
		else
		{
			if(oleDTStart < oleDTEnd)
				oleDTEnd = oleDTStart + m_DTInterval;
			else
			{
				COleDateTimeSpan spanTime( 0, 1, 0, 0 );
				oleDTEnd = oleDTStart + spanTime;
			}
			SetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);
		}
		*pResult = 0;
	}

	void CAppointmentEditor::OnDtnCloseupDEnd(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

		COleDateTime oleDTStart, oleDTEnd;
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, oleDTStart);
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);

		if(oleDTStart > oleDTEnd)
		{
			COleDateTimeSpan spanTime( 0, 1, 0, 0 );
			oleDTEnd = oleDTStart + spanTime;
			SetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);
		}
		m_DTInterval = oleDTEnd - oleDTStart;
		*pResult = 0;
	}



	void CAppointmentEditor::OnBtnChangeTStart(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
		COleDateTime oleDTStart, oleDTEnd;
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, oleDTStart);
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);

		if(oleDTStart < COleDateTime::GetCurrentTime())
		{
			ShowMessage(GetSafeHwnd(),GetString(_T("IDS_Appointment_MsgCurrentTime")),GetString(_T("IDS_CALENDAR_FUNCTION")));
			SetCurrentTimeCtrl();
		}
		else
		{
			if(oleDTStart < oleDTEnd)
				oleDTEnd = oleDTStart + m_DTInterval;
			else
			{
				COleDateTimeSpan spanTime( 0, 1, 0, 0 );
				oleDTEnd = oleDTStart + spanTime;
			}
			SetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);
		}

		*pResult = 0;
	}

	void CAppointmentEditor::OnBtnChangeTEnd(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
		COleDateTime oleDTStart, oleDTEnd;
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, oleDTStart);
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);
		if(oleDTStart > oleDTEnd)
		{
			COleDateTimeSpan spanTime( 0, 1, 0, 0 );
			oleDTEnd = oleDTStart + spanTime;
			SetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);
		}
		m_DTInterval = oleDTEnd - oleDTStart;

		*pResult = 0;
	}
	void CAppointmentEditor::OnBnClickedOk()
	{
		if(CheckDataIsCorrect())
		{
			SaveEditBox();
			OnOK();
		}
	}
	void CAppointmentEditor::OnBnClickedCancel()
	{
		OnCancel();
	}

	bool CAppointmentEditor::CheckDataIsCorrect()
	{
		UpdateData(TRUE);
		COleDateTime oleDTStart, oleDTEnd;
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME, oleDTStart);
		GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME,oleDTEnd);

		bool bTimeCompare = true;
		if(oleDTEnd < oleDTStart)
		{
			ShowMessage(GetSafeHwnd(),GetString(_T("IDS_Appointment_MsgDTEndBeforeDTStart")), GetString(_T("IDS_CALENDAR_FUNCTION")));
			bTimeCompare = false;
		}
		CString strNoSpace = m_szSubject;
		strNoSpace.Remove(' ');
		if( strNoSpace.GetLength() == 0 )
		{
			ShowMessage(GetSafeHwnd(),GetString(_T("IDS_Appointment_MsgEmptySubject")),GetString(_T("IDS_CALENDAR_FUNCTION")));
			bTimeCompare = false;
		}
		return bTimeCompare;
	}


	void CAppointmentEditor::ClearEditBox()
	{
		m_szSubject = _T("");
		m_szLocation = _T("");
		m_szAttendees = _T("");
		m_szDescription = _T("");

		SetCurrentTimeCtrl();

		m_pAlarmComBoCtrl->SetCurSel(0); 
		m_pRecurrenceComBoCtrl->SetCurSel(0);	
		m_pCategoriesComBoCtrl->SetCurSel(0);
	}

	BOOL CAppointmentEditor::SetEditBox()
	{
		BOOL bRet =TRUE;
		if(m_pListData)
		{
			m_pListData->GetCmnData(KEYWORD_SUBJECT, m_szSubject);
			m_pListData->GetCmnData(KEYWORD_LOCATION, m_szLocation);
			m_pListData->GetCmnData(KEYWORD_ATTENDEE, m_szAttendees);
			m_pListData->GetCmnData(KEYWORD_DESCRIPTION, m_szDescription);
			SetTimeCtrl(KEYWORD_DTSTART , IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE);
			SetTimeCtrl(KEYWORD_DTSTART , IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME);
			SetTimeCtrl(KEYWORD_DTEND , IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE);
			SetTimeCtrl(KEYWORD_DTEND , IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME);
			

			COleDateTime oleDTStart, oleDTEnd;
			CString strDTStart, strDTEnd, strAlarm, strRule, strCate = _T("");
			m_pListData->GetCmnData(KEYWORD_DTSTART, strDTStart);
			m_pListData->GetCmnData(KEYWORD_DTEND, strDTEnd);
			m_pListData->GetCmnData(KEYWORD_DALARM, strAlarm);
			m_pListData->GetCmnData(KEYWORD_RECURRENCE, strRule);
			m_pListData->GetCmnData(KEYWORD_CATEGORIES, strCate);

			StrToTime(strDTStart, oleDTStart);
			StrToTime(strDTEnd, oleDTEnd);
			m_DTInterval = oleDTEnd - oleDTStart;

			if(MinusTime(strDTStart,strAlarm) == 8)
				m_pAlarmComBoCtrl->AddString(GetString(_T("IDS_AlarmOther")));
			m_pAlarmComBoCtrl->SetCurSel(MinusTime(strDTStart,strAlarm));		

			CString strRuleTemp = strRule;
			if(m_pListData->GetRule(strRuleTemp) == 5)
				m_pRecurrenceComBoCtrl->AddString(GetString(_T("IDS_RecurOther")));
			m_pRecurrenceComBoCtrl->SetCurSel(m_pListData->GetRule(strRule));
			m_pCategoriesComBoCtrl->SetCurSel(m_pListData->GetCategories((strCate), true));
		}
		else
		{
			bRet = FALSE;
		}
		return bRet;
	}

	void CAppointmentEditor::SetDateRange(const TSTRING& strKey, int nID)
	{
		CVCalCtrl VCtrl(m_pListData->GetPIMObj());
		TSTRING strLimitation, strFirst, strSecond;
		VCtrl.GetLimitation(strKey, strLimitation);
		Split<TSTRING>(_T("-"), strLimitation, strFirst, strSecond);
		COleDateTime oleFirst, oleSecond;
		StrToTime(strFirst.c_str(), oleFirst);
		StrToTime(strSecond.c_str(), oleSecond);
		((CDateTimeCtrl*) GetDlgItem(nID))->SetRange(&oleFirst, &oleSecond);
	}
	bool CAppointmentEditor::SaveEditBox()
	{
		CVCalCtrl VCalCtrl(m_pListData->GetPIMObj());
		bool bRet = true;
		bool bSave[9] = {true};
		bSave[0] = VCalCtrl.SaveCmnData(KEYWORD_SUBJECT,  m_szSubject);
		bSave[1] = VCalCtrl.SaveCmnData(KEYWORD_LOCATION, m_szLocation);
		bSave[2] = VCalCtrl.SaveCmnData(KEYWORD_ATTENDEE, m_szAttendees);
		bSave[3] = VCalCtrl.SaveCmnData(KEYWORD_DESCRIPTION, m_szDescription);
		bSave[4] = VCalCtrl.SaveCmnData(KEYWORD_DTSTART, GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME));
		bSave[5] = VCalCtrl.SaveCmnData(KEYWORD_DTEND,GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME));	
		bSave[6] = VCalCtrl.SaveCmnData(KEYWORD_DALARM, GetAlarmTime(m_pAlarmComBoCtrl->GetCurSel()));
		bSave[7] = VCalCtrl.SaveCmnData(KEYWORD_RECURRENCE, GetRule(m_pRecurrenceComBoCtrl->GetCurSel()));

		CString strCate = m_vStrCate[m_pCategoriesComBoCtrl->GetCurSel()];
		m_pListData->GetCategories((strCate), false);
		bSave[8] = VCalCtrl.SaveCmnData(KEYWORD_CATEGORIES, strCate);

		for(int i=0; i<9 && bRet; i++)
		{
			bRet = bRet && bSave[i];
		}
		return bRet;
	}

	bool CAppointmentEditor::SetNewPIMObj() 
	{ 
		CVCalCtrl VCalCtrl(m_pListData->GetPIMObj());
		return VCalCtrl.SetEmptyIPIMObjPt();
	}


	void CAppointmentEditor::SetCurrentTimeCtrl()
	{
		COleDateTimeSpan spanTime( 0, 1, 0, 0 );
		COleDateTime oleCurDT(COleDateTime::GetCurrentTime());
		oleCurDT.SetDateTime(oleCurDT.GetYear(), oleCurDT.GetMonth(), oleCurDT.GetDay(), oleCurDT.GetHour(), 0, 0);

		oleCurDT += spanTime;
		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE);
		pDTCtrl->SetTime(oleCurDT);
		pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME);
		pDTCtrl->SetTime(oleCurDT);

		oleCurDT += spanTime;
		pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDDATE);
		pDTCtrl->SetTime(oleCurDT);
		pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTENDTIME);
		pDTCtrl->SetTime(oleCurDT);
		m_DTInterval = spanTime;
	}
	bool CAppointmentEditor::SetTimeCtrl(const char* strKey, int nID)
	{
		bool bRet = true;
		CString strCmnData;
		bRet = m_pListData->GetCmnData(strKey, strCmnData);
		COleDateTime oleUTCDT, oleLocalDT;
		if(bRet && StrToTime(strCmnData, oleUTCDT))
		{
			ConvertTime(oleUTCDT, oleLocalDT, true);
			CDateTimeCtrl* pDateCtrl = (CDateTimeCtrl*) GetDlgItem(nID);
			bRet = pDateCtrl->SetTime(oleLocalDT) == TRUE;
		}
		assert( StrToTime(strCmnData, oleUTCDT) == true);
		return bRet;
	}

	CString CAppointmentEditor::GetTimeCtrl(int nDateID, int nTimeID)
	{
		CString strDT = _T("");
		COleDateTime oleDT, oleUTC;

		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(nDateID);
		pDTCtrl->GetTime(oleDT);
		ConvertTime(oleDT, oleUTC, false);
		strDT += oleUTC.Format(_T("%Y%m%dT"));

		pDTCtrl = (CDateTimeCtrl*) GetDlgItem(nTimeID);
		pDTCtrl->GetTime(oleDT);
		ConvertTime(oleDT, oleUTC, false);
		strDT += oleUTC.Format(_T("%H%M%SZ"));

		return strDT;
	}
	void CAppointmentEditor::SetTimeCtrl(int nDateID, int nTimeID,const COleDateTime& oleDT)
	{
		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(nDateID);
		pDTCtrl->SetTime(oleDT);
		pDTCtrl = (CDateTimeCtrl*) GetDlgItem(nTimeID);
		pDTCtrl->SetTime(oleDT);
	}
	void CAppointmentEditor::GetTimeCtrl(int nDateID, int nTimeID, COleDateTime& oleDT)
	{
		COleDateTime oleDate, oleTime;
		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(nDateID);
		pDTCtrl->GetTime(oleDate);

		pDTCtrl = (CDateTimeCtrl*) GetDlgItem(nTimeID);
		pDTCtrl->GetTime(oleTime);
		oleDT.SetDateTime(oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());
	}
	// Merlin Interface
	CString CAppointmentEditor::GetRule(int nRuleSel)  
	{
		COleDateTime oleDT;
		CString strDTStart = GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME);
		QSYNC_LIB::StrToTime(strDTStart, oleDT);
		CString strRule = _T("");
		if(nRuleSel == 0)
			strRule =  _T("");
		else if(nRuleSel == 1)
		{
			strRule = StrConnect<CString>(_T("  "), RRULE_TYPE_DAILY, _T("#0"), false);
		}
		else if(nRuleSel == 2)
		{
			CString strWeekDay = oleDT.Format(_T("%a"));
			strWeekDay = strWeekDay.Mid(0,2);
			strRule = StrConnect<CString>(_T(" "), RRULE_TYPE_WEEKLY, strWeekDay.MakeUpper(), _T("#0"), false);
		}
		else if(nRuleSel == 3)
		{
			strRule = StrConnect<CString>(_T(" "), RRULE_TYPE_MONTHLY, oleDT.Format(_T("%d")), _T("#0"), false);
		}
		else if(nRuleSel == 4)
		{
			strRule = StrConnect<CString>(_T(" "), RRULE_TYPE_YEARLY, oleDT.Format(_T("%m")), _T("#0"), false);
		}
		else
		{
			CVCalCtrl VCalCtrl(m_pListData->GetPIMObj());
			VCalCtrl.GetCmnStrData(KEYWORD_RECURRENCE, strRule);
		}
		return strRule;
	}
	CString CAppointmentEditor::GetAlarmTime(int nAlarmSel)
	{
		COleDateTime oleDT, oleAlarm;
		CString strDTStart = GetTimeCtrl(IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTDATE, IDC_APPOINTMENTEDITOR_DATETIMEPICKER_DTSTARTTIME);
		QSYNC_LIB::StrToTime(strDTStart, oleDT);
		if(nAlarmSel == 0)
		{
			return _T("");
		}
		else if(nAlarmSel == 1)
		{
			COleDateTimeSpan spanTime( 0, 0, 1, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else if(nAlarmSel == 2)
		{
			COleDateTimeSpan spanTime( 0, 0, 5, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else if(nAlarmSel == 3)
		{
			COleDateTimeSpan spanTime( 0, 0, 10, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else if(nAlarmSel == 4)
		{
			COleDateTimeSpan spanTime( 0, 0, 15, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else if(nAlarmSel == 5)
		{
			COleDateTimeSpan spanTime( 0, 0, 30, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else if(nAlarmSel == 6)
		{
			COleDateTimeSpan spanTime( 0, 1, 0, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else if(nAlarmSel == 7)
		{
			COleDateTimeSpan spanTime( 1, 0, 0, 0 );
			oleAlarm = oleDT - spanTime;
		}
		else
		{
			CVCalCtrl VCalCtrl(m_pListData->GetPIMObj());
			CString strAlarm;
			VCalCtrl.GetCmnStrData(KEYWORD_DALARM, strAlarm);
			QSYNC_LIB::StrToTime(strAlarm, oleDT);
		}

		return oleAlarm.Format(_T("%Y%m%dT%H%M%SZ"));
	}
	void CAppointmentEditor::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}

	void CAppointmentEditor::SetBackground()
	{
		TSTRING strBkImage = L"Calendar\\Background\\BK_CalendarEdit.png";
		SetBKImage(strBkImage,BK_Fixed);

		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcWindow;
		GetWindowRect(&rcWindow);

		int nDeltaWidth = rcWindow.Width() - rcClient.Width();
		int nDeltaHeight = rcWindow.Height() - rcClient.Height();
		const int nDlgWIDTH = GetBKImage()->GetWidth(); 
		const int nDlgHEIGHT = GetBKImage()->GetHeight() ;

		SetWindowPos(NULL, 0, 0, nDlgWIDTH + nDeltaWidth, nDlgHEIGHT + nDeltaHeight, SWP_NOMOVE);

	}
	//Button function:

	void CAppointmentEditor::WinClose()
	{
		g_pThis->FnWinClose();
	}
	void CAppointmentEditor::FnWinClose()
	{
		PostMessage(WM_CLOSE);
	}
	void CAppointmentEditor::Cancel()
	{
		g_pThis->FnWinClose();
	}
	void CAppointmentEditor::OK()
	{
		g_pThis->OnBtnClickOK();
	}
	void CAppointmentEditor::OnBtnClickOK()
	{
		if(CheckDataIsCorrect())
		{
			SaveEditBox();
			OnOK();
		}
	}
}