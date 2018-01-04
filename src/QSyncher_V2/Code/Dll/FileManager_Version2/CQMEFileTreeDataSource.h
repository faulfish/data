/*$Log:



*/


///////////////////////////////////////////////////////////
//  CQMEFileTreeDataSource.h
//  Implementation of the Class CQMEFileTreeDataSource
//  Created on:      07-Feb-2007 15:14:57
//  Original author: YC Liang
///////////////////////////////////////////////////////////

#if !defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CQMEFileTreeDataSource__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CQMEFileTreeDataSource__INCLUDED_


#pragma once

#include "../../Components/UIFramework/interface/ITreeDatasourceBase.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "CMEFileNode.h"
#include "..\..\Components\Localization\Ilocalization.h"
#include "..\..\CommonUtil\Buffer.h"

namespace QSYNC_LIB
{
	class CFileManager_VersionView2;
	class CQMEFileTreeDataSource: public CITreeDatasourceBase , public INotify
	{
	public:
		CQMEFileTreeDataSource();
		virtual ~CQMEFileTreeDataSource(void);

		virtual CQTreeNode* GetRoot();
		virtual	bool		CreateNode(CQTreeNode* pNode);
		virtual	bool		ExpandNode(CQTreeNode* pNode);
		virtual	bool		DeleteNode(CQTreeNode* pNode);
		virtual	bool		UpdateNode(CQTreeNode* pNode);

		bool				UploadFilesIntoME(CEvent& aboutEvent,CQTreeNode *pNode, vector<CString> &vct_strFilePath,HWND hParent);
		bool				DownloadOneFile_ME2PC(CEvent& aboutEvent,const CString& strSrcMEPath, const CString& strPCPath,HWND hParent);
		bool				DownloadFilesIntoPC(CEvent& aboutEvent,const CString& strPCFolder, vector<CString> &vct_strMEFilePath,HWND hParent);

		IEFS*			GetIEFS(void);
		bool				IsIEFSConnecting(void);

		// µù¥U©M¤Ïµù¥Unotify functiion
		virtual	void		Event(const TSTRING& strEvent,long nParam);

		bool				GetDeviceConnection(void);
		bool				NewFolder(const CString& strFolder);
		int				GetMESize(void);
		bool				RecursiveDelNode(CQTreeNode* pNode, bool bDelFlag = false);
		bool				CanRenameFile(const CString& strFolder,CMEFileNode *pNode);
		bool				RenameFile(const CString& strFolder,CMEFileNode *pNode,const CString& strNewFileName);
		bool				CanRenameFolder(const CString& strFolder,CMEFileNode *pNode);
		bool				RenameFolder(CMEFileNode *pNode,const CString& strNewFolderName);

		bool				DelFile(CQTreeNode* pNode);
		bool				DelFolder(CQTreeNode* pNode);
		bool				DelFileByFilePath(const CString& strFileName);
        void                DeleteFileManagerInterface();
		void				SetParentHWND(HWND hParent){m_hParent = hParent;}

	protected:
		bool				ExpandTwoLevel(CMEFileNode* pNode);
		bool				ExpandOneLevel(CMEFileNode* pNode);
		bool				IsMEConnected(void);
		bool				DelMEData(CMEFileNode* pFNode);
		CMEFileNode*		UploadOneFile_PC2ME(CEvent& aboutEvent,CMEFileNode* pNode,CString strSrcPCFilePath, CString strDstMEFolderPath, UINT &nFileSize);

		bool				DelFolderByFolderPath(CString strFileName);		
		void				NotifyProgress(const TSTRING& strDisplay , int nPos);
		void				AbortDownload();
		virtual bool	RecursiveDelFile(CQTreeNode* pNode);
		int				WriteDownloadFileBlock(int nID,BOOL& bLast,bool bFlush);

	private:
		////////////////////////////////////////////////////////////////////////////////
		//  Member data 
		////////////////////////////////////////////////////////////////////////////////		
		IEFS*					m_pIEFS;
		int						m_nUploadFileBlocks;
		int						m_nUploadedFileBlocks;
		ILocalization*		m_pILocalization;

		//For Download function
		CString				m_strNewFile;
		CFile					m_newFile;
		bool						m_bFileOpen;
		CEvent				m_DownloadAvailableEvent;	
		int						m_nTotalDownloadSize;
		int						m_nDownloadedSize;
		CCriticalSection	m_CritSection;
		CFileManager_VersionView2*	m_pParent;
		HWND				m_hParent;
		int						m_nTempProgress;
		std::vector<CBufferPtr>			m_vectBuffer;

	public:
		CMEFileNode*		FindNodeByName(const CString& strFileName);
	};

}

#endif//!defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CQMEFileTreeDataSource__INCLUDED_)
