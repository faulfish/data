// FolderListingCodec.h: interface for the CFolderListingCodec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOLDERLISTINGCODEC_H__4B3E8BA5_60F0_4A73_9F61_851238625B16__INCLUDED_)
#define AFX_FOLDERLISTINGCODEC_H__4B3E8BA5_60F0_4A73_9F61_851238625B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "Buffer.h"
#include <vector>

class CFolderListingCodec  
{
public:
	CFolderListingCodec();
	virtual ~CFolderListingCodec();

public:
    bool DecodeBuffer( CBufferPtr spBuffer );

public:    
    DWORD GetNoOfEntries() const { return static_cast<DWORD>(m_listEntry.size()); }
    LPCTSTR GetName( DWORD dw ) const { return (dw > m_listEntry.size()) ? _T("") : m_listEntry[dw].m_strName; }
    DWORD GetFileSize( DWORD dw ) const { return (dw > m_listEntry.size()) ? 0 : m_listEntry[dw].m_dwFileSize; }
    bool GetAttributeHidden( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeHidden; }
    bool GetAttributeReadonly( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeReadonly; }
    bool GetAttributeSystem( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeSystem; }
    bool GetAttributeArchive( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeArchive; }
    bool GetAttributeDirectory( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeDirectory; }
    bool GetAttributeVolume( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeVolume; }
    bool GetAttributeRemoveable( DWORD dw ) const { return (dw > m_listEntry.size()) ? false : m_listEntry[dw].m_bAttributeRemoveable; }
    DATE GetCreationTime( DWORD dw ) const { return (dw > m_listEntry.size()) ? COleDateTime() : m_listEntry[dw].m_dateCreationTime; }
    DATE GetLastAccessTime( DWORD dw ) const { return (dw > m_listEntry.size()) ? COleDateTime() : m_listEntry[dw].m_dateLastAccessTime; }
    DATE GetLastWriteTime( DWORD dw ) const { return (dw > m_listEntry.size()) ? COleDateTime() : m_listEntry[dw].m_dateLastWriteTime; }
    bool ContainsFile( LPCTSTR szFilename );

private:
    HRESULT MakeListingXMLCompliant(std::string* strListing);

    class CEntry
    {
    public:
        CEntry() { reset(); }
        virtual ~CEntry() {}
        void reset() { m_dwFileSize = 0; m_bAttributeHidden = false; m_bAttributeReadonly = false; m_bAttributeSystem = false;
            m_bAttributeArchive = false; m_bAttributeDirectory = false; m_bAttributeVolume = false;
            m_bAttributeRemoveable = false; }

        CString m_strName;
        DWORD m_dwFileSize;
        bool m_bAttributeHidden;
        bool m_bAttributeReadonly;
        bool m_bAttributeSystem;
        bool m_bAttributeArchive;
        bool m_bAttributeDirectory;
        bool m_bAttributeVolume;
        bool m_bAttributeRemoveable;
        COleDateTime m_dateCreationTime;
        COleDateTime m_dateLastAccessTime;
        COleDateTime m_dateLastWriteTime;
    };

    std::vector<CEntry> m_listEntry;
};

#endif // !defined(AFX_FOLDERLISTINGCODEC_H__4B3E8BA5_60F0_4A73_9F61_851238625B16__INCLUDED_)
