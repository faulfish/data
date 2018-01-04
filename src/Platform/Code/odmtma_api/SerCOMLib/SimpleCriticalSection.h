// CriticalSection.h: interface for the CCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSECTION_H__E73AEFEB_F51C_41E9_BF9C_6652DA0C571F__INCLUDED_)
#define AFX_CRITICALSECTION_H__E73AEFEB_F51C_41E9_BF9C_6652DA0C571F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimpleCriticalSection
{
public:
    void Lock() { EnterCriticalSection(&m_sec); }
    void Unlock() { LeaveCriticalSection(&m_sec); }
    CSimpleCriticalSection() { InitializeCriticalSection(&m_sec); } 
    ~CSimpleCriticalSection() { DeleteCriticalSection(&m_sec); }
private:
    CRITICAL_SECTION m_sec;
};

#endif // !defined(AFX_CRITICALSECTION_H__E73AEFEB_F51C_41E9_BF9C_6652DA0C571F__INCLUDED_)
