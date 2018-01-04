
#include "stdafx.h"
#include "utility.h"


CString dirname( LPCTSTR szPathname )
{
    CString strPathname(szPathname);
    CString strDirname;

    int nResult = strPathname.ReverseFind( _TCHAR('\\') );
    if( nResult != -1 && nResult != 0 )
        strDirname = strPathname.Mid( 0, nResult );

    return strDirname;
}

CString basename( LPCTSTR szPathname )
{
    CString strPathname(szPathname);
    CString strBasename;

    int nResult = strPathname.ReverseFind( _TCHAR('\\') );
    if( nResult == -1 )
        strBasename = strPathname;
    else
        strBasename = strPathname.Mid( nResult+1 );

    return strBasename;
}


CString adjustslash( LPCTSTR szPathname )
{
    CString strPathname(szPathname);
    CString strBasename;

    if( strPathname.IsEmpty() )
    {
        return strBasename;
    }
    
    int nCounter = 0;
    if( strPathname[nCounter] != _TCHAR('//') && 
        strPathname[nCounter] != _TCHAR('\\') )
    {
        strBasename += _TCHAR('\\');
    }

    // convert '/' to '\'
    for( nCounter; nCounter < strPathname.GetLength(); nCounter++ )
    {
        if( strPathname[ nCounter ] == _TCHAR('//') )
        {
            strBasename += _TCHAR('\\');
        }
        else
        {
            strBasename += strPathname[ nCounter ];
        }
    }

    return strBasename;
}

bool IsPathSeperator( _TCHAR c )
{
    bool bResult = false;

    if( c == _TCHAR('/') || c == _TCHAR('\\') )
        bResult = true;

    return bResult;
}

bool IsValidFilename( LPCTSTR szFilename )
{
    bool bValid = true;

    if( szFilename == NULL )
    {
        bValid = false;
    }
    else
    {
        CString strFilename = szFilename;

        // Filename         :- Path, { Path }
        // Path             :- Seperator, Ident
        // Seperator        :- '/' | '\'
        // Ident            :- Alnum, { Alnum }
        // Alnum            :- "ascii token which is not a Seperator"

        // The filename may not be empty
        if( strFilename.GetLength() == 0 )
            bValid = false;
        else if( !IsPathSeperator(strFilename[0]) )
            bValid = false;
        else if( IsPathSeperator(strFilename[strFilename.GetLength()-1]) )
            bValid = false;

        // Missing: two adjacent seperators not allowed
    }

    return bValid;
}





