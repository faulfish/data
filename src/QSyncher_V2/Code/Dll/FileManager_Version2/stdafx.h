// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



const LPCTSTR				EVENT_FILE_SUBMENU_INSERT_NODE = _T("TNotify_SubMenu_InsertNode");
const LPCTSTR				EVENT_FILE_SUBMENU_DELETE_NODE = _T("TNotify_SubMenu_DeleteNode");
const LPCTSTR				EVENT_FILE_SUBMENU_REFRESH_NODE = _T("TNotify_SubMenu_RefreshNode");

const LPCTSTR				EVENT_ME_LIST_SUBMENU_DOWNLOAD_NODE = _T("TNotify_MEListSubMenu_DownloadNode");
const LPCTSTR				EVENT_ME_LIST_SUBMENU_DELETE_NODE = _T("TNotify_MEListSubMenu_DeleteNode");
const LPCTSTR				EVENT_ME_LIST_SUBMENU_RENAME_NODE = _T("TNotify_MEListSubMenu_RenameNode");

const LPCTSTR				strFILE_NAME_LISTKEY = _T("Name");
const LPCTSTR				strFILE_ATTR_LISTKEY = _T("Attr");
const LPCTSTR				strFILE_SIZE_LISTKEY = _T("Size");
const LPCTSTR				strFILE_MDATE_LISTKEY = _T("M_Date");
const LPCTSTR				strFILE_FULL_LISTKEY = _T("M_FULLNAME");

const int					FILE_LIST_MAX_ITEM = 4;

const LPCTSTR				ME_FILE_PATH_SEPARATOR_STR = _T("/"); 
const char					ME_FILE_PATH_SEPARATOR_CHAR = '/';
const LPCTSTR				PC_FILE_PATH_SEPARATOR_STR = _T("\\"); 
const char					PC_FILE_PATH_SEPARATOR_CHAR = '\\';

const int					MAX_FILE_PACKAGE_SIZE = 2048;


const LPCTSTR				STR_FOLDER_UNSELECT = _T("Tree_Close.PNG");
const LPCTSTR				STR_FOLDER_SELECT = _T("Tree_Open.png");

#include "..\..\Components\UIFramework\Porting\QProgressServer.h"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\Apps\\debug\\UIFramework.lib")
#else
	#pragma comment(lib, "..\\..\\Apps\\release\\UIFramework.lib")
#endif
