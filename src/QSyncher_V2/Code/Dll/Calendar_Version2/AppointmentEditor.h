#ifndef __CAppointmentEditor_HPP_
#define __CAppointmentEditor_HPP_

#pragma once
#include "resource.h"
#include "TimeComboCtrl.h"
#include "QDateTimeCtrl.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "../../Components/UIFramework/interface/IQListCtrlUI.h"
#include "../../Components/Localization/ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "QDataCtrl.h"
#include "..\..\Components\PIMDB\VCalCtrl.h"
#include <map>
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "../../Components/UIFramework/Porting/QEditCtrl.h"
#include "..\..\CommonUtil\Win32ThreadUtility.h"

namespace QSYNC_LIB
{
	class CAppointmentEditor : public Win32DoubleBufferQDialogWidge , public INotify

	{
		DECLARE_DYNAMIC(CAppointmentEditor)
	public:
		QSYNC_LIB::ILocalization*	m_pILocale;

		CStatic m_dlgSubject;
		CStatic m_dlgLocation;
		CStatic m_dlgAttendee;
		CStatic m_dlgStartDT;
		CStatic m_dlgEndDT;
		CStatic m_dlgRecur;
		CStatic m_dlgAlarm;
		CStatic m_dlgDescript;
		CStatic m_dlgCategories;
		CString	m_szSubject;
		CString	m_szLocation;
		CString	m_szAttendees;
		CString	m_szDescription;

		CQEditCtrl m_ECtrlSubject;
		CQEditCtrl m_ECtrlLocation;
		CQEditCtrl m_ECtrlAttendee;
		CQEditCtrl m_ECtrlDescription;

		vector<CString> m_vStrAlarm;
		vector<CString> m_vStrRecur;
		vector<CString> m_vStrCate;

	public:
		CAppointmentEditor(CWnd* pParent = NULL);
		virtual ~CAppointmentEditor();
		INT_PTR DoModal(CQPObjListData* pListData, bool bIsNew)
		{
			m_pListData = pListData;
			m_bIsNew = bIsNew;
			INT_PTR nRet = Win32DoubleBufferQDialogWidge::DoModal();
			return nRet;
		}
		CString GetString(const TSTRING& strID);
		bool GetXMLString(const TSTRING& strSettingsID, vector<CString>& vCString);
		virtual void Event(const TSTRING& strEvent,long nParam);
	public:
		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnBtnChangeTStart(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnBtnChangeTEnd(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDtnCloseupDStart(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDtnCloseupDEnd(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL	OnInitDialog();
		DECLARE_MESSAGE_MAP()

	private:
		SplitLayout<HDC>* m_pLayout;
		IPIMCONFIG* m_pIPIMConfig;
		CComboBox* m_pAlarmComBoCtrl;
		CComboBox* m_pRecurrenceComBoCtrl;
		CComboBox* m_pCategoriesComBoCtrl;
		CQPObjListData* m_pListData;
		COleDateTimeSpan m_DTInterval;
		bool m_bIsNew;
		WIN32IconButton* m_pBtnCancel;
		WIN32IconButton* m_pBtnOK;
		WIN32IconButton* m_pWinCloseButton;
	private:
		void SetXMLUI();
		void SetUILimitation();
		void SetDateRange(const TSTRING& strKey, int nID);
		void SetLimit(CQEditCtrl* pEditCtrl, CVCalCtrl* pVCalCtrl, const TSTRING& strKey);
		bool SaveEditBox();
		BOOL SetEditBox();
		void ClearEditBox();
		bool SetNewPIMObj();

		bool SetTimeCtrl(const char* strKey, int nID);
		CString GetTimeCtrl(int nDateID, int nTimeID);
		void GetTimeCtrl(int nDateID, int nTimeID, COleDateTime& oleDT);
		void SetTimeCtrl(int nDateID, int nTimeID, const COleDateTime& oleDT);
		void SetCurrentTimeCtrl();
		bool CheckDataIsCorrect();		
		CString GetAlarmTime(int nAlarmSel);
		CString GetRule(int nRuleSel);

		void SetBackground();

		//Button Function:
		static void WinClose();
		void FnWinClose();
		static void Cancel();
		static void OK();
		void OnBtnClickOK();

	};

}
#endif // __CAppointmentEditor_HPP_