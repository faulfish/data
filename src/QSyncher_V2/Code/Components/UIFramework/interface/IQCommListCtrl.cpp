///////////////////////////////////////////////////////////
//  CQCommListCtrl.cpp
//  Implementation of the Class CQCommListCtrl
//  Created on:      15-Feb-2007 11:07:34
//  Original author: Eddy Guo
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IQCommListCtrl.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB
{
	CQCommListCtrl::CQCommListCtrl(IDataSource* source, IQListCtrlUI* listCtrl)
		:m_pQDataSource(source), m_pQListCtrl(listCtrl) 
	{
		TRACE("CQCommListCtrl Constructor...\n");
		if(m_pQDataSource == NULL || m_pQListCtrl == NULL) ASSERT(false);
		DefineNotify(EVENT_LIST_DATA_DELETE_NODE);
		DefineNotify(EVENT_LIST_DATA_INSERT_NODE);
		DefineNotify(EVENT_LIST_DATA_UPDATE_NODE);
		DefineNotify(EVENT_LIST_COLUMN_DELETE_NODE);
		DefineNotify(EVENT_LIST_COLUMN_INSERT_NODE);
		DefineNotify(EVENT_LIST_COLUMN_UPDATE_NODE);
		DefineNotify(EVENT_LIST_COLUMN_SORTOK);

		DefineNotify(EVENT_LIST_UI_DELETE_ROW);
		DefineNotify(EVENT_LIST_UI_INSERT_ROW);
		DefineNotify(EVENT_LIST_UI_UPDATE_ROW);
		DefineNotify(EVENT_LIST_UI_UPDATE_DATA);
		DefineNotify(EVENT_LIST_UI_SELECT_ROW);
		DefineNotify(EVENT_LIST_UI_LBTNDOWN);
		DefineNotify(EVENT_LIST_UI_RBTNDOWN);
		DefineNotify(EVENT_LIST_UI_LDBCLICK);
		DefineNotify(EVENT_LIST_UI_RDBCLICK);
		DefineNotify(EVENT_LIST_UI_LCLICK);
		DefineNotify(EVENT_LIST_UI_RCLICK);
		DefineNotify(EVENT_LIST_UI_DROP);
		DefineNotify(EVENT_LIST_UI_DRAG_ENTER);
		DefineNotify(EVENT_LIST_UI_DRAG_OVER);
		DefineNotify(EVENT_LIST_UI_DRAG_LEAVE);
		DefineNotify(EVENT_LIST_UI_HEADER_BUTTON_HIT);
		DefineNotify(EVENT_LIST_UI_ITEMCHANGED);
		DefineNotify(EVENT_LIST_UI_DRAG_BEGIN);
		DefineNotify(EVENT_DRAG_END);		
		DefineNotify(EVENT_LIST_UI_EDIT_BEGIN);
		DefineNotify(EVENT_LIST_UI_EDIT_END);
        DefineNotify(EVENT_LIST_UI_ONSIZE);
        DefineNotify(EVENT_LIST_UI_ONSIZING);
		DefineNotify(EVENT_LIST_UI_KILLFOCUS);
		DefineNotify(EVENT_LIST_UI_SETFOCUS);

		// ���ǤJfunction pointer�����ϥ�Event�Ӱ�, �i�H�Ѧ�saffin���@�k, �ثe��������Event
		// �VData Source���U, �i�������ǰʧ@�����q��CommListCtrl
		m_pQDataSource->Register(this, EVENT_LIST_DATA_DELETE_NODE);
		m_pQDataSource->Register(this, EVENT_LIST_DATA_INSERT_NODE);
		m_pQDataSource->Register(this, EVENT_LIST_DATA_UPDATE_NODE);
		m_pQDataSource->Register(this, EVENT_LIST_COLUMN_DELETE_NODE);
		m_pQDataSource->Register(this, EVENT_LIST_COLUMN_INSERT_NODE);
		m_pQDataSource->Register(this, EVENT_LIST_COLUMN_UPDATE_NODE);
		m_pQDataSource->Register(this, EVENT_LIST_COLUMN_SORTOK);		

		// �VUI���U, �i�������ǰʧ@�����q��CommListCtrl
		m_pQListCtrl->Register(this, EVENT_LIST_UI_DELETE_ROW);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_INSERT_ROW);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_UPDATE_ROW);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_UPDATE_DATA);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_SELECT_ROW);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_LBTNDOWN);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_RBTNDOWN);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_LDBCLICK);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_RDBCLICK);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_LCLICK);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_RCLICK);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_DROP);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_DRAG_ENTER);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_DRAG_OVER);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_DRAG_LEAVE);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_HEADER_BUTTON_HIT);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_ITEMCHANGED);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_DRAG_BEGIN);
		m_pQListCtrl->Register(this, EVENT_DRAG_END);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_EDIT_BEGIN);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_EDIT_END);
        m_pQListCtrl->Register(this, EVENT_LIST_UI_ONSIZE);
        m_pQListCtrl->Register(this, EVENT_LIST_UI_ONSIZING);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_KILLFOCUS);
		m_pQListCtrl->Register(this, EVENT_LIST_UI_SETFOCUS);
	}

	CQCommListCtrl::~CQCommListCtrl() 
	{
		TRACE("CQCommListCtrl Destructor...\n");
		m_pQDataSource->Unregister(this, EVENT_LIST_DATA_DELETE_NODE);
		m_pQDataSource->Unregister(this, EVENT_LIST_DATA_INSERT_NODE);
		m_pQDataSource->Unregister(this, EVENT_LIST_DATA_UPDATE_NODE);
		m_pQDataSource->Unregister(this, EVENT_LIST_COLUMN_DELETE_NODE);
		m_pQDataSource->Unregister(this, EVENT_LIST_COLUMN_INSERT_NODE);
		m_pQDataSource->Unregister(this, EVENT_LIST_COLUMN_UPDATE_NODE);
		m_pQDataSource->Unregister(this, EVENT_LIST_COLUMN_SORTOK);

		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_DELETE_ROW);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_INSERT_ROW);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_UPDATE_ROW);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_UPDATE_DATA);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_SELECT_ROW);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_LBTNDOWN);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_RBTNDOWN);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_LDBCLICK);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_RDBCLICK);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_LCLICK);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_RCLICK);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_DROP);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_DRAG_ENTER);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_DRAG_OVER);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_DRAG_LEAVE);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_HEADER_BUTTON_HIT);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_ITEMCHANGED);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_EDIT_BEGIN);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_EDIT_END);
        m_pQListCtrl->Unregister(this, EVENT_LIST_UI_ONSIZE);
        m_pQListCtrl->Unregister(this, EVENT_LIST_UI_ONSIZING);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_KILLFOCUS);
		m_pQListCtrl->Unregister(this, EVENT_LIST_UI_SETFOCUS);
	}

	// Logic���O��UI��Source�ҶǨӪ�Notifies���B�z, �`�N�C�Ӧ^�ǭ�
	void CQCommListCtrl::Event(const TSTRING& strEvent, long nParam) 
	{
		FireListDataParameter* rowData = NULL;
		FireColumnDataParameter* colData = NULL;
		if(strEvent == EVENT_LIST_UI_INSERT_ROW) 
		{
			rowData = (FireListDataParameter*)nParam;
			int nRow = rowData->nRow; 
			CQListData* pListData = rowData->pListData;
			m_pQDataSource->InsertDataByIndex(pListData,nRow);

		} 
		else if(strEvent == EVENT_LIST_UI_UPDATE_ROW) 
		{
			rowData = (FireListDataParameter*)nParam;
			int nRow = rowData->nRow; 
			CQListData* pListData = rowData->pListData;
			m_pQDataSource->UpdateDataByIndex(pListData,nRow);

		}
		else if(strEvent == EVENT_LIST_DATA_DELETE_NODE) 
		{
			// delete a row
			rowData = (FireListDataParameter*)nParam;
			m_pQListCtrl->DeleteRowByIndex(rowData->nRow);
		} 
		else if(strEvent == EVENT_LIST_DATA_INSERT_NODE) 
		{
			// insert a row
			rowData = (FireListDataParameter*)nParam;
			m_pQListCtrl->InsertDataByIndex(rowData->pListData, rowData->nRow);
		} 
		else if(strEvent == EVENT_LIST_DATA_UPDATE_NODE) 
		{
			// update a row
			rowData = (FireListDataParameter*)nParam;
			m_pQListCtrl->UpdateRowByIndex(rowData->nRow, rowData->pListData);
		} 
		else if(strEvent == EVENT_LIST_COLUMN_DELETE_NODE) 
		{
			// delete a column
			colData = (FireColumnDataParameter*)nParam;
			m_pQListCtrl->DeleteColumnByKey(colData->pColumnData->m_strKey);
		} 
		else if(strEvent == EVENT_LIST_COLUMN_INSERT_NODE) 
		{
			// insert a column
			colData = (FireColumnDataParameter*)nParam;
			m_pQListCtrl->InsertColumn(m_pQListCtrl->GetColumnCount(), colData->pColumnData);
		} 
		else if(strEvent == EVENT_LIST_COLUMN_UPDATE_NODE) 
		{
			// update a column
			colData = (FireColumnDataParameter*)nParam;
			m_pQListCtrl->UpdateColumnByKey(colData->pColumnData->m_strKey, colData->pColumnData->m_strDisplay);
		} 
		else if(strEvent == EVENT_LIST_COLUMN_SORTOK) 
		{
			while(m_pQListCtrl->GetRowCount() > 0) {
				m_pQListCtrl->DeleteRowByIndex(0);
			};
			typedef std::vector<CQListData*> DataVec;
			DataVec* vec = (std::vector<CQListData*>*)nParam;
			DataVec::iterator iter = vec->begin();
			for(iter = vec->begin(); iter != vec->end(); iter++) {
				m_pQListCtrl->InsertDataByIndex(*iter, -1);
			}

			// sort ok, reload all data
			// �b�o�̭n����insert��list control���Y, �`�N�^�Ǧ^�Ӫ��F��
			// �O�@�ӥ]�t�Ҧ���ƪ�pointer...
		}
		else 
		{
			Fire(strEvent,nParam);
		}
	}

}  // end of namespace


#pragma warning(pop)