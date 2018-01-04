#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "..\..\CommonUtil\NotifyBase.h"
#pragma once


// CQAboutBox dialog
namespace QSYNC_LIB
{
	class CQAboutBox : public Win32DoubleBufferQDialogWidge,public QSYNC_LIB::CINotifyBase
	{
		DECLARE_DYNAMIC(CQAboutBox)

	public:
		CQAboutBox(CWnd* pParent = NULL);   // standard constructor
		virtual ~CQAboutBox();
		void Event(const TSTRING& strEvent, long nParam);

		// Dialog Data
		enum { IDD = IDD_DIALOG1 };
	public:
		virtual BOOL OnInitDialog();
	private:
		void SetXMLUI();
		void SetBackground();
		void OnBtnOK();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	private:
		WIN32IconButton* m_pButtonOK;
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

		DECLARE_MESSAGE_MAP()
	};
}