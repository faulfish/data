/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   IGeneralBackupRestore.h  $
 *  $Revision:   1.3  $
 *  $Date:   Nov 09 2007 13:04:30  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/BackupRestore/IGeneralBackupRestore.h.-arc  $
 * 
 *    Rev 1.3   Nov 09 2007 13:04:30   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file IGeneralBackupRestore.h
    @brief {brief description of IGeneralBackupRestore.h}

    {detail description of IGeneralBackupRestore.h}
*/
#ifndef __IGENERAL_BACKUP_RESTORE_H__
#define __IGENERAL_BACKUP_RESTORE_H__
 
#ifdef BACKUP_RESTORE_EXPORTS
#define BACKUP_RESTORE_DLL_DECLSPEC __declspec(dllexport)
#else
#define BACKUP_RESTORE_DLL_DECLSPEC __declspec(dllimport)
#endif
 
const TCHAR BACKUP_RESTORE_VCALENDAR[] = _T("BACKUP_RESTORE_VCALENDAR");
const TCHAR BACKUP_RESTORE_VCARD[]     = _T("BACKUP_RESTORE_VCARD");
const TCHAR BACKUP_RESTORE_PHONEBOOK[] = _T("BACKUP_RESTORE_PHONEBOOK");
const TCHAR BACKUP_RESTORE_SMS[]       = _T("BACKUP_RESTORE_SMS");
const TCHAR BACKUP_RESTORE_JAVA[]      = _T("BACKUP_RESTORE_JAVA");
 
 
interface IGeneralBackupRestore {
    virtual void GeneralBackup(LPCTSTR szFileName) = 0;
    virtual void GeneralRestore(LPCTSTR szFileName) = 0;
};
 
BACKUP_RESTORE_DLL_DECLSPEC IGeneralBackupRestore* CreateBackupRestoreInstance();
 
#endif
