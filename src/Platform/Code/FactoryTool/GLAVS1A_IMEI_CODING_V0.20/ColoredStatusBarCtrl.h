// ColoredStatusBarCtrl.h: interface for the CColoredStatusBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLOREDSTATUSBARCTRL_H__872EDA01_CFCD_4A21_A022_4B2BF486FE5C__INCLUDED_)
#define AFX_COLOREDSTATUSBARCTRL_H__872EDA01_CFCD_4A21_A022_4B2BF486FE5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColoredStatusBarCtrl : public CStatusBar  
{
public:
	CColoredStatusBarCtrl();
	virtual ~CColoredStatusBarCtrl();
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

};

#endif // !defined(AFX_COLOREDSTATUSBARCTRL_H__872EDA01_CFCD_4A21_A022_4B2BF486FE5C__INCLUDED_)
