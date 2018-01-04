/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                               3G Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   Win32Widge.h  $
 *  $Revision:   1.5  $
 *  $Date:   Apr 01 2009 14:17:26  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Components/UIFramework/LayoutManager/Win32Widge.h.-arc  $
 * 
 *    Rev 1.5   Apr 01 2009 14:17:26   Kevin Tai
 * no change
 * 
 *    Rev 1.4   Aug 02 2008 01:37:02   Alan Tu
 * update
 * 
 *    Rev 1.3   Jul 01 2008 10:41:54   Jame Tsai
 * Update button color
 * 
 *    Rev 1.108   Jul 01 2008 10:18:22   Jame Tsai
 * Update button color
 * 
 *    Rev 1.107   Jun 23 2008 16:49:38   Jame Tsai
 * Add warning message
 * 
 *    Rev 1.106   Jun 18 2008 17:14:28   Jame Tsai
 * Add function
 * 
 *    Rev 1.104   Apr 17 2008 14:45:56   Alan Tu
 * add remove track function
 * 
 *    Rev 1.103   Apr 17 2008 14:16:34   Alan Tu
 * add track function
 * 
 *    Rev 1.102   Apr 16 2008 18:21:10   Alan Tu
 * add viewer widget
 * 
 *    Rev 1.101   Apr 02 2008 16:04:26   Jame Tsai
 * fix z-order error
 * 
 *    Rev 1.100   Mar 14 2008 14:58:32   Alan Tu
 * Add group function
 * 
 *    Rev 1.99   Mar 14 2008 09:38:56   Jame Tsai
 * update
 * 
 *    Rev 1.98   Mar 13 2008 16:15:12   Jame Tsai
 * update
 * 
 *    Rev 1.97   Mar 11 2008 16:01:42   Alan Tu
 * Add application widget
 * 
 *    Rev 1.96   Mar 07 2008 11:37:38   Alan Tu
 * remove remark
 * 
 *    Rev 1.95   Feb 27 2008 12:34:02   Mei Yang
 * fix GetnID
 * 
 *    Rev 1.94   Feb 26 2008 23:39:32   Mei Yang
 * add GetnID, remove _DoButton's m_nID, and remove TWin32WindowWidge(CWnd* pWindow, int nDlgItem)
 * 
 *    Rev 1.93   Feb 26 2008 22:25:32   Mei Yang
 * replace m_nDlgItem 0->-1
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Win32Widge.h
    @brief {brief description of Win32Widge.h}

    {detail description of Win32Widge.h}
*/
#ifndef WIN32Widge_header_
#define WIN32Widge_header_
 
#include <string>
#include "TWidge.h"
#include "../../../CommonUtil/CommonDrawer.h"
#include "..\..\Localization\ilocalization.h"
#include "../../../CommonUtil/XMLManager.h"
#include "../../../CommonUtil/UnicodeUtility.h"
#include "LayoutBase.h"

#pragma once
 
namespace QSYNC_LIB
{ 
	/**********************************************************************************************************************/
	/*		TWidge for win32 class																																*/
	/*																																												*/
	/*																																												*/
	/*																																												*/
	/*																																												*/
	/**********************************************************************************************************************/
#ifdef WIN32
 
enum ShowStyle
{
	Stretch = 0, //Default
	Tile = 1,
	WFixed = 2,
	HFixed = 4,
	Fixed = 6
};
enum ProgressBarStyle
{
	ClipPicture = 0, //Default
	AnimatePicture = 1 
};

	class Win32WindowBase : public Widge<HDC>
	{
	public:
		Win32WindowBase():m_bEnable(true){}
		virtual ~Win32WindowBase(){}
		virtual bool IsLayoutManager(){return false;}
		virtual bool IsWindow(){return true;}
		virtual bool Save(XMLMANAGER::XMLElement& Element, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase){return true;}
		bool Save(XMLMANAGER::XMLElement& EleWidget)
		{
			bool bRes = true;
			XMLMANAGER::XMLElement* pProperty = &(EleWidget.getAllElement()->at(0));
			pProperty->addAttribute(_T("Enable"), m_bEnable ? _T("true") : _T("false"));
			pProperty->addAttribute(g_strDisplay, GetTSTRING(m_strDisplay));
			pProperty->addAttribute(_T("nID"), GetnID());
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = true;
			if(Element.name() == g_strProperty)
			{
				TSTRING strDisplay;
				Element.getAttr(g_strDisplay, strDisplay);
				m_strDisplay = QSYNC_LIB::GetUnicode(strDisplay);
				TSTRING strEnable;
				Element.getAttr(_T("Enable"), strEnable);
				m_bEnable = strEnable == _T("false") ? false: true;  //Default true
				int nID = Element.getAttr((_T("nID")));
				if(nID != -1)
					SetID(nID);
			}
			return bRes;
		}
		const std::wstring& GetDisplayString() const{ return m_strDisplay;}
		void SetDisplayString(const std::wstring& strDisplay)
		{
			if(m_strDisplay != strDisplay)
			{
				m_strDisplay = strDisplay;
				Update(false);
			}
		}
		bool GetEnable(){return m_bEnable;}
		void SetEnable(bool bSet)
		{
			if(m_bEnable != bSet)
			{
				m_bEnable = bSet;
				Update(false);
			}
		}
	private:
		int GetnID()
		{
			int nRes = Widge::GetID();
			static int nID =0;
			++nID;
			if(nRes == 0)
				nRes = nID;
			return nRes;
		}
	private:
		std::wstring				m_strDisplay;
		bool						m_bEnable;
	};
 
	class _DoNothing
	{
	public:
		_DoNothing():m_strClassName(_T("Win32Windows")){}
		_DoNothing(Win32WindowBase* pControl, INotify* pINotify):m_strClassName(_T("Win32Windows")){}
		virtual ~_DoNothing(){}
		virtual void implementMouseDown(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseUp(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementMouseMove(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseDoubleClick(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementFocus(Win32WindowBase* pControl,bool bFocused){}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false){}
		virtual void implementClose(Win32WindowBase* pControl){}
		virtual bool Save(XMLMANAGER::XMLElement& Element, TSTRING& strID)
		{
			bool bRes = true;
			Element.addAttribute(g_strClsName, m_strClassName);
			iToTStr(GetnID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element){return true;}
	private:
		int GetnID()
		{
			static int nID=-1;
			nID++;
			return nID;
		}
	private:
		TSTRING m_strClassName;
	};
 
	class _DoDrawer
	{
	public:
		_DoDrawer(Win32WindowBase* pControl,const std::wstring& strImagePath,ShowStyle style)
			:m_strImagePath(strImagePath),m_pImage(NULL),m_ShowStyle(style),m_pControl(pControl),m_strClassName(_T("Win32Image"))
		{
			Initiate();
		}
		//Constructor for XML
		_DoDrawer(Win32WindowBase* pControl, INotify* pINotify):m_pImage(NULL),m_ShowStyle(Stretch),m_pControl(pControl),m_strClassName(_T("Win32Image")){}
		virtual ~_DoDrawer(){}
		virtual void implementMouseDown(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseUp(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementMouseMove(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseDoubleClick(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementFocus(Win32WindowBase* pControl,bool bFocused){}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false)
		{
			GeometryRect rect = pControl->GetGeometry();
			Graphics graphics(GC);

			Rect rcArea(rect.m_nX,rect.m_nY, rect.m_nWidth,rect.m_nHeight);
			graphics.SetClip(rcArea);

			switch (m_ShowStyle)
			{
			case Tile:
				if(pControl && m_pImage)
				{

					UINT imageW = m_pImage->GetWidth();
					UINT imageH = m_pImage->GetHeight();

					int nX = rect.m_nX;
					int nY = rect.m_nY;
					
					while(nY <= (rect.m_nY + rect.m_nHeight) )
					{
						while(nX <= (rect.m_nX+rect.m_nWidth) )
						{
							graphics.DrawImage(m_pImage,nX,nY,imageW, imageH);
							nX += imageW;
						};
						nX = rect.m_nX;
						nY += imageH;
					}
				}
				break;
			//理論上其實下面WFixed,HFixed,Fixed三個Case都可以不寫,跟Default跑出來應該要一樣
            //但保險起見,保留設定
			case WFixed:
				if(pControl && m_pImage)
				{
					int nX = pControl->GetGeometry().m_nX;
					int nY = pControl->GetGeometry().m_nY;
					UINT nImageW = m_pImage->GetWidth();
					int nWidgetH = pControl->GetGeometry().m_nHeight;
					graphics.DrawImage(m_pImage,nX,nY,nImageW, nWidgetH);
				}
				break;
			case HFixed:
				if(pControl && m_pImage)
				{
					int nX = pControl->GetGeometry().m_nX;
					int nY = pControl->GetGeometry().m_nY;
					int nWidgetW = pControl->GetGeometry().m_nWidth;
					UINT nImageH = m_pImage->GetHeight();
					graphics.DrawImage(m_pImage,nX,nY,nWidgetW, nImageH);
				}
				break;
			case Fixed:
				if(pControl && m_pImage)
				{
					UINT nImageW = m_pImage->GetWidth();
					UINT nImageH = m_pImage->GetHeight();
					int nX = pControl->GetGeometry().m_nX;
					int nY = pControl->GetGeometry().m_nY;
					graphics.DrawImage(m_pImage,nX,nY,nImageW, nImageH);
				}
				break;
			default: //	Stretch
				if(pControl && m_pImage)
				{
					GeometryRect rect = pControl->GetGeometry();
					graphics.DrawImage(m_pImage,rect.m_nX,rect.m_nY,rect.m_nWidth, rect.m_nHeight);
				}
				break;
			}
		}
		virtual void implementClose(Win32WindowBase* pControl)
		{
			if(m_pImage != NULL)
			{
				delete m_pImage;
				m_pImage = NULL;
			}
		}
		virtual bool Save(XMLMANAGER::XMLElement& Element, TSTRING& strID)
		{
			bool bRes = true;

			Element.addAttribute(g_strClsName, m_strClassName);
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);
			
			XMLMANAGER::XMLElement* Property = &(Element.getAllElement()->at(0));
			Property->addAttribute(_T("ImagePath"), m_strImagePath);
			if(m_ShowStyle == Stretch)
				Property->addAttribute(_T("ShowStyle"), _T("Stretch"));
			else if(m_ShowStyle == Tile)
				Property->addAttribute(_T("ShowStyle"), _T("Tile"));
			else if(m_ShowStyle == WFixed)
				Property->addAttribute(_T("ShowStyle"), _T("WFixed"));
			else if(m_ShowStyle == HFixed)
				Property->addAttribute(_T("ShowStyle"), _T("HFixed"));
			else if(m_ShowStyle == Fixed)
				Property->addAttribute(_T("ShowStyle"), _T("Fixed"));
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = true;
			if(Element.name() == g_strProperty)
			{			
				bRes = Element.getAttr(_T("ImagePath"),m_strImagePath);
				TSTRING strShowStyle;
				Element.getAttr(_T("ShowStyle"), strShowStyle);
				if(strShowStyle == _T("Stretch"))
					m_ShowStyle = Stretch;
				else if(strShowStyle == _T("Tile"))
					m_ShowStyle = Tile;
				else if(strShowStyle == _T("WFixed"))
					m_ShowStyle = WFixed;
				else if(strShowStyle == _T("HFixed"))
					m_ShowStyle = HFixed;
				else if(strShowStyle == _T("Fixed"))
					m_ShowStyle = Fixed;
				else
					m_ShowStyle = Stretch;	
				Initiate();
			}
			return bRes;
		}
		virtual void implementSetImagePath(const std::wstring& strImagePath)
		{			
			if(m_strImagePath != strImagePath)
			{
				m_strImagePath = strImagePath;
				if(m_pImage)
				{
					delete m_pImage;
					m_pImage = new Image(m_pControl->GetPictureFolder(m_strImagePath).c_str());
				}
			}
		}
	private:
		virtual void Initiate()
		{
			if(m_pImage == NULL && m_strImagePath.size() > 0)
			{
				m_pImage = new Image(m_pControl->GetPictureFolder(m_strImagePath).c_str());
			}
			if( (m_pImage != NULL)&&(m_ShowStyle & WFixed) )
			{
				UINT nImageW = m_pImage->GetWidth();
				m_pControl->SetMinWidth(nImageW);
				m_pControl->SetMaxWidth(nImageW);
			}
			if( (m_pImage != NULL)&&(m_ShowStyle & HFixed) )
			{
				UINT nImageH = m_pImage->GetHeight();
				m_pControl->SetMinHeight(nImageH);
				m_pControl->SetMaxHeight(nImageH);
			}
		}

		int GetID()
		{
			static int nID=-1;
			nID++;
			return nID;
		}
	protected:
		std::wstring									m_strImagePath;
		Image*											m_pImage;
		ShowStyle                                       m_ShowStyle;
		Win32WindowBase*								m_pControl;
		TSTRING											m_strClassName;
	};

	class _DoViewer : public _DoDrawer
	{
	public:
		_DoViewer(Win32WindowBase* pControl,const std::wstring& strImagePath,ShowStyle style):_DoDrawer(pControl,strImagePath,style){}
		//Constructor for XML
		_DoViewer(Win32WindowBase* pControl, INotify* pINotify):_DoDrawer(pControl,pINotify){}
		virtual ~_DoViewer(){}
		virtual void implementAddTrack(double x,double y)
		{
			Point pt((int)x,(int)y);
			m_ptTracks.push_back(pt);
		}
		virtual void implementRemoveTrack()
		{
			m_ptTracks.clear();
		}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false)
		{
			Initiate();
			
			_DoDrawer::implementDraw(pControl,GC,bErase);

			GeometryRect rect = pControl->GetGeometry();
			Graphics graphics(GC);

			int nX = pControl->GetGeometry().m_nX;
			int nY = pControl->GetGeometry().m_nY;
			for(vector<Point>::iterator Iter = m_ptTracks.begin();Iter != m_ptTracks.end();Iter++)
			{
				Pen blackPen(Color(255, 0, 0, 0), 1);
				Pen greenPen(Color(255, 0, 255, 0), 2);
				greenPen.SetAlignment(PenAlignmentCenter);

				// Draw the rectangle with the wide green pen.
				graphics.DrawRectangle(&greenPen, nX + Iter->X - 2, nY + Iter->Y - 2, 4, 4);
				// Draw the same rectangle with the thin black pen.
				graphics.DrawRectangle(&blackPen, nX + Iter->X - 2, nY + Iter->Y - 2, 4, 4);
			}
			
			if(m_pImage != NULL)
			{
				delete m_pImage;
				m_pImage = NULL;
			}
		}		
	private:
		virtual void Initiate()
		{
			if(m_pImage == NULL && m_strImagePath.size() > 0)
			{
				m_pImage = new Image(m_strImagePath.c_str());
			}
			if( (m_pImage != NULL)&&(m_ShowStyle & WFixed) )
			{
				UINT nImageW = m_pImage->GetWidth();
				m_pControl->SetMinWidth(nImageW);
				m_pControl->SetMaxWidth(nImageW);
			}
			if( (m_pImage != NULL)&&(m_ShowStyle & HFixed) )
			{
				UINT nImageH = m_pImage->GetHeight();
				m_pControl->SetMinHeight(nImageH);
				m_pControl->SetMaxHeight(nImageH);
			}
		}
		private:
			vector<Point> m_ptTracks;
	};
 
	class _DoLabel : public INotify
	{
	public:
		//Constructor for XML
		_DoLabel(Win32WindowBase* pControl, INotify* pINotify)
			:m_nAlignment(Left|Top)
			,m_strFont(L"Arial")
			,m_nFontSize(16)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pBkImage(NULL)
			,m_nTextColor( Color::MakeARGB(255,0,0,0) )
			,m_nTopMargin(0)
			,m_nBottomMargin(0)
			,m_nLeftMargin(0)
			,m_nRightMargin(0)
			,m_bFontBold(false)
			,m_strClassName(_T("Win32Label"))
		{

		}
		_DoLabel(Win32WindowBase* pControl,int nAlignment = Left,std::wstring strFont = L"Arial",int nFontSize = 16)
			:m_nAlignment(nAlignment)
			,m_strFont(strFont)
			,m_nFontSize(nFontSize)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pBkImage(NULL)
			,m_nTextColor( Color::MakeARGB(255,0,0,0) )
			,m_nTopMargin(0)
			,m_nBottomMargin(0)
			,m_nLeftMargin(0)
			,m_nRightMargin(0)
			,m_bFontBold(false)
			,m_strClassName(_T("Win32Label"))
		{
			Initiate();
		}

		
		virtual ~_DoLabel()
		{
			if(m_pILocalization)
				m_pILocalization->Unregister(this,TNotify_Language_Change);
		}
		virtual void Event(const TSTRING& strEvent, long nParam)
		{
			if(TNotify_Language_Change == strEvent)
			{
				if(m_pControl)
					m_pControl->Update();
			}
		}
		virtual void implementMouseDown(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseUp(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementMouseMove(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseDoubleClick(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementFocus(Win32WindowBase* pControl,bool bFocused){}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false)
		{
			if(pControl)
			{
				GeometryRect rect = pControl->GetGeometry();
				Graphics graphics(GC);
				// Initialize arguments.
				FontStyle nMyFontStyle = m_bFontBold ? FontStyleBold : FontStyleRegular;
				Font myFont(m_strFont.c_str(), (float)m_nFontSize, nMyFontStyle);
 
				StringFormat format;
				if(m_nAlignment & Left)
					format.SetAlignment(StringAlignmentNear);
				else if(m_nAlignment & Right)
					format.SetAlignment(StringAlignmentFar);
				else
					format.SetAlignment(StringAlignmentCenter);
				if(m_nAlignment & Top)
					format.SetLineAlignment(StringAlignmentNear);
				else if(m_nAlignment & Bottom)
					format.SetLineAlignment(StringAlignmentFar);
				else
					format.SetLineAlignment(StringAlignmentCenter);

 
				SolidBrush TextBrush(m_nTextColor);
 
				std::wstring		strTemp;
				m_pILocalization->GetString(pControl->GetDisplayString(),strTemp);
 
				RectF layoutRect((float)rect.m_nX,(float)rect.m_nY,(float)rect.m_nWidth,(float)rect.m_nHeight);
 
				// Draw background
				if(m_pBkImage)
					graphics.DrawImage(m_pBkImage,layoutRect);
				// Draw string.
				RectF TextRect(
					(float)rect.m_nX + m_nLeftMargin,
					(float)rect.m_nY + m_nTopMargin,
					(float)rect.m_nWidth - m_nLeftMargin - m_nRightMargin,
					(float)rect.m_nHeight - m_nTopMargin - m_nBottomMargin
					);

				graphics.DrawString(
					strTemp.c_str(),
					static_cast<int>(strTemp.size()),
					&myFont,
					TextRect,
					&format,
					&TextBrush);
			}
		}
		virtual void implementClose(Win32WindowBase* pControl)
		{
			if(m_pBkImage != NULL)
			{
				delete m_pBkImage;
				m_pBkImage = NULL;
			}
		}
		virtual bool Save(XMLMANAGER::XMLElement& Element, TSTRING& strID)
		{
			bool bRes = true;
			
			Element.addAttribute(g_strClsName, m_strClassName);
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);

			XMLMANAGER::XMLElement* Property = &(Element.getAllElement()->at(0));
			Property->addAttribute(_T("Font"), m_strFont);
			Property->addAttribute(_T("FontSize"), m_nFontSize);
			Property->addAttribute(_T("FontBold"), m_bFontBold ? _T("true") : _T("false"));
			Property->addAttribute(_T("Alignment"), GetAlignment(m_nAlignment));
			Property->addAttribute(_T("BkImage"),m_strBkImage);
			Color TextColor(m_nTextColor);
			Property->addAttribute(_T("TextColorAlpha"), (int)TextColor.GetAlpha());
			Property->addAttribute(_T("TextColorR"), (int)TextColor.GetR());
			Property->addAttribute(_T("TextColorG"), (int)TextColor.GetG());
			Property->addAttribute(_T("TextColorB"), (int)TextColor.GetB());
			Property->addAttribute(_T("TopMargin"), m_nTopMargin);
			Property->addAttribute(_T("BottomMargin"), m_nBottomMargin);
			Property->addAttribute(_T("LeftMargin"), m_nLeftMargin);
			Property->addAttribute(_T("RightMargin"), m_nRightMargin);

			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = true;
			if(Element.name() == g_strProperty)
			{	
				TSTRING strFont;
				Element.getAttr(_T("Font"), strFont);
				m_strFont = GetUnicode(strFont);

				int nFontSize = Element.getAttr(_T("FontSize"));
				m_nFontSize = nFontSize == -1 ? 0 : nFontSize;

				TSTRING strFontBold;
				Element.getAttr(_T("FontBold"), strFontBold);
				m_bFontBold = strFontBold == _T("true") ? true : false;

				TSTRING strBkImage;
				Element.getAttr(_T("BkImage"),strBkImage);
				SetBkImage(strBkImage);

				int nAlpha = Element.getAttr(_T("TextColorAlpha"));
				nAlpha = nAlpha == -1 ? 255 : nAlpha;
				int nR = Element.getAttr(_T("TextColorR"));
				nR = nR == -1 ? 0 : nR;
				int nG = Element.getAttr(_T("TextColorG"));
				nG = nG == -1 ? 0 : nG;
				int nB = Element.getAttr(_T("TextColorB"));
				nB = nB == -1 ? 0 : nB;
				SetTextColor(Color::MakeARGB(nAlpha,nR,nG,nB));

				TSTRING strAlignment;
				Element.getAttr(_T("Alignment"), strAlignment);
				if(strAlignment != _T(""))
				{
					vector<TSTRING> vstrAlignment;
					Tokenize<TSTRING>(_T("|"), strAlignment,vstrAlignment,false);
					m_nAlignment = 0;
					for(vector<TSTRING>::iterator iter=vstrAlignment.begin(); iter!= vstrAlignment.end(); ++iter)
					{
						if((*iter) == _T("Left"))
							m_nAlignment |= Left;
						else if((*iter) == _T("HCenter"))
							m_nAlignment |= HCenter;
						else if((*iter) == _T("Right"))
							m_nAlignment |= Right;
						else if((*iter) == _T("Top"))
							m_nAlignment |= Top;
						else if((*iter) == _T("VCenter"))
							m_nAlignment |= VCenter;
						else if((*iter) == _T("Bottom"))
							m_nAlignment |= Bottom;
					}
				}
				else
					m_nAlignment = Default;

				int nTopMargin = Element.getAttr(_T("TopMargin"));
				m_nTopMargin = nTopMargin==-1 ? 0 : nTopMargin;
				int nBottomMargin = Element.getAttr(_T("BottomMargin"));
				m_nBottomMargin = nBottomMargin==-1 ? 0 : nBottomMargin;
				int nLeftMargin = Element.getAttr(_T("LeftMargin"));
				m_nLeftMargin = nLeftMargin==-1 ? 0 : nLeftMargin;
				int nRightMargin = Element.getAttr(_T("RightMargin"));
				m_nRightMargin = nRightMargin==-1 ? 0 : nRightMargin;
				Initiate();
			}
			return bRes;
		}
	public:
		virtual void SetBkImage(const TSTRING strBkImage)
		{
			m_strBkImage = strBkImage;
			if(!strBkImage.empty())
			{
				if(m_pBkImage != NULL)
				{
					delete m_pBkImage;
					m_pBkImage = NULL;
				}
				m_pBkImage = new Image(strBkImage.c_str());
			}
		}
		void SetTextColor(ARGB nTextColor)
		{
			m_nTextColor = nTextColor;
		}
		virtual void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			m_nTopMargin	= nTopMargin	; 
			m_nBottomMargin	= nBottomMargin	; 
			m_nLeftMargin   = nLeftMargin   ; 
			m_nRightMargin	= nRightMargin	;
		}
		void SetFontBold(bool bFontBold)
		{
			m_bFontBold = bFontBold;
		}
	private:
		virtual void Initiate()
		{
			CreateILocalization(&m_pILocalization);
			m_pILocalization->Register(this,TNotify_Language_Change);
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
					strAlignment = strAlignment = StrConnect<TSTRING>(_T("|"),strAlignment, _T("Top"), false);
				else if(nAlignment & VCenter)
					strAlignment = strAlignment = StrConnect<TSTRING>(_T("|"), strAlignment, _T("VCenter"), false);
				else if(nAlignment & Bottom)
					strAlignment = StrConnect<TSTRING>(_T("|"), strAlignment, _T("Bottom"), false);
			}
			return strAlignment;
		}
	private:
		std::wstring										m_strFont;
		int													m_nAlignment;
		int													m_nFontSize;
		bool												m_bFontBold;
		ILocalization*										m_pILocalization;
		Win32WindowBase*									m_pControl;
		Image*												m_pBkImage;
		TSTRING												m_strBkImage;		

		ARGB												m_nTextColor;
		int													m_nTopMargin;	
		int													m_nBottomMargin;
		int													m_nLeftMargin;   
		int													m_nRightMargin;

		TSTRING												m_strClassName;
	};
 
	typedef void ( * LPButtonFunc)(void);
	typedef void ( * LPButtonFunc2)(UINT);
 
	class _DoButton : public INotify
	{
	public:
		_DoButton(Win32WindowBase* pControl,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc2 pFunc2,UINT nID)
			:m_bEnter(false)
			,m_bMouseDown(false)
			,m_strImagePath(strImagePath)
			,m_strHighlightImagePath(strHighlightImagePath)
			,m_pImage(NULL),m_pHighlightImage(NULL),m_pMouseDownImage(NULL),m_pDisableImage(NULL)
			,m_pFunc(NULL),m_pFunc2(pFunc2)
			,m_nID(nID)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pINotify(NULL)
			,m_strClassName(_T("Win32Button"))
		{
			Initiate();
		}
		_DoButton(Win32WindowBase* pControl,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc pFunc)
			:m_bEnter(false)
			,m_bMouseDown(false)
			,m_strImagePath(strImagePath)
			,m_strHighlightImagePath(strHighlightImagePath)
			,m_pImage(NULL),m_pHighlightImage(NULL),m_pMouseDownImage(NULL),m_pDisableImage(NULL)
			,m_pFunc(pFunc)
			,m_pFunc2(NULL)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pINotify(NULL)
			,m_strClassName(_T("Win32Button"))
		{
			Initiate();
		}
		_DoButton(Win32WindowBase* pControl,const std::wstring& strImagePath,
			const std::wstring& strHighlightImagePath,const std::wstring& strMouseDownImagePath,
			const std::wstring& strDisableImagePath, LPButtonFunc pFunc,INotify *pINotify = NULL)
			:m_bEnter(false)
			,m_bMouseDown(false)
			,m_strImagePath(strImagePath)
			,m_strHighlightImagePath(strHighlightImagePath)
			,m_strMouseDownImagePath(strMouseDownImagePath)
			,m_strDisableImagePath(strDisableImagePath)
			,m_pImage(NULL),m_pHighlightImage(NULL),m_pMouseDownImage(NULL),m_pDisableImage(NULL)
			,m_pFunc(pFunc)
			,m_pFunc2(NULL)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pINotify(pINotify)
			,m_strClassName(_T("Win32Button"))
		{
			Initiate();
		}
		_DoButton(Win32WindowBase* pControl,const std::wstring& strImagePath,
			const std::wstring& strHighlightImagePath,const std::wstring& strMouseDownImagePath,
			const std::wstring& strDisableImagePath, LPButtonFunc2 pFunc2,UINT nID)
			:m_bEnter(false)
			,m_bMouseDown(false)
			,m_strImagePath(strImagePath)
			,m_strHighlightImagePath(strHighlightImagePath)
			,m_strMouseDownImagePath(strMouseDownImagePath)
			,m_strDisableImagePath(strDisableImagePath)
			,m_pImage(NULL),m_pHighlightImage(NULL),m_pMouseDownImage(NULL),m_pDisableImage(NULL)
			,m_pFunc(NULL)
			,m_pFunc2(pFunc2)
			,m_nID(nID)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pINotify(NULL)
			,m_strClassName(_T("Win32Button"))
		{
			Initiate();
		}

		//Constructor for XML
		_DoButton(Win32WindowBase* pControl, INotify* pINotify)
			:m_bEnter(false)
			,m_bMouseDown(false)
			,m_strImagePath(L"General\\Button\\Btn_Original.png")
			,m_strHighlightImagePath(L"General\\Button\\Btn_MouseOver.png")
			,m_strMouseDownImagePath(L"General\\Button\\Btn_MouseDown.png")
			,m_strDisableImagePath(L"General\\Button\\Btn_Disable.png")
			,m_pImage(NULL),m_pHighlightImage(NULL),m_pMouseDownImage(NULL),m_pDisableImage(NULL)
			,m_pFunc(NULL)
			,m_pFunc2(NULL)
			,m_nID(0)
			,m_pILocalization(NULL)
			,m_pControl(pControl)
			,m_pINotify(pINotify)
			,m_strClassName(_T("Win32Button"))
		{

		}

		virtual ~_DoButton()
		{
			if(m_pILocalization)
				m_pILocalization->Unregister(this,TNotify_Language_Change);
		}
		virtual void Event(const TSTRING& strEvent, long nParam)
		{
			if(TNotify_Language_Change == strEvent)
			{
				if(m_pControl)
					m_pControl->Update();
			}
		}
		virtual void implementClose(Win32WindowBase* pControl)
		{
			if(m_pImage != NULL)
			{
				delete m_pImage;
				m_pImage = NULL;
			}
			if(m_pHighlightImage != NULL)
			{
				delete m_pHighlightImage;
				m_pHighlightImage = NULL;
			}
			if(m_pMouseDownImage != NULL)
			{
				delete m_pMouseDownImage;
				m_pMouseDownImage = NULL;
			}
			if(m_pDisableImage != NULL)
			{
				delete m_pDisableImage;
				m_pDisableImage = NULL;
			}
		}
		virtual void implementMouseDown(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY)
		{
			if(pControl && pControl->GetEnable() && pControl->IsVaildRect(pControl->GetGeometry(),nCX,nCY))
			{
				m_bMouseDown = true;
				pControl->Update();
				pControl->CaptureMouse(true);
			}
		}
		virtual bool implementMouseUp(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY)
		{
			bool bRes = false;
			if(pControl && pControl->GetEnable() && m_bMouseDown)
			{
				m_bMouseDown = false;
				pControl->Update();
				pControl->CaptureMouse(false);
				if(pControl->IsVaildRect(pControl->GetGeometry(),nCX,nCY))
					bRes = implementMouseDoubleClick(pControl,nFlags,nCX,nCY);
			}
			return bRes;
		}
		virtual void implementMouseMove(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY)
		{
			if(pControl && pControl->GetEnable())
			{
				m_nCX = nCX;
				m_nCY = nCY;
				bool bisVaildRect = pControl->IsVaildRect(pControl->GetGeometry(),m_nCX,m_nCY);
 
				if(m_bEnter == false && bisVaildRect)
				{
					m_bEnter = true;
					pControl->Update();
				}
				else if(m_bEnter == true &&  !bisVaildRect)
				{
					m_bEnter = false;
					pControl->Update();
				}
			}
		}
		virtual bool implementMouseDoubleClick(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY)
		{
			bool bRes = false;
			if(pControl && pControl->GetEnable())
			{
				bRes = true;
				if(m_pINotify)
					m_pINotify->Event(EVENT_BUTTON_CLICK,pControl->GetID());
				else if(m_pFunc)
					m_pFunc();
				else if(m_pFunc2)
					m_pFunc2(m_nID);
			}
			return bRes;
		}
		virtual void implementFocus(Win32WindowBase* pControl,bool bFocused)
		{
			if(pControl && pControl->GetEnable())
				if(bFocused == false)
					pControl->CaptureMouse(false);
		}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false)
		{
			if(pControl)
			{
				Graphics graphics(GC);
 
				// Create a Rect object.
				GeometryRect rect = pControl->GetGeometry();
				Rect rcArea(rect.m_nX,rect.m_nY,rect.m_nWidth,rect.m_nHeight);
				DrawBkImage(pControl,graphics,rcArea);
				if(m_pILocalization)
					DrawText(pControl, graphics, rect);
			}
		}
		void JigsawDraw(Graphics& graphics,Image* pImage,const Rect& rcArea)
		{
			if(pImage && pImage->GetHeight() > 0 && pImage->GetWidth() > 0)
			{
				graphics.SetClip(rcArea);
 
				UINT imageW = pImage->GetWidth();
				UINT imageH = pImage->GetHeight();
 
				int nX = rcArea.X;
				int nY = rcArea.Y;
 
				while(nY <= (rcArea.Height + rcArea.Y) )
				{
					while(nX <= (rcArea.Width + rcArea.X) )
					{
						graphics.DrawImage(pImage,nX,nY,imageW, imageH);
						nX += imageW;
					};
					nX = 0;
					nY += imageH;
				}
			}
		}
		virtual bool Save(XMLMANAGER::XMLElement& Element, TSTRING& strID)
		{
			bool bRes = true;
			
			Element.addAttribute(g_strClsName, m_strClassName);
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);

			XMLMANAGER::XMLElement* Property = &(Element.getAllElement()->at(0));
			Property->addAttribute(_T("OriginalImagePath"), GetTSTRING(m_strImagePath));
			Property->addAttribute(_T("MouseOverImagePath"), GetTSTRING(m_strHighlightImagePath));
			Property->addAttribute(_T("MouseDownImagePath"), GetTSTRING(m_strMouseDownImagePath));
			Property->addAttribute(_T("DisableImagePath"), GetTSTRING(m_strDisableImagePath));
			Property->addAttribute(_T("MinWidth"), m_pControl->GetMinWidth());
			Property->addAttribute(_T("MinHeight"), m_pControl->GetMinHeight());
			Property->addAttribute(_T("MaxWidth"), m_pControl->GetMaxWidth());
			Property->addAttribute(_T("MaxHeight"), m_pControl->GetMaxHeight());
			//Property->addAttribute(_T("nID"), (int)m_nID);
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = true;
			if(Element.name() == g_strProperty)
			{
				TSTRING strImage;
				Element.getAttr(_T("OriginalImagePath"), strImage);
				if(!strImage.empty())
					m_strImagePath = GetUnicode(strImage);
				Element.getAttr(_T("MouseOverImagePath"), strImage);
				if(!strImage.empty())
					m_strHighlightImagePath = GetUnicode(strImage);
				Element.getAttr(_T("MouseDownImagePath"), strImage);
				if(!strImage.empty())
					m_strMouseDownImagePath = GetUnicode(strImage);
				Element.getAttr(_T("DisableImagePath"), strImage);
				if(!strImage.empty())
					m_strDisableImagePath = GetUnicode(strImage);

				m_pControl->SetMinWidth(Element.getAttr(_T("MinWidth")));
				m_pControl->SetMinHeight(Element.getAttr(_T("MinHeight")));
				m_pControl->SetMaxWidth(Element.getAttr(_T("MaxWidth")));
				m_pControl->SetMaxHeight(Element.getAttr(_T("MaxHeight")));
				
				int nID = Element.getAttr((_T("nID")));
				if(nID != -1)
					m_nID = nID;
				Initiate();
			}
			return bRes;
		}
	protected:
		virtual void SetImage(Win32WindowBase* pControl)
		{
			if(m_pImage == NULL && m_strImagePath.size() > 0)
				m_pImage = new Image(pControl->GetPictureFolder(m_strImagePath).c_str());
			if(m_pHighlightImage == NULL && m_strHighlightImagePath.size() > 0)
				m_pHighlightImage = new Image(pControl->GetPictureFolder(m_strHighlightImagePath).c_str());
			if(m_pMouseDownImage == NULL && m_strMouseDownImagePath.size() >0)
				m_pMouseDownImage = new Image(pControl->GetPictureFolder(m_strMouseDownImagePath).c_str());
			if(m_pDisableImage == NULL && m_strDisableImagePath.size() >0)
				m_pDisableImage = new Image(pControl->GetPictureFolder(m_strDisableImagePath).c_str());
		}
		virtual void DrawBkImage(Win32WindowBase* pControl,Graphics& graphics,Rect& rcArea)
		{
			Image* pImage = m_pImage;
			// Draw rect.
			if( pControl->GetEnable() && m_bEnter)
			{
				if(!m_bMouseDown && m_pHighlightImage)// MouseOver
				{
					pImage = m_pHighlightImage;
				}
				else if (m_pMouseDownImage)
				{
					pImage = m_pMouseDownImage;
				}
			}
			else if ( pControl->GetEnable() )
			{
				if(m_pImage)
					pImage = m_pImage;
			}
			else if(!pControl->GetEnable())
			{
				if(m_pDisableImage)
					pImage = m_pDisableImage;
			}
			graphics.DrawImage(pImage,rcArea);
		}
		void DrawText(Win32WindowBase* pControl,Graphics& graphics,GeometryRect& rect)
		{			
			// Initialize arguments.
			Font myFont(L"Arial", 9);

			StringFormat format;
			format.SetAlignment(StringAlignmentCenter);
			format.SetLineAlignment(StringAlignmentCenter);


			std::wstring		strTemp;
			m_pILocalization->GetString(pControl->GetDisplayString(),strTemp);

			RectF layoutRect((float)rect.m_nX,(float)rect.m_nY,(float)rect.m_nWidth,(float)rect.m_nHeight);

			// Draw string.
			if(pControl->GetEnable())
			{
				SolidBrush WhiteBrush(Color(255, 255, 255, 255));
				graphics.DrawString(
					strTemp.c_str(),
					static_cast<int>(strTemp.size()),
					&myFont,
					layoutRect,
					&format,
					&WhiteBrush);		
			}
			else
			{
				SolidBrush GrayBrush(Color(255, 97, 97, 97));
				graphics.DrawString(
					strTemp.c_str(),
					static_cast<int>(strTemp.size()),
					&myFont,
					layoutRect,
					&format,
					&GrayBrush);		
			}
	
		}
		ILocalization* GetILocalization(){return m_pILocalization;}
	public:
		bool IsMouseEnter(){return m_bEnter;}
		bool IsMouseDown(){return m_bMouseDown;}
		int GetImageWidth()
		{
			int nRes = 0;
			if(m_pImage)
				nRes = static_cast<int>(m_pImage->GetWidth());
			return nRes;
		}
		int GetImageHeight()
		{
			int nRes = 0;
			if(m_pImage)
				nRes = static_cast<int>(m_pImage->GetHeight());
			return nRes;
		}
	private:
		virtual void Initiate()
		{
			CreateILocalization(&m_pILocalization);
			m_pILocalization->Register(this,TNotify_Language_Change);
			SetImage(m_pControl);
		}

		int GetID()
		{
			static int nID=-1;
			nID++;
			return nID;
		}
	private:
		int											m_nCX;
		int											m_nCY;
		bool										m_bEnter;
		bool										m_bMouseDown;
		std::wstring								m_strImagePath;
		std::wstring								m_strHighlightImagePath;
		std::wstring								m_strMouseDownImagePath;
		std::wstring								m_strDisableImagePath;
		Image*										m_pImage;
		Image*										m_pHighlightImage;
		Image*										m_pMouseDownImage;
		Image*										m_pDisableImage;
		LPButtonFunc								m_pFunc;
		LPButtonFunc2								m_pFunc2;
		UINT										m_nID;
		ILocalization*								m_pILocalization;
		Win32WindowBase*							m_pControl;
		INotify*									m_pINotify;
		TSTRING										m_strClassName;
	};
 
	class AttachPolicy
	{
	public:
		static bool Move(Win32WindowBase* pThis,HWND hWindow,HWND hParentWindow = NULL)
		{
			if(hWindow && pThis->IsShow())
			{
				GeometryRect rect = pThis->GetGeometry();
				bool bGetPos = false;
 
				if(hParentWindow)
				{
					CRect rcParent;
					::GetWindowRect(hParentWindow,&rcParent);
					::SetWindowPos(hWindow,HWND_TOP,rcParent.left,rcParent.top,rcParent.Width(),rcParent.Height(),SWP_SHOWWINDOW);
				}
				else
				{
					CRect rcSelf;
					::GetWindowRect(hWindow,&rcSelf);
					CRect rcParent;
					HWND hParent = ::GetParent(hWindow);
					if(hParent)
					{
						CPoint ptCur = rcSelf.TopLeft();
						::ScreenToClient(hParent,&ptCur);
						rcSelf.MoveToXY(ptCur);
						bGetPos = true;
					}

					bool bNeedMove = true;
					if(bGetPos)
						if(rcSelf.left == rect.m_nX && rcSelf.top == rect.m_nY && rcSelf.Width() == rect.m_nWidth && rcSelf.Height() == rect.m_nHeight)
							bNeedMove = false;

					if(bNeedMove)
						::MoveWindow(hWindow,rect.m_nX,rect.m_nY,rect.m_nWidth,rect.m_nHeight,FALSE);
				}
 
				pThis->SetValidateRect(pThis->GetGeometry());
			}

			return true;
		}
		static void Update(HWND hWindow,const GeometryRect& rcNew,bool bErase = false)
		{
			if(hWindow)
			{
				::InvalidateRect(hWindow,NULL,bErase == true);
			}
		}
		static void Show(Win32WindowBase* pThis,HWND hWindow,bool bShow,bool bErase = false)
		{
			if(hWindow)
			{
				//參數SWP_NOZORDER代表保留原本的Order,若不加入這個參數,會造成resource裡設定的Tab順序失效
				::SetWindowPos(hWindow,NULL,0,0,0,0,bShow ? SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER : SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER);
				//::ShowWindow(hWindow,bShow ? SW_SHOW : SW_HIDE);
			
				pThis->Widge::Show(bShow);
			}
			if(bShow == false)
			{
				GeometryRect EmptyRect;
				pThis->SetValidateRect(EmptyRect);
			}
			else
			{
				pThis->SetValidateRect(pThis->GetGeometry());
			}
		}
	};
  
	class ButtonPolicy
	{
	public:
		static bool Move(Win32WindowBase* pThis,HWND hWindow,HWND hParentWindow = NULL){ return true;}
		static void Update(HWND hWindow,const GeometryRect& rect,bool bErase = false)
		{
			if(hWindow)
			{
				CRect rcArea(rect.m_nX,rect.m_nY,rect.m_nX + rect.m_nWidth,rect.m_nY + rect.m_nHeight);
				::InvalidateRect(hWindow,&rcArea,FALSE);
			}
		}
		static void Show(Win32WindowBase* pThis,HWND m_hWindow,bool bShow,bool bErase = false)
		{
			pThis->Widge::Show(bShow,bErase);
		}
	};

	template <class GeneralPolicy,class ActionPolicy = _DoNothing>
	class TWin32WindowWidge : public Win32WindowBase
	{
	protected:
		//For WIN32LabelWidge overwrite
		TWin32WindowWidge(CWnd* pWindow,const std::wstring& strDisplay,int nAlignment ,std::wstring strFont = L"Arial",int nFontSize = 16)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Label")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,nAlignment,strFont,nFontSize));
			SetDisplayString(strDisplay);
		}
	public:
		//For WIN32ButtonWidge overwrite
		TWin32WindowWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc2 pFunc2,UINT nID)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Button")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,strImagePath,strHighlightImagePath,pFunc2,nID));
			SetID(nID);
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		TWin32WindowWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strImagePath,const std::wstring& strHighlightImagePath,LPButtonFunc pFunc)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Button")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,strImagePath,strHighlightImagePath,pFunc));
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With four images
		TWin32WindowWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strOrgImagePath,const std::wstring& strMouseOverImagePath,
			const std::wstring& strMouseDownImagePath,const std::wstring& strDisableImagePath,LPButtonFunc pFunc)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Button")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,pFunc));
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With four images
		TWin32WindowWidge(CWnd* pWindow,const std::wstring& strDisplay,const std::wstring& strOrgImagePath,const std::wstring& strMouseOverImagePath,
			const std::wstring& strMouseDownImagePath,const std::wstring& strDisableImagePath,LPButtonFunc2 pFunc2,UINT nID)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Button")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,pFunc2,nID));
			SetID(nID);
 			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With Default four images
		TWin32WindowWidge(CWnd* pWindow,LPButtonFunc pFunc,const std::wstring& strDisplay)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Button")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{ 
			std::wstring strOrgImagePath = L"General\\Button\\Btn_Original.png";
			std::wstring strMouseOverImagePath = L"General\\Button\\Btn_MouseOver.png";
			std::wstring strMouseDownImagePath = L"General\\Button\\Btn_MouseDown.png";
			std::wstring strDisableImagePath = L"General\\Button\\Btn_Disable.png";
 
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,pFunc));
			SetDisplayString(strDisplay);
		}
		//For WIN32ButtonWidge overwrite
		//With Default four images
		TWin32WindowWidge(CWnd* pWindow,INotify* pINotify,const std::wstring& strDisplay)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Button")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{ 
			std::wstring strOrgImagePath = L"General\\Button\\Btn_Original.png";
			std::wstring strMouseOverImagePath = L"General\\Button\\Btn_MouseOver.png";
			std::wstring strMouseDownImagePath = L"General\\Button\\Btn_MouseDown.png";
			std::wstring strDisableImagePath = L"General\\Button\\Btn_Disable.png";
 
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,strOrgImagePath,strMouseOverImagePath,strMouseDownImagePath, strDisableImagePath,NULL,pINotify));
			SetDisplayString(strDisplay);
		}
		//For WIN32DrawerWidge
		TWin32WindowWidge(CWnd* pWindow,const std::wstring& strImagePath ,ShowStyle style = Stretch)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Image")),m_nDlgItem(-1),m_hParentWnd(NULL)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			SetWindow(pWindow);
 			SetAction(new ActionPolicy(this,strImagePath,style));
		}
		//For win23 window widget
		TWin32WindowWidge(CWnd* pWindow,INotify* pINotify = NULL)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Windows")),m_nDlgItem(::GetDlgCtrlID(pWindow->GetSafeHwnd())),m_hParentWnd(NULL)
		{
			SetWindow(pWindow);
			SetAction(new ActionPolicy(this,pINotify));
		}
		//For win23 application widget
		TWin32WindowWidge(HWND hParent,const TSTRING& strWindow,const TSTRING& strClass,const TSTRING& strAppPath,INotify* pINotify = NULL)
			:m_pAction(NULL),m_hWnd(NULL),m_bCaptured(false),m_strClassName(_T("Win32Windows")),m_nDlgItem(-1),m_hParentWnd(hParent)
		{
			HWND hFindWnd = ::FindWindow(strClass.c_str(),strWindow.c_str());
			if(::IsWindow(hFindWnd) == FALSE)
			{
				::ShellExecute(NULL,_T("open"),strAppPath.c_str(),NULL,NULL,SW_HIDE);
				hFindWnd = ::FindWindow(strClass.c_str(),strWindow.c_str());
				while(::IsWindow(hFindWnd) == FALSE)
				{
					::Sleep(50);
					hFindWnd = ::FindWindow(strClass.c_str(),strWindow.c_str());
				};
			}			
			SetWindow(CWnd::FromHandle(hFindWnd));
			SetAction(new ActionPolicy(this,pINotify));
		}
		virtual ~TWin32WindowWidge(void)
		{
			if(m_pAction)
			{
				delete m_pAction;
				m_pAction = NULL;
			}
		}
	public:
		virtual void CaptureMouse(bool bCapture)
		{
			if(m_hWnd)
			{
				if(bCapture)
				{
					if(m_bCaptured == false)
					{
						::SetCapture(m_hWnd);
						Capture(true);
						m_bCaptured = true;
					}
				}
				else
				{
					if(m_bCaptured == true)
					{
						::ReleaseCapture();
						Capture(false);
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
			if(m_pAction && IsShow())
				m_pAction->implementMouseUp(this,0,nCX,nCY);
		}
		virtual void MouseDown(int nCX,int nCY)
		{
			if(m_pAction && IsShow())
				m_pAction->implementMouseDown(this,0,nCX,nCY);
		}
		virtual void MouseDBClk(int nCX,int nCY)
		{
			if(m_pAction && IsShow() && IsVaild(nCX,nCY))
				m_pAction->implementMouseDoubleClick(this,0,nCX,nCY);
		}
		virtual void MouseMove(int nCX,int nCY)
		{
			if(m_pAction && IsShow())
				m_pAction->implementMouseMove(this,0,nCX,nCY);
		}
		virtual void Draw(HDC GC,bool bErase = false)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			if(m_pAction && IsShow())
				m_pAction->implementDraw(this,GC,bErase);
		}
		virtual void Close()
		{
			if(::IsWindow(m_hWnd) && m_hParentWnd)
				::PostMessage(m_hWnd,WM_CLOSE,NULL,NULL);

			if(m_pAction)
				m_pAction->implementClose(this);
		}
		virtual void Update(bool bErase = false)
		{
			GeneralPolicy::Update(m_hWnd,GetGeometry(),bErase);
		}
		virtual bool SetLayout(Widge* pIWidge = 0)
		{
			if(GeneralPolicy::Move(this,m_hWnd,m_hParentWnd))
				Update(false);
			return true;
		}
		virtual bool Save(XMLMANAGER::XMLElement& MainElement, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase/*Don't care*/)
		{
			using namespace XMLMANAGER;
			bool bRes = false;
			XMLElement EleWidget(g_strWidget);
			XMLElement Property(g_strProperty);
			if(m_nDlgItem != -1)
				Property.addAttribute(_T("Win32ID"), m_nDlgItem);
			EleWidget.getAllElement()->push_back(Property);
			TSTRING strID;
			if(m_pAction)
			{
				bRes = m_pAction->Save(EleWidget, strID);
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
				m_nDlgItem = Element.getAttr(_T("Win32ID"));
				if(m_nDlgItem != -1 && m_hWnd)
					SetWindow(CWnd::FromHandle(m_hWnd)->GetDlgItem(m_nDlgItem));
				if(m_pAction)
					bRes = m_pAction->Load(Element) && Win32WindowBase::Load(Element);
			}
			return bRes;
		}

		virtual void Show(bool bShow,bool bErase = false)
		{
			GeneralPolicy::Show(this,m_hWnd,bShow,bErase);
		}
		void AddTrack(double x,double y)
		{
			if(m_pAction)
				m_pAction->implementAddTrack(x,y);
		}
		void RemoveTrack()
		{
			if(m_pAction)
				m_pAction->implementRemoveTrack();
		}		

		//Just for WIN32DrawerWidge
		void SetImagePath(const std::wstring& strImagePath)
		{
			//Warning: 若有執行過CFileDialog, 則CurrentDirectory會被改變
			//所以要在CFileDialog執行前先GetCurrentDirectory,再CFileDialog結束後執行SetCurrentDirectory還原，才不會出問題
			if(m_pAction)
				m_pAction->implementSetImagePath(strImagePath);
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
		void SetWindow(CWnd* pWindow)
		{
			if(pWindow)
				m_hWnd = pWindow->GetSafeHwnd();
		}
		void SetAction(ActionPolicy* pNewAction)
		{
			if(m_pAction == NULL)
				m_pAction  = pNewAction;
		}
		ActionPolicy* GetActionPolicy() const {return m_pAction;}
	private:
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
		HWND											m_hParentWnd;
		ActionPolicy*									m_pAction;
		bool											m_bCaptured;
		TSTRING											m_strClassName;
		int												m_nDlgItem;
	};

	class WIN32LabelWidge : public TWin32WindowWidge<ButtonPolicy,_DoLabel>
	{
	public:
		WIN32LabelWidge(CWnd* pWindow,const std::wstring& strDisplay,int nAlignment ,std::wstring strFont = L"Arial",int nFontSize = 16)
			:TWin32WindowWidge(pWindow,strDisplay,nAlignment ,strFont,nFontSize){}
		WIN32LabelWidge(CWnd* pWindow, INotify* pINotify):TWin32WindowWidge(pWindow,pINotify){}
		virtual ~WIN32LabelWidge(){}
	public:
		virtual void SetBkImage(const TSTRING& strBkImage)
		{
			GetActionPolicy()->SetBkImage(GetPictureFolder() + strBkImage);
		}
		void SetTextColor(int nAlpha,int nRed,int nGreen,int nBlue)
		{
			if(GetActionPolicy())
			{
				ARGB nTextColor = Color::MakeARGB(nAlpha, nRed, nGreen, nBlue);
				GetActionPolicy()->SetTextColor(nTextColor);
			}
		}
		void SetFontBold(bool bFontBold)
		{
			if(GetActionPolicy())
				GetActionPolicy()->SetFontBold(bFontBold);
		}
		virtual void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			if(GetActionPolicy())
				GetActionPolicy()->SetMargin( nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
		}
	};
 
	typedef TWin32WindowWidge<AttachPolicy>							WIN32ApplicationWidge;
	typedef TWin32WindowWidge<AttachPolicy>							WIN32WindowWidge;
	typedef TWin32WindowWidge<ButtonPolicy,_DoDrawer>	WIN32DrawerWidge;
	typedef TWin32WindowWidge<ButtonPolicy,_DoButton>		WIN32ButtonWidge;
	typedef TWin32WindowWidge<ButtonPolicy,_DoViewer>		WIN32PictureViewerWidge;
#endif //WIN32
}
#endif //#ifndef WIN32Widge_header_
