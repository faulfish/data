/*********************************************************************/
/** @file:	QProgressCtrl.h
  
  Defines the CQProgressCtrl class.
  
  $Original Author:   Steven CC Chen  $
  $Version:	 $
  $Date:   Apr 01 2009 14:17:28  $
  */
/*********************************************************************/

/*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Components/UIFramework/Porting/QProgressCtrl.h.-arc  $
 * 
 *    Rev 1.1   Apr 01 2009 14:17:28   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:54:24   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Mar 05 2007 22:02:26   steven cc chen
 * Initial revision.
 */

#pragma once

#include "..\\CommUIExportAPI.h"


namespace QSYNC_LIB
{

#ifndef QPROGRESSCTRL_POS_MAX_VALUE
	#define QPROGRESSCTRL_POS_MAX_VALUE						65535
#endif

#ifndef QPROGRESSCTRL_DEFAULT_MIN_POS
	#define QPROGRESSCTRL_DEFAULT_MIN_POS					0
#endif

#ifndef QPROGRESSCTRL_DEFAULT_MAX_POS
	#define QPROGRESSCTRL_DEFAULT_MAX_POS					100
#endif

#ifndef QPROGRESSCTRL_DEFAULT_CAPTION
	#define QPROGRESSCTRL_DEFAULT_CAPTION					_T("")
#endif

#ifndef QPROGRESSCTRL_DEFAULT_CAPTION_PERCENT_CHAR
	#define QPROGRESSCTRL_DEFAULT_CAPTION_PERCENT_CHAR		_T("%")
#endif

#ifndef QPROGRESSCTRL_DEFAULT_TEXT_COLOR
	#define QPROGRESSCTRL_DEFAULT_TEXT_COLOR				RGB(120,120,120)
#endif

#ifndef QPROGRESSCTRL_DEFAULT_FG_COLOR
	#define QPROGRESSCTRL_DEFAULT_FG_COLOR					RGB(230,230,230)
#endif

#ifndef QPROGRESSCTRL_DEFAULT_BG_COLOR
	#define QPROGRESSCTRL_DEFAULT_BG_COLOR					RGB(246,246,246)
#endif

#ifndef GET_COLOR_R_CHANNEL
	#define GET_COLOR_R_CHANNEL(RGB)						(int)((RGB >> 16) & 0xFF)
#endif

#ifndef GET_COLOR_G_CHANNEL
	#define GET_COLOR_G_CHANNEL(RGB)						(int)((RGB >> 8) & 0xFF)
#endif

#ifndef GET_COLOR_B_CHANNEL
	#define GET_COLOR_B_CHANNEL(RGB)						(int)(RGB & 0xFF)
#endif



//////////////////////////////////////////////////////////////////////////////////////
// CQProgressCtrl

class COMMONUI_API CQProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CQProgressCtrl)
	
public:
	CQProgressCtrl();
	virtual ~CQProgressCtrl();
	
// class data members
private:
	int			m_nMinPos;
	int			m_nMaxPos;
	CString		m_strCaption;
	CString		m_strCaptionPercentChar;
	COLORREF	m_crTextColor;
	HBRUSH		m_hFgBrush;
	HBRUSH		m_hBgBrush;
	LOGFONT		m_MultiLangLogFont;  //(the new default log font to fully support the Multi-language.)
	
// class private functions
private:
	BOOL		SetupMuiltLangLogFont(CDC* pDC);
	
// class public methods and operations
public:
	int			GetMinPos(void);
	int			GetMaxPos(void);
	BOOL		SetMinMaxPos(int nMinPos, int nMaxPos);
	CString		GetCaption(void);
	void		SetCaption(LPCTSTR pszCaption);
	CString		GetCaptionPercentChar(void);
	void		SetCaptionPercentChar(LPCTSTR pszPercentChar);
	COLORREF	GetTextColor(void);
	void		SetTextColor(COLORREF crTextColor);
	HBRUSH		GetFgBrush(void);
	BOOL		SetFgBrush(HBRUSH hFgBrush);
	HBRUSH		GetBgBrush(void);
	BOOL		SetBgBrush(HBRUSH hBgBrush);
	
protected:	
// Windows override functions
	///////////////////////////////////////////
	////////////////// to-do //////////////////
	///////////////////////////////////////////	
	
// Windiows message handlers
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};

}