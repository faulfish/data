#ifndef _TWidge_Header_
#define _TWidge_Header_

#include <vector>
namespace QSYNC_LIB
{
#pragma once

	class GeometryRect
	{
	public:
		GeometryRect():m_nX(0),m_nY(0),m_nWidth(0),m_nHeight(0){}
		~GeometryRect(){}
		int m_nX;
		int m_nY;
		int m_nWidth;
		int m_nHeight;
	};

	interface IWidge
	{		
		IWidge(){}
		virtual ~IWidge(){}
		virtual void SetMinWidth(int nValue) = 0;
		virtual void SetMinHeight(int nValue) = 0;
		virtual void SetMaxWidth(int nValue) = 0;
		virtual void SetMaxHeight(int nValue) = 0;
		virtual void SetGeometry(int nX,int nY,int nWidth,int nHeight) = 0;
		virtual void SetGeometry(const GeometryRect& rect) = 0;
		virtual const GeometryRect& GetGeometry() const = 0;
		virtual int GetMinWidth() const = 0;
		virtual int GetMinHeight() const = 0;
		virtual int GetMaxWidth() const = 0;
		virtual int GetMaxHeight() const = 0;
		virtual bool SetLayout(IWidge* pIWidge = 0) = 0;
		virtual IWidge* GetLayout() = 0;
		virtual bool IsLayoutManager() = 0;
		virtual bool IsWindow() = 0;
		virtual void Update(bool bErase = false) = 0;
		virtual void AddValidateRect(const GeometryRect& rect) = 0;
		virtual void SetVaildRect() = 0;
		virtual bool GetValidateRect(GeometryRect &rect) = 0;
	};

	class WidgeBase : public IWidge
	{
	protected:
		WidgeBase():
				 m_pIWidgeLayout(0),
				 m_nMinWidth(-1),
				 m_nMinHeight(-1),
				 m_nMaxWidth(-1),
				 m_nMaxHeight(-1){}
			 virtual ~WidgeBase()
			 {
				 if(m_pIWidgeLayout)
				 {
					 delete m_pIWidgeLayout;
					 m_pIWidgeLayout = NULL;
				 }
			 }
	public:
		virtual void SetGeometry(const GeometryRect& rect)
		{
			SetGeometry(rect.m_nX,rect.m_nY,rect.m_nWidth,rect.m_nHeight);
		}
		virtual void SetGeometry(int nX,int nY,int nWidth,int nHeight)
		{
			m_Rect.m_nX = nX;
			m_Rect.m_nY = nY;
			m_Rect.m_nWidth = nWidth;
			m_Rect.m_nHeight = nHeight;
		}
		virtual void SetMinWidth(int nValue){m_nMinWidth = nValue;}
		virtual void SetMinHeight(int nValue){m_nMinHeight = nValue;}
		virtual void SetMaxWidth(int nValue){m_nMaxWidth = nValue;}
		virtual void SetMaxHeight(int nValue){m_nMaxHeight = nValue;}
		virtual const GeometryRect& GetGeometry() const {return m_Rect;}
		virtual int GetMinWidth() const{return m_nMinWidth;}
		virtual int GetMinHeight() const{return m_nMinHeight;}
		virtual int GetMaxWidth() const{return m_nMaxWidth;}
		virtual int GetMaxHeight() const{return m_nMaxHeight;}
		virtual bool SetLayout(IWidge* pIWidge = 0)
		{
			bool bRes = true;
			if(pIWidge && pIWidge->IsLayoutManager())
			{				
				if(m_pIWidgeLayout)
					delete m_pIWidgeLayout;
				m_pIWidgeLayout = pIWidge;
			}
			if(m_pIWidgeLayout)
			{				
				m_pIWidgeLayout->SetGeometry(GetGeometry());
				bRes = m_pIWidgeLayout->SetLayout();
			}
			Update(false);
			return bRes;
		}
		virtual IWidge* GetLayout(){ return m_pIWidgeLayout;}
		virtual void AddValidateRect(const GeometryRect& rect)
		{
			m_VaildRects.push_back(rect);
		}
		virtual bool GetValidateRect(GeometryRect &rect)
		{
			bool bRes = false;
			if(m_VaildRects.size() > 0)
			{
				bRes =  true;
				rect = m_VaildRects.back();
				m_VaildRects.pop_back( );
			}
			if(bRes == false && m_pIWidgeLayout)
			{
				bRes = m_pIWidgeLayout->GetValidateRect(rect);
			}
			return bRes;
		}
	private:
		GeometryRect								m_Rect;
		int													m_nMinWidth;
		int													m_nMinHeight;
		int													m_nMaxWidth;
		int													m_nMaxHeight;
		IWidge*											m_pIWidgeLayout;
		std::vector<GeometryRect>			m_VaildRects;
	};

	class LayoutBase : public WidgeBase
	{
	public:		
		LayoutBase(){}
		virtual ~LayoutBase(){}
		virtual bool IsLayoutManager(){return true;}
		virtual bool IsWindow(){return false;}		
		virtual void SetVaildRect(){}
		virtual void Update(bool bErase = false){}
	};

	class WindowBase : public WidgeBase
	{
	public:		
		WindowBase(){}
		virtual ~WindowBase(){}
		virtual bool IsLayoutManager(){return false;}
		virtual bool IsWindow(){return true;}
	};

	/**********************************************************************************************************************/
	/*		TWidge for other class																																	*/
	/*																																												*/
	/*																																												*/
	/*																																												*/
	/*																																												*/
	/**********************************************************************************************************************/
#ifdef WIN32
	template <class BaseClass,class WindowClass,class IDType = UINT>
	class TWidge : public BaseClass, public WindowBase
	{
	public:
		TWidge(IDType ID,WindowClass* pParent):BaseClass(ID,pParent){}
		virtual ~TWidge(void){}
	public:		
		virtual bool IsLayoutManager(){return false;}
		virtual bool IsWindow(){return true;}			
		virtual void Update(bool bErase = false)
		{			
			Invalidate(bErase == true);
			SetVaildRect();			
			TRACE(_T("WindowClass::Invaild false\n"));
		}
		virtual void SetVaildRect()
		{
			GeometryRect rect;
			while(GetValidateRect(rect) == true)
			{
				CRect rcVaild(rect.m_nX,rect.m_nY,rect.m_nX + rect.m_nWidth,rect.m_nY + rect.m_nHeight);
				WindowClass::ValidateRect(rcVaild);
				TRACE(_T("VailRect %d %d %d %d"),rect.m_nX,rect.m_nY,rect.m_nWidth,rect.m_nHeight);
			}
			TRACE(_T("\n"));
		}
	};

	template <class WindowClass>
	class TAttachWidge : public WindowBase
	{
	public:
		TAttachWidge(WindowClass* pWindow):m_pWindow(pWindow){}
		virtual ~TAttachWidge(void){}
	public:		
		virtual bool IsLayoutManager(){return false;}
		virtual bool IsWindow(){return true;}			
		virtual void Update(bool bErase = false)
		{
			if(m_pWindow)
			{
				m_pWindow->Invalidate(bErase == true);
				TRACE(_T("m_pWindow->Invaild false\n"));
			}
		}
		virtual bool SetLayout(IWidge* pIWidge = 0)
		{
			if(m_pWindow)
			{				
				AddValidateRect(GetGeometry());
				//CRect rcVaild(GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nX + GetGeometry().m_nWidth,GetGeometry().m_nY + GetGeometry().m_nHeight);
				m_pWindow->MoveWindow(GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nWidth,GetGeometry().m_nHeight,FALSE);
				//m_pWindow->GetParent()->ValidateRect(rcVaild);
				TRACE(_T("MoveRect %d %d %d %d"),GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nWidth,GetGeometry().m_nHeight);
				Update(false);
			}
			return true;
		}		
		virtual void SetVaildRect()
		{
			if(m_pWindow)
			{
				GeometryRect rect;
				while(GetValidateRect(rect) == true)
				{
					CRect rcVaild(rect.m_nX,rect.m_nY,rect.m_nX + rect.m_nWidth,rect.m_nY + rect.m_nHeight);
					m_pWindow->ValidateRect(rcVaild);
				}
			}
		}
	private:
		WindowClass*								m_pWindow;
	};

	typedef TWidge<CDialog,CWnd> WIN32DialogWidge;
	typedef TAttachWidge<CWnd> WIN32WindowWidge;
#endif //WIN32
}

#endif  //#ifndef _TWidge_Header_