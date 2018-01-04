/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                               3G Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   WIN32IconButtonWidgeVer2.h  $
 *  $Revision:   1.26  $
 *  $Date:   Mar 21 2008 16:26:12  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h.-arc  $
 * 
 *    Rev 1.26   Mar 21 2008 16:26:12   Jame Tsai
 * update
 * 
 *    Rev 1.25   Mar 21 2008 14:42:42   Jame Tsai
 * refactory
 * 
 *    Rev 1.24   Mar 18 2008 17:44:50   Jame Tsai
 * update save function
 * 
 *    Rev 1.23   Mar 18 2008 15:52:06   Jame Tsai
 * update
 * 
 *    Rev 1.22   Mar 14 2008 14:58:32   Alan Tu
 * Add group function
 * 
 *    Rev 1.21   Mar 14 2008 11:37:04   Jame Tsai
 * update
 * 
 *    Rev 1.20   Feb 26 2008 22:14:40   Mei Yang
 * add _DoButton Save
 * 
 *    Rev 1.19   Feb 26 2008 21:10:36   Mei Yang
 * fix Property error
 * 
 *    Rev 1.18   Feb 26 2008 18:35:36   Mei Yang
 * fix GetAlignment
 * 
 *    Rev 1.17   Feb 26 2008 12:57:18   Mei Yang
 * Update Save
 * 
 *    Rev 1.16   Feb 25 2008 22:02:32   Mei Yang
 * update Save Parameter
 * 
 *    Rev 1.15   Jan 31 2008 18:31:38   Jame Tsai
 * fix error
 * 
 *    Rev 1.14   Jan 31 2008 14:44:10   Mei Yang
 * add INotify
 * 
 *    Rev 1.13   Jan 30 2008 14:30:20   Jame Tsai
 * add constructor for XML
 * 
 *    Rev 1.12   Jan 29 2008 18:41:18   Mei Yang
 * add Load
 * 
 *    Rev 1.11   Jan 24 2008 18:25:30   Mei Yang
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file WIN32IconButtonWidgeVer2.h
    @brief {brief description of WIN32IconButtonWidgeVer2.h}

    {detail description of WIN32IconButtonWidgeVer2.h}
*/
//It's a WIN32ButtonWidge which can show icon and text,and can be set toggle.
//WIN32IconButtonWidge完全是照抄TWin32WindowWidge的,只是多加了幾個member function
//不直接用TWin32WindowWidge套用樣板_DoIconButton是因為不希望在TWin32WindowWidge裡加入太多WIN32IconButtonWidge專用的member function
 
 
//目前Icon必須輸入兩張圖才能Work
//_DoIconButton有許多函式根本不會用到bErase參數,應該去掉
//可以考慮把BeToggleButten()拿掉,把ToggleButton寫死在Constructor裡面不給予切換,以免出現"只有四張圖"的ToggleButton
 
#ifndef _WIN32IconButtonWidge_Header_
#define _WIN32IconButtonWidge_Header_
#pragma once
#pragma warning(push)
#pragma warning(disable:4311)
 
#include "win32widge.h"
#include <stdlib.h>
#include <math.h>
#include "../../../CommonUtil/NotifyBase.h"

namespace QSYNC_LIB
{
	enum Arrangement{
		LeftIconRightText = 0, //Default
		LeftTextRightIcon = 1,
		TopIconBottomText = 2,
		TopTextBottomIcon = 3
	};
 
	class _DoIconButton : public _DoButton
	{
	public:
		_DoIconButton(Win32WindowBase* pControl,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc2 pFunc2,UINT nID)
			:_DoButton(pControl,strImagePath,strHighlightImagePath,pFunc2,nID),
			m_nTextEnableColor( Color::MakeARGB(255,0,0,0) ),m_nTextDisableColor( Color::MakeARGB(255,100,100,100) ),m_nFontSize(16),
			m_nTopMargin(0),m_nBottomMargin(0),m_nLeftMargin(0),m_nRightMargin(0),m_nSpacing(0),m_FontAlignment(VCenter | HCenter),
			m_pImage2(NULL),m_pHighlightImage2(NULL),m_pMouseDownImage2(NULL),m_pDisableImage2(NULL),
			m_pEnableIcon(NULL),m_pDisableIcon(NULL),m_Arrangement(LeftIconRightText),m_bToggleButton(false),m_bButtonCheck(false),
			m_bSetWidthHeightByImage(false), m_strClassName(_T("Win32IconButton")){}
		_DoIconButton(Win32WindowBase* pControl,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc pFunc)
			:_DoButton(pControl,strImagePath,strHighlightImagePath,pFunc),
			m_nTextEnableColor( Color::MakeARGB(255,0,0,0) ),m_nTextDisableColor( Color::MakeARGB(255,100,100,100) ),m_nFontSize(16),
			m_nTopMargin(0),m_nBottomMargin(0),m_nLeftMargin(0),m_nRightMargin(0),m_nSpacing(0),m_FontAlignment(VCenter | HCenter),
			m_pImage2(NULL),m_pHighlightImage2(NULL),m_pMouseDownImage2(NULL),m_pDisableImage2(NULL),
			m_pEnableIcon(NULL),m_pDisableIcon(NULL),m_Arrangement(LeftIconRightText),m_bToggleButton(false),m_bButtonCheck(false),
			m_bSetWidthHeightByImage(false), m_strClassName(_T("Win32IconButton")){}
		_DoIconButton(Win32WindowBase* pControl,const std::wstring& strImagePath,
			const std::wstring& strHighlightImagePath,const std::wstring& strMouseDownImagePath,
			const std::wstring& strDisableImagePath, LPButtonFunc pFunc,INotify *pINotify = NULL)
			:_DoButton(pControl,strImagePath,strHighlightImagePath,strMouseDownImagePath,strDisableImagePath,pFunc,pINotify),
			m_nTextEnableColor( Color::MakeARGB(255,0,0,0) ),m_nTextDisableColor( Color::MakeARGB(255,100,100,100) ),m_nFontSize(16),
			m_nTopMargin(0),m_nBottomMargin(0),m_nLeftMargin(0),m_nRightMargin(0),m_nSpacing(0),m_FontAlignment(VCenter | HCenter),
			m_pImage2(NULL),m_pHighlightImage2(NULL),m_pMouseDownImage2(NULL),m_pDisableImage2(NULL),
			m_pEnableIcon(NULL),m_pDisableIcon(NULL),m_Arrangement(LeftIconRightText),m_bToggleButton(false),m_bButtonCheck(false),
			m_bSetWidthHeightByImage(false), m_strClassName(_T("Win32IconButton")){}
		_DoIconButton(Win32WindowBase* pControl,const std::wstring& strImagePath,
			const std::wstring& strHighlightImagePath,const std::wstring& strMouseDownImagePath,
			const std::wstring& strDisableImagePath, LPButtonFunc2 pFunc2,UINT nID)
			:_DoButton(pControl,strImagePath,strHighlightImagePath,strMouseDownImagePath,strDisableImagePath,pFunc2,nID),
			m_nTextEnableColor( Color::MakeARGB(255,0,0,0) ),m_nTextDisableColor( Color::MakeARGB(255,100,100,100) ),m_nFontSize(16),
			m_nTopMargin(0),m_nBottomMargin(0),m_nLeftMargin(0),m_nRightMargin(0),m_nSpacing(0),m_FontAlignment(VCenter | HCenter),
			m_pImage2(NULL),m_pHighlightImage2(NULL),m_pMouseDownImage2(NULL),m_pDisableImage2(NULL),
			m_pEnableIcon(NULL),m_pDisableIcon(NULL),m_Arrangement(LeftIconRightText),m_bToggleButton(false),m_bButtonCheck(false),
			m_bSetWidthHeightByImage(false), m_strClassName(_T("Win32IconButton")){}

		//Constructor for XML
		_DoIconButton(Win32WindowBase* pControl, INotify* pINotify)
			:_DoButton(pControl,pINotify),
			m_nTextEnableColor( Color::MakeARGB(255,0,0,0) ),m_nTextDisableColor( Color::MakeARGB(255,100,100,100) ),m_nFontSize(16),
			m_nTopMargin(0),m_nBottomMargin(0),m_nLeftMargin(0),m_nRightMargin(0),m_nSpacing(0),m_FontAlignment(VCenter | HCenter),
			m_pImage2(NULL),m_pHighlightImage2(NULL),m_pMouseDownImage2(NULL),m_pDisableImage2(NULL),
			m_pEnableIcon(NULL),m_pDisableIcon(NULL),m_Arrangement(LeftIconRightText),m_bToggleButton(false),m_bButtonCheck(false),
			m_bSetWidthHeightByImage(false), m_strClassName(_T("Win32IconButton")){}

		virtual ~_DoIconButton(){}
		virtual void implementClose(Win32WindowBase* pControl)
		{
			if(m_pEnableIcon)
			{
				delete m_pEnableIcon;
				m_pEnableIcon=NULL;
			}
			if(m_pDisableIcon)
			{
				delete m_pDisableIcon;
				m_pDisableIcon=NULL;
			}
 
			if(m_pImage2)
			{
				delete m_pImage2;
				m_pImage2=NULL;
			}
			if(m_pHighlightImage2)
			{
				delete m_pHighlightImage2;
				m_pHighlightImage2=NULL;
			}
			if(m_pMouseDownImage2)
			{
				delete m_pMouseDownImage2;
				m_pMouseDownImage2=NULL;
			}
			if(m_pDisableImage2)
			{
				delete m_pDisableImage2;
				m_pDisableImage2=NULL;
			}
			_DoButton::implementClose(pControl);
		}
		virtual bool implementMouseUp(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY)
		{
			bool bRes = false;
			if(pControl && pControl->GetEnable())
			{
				if(m_bToggleButton && IsMouseEnter() && IsMouseDown())
					m_bButtonCheck = !m_bButtonCheck;
			}
			bRes = _DoButton::implementMouseUp(pControl,nFlags,nCX,nCY);
			return bRes;
		}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false)
		{
			if(pControl)
			{
				Graphics graphic(GC);
				GeometryRect rect = pControl->GetGeometry();
				Rect rcTotalArea( rect.m_nX, rect.m_nY, rect.m_nWidth, rect.m_nHeight );
				graphic.SetClip(rcTotalArea);
 
				DrawBkImage(
					pControl,
					graphic,
					rcTotalArea
					);
 
				Rect rcArea(
					rect.m_nX + m_nLeftMargin,
					rect.m_nY + m_nTopMargin,
					rect.m_nWidth - m_nLeftMargin - m_nRightMargin,
					rect.m_nHeight - m_nTopMargin - m_nBottomMargin	);
 
				//考慮加入字與圖都沒有的情況
				if(m_strEnableIcon.empty())
				{
					if(pControl->IsShow())
						DrawText(pControl,graphic,rcArea);
				}
				else
				{
					bool bEnable = pControl->GetEnable();
					if(pControl->GetDisplayString().empty()) //此時為有Icon沒文字
						DrawOnlyIcon(pControl,graphic,rcArea,bErase,bEnable);
					else //此時有圖也有文字
					{
						switch(m_Arrangement)
						{
						case LeftTextRightIcon:
							DrawLeftTextRightIcon(pControl,graphic,rcArea,bErase,bEnable);
							break;
						case TopIconBottomText:
							DrawTopIconBottomText(pControl,graphic,rcArea,bErase,bEnable);
							break;
						case TopTextBottomIcon:
							DrawTopTextBottomIcon(pControl,graphic,rcArea,bErase,bEnable);
							break;
						default:
							DrawLeftIconRightText(pControl,graphic,rcArea,bErase,bEnable);
							break;
						}
					}
 
				}
			}
		}
 
	public:
		//Save all UI setting to the file QLayoutManager.xml
		virtual bool Save(XMLMANAGER::XMLElement& Element, TSTRING& strID)
		{
			bool bRes = true;
			_DoButton::Save(Element, strID);
			Element.ReplaceAttr(g_strClsName, m_strClassName);
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.ReplaceAttr(g_strID, strID);

			XMLMANAGER::XMLElement* Property = &(Element.getAllElement()->at(0));
			Property->addAttribute(_T("TopMargin"), m_nTopMargin);
			Property->addAttribute(_T("BottomMargin"), m_nBottomMargin);
			Property->addAttribute(_T("LeftMargin"), m_nLeftMargin);
			Property->addAttribute(_T("RightMargin"), m_nRightMargin);
			Property->addAttribute(_T("Spacing"), m_nSpacing);
			Property->addAttribute(_T("FontSize"), (double)m_nFontSize);
			Property->addAttribute(_T("Alignment"), GetAlignment(m_FontAlignment));
			Color TextEnableColor(m_nTextEnableColor);
			Property->addAttribute(_T("TextEnableColorAlpha"), (int)TextEnableColor.GetAlpha());
			Property->addAttribute(_T("TextEnableColorR"), (int)TextEnableColor.GetR());
			Property->addAttribute(_T("TextEnableColorG"), (int)TextEnableColor.GetG());
			Property->addAttribute(_T("TextEnableColorB"), (int)TextEnableColor.GetB());
			Color TextDisableColor(m_nTextDisableColor);
			Property->addAttribute(_T("TextDisableColorAlpha"), (int)TextDisableColor.GetAlpha());
			Property->addAttribute(_T("TextDisableColorR"), (int)TextDisableColor.GetR());
			Property->addAttribute(_T("TextDisableColorG"), (int)TextDisableColor.GetG());
			Property->addAttribute(_T("TextDisableColorB"), (int)TextDisableColor.GetB());

			Property->addAttribute(_T("FontFamily"),m_strFontFamily);
			Property->addAttribute(_T("EnableIcon"), m_strEnableIcon);
			Property->addAttribute(_T("DisableIcon"), m_strDisableIcon);
			Property->addAttribute(_T("OrgImagePath2"),m_strOrgImagePath2);
			Property->addAttribute(_T("MouseOverImagePath2"),m_strMouseOverImagePath2);
			Property->addAttribute(_T("MouseDownImagePath2"),m_strMouseDownImagePath2);
			Property->addAttribute(_T("DisableImagePath2"),m_strDisableImagePath2);

			if(m_Arrangement == LeftIconRightText)
				Property->addAttribute(_T("Arragement"), _T("LeftIconRightText"));
			else if(m_Arrangement == LeftTextRightIcon)
				Property->addAttribute(_T("Arragement"), _T("LeftTextRightIcon"));
			else if(m_Arrangement == TopIconBottomText)
				Property->addAttribute(_T("Arragement"), _T("TopIconBottomText"));
			else if(m_Arrangement == TopTextBottomIcon)
				Property->addAttribute(_T("Arragement"), _T("TopTextBottomIcon"));

			Property->addAttribute(_T("ToggleButton"), m_bToggleButton?_T("true"):_T("false"));
			Property->addAttribute(_T("SetWidthHeightByImage"), m_bSetWidthHeightByImage?_T("true"):_T("false"));
			return bRes;
		}

		//Load all UI setting from the file QLayoutManager.xml
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = true;
			if(Element.name() == g_strProperty)
			{
				int nTopMargin = Element.getAttr(_T("TopMargin"));
				m_nTopMargin = nTopMargin==-1 ? 0 : nTopMargin;
				int nBottomMargin = Element.getAttr(_T("BottomMargin"));
				m_nBottomMargin = nBottomMargin==-1 ? 0 : nBottomMargin;
				int nLeftMargin = Element.getAttr(_T("LeftMargin"));
				m_nLeftMargin = nLeftMargin==-1 ? 0 : nLeftMargin;
				int nRightMargin = Element.getAttr(_T("RightMargin"));
				m_nRightMargin = nRightMargin==-1 ? 0 : nRightMargin;
				int nSpacing = Element.getAttr(_T("Spacing"));
				m_nSpacing = nSpacing==-1 ? 0 : nSpacing;
				TSTRING strFontSize;
				Element.getAttr(_T("FontSize"), strFontSize);
				if(!strFontSize.empty())
					SetFontSize((Gdiplus::REAL)_tstof(strFontSize.c_str()));
			
				TSTRING strAlignment;
				Element.getAttr(_T("Alignment"), strAlignment);
				if(strAlignment != _T(""))
				{
					vector<TSTRING> vstrAlignment;
					Tokenize<TSTRING>(_T("|"), strAlignment,vstrAlignment,false);
					m_FontAlignment = 0;
					for(vector<TSTRING>::iterator iter=vstrAlignment.begin(); iter!= vstrAlignment.end(); ++iter)
					{
						if((*iter) == _T("Left"))
							m_FontAlignment &= Left;
						else if((*iter) == _T("HCenter"))
							m_FontAlignment &= HCenter;
						else if((*iter) == _T("Right"))
							m_FontAlignment &= Right;
						else if((*iter) == _T("Top"))
							m_FontAlignment &= Top;
						else if((*iter) == _T("VCenter"))
							m_FontAlignment &= VCenter;
						else if((*iter) == _T("Bottom"))
							m_FontAlignment &= Bottom;
					}
				}
				
				int nAlpha = Element.getAttr(_T("TextEnableColorAlpha"));
				nAlpha = nAlpha == -1 ? 255 : nAlpha;
				int nR = Element.getAttr(_T("TextEnableColorR"));
				nR = nR == -1 ? 0 : nR;
				int nG = Element.getAttr(_T("TextEnableColorG"));
				nG = nG == -1 ? 0 : nG;
				int nB = Element.getAttr(_T("TextEnableColorB"));
				nB = nB == -1 ? 0 : nB;
				SetEnableTextColor(Color::MakeARGB(nAlpha,nR,nG,nB));
				nAlpha = Element.getAttr(_T("TextDisableColorAlpha"));
				nAlpha = nAlpha == -1 ? 255 : nAlpha;
				nR = Element.getAttr(_T("TextDisableColorR"));
				nR = nR == -1 ? 0 : nR;
				nG = Element.getAttr(_T("TextDisableColorG"));
				nG = nG == -1 ? 0 : nG;
				nB = Element.getAttr(_T("TextDisableColorB"));
				nB = nB == -1 ? 0 : nB;
				SetDisableTextColor(Color::MakeARGB(nAlpha,nR,nG,nB));
			
				Element.getAttr(_T("FontFamily"),m_strFontFamily);
				Element.getAttr(_T("EnableIcon"), m_strEnableIcon);
				Element.getAttr(_T("DisableIcon"), m_strDisableIcon);
				Element.getAttr(_T("OrgImagePath2"),m_strOrgImagePath2);
				Element.getAttr(_T("MouseOverImagePath2"),m_strMouseOverImagePath2);
				Element.getAttr(_T("MouseDownImagePath2"),m_strMouseDownImagePath2);
				Element.getAttr(_T("DisableImagePath2"),m_strDisableImagePath2);
				
				TSTRING strArragement;
				Element.getAttr(_T("Arragement"), strArragement);
				if(strArragement==_T("LeftIconRightText"))
					m_Arrangement = LeftIconRightText;
				else if(strArragement==_T("LeftTextRightIcon"))
					m_Arrangement = LeftTextRightIcon;
				else if(strArragement==_T("TopIconBottomText"))
					m_Arrangement = TopIconBottomText;
				else if(strArragement==_T("TopTextBottomIcon"))
					m_Arrangement = TopTextBottomIcon;
				else
					m_Arrangement = LeftIconRightText;

				TSTRING strToggleButton;
				Element.getAttr(_T("ToggleButton"), strToggleButton);
				m_bToggleButton = strToggleButton==_T("true")? true:false;

				TSTRING strSetWidthHeightByImage;
				Element.getAttr(_T("SetWidthHeightByImage"), strSetWidthHeightByImage);
				m_bSetWidthHeightByImage = strSetWidthHeightByImage==_T("true")? true:false;

				_DoButton::Load(Element);
			}
			return bRes;
		}
		virtual void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			m_nTopMargin	= nTopMargin	;
			m_nBottomMargin	= nBottomMargin	;
			m_nLeftMargin   = nLeftMargin   ;
			m_nRightMargin	= nRightMargin	;
		}
		virtual void SetSpacing(int nSpacing)
		{
			m_nSpacing = nSpacing;
		}
		void SetEnableIcon(const TSTRING& strEnableIconPath)
		{
			m_strEnableIcon = strEnableIconPath;
		}
		void SetDisableIcon(const TSTRING& strDisableIconPath)
		{
			m_strDisableIcon = strDisableIconPath;
		}
		void SetArrangement(Arrangement arrangement)
		{
			m_Arrangement = arrangement;
		}
 
		void SetEnableTextColor(ARGB nEnableTextColor)
		{
			m_nTextEnableColor = nEnableTextColor;
		}
		void SetDisableTextColor(ARGB nDisableTextColor)
		{
			m_nTextDisableColor = nDisableTextColor;
		}
		void SetFontFamily(TSTRING& strFontFamily)
		{
			m_strFontFamily = strFontFamily;
		}
		void SetFontSize(REAL nFontSize)
		{
				m_nFontSize = nFontSize;
		}
		void SetFontAlignment(Alignment FontAlignment)
		{
			m_FontAlignment = FontAlignment;
		}
 
		ARGB GetEnableTextColor() const	{return m_nTextEnableColor;}
		ARGB GetDisableTextColor() const {return m_nTextDisableColor;}
		
		//Set this button to be a toggle button.
		void BeToggleButten()
		{
			m_bToggleButton = true;
		}

		bool IsToggleButton() const
		{
			return m_bToggleButton;
		}

		//Is this button be pushed down.
		//We need this function only when this button is a toggle button.
		bool GetButtonCheck() const
		{
			return m_bButtonCheck;
		}
		//Set this button be pushed down or up.
		//We need this function only when this button is a toggle button.
		void SetButtonCheck(bool bButtonCheck)
		{
			if(m_bToggleButton)
				m_bButtonCheck = bButtonCheck;
		}
		void SetFourImagesOfButtonDown(
			const std::wstring& strOrgImagePath2,
			const std::wstring& strMouseOverImagePath2,
			const std::wstring& strMouseDownImagePath2,
			const std::wstring& strDisableImagePath2)
		{
			m_strOrgImagePath2 = strOrgImagePath2;
			m_strMouseOverImagePath2 = strMouseOverImagePath2;
			m_strMouseDownImagePath2 = strMouseDownImagePath2;
			m_strDisableImagePath2 = strDisableImagePath2;
		}
		void SetWidthHeightByImage(Win32WindowBase* pControl,bool bByImage)
		{
			m_bSetWidthHeightByImage = bByImage;
			if(m_bSetWidthHeightByImage && pControl)
			{
				pControl->SetMinWidth(GetImageWidth());
				pControl->SetMinHeight(GetImageHeight());
				pControl->SetMaxWidth(GetImageWidth());
				pControl->SetMaxHeight(GetImageHeight());
			}
		}
	private:
		virtual void DrawOnlyIcon(Win32WindowBase* pControl,Graphics& graphic, Rect& rect, bool bErase ,bool bEnable )
		{
			SetIconImage(pControl);
			Image* pIcon = bEnable ? m_pEnableIcon : m_pDisableIcon;
			int nIconWidth = pIcon->GetWidth();
			int nIconHeight = pIcon->GetHeight();
			int nStartPointX = rect.X + (rect.Width - nIconWidth)/2;
			int nStartPointY = rect.Y + (rect.Height - nIconHeight)/2;
			graphic.DrawImage(pIcon,nStartPointX,nStartPointY);
		}
		virtual void DrawLeftTextRightIcon(Win32WindowBase* pControl,Graphics& graphic, Rect& rect, bool bErase ,bool bEnable )
		{
			SetIconImage(pControl);
			Image* pIcon = bEnable ? m_pEnableIcon : m_pDisableIcon;
			int nIconWidth = pIcon->GetWidth();
			int nIconHeight = pIcon->GetHeight();
			int nIconStartPointX = rect.X + rect.Width - nIconWidth;
			int nIconStartPointY = rect.Y + (rect.Height - nIconHeight)/2;
			graphic.DrawImage(pIcon,nIconStartPointX,nIconStartPointY);
			Rect TextRect(rect.X,rect.Y,rect.Width-nIconWidth-m_nSpacing,rect.Height);
			DrawText(pControl,graphic, TextRect);
		}
		virtual void DrawTopIconBottomText(Win32WindowBase* pControl,Graphics& graphic, Rect& rect, bool bErase ,bool bEnable )
		{
			SetIconImage(pControl);
			Image* pIcon = bEnable ? m_pEnableIcon : m_pDisableIcon;
			int nIconWidth = pIcon->GetWidth();
			int nIconHeight = pIcon->GetHeight();
			int nIconStartPointX = rect.X + (rect.Width - nIconWidth)/2;
			int nIconStartPointY = rect.Y;
			graphic.DrawImage(pIcon,nIconStartPointX,nIconStartPointY);
			Rect TextRect(rect.X,nIconStartPointY+nIconHeight+m_nSpacing,rect.Width,rect.Height-nIconHeight-m_nSpacing);
			DrawText(pControl,graphic, TextRect);
		}
		virtual void DrawTopTextBottomIcon(Win32WindowBase* pControl,Graphics& graphic, Rect& rect, bool bErase ,bool bEnable )
		{
			SetIconImage(pControl);
			Image* pIcon = bEnable ? m_pEnableIcon : m_pDisableIcon;
			int nIconWidth = pIcon->GetWidth();
			int nIconHeight = pIcon->GetHeight();
			int nIconStartPointX = rect.X + (rect.Width - nIconWidth)/2;
			int nIconStartPointY = rect.Y + rect.Height - nIconHeight;
			graphic.DrawImage(pIcon,nIconStartPointX,nIconStartPointY);
			Rect TextRect(rect.X,rect.Y,rect.Width,rect.Height-nIconHeight-m_nSpacing);
			DrawText(pControl,graphic, TextRect);
		}
		virtual void DrawLeftIconRightText(Win32WindowBase* pControl,Graphics& graphic, Rect& rect, bool bErase ,bool bEnable )
		{
			SetIconImage(pControl);
			Image* pIcon = bEnable ? m_pEnableIcon : m_pDisableIcon;
			int nIconWidth = pIcon->GetWidth();
			int nIconHeight = pIcon->GetHeight();
			int nIconStartPointX = rect.X ;
			int nIconStartPointY = rect.Y + (rect.Height - nIconHeight)/2;
			graphic.DrawImage(pIcon,nIconStartPointX,nIconStartPointY);
			Rect TextRect(nIconStartPointX+nIconWidth+m_nSpacing,rect.Y,rect.Width-nIconWidth-m_nSpacing,rect.Height);
			DrawText(pControl,graphic, TextRect);
		}
 
		virtual void SetIconImage(Win32WindowBase* pControl)
		{
			if(m_pEnableIcon != NULL)
			{
				delete m_pEnableIcon;
				m_pEnableIcon = NULL;
			}
			m_pEnableIcon = new Image(pControl->GetPictureFolder(m_strEnableIcon).c_str());
			if(m_pDisableIcon != NULL)
			{
				delete m_pDisableIcon;
				m_pDisableIcon = NULL;
			}
			m_pDisableIcon = new Image(pControl->GetPictureFolder(m_strDisableIcon).c_str());
		}
		virtual void DrawText(Win32WindowBase* pControl,Graphics& graphics,Rect& rect)
		{
			// Initialize arguments.
			bool bEnable = pControl->GetEnable();
			WCHAR* pFontFamily = m_strFontFamily.empty() ? L"Arial" : m_strFontFamily.c_str();
 
			Font TextFont(pFontFamily,m_nFontSize);
			StringFormat format;
 
			if(m_FontAlignment & Left)
				format.SetAlignment(StringAlignmentNear);
			else if(m_FontAlignment & Right)
				format.SetAlignment(StringAlignmentFar);
			else
				format.SetAlignment(StringAlignmentCenter);
			if(m_FontAlignment & Top)
				format.SetLineAlignment(StringAlignmentNear);
			else if(m_FontAlignment & Bottom)
				format.SetLineAlignment(StringAlignmentFar);
			else
				format.SetLineAlignment(StringAlignmentCenter);
 
			Color TextColor(bEnable ? m_nTextEnableColor : m_nTextDisableColor);
			SolidBrush TextBrush(TextColor);
 
			std::wstring		strTemp;
			ILocalization*	pILocalization = GetILocalization();
			pILocalization->GetString(pControl->GetDisplayString(),strTemp);
 
			RectF layoutRect((float)rect.X,(float)rect.Y,(float)rect.Width,(float)rect.Height);
 
			// Draw string.
			graphics.DrawString(
				strTemp.c_str(),
				static_cast<int>(strTemp.size()),
				&TextFont,
				layoutRect,
				&format,
				&TextBrush);
		}
		virtual void DrawBkImage(Win32WindowBase* pControl,Graphics& graphics,Rect& rcArea)
		{
			if(!IsToggleButton())
				_DoButton::DrawBkImage(pControl,graphics,rcArea);
			else if(IsToggleButton() && (!GetButtonCheck()))
				_DoButton::DrawBkImage(pControl,graphics,rcArea);
			else if(IsToggleButton() && GetButtonCheck()) //用ButtonCheck四張圖來畫
			{
				SetButtonCheckImage(pControl);
				Image* pImage = m_pImage2;
				if( pControl->GetEnable() && IsMouseEnter())
				{
					if(!IsMouseDown() && m_pHighlightImage2)//按鈕啟動,滑鼠在按鈕內且沒有按下
					{
						pImage = m_pHighlightImage2;
					}
					else if (IsMouseDown() && m_pMouseDownImage2)//按鈕啟動,滑鼠在按鈕內按下
					{
						pImage = m_pMouseDownImage2;
					}
				}
				else if ( pControl->GetEnable() )//按鈕啟動,滑鼠不在按鈕內
				{
					if(m_pImage2)
						pImage = m_pImage2;
				}
				else if(!pControl->GetEnable())//按鈕沒啟動
				{
					if(m_pDisableImage2)
						pImage = m_pDisableImage2;
				}
				graphics.DrawImage(pImage,rcArea);
			}
		}
		virtual void SetButtonCheckImage(Win32WindowBase* pControl)
		{
			if(m_pImage2 == NULL && m_strOrgImagePath2.size() > 0)
				m_pImage2 = new Image(pControl->GetPictureFolder(m_strOrgImagePath2).c_str());
			if(m_pHighlightImage2 == NULL && m_strMouseOverImagePath2.size() > 0)
				m_pHighlightImage2 = new Image(pControl->GetPictureFolder(m_strMouseOverImagePath2).c_str());
			if(m_pMouseDownImage2 == NULL && m_strMouseDownImagePath2.size() >0)
				m_pMouseDownImage2 = new Image(pControl->GetPictureFolder(m_strMouseDownImagePath2).c_str());
			if(m_pDisableImage2 == NULL && m_strDisableImagePath2.size() >0)
				m_pDisableImage2 = new Image(pControl->GetPictureFolder(m_strDisableImagePath2).c_str());
		}
		int GetID()
		{
			static int nID=-1;
			nID++;
			return nID;
		}
		TSTRING GetAlignment(int nAlignment)
		{
			TSTRING strAlignment;
			if(Default == nAlignment)
				strAlignment = _T("Left|Top");
			else
			{
				if(nAlignment & Left)
					strAlignment = _T("Left");
				else if(nAlignment & HCenter)
					strAlignment = _T("HCenter");
				else if(nAlignment & Right)
					strAlignment = _T("Right");
				if(nAlignment & Top)
					strAlignment = StrConnect<TSTRING>(_T("|"),strAlignment, _T("Top"), false);
				else if(nAlignment & VCenter)
					strAlignment = StrConnect<TSTRING>(_T("|"), strAlignment, _T("VCenter"), false);
				else if(nAlignment & Bottom)
					strAlignment = StrConnect<TSTRING>(_T("|"), strAlignment, _T("Bottom"), false);
			}
			return strAlignment;
		}
	private:
		TSTRING m_strClassName;
		int m_nTopMargin;
		int m_nBottomMargin;
		int m_nLeftMargin;
		int m_nRightMargin;
		int m_nSpacing;
 
		REAL m_nFontSize;
		int m_FontAlignment;
		ARGB m_nTextEnableColor;
		ARGB m_nTextDisableColor;
		TSTRING m_strFontFamily;
 
 
		Image* m_pEnableIcon;
		Image* m_pDisableIcon;
 
		TSTRING m_strEnableIcon;
		TSTRING m_strDisableIcon;
 
		TSTRING m_strOrgImagePath2;
		TSTRING m_strMouseOverImagePath2;
		TSTRING m_strMouseDownImagePath2;
		TSTRING m_strDisableImagePath2;
 
		Image*										m_pImage2;
		Image*										m_pHighlightImage2;
		Image*										m_pMouseDownImage2;
		Image*										m_pDisableImage2;
 
		Arrangement m_Arrangement;
 
		bool    m_bToggleButton;
		bool	m_bButtonCheck;
		bool	m_bSetWidthHeightByImage;
	};
 
	
	template <class GeneralPolicy,class ActionPolicy = _DoIconButton>
	class WIN32IconButtonWidge : public Win32WindowBase , public INotify
	{
		class ToggleEngine : public CINotifyBase
		{
		public:
			ToggleEngine()
			{
			}
			virtual ~ToggleEngine()
			{
			}
			virtual void Event(const TSTRING& strEvent,long nParam){}			
			virtual void DefineNotify(const TSTRING& strNewEvent)
			{
				CINotifyBase::DefineNotify(strNewEvent);
			}
			virtual void Fire(const TSTRING& strEvent,long nParam = 0)
			{
				CINotifyBase::Fire(strEvent,nParam);
			}
		};
		static ToggleEngine& GetToggleEngine()
		{
			static ToggleEngine m_ToggleEngine;
			return m_ToggleEngine;
		}		
	public:
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			long nThis = (long)this;
			SetButtonCheck(nParam == nThis);
		}
	public:
		//For WIN32ButtonWidge overwrite
		WIN32IconButtonWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc2 pFunc2,UINT nID)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
 
			if(m_pAction == NULL)
				m_pAction  = new ActionPolicy(this,strImagePath,strHighlightImagePath,pFunc2,nID);
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		WIN32IconButtonWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc pFunc)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
 
			if(m_pAction == NULL)
				m_pAction  = new ActionPolicy(this,strImagePath,strHighlightImagePath,pFunc);
 
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With four images
		WIN32IconButtonWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strOrgImagePath,const std::wstring& strMouseOverImagePath,
			const std::wstring& strMouseDownImagePath,const std::wstring& strDisableImagePath,LPButtonFunc pFunc)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
 
			if(m_pAction == NULL)
				m_pAction  = new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,pFunc);
 
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With four images
		WIN32IconButtonWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strOrgImagePath,const std::wstring& strMouseOverImagePath,
			const std::wstring& strMouseDownImagePath,const std::wstring& strDisableImagePath,LPButtonFunc2 pFunc2,UINT nID)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
 
			if(m_pAction == NULL)
				m_pAction  = new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,pFunc2,nID);
 
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With Default four images
		WIN32IconButtonWidge(CWnd* pWindow,LPButtonFunc pFunc,const std::wstring& strDisplay)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			std::wstring strOrgImagePath = L"General\\Button\\Btn_Original.png";
			std::wstring strMouseOverImagePath = L"General\\Button\\Btn_MouseOver.png";
			std::wstring strMouseDownImagePath = L"General\\Button\\Btn_MouseDown.png";
			std::wstring strDisableImagePath = L"General\\Button\\Btn_Disable.png";
 
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
 
			if(m_pAction == NULL)
				m_pAction  = new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,pFunc);
 
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With Default four images
		WIN32IconButtonWidge(CWnd* pWindow,INotify* pINotify,const std::wstring& strDisplay)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			std::wstring strOrgImagePath = L"General\\Button\\Btn_Original.png";
			std::wstring strMouseOverImagePath = L"General\\Button\\Btn_MouseOver.png";
			std::wstring strMouseDownImagePath = L"General\\Button\\Btn_MouseDown.png";
			std::wstring strDisableImagePath = L"General\\Button\\Btn_Disable.png";
 
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
 
			if(m_pAction == NULL)
				m_pAction  = new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,NULL,pINotify);
 
			SetDisplayString(strDisplay);
		}
		WIN32IconButtonWidge(CWnd* pWindow, INotify* pINotify):m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_nDlgItem(0)
		{
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
			m_pAction  = new ActionPolicy(this,pINotify);
			//_DoNothing
		}
 
		virtual ~WIN32IconButtonWidge()
		{
			if(!m_strGroup.empty())
				GetToggleEngine().Unregister(this,m_strGroup);

			if(m_pAction)
			{
				delete m_pAction;
				m_pAction = NULL;
			}
		}
	public:
		virtual bool Save(XMLMANAGER::XMLElement& MainElement, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase)
		{
			using namespace XMLMANAGER;
			bool bRes = false;
			XMLElement EleWidget(g_strWidget);
			XMLElement Property(g_strProperty);
			EleWidget.getAllElement()->push_back(Property);

			if(m_pAction)
			{
				TSTRING strID;
				bRes = m_pAction->Save(EleWidget, strID);
				XMLMANAGER::XMLElement* pProperty = &(EleWidget.getAllElement()->at(0));
				if(bRes)
					pProperty->addAttribute(_T("Group"), m_strGroup);

				if(strID!=_T(""))
				{
					FillOrderSetting(EleOrder, strID);
					bRes = true;
				}
			}
			Win32WindowBase::Save(EleWidget);
			MainElement.getAllElement()->push_back(EleWidget);
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = true;
			if(Element.name() == g_strProperty)
			{
				TSTRING strGroup;
				Element.getAttr(_T("Group"), strGroup);
				SetGroup(strGroup);

				if(m_pAction)
					bRes = m_pAction->Load(Element) && Win32WindowBase::Load(Element);
			}
			return bRes;
		}
		virtual void CaptureMouse(bool bCapture)
		{
			if(m_hWnd)
			{
				if(bCapture)
				{
					if(m_bCaptured == false)
					{
						::SetCapture(m_hWnd);
						m_bCaptured = true;
					}
				}
				else
				{
					if(m_bCaptured == true)
					{
						::ReleaseCapture();
						m_bCaptured = false;
					}
				}
			}
		}
		virtual void Focus(bool bFocus)
		{
			if(m_pAction && IsShow())
				m_pAction->implementFocus(this,bFocus);
		}
		virtual void MouseUp(int nCX,int nCY)
		{
			bool bRes = false;
			if(m_pAction && IsShow())
				bRes = m_pAction->implementMouseUp(this,0,nCX,nCY);			
			if(bRes && !m_strGroup.empty())
				GetToggleEngine().Fire(m_strGroup,(long)this);
		}
		virtual void MouseDown(int nCX,int nCY)
		{
			if(m_pAction && IsShow())
				m_pAction->implementMouseDown(this,0,nCX,nCY);
		}
		virtual void MouseDBClk(int nCX,int nCY)
		{
			bool bRes = false;
			if(m_pAction && IsShow() && IsVaild(nCX,nCY))
				bRes = m_pAction->implementMouseDoubleClick(this,0,nCX,nCY);
			if(bRes && !m_strGroup.empty())
				GetToggleEngine().Fire(m_strGroup,(long)this);
		}
		virtual void MouseMove(int nCX,int nCY)
		{
			if(m_pAction && IsShow())
				m_pAction->implementMouseMove(this,0,nCX,nCY);
		}
		virtual void Draw(HDC GC,bool bErase = false)
		{
			if(m_pAction && IsShow())
				m_pAction->implementDraw(this,GC,bErase);
		}
		virtual void Close()
		{
			if(m_pAction)
				m_pAction->implementClose(this);
		}
		virtual void Update(bool bErase = false)
		{
			GeneralPolicy::Update(m_hWnd,GetGeometry(),bErase);
		}
		virtual bool SetLayout(Widge* pIWidge = 0)
		{
			if(GeneralPolicy::Move(this,m_hWnd))
				Update(false);
			return true;
		}
 
		virtual void Show(bool bShow,bool bErase = false)
		{
			GeneralPolicy::Show(this,m_hWnd,bShow,bErase);
		}
	protected:
		bool IsVaild(int nCX,int nCY)
		{
			return m_bCaptured || InArea(nCX,nCY);
		}
		bool InArea(int nCX,int nCY)
		{
			GeometryRect rect = GetGeometry();
			Rect rc(rect.m_nX,rect.m_nY,rect.m_nWidth,rect.m_nHeight);
			return rc.Contains(nCX,nCY) == TRUE;
		}



	public:
		virtual void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			if(m_pAction)
				m_pAction->SetMargin( nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
		}
		virtual void SetSpacing(int nSpacing)
		{
			if(m_pAction)
				m_pAction->SetSpacing(nSpacing);
		}
		void SetIcon(const TSTRING& strEnableIconPath,const TSTRING& strDisableIconPath,Arrangement arrangement)
		{
			SetEnableIcon(strEnableIconPath);
			SetDisableIcon(strDisableIconPath);
			SetArrangement(arrangement);
		}
		void SetIcon(const TSTRING& strEnableIconPath,const TSTRING& strDisableIconPath)
		{
			SetEnableIcon(strEnableIconPath);
			SetDisableIcon(strDisableIconPath);
		}
		void SetEnableIcon(const TSTRING& strEnableIconPath)
		{
			if(m_pAction)
				m_pAction->SetEnableIcon(strEnableIconPath);
		}
		void SetDisableIcon(const TSTRING& strDisableIconPath)
		{
			if(m_pAction)
				m_pAction->SetDisableIcon(strDisableIconPath);
		}
		void SetArrangement(Arrangement arrangement)
		{
			if(m_pAction)
				m_pAction->SetArrangement(arrangement);
		}
		void SetTextColor(
			int nEnableAlpha,int nEnableRed,int nEnableGreen,int nEnableBlue,
			int nDisableAlpha,int nDisableRed,int nDisableGreen,int nDisableBlue)
		{
			SetEnableTextColor(nEnableAlpha, nEnableRed, nEnableGreen, nEnableBlue);
			SetDisableTextColor(nDisableAlpha, nDisableRed, nDisableGreen, nDisableBlue);
		}
		void SetEnableTextColor(int nEnableAlpha,int nEnableRed,int nEnableGreen,int nEnableBlue)
		{
			if(m_pAction)
			{
				ARGB nEnableTextColor = Color::MakeARGB(nEnableAlpha, nEnableRed, nEnableGreen, nEnableBlue);
				m_pAction->SetEnableTextColor(nEnableTextColor);
			}
		}
		void SetDisableTextColor(int nDisableAlpha,int nDisableRed,int nDisableGreen,int nDisableBlue)
		{
			if(m_pAction)
			{
				ARGB nDisableTextColor = Color::MakeARGB(nDisableAlpha, nDisableRed, nDisableGreen, nDisableBlue);
				m_pAction->SetDisableTextColor(nDisableTextColor);
			}
		}
		void SetFontFamily(TSTRING& strFontFamily)
		{
			if(m_pAction)
				m_pAction->SetFontFamily(strFontFamily);
		}
		void SetFontSize(int nFontSize)
		{
			if(m_pAction)
				m_pAction->SetFontSize((Gdiplus::REAL)nFontSize);
		}
		void SetFontAlignment(Alignment FontAlignment)
		{
			if(m_pAction)
				m_pAction->SetFontAlignment(FontAlignment);
		}
		ARGB GetEnableTextColor() const	{return m_pAction->GetEnableTextColor();}
		ARGB GetDisableTextColor() const {return m_pAction->GetDisableTextColor();}
		void BeToggleButten()
		{
			if(m_pAction)
				m_pAction->BeToggleButten();
		}
		bool GetButtonCheck() const
		{
			bool bRes = false;
			if(m_pAction)
				bRes = m_pAction->GetButtonCheck();
			return bRes;
		}
		void SetButtonCheck(bool bButtonCheck)
		{
			if(m_pAction)
			{
				m_pAction->SetButtonCheck(bButtonCheck);
				Update();
			}
		}
		void SetFourImagesOfButtonDown(
			const std::wstring& strOrgImagePath,
			const std::wstring& strMouseOverImagePath,
			const std::wstring& strMouseDownImagePath,
			const std::wstring& strDisableImagePath,
			const std::wstring strGroup = L"")
		{			
			SetGroup(strGroup);
			if(m_pAction)
				m_pAction->SetFourImagesOfButtonDown(strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath,strDisableImagePath);
		}
 
		const TSTRING& GetGroup() const
		{
			return m_strGroup;
		}


		void SetWidthHeightByImage(bool bByImage = true)
		{
			if(m_pAction)
				m_pAction->SetWidthHeightByImage(this,bByImage);
		}
	private:
		void SetGroup(const TSTRING& strGroup)
		{
			//呼叫此函式時,需注意是否有設為ToggleButton
			if(!strGroup.empty())
			{
				m_strGroup = strGroup;
				GetToggleEngine().DefineNotify(m_strGroup);
				GetToggleEngine().Register(this,m_strGroup);
			}
		}
		void FillOrderSetting(XMLMANAGER::XMLElement& EleOrder, const TSTRING& strID)
		{
			using namespace XMLMANAGER;
			for(XMLAllElement::iterator iter = EleOrder.getAllElement()->begin(); iter!= EleOrder.getAllElement()->end(); ++iter)
			{
				TSTRING strAttrValue;
				iter->getAttr(_T("ID"), strAttrValue);
				if(strAttrValue == _T(""))
					iter->ReplaceAttr(_T("ID"),strID);
			}
		}
	private:
		HWND											m_hWnd;
		ActionPolicy*									m_pAction;
		bool												m_bCaptured;
		int												m_nDlgItem;
	
		//設定ButtonGroup,此字串非空的話,所有此字串相同的Button為同一個group
		//當一個Group裡某個Button被按下時,其他Button會跳起來
		TSTRING									m_strGroup;
	};
	typedef WIN32IconButtonWidge<ButtonPolicy,_DoIconButton>	WIN32IconButton;
}
 
#pragma warning(pop)
#endif
