// QImageEncoder
//
// Encapsulation of GDI+ GetImageEncoders() function, used to save an Image.
// Use at your own risk. Comments welcome.
//
// Version 1.0 (c) 2003, Sjaak Priester, Amsterdam.
// mailto:sjaak@sjaakpriester.nl

#include "stdafx.h"
#include "QImageEncoder.h"

// QImageEncoder can be constructed from file path or from mime type
QImageEncoder::QImageEncoder(LPCTSTR s)
: m_Quality(75)
, m_Compression(EncoderValueCompressionLZW)
{
	m_bStatus = false;

	CString sPath(s);
	sPath.MakeLower();
	int dot = sPath.ReverseFind(_T('.'));
	if (dot > -1) sPath = _T("*") + sPath.Right(sPath.GetLength() - dot);

	UINT count = 0;
	UINT size = 0;

	if (GetImageEncodersSize(& count, & size) != Ok) return;

	BYTE * pBytes = new BYTE[size];
	if (! pBytes) return;

	ImageCodecInfo * pInfo = (ImageCodecInfo *) pBytes;

	if (GetImageEncoders(count, size, pInfo) != Ok) return;

	UINT i = 0;
	for ( i = 0; i < count; i++)
	{
		CString sExt(pInfo->FilenameExtension);
		sExt.MakeLower();

		CString sMime(pInfo->MimeType);
		sMime.MakeLower();

		if (sMime == sPath || sExt.Find(sPath) >= 0)
		{
	        m_Clsid = pInfo->Clsid;
			m_MimeType = sMime;
			break;
		}
		pInfo++;
	}
	delete[] pBytes;
	m_bStatus = (i < count);
}

QImageEncoder::~QImageEncoder(void)
{
}

Status QImageEncoder::Save(Image * pImage, LPCTSTR pPath)
{
	if (! m_bStatus) return GenericError;

	CString s(pPath);

	EncoderParameters params;
	EncoderParameters * pParams = NULL;

	params.Count = 1;
	params.Parameter[0].Type = EncoderParameterValueTypeLong;
	params.Parameter[0].NumberOfValues = 1;

	if (m_MimeType == _T("image/jpeg"))
	{
		pParams = & params;
		params.Parameter[0].Guid = EncoderQuality;
		params.Parameter[0].Value = & m_Quality;
	}
	else if (m_MimeType == _T("image/tiff"))
	{
		// With GDI+ 1.0, EncoderValueCompressionLZW and EncoderValueCompressionNone
		// are the only values that work. EncoderValueCompressionCCITT3, -CCITT4, and -Rle
		// always let fail a save operation with a InvalidParameter error.
		// (Could it be that they only work with monochrome bitmaps?)
		// Because of that, for the time being we leave m_Compression at
		// EncoderValueCompressionLZW. As this is also the default, we might as well
		// refrain from setting the parameter alltogether. But we leave the code, as it
		// probably doesn't hurt.
		pParams = & params;
		params.Parameter[0].Guid = EncoderCompression;
		params.Parameter[0].Value = & m_Compression;
	}

	return pImage->Save(s.AllocSysString(), & m_Clsid, pParams);
}
