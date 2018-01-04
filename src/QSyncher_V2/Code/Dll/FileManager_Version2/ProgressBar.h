#ifndef _PROGRESSBAR_HPP_
#define _PROGRESSBAR_HPP_

#pragma once

#include "..\..\Components\UIFramework\interface\IQProgressClient.h"
#include "..\..\Components\Localization\ilocalization.h"
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
namespace QSYNC_LIB 
{

	class ProgressBar : public QProgressClientBase , public INotify
	{
	public :
		ProgressBar():m_pILocalization(NULL),m_AbortEvent(FALSE,TRUE,_T("EFS_ABOUT_EVENT"))
		{
			CreateILocalization(&m_pILocalization);
		}
		virtual ~ProgressBar(){}
		void InitDownload(bool bCanCancel,CQMEFileTreeDataSource* datasource,const CString& strPCFolder, vector<CString> &vct_strFilePath)
		{
			m_bUploadFunction = false;
			m_bCanCancel = bCanCancel;
			m_pDatasource = datasource;
			m_vct_strFilePath = vct_strFilePath;
			m_AbortEvent.ResetEvent();
			m_strPCFolder = strPCFolder;
		}
		void InitUpload(bool bCanCancel,CQMEFileTreeDataSource* datasource,CQTreeNode *pNode, vector<CString> &vct_strFilePath)
		{
			m_bUploadFunction = true;
			m_bCanCancel = bCanCancel;
			m_pDatasource = datasource;
			m_pNode = pNode;
			m_vct_strFilePath = vct_strFilePath;
			m_AbortEvent.ResetEvent();
		}

	public:
		virtual CString GetTitle()
		{
			return m_pILocalization->GetString(_T("IDS_FileMgr_Copy_File_Title")).c_str();
		}
		virtual CString GetDisplayText()
		{
			return m_pILocalization->GetString(_T("IDS_Initiation")).c_str();
		}
		virtual CString GetCancelButtonCaption()
		{
			return m_pILocalization->GetString(_T("IDS_Cancel")).c_str();
		}
		virtual CString GetProgressCaption()
		{
			return _T("");
		}
		virtual BOOL GetAutoClose()
		{
			return TRUE;
		}
		virtual BOOL GetCancelable(void)
		{
			return m_bCanCancel;
		}

		// To implemtnet INotify interface
		virtual void	Event(const TSTRING& strEvent, long nParam)
		{
			if(strEvent == EVENT_PROGRESS_TASK_UPDATE)
				Fire(EVENT_PROGRESS_TASK_UPDATE,nParam);
		}
	public:
		virtual bool StartTask(void)
		{
			HWND hwnd = NULL;
			Fire(EVENT_PROGRESS_TASK_GETHWMD,(long)&hwnd);

			bool bRes = false;
			if(m_pDatasource)
			{
				m_pDatasource->Register(this,EVENT_PROGRESS_TASK_UPDATE);
				if(m_bUploadFunction)
					bRes = m_pDatasource->UploadFilesIntoME(m_AbortEvent,m_pNode, m_vct_strFilePath,hwnd);
				else
					bRes = m_pDatasource->DownloadFilesIntoPC(m_AbortEvent,m_strPCFolder,m_vct_strFilePath,hwnd);
			}
			Fire(EVENT_PROGRESS_TASK_FINISH);
			return bRes;
		}
		virtual bool AbortTask(void)
		{
			m_AbortEvent.SetEvent();
			return true;
		}
		virtual bool ResumeTask(void)
		{
			return false;
		}
		virtual bool SuspendTask(void)
		{
			return true;
		}

	private:
		ILocalization*			m_pILocalization;
		CQMEFileTreeDataSource*	m_pDatasource;
		CQTreeNode*				m_pNode;
		std::vector<CString>	m_vct_strFilePath;
		bool					m_bCanCancel;
		CEvent					m_AbortEvent;
		CString					m_strPCFolder;
		bool					m_bUploadFunction;
	};

}//end of namespace QSYNC_LIB 

#pragma warning(pop)

#endif //_PROGRESSBAR_HPP_