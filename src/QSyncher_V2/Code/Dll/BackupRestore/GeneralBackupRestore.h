/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   GeneralBackupRestore.h  $
 *  $Revision:   1.1  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/BackupRestore/GeneralBackupRestore.h.-arc  $
 * 
 *    Rev 1.1   Nov 09 2007 13:04:30   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file GeneralBackupRestore.h
    @brief {brief description of GeneralBackupRestore.h}

    {detail description of GeneralBackupRestore.h}
*/
#include "IGeneralBackupRestore.h"
 
#pragma once
 
class CGeneralBackupRestore: public IGeneralBackupRestore {
public:
    CGeneralBackupRestore() {
    }
    virtual ~CGeneralBackupRestore() {
    }
    virtual void GeneralBackup() {
    }
    virtual void GeneralRestore() {
    }
 
};
 
template<class BaseClass, class InstanceClass>
BaseClass* CreateBackupRestoreInstance() {
    return new InstanceClass();
}
