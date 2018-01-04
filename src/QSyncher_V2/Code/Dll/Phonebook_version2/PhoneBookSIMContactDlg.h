/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                               3G Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhoneBookSIMContactDlg.h  $
 *  $Revision:   1.15  $
 *  $Date:   Mar 26 2008 16:42:52  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhoneBookSIMContactDlg.h.-arc  $
 * 
 *    Rev 1.15   Mar 26 2008 16:42:52   Alan Tu
 * OnBnClickedOK
 * 
 *    Rev 1.14   Mar 26 2008 16:32:24   Alan Tu
 * update
 * 
 *    Rev 1.13   Feb 27 2008 17:36:14   Jame Tsai
 * Add SetXML
 * 
 *    Rev 1.12   Feb 04 2008 09:07:08   Mei Yang
 * add ExistSameName validation
 * 
 *    Rev 1.11   Feb 01 2008 17:14:22   Mei Yang
 * update name
 * 
 *    Rev 1.10   Feb 01 2008 17:07:30   Mei Yang
 * add vDisplayString
 * 
 *    Rev 1.9   Jan 08 2008 15:44:18   Mei Yang
 * add Phone number limitation
 * 
 *    Rev 1.8   Jan 07 2008 15:50:04   Mei Yang
 * add UILimitation
 * 
 *    Rev 1.7   Jan 07 2008 15:23:20   Mei Yang
 * add "Can't be empty" limitation
 * 
 *    Rev 1.6   Jan 02 2008 14:55:56   Jame Tsai
 * update
 * 
 *    Rev 1.5   Jan 02 2008 13:51:14   Jame Tsai
 * change CDialog to CQDialog
 * 
 *    Rev 1.4   Dec 31 2007 14:33:30   Jame Tsai
 * save type and refactory
 * 
 *    Rev 1.3   Dec 26 2007 16:35:08   Mei Yang
 * unuse bool SaveText(int nID, const char* cpKey)
 * 
 *    Rev 1.2   Dec 10 2007 21:13:54   Mei Yang
 * add Save
 * 
 *    Rev 1.1   Dec 10 2007 20:24:04   Mei Yang
 * update
 * 
 *    Rev 1.0   Dec 10 2007 17:08:48   Eddy Guo
 * Initial revision.
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file PhoneBookSIMContactDlg.h
    @brief {brief description of PhoneBookSIMContactDlg.h}

    {detail description of PhoneBookSIMContactDlg.h}
*/
#ifndef __CPhoneBookSIMContactDlg_HPP_
#define __CPhoneBookSIMContactDlg_HPP_
#pragma once
 
#include "resource.h"
 
#include "..\..\Components\Localization\ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\splitLayout.h"
#include "../../Components/OutlookAccess/IOutlookCtrl.h"
#include "..\..\Components\PIMDB\VCardCtrl.h"
#include "../../Components/UIFramework/Porting/QEditCtrl.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "..\..\CommonUtil\NotifyBase.h"
 
// CPhoneBookSIMContactDlg dialog

namespace QSYNC_LIB
{
	class CPhoneBookSIMContactDlg : public Win32DoubleBufferQDialogWidge,public QSYNC_LIB::CINotifyBase
	{
		DECLARE_DYNAMIC(CPhoneBookSIMContactDlg)
	 
	public:
		CPhoneBookSIMContactDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CPhoneBookSIMContactDlg();
		void Event(const TSTRING& strEvent, long nParam);
	 
		INT_PTR DoModal(const IPIMObjPt& PIMObjPt,const vector<TSTRING>& vDisplayString)
		{
			m_PIMObjPt = PIMObjPt;
			m_vDisplayString.assign(vDisplayString.begin(), vDisplayString.end());
			return CQDialog::DoModal();
		}

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();
		DECLARE_MESSAGE_MAP()
		

	private:
		void SetXMLUI();
		std::wstring GetWString(const TSTRING& strID);
		CString GetString(const TSTRING& strID);
		BOOL SetEditBox();
		void SetOrgText(int nID, const char* cpKey);
		void SetSpecialOrgText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount=0);
		bool SaveEditBox();
		bool IsBothEmpty();
		bool ExistSameName();
		bool SaveText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount = 0);
		void SetUILimitation();
		void SetLimit(CQEditCtrl* pEditCtrl,  CVCardCtrl* pVCardCtrl, const TSTRING& strKey);
		void SetPhoneCheckingWords();

		void SetBackground();

		void FnWinClose();
		void OnBnClickedCancel();
		void OnBnClickedOK();

	private:
		ILocalization*						m_pILocale;
		IPIMObjPt							m_PIMObjPt;
		SplitLayout<HDC>*					m_pLayout;
		WIN32IconButton*					m_pWinCloseButton;
		WIN32IconButton*					m_pBtnCancel;
		WIN32IconButton*					m_pBtnOK;
		CQEditCtrl							m_QEditCtrlPhone;
		vector<TSTRING>						m_vDisplayString;
	public:
		//afx_msg void OnBnClickedOk();
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	};
} // namespace QSYNC_LIB
#endif // __CPhoneBookSIMContactDlg_HPP_