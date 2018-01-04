/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageSubMenu.h  $
 *  $Revision:   1.4  $
 *  $Date:   Nov 09 2007 13:02:42  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageSubMenu.h.-arc  $
 * 
 *    Rev 1.4   Nov 09 2007 13:02:42   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageSubMenu.h
    @brief {brief description of MessageSubMenu.h}

    {detail description of MessageSubMenu.h}
*/
#ifndef __MESSAGE_VERSION2VIEW_MESSAGESUBMENU_H__
#define __MESSAGE_VERSION2VIEW_MESSAGESUBMENU_H__
 
 
#pragma once
 
#include "../../Components/UIFramework/interface/ISubMenu.h"
#include "../../Components/Localization/ilocalization.h"
 
namespace QSYNC_LIB {
// this class is sub-menu for message version 2
class CMessageSubMenu : public ISubMenu
{
public:
    CMessageSubMenu(void) : m_pILocalization(NULL) {
        CreateILocalization(&m_pILocalization);
    }
    virtual ~CMessageSubMenu(void) {
    }
    virtual int GetMenuItemCount() {
        return 5;
    }
    virtual bool GetMenuItem(int nIndex, TSTRING& strDisplayID, TSTRING& strNotifyID)
	{
        bool bRes = false;
        m_pstrDisplayID = &strDisplayID;
        m_pstrNotifyID = &strNotifyID;
        ASSERT(m_pILocalization);
        if(m_pILocalization) {
            switch(nIndex) {
                case 0:
                    SetstrDisplayIDAndstrNotifyID(_T("IDS_MessageSubMenu_New"));
                    bRes = true;
                break;
                case 1:
                    SetstrDisplayIDAndstrNotifyID(_T("IDS_MessageSubMenu_Forward"));
                    bRes = true;
                break;
                case 2:
                    SetstrDisplayIDAndstrNotifyID(_T("IDS_MessageSubMenu_Reply"));
                    bRes = true;
                break;
                case 3:
                    SetstrDisplayIDAndstrNotifyID(_T("IDS_MessageSubMenu_Send"));
                    bRes = true;
                break;
                case 4:
                    SetstrDisplayIDAndstrNotifyID(_T("IDS_MessageSubMenu_Delete"));
                    bRes = true;
                break;
                default:
                    ASSERT(false);
                break;
            }
        }
        return bRes;
    }
private:
    void SetstrDisplayIDAndstrNotifyID(TCHAR* strIDS)
	{
        *m_pstrDisplayID = m_pILocalization->GetString(strIDS).c_str();
        *m_pstrNotifyID  = strIDS;
    }
private:
    TSTRING* m_pstrDisplayID;
    TSTRING* m_pstrNotifyID;
    ILocalization* m_pILocalization;
};
 
}
#endif
