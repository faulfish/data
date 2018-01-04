/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
*
*                               3G Project
*
*	             Copyright (c) 2005 BENQ Corpration
*
***************************************************************************
*
*  $Workfile:   Win32FrameworkWidge.h  $
*  $Revision:   1.2  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Components/UIFramework/LayoutManager/Win32FrameworkWidge.h.-arc  $
 * 
 *    Rev 1.2   Apr 01 2009 14:17:26   Kevin Tai
 * no change
 * 
 *    Rev 1.1   Jun 25 2008 09:52:44   Jame Tsai
 * Update to the latest version of this is QSyncer
 * 
 *    Rev 1.45   Jun 19 2008 13:30:34   Jame Tsai
 * add function: SetBKImage_AbsolutePath
 * 
 *    Rev 1.44   Feb 25 2008 22:02:42   Mei Yang
 * update Save Parameter
 * 
 *    Rev 1.43   Jan 29 2008 08:59:04   Mei Yang
 * add Save and Load
 * 
 *    Rev 1.42   Jan 28 2008 17:49:08   Alan Tu
 * update
 * 
 *    Rev 1.41   Jan 08 2008 20:43:04   Jame Tsai
 * fix error
 * 
 *    Rev 1.40   Jan 02 2008 10:20:22   Jame Tsai
 * add function
 * 
 *    Rev 1.39   Dec 28 2007 08:59:14   Eddy Guo
 * fix include error
 * 
 *    Rev 1.38   Dec 27 2007 16:37:16   Eddy Guo
 * add CQDialog
 * 
 *    Rev 1.37   Dec 20 2007 21:00:18   Jame Tsai
 * add timer message
 * 
 *    Rev 1.36   Dec 20 2007 18:31:24   Alan Tu
 * Avoid ESC to close window
 * 
 *    Rev 1.35   Dec 17 2007 14:24:00   Alan Tu
 * add BK function
 * 
 *    Rev 1.34   Nov 30 2007 13:31:38   Alan Tu
 * update for MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y
 * 
 *    Rev 1.33   Nov 22 2007 18:29:40   Alan Tu
 * Add default picture folder
 * 
 *    Rev 1.32   Nov 22 2007 18:13:44   Alan Tu
 * update
 * 
 *    Rev 1.31   Nov 20 2007 09:04:26   Alan Tu
 * update pictures
 * 
 *    Rev 1.30   Nov 15 2007 10:55:12   Alan Tu
 * update for clip
* 
*    Rev 1.29   Nov 14 2007 09:24:00   Mei Yang
* add VM log
*
*
***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Win32FrameworkWidge.h
@brief {brief description of Win32FrameworkWidge.h}

{detail description of Win32FrameworkWidge.h}
*/
#ifndef WIN32FrameworkWidge_header_
#define WIN32FrameworkWidge_header_

#include <string>
#include "TWidge.h"
#include "../../../CommonUtil/CommonDrawer.h"
#include "../Porting/QDialog.h"
#include <cassert>
//#include "Win32Widge.h"
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

	enum BKStyle
	{
		BK_Stretch = 0, //Default
		BK_Tile = 1,
		BK_Fixed = 6
	};

	template <class MFCWindowBase>
	class Win32DoubleBufferFormCWndWidge : public MFCWindowBase, public Widge<HDC>
	{
	protected:
		Win32DoubleBufferFormCWndWidge():m_pVaildRgn(NULL),m_pImage(NULL),m_nShowStyle(BK_Tile){}
		Win32DoubleBufferFormCWndWidge(UINT ID):MFCWindowBase(ID),m_pVaildRgn(NULL),m_pImage(NULL),m_nShowStyle(BK_Tile){}
		Win32DoubleBufferFormCWndWidge(UINT ID,CWnd* pParent):MFCWindowBase(ID,pParent),m_pVaildRgn(NULL),m_pImage(NULL),m_nShowStyle(BK_Tile){}
		virtual ~Win32DoubleBufferFormCWndWidge(void)
		{
			if(m_pImage)
			{
				delete m_pImage;
				m_pImage = NULL;
			}

			if(m_pVaildRgn)
			{
				delete m_pVaildRgn;
				m_pVaildRgn = NULL;
			}
		}
	public:
		void SetBKImage(const std::wstring& strBKImage,BKStyle nShowStyle = BK_Tile)
		{
			m_nShowStyle = nShowStyle;
			
			if(m_pImage)
			{
				delete m_pImage;
				m_pImage = NULL;
			}

			if(m_pImage == NULL)
			{
				std::wstring strPath = GetPictureFolder() + strBKImage;
				m_pImage = new Image(strPath.c_str());
			}
		}
		void SetBKImage_AbsolutePath(const std::wstring& strBKImage,BKStyle nShowStyle = BK_Tile)
		{
			m_nShowStyle = nShowStyle;

			if(m_pImage)
			{
				delete m_pImage;
				m_pImage = NULL;
			}
			if(m_pImage == NULL)
				m_pImage = new Image(strBKImage.c_str());
		}
		Image* GetBKImage() const {return m_pImage;}
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if(WM_INITDIALOG == message)
			{
				MFCWindowBase::WindowProc(message,wParam,lParam);

				RECT rcEmpty;
				GetWindowRect(&rcEmpty);
				if(SendMessage(WM_SIZING,WMSZ_BOTTOMRIGHT,(LPARAM)&rcEmpty))
					MoveWindow(&rcEmpty);

				return TRUE;
			}/*
			 else if(WM_WINDOWPOSCHANGED == message)
			 {
			 //if(GetLayout())
				{
				CRect rcClient;
				GetClientRect(&rcClient);

				CRect rcParent;
				GetWindowRect(rcParent);

				int nHighOffset = rcParent.Height() - rcClient.Height();
				int nWidthOffset = rcParent.Width()- rcClient.Width();

				WINDOWPOS* pPos = (WINDOWPOS*)lParam;
				SetGeometry(0,0,pPos->cx - nWidthOffset,pPos->cy - nHighOffset);

				SetLayout();

				return TRUE;
				}
				}
				else if(message == WM_WINDOWPOSCHANGING)
				{
				return TRUE;
				}*/
			else if(WM_SIZE == message)
			{
				SetGeometry(0,0,MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y);
				SetLayout();
				if(GetLayout())
					return TRUE;
			}
			else if(WM_SIZING == message)
			{
				CRect rcClient;
				GetClientRect(&rcClient);

				CRect rcParent;
				GetWindowRect(rcParent);

				int nHighOffset = rcParent.Height() - rcClient.Height();
				int nWidthOffset = rcParent.Width()- rcClient.Width();

				RECT* pRect = (RECT*)lParam;
				int nCX = pRect->right - pRect->left - nWidthOffset;
				int nCY = pRect->bottom - pRect->top - nHighOffset;

				bool bTestSizeOK = false;
				//for test the new size if fine for this widge
				bTestSizeOK = SetGeometry(0,0,nCX,nCY);
				if(bTestSizeOK == false)
				{
					nCX = GetGeometry().m_nWidth;
					nCY = GetGeometry().m_nHeight;
				}
				//for test the new size if fine for all widge in layout
				if(GetLayout() && GetLayout()->SetGeometry(0,0,nCX,nCY) == false)
					bTestSizeOK = false;

				if(bTestSizeOK == false)
				{
					/*
					If SetGeometry return fail, it means it can't fit the size and it asign a new size automatically
					So we should resize by the new size.
					*/

					//Set the maximum size for Win32DoubleBufferDialogWidge's policy
					if(GetLayout())
					{
						GeometryRect rectNew(GetGeometry());

						if(rectNew.m_nHeight < GetLayout()->GetGeometry().m_nHeight)
							rectNew.m_nHeight = GetLayout()->GetGeometry().m_nHeight;
						if(rectNew.m_nWidth < GetLayout()->GetGeometry().m_nWidth)
							rectNew.m_nWidth = GetLayout()->GetGeometry().m_nWidth;

						SetGeometry(rectNew);
					}

					pRect->right = pRect->left + GetGeometry().m_nWidth + nWidthOffset;
					pRect->bottom = pRect->top + GetGeometry().m_nHeight + nHighOffset;
                    MFCWindowBase::WindowProc(message,wParam,lParam);
					return TRUE;
				}
			}
			else if(WM_DESTROY  == message)
			{
				Close();
			}
			else if(WM_ERASEBKGND  == message)
			{
				return TRUE;
			}
			else if(WM_PAINT == message)
			{
				CRect rcUpdate;
				if(GetUpdateRect(rcUpdate))
				{
					PAINTSTRUCT ps;
					CDC* pDC = BeginPaint(&ps);
					if(pDC)
					{
						Draw(pDC->GetSafeHdc(),ps.fErase == TRUE);
						EndPaint(&ps);
					}
					return TRUE;
				}
			}
			else if(WM_MOUSEMOVE == message)
			{
				if(IsVaildRect(GetGeometry(),MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y))
				{
					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_LEAVE;
					tme.hwndTrack = GetSafeHwnd();
					TrackMouseEvent(&tme);
				}

				MouseMove(MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y);
			}
			else if(WM_LBUTTONUP == message)
			{
				MouseUp(MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y);
			}
			else if(WM_LBUTTONDOWN == message)
			{
				MouseDown(MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y);
			}
			else if(WM_LBUTTONDBLCLK == message)
			{
				MouseDBClk(MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y);
			}
			else if(WM_MOUSELEAVE == message)
			{
				CPoint ptOld;
				GetCursorPos(&ptOld);

				ScreenToClient(&ptOld);
				MouseMove(ptOld.x,ptOld.y);
			}
			else if(WM_NCPAINT == message)
			{
				//return TRUE;
			}
			else if(WM_SETFOCUS == message)
			{
				Focus(true);
			}
			else if(WM_KILLFOCUS == message)
			{
				Focus(false);
			}
			else if(WM_KEYDOWN == message)			
			{
				switch(wParam)
				{
				case VK_ESCAPE:
						return TRUE;  //(to disable <Esc>-Key-Exit.)
					break;
				default:
					break;
				};
			}
			else if(WM_TIMER == message)
			{
				Timer(wParam);
			}

			return MFCWindowBase::WindowProc(message, wParam, lParam);
		}
		virtual bool IsLayoutManager(){return false;}
		virtual bool IsWindow(){return true;}
		virtual void Update(bool bErase = false)
		{
			Invalidate(false);
		}
		virtual bool GetInvaildRgn(CRgn& hInvaildRgn,bool bErase)
		{
			std::vector<GeometryRect> rectVaildArray;
			GetValidateRect(rectVaildArray);
			for(std::vector<GeometryRect>::iterator Iter = rectVaildArray.begin();Iter != rectVaildArray.end();Iter++)
			{
				GeometryRect rect = *Iter;
				CRect rcVaild(rect.m_nX,rect.m_nY,rect.m_nX + rect.m_nWidth,rect.m_nY + rect.m_nHeight);

				CRgn hRgn;
				hRgn.CreateRectRgnIndirect(rcVaild);

				if(m_pVaildRgn == NULL)
				{
					m_pVaildRgn = new CRgn;
					m_pVaildRgn->CreateRectRgn( 0, 0, 0, 0 );
				}

				int nCombineResult = m_pVaildRgn->CombineRgn( m_pVaildRgn, &hRgn, RGN_OR );
				assert(nCombineResult != ERROR || nCombineResult != NULLREGION  );
			}

			int nCombineResult = ERROR;
			CRect rcAllArea(GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nX + GetGeometry().m_nWidth,GetGeometry().m_nY + GetGeometry().m_nHeight);
			if(hInvaildRgn.CreateRectRgnIndirect(rcAllArea))
				nCombineResult = SIMPLEREGION;

			if(m_pVaildRgn)
			{
				/*
				Specifies the type of the resulting region. It can be one of the following values:
				3 COMPLEXREGION   New region has overlapping borders.
				0 ERROR   No new region created.
				1 NULLREGION   New region is empty.
				2 SIMPLEREGION   New region has no overlapping borders.
				*/
				nCombineResult = hInvaildRgn.CombineRgn( &hInvaildRgn, m_pVaildRgn, RGN_DIFF );
				delete m_pVaildRgn;
				m_pVaildRgn = NULL;
			}
			return nCombineResult != NULLREGION && nCombineResult != ERROR;
		}
		virtual void Draw(HDC GC,bool bErase = false)
		{
			// Set the clipping region with invaild region.
			CRgn rgnTest;
			if(GetInvaildRgn(rgnTest,bErase))
			{
				if(m_pImage == NULL)
					SetBKImage(_T("General\\Background\\bk_main.png"),m_nShowStyle);

				//Prepare memory buffer
				COMMONCONTROL::_DoubleBufferPolicy doubleBuffer;
				//COMMONCONTROL::_SingleBufferPolicy doubleBuffer;

				CRect rcAllArea(GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nX + GetGeometry().m_nWidth,GetGeometry().m_nY + GetGeometry().m_nHeight);
				HDC hMemoryBuffer = doubleBuffer.BeginHDC(GC,rcAllArea,rgnTest);

				Graphics graphics(hMemoryBuffer);
				Graphics* pGraphics = &graphics;

#ifdef _DEBUG
				//for test performance issue
				//Graphics graphicsD(GC);
				//SolidBrush whiteBrush( Color(255, 255,0,0) );
				//Region rgn(rgnTest);
				//graphicsD.FillRegion(&whiteBrush,&rgn);
				//Sleep(100);
#endif

				// Fill background
				if(m_pImage)
				{
					Rect rcArea(GetGeometry().m_nX, GetGeometry().m_nY,GetGeometry().m_nWidth, GetGeometry().m_nHeight);
					if(m_nShowStyle == BK_Tile)
						JigsawDraw(*pGraphics,m_pImage,rcArea,m_pImage->GetWidth(),m_pImage->GetHeight());
					else if(m_nShowStyle == BK_Stretch)
						graphics.DrawImage(m_pImage,GetGeometry().m_nX, GetGeometry().m_nY,GetGeometry().m_nWidth, GetGeometry().m_nHeight);
					else if(m_nShowStyle == BK_Fixed)
					{						
						UINT nImageW = m_pImage->GetWidth();
						UINT nImageH = m_pImage->GetHeight();
						graphics.DrawImage(m_pImage,GetGeometry().m_nX, GetGeometry().m_nY,nImageW, nImageH);
					}
				}
				else
				{
					DWORD dwResult = ::GetSysColor(COLOR_BTNFACE);
					SolidBrush whiteBrush(Color(255, GetRValue(dwResult), GetGValue(dwResult), GetBValue(dwResult)));
					pGraphics->FillRectangle(&whiteBrush, 0,0,GetGeometry().m_nWidth, GetGeometry().m_nHeight);
				}

				hMemoryBuffer = pGraphics->GetHDC();
				Widge<HDC>::Draw(hMemoryBuffer,bErase);
			}
		}

		virtual bool Save(XMLMANAGER::XMLElement& Element, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase){return true;}
		virtual bool Load(const XMLMANAGER::XMLElement& Element){return true;}
		void JigsawDraw(Graphics& graphics,Image* pImage,const Rect& rcArea,UINT imageW,UINT imageH)
		{
			if(pImage && imageW > 0 && imageH > 0)
			{ 
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

		void JigsawDraw(Graphics& graphics,CachedBitmap* pImage,const Rect& rcArea,UINT imageW,UINT imageH)
		{
			if(pImage && imageW > 0 && imageH > 0)
			{ 
				int nX = rcArea.X;
				int nY = rcArea.Y;

				while(nY <= (rcArea.Height + rcArea.Y) )
				{
					while(nX <= (rcArea.Width + rcArea.X) )
					{
						graphics.DrawCachedBitmap(pImage, nX,nY);
						nX += imageW;
					};
					nX = 0;
					nY += imageH;
				}
			}
		}

	private:
		Image*											m_pImage;
		CRgn*												m_pVaildRgn;
		BKStyle											m_nShowStyle;
		COMMONCONTROL::GlobalCreater<COMMONCONTROL::_GdiplusCreaterPolicy> m_GDIPLUS;
	};

	typedef Win32DoubleBufferFormCWndWidge<CFormView>			Win32DoubleBufferFormViewWidge;
	typedef Win32DoubleBufferFormCWndWidge<CDialog>					Win32DoubleBufferDialogWidge;
    typedef Win32DoubleBufferFormCWndWidge<CQDialog>				Win32DoubleBufferQDialogWidge;
	typedef Win32DoubleBufferFormCWndWidge<CMDIFrameWnd>	Win32DoubleBufferMDIFrameWndWidge;
	typedef Win32DoubleBufferFormCWndWidge<CControlBar>			Win32DoubleBufferControlBarWidge;

#endif //WIN32
}
#endif //#ifndef WIN32FrameworkWidge_header_
