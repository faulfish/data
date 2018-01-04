/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   tstl.h  $
 *  $Revision:   1.1  $
 *  $Date:   Nov 09 2007 12:59:08  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/tstl.h.-arc  $
 * 
 *    Rev 1.1   Nov 09 2007 12:59:08   Eddy Guo
 * Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file tstl.h
    @brief {brief description of tstl.h}

    {detail description of tstl.h}
*/
// tstl.h - header file for TCHAR equivalents of STL string classes
//
// Copyright (c) 2006 PJ Arends
//
// This file is provided "AS-IS". Use and/or abuse it in any way you feel fit.
//
 
#pragma once
 
#include <string>
 
namespace std
{
#if defined UNICODE || defined _UNICODE
 
    typedef wstring         tstring;
 
    typedef wstringbuf      tstringbuf;
    typedef wstringstream   tstringstream;
    typedef wostringstream  tostringstream;
    typedef wistringstream  tistringstream;
 
    typedef wstreambuf      tstreambuf;
 
    typedef wistream        tistream;
    typedef wiostream       tiostream;
 
    typedef wostream        tostream;
 
    typedef wfilebuf        tfilebuf;
    typedef wfstream        tfstream;
    typedef wifstream       tifstream;
    typedef wofstream       tofstream;
 
    typedef wios            tios;
 
#   define tcerr            wcerr
#   define tcin             wcin
#   define tclog            wclog
#   define tcout            wcout
 
#else // defined UNICODE || defined _UNICODE
 
    typedef string          tstring;
 
    typedef stringbuf       tstringbuf;
    typedef stringstream    tstringstream;
    typedef ostringstream   tostringstream;
    typedef istringstream   tistringstream;
 
    typedef streambuf       tstreambuf;
 
    typedef istream         tistream;
    typedef iostream        tiostream;
 
    typedef ostream         tostream;
 
    typedef filebuf         tfilebuf;
    typedef fstream         tfstream;
    typedef ifstream        tifstream;
    typedef ofstream        tofstream;
 
    typedef ios             tios;
 
#   define tcerr            cerr
#   define tcin             cin
#   define tclog            clog
#   define tcout            cout
 
#endif // defined UNICODE || defined _UNICODE
} // namespace std
