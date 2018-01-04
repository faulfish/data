#ifndef __CREATE_RGN_FROM_FILE_H__
#define __CREATE_RGN_FROM_FILE_H__

#pragma once
#include "afxwin.h"
#include "CommUIExportAPI.h"
#include <string>

class COMMONUI_API CCreateRgnFromFile
{
public:
    CCreateRgnFromFile(CString fileName, COLORREF color);
    ~CCreateRgnFromFile(void);
public:
    HRGN CreateRgnFromFile();
    HRGN GetResizeRgn(int cx, int cy);

public:
    CString m_fileName;
    COLORREF m_color;
    HBITMAP m_hBitmap;
    HDC     m_hDC;
    long    m_nWidth;
    long    m_nHeight;
};


#endif // __CREATE_RGN_FROM_FILE_H__