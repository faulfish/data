#ifndef _BENQ_MOBILEPHONE_API_IPIMCONFIG_H_
#define _BENQ_MOBILEPHONE_API_IPIMCONFIG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
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

//PIM Class  Definition
typedef enum PIMTypeTag
{
  eContact,
  eCalendar,
  eNote,
  eTask,
  eEmail
}ePIMType; 


class CPIMGroupInfo
{
public:
	CPIMGroupInfo(int nID,const TSTRING& strDisplayText)
		:m_nID(nID)
		,m_strDisplayText(strDisplayText)
	{
	}
	~CPIMGroupInfo()
	{
	}
	int			m_nID;
	TSTRING		m_strDisplayText;
	TSTRING		m_strImageID;
	TSTRING		m_strImagePath;
	TSTRING		m_strMelodyID;
	TSTRING		m_strMelodyPath;
};


/***************************************************************************/
/* IPIMCONFIG Interface                                                    */
/***************************************************************************/

interface IPIMCONFIG
{
  virtual void	STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) = 0;
  virtual int		STDMETHODCALLTYPE ListPIMGroup(ePIMType eType, std::vector<CPIMGroupInfo> *vGroupList) = 0;
  virtual int		STDMETHODCALLTYPE SetPIMGroup(ePIMType eType, int nGroupID, CPIMGroupInfo &setGroup) = 0;
  virtual int		STDMETHODCALLTYPE CreatePIMGroup(CPIMGroupInfo &newGroup, int &nNewID) = 0;
  virtual int		STDMETHODCALLTYPE DeletePIMGroup(int nDeleteID) = 0;
  virtual int		STDMETHODCALLTYPE GetProperty(const TSTRING& strGroup,const TSTRING& strName,TSTRING& strValue) = 0;
  virtual int		STDMETHODCALLTYPE GetIntegerProperty(const TSTRING& strGroup,const TSTRING& strName,int& nValue) = 0;
};

#endif