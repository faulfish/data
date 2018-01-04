///////////////////////////////////////////////////////////
//  QControlNotify.h
//  Define of the Notify string IDs
//  Created on:      12-Feb-2007 15:59:36
//  Original author: Alan tu
///////////////////////////////////////////////////////////

#if !defined(EA_12097F6D_0B61_4009_AA2A_989972578994__INCLUDED_)
#define EA_12097F6D_0B61_4009_AA2A_989972578994__INCLUDED_

namespace QSYNC_LIB
{

//********************************************************//
//Event list for ISubMenu
//Parameter is pointer of QPoint
const LPCTSTR EVENT_SUBMENU = _T("TNotify_DeleteListNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//---------------------------------------------------------------------------
//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_DELETE_NODE = _T("TNotify_DeleteTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_INSERT_NODE = _T("TNotify_InsertTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_UPDATE_NODE = _T("TNotify_UpdateTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_SELECT_NODE = _T("TNotify_SelectTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_EXPAND_NODE = _T("TNotify_ExpandTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_DBCLICK_NODE = _T("TNotify_DBClickTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_LBUTTONDOWN_NODE = _T("TNotify_LButtonDownTreeNode");
//********************************************************//


//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of FireUIParam<T>
//Tree pop-up menu use
const LPCTSTR EVENT_TREE_RBUTTONDOWN_NODE = _T("TNotify_RButtonDownTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeUI
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_ITEM_BUTTONDOWN_NODE = _T("TNotify_Item_ButtonDownTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeDataSource
//---------------------------------------------------------------------------
//********************************************************//
//Event list for IQTreeDataSource
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_DATA_DELETE_NODE = _T("TNotify_DATA_DeleteTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeDataSource
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_DATA_INSERT_NODE = _T("TNotify_DATA_InsertTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeDataSource
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_DATA_UPDATE_NODE = _T("TNotify_DATA_UpdateTreeNode");
//********************************************************//

//********************************************************//
//Event list for IQTreeDataSource
//Event parameter is pointer of CQTreeNode
const LPCTSTR EVENT_TREE_DATA_EXPAND_NODE = _T("TNotify_DATA_ExpandTreeNode");
//********************************************************//

//********************************************************//
//Event parameter is pointer of CTreeEditParam
const LPCTSTR EVENT_TREE_UI_EDIT_BEGIN      = _T("TNotify_TREE_UI_EDIT_BEGIN");
const LPCTSTR EVENT_TREE_UI_EDIT_END		= _T("TNotify_TREE_UI_EDIT_END");
//********************************************************//

//********************************************************//
//Event parameter is the HWND whose focus is changed
const LPCTSTR EVENT_TREE_UI_KILLFOCUS					= _T("TNotify_TREE_UI_KillFocus");
const LPCTSTR EVENT_TREE_UI_SETFOCUS					= _T("TNotify_TREE_UI_SetFocus");
const LPCTSTR EVENT_LIST_UI_KILLFOCUS					= _T("TNotify_LIST_UI_KillFocus");
const LPCTSTR EVENT_LIST_UI_SETFOCUS					= _T("TNotify_LIST_UI_SetFocus");
//********************************************************//

//********************************************************//
//Event list for IQListCtrlUI
//Event parameter is pointer of vector<FireListDataParameter*>
//Hit delete-key or delete-button
const LPCTSTR EVENT_LIST_UI_DELETE_ROW      = _T("TNotify_UI_DeleteListRow");
//Insert a row from data source
const LPCTSTR EVENT_LIST_UI_INSERT_ROW      = _T("TNotify_UI_InsertListRow");
//Update a row
const LPCTSTR EVENT_LIST_UI_UPDATE_ROW      = _T("TNotify_UI_UpdateListRow");
//Update an indexded data, parameter is a pointer to CBasicData and its index
const LPCTSTR EVENT_LIST_UI_UPDATE_DATA     = _T("TNotify_UI_UpdateListData");
//Select change of a row, hit up-key, down-key, left-key, right-key
const LPCTSTR EVENT_LIST_UI_SELECT_ROW      = _T("TNotify_UI_SelectListRow");
//Mouse left button double click
const LPCTSTR EVENT_LIST_UI_LDBCLICK        = _T("TNotify_UI_LDBClickListCtrl");
//Mouse right button double click
const LPCTSTR EVENT_LIST_UI_RDBCLICK        = _T("TNotify_UI_RDBClickListCtrl");
//Mouse left button click
const LPCTSTR EVENT_LIST_UI_LCLICK          = _T("TNotify_UI_LClickListCtrl");
//Mouse right button click
const LPCTSTR EVENT_LIST_UI_RCLICK          = _T("TNotify_UI_RClickListCtrl");
//Mouse left button down
const LPCTSTR EVENT_LIST_UI_LBTNDOWN        = _T("TNotify_UI_LButtonDownListCtrl");
//Mouse right button down
const LPCTSTR EVENT_LIST_UI_RBTNDOWN        = _T("TNotify_UI_RButtonDownListCtrl");

//Event parameter is pointer of vector<FireListDataParameter>
const LPCTSTR EVENT_LIST_UI_ITEMCHANGED     = _T("TNotify_UI_ITEMCHANGED_ListCtrl");
//Mouse drag

//Event parameter is pointer of CDragDropFireParam
const LPCTSTR EVENT_LIST_UI_DRAG_BEGIN      = _T("TNotify_UI_DragListCtrl");
//Event parameter is pointer of NULL
const LPCTSTR EVENT_DRAG_END      = _T("TNotify_UI_DragEnd");

//Event parameter is pointer of CEditParam
const LPCTSTR EVENT_LIST_UI_EDIT_BEGIN      = _T("TNotify_UI_EDIT_BEGIN");
const LPCTSTR EVENT_LIST_UI_EDIT_END		= _T("TNotify_UI_EDIT_END");

// Drag and Drop Message
//Event parameter is pointer of CDragDropFireParam
const LPCTSTR EVENT_DRAG_BEGIN		= _T("TNotify_DragBegin");
//Mouse drop
//Event parameter is pointer of CDragDropFireParam
const LPCTSTR EVENT_DROP            = _T("TNotify_Drop");
//Mouse drag enter a UI component
const LPCTSTR EVENT_DRAG_ENTER      = _T("TNotify_DragEnter");
//Mouse drag over a UI component
const LPCTSTR EVENT_DRAG_OVER       = _T("TNotify_DragOver");
//Mouse drag leave a UI component
const LPCTSTR EVENT_DRAG_LEAVE      = _T("TNotify_DragLeave");
//Event parameter is pointer of CDragDropFireParam
const LPCTSTR EVENT_TREE_UI_DROP    = _T("TNotify_UI_DropTreeCtrl");
//Event parameter is pointer of CDragDropFireParam
const LPCTSTR EVENT_TREE_UI_DRAG_BEGIN    = _T("TNotify_UI_DragTreeCtrl");




//Mouse drop
//Event parameter is pointer of CDragDropFireParam
const LPCTSTR EVENT_LIST_UI_DROP            = _T("TNotify_UI_DropListCtrl");
//Mouse drag enter a UI component
const LPCTSTR EVENT_LIST_UI_DRAG_ENTER      = _T("TNotify_UI_DragEnterListCtrl");
//Mouse drag over a UI component
const LPCTSTR EVENT_LIST_UI_DRAG_OVER       = _T("TNotify_UI_DragOverListCtrl");
//Mouse drag leave a UI component
const LPCTSTR EVENT_LIST_UI_DRAG_LEAVE      = _T("TNotify_UI_DragLeaveListCtrl");
//List control header button hit
const LPCTSTR EVENT_LIST_UI_HEADER_BUTTON_HIT = _T("TNotify_UI_HeaderBtnHit");


// UI Behavior
const LPCTSTR EVENT_LIST_UI_ONSIZE          = _T("TNotify_UI_OnSize");
const LPCTSTR EVENT_LIST_UI_ONSIZING        = _T("TNotify_UI_OnSizing");

//********************Event list for CQDataCtrl********************//
//Event parameter is pointer of fdp (class FireDataParam)
//Data source delete a row
const LPCTSTR EVENT_LIST_DATA_DELETE_NODE = _T("TNotify_DATA_DeleteListNode");
//Data source insert a row
const LPCTSTR EVENT_LIST_DATA_INSERT_NODE = _T("TNotify_DATA_InsertListNode");
//Data source update a row
const LPCTSTR EVENT_LIST_DATA_UPDATE_NODE = _T("TNotify_DATA_UpdateListNode");
//Event parameter is pointer of fcp (class FireColumnParam)
//Data source delete a column
const LPCTSTR EVENT_LIST_COLUMN_DELETE_NODE = _T("TNotify_COLUMN_DeleteListNode");
//Data source insert a column
const LPCTSTR EVENT_LIST_COLUMN_INSERT_NODE = _T("TNotify_COLUMN_InsertListNode");
//Data source update a column
const LPCTSTR EVENT_LIST_COLUMN_UPDATE_NODE = _T("TNotify_COLUMN_UpdateListNode");
//Event parameter is pointer of vector<CQListData*>
//Data source sort by a column name, after sorted
const LPCTSTR EVENT_LIST_COLUMN_SORTOK = _T("TNotify_COLUMN_SORTOK");
//*****************************************************************//

//*****************************************************************//
//Event list for IQProgressClient
//Event parameter is a pointer to an QProgressNotify structure
const LPCTSTR EVENT_PROGRESS_TASK_FINISH		= _T("TNotify_TASK_Finish");
const LPCTSTR EVENT_PROGRESS_TASK_ABORT			= _T("TNotify_TASK_Abort");
const LPCTSTR EVENT_PROGRESS_TASK_UPDATE		= _T("TNotify_TASK_Update");
//Event parameter is a pointer of HWND
const LPCTSTR EVENT_PROGRESS_TASK_GETHWMD		= _T("TNotify_TASK_GetHwnd");
//********************************************************//


//********************************************************//
// SMS Server Notifications (to Logical Layer)
const LPCTSTR EVENT_SMSSERVER_READ_MESSAGE      = _T("TNotify_SMSServer_Read_Message");
const LPCTSTR EVENT_SMSSERVER_DELETE_MESSAGE    = _T("TNotify_SMSServer_Delete_Message");
const LPCTSTR EVENT_SMSSERVER_SAVE_MESSAGE      = _T("TNotify_SMSServer_Save_Message");
const LPCTSTR EVENT_SMSSERVER_LIST_ALL_MESSAGE  = _T("TNotify_SMSServer_List_All_Message");
const LPCTSTR EVENT_SMSSERVER_QUERY_SCA         = _T("TNotify_SMSServer_Query_CSA");
const LPCTSTR EVENT_SMSSERVER_SEND_MESSAGE      = _T("TNotify_SMSServer_Send_Message");
const LPCTSTR EVENT_SMSSERVER_BACKUP_MESSAGE    = _T("TNotify_SMSServer_Backup_Message");
const LPCTSTR EVENT_SMSSERVER_RESTORE_MESSAGE   = _T("TNotify_SMSServer_Restore_Message");
const LPCTSTR EVENT_SMSSERVER_QUERY_FOLDER      = _T("TNotify_SMSServer_Query_Folder");


}

#endif // !defined(EA_12097F6D_0B61_4009_AA2A_989972578994__INCLUDED_)
