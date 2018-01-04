#include <assert.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma once

// QImageEncoder
//
// Encapsulation of GDI+ GetImageEncoders() function, used to save an Image.
// Use at your own risk. Comments welcome.
//
// Version 1.0 (c) 2003, Sjaak Priester, Amsterdam.
// mailto:sjaak@sjaakpriester.nl

class QImageEncoder
{
public:
	QImageEncoder(LPCTSTR s);
	~QImageEncoder(void);
protected:
	CLSID m_Clsid;
	bool m_bStatus;
	CString m_MimeType;
public:
	Status Save(Image * pImage, LPCTSTR pPath);
	long m_Quality;
	int m_Compression;
};
