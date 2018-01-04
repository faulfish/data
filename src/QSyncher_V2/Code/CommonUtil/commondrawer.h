/*********************************************************************************\
  FileName: CommonDrawer.h

  Descript: A wapper class for CommonDrawer

  Creater: Alan tu

  Create Date:2005/5/16
\*********************************************************************************/

#if !defined(_INC_COMMONDRAWERL_)
#define _INC_COMMONDRAWERL_

#include <afxwin.h>
#include "CommonUtility.h"

namespace COMMONCONTROL
{	
	using namespace Gdiplus;
	
	//memoryBuffer
	class _MemoryBuffer
	{
	public:
		_MemoryBuffer():m_hCompatibleBitmap(NULL),m_hCompatibleDC(NULL){}
		~_MemoryBuffer(){releaseBuffer();}
		HDC beginDraw(HDC hDC,int nX,int nY,int nWidth,int nHeight)	{
			if(m_hCompatibleDC == NULL)
			{
				if(nX != m_nOriginalX || m_nOriginalY != nY || m_nOriginalWidth != nWidth || m_nOriginalHeight != nHeight){
					releaseBuffer();
					
					m_nOriginalX = nX;
					m_nOriginalY = nY;
					m_nOriginalWidth = nWidth;
					m_nOriginalHeight = nHeight;
					m_OriginalhDC = hDC;
					m_hCompatibleDC = ::CreateCompatibleDC(m_OriginalhDC);
					m_hCompatibleBitmap = ::CreateCompatibleBitmap(m_OriginalhDC,m_nOriginalWidth,m_nOriginalHeight);
					m_hOldBmp = (HBITMAP)::SelectObject ( m_hCompatibleDC, m_hCompatibleBitmap );
					
					//Give orginal background
					::BitBlt(m_hCompatibleDC, 
							0,0,
							m_nOriginalWidth, m_nOriginalHeight, 
							m_OriginalhDC, 
							m_nOriginalX,m_nOriginalY,
							SRCCOPY);
				}
			}
			return m_hCompatibleDC;
		}
		HDC draw(HDC hDC,int nX,int nY,int nWidth,int nHeight)	{
			m_hCompatibleDC = beginDraw(hDC,nX,nY,nWidth,nHeight);
			
			::BitBlt(hDC, 
                m_nOriginalX,m_nOriginalY,m_nOriginalWidth,m_nOriginalHeight,
                m_hCompatibleDC, 
                0,0,
                SRCCOPY);

			return m_hCompatibleDC;
		}
		const CRect translateRect(const CRect& rect){return CRect(0,0,m_nOriginalWidth,m_nOriginalHeight);}
	private:
		void releaseBuffer()	{
			if(m_hCompatibleBitmap) {
				::SelectObject (m_hCompatibleDC,m_hOldBmp);
				::DeleteObject(m_hCompatibleBitmap);
				::DeleteDC(m_hCompatibleDC);
				m_hCompatibleDC = NULL;
			}
		}
	private:
		HDC m_hCompatibleDC;
		HDC m_OriginalhDC;
		HBITMAP m_hCompatibleBitmap;
		HBITMAP	m_hOldBmp;
		int m_nOriginalX;
		int m_nOriginalY;
		int m_nOriginalWidth;
		int m_nOriginalHeight;
	};

	//Drawer policy
	class _SingleBufferPolicy
	{
	public:
		//only CommonUIControl can use it
		_SingleBufferPolicy():m_hInvaildRgn(NULL),m_hrgn(NULL){}
		~_SingleBufferPolicy(){EndHDC();}
		HDC BeginHDC(HDC hDC,const CRect& reArea,HRGN hInvaildRgn = NULL)	
		{
			m_hInvaildRgn = hInvaildRgn;
			return BeginHDC(hDC,reArea.left,reArea.top,reArea.Width(),reArea.Height());
		}
		HDC BeginHDC(HDC hDC,int nX,int nY,int nWidth,int nHeight)
		{
			m_OriginalhDC = hDC;
			if(m_hInvaildRgn == NULL)
			{
				m_hrgn = ::CreateRectRgn(nX,nY,nX+nWidth,nY+nHeight);
				m_hInvaildRgn = m_hrgn;
			}

			//Set clip
			if(m_hInvaildRgn)
				::SelectClipRgn(m_OriginalhDC,m_hInvaildRgn);

			return m_OriginalhDC;
		}
		const CRect translateRect(const CRect& rect){return rect;}
	private:
		void EndHDC()
		{
			//For recover clip
			::SelectClipRgn(m_OriginalhDC,NULL);

			//Garbage recycle
			if(m_hrgn)
			{
				::DeleteObject(m_hrgn);
				m_hrgn = NULL;
			}
		}
	private:
		HDC m_OriginalhDC;
		HRGN m_hrgn;
		HRGN m_hInvaildRgn;
	};

	//Drawer policy
	class _DoubleBufferPolicy
	{
	public:
		//only CommonUIControl can use it
		_DoubleBufferPolicy():m_hCompatibleBitmap(NULL),m_hCompatibleDC(NULL){}
		~_DoubleBufferPolicy(){EndHDC();}
		HDC BeginHDC(HDC hDC,const CRect& reArea,HRGN hInvaildRgn = NULL)
		{
			m_hInvaildRgn = hInvaildRgn;
			return BeginHDC(hDC,reArea.left,reArea.top,reArea.Width(),reArea.Height());
		}
		HDC BeginHDC(HDC hDC,int nX,int nY,int nWidth,int nHeight)	
		{
			if(m_hCompatibleDC == NULL)
			{
				m_nOriginalX = nX;
				m_nOriginalY = nY;
				m_nOriginalWidth = nWidth;
				m_nOriginalHeight = nHeight;
				m_OriginalhDC = hDC;
				m_hCompatibleDC = ::CreateCompatibleDC(m_OriginalhDC);
				m_hCompatibleBitmap = ::CreateCompatibleBitmap(m_OriginalhDC,m_nOriginalWidth,m_nOriginalHeight);
				m_hOldBmp = (HBITMAP)::SelectObject ( m_hCompatibleDC, m_hCompatibleBitmap );				
			}			
			return m_hCompatibleDC;
		}
		const CRect translateRect(const CRect& rect){return CRect(0,0,m_nOriginalWidth,m_nOriginalHeight);}
	private:
		void EndHDC()
		{
			//Set clip
			if(m_hInvaildRgn)
				::SelectClipRgn(m_OriginalhDC,m_hInvaildRgn);

			::BitBlt(m_OriginalhDC, 
                    m_nOriginalX,m_nOriginalY, 
                    m_nOriginalWidth, m_nOriginalHeight, 
                    m_hCompatibleDC, 
                    0,0, 
                    SRCCOPY);

			//Recover clip
			if(m_hInvaildRgn)
				::SelectClipRgn(m_OriginalhDC,NULL);

			::SelectObject (m_hCompatibleDC,m_hOldBmp);
			::DeleteObject(m_hCompatibleBitmap);
			::DeleteDC(m_hCompatibleDC);
		}		
	private:
		HDC m_hCompatibleDC;
		HDC m_OriginalhDC;
		HBITMAP m_hCompatibleBitmap;
		HBITMAP	m_hOldBmp;
		int m_nOriginalX;
		int m_nOriginalY;
		int m_nOriginalWidth;
		int m_nOriginalHeight;
		HRGN m_hInvaildRgn;
	};

	template <class Drawer>
	class Painter
	{
	public:
		Painter(CWnd* pWnd,HDC hDC,int nX,int nY,int nWidth,int nHeight) {			
			m_hDC = m_Drawer.BeginHDC(hDC,nX,nY,nWidth,nHeight);
			drawTransparent(pWnd,m_hDC);
		}
		Painter(CWnd* pWnd):m_PaintDC(pWnd) {			
			if(::IsWindow(pWnd->GetSafeHwnd()))	{
				CRect rect;
				pWnd->GetClientRect(&rect);
				m_hDC = m_Drawer.BeginHDC(m_PaintDC.GetSafeHdc(),rect);
				drawTransparent(pWnd,m_hDC);
			}
		}
		inline HDC operator()() {
			return m_hDC;
		}
	protected:
		void drawTransparent(CWnd* pWnd,HDC hDC) {
			static _MemoryBuffer m_TransparentBuffer;
			if(::IsWindow(pWnd->GetSafeHwnd())) {
				CRect rcClient;
				pWnd->GetClientRect(&rcClient);
				m_TransparentBuffer.draw(hDC,rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height());
			}
		}
	private:
		CPaintDC m_PaintDC;
		Drawer m_Drawer;
		HDC m_hDC;
	};

	typedef Painter<_DoubleBufferPolicy> dPainter;
	typedef Painter<_SingleBufferPolicy> sPainter;
};

#endif //_INC_COMMONDRAWERL_