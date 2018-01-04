#ifndef CNVITEMFILE_H
#define CNVITEMFILE_H

#include "CNVItem.h"

class CNVItemFile
{
public:
	enum{
		CNV_ITEM_FILE_ERROR_FILE_NAME,
		CNV_ITEM_FILE_ERROR_FILE_OPERATOR,
		CNV_ITEM_FILE_ERROR_MAX
	}CNV_ITEM_FILE_ERROR_TYPE;

	CNVItemFile()
	{
	};
	virtual ~CNVItemFile()
	{
	}
protected:
	const CString& GetFileName() const
	{
		return m_strFileName;
	};
	bool SetFileName(LPCTSTR szFileName)
	{
		bool bRes = false;
		if ( NULL !=  szFileName )	// NULL pointer, set operation failed. Return false.
		{
			if ( 0 != _tcslen(szFileName) )	// Empty string, set operation failed. Return false.
			{
				m_strFileName = szFileName;
				bRes = true;
			}
		}
		return bRes;
	}
public:
	virtual int Open(LPCTSTR szFileName, UINT nFlag) = 0;
	virtual int Close() = 0;	
private:
	CString m_strFileName;
};
#endif