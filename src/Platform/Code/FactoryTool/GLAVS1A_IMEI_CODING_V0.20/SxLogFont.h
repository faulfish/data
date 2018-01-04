#if !defined(__LOGFONT_H_)
#define __LOGFONT_H_
#pragma once
//
//=======================================================
// This class encapsulates font properties and methods
//=======================================================
//
struct CSxLogFont : public LOGFONT
{
public:
//
// constructor 
//
	CSxLogFont
	(
		LONG fontHeight = 80, 
		LONG fontWeight = FW_NORMAL,
		BYTE fontItalic = false,
		LPCTSTR faceName = _T("MS Sans Serif")
	)
	{
		// clear the current object
		memset(this, 0, sizeof(*this));

		// store the height, weight, and italics
		lfHeight = fontHeight;
		lfWeight = fontWeight;
		lfItalic = fontItalic;

		// make sure that font name is not too long
		ASSERT( _tcslen(faceName) < LF_FACESIZE );
		_tcscpy(lfFaceName, faceName);

		// set other default parameters
		lfOutPrecision   = OUT_STROKE_PRECIS;
		lfClipPrecision  = CLIP_STROKE_PRECIS;
		lfQuality        = PROOF_QUALITY;
		lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	}
//
// sets a new font name
//
	void SetFaceName(LPCTSTR faceName)
	{
		if(!faceName) return;

		ASSERT(_tcslen(faceName) < LF_FACESIZE );
		lstrcpy(lfFaceName, faceName); 		 
	}
//
// sets a new font rotation angle
//
	int SetFontAngle(int angle)
	{
		int oldangle = lfEscapement;
		lfEscapement = angle;
		return oldangle;
	}
//
// sets the underline style
//
	int SetUnderline(int state)
	{
		int oldstate = lfUnderline;
		lfUnderline = state;
		return oldstate;
	}
//
// sets the strikeout style
//
	int SetStrikeOut(int state)
	{
		int oldstate = lfStrikeOut;
		lfStrikeOut = state;
		return oldstate;
	}
};

#endif
