// QUALCOMM_IMEI_CODINGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "QUALCOMM_IMEI_CODINGDlg.h"
#include "INITIAL.h"
#include "Global_Variable_Define.h"
//#include "Global_Variable_Declare.h"
#include "PassWord.h"
#include "NV_Item_Define.h"
#include "Config.h"
#include "MobileToolKit.h"
#include "..\SPU_Loader\LoaderExport.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include <atlenc.h>
#include "..\..\CommonUtil\LogPolicy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//#define Public_Key_Length           260
//#define Timeout_Delay_Offline		10*60*1000
//#define Timeout_Delay_SWDownload    10*60*1000
//#define Timeout_Delay_Online        10*60*1000
//#define Timeout_Delay_Isconnected   10*60*1000
//#define Timeout_Delay_Online_FTM    10*60*1000
//#define IMEI_LENGTH                 15
//#define MAL_LENGTH                  15
//#define BT_LENGTH                   12
//#define GSN_LENGTH                  13
//#define MSN_LENGTH                  10
//#define Online_Mode                 0
//#define FTM_Mode                    1
//#define Reset                       1
//#define No_Reset                    0
//#define SIZE_NV_SEC_CODE_I          6
//#define SIZE_NV_IMEI_I              9
//#define SIZE_NV_BT_I                6
//#define SIZE_NV_DSN_I               16
//#define SIZE_NV_FACTORY_BLOCK_I     128
#define Dummy_IMEI                  "000000011234560"//"123456789012345","449281960000000"
#define Zero_IMEI                   "000000000000000"
//#define Default_SPC_Code            "000000"
//#define Factory_Read                0
//#define Factory_Write               1
//#define Mobile_ID_Size              11
//#define HW_Version_Size             5
//#define SW_Version_Size             4 
//#define MAXID                       3500
//#define LOG                         1
//#define UNLOG                       0
//#define SIM_Data_For_Phone_Length   576
//#define SIM_Data_From_PICS_Length   1024
//#define SIM_Data_From_PICS_Length   576
//#define Lock_SPC_code               "999999" 
#define WM_TERMINATETHREAD          WM_USER+100
//#define WM_FUNCTIONPROCESS          WM_USER+101
//function frpcess define
//#define CHECK_IMEI                    1
//#define OPEN_SPC                      2
//#define WRITE_IMEIBT                  3
//#define UPDATE_IMEISTATUS             4
//#define WRITE_SIMDATA                 5
//#define WRITE_FSNFREEZE               6
//#define WRITE_SPC                     7  
//#define CREATE_IMEI_RECORD            8 
//#define CHECK_NAL_PN                  9
//#define CHECK_PROCESS                 11
//#define WRITE_SIMDATA_BT                 12
//#define SIM_TYPE_SIRIUS_L55           0
//#define SIM_TYPE_SIRIUS_L55_NEW_CRYPT 1
/////////////////////////   
//CString TIME;            
//HWND Global_WND;//for test
CQUALCOMM_IMEI_CODINGDlg *Global_This;

typedef UINT (*_AfxIMEICodingThread)(LPVOID pParam);
_AfxIMEICodingThread			 pAfxIMEICodingThread=NULL;
extern UINT AfxIMEICodingThread(LPVOID pParam);

#pragma warning(disable:4996)

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGDlg dialog

CQUALCOMM_IMEI_CODINGDlg::CQUALCOMM_IMEI_CODINGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQUALCOMM_IMEI_CODINGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQUALCOMM_IMEI_CODINGDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szIMEIType = _T("");
	m_szBRTBackup1Path = _T("");
}

void CQUALCOMM_IMEI_CODINGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQUALCOMM_IMEI_CODINGDlg)
	DDX_Control(pDX, IDC_EDIT_SO, m_SO_EDIT);
	DDX_Control(pDX, IDC_STATIC_SO1, m_SO);
	DDX_Control(pDX, IDC_STATIC_TRANS2, m_TRANS2);
	DDX_Control(pDX, IDC_STATIC_TRANS, m_TRANS1);
	DDX_Control(pDX, IDC_STATIC_PICASSO, m_PICASSO);
	DDX_Control(pDX, IDC_EDIT_PICASSO, m_PICASSO_EDIT);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_DUCK2, m_TIME_STAMP_DUCK2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_DUCK, m_TIME_STAMP_DUCK1);
	DDX_Control(pDX, IDC_STATIC_KB_DUCK, m_KB_DUCK);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_DUCK2, m_FILE_SIZE_DUCK2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_DUCK, m_FILE_SIZE_DUCK1);
	DDX_Control(pDX, IDC_STATIC_DUCK2, m_DUCK2);
	DDX_Control(pDX, IDC_STATIC_DUCK, m_DUCK1);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_CONFIG);
	DDX_Control(pDX, IDC_BUTTON_CHANGE, m_CHANGE);
	DDX_Control(pDX, IDC_STATIC_IMEI_MAL, m_IMEI_MAL);
	DDX_Control(pDX, IDC_STATIC_IMEI_PART, m_IMEI_PART);
	DDX_Control(pDX, IDC_STATIC_MAL_PART, m_MAL_PART);
	DDX_Control(pDX, IDC_EDIT_MAL_PART, m_MAL_PART_EDIT);
	DDX_Control(pDX, IDC_EDIT_IMEI_PART, m_IMEI_PART_EDIT);
	DDX_Control(pDX, IDC_STATIC_LOCK_INDICATOR, m_LOCK);
	DDX_Control(pDX, IDC_CHECK_SIM, m_SIM);
	DDX_Control(pDX, IDC_CHECK_USER_SIM, m_USER_SIM);
	DDX_Control(pDX, IDC_CHECK_SERVICE_PROVIDER, m_SERVICE_PROVIDER);
	DDX_Control(pDX, IDC_CHECK_NETWORK_SUBSET, m_NETWORK_SUBSET);
	DDX_Control(pDX, IDC_CHECK_NETWORK, m_NETWORK);
	DDX_Control(pDX, IDC_CHECK_CORPORATE, m_CORPORATE);
	DDX_Control(pDX, IDC_STATIC_MAL, m_MAL);
	DDX_Control(pDX, IDC_EDIT_MAL, m_MAL_EDIT);
	DDX_Control(pDX, IDC_STATIC_MOBILE_ID2, m_MOBILEID2);
	DDX_Control(pDX, IDC_STATIC_MOBILE_ID, m_MOBILEID1);
	DDX_Control(pDX, IDC_STATIC_KB_CSV, m_KB_CSV);
	DDX_Control(pDX, IDC_BUTTON_START, m_START);
	DDX_Control(pDX, IDC_BUTTON_LOG, m_LOGBUTTON);
	DDX_Control(pDX, IDC_BUTTON_EXIT2, m_EXIT);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_LIST);
	DDX_Control(pDX, IDC_PROGRESS, m_PROGRESS);
	DDX_Control(pDX, IDC_STATIC_BACKUP_TYPE2, m_BACKUP_TYPE2);
	DDX_Control(pDX, IDC_STATIC_BACKUP_TYPE, m_BACKUP_TYPE1);
	DDX_Control(pDX, IDC_STATIC_CSV2, m_CSV2);
	DDX_Control(pDX, IDC_STATIC_CSV, m_CSV1);
	DDX_Control(pDX, IDC_STATIC_QZCS2, m_QZCS2);
	DDX_Control(pDX, IDC_STATIC_QZCS, m_QZCS1);
	DDX_Control(pDX, IDC_STATIC_QVCEFS2, m_QVCEFS2);
	DDX_Control(pDX, IDC_STATIC_QVCEFS, m_QVCEFS1);
	DDX_Control(pDX, IDC_STATIC_QSWINFO2, m_QSWINFO2);
	DDX_Control(pDX, IDC_STATIC_QSWINFO_SIMDATA, m_QSWINFO_SIMDATA);
	DDX_Control(pDX, IDC_STATIC_QSWINFO, m_QSWINFO1);
	DDX_Control(pDX, IDC_STATIC_QSNV2, m_QSNV2);
	DDX_Control(pDX, IDC_STATIC_QSNV, m_QSNV1);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_PROGRESS_STATIC);
	DDX_Control(pDX, IDC_STATIC_PPF, m_PPF);
	DDX_Control(pDX, IDC_STATIC_PN2, m_PN2);
	DDX_Control(pDX, IDC_STATIC_PN, m_PN1);
	DDX_Control(pDX, IDC_STATIC_PERCENT, m_PERCENT);
	DDX_Control(pDX, IDC_STATIC_PC2, m_PC2);
	DDX_Control(pDX, IDC_STATIC_PC, m_PC1);
	DDX_Control(pDX, IDC_STATIC_MODE2, m_MODE2);
	DDX_Control(pDX, IDC_STATIC_MODE, m_MODE1);
	DDX_Control(pDX, IDC_STATIC_MECHANICAL2, m_MECHANICAL2);
	DDX_Control(pDX, IDC_STATIC_MECHANICAL, m_MECHANICAL1);
	DDX_Control(pDX, IDC_STATIC_KB_SIMDATA, m_KB_SIMDATA);
	DDX_Control(pDX, IDC_STATIC_KB_QZCS, m_KB_QZCS);
	DDX_Control(pDX, IDC_STATIC_KB_QVCEFS, m_KB_QVCEFS);
	DDX_Control(pDX, IDC_STATIC_KB_QSWINFO, m_KB_QSWINFO);
	DDX_Control(pDX, IDC_STATIC_KB_QSNV, m_KB_QSNV);
	DDX_Control(pDX, IDC_STATIC_COM, m_COM);
	DDX_Control(pDX, IDC_STATIC_SIMDATA, m_SIMDATA1);
	DDX_Control(pDX, IDC_STATIC_SIMDATA2, m_SIMDATA2);
	DDX_Control(pDX, IDC_STATIC_SW_HW_ME, m_SW_HW_ME);
	DDX_Control(pDX, IDC_STATIC_SW, m_SW1);
	DDX_Control(pDX, IDC_STATIC_SW2, m_SW2);
	DDX_Control(pDX, IDC_STATIC_TARGET_MODE_PN, m_TARGET_MODE_PN);
	DDX_Control(pDX, IDC_STATIC_TARGET2, m_TARGET2);
	DDX_Control(pDX, IDC_STATIC_TARGET, m_TARGET1);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_SIMDATA2, m_TIME_STAMP_SIMDATA2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_SIMDATA, m_TIME_STAMP_SIMDATA1);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QZCS2, m_TIME_STAMP_QZCS2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QZCS, m_TIME_STAMP_QZCS1);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QVCEFS2, m_TIME_STAMP_QVCEFS2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QVCEFS, m_TIME_STAMP_QVCEFS1);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QSWINFO2, m_TIME_STAMP_QSWINFO2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QSWINFO, m_TIME_STAMP_QSWINFO1);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QSNV2, m_TIME_STAMP_QSNV2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_QSNV, m_TIME_STAMP_QSNV1);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_CSV2, m_TIME_STAMP_CSV2);
	DDX_Control(pDX, IDC_STATIC_TIME_STAMP_CSV, m_TIME_STAMP_CSV1);
	DDX_Control(pDX, IDC_STATIC_TIME_ELAPSE, m_TIME_ELAPSE);
	DDX_Control(pDX, IDC_STATIC_TIME, m_TIME1);
	DDX_Control(pDX, IDC_STATIC_TIME2, m_TIME2);
	DDX_Control(pDX, IDC_STATIC_TIME3, m_TIME3);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_TITLE);
	DDX_Control(pDX, IDC_EDIT_IMEI, m_IMEI_EDIT);
	DDX_Control(pDX, IDC_STATIC_IMEI, m_IMEI);
	DDX_Control(pDX, IDC_STATIC_HW2, m_HW2);
	DDX_Control(pDX, IDC_STATIC_HW, m_HW1);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_SIMDATA2, m_FILE_SIZE_SIMDATA2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_SIMDATA, m_FILE_SIZE_SIMDATA1);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QZCS2, m_FILE_SIZE_QZCS2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QZCS, m_FILE_SIZE_QZCS1);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QVCEFS2, m_FILE_SIZE_QVCEFS2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QVCEFS, m_FILE_SIZE_QVCEFS1);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QSWINFO2, m_FILE_SIZE_QSWINFO2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QSWINFO, m_FILE_SIZE_QSWINFO1);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QSNV2, m_FILE_SIZE_QSNV2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_QSNV, m_FILE_SIZE_QSNV1);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_CSV2, m_FILE_SIZE_CSV2);
	DDX_Control(pDX, IDC_STATIC_FILE_SIZE_CSV, m_FILE_SIZE_CSV1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQUALCOMM_IMEI_CODINGDlg, CDialog)
	//{{AFX_MSG_MAP(CQUALCOMM_IMEI_CODINGDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, OnButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_EXIT2, OnButtonExit2)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, OnButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnBnClickedButtonLog)
	ON_MESSAGE(WM_TERMINATETHREAD, OnTerminateThread)
	ON_MESSAGE(WM_RESETTESTCOUNT, OnResetTestCount)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGDlg message handlers
CQUALCOMM_IMEI_CODINGDlg::~CQUALCOMM_IMEI_CODINGDlg()
{
	::FreeLibrary(hDll);
	::FreeLibrary(hDll2);
	::FreeLibrary(hDll3);
//	Phone_Ctrl.DisconnectAllServers();//to effectivly release the resource
}

 BOOL CQUALCOMM_IMEI_CODINGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(false);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, true);			// Set big icon
	SetIcon(m_hIcon, false);		// Set small icon
	
	// TODO: Add extra initialization here
	CINITIAL INITIAL_Ctrl;
	if(INITIAL_Ctrl.DoModal()==IDCANCEL)
	{
		CDialog::OnCancel();
		return FALSE;
	}
	//set status bar
	SetStatusBar();

	Set_Ctrl_Property();
	Set_Ctrl_Text();
	Initial_Parameter();

	//Register PICS and SSSO2
	HANDLE hSPU = GetSPULoader(this);
	if(hSPU)
	{
		SPU_RESULT res =SPU_SUCCESS;
		if(LoadScript(hSPU,_T("Initial_Com.script")))
			res =ExecuteCommand(hSPU);
		ReleaseHandle(hSPU);
		if(res != SPU_SUCCESS){
			CDialog::OnCancel();
			return errFlag;
		}
	}
	
	//Mark by Magan Mei
	//if((!SSSO2_Ctrl.Initial_SSSO2Dll())||(!Pics_Interface_Ctrl.Initial_PICSInterfaceDLL()))
	//{
	//	AfxMessageBox("Initial SSS_O2/Picsinterface DLL fail, \nplease regedit the DLL");
	//	CDialog::OnCancel();
	//	return FALSE;
	//}
    
	Global_WND=this->m_hWnd;
    Global_This=this;

   
	//Mark by Magan Mei
	//if(!Initial_IMEIStationDLL())
	//{
	//	CDialog::OnCancel();
	//	return FALSE;
	//}
	ShowPicassoTrans();
	//SetLog();
    SetReady();

/*	if(Model_Name=="ATHB1A")
	{
     char buff[100];
	 //USB ADO Add Get XML Material PN from EXCEL
     CString strPath = "\\\\10.94.5.17\\ATHB1A\\";
	 strPath+=SO_Str;
	 strPath+=".xls";

	 BOOL Binfind;
	 CFileFind file;
	 Binfind=file.FindFile(strPath,0);
	 if(!Binfind)
	 {
		 sprintf(buff,"Can't found matching file: %s,\nPlease contact IE/ZhouChao about this issue",strPath);
		 AfxMessageBox(buff);
		 CDialog::OnCancel();
		 return FALSE;
	 }

	 if(!exlapp.CreateDispatch("Excel.Application",NULL)) //启动服务器
	 {
		AfxMessageBox("无法启动Excel服务器!");
		CDialog::OnCancel();
		return FALSE;
	 }
 
//   exlapp.SetVisible(TRUE);//?Excel可见
     exlapp.SetUserControl(TRUE);
	 COleVariant  avar((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
     wbks.AttachDispatch(exlapp.GetWorkbooks(),TRUE);
     lpDisp=wbks.Open(strPath,           
     avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar);

     wbk.AttachDispatch(lpDisp);
     whts.AttachDispatch(wbk.GetWorksheets(),TRUE);
     lpDisp=wbk.GetActiveSheet();
     wht.AttachDispatch(lpDisp);

	 usedRange.AttachDispatch(wht.GetUsedRange(),TRUE);
	 range.AttachDispatch(usedRange.GetRows(),TRUE);
	 long iRowNum=range.GetCount();//已经使用的行数
	 range.AttachDispatch(wht.GetCells(),TRUE);

	 _variant_t va;
	 int i=1;
	 bool check=true;
	 CString ColNo1,ColNo5;

	 do
	 {
		i++;
        va=range.GetItem(_variant_t((long)i),_variant_t((long)2));
	    ColNo1 =(char *)_bstr_t(va);
		ColNo1.Remove(' ');
        va=range.GetItem(_variant_t((long)i),_variant_t((long)5));
	    ColNo5 =(char *)_bstr_t(va);
	    ColNo5.Remove(' ');
        if((ColNo5!="")&&(strncmp(ColNo1,ColNo5,12)!=0||ColNo5.GetLength()!=30))
		{
		   check=false;
           break;
		}
		PNva[i-2]=ColNo5+"\n";
	 }while(i<iRowNum+2);

	 wbks.ReleaseDispatch(); 
	 wbk.ReleaseDispatch(); 
     whts.ReleaseDispatch(); 
     wht.ReleaseDispatch(); 
     range.ReleaseDispatch(); 
     usedRange.ReleaseDispatch(); 
 	 wbk.Close(avar,COleVariant(strPath),avar);
  	 wbks.Close();
	 exlapp.Quit();
	 exlapp.ReleaseDispatch();
    
	 if(!check)
     {
		 sprintf(buff,"The Component %s, is not matching its carton.No %s, \nplease connect IE/Raul",ColNo1,ColNo5);
		 AfxMessageBox(buff);
		 CDialog::OnCancel();
		 return FALSE;
	 }
	}*/
	//USB ADO Add Get XML Material PN from EXCEL

	return true;  // return true  unless you set the focus to a control
}

void CQUALCOMM_IMEI_CODINGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQUALCOMM_IMEI_CODINGDlg::OnPaint() 
{
	Set_Focus();//to effectivly set focus
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQUALCOMM_IMEI_CODINGDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQUALCOMM_IMEI_CODINGDlg::Set_Ctrl_Property()
{
	m_brush.CreateSolidBrush(RGB(200, 200, 100));
    
	CSxLogFont Arial10a(100,FW_BOLD,false,"Arial");
	CSxLogFont Arial10b(90,FW_BOLD,false,"Arial");

    m_START.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_START)));
    m_EXIT.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_EXIT)));
    m_CHANGE.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_CHANGE)));
	m_CONFIG.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_CONFIG)));
	m_LOGBUTTON.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_LOG)));
	m_TITLE.SetBkColor(RGB(200, 200, 100));
	m_TITLE.SetTextColor(RGB(0, 0, 255));
	m_TITLE.SetFontSize(28);
    m_TITLE.SetFontBold(true);
	//*****************************
	m_PPF.SetTextColor(RGB(255,255,255));
	m_PPF.SetBoxColors(RGB(0,0,255));
	m_PPF.SetLineThickness(1,true);
	m_PPF.SetLineStyle(BS_RECT);
	m_PPF.SetFont(&Arial10b);

	m_TARGET_MODE_PN.SetTextColor(RGB(255,255,255));
	m_TARGET_MODE_PN.SetBoxColors(RGB(0,0,255));
	m_TARGET_MODE_PN.SetLineThickness(1,true);
	m_TARGET_MODE_PN.SetLineStyle(BS_RECT);
	m_TARGET_MODE_PN.SetFont(&Arial10b);

	m_QSWINFO_SIMDATA.SetTextColor(RGB(255,255,255));
	m_QSWINFO_SIMDATA.SetBoxColors(RGB(0,0,255));
	m_QSWINFO_SIMDATA.SetLineThickness(1,true);
	m_QSWINFO_SIMDATA.SetLineStyle(BS_RECT);
	m_QSWINFO_SIMDATA.SetFont(&Arial10b);

	m_SW_HW_ME.SetTextColor(RGB(255,255,255));
	m_SW_HW_ME.SetBoxColors(RGB(0,0,255));
	m_SW_HW_ME.SetLineThickness(1,true);
	m_SW_HW_ME.SetLineStyle(BS_RECT);
	m_SW_HW_ME.SetFont(&Arial10b);

	m_TIME_ELAPSE.SetTextColor(RGB(255,255,255));
	m_TIME_ELAPSE.SetBoxColors(RGB(0,0,255));
	m_TIME_ELAPSE.SetLineThickness(1,true);
	m_TIME_ELAPSE.SetLineStyle(BS_RECT);
	m_TIME_ELAPSE.SetFont(&Arial10b);

	m_LOCK.SetTextColor(RGB(255,255,255));
	m_LOCK.SetBoxColors(RGB(0,0,255));
	m_LOCK.SetLineThickness(1,true);
	m_LOCK.SetLineStyle(BS_RECT);
	m_LOCK.SetFont(&Arial10b);
    
	m_IMEI_MAL.SetTextColor(RGB(255,255,255));
	m_IMEI_MAL.SetBoxColors(RGB(0,0,255));
	m_IMEI_MAL.SetLineThickness(1,true);
	m_IMEI_MAL.SetLineStyle(BS_RECT);
	m_IMEI_MAL.SetFont(&Arial10b);

	//m_COM.SetTextColor(RGB(125,0,255));
	//m_COM.SetBoxColors(RGB(125,0,255));
	m_COM.SetTextColor(RGB(255,0,255));
	m_COM.SetBoxColors(RGB(0,0,255));
	m_COM.SetLineThickness(1,true);
	m_COM.SetLineStyle(BS_RECT);
	m_COM.SetFont(&Arial10a);
    //*****************************
	m_QSNV1.SetBkColor(RGB(200, 200, 100));
	m_QSNV1.SetTextColor(RGB(0, 0, 255));
    m_QSNV1.SetFontBold(true);
	m_QSNV2.SetBkColor(RGB(255, 255, 255));
	m_QSNV2.SetTextColor(RGB(125, 0, 255));
	m_QSNV2.SetFontBold(true);
	
	m_QVCEFS1.SetBkColor(RGB(200, 200, 100));
	m_QVCEFS1.SetTextColor(RGB(0, 0, 255));
    m_QVCEFS1.SetFontBold(true);
	m_QVCEFS2.SetBkColor(RGB(255, 255, 255));
	m_QVCEFS2.SetTextColor(RGB(125, 0, 255));
	m_QVCEFS2.SetFontBold(true);

	m_QZCS1.SetBkColor(RGB(200, 200, 100));
	m_QZCS1.SetTextColor(RGB(0, 0, 255));
    m_QZCS1.SetFontBold(true);
	m_QZCS2.SetBkColor(RGB(255, 255, 255));
	m_QZCS2.SetTextColor(RGB(125, 0, 255));
	m_QZCS2.SetFontBold(true);

	m_DUCK1.SetBkColor(RGB(200, 200, 100));
	m_DUCK1.SetTextColor(RGB(0, 0, 255));
    m_DUCK1.SetFontBold(true);
	m_DUCK2.SetBkColor(RGB(255, 255, 255));
	m_DUCK2.SetTextColor(RGB(125, 0, 255));
	m_DUCK2.SetFontBold(true);

	m_TIME_STAMP_QSNV1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_QSNV1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_QSNV1.SetFontBold(true);
	m_TIME_STAMP_QSNV2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_QSNV2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_QSNV2.SetFontBold(true);

	m_TIME_STAMP_QVCEFS1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_QVCEFS1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_QVCEFS1.SetFontBold(true);
	m_TIME_STAMP_QVCEFS2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_QVCEFS2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_QVCEFS2.SetFontBold(true);

	m_TIME_STAMP_QZCS1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_QZCS1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_QZCS1.SetFontBold(true);
	m_TIME_STAMP_QZCS2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_QZCS2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_QZCS2.SetFontBold(true);

	m_TIME_STAMP_DUCK1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_DUCK1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_DUCK1.SetFontBold(true);
	m_TIME_STAMP_DUCK2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_DUCK2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_DUCK2.SetFontBold(true);

	m_FILE_SIZE_QSNV1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_QSNV1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_QSNV1.SetFontBold(true);
	m_FILE_SIZE_QSNV2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_QSNV2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_QSNV2.SetFontBold(true);

	m_FILE_SIZE_QVCEFS1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_QVCEFS1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_QVCEFS1.SetFontBold(true);
	m_FILE_SIZE_QVCEFS2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_QVCEFS2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_QVCEFS2.SetFontBold(true);

	m_FILE_SIZE_QZCS1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_QZCS1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_QZCS1.SetFontBold(true);
	m_FILE_SIZE_QZCS2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_QZCS2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_QZCS2.SetFontBold(true);

	m_FILE_SIZE_DUCK1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_DUCK1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_DUCK1.SetFontBold(true);
	m_FILE_SIZE_DUCK2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_DUCK2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_DUCK2.SetFontBold(true);

	m_KB_QSNV.SetBkColor(RGB(200, 200, 100));
	m_KB_QSNV.SetTextColor(RGB(125, 0, 255));
    m_KB_QSNV.SetFontBold(true);

	m_KB_QVCEFS.SetBkColor(RGB(200, 200, 100));
	m_KB_QVCEFS.SetTextColor(RGB(125, 0, 255));
    m_KB_QVCEFS.SetFontBold(true);

	m_KB_QZCS.SetBkColor(RGB(200, 200, 100));
	m_KB_QZCS.SetTextColor(RGB(125, 0, 255));
    m_KB_QZCS.SetFontBold(true);

	m_KB_DUCK.SetBkColor(RGB(200, 200, 100));
	m_KB_DUCK.SetTextColor(RGB(125, 0, 255));
    m_KB_DUCK.SetFontBold(true);
	//*****************************
	m_MODE1.SetBkColor(RGB(200, 200, 100));
	m_MODE1.SetTextColor(RGB(0, 0, 255));
    m_MODE1.SetFontBold(true);
	m_MODE2.SetBkColor(RGB(255, 255, 255));
	m_MODE2.SetTextColor(RGB(125, 0, 255));
	m_MODE2.SetFontBold(true);

	m_TARGET1.SetBkColor(RGB(200, 200, 100));
	m_TARGET1.SetTextColor(RGB(0, 0, 255));
    m_TARGET1.SetFontBold(true);
	m_TARGET2.SetBkColor(RGB(255, 255, 255));
	m_TARGET2.SetTextColor(RGB(125, 0, 255));
	m_TARGET2.SetFontBold(true);

	m_PN1.SetBkColor(RGB(200, 200, 100));
	m_PN1.SetTextColor(RGB(0, 0, 255));
    m_PN1.SetFontBold(true);
	m_PN2.SetBkColor(RGB(255, 255, 255));
	m_PN2.SetTextColor(RGB(125, 0, 255));
	m_PN2.SetFontBold(true);
	//*****************************

	m_QSWINFO1.SetBkColor(RGB(200, 200, 100));
	m_QSWINFO1.SetTextColor(RGB(0, 0, 255));
    m_QSWINFO1.SetFontBold(true);
	m_QSWINFO2.SetBkColor(RGB(255, 255, 255));
	m_QSWINFO2.SetTextColor(RGB(125, 0, 255));
	m_QSWINFO2.SetFontBold(true);

	m_SIMDATA1.SetBkColor(RGB(200, 200, 100));
	m_SIMDATA1.SetTextColor(RGB(0, 0, 255));
    m_SIMDATA1.SetFontBold(true);
	m_SIMDATA2.SetBkColor(RGB(255, 255, 255));
	m_SIMDATA2.SetTextColor(RGB(125, 0, 255));
	m_SIMDATA2.SetFontBold(true);

	m_CSV1.SetBkColor(RGB(200, 200, 100));
	m_CSV1.SetTextColor(RGB(0, 0, 255));
    m_CSV1.SetFontBold(true);
	m_CSV2.SetBkColor(RGB(255, 255, 255));
	m_CSV2.SetTextColor(RGB(125, 0, 255));
	m_CSV2.SetFontBold(true);

	m_TIME_STAMP_QSWINFO1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_QSWINFO1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_QSWINFO1.SetFontBold(true);
	m_TIME_STAMP_QSWINFO2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_QSWINFO2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_QSWINFO2.SetFontBold(true);

	m_TIME_STAMP_SIMDATA1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_SIMDATA1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_SIMDATA1.SetFontBold(true);
	m_TIME_STAMP_SIMDATA2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_SIMDATA2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_SIMDATA2.SetFontBold(true);

	m_TIME_STAMP_CSV1.SetBkColor(RGB(200, 200, 100));
	m_TIME_STAMP_CSV1.SetTextColor(RGB(0, 0, 255));
    m_TIME_STAMP_CSV1.SetFontBold(true);
	m_TIME_STAMP_CSV2.SetBkColor(RGB(255, 255, 255));
	m_TIME_STAMP_CSV2.SetTextColor(RGB(125, 0, 255));
	m_TIME_STAMP_CSV2.SetFontBold(true);

	m_FILE_SIZE_QSWINFO1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_QSWINFO1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_QSWINFO1.SetFontBold(true);
	m_FILE_SIZE_QSWINFO2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_QSWINFO2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_QSWINFO2.SetFontBold(true);

	m_FILE_SIZE_SIMDATA1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_SIMDATA1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_SIMDATA1.SetFontBold(true);
	m_FILE_SIZE_SIMDATA2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_SIMDATA2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_SIMDATA2.SetFontBold(true);

	m_FILE_SIZE_CSV1.SetBkColor(RGB(200, 200, 100));
	m_FILE_SIZE_CSV1.SetTextColor(RGB(0, 0, 255));
    m_FILE_SIZE_CSV1.SetFontBold(true);
	m_FILE_SIZE_CSV2.SetBkColor(RGB(255, 255, 255));
	m_FILE_SIZE_CSV2.SetTextColor(RGB(125, 0, 255));
	m_FILE_SIZE_CSV2.SetFontBold(true);

	m_KB_QSWINFO.SetBkColor(RGB(200, 200, 100));
	m_KB_QSWINFO.SetTextColor(RGB(125, 0, 255));
    m_KB_QSWINFO.SetFontBold(true);

	m_KB_SIMDATA.SetBkColor(RGB(200, 200, 100));
	m_KB_SIMDATA.SetTextColor(RGB(125, 0, 255));
    m_KB_SIMDATA.SetFontBold(true);

	m_KB_CSV.SetBkColor(RGB(200, 200, 100));
	m_KB_CSV.SetTextColor(RGB(125, 0, 255));
    m_KB_CSV.SetFontBold(true);
    //*****************************
	m_SW1.SetBkColor(RGB(200, 200, 100));
	m_SW1.SetTextColor(RGB(0, 0, 255));
    m_SW1.SetFontBold(true);
	m_SW2.SetBkColor(RGB(255, 255, 255));
	m_SW2.SetTextColor(RGB(125, 0, 255));
	m_SW2.SetFontBold(true);

	m_HW1.SetBkColor(RGB(200, 200, 100));
	m_HW1.SetTextColor(RGB(0, 0, 255));
    m_HW1.SetFontBold(true);
	m_HW2.SetBkColor(RGB(255, 255, 255));
	m_HW2.SetTextColor(RGB(125, 0, 255));
	m_HW2.SetFontBold(true);

	m_MECHANICAL1.SetBkColor(RGB(200, 200, 100));
	m_MECHANICAL1.SetTextColor(RGB(0, 0, 255));
    m_MECHANICAL1.SetFontBold(true);
	m_MECHANICAL2.SetBkColor(RGB(255, 255, 255));
	m_MECHANICAL2.SetTextColor(RGB(125, 0, 255));
	m_MECHANICAL2.SetFontBold(true);
	//*****************************
    m_PC1.SetBkColor(RGB(200, 200, 100));
	m_PC1.SetTextColor(RGB(0, 0, 255));
    m_PC1.SetFontBold(true);
	m_PC2.SetBkColor(RGB(255, 255, 255));
	m_PC2.SetTextColor(RGB(125, 0, 255));
	m_PC2.SetFontBold(true);

	m_TIME1.SetBkColor(RGB(200, 200, 100));
	m_TIME1.SetTextColor(RGB(0, 0, 255));
    m_TIME1.SetFontBold(true);
	m_TIME2.SetBkColor(RGB(255, 255, 255));
	m_TIME2.SetTextColor(RGB(125, 0, 255));
	m_TIME2.SetFontBold(true);
	m_TIME3.SetBkColor(RGB(200, 200, 100));
	m_TIME3.SetTextColor(RGB(125, 0, 255));
    m_TIME3.SetFontBold(true);

	m_BACKUP_TYPE1.SetBkColor(RGB(200, 200, 100));
	m_BACKUP_TYPE1.SetTextColor(RGB(0, 0, 255));
    m_BACKUP_TYPE1.SetFontBold(true);
	m_BACKUP_TYPE2.SetBkColor(RGB(255, 255, 255));
	m_BACKUP_TYPE2.SetTextColor(RGB(125, 0, 255));
	m_BACKUP_TYPE2.SetFontBold(true);
	//*****************************
	m_IMEI.SetBkColor(RGB(200, 200, 100));
	m_IMEI.SetTextColor(RGB(0, 0, 255));
	//m_IMEI.SetFontSize(10);
    m_IMEI.SetFontBold(true);

	m_SO.SetBkColor(RGB(200, 200, 100));
	m_SO.SetTextColor(RGB(0, 0, 255));
	//m_SO.SetFontSize(10);
    m_SO.SetFontBold(true);

	m_MAL.SetBkColor(RGB(200, 200, 100));
	m_MAL.SetTextColor(RGB(0, 0, 255));
	//m_MAL.SetFontSize(10);
    m_MAL.SetFontBold(true);

	m_PICASSO.SetBkColor(RGB(200, 200, 100));
	m_PICASSO.SetTextColor(RGB(0, 0, 255));
	//m_PICASSO.SetFontSize(10);
    m_PICASSO.SetFontBold(true);

	m_IMEI_PART.SetBkColor(RGB(200, 200, 100));
	m_IMEI_PART.SetTextColor(RGB(0, 0, 255));
	//m_IMEI_PART.SetFontSize(10);
    m_IMEI_PART.SetFontBold(true);

	m_MAL_PART.SetBkColor(RGB(200, 200, 100));
	m_MAL_PART.SetTextColor(RGB(0, 0, 255));
	//m_MAL_PART.SetFontSize(10);
    m_MAL_PART.SetFontBold(true);

	CFont   font1;   
    font1.CreatePointFont(15*8,   "Arial");   
    m_IMEI_EDIT.SetFont(&font1,1);

	m_MAL_EDIT.SetFont(&font1,1);

	m_PICASSO_EDIT.SetFont(&font1,1);

	m_SO_EDIT.SetFont(&font1,1);

	m_IMEI_PART_EDIT.SetFont(&font1,1);

	m_MAL_PART_EDIT.SetFont(&font1,1);

	m_IMEI_EDIT.SetLimitText(15);

	m_MAL_EDIT.SetLimitText(15);

	m_PICASSO_EDIT.SetLimitText(10);

	m_IMEI_PART_EDIT.SetLimitText(8);

	m_MAL_PART_EDIT.SetLimitText(5);

	CFont   font2;   
    font2.CreatePointFont(12*8,   "Arial"); 
	//m_LIST.SetFont(&font2,1);

	m_PROGRESS_STATIC.SetBkColor(RGB(200, 200, 100));
	m_PROGRESS_STATIC.SetTextColor(RGB(0, 0, 255));
	m_PROGRESS_STATIC.SetFontSize(12);
    m_PROGRESS_STATIC.SetFontBold(true);

	m_MOBILEID1.SetBkColor(RGB(200, 200, 100));
	m_MOBILEID1.SetTextColor(RGB(0, 0, 255));
	m_MOBILEID1.SetFontSize(12);
    m_MOBILEID1.SetFontBold(true);

	m_MOBILEID2.SetBkColor(RGB(255, 255, 255));
	//m_MOBILEID2.SetTextColor(RGB(0, 0, 255));
	m_MOBILEID2.SetTextColor(RGB(125,0,255));
	m_MOBILEID2.SetFontSize(12);
    m_MOBILEID2.SetFontBold(true);

	m_TRANS1.SetBkColor(RGB(200, 200, 100));
	m_TRANS1.SetTextColor(RGB(0, 0, 255));
	m_TRANS1.SetFontSize(12);
    m_TRANS1.SetFontBold(true);

	m_TRANS2.SetBkColor(RGB(255, 255, 255));
	//m_TRANS2.SetTextColor(RGB(0, 0, 255));
	m_TRANS2.SetTextColor(RGB(125,0,255));
	m_TRANS2.SetFontSize(12);
    m_TRANS2.SetFontBold(true);

	m_PERCENT.SetBkColor(RGB(200, 200, 100));
	m_PERCENT.SetTextColor(RGB(125, 0, 255));
    m_PERCENT.SetFontBold(true);

	//*****************************
	m_PROGRESS.SetRange(0,100);
	m_PROGRESS.SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
    m_PROGRESS.SendMessage(PBM_SETBARCOLOR,0,RGB(125,0,255));  
    m_PROGRESS.SetPos(0);
	m_PERCENT.SetWindowText("0%");
	if(Target_Type=="MODULE")//no ppf
	{
		m_QSNV1.EnableWindow(false);
		m_QSNV2.EnableWindow(false);
		m_QVCEFS1.EnableWindow(false);
		m_QVCEFS2.EnableWindow(false);
		m_QZCS1.EnableWindow(false);
		m_QZCS2.EnableWindow(false);
		m_DUCK1.EnableWindow(false);
		m_DUCK2.EnableWindow(false);

		m_TIME_STAMP_QSNV1.EnableWindow(false);
		m_TIME_STAMP_QSNV2.EnableWindow(false);
		m_TIME_STAMP_QVCEFS1.EnableWindow(false);
		m_TIME_STAMP_QVCEFS2.EnableWindow(false);
		m_TIME_STAMP_QZCS1.EnableWindow(false);
		m_TIME_STAMP_QZCS2.EnableWindow(false);
		m_TIME_STAMP_DUCK1.EnableWindow(false);
		m_TIME_STAMP_DUCK2.EnableWindow(false);

		m_FILE_SIZE_QSNV1.EnableWindow(false);
		m_FILE_SIZE_QSNV2.EnableWindow(false);
		m_FILE_SIZE_QVCEFS1.EnableWindow(false);
		m_FILE_SIZE_QVCEFS2.EnableWindow(false);
		m_FILE_SIZE_QZCS1.EnableWindow(false);
		m_FILE_SIZE_QZCS2.EnableWindow(false);
		m_FILE_SIZE_DUCK1.EnableWindow(false);
		m_FILE_SIZE_DUCK2.EnableWindow(false);

		m_KB_QSNV.EnableWindow(false);
		m_KB_QVCEFS.EnableWindow(false);
		m_KB_QZCS.EnableWindow(false);
		m_KB_DUCK.EnableWindow(false);
		
	}
	if(!BRT_QCN_FLAG)//back up QCN
	{
		m_CSV1.EnableWindow(false);
		m_CSV2.EnableWindow(false);
		m_TIME_STAMP_CSV1.EnableWindow(false);
		m_TIME_STAMP_CSV2.EnableWindow(false);
		m_FILE_SIZE_CSV1.EnableWindow(false);;
		m_FILE_SIZE_CSV2.EnableWindow(false);;
		m_KB_CSV.EnableWindow(false);
	}

	if(!SECURITY_MECHANISM_FLAG)//there is no public key and sim data exist
	{
		m_QSWINFO1.EnableWindow(false);
		m_QSWINFO2.EnableWindow(false);
		m_TIME_STAMP_QSWINFO1.EnableWindow(false);
		m_TIME_STAMP_QSWINFO2.EnableWindow(false);
		m_FILE_SIZE_QSWINFO1.EnableWindow(false);;
		m_FILE_SIZE_QSWINFO2.EnableWindow(false);;
		m_KB_QSWINFO.EnableWindow(false);

		m_SIMDATA1.EnableWindow(false);
		m_SIMDATA2.EnableWindow(false);
		m_TIME_STAMP_SIMDATA1.EnableWindow(false);
		m_TIME_STAMP_SIMDATA2.EnableWindow(false);
		m_FILE_SIZE_SIMDATA1.EnableWindow(false);;
		m_FILE_SIZE_SIMDATA2.EnableWindow(false);;
		m_KB_SIMDATA.EnableWindow(false);

	}

	//********************LOCK
	  font_LOCK.CreateFont(15,							//   nHeight   
						   0,							//   nWidth   
						   0,							//   nEscapement   
						   0,							//   nOrientation   
						   FW_BOLD,						//   nWeight   
						   false,                       //   bItalic   
						   false,                       //   bUnderline   
						   0,                           //   cStrikeOut   
						   ANSI_CHARSET,                //   nCharSet   
						   OUT_DEFAULT_PRECIS,          //   nOutPrecision   
						   CLIP_DEFAULT_PRECIS,         //   nClipPrecision   
						   DEFAULT_QUALITY,             //   nQuality   
						   DEFAULT_PITCH   |   FF_SWISS,//   nPitchAndFamily   
						   _T("Arial")); 

	  m_NETWORK.SetFont(&font_LOCK,1); 
	  m_SERVICE_PROVIDER.SetFont(&font_LOCK,1); 
	  m_CORPORATE.SetFont(&font_LOCK,1); 
	  m_NETWORK_SUBSET.SetFont(&font_LOCK,1); 
	  m_SIM.SetFont(&font_LOCK,1); 
	  m_USER_SIM.SetFont(&font_LOCK,1); 

	  if(NetworkLock_Flag)
		  m_NETWORK.SetCheck(1);
	  else
		  m_NETWORK.SetCheck(0);

      if(ServiceProviderLock_Flag)
		  m_SERVICE_PROVIDER.SetCheck(1);
	  else
		  m_SERVICE_PROVIDER.SetCheck(0);

	  if(CorporateLock_Flag)
		  m_CORPORATE.SetCheck(1);
	  else
		  m_CORPORATE.SetCheck(0);

	  if(NetworkSubsetLock_Flag)
		  m_NETWORK_SUBSET.SetCheck(1);
	  else
		  m_NETWORK_SUBSET.SetCheck(0);

	  if(SIMLock_Flag)
		  m_SIM.SetCheck(1);
	  else
		  m_SIM.SetCheck(0);

	  if(UserSIMLock_Flag)
		  m_USER_SIM.SetCheck(1);
	  else
		  m_USER_SIM.SetCheck(0);
	 //**************************MAL IMEI FIRST PART

	if(Write_MAL_Flag)//write MAL
	{
		//m_MAL.ShowWindow(SW_SHOW);
		//m_MAL_EDIT.ShowWindow(SW_SHOW);
		m_MAL.EnableWindow(true);
		m_MAL_EDIT.EnableWindow(true);
	}
	else
	{
		//m_MAL.ShowWindow(SW_HIDE);
		//m_MAL_EDIT.ShowWindow(SW_HIDE);
		m_MAL.EnableWindow(false);
		m_MAL_EDIT.EnableWindow(false);
	}

}

void CQUALCOMM_IMEI_CODINGDlg::Set_Ctrl_Text()
{
	m_bTranciver = ::GetPrivateProfileInt("TRANCIVER_INFO","ONOFF",0,Ini_File_Path);
	m_TITLE.SetWindowText(Set_Title());

/*	m_QSNV2.SetWindowText(QSNV_File_Name); 
	QSNV_File_Time=GetFileTime(QSNV_File_Name);
	m_TIME_STAMP_QSNV2.SetWindowText(QSNV_File_Time);
   
	QSNV_File_Size=GetFileSize(QSNV_File_Name);
	m_FILE_SIZE_QSNV2.SetWindowText(QSNV_File_Size);

	m_QZCS2.SetWindowText(QZCS_File_Name);  
	QZCS_File_Time=GetFileTime(QZCS_File_Name);
	m_TIME_STAMP_QZCS2.SetWindowText(GetFileTime(QZCS_File_Name));
    
	QZCS_File_Size=GetFileSize(QZCS_File_Name);
	m_FILE_SIZE_QZCS2.SetWindowText(QZCS_File_Size);*/


	m_QVCEFS2.SetWindowText(QVCEFS_File);
  
	QVCEFS_File_Time=GetFileTime(QVCEFS_File_Name);
	m_TIME_STAMP_QVCEFS2.SetWindowText(QVCEFS_File_Time);

	QVCEFS_File_Size=GetFileSize(QVCEFS_File_Name);
	m_FILE_SIZE_QVCEFS2.SetWindowText(QVCEFS_File_Size);


/*	m_DUCK2.SetWindowText(DUCK_File);

	DUCK_File_Time=GetFileTime(DUCK_File_Name);
	m_TIME_STAMP_DUCK2.SetWindowText(DUCK_File_Time);

	DUCK_File_Size=GetFileSize(DUCK_File_Name);
	m_FILE_SIZE_DUCK2.SetWindowText(DUCK_File_Size);*/

    m_QSWINFO2.SetWindowText(QSWINFO_File_Name);

	m_TIME_STAMP_QSWINFO2.SetWindowText("N/A");
	m_FILE_SIZE_QSWINFO2.SetWindowText("N/A");

//	m_TIME_STAMP_QSWINFO2.SetWindowText(GetFileTime(QSWINFO_File_Name));
//	m_FILE_SIZE_QSWINFO2.SetWindowText(GetFileSize(QSWINFO_File_Name));

	Sim_Data_File_Name=QSWINFO_File_Name;   //Select SIM file at QSWINFO location,sunlight 081110
	m_SIMDATA2.SetWindowText(Sim_Data_File_Name);

//	m_TIME_STAMP_SIMDATA2.SetWindowText(GetFileTime(Sim_Data_File_Name));
//	m_FILE_SIZE_SIMDATA2.SetWindowText(GetFileSize(Sim_Data_File_Name));

	m_TIME_STAMP_SIMDATA2.SetWindowText("N/A");
	m_FILE_SIZE_SIMDATA2.SetWindowText("N/A");

	//********************************
	m_CSV2.SetWindowText(CSV_File_Name);
	m_TIME_STAMP_CSV2.SetWindowText(GetFileTime(CSV_File_Name));
	m_FILE_SIZE_CSV2.SetWindowText(GetFileSize(CSV_File_Name));
	//********************************
	m_TARGET2.SetWindowText(Target_Type);

	m_MODE2.SetWindowText(Mode_Name);

	m_PN2.SetWindowText(Part_Number);

	m_SO_EDIT.SetWindowText(SO_Str);
    //********************************
	m_PC2.SetWindowText(GetComputerName());

	m_TIME2.SetWindowText("0");
	//********************************
	m_SW2.SetWindowText(Software_Version);

	m_MECHANICAL2.SetWindowText(Mechanical_Version);

	m_HW2.SetWindowText(Hardware_Version);

	if(ATHB1A_Flag)
		m_TRANS2.SetWindowText(TRANS_NO);
	//********************************
	if(BRT_QCN_FLAG)//back up brt
		m_BACKUP_TYPE2.SetWindowText("BRT");
	else
		m_BACKUP_TYPE2.SetWindowText("QCN");

    ::GetPrivateProfileString("IMEI_MAL","IMEI_FIRST_PART","",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
    IMEI_FIRST_PART=Temp_Global;
	m_IMEI_PART_EDIT.SetWindowText(IMEI_FIRST_PART);
    m_IMEI_PART_EDIT.EnableWindow(false);
	m_IMEI_PART.EnableWindow(false);
    
	::GetPrivateProfileString("IMEI_MAL","MAL_FIRST_PART","",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
    MAL_FIRST_PART=Temp_Global;
	m_MAL_PART_EDIT.SetWindowText(MAL_FIRST_PART);
	m_MAL_PART_EDIT.EnableWindow(false);
	m_MAL_PART.EnableWindow(false);
    
	IMEI_Difference=::GetPrivateProfileInt("IMEI_MAL","IMEI_DIFFERENCE",1,Ini_File_Path);
    ButtonChange_Flag=0;

	m_bBRTBackup1 = ::GetPrivateProfileInt("BRT_BACKUP_SET","BRT1_BACKUP",0,Ini_File_Path);

	m_bBRTBackup2 = ::GetPrivateProfileInt("BRT_BACKUP_SET","BRT2_BACKUP",1,Ini_File_Path);

	m_LIST.ResetContent();
	m_LIST.SetTopIndex(m_LIST.GetCount()-1);
	return;

}

CString CQUALCOMM_IMEI_CODINGDlg::Set_Title()
{
	if(m_bTranciver)
		return (Model_Name+" TRANSCEIVER CODING TOOL "+Tool_Version);
	else
		return (Model_Name+" IMEI CODING TOOL "+Tool_Version);

}

void CQUALCOMM_IMEI_CODINGDlg::Initial_Parameter()
{
	 AutoRestore=true;
	 SkipReset=false;//true to skip the reset operation after the phone is programmed   
	 AllowESN_Mismatch=false;
	 IgnorePhoneModel=true;

	 Port_Number=::GetPrivateProfileInt("CONNECT_SERVER","COMPORT",4,Ini_File_Path);
	 SW_CHECK=::GetPrivateProfileInt("CHECK","SW_CHECK",1,Ini_File_Path);
	 HW_CHECK=::GetPrivateProfileInt("CHECK","HW_CHECK",1,Ini_File_Path);
	 ME_CHECK=::GetPrivateProfileInt("CHECK","ME_CHECK",1,Ini_File_Path);
	 PROCESS_CHECK=::GetPrivateProfileInt("CHECK","PROCESS_CHECK",1,Ini_File_Path);
	 if(!PROCESS_CHECK)
	 {
		 BASEBAND_TEST_CHECK_FLAG=0;
		 CAL_PRETEST_2G_CHECK_FLAG=0;
		 CAL_PRETEST_3G_CHECK_FLAG=0;
		 LEAKAGE_CHARGER_CHECK_FLAG=0;
		 FUNCTION_WIRELESS_TEST_CHECK_FLAG=0;
		 PPF_CHECK_FLAG=0;
		 ACM_BOARD_CHECK_FLAG=0;     
	 }
	 ::GetPrivateProfileString("QCN_BACKUP_PATH","QCN_BACKUP_PATH","c\\picsdata\\",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
     Backup_Path=Temp_Global;
	 //Mark by Magan Mei
//	 Backup_Path="\\\\10.94.5.50\\BackupFile";//the backup path is fixed to \\10.94.5.50\BackupFile
//	 Backup_Path="c:\\";
	 ::GetPrivateProfileString("PICSFILE_PATH","PICSFILE_PATH","c\\picsdata\\",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
     PICSFile_Path=Temp_Global;
	 CreateDirectory(Backup_Path,NULL); 
	 if((Backup_Path.Find("picsdata")==-1)||(Backup_Path.Find("PICSDATA")==-1))//\\10.94.5.50\BackupFile
	 {
		if(Backup_Path.Right(1)!="\\")
		{
			Backup_Path+="\\";
		}
		
		Backup_Path+=Model_Name;
		CreateDirectory(Backup_Path,NULL); 
	 }
	 ButtonStart_Flag=1;
     DEBUG_ONOFF_FLAG=::GetPrivateProfileInt("DEBUG_INFOR","ONOFF",0,Ini_File_Path);
	 Connect_Server_Timeout=::GetPrivateProfileInt("CONNECT_SERVER","ConnectServerTimeout",5000,Ini_File_Path);
	 WriteSIM_Delay=::GetPrivateProfileInt("CONNECT_SERVER","WriteSIM_Delay",5000,Ini_File_Path);
	 Cycle_Count=::GetPrivateProfileInt("CONNECT_SERVER","ConnectServerCycleCount",5,Ini_File_Path);
	 pAfxIMEICodingThread=AfxIMEICodingThread;

	 return;
}

CString CQUALCOMM_IMEI_CODINGDlg::GetComputerName()
{
	DWORD Buffer_Size=sizeof(Temp_Global);
	::GetComputerName(Temp_Global,&Buffer_Size);
	Computer_Name=Temp_Global;
	return Computer_Name;
}

CString CQUALCOMM_IMEI_CODINGDlg::GetFileSize(CString FileName)
{   
	if((FileName=="N/A")||(FileName==Sim_Data_File_Name))
		return "N/A";
	CString File_Size_String="";
	int File_Size_String_Length=0;
	DWORD File_Size=0;
	DWORD FileSize_LowOrder=0;
	DWORD FileSize_HighOrder=0;
	HANDLE File_HANDLE=::CreateFile(FileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL);
	VERIFY(File_HANDLE);
	FileSize_LowOrder=::GetFileSize(File_HANDLE,&FileSize_HighOrder);
	if(FileSize_LowOrder==0xFFFFFFFF)
	{
		Message_String="Get file size for "+FileName+" fail!!";
		AfxMessageBox(Message_String);
		return "N/A";
	}
	File_Size=(FileSize_LowOrder+1023)/1024;//----->change to KB
	sprintf(Temp_Global,"%ld",File_Size);
    File_Size_String=Temp_Global;
	File_Size_String_Length=File_Size_String.GetLength();
	if((File_Size_String_Length>3)&&(File_Size_String_Length<7))
		File_Size_String.Insert(File_Size_String_Length-3,',');
	if(File_Size_String_Length>=7)
	{
		File_Size_String.Insert(File_Size_String_Length-3,',');
		File_Size_String.Insert(File_Size_String_Length-6,',');
	}
	::CloseHandle(File_HANDLE);
	return File_Size_String;
}

CString CQUALCOMM_IMEI_CODINGDlg::GetFileTime(CString FileName)
{
	if((FileName=="N/A")||(FileName==Sim_Data_File_Name))
		return "N/A";
	CString File_Time_String="";
	CString Month_String="";
	CString Day_String="";
	CString Hour_String="";
	CString Minute_String="";
	CString Second_String="";
	int File_Time_String_Length=0;
	FILETIME CrateTime={0,0};
	FILETIME LastAccessTime={0,0};
	FILETIME LastWriteTime={0,0};
	SYSTEMTIME CrateTime_System={0,0,0,0,0,0,0,0};
	SYSTEMTIME LastAccessTime_System={0,0,0,0,0,0,0,0};
	SYSTEMTIME LastWriteTime_System={0,0,0,0,0,0,0,0};
	HANDLE File_HANDLE=::CreateFile(FileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL);
	VERIFY(File_HANDLE);
    if(!::GetFileTime(File_HANDLE,&CrateTime,&LastAccessTime,&LastWriteTime))
	{
		Message_String="Get file time for "+FileName+" fail!!";
		AfxMessageBox(Message_String);
		return "N/A";
	}
    /*if(!::FileTimeToSystemTime(&CrateTime,&CrateTime_System))
	{
		Message_String="Get file time for "+FileName+" fail!!";
		AfxMessageBox(Message_String);
		return "N/A";
	}

	if(!::FileTimeToSystemTime(&LastAccessTime,&LastAccessTime_System))
	{
		Message_String="Get file time for "+FileName+" fail!!";
		AfxMessageBox(Message_String);
		return "N/A";
	}*/

	if(!::FileTimeToSystemTime(&LastWriteTime,&LastWriteTime_System))
	{
		Message_String="Get file time for "+FileName+" fail!!";
		AfxMessageBox(Message_String);
		return "N/A";
	}
    if((LastWriteTime_System.wHour>=0)&&(LastWriteTime_System.wHour<16))
		LastWriteTime_System.wHour+=8;
	else
        LastWriteTime_System.wHour-=16;
    if(LastWriteTime_System.wMonth<10)
		sprintf(Temp_Global,"0%u",LastWriteTime_System.wMonth);
	else
		sprintf(Temp_Global,"%u",LastWriteTime_System.wMonth);
	Month_String=Temp_Global;

	if(LastWriteTime_System.wDay<10)
		sprintf(Temp_Global,"0%u",LastWriteTime_System.wDay);
	else
		sprintf(Temp_Global,"%u",LastWriteTime_System.wDay);
	Day_String=Temp_Global;

	if(LastWriteTime_System.wHour<10)
		sprintf(Temp_Global,"0%u",LastWriteTime_System.wHour);
	else
		sprintf(Temp_Global,"%u",LastWriteTime_System.wHour);
	Hour_String=Temp_Global;

	if(LastWriteTime_System.wMinute<10)
		sprintf(Temp_Global,"0%u",LastWriteTime_System.wMinute);
	else
		sprintf(Temp_Global,"%u",LastWriteTime_System.wMinute);
	Minute_String=Temp_Global;

	if(LastWriteTime_System.wSecond<10)
		sprintf(Temp_Global,"0%u",LastWriteTime_System.wSecond);
	else
		sprintf(Temp_Global,"%u",LastWriteTime_System.wSecond);
	Second_String=Temp_Global;

	sprintf(Temp_Global,"%u/%s/%s %s:%s:%s",
			LastWriteTime_System.wYear,
			Month_String,
			Day_String,
			Hour_String,
			Minute_String,
			Second_String);

    File_Time_String=Temp_Global;
	::CloseHandle(File_HANDLE);
	return File_Time_String;
}

HBRUSH CQUALCOMM_IMEI_CODINGDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor ==CTLCOLOR_DLG)
         return m_brush;
	if((pWnd->GetDlgCtrlID()==IDC_EDIT_IMEI)||
		(pWnd->GetDlgCtrlID()==IDC_EDIT_MAL)||
	    (pWnd->GetDlgCtrlID()==IDC_EDIT_IMEI_PART)||
	    (pWnd->GetDlgCtrlID()==IDC_EDIT_MAL_PART)||
	    (pWnd->GetDlgCtrlID()==IDC_LIST_MESSAGE)||
	    (pWnd->GetDlgCtrlID()==IDC_EDIT_PICASSO)||
		(pWnd->GetDlgCtrlID()==IDC_EDIT_SO))   
	{   
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetTextColor(RGB(125,0,255));   
	}  

	if((pWnd->GetDlgCtrlID()==IDC_CHECK_NETWORK)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_SERVICE_PROVIDER)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_CORPORATE)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_NETWORK_SUBSET)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_SIM)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_USER_SIM))
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkColor(RGB(200,200,100)); 
		return m_brush;  
        //pDC->SetBkMode(TRANSPARENT); 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CQUALCOMM_IMEI_CODINGDlg::GetLockIndicator()
{
	NetworkLock_Flag=0;
	CorporateLock_Flag=0;
	ServiceProviderLock_Flag=0;
	NetworkSubsetLock_Flag=0;
	SIMLock_Flag=0;
	UserSIMLock_Flag=0;
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::OnButtonChange() 
{
	// TODO: Add your control notification handler code here
	if(ButtonChange_Flag==0)//the control is enabled, enable it
	{
		if(Write_MAL_Flag)//write
		{
			m_MAL_PART.EnableWindow(true);
			m_MAL_PART_EDIT.EnableWindow(true);
		}
		m_IMEI_PART.EnableWindow(true);
		m_IMEI_PART_EDIT.EnableWindow(true);
		ButtonChange_Flag=1;
	}
	else
	{
		if(Write_MAL_Flag)
		{
			m_MAL_PART_EDIT.GetWindowText(MAL_FIRST_PART);
			::WritePrivateProfileString("IMEI_MAL","MAL_FIRST_PART",MAL_FIRST_PART,Ini_File_Path);
		}
		m_IMEI_PART_EDIT.GetWindowText(IMEI_FIRST_PART);
		::WritePrivateProfileString("IMEI_MAL","IMEI_FIRST_PART",IMEI_FIRST_PART,Ini_File_Path);
		m_MAL_PART.EnableWindow(false);
		m_MAL_PART_EDIT.EnableWindow(false);
		m_IMEI_PART.EnableWindow(false);
		m_IMEI_PART_EDIT.EnableWindow(false);
        ButtonChange_Flag=0;
	}
	return;	
}

void CQUALCOMM_IMEI_CODINGDlg::Set_Focus()
{
	if(Write_MAL_Flag)
		m_MAL_EDIT.SetFocus();
	else
	{
		if(ATHB1A_Flag)
			m_PICASSO_EDIT.SetFocus();
		else
			m_IMEI_EDIT.SetFocus();
	}
		
	return;
}


void CQUALCOMM_IMEI_CODINGDlg::OnButtonExit2() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CQUALCOMM_IMEI_CODINGDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	Initial();
	EnableDisableButton(false);
	ThreadHandle=AfxBeginThread(pAfxIMEICodingThread,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	return;
}

unsigned char CQUALCOMM_IMEI_CODINGDlg::Check_MAL_FirstPart(CString MAL_String)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	if(MAL_FIRST_PART!=MAL_String.Left(5))
	{
		sprintf(temp_log,"The first 5 digits of MAL %s is not %s......",MAL_String,MAL_FIRST_PART);
		InsertStringIntoList(temp_log);
		return false;
	}
	else
		return true;
}

unsigned char CQUALCOMM_IMEI_CODINGDlg::Check_IMEI_FirstPart(CString IMEI_String_Parameter)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	if(m_bTranciver)
	{
		if(IMEI_FIRST_PART!=IMEI_String_Parameter.Left(5))
		{
			sprintf(temp_log,"The first 5 digits of BT address %s is not %s......",IMEI_String_Parameter,IMEI_FIRST_PART);
			InsertStringIntoList(temp_log);
			return false;
		}
	}
	else
	{
		if(IMEI_FIRST_PART!=IMEI_String_Parameter.Left(8))
		{
			sprintf(temp_log,"The first 8 digits of IMEI %s is not %s......",IMEI_String_Parameter,IMEI_FIRST_PART);
			InsertStringIntoList(temp_log);
			return false;
		}
	}
	return true;
}

unsigned char CQUALCOMM_IMEI_CODINGDlg::CheckMALLength(CString MAL_String)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	int nMALLength = ::GetPrivateProfileInt("LENGTH_SETTING","MAL_LENGTH",15,Ini_File_Path);
	if(MAL_String.GetLength()!=nMALLength)
	{
		sprintf(temp_log,"The length of MAL %s is not %d......",MAL_String,nMALLength);
		InsertStringIntoList(temp_log);
		return false;
	}
	else 
		return true;

}

unsigned char CQUALCOMM_IMEI_CODINGDlg::CheckIMEILength(CString IMEI_String_Parameter)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	if(m_bTranciver)
	{
		int nBTLength = ::GetPrivateProfileInt("LENGTH_SETTING","BT_LENGTH",12,Ini_File_Path);
		if(IMEI_String_Parameter.GetLength()!=nBTLength)
		{
			sprintf(temp_log,"The length of BT address %s is not %d......",IMEI_String_Parameter,nBTLength);
			m_LIST.InsertString(-1,temp_log);
			m_LIST.SetTopIndex(m_LIST.GetCount()-1);
			Log_String+=temp_log;
			Log_String+="\n";
			return false;
		}

	}
	else
	{
		int nIMEILength = ::GetPrivateProfileInt("LENGTH_SETTING","IMEI_LENGTH",15,Ini_File_Path);
		if(IMEI_String_Parameter.GetLength()!=nIMEILength)
		{
			sprintf(temp_log,"The length of IMEI %s is not %d......",IMEI_String_Parameter,nIMEILength);
			m_LIST.InsertString(-1,temp_log);
			m_LIST.SetTopIndex(m_LIST.GetCount()-1);
			Log_String+=temp_log;
			Log_String+="\n";
			return false;
		}
	}
	return true;
}


unsigned char CQUALCOMM_IMEI_CODINGDlg::CheckIMEIValidity(CString IMEI_String_Parameter)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	int s = 0;
	int i,t;
	for(i=0;i<14;i++)
	{
		if(i%2==0)
			s+=atoi(IMEI_String_Parameter.Mid(i,1));
		else
		{
			t=atoi(IMEI_String_Parameter.Mid(i,1))*2;
			s=s+t%10+t/10;
		}
	}
	if(s%10!=0)
		t=(s/10+1)*10-s;
	else
		t=0;
	if(t==atoi(IMEI_String_Parameter.Mid(14,1)))
		return true;
	else 
	{
		sprintf(temp_log,"The IMEI %s is not a valid IMEI......",IMEI_String_Parameter);
		InsertStringIntoList(temp_log);
		return false;
	}

}

unsigned char CQUALCOMM_IMEI_CODINGDlg::CheckIMEIDifference(CString IMEI_String_Parameterameter)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	CString IMEI_Temp=IMEI_LAST_WRITE.Right(7);
	IMEI_Temp=IMEI_Temp.Left(6);
	sprintf(Temp_Global,"%.6d",atoi(IMEI_Temp)+IMEI_Difference);
    IMEI_Temp=IMEI_LAST_WRITE.Left(8)+Temp_Global;
	if(IMEI_Temp!=IMEI_String_Parameterameter.Left(14))
	{
		sprintf(temp_log,"The difference between IMEI %s and the last wrote IMEI %s is not %d......",IMEI_String_Parameterameter,IMEI_LAST_WRITE,IMEI_Difference);
		InsertStringIntoList(temp_log);
		return false;
	}
	return true;
}

unsigned char CQUALCOMM_IMEI_CODINGDlg::CheckIMEIZeroDummyEmpty(char IMEI_Array[])
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	if(!strcmp((char *)IMEI_Array,""))
	{
		InsertStringIntoList(_T("The IMEI read from phone is empty......"));
		return true;
	}

	if(!strcmp((char *)IMEI_Array,Zero_IMEI))
	{
		InsertStringIntoList(_T("The IMEI read from phone is all zero......"));
		return true;
	}

	if(!strcmp((char *)IMEI_Array,Dummy_IMEI))
	{
		InsertStringIntoList(_T("The IMEI read from phone is a dummy IMEI......"));
		return true;
	}
	return false;
}


void CQUALCOMM_IMEI_CODINGDlg::SetReady()
{
	InsertStringIntoList(_T("IMEI coding tool is ready......"));
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::OnButtonConfig() 
{
	// TODO: Add your control notification handler code here
	SetConfig=1;
	CPassWord PassWordDlg;
	if(PassWordDlg.DoModal()==IDCANCEL)
	{
		SetConfig=0;
		return;
	}
	CConfig Config_Ctrl;
	Config_Ctrl.DoModal();
	SetConfig=0;
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::Initial()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	m_LIST.ResetContent();
	sprintf(temp_log,"============INITIAL PARAMETER============");
	m_LIST.InsertString(-1,temp_log);
	m_LIST.SetTopIndex(m_LIST.GetCount()-1);

	Log_String+=temp_log;
	Log_String+="\n";
	TimeCount=0;
	m_PROGRESS.SetPos(0);
	m_PERCENT.SetWindowText("0%");
	m_TIME2.SetWindowText("0");
	ZeroMemory(Temp_Global,sizeof(Temp_Global));
	Error_Code_String="F00";
//	Lock_Status=FALSE;
	IMEI_BT_GSN_Flag=0;
	Unlock_Count=0;
	Status=0; 
	SPC_Code[0]='\0';
	IMEI_From_Phone[0]='\0';
	IMEI_For_Phone[0]='\0';
	BT_From_Phone[0]='\0';
	BT_For_Phone[0]='\0';
	IMEI_String[0]='\0';
	Status_SPC=0;
	IMEI="";
	Mobile_ID="";
	Log_String="";
	Duck_Log_String="";
	KLF_Byte=0x00;
	HW_Version_In_Mobile="";
	SW_Version_In_Mobile="";
	BT_Address="";
	GSN_String="";
	Device_Serial_Number[0]='\0';
	Device_Serial_Number_Digit[0]='\0';
	SIM_Data_For_Phone[0]='\0';
	SIM_Data_From_PICS[0]='\0';
	Network_Unlock_Code[0]='\0';
	Service_Provider_Unlock_Code[0]='\0';
	Coporate_Unlock_Code[0]='\0';
	Network_Subset_Unlock_Code[0]='\0';
	User_SIM_Unlock_Code[0]='\0';
	User_USIM_Unlock_Code[0]='\0';
	Serial_Code[0]='\0';
	Phone_Lock_Code[0]='\0';
	IMEI_LAST_WRITE="";
	IMEI_FROM_SCANNER="";
	MAL_FROM_SCANNER="";
	ME_Version_In_Mobile="";

	//
	mFlag = 0;
	Time_Elapse = 0;
	::GetPrivateProfileString("IMEI_MAL","LAST_WRITE_IMEI","",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
    IMEI_LAST_WRITE=Temp_Global;

	::GetPrivateProfileString("CONFIG_VERSION","CONFIG_VERSION","V1.00",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
	Config_Version=Temp_Global;

	//set path for PICS file
	ServerDirectory = PICSFile_Path;
	::CreateDirectory(ServerDirectory,NULL);
	/*if(ServerDirectory.GetLength()==0)
	{
		ServerDirectory = "c:\\picsdata";
		::CreateDirectory(ServerDirectory,NULL);
	}
    else
	{   
		sprintf(Temp_Global,"%s\\%sEID",ServerDirectory,Model_Name);
		ServerDirectory=Temp_Global;
        ::CreateDirectory(ServerDirectory,NULL);
	    ServerDirectory += "\\";
	    ServerDirectory += Computer_Name;
	    ::CreateDirectory(ServerDirectory,NULL);
	}*/
	///////////////////////


	::CreateDirectory(Current_Directory+"\\log",NULL);
	::CreateDirectory(Current_Directory+"\\log\\imeicoding_log",NULL);

	char LogFileName[512];
	CStdioFile File_Ctrl;
	TCHAR szFolderOld[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szFolderOld,CSIDL_PERSONAL,_T("Log"));

	sprintf(LogFileName,"%s\\%s.csv",szFolderOld,TRANS_NO);
	CString szLog = _T("SO,Model_Name,MobileID,IMEI,Track ID (Picasso number),SW Version,HW Version,Start Time,Total flash time, Error message, rewrite time");

	if(!File_Ctrl.Open(LogFileName,CFile::modeRead,NULL))
	{
		if(File_Ctrl.Open(LogFileName,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate,NULL))
		{
			File_Ctrl.SeekToEnd();
			File_Ctrl.WriteString(szLog);
			File_Ctrl.WriteString(_T("\n"));
			File_Ctrl.Close();
		}
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);
	m_szStartTime.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	sprintf(temp_log,"============INITIAL PARAMETER SUCCESS============");
	InsertStringIntoList(temp_log);
}

void CQUALCOMM_IMEI_CODINGDlg::ShowMessageDlg()
{
	CMessage MessageDlg_Ctrl;
    MessageDlg_Ctrl.DoModal();
}

void CQUALCOMM_IMEI_CODINGDlg::ResetEditContent()
{
	m_MAL_EDIT.SetWindowText("");
	
	m_IMEI_EDIT.SetWindowText("");

//	if(Model_Name=="ATHB1A")
	if(ATHB1A_Flag)
		m_PICASSO_EDIT.SetWindowText("");
	
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::EnableDisableButton(unsigned char Enable_Disable_Flag)
{
	m_CHANGE.EnableWindow(Enable_Disable_Flag);
	m_START.EnableWindow(Enable_Disable_Flag);
	m_CONFIG.EnableWindow(Enable_Disable_Flag);
	m_EXIT.EnableWindow(Enable_Disable_Flag);
	m_LOGBUTTON.EnableWindow(Enable_Disable_Flag);
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::EndReset(unsigned char ShowMessageDlg_Flag)
{
    SetTestRecord(Error_Code_String);
	ResetEditContent();
	EnableDisableButton(true);
	Set_Focus();
	KillTimer(1);
	if(ShowMessageDlg_Flag)
	{
		ShowMessageDlg();
	}
	errFlag = false;
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		TimeCount++;
		sprintf(Temp_Global,"%d",TimeCount);
		m_TIME2.SetWindowText(Temp_Global);
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

double CQUALCOMM_IMEI_CODINGDlg::TimeMeter(int nIndex, bool bStart)
{
	static clock_t start[8], finish[8];
	
	if(bStart) 
	{
		start[nIndex]=clock();
		return 0;
	}
	else 
	{
		finish[nIndex]=clock();
		return (double)((finish[nIndex]-start[nIndex])/(double)CLK_TCK);
	}

}

LRESULT CQUALCOMM_IMEI_CODINGDlg::OnTerminateThread(WPARAM wParam, LPARAM lParam)
{   
	CStdioFile File_Ctrl;
	char LogFileName[512];
	char temp_log[512];
	ZeroMemory(LogFileName,sizeof(LogFileName));
	ZeroMemory(temp_log,sizeof(temp_log));
	Time_Elapse=TimeMeter(1,false);
	sprintf(temp_log,"Time elapse %f......",Time_Elapse);
	InsertStringIntoList(temp_log);
	{
		TCHAR szFolderOld[MAX_PATH] = {0};
		Win32Utility::GetShellFolder(szFolderOld,CSIDL_PERSONAL,_T("Log"));

		sprintf(LogFileName,"%s\\%s.csv",szFolderOld,TRANS_NO);
		CString szTimeTotal;
		szTimeTotal.Format(_T("%f"), Time_Elapse);
		CString szLog=SO_Str + _T(",")\
			+ Model_Name + _T(",")\
			+ Mobile_ID + _T(",") \
			+ IMEI_FROM_SCANNER + _T(",") \
			+ Picasso_No + _T(",") \
			+ Software_Version + _T(",") \
			+ Hardware_Version + _T(",") \
			+ m_szStartTime + _T(",")\
			+ szTimeTotal + _T(",") \
			+ Error_Code_String + _T("\n");

		if(File_Ctrl.Open(LogFileName,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate,NULL))
		{
			File_Ctrl.SeekToEnd();
			File_Ctrl.WriteString(szLog);
			File_Ctrl.Close();
			InsertStringIntoList(_T("Write csv log File success..."));
		}
		else
			InsertStringIntoList(_T("Write csv log File fail..."));
	}
	if(DEBUG_ONOFF_FLAG)//to generate debug infor
	{
		sprintf(LogFileName,"%s\\log\\imeicoding_log\\%s.log",Current_Directory,IMEI_FROM_SCANNER);
		if(File_Ctrl.Open(LogFileName,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate,NULL))
		{
			File_Ctrl.SeekToEnd();
			File_Ctrl.WriteString(Log_String);
			File_Ctrl.WriteString("=============================END============================");
			File_Ctrl.WriteString("\n\n");
			File_Ctrl.Close();
		}
	}
	Start_Flag=0;
//	Phone_Ctrl.DisConnect_Server(PHONE_HANDLE);
	EndReset(errFlag);	
	CloseHandle((HANDLE)ThreadHandle->m_hThread);
	Set_Focus();
	return true;
}

LRESULT CQUALCOMM_IMEI_CODINGDlg::OnResetTestCount(WPARAM wParam, LPARAM lParam)
{
	char temp[25];
	ZeroMemory(temp,sizeof(temp));
	RECT StatusBar_Rect;
	RECT Client_Rect;
	m_StatusBar.GetItemRect(4,&StatusBar_Rect);//Reset Test Record item

    m_StatusBar.GetClientRect(&Client_Rect);
	m_StatusBar.ClientToScreen(&Client_Rect);
    StatusBar_Rect.left=Client_Rect.left+StatusBar_Rect.left;
	StatusBar_Rect.top=Client_Rect.top+StatusBar_Rect.top;
    StatusBar_Rect.right=Client_Rect.right;
    StatusBar_Rect.bottom=Client_Rect.bottom;
    //ClientToScreen(&StatusBar_Rect);
	//ScreenToClient
    if((wParam>=(WPARAM)StatusBar_Rect.left)&&
	   (wParam<=(WPARAM)StatusBar_Rect.right)&&
	   (lParam>=(LPARAM)StatusBar_Rect.top)&&
	   (lParam<=(LPARAM)StatusBar_Rect.bottom))
	{
		if((!Start_Flag)&&TotalNo)//program is in idle and TotalNo!=0
		{
			if((::AfxMessageBox("WARING:You are trying to reset the test record to zero, click YES to reset, or click NO not to reset!!",MB_YESNO|MB_ICONINFORMATION)==IDYES))
			{
				TotalNo=0;
				PassNo=0;
				FailNo=0;
				sprintf(Total_String,"TEST TOTAL:  %d",TotalNo);
				sprintf(Pass_String,"TEST PASS:  %d",PassNo);
				sprintf(Fail_String,"TEST FAIL:  %d",FailNo);
				sprintf(temp,"%d",TotalNo);
				::WritePrivateProfileString("TEST_RECORD","TOTAL",temp,Ini_File_Path);

				sprintf(temp,"%d",PassNo);
				::WritePrivateProfileString("TEST_RECORD","PASS",temp,Ini_File_Path);

				sprintf(temp,"%d",FailNo);
				::WritePrivateProfileString("TEST_RECORD","FAIL",temp,Ini_File_Path);

				m_StatusBar.Invalidate();  
				m_StatusBar.UpdateWindow();

			}
			else
				return true;//not to reset

		}
		else
			return true;//program is running
	}
	return true;
}

CQUALCOMM_IMEI_CODINGDlg *IMEI_Coding;
UINT AfxIMEICodingThread(LPVOID pParam)//LPVOID is a 32-bit pointer to an unspecified type
{  
	IMEI_Coding=(CQUALCOMM_IMEI_CODINGDlg *)pParam;//---->this
	
		
	Start_Flag=1;
	SetTimer(IMEI_Coding->m_hWnd,1,1000,NULL);
	IMEI_Coding->TimeMeter(1,true);
	SPU_RESULT res = SPU_SUCCESS;
	vector <std::string> scriptFile;
	scriptFile.push_back(_T("IMEI_CodingInit.script"));
	scriptFile.push_back(_T("IMEI_ReadMobileID.script"));
	if(IMEI_Coding->m_bBRTBackup1)
		scriptFile.push_back(_T("IMEI_BackupBRT_0.script"));
	scriptFile.push_back(_T("IMEI_ReadIMEIFromPhone.script"));
	if(!IMEI_Coding->m_bTranciver)
		scriptFile.push_back(_T("IMEI_CheckMALPNUseSSS02.script"));
	scriptFile.push_back(_T("IMEI_ResetSPCtoDefalt.script"));
	if(!IMEI_Coding->m_bTranciver)
		scriptFile.push_back(_T("IMEI_WriteImeiBtGsn.script"));
	scriptFile.push_back(_T("IMEI_WritePicassoTranceiver.script"));
	if(IMEI_Coding->m_bBRTBackup2)
		scriptFile.push_back(_T("IMEI_BackupBRT.script"));

	
	if(!IMEI_Coding->m_bTranciver)
	{
	scriptFile.push_back(_T("IMEI_WriteFSNFreeze.script"));
	scriptFile.push_back(_T("IMEI_WriteSimData.script"));

	scriptFile.push_back(_T("IMEI_WriteSPC.script"));
	}
	scriptFile.push_back(_T("IMEI_WriteKLFByte.script"));
	scriptFile.push_back(_T("IMEI_BackUpNV.script"));
	scriptFile.push_back(_T("IMEI_ResetHandset.script"));

	std::vector<std::string>::iterator firstIter = scriptFile.begin();

	while(firstIter!=scriptFile.end()&&res==SPU_SUCCESS)
	{
		//CString strMessage = _T("\r\n");
		//strMessage += firstIter->c_str();
		//strMessage += _T("\r\n");
		//LogPolicy::Log(strMessage, _T("SPU_RunTime.csv"), false);

		res = IMEI_Coding->ExecuteSPU(*firstIter);
		firstIter++;
	}

	IMEI_Coding->ExecuteSPU(_T("IMEI_ChangeUSBLogName.script"));

	if(res==SPU_FAIL)
	{
		return false;
	}
	IMEI_Coding->DealIMEICodingEnd();
return true;
}

SPU_RESULT CQUALCOMM_IMEI_CODINGDlg::ExecuteSPU(const TSTRING& szScript)
{
	SPU_RESULT res = SPU_SUCCESS;
	HANDLE hSPU = GetSPULoader(this);
	if(hSPU)
	{
		if(LoadScript(hSPU,szScript))
			res = ExecuteCommand(hSPU);
		ReleaseHandle(hSPU);
	}
	return res;
}
void CQUALCOMM_IMEI_CODINGDlg::InsertStringIntoList(CString szStrng)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString szTime;
	szTime.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d]: "),time.wYear,time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	int nCount = m_LIST.GetCount();
	m_LIST.InsertString(nCount, szTime+szStrng);
	m_LIST.SetCurSel(nCount);
	Log_String+=szTime+szStrng;
	Log_String+="\n";
}

bool CQUALCOMM_IMEI_CODINGDlg::CheckSWVersion()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));

	InsertStringIntoList(_T("Check software version......"));

	CString Software_Version_Temp=Software_Version;
	while(Software_Version_Temp.Find(_T('.')) != -1)
	{
		Software_Version_Temp.Remove(_T('.'));
	}
	CString Software_Version_Phone_Temp = SW_Version_In_Mobile;
	while(Software_Version_Phone_Temp.Find(_T('.')) != -1)
	{
		Software_Version_Phone_Temp.Remove(_T('.'));
	}

	if((Software_Version_Temp!=Software_Version_Phone_Temp))
	{
		Error_Code_String="M40";
		errFlag = true;
		sprintf(temp_log,"Check software version fail: Mobile------->%s  Select------->%s",
			SW_Version_In_Mobile,
			Software_Version);
		InsertStringIntoList(temp_log);

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);
		return false;
	}
	SW_Version_In_Mobile.Insert(1,'.');
	sprintf(temp_log,"Check software version pass: Mobile------->%s  Select------->%s",
		SW_Version_In_Mobile,
		Software_Version);
	InsertStringIntoList(temp_log);
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::CheckHWVersion()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	InsertStringIntoList(_T("Check hardware version......"));
	if(Hardware_Version!=HW_Version_In_Mobile)
	{
		sprintf(temp_log,"Check hardware version fail: Mobile------->%s  Select------->%s",
			HW_Version_In_Mobile,
			Hardware_Version);
		InsertStringIntoList(temp_log);

		Error_Code_String="M41";
		errFlag = true;
		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}
	sprintf(temp_log,"Check hardware version pass:Mobile------->%s  Select------->%s",
		HW_Version_In_Mobile,
		Hardware_Version);
	InsertStringIntoList(temp_log);
	return true;

}

bool CQUALCOMM_IMEI_CODINGDlg::CheckMEVersion()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	if(Target_Type=="PHONE")//there is no ME version for MODULE
	{
		InsertStringIntoList(_T("Check mechanical version......"));

		if(Mechanical_Version!=ME_Version_In_Mobile)
		{
			sprintf(temp_log,"Check mechanical version fail:Mobile------->%s  Select------->%s",
				ME_Version_In_Mobile,
				Mechanical_Version);
			InsertStringIntoList(temp_log);
			errFlag = true;
			Error_Code_String="M42";

			sprintf(temp_log,"IMEI coding fail,Error code-->",Error_Code_String);
			InsertStringIntoList(temp_log);

			return false;
		}
		sprintf(temp_log,"Check mechanical version pass:Mobile------->%s  Select------->%s",
			ME_Version_In_Mobile,
			Mechanical_Version);
		InsertStringIntoList(temp_log);
	}
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::CheckProcess()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));

	InsertStringIntoList(_T("Check process......"));

	if(BASEBAND_TEST_CHECK_FLAG&&(!(KLF_Byte&0x01)))
	{   
		Error_Code_String="M93-1";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----base band has not been tested yet......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}

	if(CAL_PRETEST_2G_CHECK_FLAG&&(!(KLF_Byte&0x02)))
	{   
		Error_Code_String="M93-2";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----cal&pre(2G) has not been tested yet......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);
		return false;
	}

	if(CAL_PRETEST_3G_CHECK_FLAG&&(!(KLF_Byte&0x04)))
	{   
		Error_Code_String="M93-3";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----cal&pre(3G) has not been tested yet......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}

	if(LEAKAGE_CHARGER_CHECK_FLAG&&(!(KLF_Byte&0x08)))
	{   
		Error_Code_String="M93-4";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----leackage&charger has not been tested yet......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}

	if((Target_Type=="PHONE")&&FUNCTION_WIRELESS_TEST_CHECK_FLAG&&(!(KLF_Byte&0x10)))
	{   
		Error_Code_String="M93-5";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----function&wireless has not been tested yet......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}

	if((Target_Type=="PHONE")&&PPF_CHECK_FLAG&&(!(KLF_Byte&0x20)))
	{   
		Error_Code_String="M93-6";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----PPF has not been wrote yet......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}

	if(ACM_BOARD_CHECK_FLAG&&(!(KLF_Byte&0x40)))
	{   
		Error_Code_String="M93-7";
		errFlag = true;
		InsertStringIntoList(_T("Check process fail----thie board is a ACM board......"));

		sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
		InsertStringIntoList(temp_log);

		return false;
	}

	InsertStringIntoList(_T("Check process success......"));
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::IMEIUpdateMobileID(const char *szFactoryArea)
{
	BYTE  temp[256] = {0};
	int nDataLen = Base64DecodeGetRequiredLength(strlen(szFactoryArea));
	memset(temp, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactoryArea, strlen(szFactoryArea), temp, &nDataLen) == TRUE;
	if(!bRes)
		return bRes;
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	unsigned char ID_Flag=0;
	unsigned char mobileID[12];
	unsigned char hw_version[6];
	unsigned char sw_version[9];
	ZeroMemory(mobileID,sizeof(mobileID));
	ZeroMemory(hw_version,sizeof(hw_version));
	ZeroMemory(sw_version, sizeof(sw_version));

//SW Version

	for(int i = 0; i < 4; i++)
	{
		sw_version[2*i]=(temp[85+i] >> 4) + '0';
		sw_version[i*2+1] = (temp[85+i] & 0x0F) +'0';
	}
	sw_version[8] = '\0';
	SW_Version_In_Mobile = sw_version;
	SW_Version_In_Mobile.TrimRight(_T("0"));
	sprintf(temp_log,"Read SW version success,SW version:%s",SW_Version_In_Mobile);
	InsertStringIntoList(temp_log);

	if(SW_CHECK)
	{
		if(!CheckSWVersion())
			return false;
	}
	else
	{
		InsertStringIntoList(_T("Check software version is skipped......"));
	}

//HW Version
	for(int i=0;i<=4;i++)//89 to 93 is for HW version
	{
		hw_version[i]=temp[89+i];
	}
	hw_version[5]='\0';
	HW_Version_In_Mobile=hw_version;

	sprintf(temp_log,"Read HW version success,HW version:%s",HW_Version_In_Mobile);
	InsertStringIntoList(temp_log);
	if(HW_CHECK)
	{
		if(!CheckHWVersion())
			return false;
	}
	else
	{
		InsertStringIntoList("Check hardware version is skipped......");
	}

//ME Version
	ME_Version_In_Mobile.Format(_T("%C"),temp[84]);//byte 84 is for ME version
	sprintf(temp_log,"ME version------->%s",ME_Version_In_Mobile);
	InsertStringIntoList(temp_log);
	if(ME_CHECK)
	{
		if(!CheckMEVersion())
			return false;
	}
	else
	{
		InsertStringIntoList("Check mechanical version is skipped......");
	}

//Process
	KLF_Byte=temp[3];//byte 3 is for KLF byte
	sprintf(temp_log,"KLF byte------->0x%02X",KLF_Byte);
	InsertStringIntoList(temp_log);
	if(PROCESS_CHECK)
	{
		if(!CheckProcess())
			return false;
	}
	else
	{
		InsertStringIntoList("Check process is skipped......");
	}
//Mobile ID
	ID_Flag=temp[52];//byte 7 for ID Flag
	if(ID_Flag==0x00)
	{
		sprintf(temp_log,"Mobile ID flag-->0x%02X namely mobile ID does not exist, it is empty",ID_Flag);
		InsertStringIntoList(temp_log);
		Mobile_ID="";
		Error_Code_String="M21";
		errFlag = true;
		return false;
	}
	else//ID_Flag==0xFF,means Mobile ID has been written
	{
		sprintf(temp_log,"Mobile ID flag------->0x%02X namely mobile ID exist",ID_Flag);
		InsertStringIntoList(temp_log);

		int nMobileIDLength = ::GetPrivateProfileInt("LENGTH_SETTING","MOBILE_ID_LENGTH",11,Ini_File_Path);
		for(int i=0;i<nMobileIDLength;i++)
		{
			mobileID[i]=temp[45+i];//byte 45 to 55 is for Mobile ID
		}
		mobileID[11]='\0';
		CString MobileID_Check=mobileID;
		if(MobileID_Check.IsEmpty())
		{
			InsertStringIntoList("The mobile ID is empty......");

			Mobile_ID="";
			InsertStringIntoList("Read Mobile ID Fail....");
			Error_Code_String="M30";
			errFlag = true;
			return false;
		}
		else
		{
			InsertStringIntoList("Read mobile ID success......");
		}

		sprintf(temp_log,"The mobile ID from phone------->0x%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
			mobileID[0],
			mobileID[1],
			mobileID[2],
			mobileID[3],
			mobileID[4],
			mobileID[5],
			mobileID[6],
			mobileID[7],
			mobileID[8],
			mobileID[9],
			mobileID[10]);
		InsertStringIntoList(temp_log);

		//change the Mobile ID from 0x to decimal
		unsigned char FirstNibble;
		unsigned char SecondNibble;

		unsigned char mobileID_temp[23];
		ZeroMemory(mobileID_temp,sizeof(mobileID_temp));
		int j=0;
		for (int i=0;i< nMobileIDLength;i++)							
		{							
			FirstNibble = (unsigned char)(mobileID[i] >> 4)+'0';
			SecondNibble = (unsigned char)(mobileID[i] & 0x0F)+'0';					

			mobileID_temp[j++] = FirstNibble;
			mobileID_temp[j++] = SecondNibble;					
		}
		mobileID_temp[22]='\0';
		Mobile_ID=mobileID_temp;
		USBLogName = mobileID_temp;
		USBLogName.Replace(':', '_');
		USBLogName += ".txt";
		InsertStringIntoList("The mobile ID after conversion------->"+Mobile_ID);

		m_MOBILEID2.SetWindowText(Mobile_ID);
	}
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::IMEICheckIMEIIsValid(const char* szIMEI)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	CString strIMEI = szIMEI;

	if(strIMEI.GetLength()==0)
	{
		memcpy(IMEI_From_Phone, _T("000000011234560"),sizeof(IMEI_From_Phone));
	}else
	{
		memcpy(IMEI_From_Phone,strIMEI.GetBuffer(),strIMEI.GetLength());
	}
	IMEI_From_Phone[15]='\0';

	InsertStringIntoList(_T("IMEI----->")+strIMEI);
//	Lock_Status=false;

	if(m_bTranciver)
	{
		if(!::strcmp(IMEI_From_Phone, Dummy_IMEI))
		{
			InsertStringIntoList("The IMEI read from phone is a dummy IMEI......");
			return true;
		}
		else
		{
			InsertStringIntoList("The IMEI read from phone is not a dummy IMEI......");
			Error_Code_String = _T("F67");
			errFlag = true;
			InsertStringIntoList("IMEI coding fail, Error code-->"+Error_Code_String);
			return false;
		}
	}
	else
	{
		if(!::strcmp(IMEI_From_Phone, Dummy_IMEI) || !strcmp(IMEI_From_Phone, Zero_IMEI))
		{
			//check difference
			if(!ButtonStart_Flag)//not check in the first time
			{
				if(!CheckIMEIDifference(IMEI_FROM_SCANNER))
				{
					CheckDiff=1;
					CPassWord PassWordDlg;
					if(PassWordDlg.DoModal()==IDCANCEL)
					{
						CheckDiff=0;
						InsertStringIntoList("IMEI coding fail......");

						::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
						return false;
					}
					CheckDiff=0;
				}
			}
			ButtonStart_Flag=0;
		}
		else
		{
			if(strcmp((char *)IMEI_From_Phone,IMEI_FROM_SCANNER))//the IMEI in phone is not eqaul to the IMEI from scaaner
			{
				Error_Code_String="M58";//IMEI in phone is not 
				errFlag = true;
				sprintf(temp_log,"The IMEI from phone(%s) is not equal to the IMEI you scaned(%s)......",IMEI_From_Phone,IMEI_FROM_SCANNER);
				InsertStringIntoList(temp_log);

				InsertStringIntoList("IMEI coding fail, Error code-->"+Error_Code_String);

				return false;
			}
		}	
	}
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::IMEICheckPPFFILE(const char *szFactoryArea)
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	CString szCEFSMobile = szFactoryArea;

	szCEFSMobile.MakeLower();
	QVCEFS_File_Name.MakeLower();

	if(szCEFSMobile != QVCEFS_File_Name)
	{
		InsertStringIntoList(_T("qvcefs name not equal Mobile: ") + szCEFSMobile + _T("; Select: ......") + +QVCEFS_File_Name);
		Error_Code_String="M94-1";
		errFlag = true;
		return false;
	}

	/*unsigned char temp1[128];
	unsigned char temp[101];
	unsigned char temp2[128];
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	ZeroMemory(temp,sizeof(temp));
	ZeroMemory(temp2,sizeof(temp2));
	ZeroMemory(temp1,sizeof(temp1));
	int nDataLen = Base64DecodeGetRequiredLength(128);
	memset(temp1, 0x00, nDataLen);

	bool bRes = Base64Decode(szFactoryArea, 128, temp1, &nDataLen) == TRUE;
	if(!bRes)
		return bRes;


	CString File_Temp="";
	CString Time_Temp1="";
	CString Time_Temp2="";
	CString Size_Temp1="";
	CString Size_Temp2="";

	int i=0;
	int index=0;

	sprintf(temp_log,"Read Variant info success......",Port_Number);
	InsertStringIntoList(temp_log);

	for(i=0;i<100;i++)//0 to 100, total 100 byte is for QVCEFS file name
	{
		temp[i]=temp1[i];
	}
	temp[100]='\0';
	File_Temp=temp;

	if((index=File_Temp.Find(".qvcefs"))==-1)
	{
		sprintf(temp_log,"variant file name wrong......",Port_Number);
		InsertStringIntoList(temp_log);
		Error_Code_String="M94-1";
		errFlag = true;
		return false;
	}

	if(QVCEFS_File!="" && QVCEFS_File!="N/A")
	{
		sprintf(temp_log,"Check QVCEFS file time......",Port_Number);
		InsertStringIntoList(temp_log);

		ZeroMemory(temp,sizeof(temp));
		for(i=101;i<114;i++)     //total 14 bytes is for .qvcefs file time
		{
			temp[i-101]=temp1[i];
		}
		temp[14]='\0';
		Time_Temp1=temp;
		Time_Temp2=QVCEFS_File_Time;
		Time_Temp2.Remove('/');
		Time_Temp2.Remove(' ');
		Time_Temp2.Remove(':');

		sprintf(temp_log,"QVCEFS file time from mobile------->%s",Port_Number,Time_Temp1);
		InsertStringIntoList(temp_log);

		sprintf(temp_log,"QVCEFS file time you select------->%s",Port_Number,Time_Temp2);
		InsertStringIntoList(temp_log);

		if(Time_Temp1.Left(12)==Time_Temp2.Left(12))
		{
			sprintf(temp_log,"They are the same, check QVCEFS file time success......",Port_Number);
			InsertStringIntoList(temp_log);
		}
		else
		{
			sprintf(temp_log,"They are not the same, check QVCEFS file time fail......",Port_Number);
			InsertStringIntoList(temp_log);

			Error_Code_String="M94-2";
			errFlag = true;
			return false;
		}

		sprintf(temp_log,"Check QVCEFS file size......",Port_Number);
		InsertStringIntoList(temp_log);

		ZeroMemory(temp,sizeof(temp));
		for(i=114;i<119;i++)//total 6 bytes is for .qvcefs file size
		{
			temp[i-114]=temp1[i];
		}
		temp[6]='\0';
		Size_Temp1=temp;
		Size_Temp2=QVCEFS_File_Size;
		Size_Temp2.Remove(',');

		sprintf(temp_log,"QVCEFS file size from mobile------->%s",Port_Number,Size_Temp1);
		InsertStringIntoList(temp_log);

		sprintf(temp_log,"QVCEFS file size you select------->%s",Port_Number,Size_Temp2);
		InsertStringIntoList(temp_log);

		if(Size_Temp1==Size_Temp2)
		{
			sprintf(temp_log,"They are the same, check QVCEFS file size success......",Port_Number);
			InsertStringIntoList(temp_log);
		}
		else
		{
			sprintf(temp_log,"They are not the same, check QVCEFS file size fail......",Port_Number);
			InsertStringIntoList(temp_log);

			Error_Code_String="M94-3";
			errFlag = true;
			return false;
		}
	}
*/
	return true;
}
void CQUALCOMM_IMEI_CODINGDlg::SetLog()
{
	//CTime Time=CTime::GetCurrentTime();
 //   CString szTIME=Time.Format("%Y%m%d");
	//CreateDirectory(Current_Directory+"\\log",NULL);
	//sprintf(Temp_Global,"%s\\log\\picsinterface_log",Current_Directory);
	//CreateDirectory(Temp_Global,NULL);
	//sprintf(Temp_Global,"%s\\log\\picsinterface_log\\%s.log",Current_Directory,szTIME);
	//if(Pics_Interface_Ctrl.EnableLog(LOG,Temp_Global))
	//{
	//	AfxMessageBox("Set pics interface log file fail!!");
	//}
 //   //create folder for imeicoding log file
 //   sprintf(Temp_Global,"%s\\log\\imeicoding_log",Current_Directory);
	//CreateDirectory(Temp_Global,NULL);
	//return;
}

BOOL CQUALCOMM_IMEI_CODINGDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
				return true;
				break;
			case VK_RETURN:
				if(pMsg->hwnd==GetDlgItem(IDC_EDIT_IMEI)->m_hWnd)
				{
					OnButtonStart();
					return true;
				}
                if(pMsg->hwnd==GetDlgItem(IDC_EDIT_MAL)->m_hWnd)
				{
					if(ATHB1A_Flag)
						m_PICASSO_EDIT.SetFocus();
					else
						m_IMEI_EDIT.SetFocus();
					return true;
				}
				if(pMsg->hwnd==GetDlgItem(IDC_EDIT_PICASSO)->m_hWnd)
				{
					m_IMEI_EDIT.SetFocus();
					return true;
				}
				break;
			default:
				break;
			}
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

unsigned char CQUALCOMM_IMEI_CODINGDlg::WritePICSFile()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	sprintf(temp_log,"============WRITE PICS FILE============");
	InsertStringIntoList(temp_log);

	CStdioFile file;
	CTime time;
	time=time.GetCurrentTime();
	CString picsfile,host;
    host=Computer_Name[1];
    ZeroMemory(Temp_Global,sizeof(Temp_Global));
	picsfile=ServerDirectory+"\\"+Model_Name;
	sprintf(Temp_Global,"%s%s%s.%sEID",picsfile,Mobile_ID,time.Format("_%H%M%S"),Model_Name);
	picsfile=Temp_Global; 
    sprintf(temp_log,"Write PICS file to %s......",picsfile);
	InsertStringIntoList(temp_log);

	if(!file.Open(picsfile,CFile::modeCreate | CFile::modeWrite))
	{
		sprintf(temp_log,"Write PICS file to %s fail......",picsfile);
		InsertStringIntoList(temp_log);
		Error_Code_String="M16";
		errFlag = true;
		return false;
	}
	//file.WriteString("3456\n");//1
	file.WriteString(SO_Str+"\n");//1
	file.WriteString(Mobile_ID+"\n");//2
	file.WriteString(Model_Name+"\n");//3	
    file.WriteString(host);
	host=Computer_Name[2];
    file.WriteString(host+"\n");//4
	host=Computer_Name[5];
    file.WriteString(host+"\n");//5
	file.WriteString("1\n");//6
	if(Error_Code_String=="PASS")
		file.WriteString("OK\n");
	else
		file.WriteString("NG\n");//7
	file.WriteString(Error_Code_String+"\n");//8
	file.WriteString(IMEI_FROM_SCANNER+"\n");//9
	/*
	file.WriteString("00000000\n");//10
	file.WriteString("00000000\n");//11
	file.WriteString("00000000\n");//12
	file.WriteString("00000000\n");//13
	file.WriteString("00000000\n");//14
	*/
	if(SECURITY_MECHANISM_FLAG)
	{
		file.WriteString((char *)BT_Network_Unlock_Code);//10
		file.WriteString("\n");
		file.WriteString((char *)BT_Service_Provider_Unlock_Code);//11
		file.WriteString("\n");
		file.WriteString((char *)BT_Coporate_Unlock_Code);//12
		file.WriteString("\n");
		file.WriteString((char *)BT_Network_Subset_Unlock_Code);//13
		file.WriteString("\n");
		file.WriteString((char *)BT_User_SIM_Unlock_Code);//14
		file.WriteString("\n");
		file.WriteString((char *)BT_User_USIM_Unlock_Code);//15
		file.WriteString("\n");
		file.WriteString((char *)Serial_Code);//16
		file.WriteString("\n");
		file.WriteString((char *)BT_Phone_Lock_Code);//17
		file.WriteString("\n");
	}
	else
	{
		file.WriteString("00000000\n");//10
		file.WriteString("00000000\n");//11
		file.WriteString("00000000\n");//12
		file.WriteString("00000000\n");//13
		file.WriteString("00000000\n");//14
		file.WriteString("00000000\n");//15
		file.WriteString("00000000\n");//16
		file.WriteString("00000000\n");//17
	}
/*	file.WriteString("00000000\n");//10
	file.WriteString("00000000\n");//11
	file.WriteString("00000000\n");//12
	file.WriteString("00000000\n");//13
	file.WriteString("00000000\n");//14
	file.WriteString("00000000\n");//15
	file.WriteString("00000000\n");//16
	file.WriteString("00000000\n");//17*/

	file.WriteString(GSN_String+"\n");//18
	file.WriteString(MAL_FROM_SCANNER+"\n\n");//19-20
    file.WriteString(Part_Number+"\n");//21
	file.WriteString(Hardware_Version+"\n");//22
	file.WriteString(Software_Version+"\n");//23
	file.WriteString(Hardware_Version+"\n\n");//24-25
	file.WriteString(Tool_Version+"\n");//26
	file.WriteString(Config_Version+"\n");//27
    CString PPF_File_Name;
	CString PPF_File_Time;
	CString PPF_File_Size;

//    m_QVCEFS2.GetWindowText(PPF_File_Name);
//	file.WriteString(PPF_File_Name+"\n");//28----QVCEFS file name
	file.WriteString(QVCEFS_File+"\n");//28----QVCEFS file name
	m_TIME_STAMP_QVCEFS2.GetWindowText(PPF_File_Time);
    file.WriteString(PPF_File_Time+"\n");//29----QVCEFS file time
    m_FILE_SIZE_QVCEFS2.GetWindowText(PPF_File_Size);
    file.WriteString(PPF_File_Size+"\n");//30----QVCEFS file size

    
//	m_QSNV2.GetWindowText(PPF_File_Name);
//	file.WriteString(PPF_File_Name+"\n");//31----QSNV file name
	file.WriteString(QSNV_File+"\n");//31----QSNV file name
	m_TIME_STAMP_QSNV2.GetWindowText(PPF_File_Time);
    file.WriteString(PPF_File_Time+"\n");//32----QSNV file time
	m_FILE_SIZE_QSNV2.GetWindowText(PPF_File_Size);
    file.WriteString(PPF_File_Size+"\n");//33----QSNV file size
   
//    m_QZCS2.GetWindowText(PPF_File_Name);
//	file.WriteString(PPF_File_Name+"\n");//34----QZCS file name
	file.WriteString(QZCS_File+"\n");//34----QZCS file name
	m_TIME_STAMP_QZCS2.GetWindowText(PPF_File_Time);
    file.WriteString(PPF_File_Time+"\n");//35----QZCS file time
	m_FILE_SIZE_QZCS2.GetWindowText(PPF_File_Size);
    file.WriteString(PPF_File_Size+"\n");//36----QZCS file size

//    m_DUCK2.GetWindowText(PPF_File_Name);
//	file.WriteString(PPF_File_Name+"\n");//37----DUCK file name
	file.WriteString(DUCK_File+"\n");//37----DUCK file name
	m_TIME_STAMP_DUCK2.GetWindowText(PPF_File_Time);
    file.WriteString(PPF_File_Time+"\n");//38----DUCK file time
	m_FILE_SIZE_DUCK2.GetWindowText(PPF_File_Size);
    file.WriteString(PPF_File_Size+"\n");//39----DUCK file size	

    file.WriteString(BT_Address+"\n");//40
	file.WriteString(Mechanical_Version+"\n");//41
	file.WriteString(TRANS_NO+"\n");//42
	file.WriteString(Picasso_No+"\n");//43
    for(int j=0;j<50;j++)
              file.WriteString(PNva[j]);
    file.Close();
	sprintf(temp_log,"Write PICS file to %s success......",picsfile);
	InsertStringIntoList(temp_log);
	return true;
}

void CQUALCOMM_IMEI_CODINGDlg::SetStatusBar()
{
	#define IDS_STATUS        AFX_IDW_STATUS_BAR
	char temp[25];
    ZeroMemory(temp,sizeof(temp));
	VERIFY(StatusFont.CreateFont(
								14,                        // nHeight
								0,                         // nWidth
								0,                         // nEscapement
								700,                         // nOrientation
								FW_NORMAL,                 // nWeight
								FALSE,                     // bItalic
								FALSE,                     // bUnderline
                                0,                         // cStrikeOut
                                ANSI_CHARSET,              // nCharSet
								OUT_DEFAULT_PRECIS,        // nOutPrecision
								CLIP_DEFAULT_PRECIS,       // nClipPrecision
								DEFAULT_QUALITY,           // nQuality
								DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
								"Arial"));                 // lpszFacename

	//StatusFont.CreatePointFont(80, "Arial"); 
	//Method 1:
	/*
	Status_WND=CreateStatusWindow(WS_CHILD|WS_VISIBLE|WS_BORDER,
								  TEXT("TEST RECORD"),
								  m_hWnd,
                                  IDS_STATUS); 
	int point[5]={200,400,600,800,-1};//the width of per part is 200
    ::SendMessage(Status_WND,SB_SETPARTS,5,(LPARAM)point);
    //to set the text for the seperated bar
	sprintf(temp,"TEST TOTAL:%d",TotalNo);
	::SendMessage(Status_WND,SB_SETTEXT,1,(LPARAM)TEXT(temp));
	sprintf(temp,"TEST PASS:%d",PassNo);
	::SendMessage(Status_WND,SB_SETTEXT,2,(LPARAM)TEXT(temp));
	sprintf(temp,"TEST FAIL:%d",FailNo);
	::SendMessage(Status_WND,SB_SETTEXT,3,(LPARAM)TEXT(temp));
	::SendMessage(Status_WND,SB_SETTEXT,4,(LPARAM)TEXT("RESET TEST RECORD"));
	*/
    
	//Method 2:
	/*
	//m_StatusBar=new   CStatusBarCtrl;
	RECT     m_Rect;   
	GetClientRect(&m_Rect);         
	m_Rect.top=m_Rect.bottom-5;//height of status bar 
	//m_StatusBar->Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,IDS_STATUS);  
	m_StatusBar.Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,IDS_STATUS); 
    
	int Width=(m_Rect.right-m_Rect.left)/5;
	int nParts[5]= {Width-50, 2*Width, 3*Width+50,4*Width+50,-1}; 
  
	//VERIFY(m_StatusBar->SetParts(5, nParts));
	VERIFY(m_StatusBar.SetParts(5, nParts));

	//m_StatusBar->SetText("TEST RECORD",0,0);
	m_StatusBar.SetText("",0,SBT_OWNERDRAW);

	sprintf(temp,"TEST TOTAL:%d",TotalNo);
    //m_StatusBar->SetText(temp,1,0); 
	m_StatusBar.SetText("",1,SBT_OWNERDRAW); 

	sprintf(temp,"TEST PASS:%d",PassNo);
	//m_StatusBar->SetText(temp,2,0); 
	m_StatusBar.SetText("",2,SBT_OWNERDRAW);

	sprintf(temp,"TEST FAIL:%d",FailNo);
    //m_StatusBar->SetText(temp,3,0); 
	m_StatusBar.SetText("",3,SBT_OWNERDRAW);

	//m_StatusBar->SetText("RESET TEST RECORD",4,0);
	m_StatusBar.SetText("",4,SBT_OWNERDRAW);

	//m_StatusBar->SetBkColor(RGB(200,200,100));
	m_StatusBar.SetBkColor(RGB(200,200,100));

	//m_StatusBar->ShowWindow(SW_SHOW); 
	//m_StatusBar.ShowWindow(SW_SHOW);
	*/
	
	//Method3:
	
	unsigned int IDArray[5]={100,101,102,103,104};
	RECT     m_Rect;   
	GetClientRect(&m_Rect);
	m_StatusBar.Create(this,WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,IDS_STATUS);
    m_StatusBar.SetIndicators(IDArray,sizeof(IDArray)/sizeof(unsigned int));
	int Width=(m_Rect.right-m_Rect.left)/5;
    for(int n=0;n<5;n++)

    {
	   if(n==0)
		   m_StatusBar.SetPaneInfo(n,IDArray[n],0,Width-50); 
	   else
		   m_StatusBar.SetPaneInfo(n,IDArray[n],0,Width+20); 
    }
    
    m_StatusBar.SetPaneText(0,"TEST RECORD");
	m_StatusBar.GetStatusBarCtrl().SetText("", 0, SBT_OWNERDRAW); 
    
	sprintf(temp,"TEST TOTAL:%d",TotalNo);
    //m_StatusBar.SetPaneText(1,temp);
	//m_StatusBar.GetStatusBarCtrl().SetText(temp, 1, SBT_OWNERDRAW); 
	m_StatusBar.GetStatusBarCtrl().SetText("", 1, SBT_OWNERDRAW); 
    
	sprintf(temp,"TEST PASS:%d",PassNo);
    //m_StatusBar.SetPaneText(2,temp);
	//m_StatusBar.GetStatusBarCtrl().SetText(temp, 2, SBT_OWNERDRAW); 
	m_StatusBar.GetStatusBarCtrl().SetText("", 2, SBT_OWNERDRAW);

	sprintf(temp,"TEST FAIL:%d",FailNo);
	//m_StatusBar.SetPaneText(3,temp);
	//m_StatusBar.GetStatusBarCtrl().SetText(temp, 3, SBT_OWNERDRAW); 
	m_StatusBar.GetStatusBarCtrl().SetText("", 3, SBT_OWNERDRAW);

    //m_StatusBar.SetPaneText(4,"RESET TEST RECORD");
	//m_StatusBar.GetStatusBarCtrl().SetText("RESET TEST RECORD", 4, SBT_OWNERDRAW); 
	m_StatusBar.GetStatusBarCtrl().SetText("", 4, SBT_OWNERDRAW); 

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0); //show status bar

    m_StatusBar.GetStatusBarCtrl().SetBkColor(RGB(200,200,100));

	m_StatusBar.SetFont(&StatusFont,1);
	
	//ceter the text by insert space
	/*
	CDC *pDC=GetDC(); 
	CRect ItemRect;
	TEXTMETRIC Text_Struc;
	m_StatusBar.GetItemRect(0,ItemRect);
	MapWindowPoints(GetDesktopWindow(),&ItemRect);
	ScreenToClient(&ItemRect);
	pDC->GetTextMetrics(&Text_Struc);
	unsigned int nMaxchar=ItemRect.Width()/Text_Struc.tmMaxCharWidth;
	int i=0;
    CString String_Text="TEST RECORD";
	if(nMaxchar>(unsigned int)String_Text.GetLength())   
	{   
        i=(nMaxchar-String_Text.GetLength())/2;   
        for(;i;i--)   
           String_Text.Insert(0,TCHAR(" "));   
	}   
	m_StatusBar.SetPaneText(0,String_Text);
	pDC->DrawText("TEST RECORD",-1,&ItemRect,DT_CENTER);
	ReleaseDC(pDC);
*/
	

	//Method4:
	/*RECT     m_Rect;   
	GetClientRect(&m_Rect);         
	m_Rect.top=m_Rect.bottom-5;//height of status bar 
	m_StatusBar.GetStatusBarCtrl().Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,IDS_STATUS); 
    int Width=(m_Rect.right-m_Rect.left)/5;
	int nParts[5]= {Width-50, 2*Width, 3*Width+50,4*Width+50,-1}; 
  
	VERIFY(m_StatusBar.GetStatusBarCtrl().SetParts(5, nParts));
	m_StatusBar.GetStatusBarCtrl().SetText("",0,SBT_OWNERDRAW);

	sprintf(temp,"TEST TOTAL:%d",TotalNo);
    m_StatusBar.GetStatusBarCtrl().SetText("",1,SBT_OWNERDRAW); 

	sprintf(temp,"TEST PASS:%d",PassNo);
	m_StatusBar.GetStatusBarCtrl().SetText("",2,SBT_OWNERDRAW); 

	sprintf(temp,"TEST FAIL:%d",FailNo);
    m_StatusBar.GetStatusBarCtrl().SetText("",3,SBT_OWNERDRAW); 

	m_StatusBar.GetStatusBarCtrl().SetText("",4,SBT_OWNERDRAW);
	m_StatusBar.GetStatusBarCtrl().SetBkColor(RGB(200,200,100));*/

    //Note:to response the left button double click, there are 2 methosd:
	//     1)To pretranslatemessage in CQUALCOMM_FLEX_OPTIONApp; --->we use
	//     2)To send left button double click message, it will be solved in CColoredStatusBarCtrlCtrl, but we must use method 2 to set status bar
	return;

}

void CQUALCOMM_IMEI_CODINGDlg::SetTestRecord(CString ErrorCode)
{
	char temp[25];
	ZeroMemory(temp,sizeof(temp));
	if(ErrorCode=="PASS")//pass
	{
		TotalNo++;
		PassNo++;
	}
	else/* if((ErrorCode!="F10")&&(ErrorCode!="F40")&&(ErrorCode!="F54")&&
			(ErrorCode!="F57")&&(ErrorCode!="F58")&&(ErrorCode!="F59")&&
			(ErrorCode!="F68")&&(ErrorCode!="F70")&&(ErrorCode!="F75")&&
			(ErrorCode!="F80")&&(ErrorCode!="F81")&&(ErrorCode!="F82")&&
			(ErrorCode!="F83")&&(ErrorCode!="F89")&&(ErrorCode!="F90")&&
			(ErrorCode!="F91")&&(ErrorCode!="F92")&&(ErrorCode!="F94")&&
			(ErrorCode!="F97")&&(ErrorCode!="F98")&&(ErrorCode!="F99")&&
			(ErrorCode!="F100")&&(ErrorCode!="F102")&&(ErrorCode!="M40")&&
			(ErrorCode!="M41")&&(ErrorCode!="M42")&&(ErrorCode!="M57")&&
			(ErrorCode!="M58")&&(ErrorCode!="M59")&&(ErrorCode!="M93-1")&&
			(ErrorCode!="M93-2")&&(ErrorCode!="M93-3")&&(ErrorCode!="M93-4")&&
			(ErrorCode!="M93-5")&&(ErrorCode!="M93-6")&&(ErrorCode!="M93-7")&&
			(ErrorCode!="M94-1")&&(ErrorCode!="M94-2")&&(ErrorCode!="M94-3")&&
			(ErrorCode!="M95-1")&&(ErrorCode!="M95-2")&&(ErrorCode!="M95-3")&&
			(ErrorCode!="M96-1")&&(ErrorCode!="M96-2")&&(ErrorCode!="M96-3")&&
			(ErrorCode!="M97-1")&&(ErrorCode!="M97-2")&&(ErrorCode!="M97-3"))//fail and error code !=.......*/
	{
		TotalNo++;
		FailNo++;
	}
	
    sprintf(temp,"%u",TotalNo);
	::WritePrivateProfileString("TEST_RECORD","TOTAL",temp,Ini_File_Path);

    sprintf(temp,"%u",PassNo);
	::WritePrivateProfileString("TEST_RECORD","PASS",temp,Ini_File_Path);

	sprintf(temp,"%u",FailNo);
	::WritePrivateProfileString("TEST_RECORD","FAIL",temp,Ini_File_Path);

	sprintf(Total_String,"TEST TOTAL:  %d",TotalNo);
	sprintf(Pass_String,"TEST PASS:  %d",PassNo);
	sprintf(Fail_String,"TEST FAIL:  %d",FailNo);

	m_StatusBar.Invalidate();  
	m_StatusBar.UpdateWindow();
	return;
}

void CQUALCOMM_IMEI_CODINGDlg::ShowPicassoTrans()
{
	if(!ATHB1A_Flag)
	{
		m_PICASSO.ShowWindow(false);
		m_PICASSO_EDIT.ShowWindow(false);
		m_TRANS1.ShowWindow(false);
		m_TRANS2.ShowWindow(false);

		CRect MAL_Rect;
		CRect IMEI_Rect;
		CRect MOBILEID_Rect;

		m_MAL_EDIT.GetWindowRect(MAL_Rect); 
		m_IMEI_EDIT.GetWindowRect(IMEI_Rect);
		m_MOBILEID2.GetWindowRect(MOBILEID_Rect);

	    m_MAL_EDIT.SetWindowPos(NULL,0,0,IMEI_Rect.Width()*2+108,IMEI_Rect.Height(),SWP_NOMOVE|SWP_NOZORDER);
	    m_MOBILEID2.SetWindowPos(NULL,0,0,IMEI_Rect.Width()*2+108,IMEI_Rect.Height(),SWP_NOMOVE|SWP_NOZORDER);
	}
}

bool CQUALCOMM_IMEI_CODINGDlg::GetEXCEL()
{
     CString strPath = "\\\\10.94.5.17\\ATHB1A\\";
	 strPath+=SO_Str;
	 strPath+=".xls";
     char temp_log[512];

	 BOOL Binfind;
	 CFileFind file;
	 Binfind=file.FindFile(strPath,0);
	 if(!Binfind)
	 {
		 sprintf(temp_log,"Can't found matching file: %s,\nPlease contact IE/ZhouChao about this issue",strPath);
		 AfxMessageBox(temp_log);
		 return FALSE;
	 }

	 if(!exlapp.CreateDispatch("Excel.Application")) //启动服务器
	 {
		AfxMessageBox("无法启动Excel服务器!");
		return FALSE;
	 }
 
//   exlapp.SetVisible(TRUE);//?Excel可见
     exlapp.SetUserControl(TRUE);
	 COleVariant  avar((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
     wbks.AttachDispatch(exlapp.GetWorkbooks());
     lpDisp=wbks.Open(strPath,           
     avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar,avar);

     wbk.AttachDispatch(lpDisp);
     whts.AttachDispatch(wbk.GetWorksheets());
     lpDisp=wbk.GetActiveSheet();
     wht.AttachDispatch(lpDisp);

	 usedRange.AttachDispatch(wht.GetUsedRange());
	 range.AttachDispatch(usedRange.GetRows());
	 long iRowNum=range.GetCount();//已经使用的行数
	 range.AttachDispatch(wht.GetCells());

	 _variant_t va;
	 int i=1;
	 bool check=true;
	 CString ColNo1,ColNo5;

	 do
	 {
		i++;
        va=range.GetItem(_variant_t((long)i),_variant_t((long)2));
	    ColNo1 =(char *)_bstr_t(va);
		ColNo1.Remove(' ');
        va=range.GetItem(_variant_t((long)i),_variant_t((long)5));
	    ColNo5 =(char *)_bstr_t(va);
	    ColNo5.Remove(' ');
        if((ColNo5!="")&&(strncmp(ColNo1,ColNo5,12)!=0||ColNo5.GetLength()!=30))
		{
		   check=false;
           break;
		}
		PNva[i-2]=ColNo5+"\n";
	 }while(i<iRowNum+2);

	 wbks.ReleaseDispatch(); 
	 wbk.ReleaseDispatch(); 
     whts.ReleaseDispatch(); 
     wht.ReleaseDispatch(); 
     range.ReleaseDispatch(); 
     usedRange.ReleaseDispatch(); 
 	 wbk.Close(avar,COleVariant(strPath),avar);
  	 wbks.Close();
	 exlapp.ReleaseDispatch();
    
	 if(!check)
     {
		 sprintf(temp_log,"The Component %s, is not matching its carton.No %s, \nplease connect IE/ZhouChao",ColNo1,ColNo5);
		 AfxMessageBox(temp_log);
		 return FALSE;
	 }
     return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::Initial_IMEIStationDLL()
{
    hDll = ::LoadLibrary(_T("IMEIStation_StaticM.dll"));
	ASSERT(hDll);
    if(hDll == NULL) return false;
    
	pSetTestMode = (SetTestMode *)::GetProcAddress(hDll, "SetTestMode");
	pSetOnlineMode = (SetOnlineMode *)::GetProcAddress(hDll, "SetOnlineMode");

	pReadIMEI = (ReadIMEI *)::GetProcAddress(hDll, "ReadIMEI");
	pWriteIMEI = (WriteIMEI  *)::GetProcAddress(hDll, "WriteIMEI");

	pWriteSimData = (WriteSimData *)::GetProcAddress(hDll, "WriteSimData");

    pReadFSN = (ReadFSN *)::GetProcAddress(hDll, "ReadFSN");
	pWriteFSN = (WriteFSN *)::GetProcAddress(hDll, "WriteFSN");

    pReadSWVersion = (ReadSWVersion *)::GetProcAddress(hDll, "ReadSWVersion");

	pReadBTAddress = (ReadBTAddress *)::GetProcAddress(hDll, "ReadBTAddress");
	pWriteBTAddress = (WriteBTAddress *)::GetProcAddress(hDll, "WriteBTAddress");

	pReadFactoryArea = (ReadFactoryArea  *)::GetProcAddress(hDll, "ReadFactoryArea");
    pWriteFactoryArea = (WRITEFACTORYAREA  *)::GetProcAddress(hDll, "WriteFactoryArea");
    
	pFreezeOTP = (FreezeOTP  *)::GetProcAddress(hDll, "FreezeOTP");
//  pIsOTPFrozen = (IsOTPFrozen  *)::GetProcAddress(hDll, "IsOTPFrozen");

	pBackupNV = (BNV  *)::GetProcAddress(hDll, "BackupNV");
    pBackupBrt = (BackupBrt  *)::GetProcAddress(hDll, "BackupBrt");

	pUnlockPhoneCode = (UnlockPhoneCode  *)::GetProcAddress(hDll, "UnlockPhoneCode");
    pUnlockNW = (UnlockNW  *)::GetProcAddress(hDll, "UnlockNW");
	pUnlockNS = (UnlockNS  *)::GetProcAddress(hDll, "UnlockNS");
    pUnlockSP = (UnlockSP  *)::GetProcAddress(hDll, "UnlockSP");
	pUnlockCP = (UnlockCP  *)::GetProcAddress(hDll, "UnlockCP");
    pUnlockSIM = (UnlockSIM  *)::GetProcAddress(hDll, "UnlockSIM");
	pUnlockSPC = (UnlockSPC   *)::GetProcAddress(hDll, "UnlockSPC");

	pIsSSCPLocked = (IsSSCPLocked  *)::GetProcAddress(hDll, "IsSSCPLocked");
	pIsNWSCPLocked = (IsNWSCPLocked  *)::GetProcAddress(hDll, "IsNWSCPLocked");
    pIsSPCLocked = (IsSPCLocked  *)::GetProcAddress(hDll, "IsSPCLocked");
	pWriteSPC = (WRITESPC   *)::GetProcAddress(hDll, "WriteSPC");
    pReadSPC = (READSPC   *)::GetProcAddress(hDll, "ReadSPC");
	pLockSPC = (LockSPC   *)::GetProcAddress(hDll, "LockSPC");
	pIsConnected = (IsConnected   *)::GetProcAddress(hDll, "IsConnected");
    pResetHandset = (ResetHandset   *)::GetProcAddress(hDll, "ResetHandset");
    pGetReleaseDateTime = (GetReleaseDateTime   *)::GetProcAddress(hDll, "GetReleaseDateTime");


	hDll2 = ::LoadLibrary(_T("SimEncorder.dll"));
	ASSERT(hDll2);
    if(hDll2 == NULL) return false;
    
    AllocSimEncObj       = (ISimEnc_AllocSimEncObj )::GetProcAddress(hDll2, "_ISimEnc_AllocSimEncObj@8");
	SetImei              = (ISimEnc_SetImei )::GetProcAddress(hDll2, "_ISimEnc_SetImei@12");
	SetFlashSerialNumber = (ISimEnc_SetFlashSerialNumber )::GetProcAddress(hDll2, "_ISimEnc_SetFlashSerialNumber@12");
	SetSimLockFile       = (ISimEnc_SetSimLockFile )::GetProcAddress(hDll2, "_ISimEnc_SetSimLockFile@8");
	SetUnlockCodes       = (ISimEnc_SetUnlockCodes )::GetProcAddress(hDll2, "_ISimEnc_SetUnlockCodes@8");
	CheckSimLockData     = (ISimEnc_CheckSimLockData )::GetProcAddress(hDll2, "_ISimEnc_CheckSimLockData@12");
	FreeSimEncObj        = (ISimEnc_FreeSimEncObj )::GetProcAddress(hDll2, "_ISimEnc_FreeSimEncObj@4");

	hDll3 = ::LoadLibrary(_T("MobileToolKit_StaticM.dll"));
    if(hDll3 == NULL) return false;
    pWriteDefaultPhoneCode  = (pFnWriteDefaultPhoneCode )::GetProcAddress(hDll3, "WriteDefaultPhoneCode");
	pReadBatteryVoltage  = (pFnReadBatteryVoltage )::GetProcAddress(hDll3, "ReadBatteryVoltage");
    pIsOTPFrozen  = (pFnIsOTPFrozen )::GetProcAddress(hDll3, "IsOTPFrozen");
    pWriteSimLockFile  = (pFnWriteSimLockFile )::GetProcAddress(hDll3, "WriteSIMLockFileNameToNV");
	pWriteSIMLockFileNameToNV_VC6  = (pFnWriteSIMLockFileNameToNV_VC6 )::GetProcAddress(hDll3, "WriteSIMLockFileNameToNV_VC6");
	pEnableLog = (pFnEnableLog  )::GetProcAddress(hDll3, "EnableLog");
    pWriteSimDataV2 = (pFnWriteSimDataV2  )::GetProcAddress(hDll3, "WriteSimDataV2");

    pEnableLog(false);

	return true;
}


//********************Magan Mei*****************************************//
bool CQUALCOMM_IMEI_CODINGDlg::WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize)

{

	bool bRes = false;

	if(::strcmp( _T("COM_PORT"),strName.c_str()) == 0)
	{
		sprintf(szBuffer, _T("%d"), Port_Number);
		bRes = true;
	}
	else if(::strcmp( _T("MODEL_NAME"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Model_Name.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("MOBILE_ID"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Mobile_ID.GetBuffer());
		bRes = true;
	}
	else if(::strcmp(_T("TARGET_TYPE"), strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Target_Type.GetBuffer());
		bRes = true;
	}
	else if(::strcmp(_T("PPF_CHECK_FLAG"), strName.c_str()) == 0)
	{
		sprintf(szBuffer, _T("%d"), PPF_CHECK_FLAG);
		bRes = true;
	}
	else if(::strcmp(_T("PART_NUMBER"), strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Part_Number.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("SIM_DATA_FILE_NAME"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Sim_Data_File_Name.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("BRT_QCN_FLAG"),strName.c_str()) == 0)
	{
		sprintf(szBuffer, _T("%d"), BRT_QCN_FLAG);
		bRes = true;
	}
	else if(::strcmp( _T("PICASSO_NO"),strName.c_str()) == 0)
	{
		m_PICASSO_EDIT.GetWindowText(Picasso_No);
		::strcpy(szBuffer, Picasso_No.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("TRANS_NO"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, TRANS_NO.GetBuffer());
		bRes = true;
	}
	else if(::strcmp(_T("GSN_STRING"), strName.c_str()) == 0)
	{
		::strcpy(szBuffer, GSN_String.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("MAL_FROM_SCANNER"),strName.c_str()) == 0)
	{
		m_IMEI_EDIT.GetWindowText(IMEI_FROM_SCANNER);
		::strcpy(szBuffer, MAL_FROM_SCANNER.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("IMEI_FROM_SCANNER"),strName.c_str()) == 0)
	{
		m_IMEI_EDIT.GetWindowText(IMEI_FROM_SCANNER);
		::strcpy(szBuffer, IMEI_FROM_SCANNER.GetBuffer());
		bRes = true;
	}

	else if(::strcmp( _T("Current_Directory"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Current_Directory.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("IMEI_FROM_PHONE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, IMEI_From_Phone);
		bRes = true;
	}
	else if(::strcmp( _T("BT_USER_USIM_UNLOCK_CODE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, BT_User_USIM_Unlock_Code);
		bRes = true;
	}
	else if(::strcmp( _T("BT_NETWORK_UNLOCK_CODE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, BT_Network_Unlock_Code);
		bRes = true;
	}
	else if(::strcmp( _T("BT_NETWORK_SUBSET_UNLOCK_CODE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, BT_Network_Subset_Unlock_Code);
		bRes = true;
	}
	else if(::strcmp( _T("BT_SERVICE_PROVIDER_UNLOCK_CODE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, BT_Service_Provider_Unlock_Code);
		bRes = true;
	}
	else if(::strcmp( _T("BT_COPORATE_UNLOCK_CODE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, BT_Coporate_Unlock_Code);
		bRes = true;
	}
	else if(::strcmp( _T("BT_USER_SIM_UNLOCK_CODE"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, BT_User_SIM_Unlock_Code);
		bRes = true;
	}
	else if(::strcmp( _T("FSN_FROM_PICS"),strName.c_str()) == 0)
	{
		::memcpy(szBuffer, Device_Serial_Number_Digit,sizeof(Device_Serial_Number_Digit));
		bRes = true;
	}
	else if(::strcmp( _T("FSN_FROM_PICS_BASE_64"),strName.c_str()) == 0)
	{
		int nDataLen = Base64EncodeGetRequiredLength(16);
		Base64Encode(Device_Serial_Number, 16, szBuffer, &nDataLen);
		bRes = true;
	}
	//else if(::strcmp( _T("DEVICE_SERIAL_NUMBER_DIGIT_BASE64"),strName.c_str()) == 0)
	//{
	//	int nDataLen = Base64EncodeGetRequiredLength(16);
	//	Base64Encode(Device_Serial_Number, 16, szBuffer, &nDataLen);
	//	bRes = true;
	//}	
	
	//else if(::strcmp( _T("LOCK_STATUS"),strName.c_str()) == 0)
	//{
	//	if(Lock_Status)
	//		::strcpy(szBuffer, _T("1"));
	//	else
	//		::strcpy(szBuffer, _T("0"));
	//	bRes = true;
	//}
	else if(::strcmp( _T("ATHB1A_FLAG"),strName.c_str()) == 0)
	{
		if(ATHB1A_Flag)
			::strcpy(szBuffer, _T("1"));
		else
			::strcpy(szBuffer, _T("0"));
		bRes = true;
	}	
	else if(/*(::strcmp( _T("WRITE_BT_ADDR_F"),strName.c_str()) == 0)||*/(::strcmp( _T("WAIT_FLAG"),strName.c_str()) == 0))
	{
		CString szTemp;
		szTemp.Format(_T("%d"), mFlag);
		::strcpy(szBuffer, szTemp.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("CSV_FILE_NAME"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, CSV_File_Name);
		bRes = true;
	}
	else if(::strcmp( _T("BRT_FILE_NAME"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, brt_file_name);
		bRes = true;
	}
	else if(::strcmp( _T("COMPUTER_NAME"),strName.c_str()) == 0)
	{
		::strcpy(szBuffer, Computer_Name.GetBuffer());
		bRes = true;
	}
	else if(::strcmp( _T("SIM_DATA_FOR_PHONE"),strName.c_str()) == 0)
	{
		//::memcpy(szBuffer, SIM_Data_For_Phone, sizeof(SIM_Data_For_Phone));
		int nDataLen = Base64EncodeGetRequiredLength(sizeof(SIM_Data_For_Phone));
		memset(szBuffer, 0, nDataLen);
		bRes = Base64Encode(SIM_Data_For_Phone, sizeof(SIM_Data_For_Phone)-1, szBuffer, &nDataLen) == TRUE;
	}
	else if(::strcmp( _T("SPC_CODE"),strName.c_str()) == 0)
	{
		//::strcpy(szBuffer, SIM_Data_For_Phone);
		::memcpy(szBuffer,SPC_Code,sizeof(SPC_Code));
		bRes = true;
	}
	//else if(::strcmp(_T("FSN_FROM_MOBILE_32"),strName.c_str()) == 0)
	//{
	//	//::strcpy(szBuffer, SIM_Data_For_Phone);
	//	CString szTemp = FSN_From_Phone_32;
	//	szTemp.MakeUpper();
	//	::memcpy(szBuffer,szTemp.GetBuffer(),sizeof(FSN_From_Phone_32));
	//	bRes = true;
	//}
	else if(::strcmp(_T("SECURITY_MECHANISM_FLAG"), strName.c_str()) == 0)
	{
		if(SECURITY_MECHANISM_FLAG)
			::strcpy(szBuffer, _T("1"));
		else
			::strcpy(szBuffer, _T("0"));
		bRes = true;
	}
	else if(::strcmp(_T("BRT1_BACKUP_PATH"), strName.c_str()) == 0)
	{
		char log_file_name[512];
		char error_message[512];
		ZeroMemory(log_file_name,sizeof(log_file_name));
		ZeroMemory(error_message,sizeof(error_message));

		char temp_log[512];
		ZeroMemory(temp_log,sizeof(temp_log));

		::CreateDirectory(Current_Directory+"\\log\\brtbackup_log",NULL);
		sprintf(log_file_name,"%s\\log\\brtbackup_log\\%s.brtbackuplog",Current_Directory,Mobile_ID.GetBuffer());
		::GetPrivateProfileString("BRT1_BACKUP_SET","BRT1_BACKUP_PATH","",temp_log,sizeof(temp_log),Ini_File_Path);

		m_szBRTBackup1Path = temp_log;
		SYSTEMTIME st;
		::GetLocalTime(&st);
		CString szTime;
		szTime.Format(_T("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wSecond, st.wMinute);
		m_szBRTBackup1Path = temp_log;
		m_szBRTBackup1Path = m_szBRTBackup1Path + _T("\\") + Mobile_ID + _T("_") + szTime + _T(".brt");

		/**********************back up NVItem to BRT according to CSV file************************/
		InsertStringIntoList(_T("Start to backup NV item to %s according to %s......")+m_szBRTBackup1Path+CSV_File_Name);

		::strcpy(szBuffer, m_szBRTBackup1Path.GetBuffer());
		return true;
	}
	else if(::strcmp(_T("TRANCIVER_FLAG"), strName.c_str()) == 0)
	{
		if(m_bTranciver)
			::strcpy(szBuffer, _T("1"));
		else
			::strcpy(szBuffer, _T("0"));
		bRes = true;
	}
	else if (::strcmp(_T("USBLogName"), strName.c_str()) == 0)
	{
		::strcpy(szBuffer, USBLogName.GetBuffer());
		bRes = true;
	}

	return bRes;

}

bool CQUALCOMM_IMEI_CODINGDlg::CheckMAL()
{
	if(Write_MAL_Flag)
	{
		m_MAL_EDIT.GetWindowText(MAL_FROM_SCANNER);
		InsertStringIntoList(_T("The MAL you scaned is %s......")+MAL_FROM_SCANNER);
		if((!Check_MAL_FirstPart(MAL_FROM_SCANNER))||(!CheckMALLength(MAL_FROM_SCANNER)))
		{
			InsertStringIntoList(_T("[COM%d]: Check MAL fail......"));

			::PostMessage(m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
			return false;
		}
	}
	else
	{
		InsertStringIntoList(_T("[COM%d]: Check MAL skiped......"));
	}
	return true;
}
bool CQUALCOMM_IMEI_CODINGDlg::CheckPICASSO()
{
	if(ATHB1A_Flag)
	{
		m_PICASSO_EDIT.GetWindowText(Picasso_No);
		InsertStringIntoList(_T("The Picasso you scaned is %s......") + Picasso_No);
		if(Picasso_No.GetLength()!=10)
		{
			InsertStringIntoList(_T("IMEI coding fail:The length of PICASSO NO is less than 10"));

			::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
			return false;
		}
		CString strIllegal = Picasso_No.SpanIncluding(_T("0123456789ABCDEFGHJKLMNPQRSTUVWXYZ"));
		if(strIllegal.GetLength() != 10)
		{
			InsertStringIntoList(_T("Picasso Number include illegal number ") + Picasso_No.Mid(strIllegal.GetLength()));
			::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
			return false;
		}
		strIllegal = Picasso_No.Left(2);
		if(strIllegal != _T("81"))
		{
			InsertStringIntoList(_T("The first part in Picasso Number is not 81, it is ") + strIllegal);
			::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
			return false;
		}
	}
	else
	{
		InsertStringIntoList(_T("[COM%d]: Check PICASSO skiped......"));
	}
	return true;
}
bool CQUALCOMM_IMEI_CODINGDlg::CheckIMEIScanner()
{
	//check IMEI, lenght==15, first parts, is Validity
	m_IMEI_EDIT.GetWindowText(IMEI_FROM_SCANNER);
	if(!m_bTranciver)
	{
		InsertStringIntoList(_T("The IMEI you scaned is %s......")+IMEI_FROM_SCANNER);
		if((!CheckIMEIValidity(IMEI_FROM_SCANNER))||(!Check_IMEI_FirstPart(IMEI_FROM_SCANNER))||(!CheckIMEILength(IMEI_FROM_SCANNER)))
		{
			InsertStringIntoList(_T("IMEI coding fail......"));

			::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
			return false;
		}
	}
	//else //Tranciver
	//{
	//	m_IMEI_EDIT.GetWindowText(IMEI_FROM_SCANNER);
	//	InsertStringIntoList(_T("The BT address you scaned is %s......") + IMEI_FROM_SCANNER);
	//	if((!Check_IMEI_FirstPart(IMEI_FROM_SCANNER))||(!CheckIMEILength(IMEI_FROM_SCANNER)))
	//	{
	//		InsertStringIntoList(_T("IMEI coding fail......"));

	//		::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
	//		return false;
	//	}

	//}
	return true;

}

bool CQUALCOMM_IMEI_CODINGDlg::Print(const TSTRING& strName,const TSTRING& strOutput)

{
	char temp_log[512];
	CString szName = strName.c_str();
	szName.TrimLeft(_T("\""));
	szName.TrimRight(_T("\""));
	CString szOutput = strOutput.c_str();
	szOutput.TrimLeft(_T("\""));
	szOutput.TrimRight(_T("\""));
	CString szMessage = _T("");
	bool bRes = true;

	if(szName == _T("Function"))
	{
		if(szOutput == _T("CHECK_MAL"))
		{
			return CheckMAL();
		}
		else if(szOutput == _T("PICASSO_CHECK"))
		{
			return CheckPICASSO();
		}
		else if(szOutput == _T("IMEI_SCANNER"))
		{
			return CheckIMEIScanner();
		}	
		else if(szOutput == _T("BackUpBRT_PRE"))
			return BackUpBRTPre();
	}
	else if(szName == _T("BackUpBRT_LAST"))
	{
		if(!BackUpBRTLast(szOutput))
		{
			errFlag = true;
			WriteDebugLog();
			errFlag = false;
			return false;
		}
		return true;
	}
	else if(szName == _T("Title"))
	{
		InsertStringIntoList(szOutput);
		return true;
	}
	else if(szName == _T("Log"))
	{
		if(szOutput == _T("ComInit Fail!"))
			return false;
		else
		{
			InsertStringIntoList(szOutput);
			return true;
		}

	}
	else if(szName == _T("Error"))
	{
		errFlag = true;
		Error_Code_String=szOutput;
		WriteDebugLog();
		return false;

	}
	else if(szName == _T("Progress"))
	{
		return SetAfxProgressStatus(szOutput);
	}
	else if(szName == _T("UPDATE_LABEL_MOBILEID"))
	{
		if(!IMEIUpdateMobileID(szOutput))
		{
			WriteDebugLog();
			return false;
		}
		return true;
	}
	else if(szName == _T("Related IMEI"))
	{
		InsertStringIntoList(_T("Picasso Number Related IMEI: ") + szOutput);
		CString szTemp = IMEI_From_Phone;
		InsertStringIntoList(_T("IMEI From Phone: ") + szTemp);
		return true;
	}
	else if(szName == _T("Related Mobile ID"))
	{
		InsertStringIntoList(_T("Picasso Number Related Mobile ID: ") + szOutput);
		InsertStringIntoList(_T("Mobile ID From Phone: ") + Mobile_ID);
		return true;

	}
	else if(szName == _T("CHECK_CEFS_FILE_NAME"))
	{
		if(!IMEICheckPPFFILE(szOutput))
		{
			WriteDebugLog();
			return false;
		}
		return true;
	}
	else if(szName == _T("IMEI_FROM_PHONE"))
	{
		if(!IMEICheckIMEIIsValid(szOutput.GetBuffer()))
		{
			if(Error_Code_String != _T("F00"))
				WriteDebugLog();
			return false;
		}
		return true;
	}
	//else if(szName == _T("Update Lock Status"))
	//{
	//	if(szOutput == _T("1"))
	//	{
	//		Lock_Status = true;
	//		szMessage.Format(_T("SPC is locked....."),Port_Number);
	//	}
	//	else
	//	{
	//		Lock_Status = false;
	//		szMessage.Format(_T("SPC is unlocked....."),Port_Number);
	//	}	
	//	InsertStringIntoList(szMessage);
	//}
	else if(szName == _T("MSN"))
	{
		GSN_String = szOutput;
		InsertStringIntoList(_T("GSN:") + GSN_String);
		TCHAR cTemp[4];
		GetPrivateProfileString(_T("IMEI_MAL"),_T("MSN_FIRST_PART"),"",cTemp,sizeof(cTemp),Ini_File_Path);
		CString szTemp = GSN_String.Left(3);
		if(cTemp[0] == '\0')
			InsertStringIntoList("Skip Check GSN Project Name");
		else
		{
			if(szTemp != cTemp)
			{
				errFlag = true;
				Error_Code_String = _T("F113");
				szTemp = _T("MSN not equal to MSN_FIRST_PART ") + GSN_String ;
				szTemp += _T(":");
				szTemp += cTemp;
				InsertStringIntoList(szTemp);
				WriteDebugLog();
				return false;
			}

		}

		if(GSN_String.GetAt(3) != _T('1'))
		{
			errFlag = true;
			Error_Code_String = _T("F114");
			InsertStringIntoList(_T("Plant code not to Qisda!"));
			WriteDebugLog();
			return false;
		}
		//Check time
		{
			TCHAR YearCode[]={_T('A'),_T('B'),_T('C'),_T('D'),_T('E'),_T('F'),_T('G'),_T('H'),_T('J'),_T('L'),_T('N'),_T('Q'),_T('S'),\
				_T('V'),_T('X'),_T('B'),_T('D'),_T('A'),_T('G'),_T('K'),_T('M'),_T('P'),_T('R'),_T('T'),_T('W'),_T('Z'),_T('C'),_T('F'),_T('H')};
			TCHAR MonthCode1[] = {_T('A'),_T('C'),_T('E'),_T('G'),_T('J'),_T('L'),_T('N'),_T('Q'),_T('S'),_T('U'),_T('W'),_T('Y')};
			TCHAR MonthCode2[] = {_T('B'),_T('D'),_T('F'),_T('H'),_T('K'),_T('M'),_T('P'),_T('R'),_T('T'),_T('V'),_T('X'),_T('Z')};

			int nYearNumber = sizeof(YearCode)/sizeof(TCHAR);
			int nYear = 9999;

			for(int i = 0; i < nYearNumber; i++)
			{
				if(GSN_String.GetAt(4) == YearCode[i])
				{	nYear= 2000 + i;
					break;
				}
			}

			int nMonth = 13;
			for(int i = 0; i < 12; i++)
			{
				if((GSN_String.GetAt(5) == MonthCode1[i]) || (GSN_String.GetAt(5) == MonthCode2[i]))
					nMonth = i + 1;
			}
			SYSTEMTIME st;
			GetLocalTime(&st);
			szTemp.Format(_T("%d/%d <==> %d%d"), nYear, nMonth,st.wYear, st.wMonth);
			InsertStringIntoList(szTemp);
			if((nYear > st.wYear) || ((nYear == st.wYear) && (nMonth > st.wMonth)) )
			{
				errFlag = true;
				Error_Code_String = _T("F115");
				InsertStringIntoList(_T("MSN Generate time error!"));
				WriteDebugLog();
				return false;
			}
		}
		
		return true;
	}
	else if(szName == _T("BT_ADDR"))
	{
		BT_Address = szOutput;
		return true;
	}
	else if(szName == _T("QUERY_IMEI_CHECK"))
	{
		if(!QueryIMEICheck(strOutput.c_str()))
		{
			WriteDebugLog();
			return false;
		}
		return true;
	}
	else if(szName == _T("BT_ADDRESS_FROM_PHONE"))
	{
		szOutput.MakeLower();
		CString szTemp = BT_Address;
		szTemp.MakeLower();
		if(szOutput != szTemp)
		{
			InsertStringIntoList(_T("[COM%d]: BT address read from phone is not equal to BT address write to phone"));
			Error_Code_String = _T("F86");
			errFlag = true;
			WriteDebugLog();
			return false;
		}
		else
		{
			InsertStringIntoList(_T("[COM%d]: BT address read from phone is equal to BT address write to phone"));

		}
		return true;
	}
	else if(szName == _T("SPC_CODE_PICS"))
	{
		::memcpy(SPC_Code,strOutput.c_str(),6);
		InsertStringIntoList(_T("Get SPC from PICS success......"));
		return true;
	}
	else if(szName == _T("SIM_DATA_FROM_PICS"))
	{
		memcpy(SIM_Data_From_PICS,strOutput.c_str(),strlen(strOutput.c_str()));
		return DealSIMDataFromPICS();
	}

	else if(szName == _T("SAVE_PICS_FILE"))
	{
		return SavePICSFile();
	}
	else if(szName == _T("DEAL_FSN_FROM_PICS"))
	{
		//for(int i=0;i<32;i++)
		//{
		//	Device_Serial_Number_Digit[i] = szOutput.GetAt(i);
		//}
		//Device_Serial_Number_Digit[32]='\0';

		//szMessage.Format(_T("Get FSN from PICS success......"),Port_Number);
		//szMessage += szOutput;
		//InsertStringIntoList(szMessage);


		if(! DealFsnFromPICS(strOutput.c_str()))
		{
			WriteDebugLog();
			return false;
		}
		return true;
	}
	else if(szName == _T("DEAL_FSN_FROM_PHONE"))
	{
		BYTE  szFSN[256] = {0};
		int nDataLen = Base64DecodeGetRequiredLength(szOutput.GetLength());
		memset(szFSN, 0x00, nDataLen);
		bool bRes = Base64Decode(strOutput.c_str(), szOutput.GetLength(), szFSN, &nDataLen) == TRUE;
		sprintf(FSN_From_Phone_32,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			szFSN[0],
			szFSN[1],
			szFSN[2],
			szFSN[3],
			szFSN[4],
			szFSN[5],
			szFSN[6],
			szFSN[7],
			szFSN[8],
			szFSN[9],
			szFSN[10],
			szFSN[11],
			szFSN[12],
			szFSN[13],
			szFSN[14],
			szFSN[15]);
		
		szMessage.Format(_T("Get FSN from phone success......%s"), FSN_From_Phone_32);
		InsertStringIntoList(szMessage);
		for(int i = 0; i < 16; i++)
		{
			if(Device_Serial_Number[i] != szFSN[i] )
			{
				InsertStringIntoList(_T("[COM%d]: The FSN read from phone is not equal to write to phone ......"));
				Error_Code_String = _T("F95");
				errFlag = true;
				WriteDebugLog();
				return false;
			}
		}
		InsertStringIntoList(_T("[COM%d]: The FSN read from phone is equal to write to phone ......"));

		return true;		

	}
	else if(szName == _T("BT_NETWORK_UNLOCK_CODE"))
	{
		::memcpy(BT_Network_Unlock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_SERVICE_PROVIDER_UNLOCK_CODE"))
	{
		::memcpy(BT_Service_Provider_Unlock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_COPORATE_UNLOCK_CODE"))
	{
		::memcpy(BT_Coporate_Unlock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_NETWORK_SUBSET_UNLOCK_CODE"))
	{
		::memcpy(BT_Network_Subset_Unlock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_USER_USIM_UNLOCK_CODE"))
	{
		::memcpy(BT_User_USIM_Unlock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_USER_SIM_UNLOCK_CODE"))
	{
		::memcpy(BT_User_SIM_Unlock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_SERIAL_CODE"))
	{
		::memcpy(BT_Serial_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	else if(szName == _T("BT_PHONE_LOCK_CODE"))
	{
		::memcpy(BT_Phone_Lock_Code,strOutput.c_str(),strlen(strOutput.c_str()));
		return true;
	}
	//else if(szName == _T("DEVICE_SERIAL_NUMBER_DIGIT"))
	//{
	//	szMessage = _T("Query FSN from PICS success, FSN->") + szOutput;
	//	::memcpy(Device_Serial_Number_Digit,strOutput.c_str(),strlen(strOutput.c_str()));
	//}
	else if(szName == _T("BT_FROM_PHONE"))
	{
		if(szOutput.GetLength() != 0)
		{
			InsertStringIntoList(_T("BT Address from Phone: ") + szOutput);
			CString szTemp = IMEI_FROM_SCANNER;
			szTemp.MakeLower();
			szOutput.MakeLower();
			if(szTemp != szOutput)
			{
				InsertStringIntoList(_T("BT Address from Scanner: ") + IMEI_FROM_SCANNER);
				InsertStringIntoList(_T("BT Address from Phone is not equal from scanner!"));
				::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
				return false;
			}
		}
		return true;
	}
	else
	{
		InsertStringIntoList(szName + szOutput);
		return true;
	}

	return bRes;

}
bool CQUALCOMM_IMEI_CODINGDlg::CheckBreak()

{

	return false;

}

bool CQUALCOMM_IMEI_CODINGDlg::WriteDebugLog()
{
	
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
	InsertStringIntoList(temp_log);

	if(SAVE_PICS_FILE_FLAG)
	{
		WritePICSFile();
	}
	
	::PostMessage(m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::SavePICSFile()
{				
	if(SAVE_PICS_FILE_FLAG)
	{
		IMEI_Coding->WritePICSFile();
	}
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::DealSIMDataFromPICS()
{				
	SIM_Data_From_PICS[1153]='\0';
	InsertStringIntoList("Get SIM data from PICS success......");

	InsertStringIntoList("The SIM data from PICS is:\n");

	int row=18;
	int column=64;
	int i=0;
	int j=0;
	for(i=0;i<row;i++)
	{
		ZeroMemory(Temp_Global,sizeof(Temp_Global));
		for(j=column*i;j<=column*(i+1)-1;j++)
		{
			Temp_Global[j-column*i]=SIM_Data_From_PICS[j]; 
			//TRACE("%d\n", SIM_Data_From_PICS[j]);
		}
		TRACE("\n");

		InsertStringIntoList(Temp_Global);
	}

	//change the length from 1024 to 512
	InsertStringIntoList("Convert SIM data to 576 bytes......");

	_strupr((char *)SIM_Data_From_PICS);
	i=0;
	j=0;
	int nSimDataLength = ::GetPrivateProfileInt("LENGTH_SETTING","SIM_DATA_LENGTH",576,Ini_File_Path);
	for(i=0;i<nSimDataLength;i++)
	{
		unsigned char FirstNibble='\0';
		unsigned char SecondNibble='\0';
		if(SIM_Data_From_PICS[j]>='A')
			FirstNibble=(unsigned char)((unsigned char)((unsigned char)SIM_Data_From_PICS[j]-'A')+10);
		else
			FirstNibble=(unsigned char)((unsigned char)SIM_Data_From_PICS[j]-'0');

		if(SIM_Data_From_PICS[j+1]>='A')
			SecondNibble=(unsigned char)((unsigned char)((unsigned char)SIM_Data_From_PICS[j+1]-'A')+10);
		else
			SecondNibble=(unsigned char)((unsigned char)SIM_Data_From_PICS[j+1]-'0');
		SIM_Data_For_Phone[i]=(unsigned char)(FirstNibble<<4)+(unsigned char)SecondNibble;
		j+=2;
	}
	SIM_Data_For_Phone[577]='\0';
	InsertStringIntoList("The SIM data converted is:\n");

	row=36;
	column=16;
	i=0;
	for(i=0;i<row;i++)
	{
		ZeroMemory(Temp_Global,sizeof(Temp_Global));
		sprintf(Temp_Global,"0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
			SIM_Data_For_Phone[column*i],
			SIM_Data_For_Phone[column*i+1],
			SIM_Data_For_Phone[column*i+2],
			SIM_Data_For_Phone[column*i+3],
			SIM_Data_For_Phone[column*i+4],
			SIM_Data_For_Phone[column*i+5],
			SIM_Data_For_Phone[column*i+6],
			SIM_Data_For_Phone[column*i+7],
			SIM_Data_For_Phone[column*i+8],
			SIM_Data_For_Phone[column*i+9],
			SIM_Data_For_Phone[column*i+10],
			SIM_Data_For_Phone[column*i+11],
			SIM_Data_For_Phone[column*i+12],
			SIM_Data_For_Phone[column*i+13],
			SIM_Data_For_Phone[column*i+14],
			SIM_Data_For_Phone[column*i+15]);
		InsertStringIntoList(Temp_Global);
	}
	return true;
}
//// for script
bool CQUALCOMM_IMEI_CODINGDlg::QueryIMEICheck(const char * Status_Output)
{
	bool bRes = true;

	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));

	InsertStringIntoList("Query BT and GSN/MSN from server success......");

	if(!m_bTranciver)
	{
		if(BT_Address.IsEmpty())//empty
		{
			sprintf(temp_log,"BT address------->EMPTY  GSN/MSN------->%s",GSN_String);
			InsertStringIntoList(temp_log);
			errFlag = true;
			Error_Code_String="F81";
			return false;
		}
		else
		{
			sprintf(temp_log,"BT address------->%s  GSN/MSN------->%s",BT_Address,GSN_String);
			InsertStringIntoList(temp_log);
		}

		sprintf(temp_log,"IMEI Status--->'%s'",Status_Output);
		InsertStringIntoList(temp_log);

		if(!strcmp(Status_Output, _T("B")))
		{
			if(strcmp(IMEI_From_Phone, Zero_IMEI) && strcmp(IMEI_From_Phone, Dummy_IMEI))
			{
				sprintf(temp_log,"In REWORK mode, the IMEI status must be 'I'//'R'//'D', but the real status is '%s'......",Port_Number,Status_Output);
				InsertStringIntoList(temp_log);
				errFlag = true;
				Error_Code_String="F58";
				return false;

			}
		}
		else //P
		{
			if(!strcmp(IMEI_From_Phone, Zero_IMEI) || !strcmp(IMEI_From_Phone, Dummy_IMEI))
			{
				sprintf(temp_log,"IMEI Status--->'%s'",Port_Number,Status_Output);
				InsertStringIntoList(temp_log);

				sprintf(temp_log,"The IMEI(%s) has been burned, namely the status is not 'B', while the handset has not IMEI......",Port_Number,IMEI_FROM_SCANNER);
				InsertStringIntoList(temp_log);
				Error_Code_String="F80";	
				errFlag = true;
				return false;

			}
		}

		if(bRes)
			bRes = QueryIMEI_CHECK_LEN();
	}

	IMEI_BT_GSN_Flag=1;
	return bRes;
}

bool CQUALCOMM_IMEI_CODINGDlg::QueryIMEI_CHECK_LEN()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));

	if(ATHB1A_Flag)
	{
		int nBTLength = ::GetPrivateProfileInt("LENGTH_SETTING","BT_LENGTH",12,Ini_File_Path);
		if(BT_Address.GetLength()!=nBTLength)
		{
			sprintf(temp_log,"Length of BT from PICS:%d  It should be:%d",BT_Address.GetLength(),nBTLength);
			InsertStringIntoList(temp_log);
			errFlag = true;
			Error_Code_String="F82";
			return false;
		}
		int nMSNLength = ::GetPrivateProfileInt("LENGTH_SETTING","MSN_LENGTH",10,Ini_File_Path);
		if(GSN_String.GetLength()!=nMSNLength)
		{
			sprintf(temp_log,"Length of MSN from PICS:%d  It should be:%d",GSN_String.GetLength(),nMSNLength);
			InsertStringIntoList(temp_log);
			errFlag = true;
			Error_Code_String="F83";
			return false;
		}
	}
	else
	{
		int nGSNLength = ::GetPrivateProfileInt("LENGTH_SETTING","GSN_LENGTH",13,Ini_File_Path);
		if(GSN_String.GetLength()!=nGSNLength)
		{
			sprintf(temp_log,"Length of GSN from PICS:%d  It should be:%d",GSN_String.GetLength(),nGSNLength);
			InsertStringIntoList(temp_log);
			errFlag = true;
			Error_Code_String="F83";
			return false;
		}
	}

	return true;
}




bool CQUALCOMM_IMEI_CODINGDlg::SetAfxProgressStatus(const char *pStatus)
{
	int nInt = atoi(pStatus);
	IMEI_Coding->m_PROGRESS.SetPos(nInt);
	CString szStatus;
	szStatus.Format(_T("%d"), nInt);
	szStatus.Append(_T("%"));
	IMEI_Coding->m_PERCENT.SetWindowText(szStatus);
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::BackUpBRTPre()
{
	char log_file_name[512];
	char error_message[512];
	ZeroMemory(log_file_name,sizeof(log_file_name));
	ZeroMemory(error_message,sizeof(error_message));

	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));

	::CreateDirectory(Current_Directory+"\\log\\brtbackup_log",NULL);
	sprintf(log_file_name,"%s\\log\\brtbackup_log\\%s.brtbackuplog",Current_Directory,IMEI_FROM_SCANNER);
	sprintf(brt_file_name,"%s\\%s.brt",Backup_Path,IMEI_FROM_SCANNER);

	sprintf(temp_log,"Set BRT backup log file %s......",log_file_name);
	InsertStringIntoList(temp_log);

	int NVItem_Count=0;
	int Completed=0;
	CString filesize="";

	/**********************back up NVItem to BRT according to CSV file************************/
	sprintf(temp_log,"Start to backup NV item to %s according to %s......",brt_file_name,CSV_File_Name);
	InsertStringIntoList(temp_log);

//	if(Mode_Name=="REWORK")
	{
		CFile brtfile;
		if(brtfile.Open(brt_file_name,CFile::modeRead,NULL)!=0)
		{
			brtfile.Close();
			filesize=GetFileSize(brt_file_name);
			if(atoi(filesize)>=80)
			{
				sprintf(temp_log,"BRT file %s size is %s >=87,the file all already exist",brt_file_name,filesize);
				InsertStringIntoList(temp_log);
				mFlag = 0;
				return true;
			}
		}
	}
	mFlag = 1;
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::BackUpBRTLast(CString szBRTPath)
{
	CString filesize="";
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));
	filesize.Empty();
//	filesize=GetFileSize(brt_file_name);
	filesize = GetFileSize(szBRTPath);
	if(atoi(filesize)<80)
	{
		sprintf(temp_log,"BRT file %s size is %s <80.....",szBRTPath,filesize);
		InsertStringIntoList(temp_log);
		Error_Code_String="F114";
		errFlag = true;
		return false;
	}

	sprintf(temp_log,"Backup NV item to %s success, size is %s......",szBRTPath,filesize);
	InsertStringIntoList(temp_log);

	/*********************************************************************/
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::DealFsnFromPICS(const char * szDevSerialNoDigit)
{
	char temp_log[512];
	for(int i=0;i<32;i++)
	{
		Device_Serial_Number_Digit[i] = szDevSerialNoDigit[i];
	}
	Device_Serial_Number_Digit[32]='\0';

	int i = 0;
	int j = 0;
	int Data = 0;
	while(Device_Serial_Number_Digit[i] != _T('\0'))
	{
		
		if( (Device_Serial_Number_Digit[i] > (_T('a') - 1)) && (Device_Serial_Number_Digit[i] < (_T('f') + 1)) )
			Data = Data*16 + Device_Serial_Number_Digit[i] - _T('a') + 10;
		else if ( (Device_Serial_Number_Digit[i] > (_T('A') - 1)) && (Device_Serial_Number_Digit[i] < (_T('F') + 1)) )
			Data = Data*16 + Device_Serial_Number_Digit[i] - _T('A') + 10;
		else if ( (Device_Serial_Number_Digit[i] > (_T('0') - 1)) && (Device_Serial_Number_Digit[i] < (_T('9') + 1)) )
			Data = Data*16 + Device_Serial_Number_Digit[i] - _T('0');
		 i++;


		 if(i % 2 == 0)
		 {
			 Device_Serial_Number[j] = Data;
			 Data = 0;
			 j++;
		 }

	}

	//Status = sscanf((char *)Device_Serial_Number_Digit,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	//	&Device_Serial_Number[0],
	//	&Device_Serial_Number[1],
	//	&Device_Serial_Number[2],
	//	&Device_Serial_Number[3],
	//	&Device_Serial_Number[4],
	//	&Device_Serial_Number[5],
	//	&Device_Serial_Number[6],
	//	&Device_Serial_Number[7],
	//	&Device_Serial_Number[8],
	//	&Device_Serial_Number[9],
	//	&Device_Serial_Number[10],
	//	&Device_Serial_Number[11],
	//	&Device_Serial_Number[12],
	//	&Device_Serial_Number[13],
	//	&Device_Serial_Number[14],
	//	&Device_Serial_Number[15]);

	int nFSNLength = ::GetPrivateProfileInt("LENGTH_SETTING","FSN_LENGTH",16,Ini_File_Path);

	if(j !=nFSNLength)
	{
		InsertStringIntoList("Get FSN from PICS fail......");
		Error_Code_String="F92";
		errFlag = true;
		return false;
	}
	else
	{
		InsertStringIntoList("Get FSN from PICS success......");

		sprintf(temp_log,"FSN=0x%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
			Device_Serial_Number[0],
			Device_Serial_Number[1],
			Device_Serial_Number[2],
			Device_Serial_Number[3],
			Device_Serial_Number[4],
			Device_Serial_Number[5],
			Device_Serial_Number[6],
			Device_Serial_Number[7],
			Device_Serial_Number[8],
			Device_Serial_Number[9],
			Device_Serial_Number[10],
			Device_Serial_Number[11],
			Device_Serial_Number[12],
			Device_Serial_Number[13],
			Device_Serial_Number[14],
			Device_Serial_Number[15]);
		InsertStringIntoList(temp_log);
		Device_Serial_Number[16]='\0';
	}
	return true;
}

bool CQUALCOMM_IMEI_CODINGDlg::DealIMEICodingEnd()
{
	char temp_log[512];
	ZeroMemory(temp_log,sizeof(temp_log));

	Error_Code_String = _T("PASS");
	errFlag = true;

	if(SAVE_PICS_FILE_FLAG)
	{
		if(!WritePICSFile())
		{
			sprintf(temp_log,"IMEI coding fail,Error code %s......",Error_Code_String);
			InsertStringIntoList(temp_log);

			IMEI_Coding->EndReset(1);

			::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
			return false;

		}
	}
	::WritePrivateProfileString("IMEI_MAL","LAST_WRITE_IMEI",IMEI_FROM_SCANNER,Ini_File_Path);

	m_PROGRESS.SetPos(100);
	m_PERCENT.SetWindowText("100%");

	InsertStringIntoList("IMEI coding success......");

	::PostMessage(IMEI_Coding->m_hWnd,WM_TERMINATETHREAD,(WPARAM)0,(LPARAM)0);
	return true;
}
void CQUALCOMM_IMEI_CODINGDlg::OnBnClickedButtonLog()
{
	TCHAR szLogFolder[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szLogFolder,CSIDL_PERSONAL,_T("Log"));
	::ShellExecute(NULL,_T("open"),szLogFolder,NULL,NULL,SW_SHOW);
}

#pragma warning(pop)
