/*********************************************************************/
/** @file:	QProgressServer.h

Defines the CQProgressServer and its associated classes.

$Original Author:   Steven CC Chen  $
$Version:	 $
$Date:   Mar 19 2008 19:36:26  $
*/
/*********************************************************************/

/*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/UIFramework/Porting/QProgressServer.h.-arc  $
 * 
 *    Rev 1.22   Mar 19 2008 19:36:26   Alan Tu
 * remove unused
 * 
 *    Rev 1.21   Mar 19 2008 16:06:26   Alan Tu
 * update
 * 
 *    Rev 1.20   Mar 19 2008 15:28:30   Alan Tu
 * refactory
 * 
 *    Rev 1.19   Mar 03 2008 11:36:14   Alan Tu
 * Enable to drag progress dialog
 * 
 *    Rev 1.18   Feb 29 2008 14:32:08   Alan Tu
 * remove mark
 * 
 *    Rev 1.17   Feb 29 2008 14:03:02   Alan Tu
 * remove SetUI
 * 
 *    Rev 1.16   Feb 27 2008 21:08:08   Alan Tu
 * update function with post message
 * 
 *    Rev 1.15   Feb 27 2008 18:04:00   Mei Yang
 * change SetUI->SetXMLUI
 * 
 *    Rev 1.14   Jan 07 2008 13:45:50   Jame Tsai
 * update
 * 
 *    Rev 1.13   Jan 07 2008 09:56:10   Jame Tsai
 * update
 * 
 *    Rev 1.12   Jan 04 2008 19:28:00   Mei Yang
 * I hate CQProgressCtrl m_ctrlQProgress
 * 
 *    Rev 1.11   Jan 04 2008 18:38:26   Mei Yang
 * make it work
 * 
 *    Rev 1.10   Dec 29 2007 00:28:48   Jame Tsai
 * Set type and fix error
 * 
 *    Rev 1.9   Dec 28 2007 20:09:30   Mei Yang
 * add SetUI
 * 
 *    Rev 1.8   Dec 28 2007 11:01:18   Mei Yang
 * add onSizing
 * 
 *    Rev 1.7   Dec 27 2007 20:03:10   Mei Yang
 * add setUI
 * 
 *    Rev 1.6   Dec 25 2007 17:14:06   Alan Tu
 * update
 * 
 *    Rev 1.5   Dec 25 2007 15:59:22   Alan Tu
 * update
* 
*    Rev 1.4   Jul 06 2007 13:40:14   Alan Tu
* update
* 
*    Rev 1.3   Apr 16 2007 18:54:06   Alan Tu
* update
* 
*    Rev 1.2   Apr 16 2007 15:46:04   Alan Tu
* update
* 
*    Rev 1.1   Mar 05 2007 22:23:52   steven cc chen
* Fixed "..\\Resource.h"
* 
*    Rev 1.0   Mar 05 2007 22:02:26   steven cc chen
* Initial revision.
*/

#pragma once

#include "..\CommUIExportAPI.h"
#include "..\Resource.h"
#include "..\Interface\IQProgressClient.h"
#include ".\QProgressCtrl.h"

#include "..\LayoutManager\Win32FrameworkWidge.h"
#include "..\LayoutManager\Win32ProgressBarWidge.h"
#include "..\LayoutManager\BoxLayout.h"
#include "..\LayoutManager\SplitLayout.h"
#include "..\LayoutManager\WIN32IconButtonWidgeVer2.h"
#include "../LayoutManager/LayoutManager.h"

namespace QSYNC_LIB
{

#ifndef QPROGRESS_TOOLTIP_DEFAULT_DELAYTIME
#define QPROGRESS_TOOLTIP_DEFAULT_DELAYTIME				MAKELONG(3000, 0)  //(3000ms)
#endif

#ifndef QPROGRESS_TOOLTIP_DEFAULT_TIPTEXTCOLOR
#define QPROGRESS_TOOLTIP_DEFAULT_TIPTEXTCOLOR			QPROGRESSCTRL_DEFAULT_TEXT_COLOR
#endif

#ifndef QPROGRESS_TOOLTIP_DEFAULT_TIPBKCOLOR
#define QPROGRESS_TOOLTIP_DEFAULT_TIPBKCOLOR			QPROGRESSCTRL_DEFAULT_BG_COLOR
#endif


#define WM_USER_UPDATE WM_USER + 1

	//////////////////////////////////////////////////////////////////////////////////////
	// CQProgressServerDialog (**Here i using DYNCREATE Creation and Self-Destruction, and it supports both Modeless Dialog and Modal Dialog**)

	class CQProgressServerDialog : public Win32DoubleBufferQDialogWidge, public CINotifyBase
	{
		DECLARE_DYNCREATE(CQProgressServerDialog) //(*)

	public:
		CQProgressServerDialog(IQProgressClient* pQProgressClient = NULL, CWnd* pParent = NULL);
		virtual ~CQProgressServerDialog();  //(*please write all your destruction codes in the OwnerDestructor() method instead*)

		// class data members
	private:
		IQProgressClient*		m_pQProgressClient;
		BOOL					m_bIsModal; //(*)
		BOOL					m_bCanAutoCloseAfterTaskComplete;
		CWinThread*				m_pTaskWorkerThread; //(#the one-and-only Worker Thread of this class#)
		BOOL					m_bEnableCancelButton;
		Win32ProgressBarWidge*	m_pProgress;
		WIN32LabelWidge*		m_pTextNearBar;
		WIN32LabelWidge*		m_pText;
		WIN32IconButton*		m_pCancelButton;
		TSTRING					m_strProgressDialogText;
		TSTRING					m_strPercentage;
		TSTRING					m_strCancelText;
		CToolTipCtrl			m_Tooltip;
		WIN32LabelWidge*		m_pCaptionText;
		CString					m_strCurrentDisplayText;
		int							m_nCurrentPosition;

		// class private functions
	private:
		void				OwnerDestructor(void); //(*the real Class Destructor*)
		void				OwnerCloseDialog(void); //(*)
		static UINT			TaskWorkerThreadProc(LPVOID pParam); //(#)
		BOOL				TaskWorkerThreadBeginSuspended(void); //(#)
		BOOL				TaskWorkerThreadResume(void); //(#)
		BOOL				TaskWorkerThreadSuspend(void); //(#)
		void				EnableCancelButton(bool bEnable);
		int					GetHeightDifferenceOfWindowAndClient();
		int					GetWidthDifferenceOfWindowAndClient();
		void				SetUIBackground();

		// class public methods and operations
	public:
		// To implement UI methods
		CString				GetProgressDialogText(void);
		BOOL				SetProgressDialogText(LPCTSTR pszText);
		TSTRING				GetProgressDialogCaption(void);
		void				SetProgressDialogCaption(LPCTSTR pszText);
		CString				GetCancelButtonCaption(void);
		BOOL				SetCancelButtonCaption(LPCTSTR pszText);
		BOOL				GetIsMovableByClientArea(void);
		void				SetIsMovableByClientArea(BOOL bIsMovable);
		BOOL				GetCanAutoCloseAfterTaskComplete(void);
		void				SetCanAutoCloseAfterTaskComplete(BOOL bAutoClose);
		void				SetXMLUI();
		// To implement INotify interface
		virtual void		Event(const TSTRING& strEvent, long nParam);
		virtual void		MouseDown(int nCX,int nCY);
	public:
		virtual INT_PTR		DoModal(); //(*)
		// Windows override functions
	protected:
		virtual void		DoDataExchange(CDataExchange* pDX);
		virtual BOOL		OnInitDialog();
		virtual BOOL		PreTranslateMessage(MSG* pMsg);
		virtual void		PostNcDestroy(); //(*)
	
		// Windows message handlers
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void		OnBnClickedCancel();
		afx_msg void		OnClose();		
		afx_msg void		OnSizing(UINT fwSide, LPRECT pRect);
		LRESULT OnUserUpdate(WPARAM wParam, LPARAM lParam) ;
	};



	//////////////////////////////////////////////////////////////////////////////////////
	// CQProgressServer

	class COMMONUI_API CQProgressServer
	{
	public:
		CQProgressServer();
		virtual ~CQProgressServer();

		// class static functions
	public:
		static BOOL	Exec(IQProgressClient* pQProgressClient,CWnd* pParentWnd = ::AfxGetMainWnd());
	private:		
		static CQProgressServerDialog*& GetServerDialog()
		{
			static CQProgressServerDialog* m_pQProgressServerDialog = NULL;
			return m_pQProgressServerDialog;
		}
	};

}