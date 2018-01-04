
#include <afxole.h>

#include "..\..\..\code\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/interface/QCommonControlBase.h"
#include "../../../code\Components\UIFramework\Porting\QUIDropTraget.h"
#include "../../../code\Components\UIFramework\Porting\UIOleDataSource.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\LayoutManager.h"
#include "..\..\..\code\Components\ConfigureTool\IConfigure.h"

#define TYPE_1FF	0
#define TYPE_HEX	1
#define TYPE_PARTITION	2
#define TYPE_QCSBL		3
#define	TYPE_QCSBLHD	4
#define TYPE_OEMSBL		5
#define TYPE_OEMSBLHD	6
#define TYPE_AMSS		7
#define TYPE_AMSSHD		8
#define TYPE_CEFS		9
#define	TYPE_DU			10

#pragma once

namespace QSYNC_LIB
{

	// COptionDlg dialog

	class COptionDlg :  public Win32DoubleBufferQDialogWidge, public INotify
	{
		DECLARE_DYNAMIC(COptionDlg)

	public:
		COptionDlg(CString szPath, CWnd* pParent = NULL);   // standard constructor
		virtual ~COptionDlg();

	// Dialog Data
		enum { IDD = IDD_OPTION_DLG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	public:
		virtual void Event(const TSTRING& strEvent,long nParam);
	public:
		virtual BOOL OnInitDialog();

	private:
		SplitLayout<HDC>* m_pMainLayout;
		CLayoutManager* m_pLayoutManager;

		WIN32IconButton* m_pBtn1FF;
		WIN32IconButton* m_pBtnHex;
		WIN32IconButton* m_pBtnPartition;
		WIN32IconButton* m_pBtnQCSBL;
		WIN32IconButton* m_pBtnQCSBLHD;
		WIN32IconButton* m_pBtnOEMSBL;
		WIN32IconButton* m_pBtnOEMSBLHD;
		WIN32IconButton* m_pBtnAMSS;
		WIN32IconButton* m_pBtnAMSSHD;
		WIN32IconButton* m_pBtnCEFS;
		WIN32IconButton* m_pBtnDU;
		WIN32IconButton* m_pBtnOK;
		WIN32IconButton* m_pBtnCancel;

		CString			m_szPath;

		IConfigure *m_pConfigure;

		std::wstring 			m_sz1FFPath;
		std::wstring			m_szHexPath;
		std::wstring			m_szPartitionPath;
		std::wstring			m_szQCSBLPath;
		std::wstring			m_szQCSBLHDPath;
		std::wstring			m_szOEMSBLPath;
		std::wstring			m_szOEMSBLHDPath;
		std::wstring			m_szAMSSPath;
		std::wstring			m_szAMSSHDPath;
		std::wstring			m_szCEFSPath;
		std::wstring			m_szDUPath;
		bool			m_bBackupChecked;
		bool			m_bVersionChecked;

		void SetXMLUI(void);
		void OnOKButton(void);
		void SearchFile(int nFileType);
		bool  GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath);
		bool  SetDLPathToIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath);
	};
}
