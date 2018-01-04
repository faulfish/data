// RefObject.h: interface for the CRefObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REFOBJECT_H__A9A4D687_E0B2_4BAE_9190_DEA73BDC9F44__INCLUDED_)
#define AFX_REFOBJECT_H__A9A4D687_E0B2_4BAE_9190_DEA73BDC9F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRefObject  
{
public:
	CRefObject():m_lRefCount(0){}
	virtual ~CRefObject(){}

	long CRefObject::AddRef()
	{
		return ::InterlockedIncrement( &m_lRefCount );
	}

	long CRefObject::Release()
	{
		long lRef = ::InterlockedDecrement( &m_lRefCount );

		if( lRef == 0 )
			delete this;

		return lRef;
	}

private:
    long m_lRefCount;
};


template <class T> class CRefObjectPtr
{
public:
    CRefObjectPtr<T>()
    {
        m_pRefObject = NULL;
    }

    CRefObjectPtr<T>( T* pRefObject )
    {
        m_pRefObject = pRefObject;
        if( m_pRefObject )
            m_pRefObject->AddRef();
    }

    // Important for std::list (standard allocator)
    CRefObjectPtr<T>( const CRefObjectPtr<T>& rRefObjectPtr )
    {
        m_pRefObject = rRefObjectPtr.m_pRefObject;
        if( m_pRefObject )
            m_pRefObject->AddRef();
    }

    ~CRefObjectPtr<T>()
    {
        if( m_pRefObject )
            m_pRefObject->Release();
    }

    const CRefObjectPtr<T>& operator =( const CRefObjectPtr<T>& rRefObjectPtr )
    {
        if( m_pRefObject )
            m_pRefObject->Release();
        m_pRefObject = rRefObjectPtr.m_pRefObject;
        if( m_pRefObject )
            m_pRefObject->AddRef();
        return *this;
    }

    const CRefObjectPtr<T>& operator =( T* pRefObject )
    {
        if( m_pRefObject )
            m_pRefObject->Release();
        m_pRefObject = pRefObject;
        if( m_pRefObject )
            m_pRefObject->AddRef();
        return *this;
    }

    T* operator ->()
    {
        return m_pRefObject;
    }

    const T* operator ->() const
    {
        return m_pRefObject;
    }

    operator const bool() const
    {		
        return m_pRefObject != NULL;
    }

    T* GetRawPointer()
    {
        return m_pRefObject;
    }

private:
    T* m_pRefObject;
};


#endif // !defined(AFX_REFOBJECT_H__A9A4D687_E0B2_4BAE_9190_DEA73BDC9F44__INCLUDED_)
