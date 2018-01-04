// CSyncOutlookDlg dialog

#ifndef __CSyncOutlookDlg_HPP_
#define __CSyncOutlookDlg_HPP_

#pragma once

#include "Resource.h"
#include "QDateTimeCtrl.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"

namespace QSYNC_LIB
{
	enum SyncWay {SyncToOL,SyncFromOL};

	class CSyncOutlookDlg : public Win32DoubleBufferQDialogWidge ,public CINotifyBase
	{
		DECLARE_DYNAMIC(CSyncOutlookDlg)

	public:
		CSyncOutlookDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CSyncOutlookDlg();
		virtual INT_PTR DoModal(SyncWay nSyncWay,bool bShowAll,CTime* pPeriodStart,CTime* pPeriodEnd);
		virtual BOOL OnInitDialog();
		void SetXMLUI();
		void Event(const TSTRING& strEvent, long nParam);
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
		void SetCaptionText(const std::wstring& strDisplay);
		void SetItemNumberText(const std::wstring& strDisplay);

	protected:

		WIN32LabelWidge* m_SubcaptionText;
		WIN32LabelWidge* m_ItemNumberText; //   符合條件的約會總數/所有約會總數

		//Button:
		WIN32IconButton* m_pWinCloseButton;
		WIN32IconButton* m_pBtnCancel;
		WIN32IconButton* m_pBtnOK;
		WIN32IconButton* m_pBtnRadioAll;
		WIN32IconButton* m_pBtnRadioStartBetween;


		//Button function:
	public:		
		void OnBnClickedCancel();
		void OnBnClickedAll();
		void OnBnClickedStartBetween();
		void OnBnClickedOK();

		// Dialog Data
		enum { IDD = IDD_SYNCOUTLOOKDLG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	private:
		CTime* m_pPeriodStartAfter;
		CTime* m_pPeriodStartBefore;
		bool m_bShowAll;
		SyncWay m_SyncWay;
		DECLARE_MESSAGE_MAP()
	};

}//end namespace QSYNC_LIB

#endif //__CSyncOutlookDlg_HPP_
