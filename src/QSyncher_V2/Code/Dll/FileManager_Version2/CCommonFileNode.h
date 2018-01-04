

#if !defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CCommonFileNode__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CCommonFileNode__INCLUDED_
#pragma once

#include "../../Components/UIFramework/interface/IQTreeUI.h"

namespace QSYNC_LIB
{

	class CCommonFileNode : public CQTreeNode
	{
	public:
		CCommonFileNode(CCommonFileNode* pParent):
		  CQTreeNode(pParent),
			  m_nSize(0),
			  m_nAttr(0),
			  m_tTime(NULL),
			  m_IsModified(false)
		  {

		  }

		  virtual CCommonFileNode::~CCommonFileNode()
		  {
			  TSTRING strTemp = GetFullPath().c_str();
			  strTemp = _T("Delete: ") + strTemp + _T("\n");
			  TRACE(strTemp.c_str());
		  }

		  bool IsFolder() const
		  {
			  return (m_nAttr & FILE_ATTRIBUTE_DIRECTORY) != 0;
		  }

		  bool SetRealFilePath(const CString strRealPath)
		  {
			  return false;
		  }

		  bool SetFileAttr(UINT nFileAttr)
		  {
			  m_nAttr = nFileAttr;
			  return true;
		  }

		  unsigned int GetFileAttr() const
		  {
			  return m_nAttr;
		  }

		  TSTRING GetFileAttrInStr() const
		  {
			  TSTRING strDefault;
			  if((m_nAttr & FILE_ATTRIBUTE_ARCHIVE) != 0)
				  strDefault += _T('A');
			  if((m_nAttr | FILE_ATTRIBUTE_READONLY) != 0)
				  strDefault += _T('R');
			  if((m_nAttr | FILE_ATTRIBUTE_HIDDEN) != 0)
				  strDefault += _T('H');
			  if((m_nAttr | FILE_ATTRIBUTE_SYSTEM) != 0)
				  strDefault += _T('S');
			  return strDefault;
		  }

		  void SetFileSize(int nFileSize)
		  {
			  m_nSize = nFileSize;
		  }

		  int GetFileSize() const
		  {
			  return m_nSize;
		  }

		  TSTRING GetFileSizeInStr() const
		  {
			  TCHAR buffer[200] = {0};
			  ::_itot_s( m_nSize, buffer, 10);
			  return buffer;
		  }

		  void CCommonFileNode::SetFileTime(FILETIME in_FileTime)
		  {
			  m_tTime = in_FileTime;
		  }

		  const CTime& CCommonFileNode::GetFileTime() const
		  {
			  return m_tTime;
		  }

		  TSTRING GetFileTimeInStr() const
		  {
			  CString strTemp = m_tTime.Format(_T("%c"));
			  return LPCTSTR(strTemp);
		  }

		  void SetNodeAsFolder()
		  {
			  m_nAttr  = m_nAttr | FILE_ATTRIBUTE_DIRECTORY;
		  }

		  void SetNodeHasChild(bool bHasChild)
		  {
			  m_bHasChild = bHasChild;
		  }

		  void SetVirtualPath(const TSTRING& strVirtualPath)
		  {
			  m_strVirtualPath = strVirtualPath;
		  }

		  const TSTRING& GetVirtualPath() const
		  {
			  return m_strVirtualPath;
		  }

	protected:
		int 						m_nSize;
		unsigned int	    m_nAttr;
		CTime					m_tTime;
		TSTRING		    m_strVirtualPath;
		bool						m_IsModified;
	};

}//namespace end
#endif