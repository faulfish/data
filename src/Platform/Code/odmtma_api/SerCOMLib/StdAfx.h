// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F0F616DC_DDB9_495D_AC64_CEDF21C216D3__INCLUDED_)
#define AFX_STDAFX_H__F0F616DC_DDB9_495D_AC64_CEDF21C216D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>

// TODO: reference additional headers your program requires here
#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define ERROR_MASK			0xF0000000

#define LOG_LEVEL_DEBUG		5
#define LOG_LEVEL_WARNING	3
#define	LOG_LEVEL_ERROR		1
#define LOG_LEVEL_NORMAL	0
#define LOG_LEVEL_NONE		(-1)

#define WEIRD_BYTE			0xA3
#define WEIRD_WORD			0xA3A3
#define WEIRD_DWORD			0xA3A3A3A3

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F0F616DC_DDB9_495D_AC64_CEDF21C216D3__INCLUDED_)
