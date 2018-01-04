// PBEntries.cpp: implementation of the CPBEntries class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PBEntries.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPBEntries::CPBEntries()
{

}

CPBEntries::~CPBEntries()
{
    m_PBEntriesList.clear();
}


CPBEntries::Iterator CPBEntries::Begin()
{
    return m_PBEntriesList.begin();
}

CPBEntries::Iterator CPBEntries::End()
{
    return m_PBEntriesList.end();
}

CPBEntries::Iterator CPBEntries::NextIterator( Iterator pos )
{
    return (++pos);
}

DWORD CPBEntries::GetLength()
{
    return  static_cast<DWORD>(m_PBEntriesList.size());
}

void CPBEntries::Insert( long lIndex, LPCTSTR szName, LPCTSTR szNumber )
{
    CPBEntryPtr pPBEntry = new CPBEntry(lIndex, szName, szNumber);
    m_PBEntriesList.push_back(pPBEntry);
}

CPBEntries::Iterator CPBEntries::Remove( CPBEntries::Iterator pos )
{
    return m_PBEntriesList.erase(pos);
}

long CPBEntries::GetIndex( CPBEntries::Iterator pos )
{
    return (*pos)->GetIndex();
}

LPCTSTR CPBEntries::GetName( CPBEntries::Iterator pos )
{
    return (*pos)->GetName();
}

LPCTSTR CPBEntries::GetNumber( CPBEntries::Iterator pos )
{
    return (*pos)->GetNumber();
}

