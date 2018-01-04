#ifndef _Win32SHELLUtility_H_
#define _Win32SHELLUtility_H_

#include <shellapi.h>
#include <Shlwapi.h>
#include <shlobj.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

/*
CSIDL_FLAG_CREATE (0x8000)
Version 5.0. Combine this CSIDL with any of the following CSIDLs to force the creation of the associated folder. 

CSIDL_ADMINTOOLS (0x0030)
Version 5.0. The file system directory that is used to store administrative tools for an individual user. The Microsoft Management Console (MMC) will save customized consoles to this directory, and it will roam with the user.

CSIDL_ALTSTARTUP (0x001d)
The file system directory that corresponds to the user's nonlocalized Startup program group.

CSIDL_APPDATA (0x001a)
Version 4.71. The file system directory that serves as a common repository for application-specific data. A typical path is C:\Documents and Settings\username\Application Data. This CSIDL is supported by the redistributable Shfolder.dll for systems that do not have the Microsoft Internet Explorer 4.0 integrated Shell installed.

CSIDL_BITBUCKET (0x000a)
The virtual folder containing the objects in the user's Recycle Bin.

CSIDL_CDBURN_AREA (0x003b)
Version 6.0. The file system directory acting as a staging area for files waiting to be written to CD. A typical path is C:\Documents and Settings\username\Local Settings\Application Data\Microsoft\CD Burning.

CSIDL_COMMON_ADMINTOOLS (0x002f)
Version 5.0. The file system directory containing administrative tools for all users of the computer.

CSIDL_COMMON_ALTSTARTUP (0x001e)
The file system directory that corresponds to the nonlocalized Startup program group for all users. Valid only for Microsoft Windows NT systems.

CSIDL_COMMON_APPDATA (0x0023)
Version 5.0. The file system directory containing application data for all users. A typical path is C:\Documents and Settings\All Users\Application Data.

CSIDL_COMMON_DESKTOPDIRECTORY (0x0019)
The file system directory that contains files and folders that appear on the desktop for all users. A typical path is C:\Documents and Settings\All Users\Desktop. Valid only for Windows NT systems.

CSIDL_COMMON_DOCUMENTS (0x002e)
The file system directory that contains documents that are common to all users. A typical paths is C:\Documents and Settings\All Users\Documents. Valid for Windows NT systems and Microsoft Windows 95 and Windows 98 systems with Shfolder.dll installed.

CSIDL_COMMON_FAVORITES (0x001f)
The file system directory that serves as a common repository for favorite items common to all users. Valid only for Windows NT systems.

CSIDL_COMMON_MUSIC (0x0035)
Version 6.0. The file system directory that serves as a repository for music files common to all users. A typical path is C:\Documents and Settings\All Users\Documents\My Music.

CSIDL_COMMON_PICTURES (0x0036)
Version 6.0. The file system directory that serves as a repository for image files common to all users. A typical path is C:\Documents and Settings\All Users\Documents\My Pictures.

CSIDL_COMMON_PROGRAMS (0x0017)
The file system directory that contains the directories for the common program groups that appear on the Start menu for all users. A typical path is C:\Documents and Settings\All Users\Start Menu\Programs. Valid only for Windows NT systems.

CSIDL_COMMON_STARTMENU (0x0016)
The file system directory that contains the programs and folders that appear on the Start menu for all users. A typical path is C:\Documents and Settings\All Users\Start Menu. Valid only for Windows NT systems.

CSIDL_COMMON_STARTUP (0x0018)
The file system directory that contains the programs that appear in the Startup folder for all users. A typical path is C:\Documents and Settings\All Users\Start Menu\Programs\Startup. Valid only for Windows NT systems.

CSIDL_COMMON_TEMPLATES (0x002d)
The file system directory that contains the templates that are available to all users. A typical path is C:\Documents and Settings\All Users\Templates. Valid only for Windows NT systems.

CSIDL_COMMON_VIDEO (0x0037)
Version 6.0. The file system directory that serves as a repository for video files common to all users. A typical path is C:\Documents and Settings\All Users\Documents\My Videos.

CSIDL_CONTROLS (0x0003)
The virtual folder containing icons for the Control Panel applications.

CSIDL_COOKIES (0x0021)
The file system directory that serves as a common repository for Internet cookies. A typical path is C:\Documents and Settings\username\Cookies.

CSIDL_DESKTOP (0x0000)
The virtual folder representing the Windows desktop, the root of the namespace.

CSIDL_DESKTOPDIRECTORY (0x0010)
The file system directory used to physically store file objects on the desktop (not to be confused with the desktop folder itself). A typical path is C:\Documents and Settings\username\Desktop.

CSIDL_DRIVES (0x0011)
The virtual folder representing My Computer, containing everything on the local computer: storage devices, printers, and Control Panel. The folder may also contain mapped network drives.

CSIDL_FAVORITES (0x0006)
The file system directory that serves as a common repository for the user's favorite items. A typical path is C:\Documents and Settings\username\Favorites.

CSIDL_FONTS (0x0014)
A virtual folder containing fonts. A typical path is C:\Windows\Fonts.

CSIDL_HISTORY (0x0022)
The file system directory that serves as a common repository for Internet history items.

CSIDL_INTERNET (0x0001)
A virtual folder representing the Internet.

CSIDL_INTERNET_CACHE (0x0020)
Version 4.72. The file system directory that serves as a common repository for temporary Internet files. A typical path is C:\Documents and Settings\username\Local Settings\Temporary Internet Files.

CSIDL_LOCAL_APPDATA (0x001c)
Version 5.0. The file system directory that serves as a data repository for local (nonroaming) applications. A typical path is C:\Documents and Settings\username\Local Settings\Application Data.

CSIDL_MYDOCUMENTS (0x000c)
Version 6.0. The virtual folder representing the My Documents desktop item.

CSIDL_MYMUSIC (0x000d)
The file system directory that serves as a common repository for music files. A typical path is C:\Documents and Settings\User\My Documents\My Music.

CSIDL_MYPICTURES (0x0027)
Version 5.0. The file system directory that serves as a common repository for image files. A typical path is C:\Documents and Settings\username\My Documents\My Pictures.

CSIDL_MYVIDEO (0x000e)
Version 6.0. The file system directory that serves as a common repository for video files. A typical path is C:\Documents and Settings\username\My Documents\My Videos.

CSIDL_NETHOOD (0x0013)
A file system directory containing the link objects that may exist in the My Network Places virtual folder. It is not the same as CSIDL_NETWORK, which represents the network namespace root. A typical path is C:\Documents and Settings\username\NetHood.

CSIDL_NETWORK (0x0012)
A virtual folder representing Network Neighborhood, the root of the network namespace hierarchy.

CSIDL_PERSONAL (0x0005)
Version 6.0. The virtual folder representing the My Documents desktop item. This is equivalent to CSIDL_MYDOCUMENTS. 
Previous to Version 6.0. The file system directory used to physically store a user's common repository of documents. A typical path is C:\Documents and Settings\username\My Documents. This should be distinguished from the virtual My Documents folder in the namespace. To access that virtual folder, use SHGetFolderLocation, which returns the ITEMIDLIST for the virtual location, or refer to the technique described in Managing the File System.


CSIDL_PRINTERS (0x0004)
The virtual folder containing installed printers.

CSIDL_PRINTHOOD (0x001b)
The file system directory that contains the link objects that can exist in the Printers virtual folder. A typical path is C:\Documents and Settings\username\PrintHood.

CSIDL_PROFILE (0x0028)
Version 5.0. The user's profile folder. A typical path is C:\Documents and Settings\username. Applications should not create files or folders at this level; they should put their data under the locations referred to by CSIDL_APPDATA or CSIDL_LOCAL_APPDATA.

CSIDL_PROFILES (0x003e)
Version 6.0. The file system directory containing user profile folders. A typical path is C:\Documents and Settings.

CSIDL_PROGRAM_FILES (0x0026)
Version 5.0. The Program Files folder. A typical path is C:\Program Files.

CSIDL_PROGRAM_FILES_COMMON (0x002b)
Version 5.0. A folder for components that are shared across applications. A typical path is C:\Program Files\Common. Valid only for Windows NT, Windows 2000, and Windows XP systems. Not valid for Windows Millennium Edition (Windows Me).

CSIDL_PROGRAMS (0x0002)
The file system directory that contains the user's program groups (which are themselves file system directories). A typical path is C:\Documents and Settings\username\Start Menu\Programs. 

CSIDL_RECENT (0x0008)
The file system directory that contains shortcuts to the user's most recently used documents. A typical path is C:\Documents and Settings\username\My Recent Documents. To create a shortcut in this folder, use SHAddToRecentDocs. In addition to creating the shortcut, this function updates the Shell's list of recent documents and adds the shortcut to the My Recent Documents submenu of the Start menu.

CSIDL_SENDTO (0x0009)
The file system directory that contains Send To menu items. A typical path is C:\Documents and Settings\username\SendTo.

CSIDL_STARTMENU (0x000b)
The file system directory containing Start menu items. A typical path is C:\Documents and Settings\username\Start Menu.

CSIDL_STARTUP (0x0007)
The file system directory that corresponds to the user's Startup program group. The system starts these programs whenever any user logs onto Windows NT or starts Windows 95. A typical path is C:\Documents and Settings\username\Start Menu\Programs\Startup.

CSIDL_SYSTEM (0x0025)
Version 5.0. The Windows System folder. A typical path is C:\Windows\System32.

CSIDL_TEMPLATES (0x0015)
The file system directory that serves as a common repository for document templates. A typical path is C:\Documents and Settings\username\Templates.

CSIDL_WINDOWS (0x0024)
Version 5.0. The Windows directory or SYSROOT. This corresponds to the %windir% or %SYSTEMROOT% environment variables. A typical path is C:\Windows.
*/


class Win32Utility
{
public:
static bool GetShellFolder( LPTSTR lpszPath, int nFolder, LPTSTR lpszExtendPath , bool bAutoCreate = true)
{
	bool bRes = false;
	int nTempFolder = nFolder;
	if(bAutoCreate)
		nTempFolder |= CSIDL_FLAG_CREATE;

	if(SUCCEEDED(::SHGetFolderPath(NULL,nTempFolder,NULL,0,lpszPath)))
	{
		PathAddBackslash(lpszPath);
		
		if(lpszExtendPath && _tcslen(lpszExtendPath) > 0)
		{
			PathAppend(lpszPath,lpszExtendPath);
			PathAddBackslash(lpszPath);
			if(bAutoCreate && ::_taccess(lpszPath,0) == -1)
				bRes = CreateDirectory(lpszPath,NULL) == TRUE;
			else bRes = true;
		}
		else
		{
			bRes = true;
		}
		
	}
	return bRes;
}

static TSTRING GetCompatiableWin32FileName(LPCTSTR strOrginal)
{
	TSTRING strRes = strOrginal;
	TSTRING::iterator Iter = strRes.begin();
	while(Iter != strRes.end())
	{
		switch(*Iter)
		{
		case _T('<'):	case _T('>'):	case _T('\"'): case _T('*'): case _T(':'):	case _T('/'):	case _T('\\'):	case _T('|'): 	case _T('?'):
			strRes.erase(Iter);
			break;
		default:			
			Iter++;
			break;
		};
	}
	return strRes;
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{	
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		{
			TCHAR* szDir = (TCHAR*)lpData;
			//Set Default folder
			if (lpData && szDir)
				SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
			break;
		}
	case BFFM_SELCHANGED: 
		{
			TCHAR szDir[MAX_PATH] = {0};
			if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir))
				SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
			break;
		}
	default:
		break;
	}

	return 0;
}

static BOOL GetFolder(TSTRING& strSelectedFolder,const TCHAR* lpszTitle,const HWND hwndOwner,const TCHAR* strRootFolder,const TCHAR* strStartFolder)
{
	TSTRING strDefaultPath;

	TCHAR pszDisplayName[MAX_PATH] = {0};
	LPITEMIDLIST lpID;
	BROWSEINFO bi;

	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL)
	{
		bi.pidlRoot = NULL;
	}
	else
	{
		LPITEMIDLIST  pIdl = NULL;
		IShellFolder* pDesktopFolder;
		TCHAR         szPath[MAX_PATH] = {0};
		OLECHAR       olePath[MAX_PATH];
		ULONG         chEaten;
		ULONG         dwAttributes;

		_tcscpy_s(szPath,MAX_PATH, (LPCTSTR)strRootFolder);
		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
#ifndef _UNICODE
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
#endif
			pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
			pDesktopFolder->Release();
		}
		bi.pidlRoot = pIdl;
	}
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL)
	{
		bi.lParam = FALSE;
	}
	else
	{
		strDefaultPath = strStartFolder;
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	bi.lParam = (LPARAM)strDefaultPath.c_str();
	lpID = SHBrowseForFolder(&bi);
	if (lpID != NULL)
	{
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE)
		{
			strSelectedFolder = pszDisplayName;
			return TRUE;
		}
	}
	return FALSE;
}

};
#endif //#ifndef _Win32SHELLUtility_H_