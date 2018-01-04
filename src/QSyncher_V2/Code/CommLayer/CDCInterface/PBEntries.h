// PBEntries.h: interface for the CPBEntries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PBENTRIES_H__5F2D713E_E049_4772_A01A_49B5F27DDB41__INCLUDED_)
#define AFX_PBENTRIES_H__5F2D713E_E049_4772_A01A_49B5F27DDB41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"
#include <list>

class CPBEntries : public CRefObject  
{
public:
	CPBEntries();
	virtual ~CPBEntries();

private:
    class CPBEntry: public CRefObject
    {
    public:
        CPBEntry( long lIndex, LPCTSTR szName, LPCTSTR szNumber )
        {
            m_lIndex = lIndex;
            m_strName = szName;
            m_strNumber = szNumber;
        }
        ~CPBEntry(){}

        long GetIndex() const { return m_lIndex; }
        LPCTSTR GetName() const { return m_strName; }
        LPCTSTR GetNumber() const { return m_strNumber; }
    private:
        long m_lIndex;
        CString m_strName;
        CString m_strNumber;
    };
    typedef CRefObjectPtr<CPBEntry> CPBEntryPtr;

public:
    typedef std::list<CPBEntryPtr>::iterator Iterator;


public:
    Iterator Begin();
    Iterator End();
    Iterator NextIterator( Iterator pos );
    DWORD GetLength();
    
    void Insert( long lIndex, LPCTSTR szName, LPCTSTR szNumber );
    Iterator Remove( Iterator pos );
    long GetIndex( Iterator pos );
    LPCTSTR GetName( Iterator pos );
    LPCTSTR GetNumber( Iterator pos );

    
private:
    std::list<CPBEntryPtr> m_PBEntriesList;
};

typedef CRefObjectPtr<CPBEntries> CPBEntriesPtr;

#endif // !defined(AFX_PBENTRIES_H__5F2D713E_E049_4772_A01A_49B5F27DDB41__INCLUDED_)
