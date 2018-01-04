/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   PhoneBookContactDlg.h  $
*  $Revision:   1.26  $
*  $Date:   Mar 10 2008 12:33:44  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhoneBookContactDlg.h.-arc  $
 * 
 *    Rev 1.26   Mar 10 2008 12:33:44   Jame Tsai
 * change limitation
 * 
 *    Rev 1.25   Feb 04 2008 17:18:36   Mei Yang
 * add ExistSameDisplayName and fix bug
 * 
 *    Rev 1.24   Feb 04 2008 13:58:32   Mei Yang
 * add ExistSameDisplayName
 * 
 *    Rev 1.23   Feb 04 2008 08:40:06   Mei Yang
 * add vDisplayString to DoModal
 * 
 *    Rev 1.22   Jan 24 2008 15:51:34   Jame Tsai
 * Replace new layout
 * 
 *    Rev 1.21   Jan 08 2008 15:35:36   Mei Yang
 * add Phone Keyin Limitation
 * 
 *    Rev 1.20   Jan 07 2008 16:17:06   Mei Yang
 * add "Can't be all empty" Limitation
 * 
 *    Rev 1.19   Jan 04 2008 13:16:46   Mei Yang
 * add m_bIsBDAYSet
 * 
 *    Rev 1.18   Jan 02 2008 13:39:02   Jame Tsai
 * change CDialog to CQDialog
 * 
 *    Rev 1.17   Dec 29 2007 17:04:06   Jame Tsai
 * change buttons
 * 
 *    Rev 1.16   Dec 28 2007 21:44:22   Jame Tsai
 * refactory
 * 
 *    Rev 1.15   Dec 27 2007 21:34:56   Jame Tsai
 * refactory
 * 
 *    Rev 1.14   Dec 26 2007 19:06:42   Mei Yang
 * add SetCallerID and SetBDAY
 * 
 *    Rev 1.13   Dec 26 2007 17:03:50   Mei Yang
 * add one more field
 * 
 *    Rev 1.12   Dec 25 2007 21:53:42   Mei Yang
 * add RestrictWords
 * 
 *    Rev 1.11   Dec 20 2007 17:27:28   Mei Yang
 * add UI Limitation
 * 
 *    Rev 1.10   Dec 17 2007 15:12:40   Mei Yang
 * show CallerID ok
 * 
 *    Rev 1.9   Dec 04 2007 11:34:58   Alan Tu
 * remove outlook library
 * 
 *    Rev 1.8   Dec 04 2007 10:57:48   Alan Tu
 * remove PIM control
 * 
 *    Rev 1.7   Nov 13 2007 10:05:50   Alan Tu
 * update UI
*
*
***************************************************************************/
#ifndef __CPhoneBookContactDlg_HPP_
#define __CPhoneBookContactDlg_HPP_

#pragma once

#include "resource.h"

#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\splitLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\Stacklayout.h"
#include "../../Components/OutlookAccess/IOutlookCtrl.h"
#include "..\..\Components\PIMDB\VCardCtrl.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "../../Components/UIFramework/Porting/QEditCtrl.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"

// CPhoneBookContactDlg dialog
namespace QSYNC_LIB
{
	enum PBContactDlgTab
	{
		General_Personal_Ifo
		,Private_Ifo
		,Business_Ifo
	};
	class CPhoneBookContactDlg : public Win32DoubleBufferQDialogWidge
	{
		DECLARE_DYNAMIC(CPhoneBookContactDlg)

	public:
		CPhoneBookContactDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CPhoneBookContactDlg();
		INT_PTR DoModal(const IPIMObjPt& PIMObjPt, const vector<CPIMGroupInfo>& vectPIMGroup,const vector<TSTRING>& vDisplayString)
		{
			m_PIMObjPt = PIMObjPt;
			m_vectPIMGroup = vectPIMGroup;
			m_vDisplayString = vDisplayString;
			return CQDialog::DoModal();
		}

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();
		DECLARE_MESSAGE_MAP()
	private:
		TSTRING GetString(const TSTRING& strID){return m_pILocale->GetString(strID);}
		std::wstring GetWString(const TSTRING& strID);
		void SetUI();
		void SetUILimitation();
		void SetLimit(CQEditCtrl* pEditCtrl, CVCardCtrl* pVCardCtrl, const TSTRING& strKey);
		bool IsAllEmpty();
		bool IsDisplayNameEmpty(); //順序檢查定義DisplayName的所有欄位,但不檢查DisplayName本身
		bool ExistSameDisplayName();
		TSTRING GetOfficialDisplayName();
		bool IsFieldEmpty(int nID);
		void SetPhoneCheckingWords();

		BOOL SetEditBox();
			void SetCallerID();
			void SetBDAY();
		void SetOrgText(int nID, const char* cpKey);
		void SetSpecialOrgText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount=0);
		//void SetOrgText(int nID, const char* cpKey1, const char* cpKey2);
		bool SaveEditBox();
		bool SaveText(int nID, const char* cpKey);
		bool SaveText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount = 0);
		void SaveDisplayName();
		
		void SetBackground();
		int	GetHeightDifferenceOfWindowAndClient();
		int	GetWidthDifferenceOfWindowAndClient();
		SplitLayout<HDC>* CreateCaptionLayout();
		SplitLayout<HDC>* CreateCLientLayout();
		BoxLayout<HDC>*	CreateButtonLayout();
		SplitLayout<HDC>* CreateTextAreaLayout();
		SplitLayout<HDC>* CreateGerenalInfoLayout();
		SplitLayout<HDC>* CreatePrivateInfoLayout();
		SplitLayout<HDC>* CreatePersonalInfoLayout();
		SplitLayout<HDC>* CreateBusinessInfoLayout();
		BoxLayout<HDC>* CreateTabLayout();


	protected:
		static void WinClose();
		void FnWinClose();
		static void Cancel();
		void OnBnClickedCancel();
		static void OK();
		void OnBnClickedOK();

		static void Tab_GPInfo();
		void OnGPInfo();
		static void Tab_PrivateInfo();
		void OnPrivateInfo();
		static void Tab_BusinessInfo();
		void OnBusinessInfo();

	private:
		ILocalization*				m_pILocale;
		IPIMObjPt					m_PIMObjPt;
		vector<CPIMGroupInfo>		m_vectPIMGroup;
		bool						m_bIsBDAYSet;
		int							m_FontSize;

		CQEditCtrl					m_QEditCtrlPrivatePhone;
		CQEditCtrl					m_QEditCtrlBusinessPhone;
		CQEditCtrl					m_QEditCtrlPrivateCellPhone;
		CQEditCtrl					m_QEditCtrlBusinessCellPhone;
		CQEditCtrl					m_QEditCtrlPrivateFax;
		CQEditCtrl					m_QEditCtrlBusinessFax;

		StackLayout<int,HDC>* m_pIuputStackLayout;
		WIN32IconButton* m_pTabGeneral_Personal;
		WIN32IconButton* m_pTabPrivate;
		WIN32IconButton* m_pTabBusiness;
		WIN32IconButton* m_pCurrentTabBtn;

		vector<TSTRING>				m_vDisplayString;
		CString						m_cstrOrgDisplayName;
		bool						m_bIsOfficialFN;

	public:
		afx_msg void OnDtnChangeContactBday(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
		//afx_msg void OnBnClickedOk();
	};

}
#endif // __CPhoneBookContactDlg_HPP_