/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
*
*                               3G Project
*
*	             Copyright (c) 2005 BENQ Corpration
*
***************************************************************************
*
*  $Workfile:   TWidge.h  $
*  $Revision:   1.1  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Components/UIFramework/LayoutManager/TWidge.h.-arc  $
 * 
 *    Rev 1.1   Apr 01 2009 14:17:26   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:54:20   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.54   Mar 11 2008 14:47:28   Alan Tu
 * Add compare operator
 * 
 *    Rev 1.53   Feb 26 2008 08:43:40   Mei Yang
 * remove unused const string
 * 
 *    Rev 1.52   Feb 25 2008 22:02:54   Mei Yang
 * update Save Parameter
 * 
 *    Rev 1.51   Jan 31 2008 10:36:02   Mei Yang
 * add nID
 * 
 *    Rev 1.50   Jan 29 2008 09:00:38   Mei Yang
 * set Save and Load as pure virtual
 * 
 *    Rev 1.49   Dec 26 2007 18:19:48   Alan Tu
 * GetCaptureStatus
* 
*    Rev 1.48   Dec 24 2007 17:05:52   Jame Tsai
* change path
* 
*    Rev 1.47   Dec 20 2007 20:59:46   Jame Tsai
* add timer
* 
*    Rev 1.46   Dec 10 2007 19:06:54   Jame Tsai
* Remove Update of 1.45
* 
*    Rev 1.45   Dec 07 2007 19:02:08   Jame Tsai
* Update SetLayout
* 
*    Rev 1.44   Dec 05 2007 15:07:16   Jame Tsai
* Add two member functions
* 
*    Rev 1.43   Nov 30 2007 17:39:36   Mei Yang
* remove unused function
* 
*    Rev 1.42   Nov 30 2007 17:33:08   Mei Yang
* add Global Name
* 
*    Rev 1.41   Nov 30 2007 13:53:46   Alan Tu
* Add background 
* 
*    Rev 1.40   Nov 28 2007 20:54:52   Mei Yang
* update
* 
*    Rev 1.39   Nov 22 2007 20:13:10   Alan Tu
* update for notify click event
* 
*    Rev 1.38   Nov 22 2007 18:56:12   Alan Tu
* update for picture path
* 
*    Rev 1.36   Nov 20 2007 09:54:34   Mei Yang
* update Save(XMLMANAGER::XMLElement& Element)
* 
*    Rev 1.35   Nov 16 2007 16:56:34   Mei Yang
* update Parameters
* 
*    Rev 1.34   Nov 15 2007 17:48:20   Mei Yang
* add GetID/GetName/Save/Load
* 
*    Rev 1.33   Nov 14 2007 09:47:48   Mei Yang
* update
* 
*    Rev 1.32   Nov 14 2007 09:21:38   Mei Yang
* update
*
*
***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file TWidge.h
@brief {brief description of TWidge.h}

{detail description of TWidge.h}
*/
#ifndef _TWidge_Header_
#define _TWidge_Header_
#pragma once	
#include <vector>
#include <map>
#include "../../../CommonUtil/XMLManager.h"

namespace QSYNC_LIB
{
	const TSTRING g_strWidget = _T("Widget");
	const TSTRING g_strClsName = _T("ClassName");
	const TSTRING g_strProperty = _T("Property");
	const TSTRING g_strID = _T("ID");
	const TSTRING g_strDisplay = _T("Display");
	const TSTRING g_strWin32ID = _T("Win32ID");
	const TSTRING EVENT_BUTTON_CLICK = _T("EVENT_BUTTON_CLICK");

	class GeometryRect
	{
	public:
		GeometryRect(const GeometryRect &rect):m_nX(rect.m_nX),m_nY(rect.m_nY),m_nWidth(rect.m_nWidth),m_nHeight(rect.m_nHeight){}
		GeometryRect(int nX,int nY,int nWidth,int nHeight):m_nX(nX),m_nY(nY),m_nWidth(nWidth),m_nHeight(nHeight){}
		GeometryRect():m_nX(0),m_nY(0),m_nWidth(0),m_nHeight(0){}
		~GeometryRect(){}
		bool IsEmpty() const
		{
			return (m_nX == 0 && m_nY == 0 && m_nWidth == 0 && m_nHeight == 0);
		}
		bool operator!=(const GeometryRect& rhs) const
		{
			return !((rhs.m_nX == m_nX) && (rhs.m_nY == m_nY) && (rhs.m_nWidth == m_nWidth) && (rhs.m_nHeight == m_nHeight));
		}
		bool operator==(const GeometryRect& rhs) const
		{
			return (rhs.m_nX == m_nX) && (rhs.m_nY == m_nY) && (rhs.m_nWidth == m_nWidth) && (rhs.m_nHeight == m_nHeight);
		}
		void Clear() {	m_nX = m_nY = m_nWidth = m_nHeight = 0; }
		int m_nX;
		int m_nY;
		int m_nWidth;
		int m_nHeight;
	};

	class GeometryRectBase
	{
	public:
		GeometryRectBase():
		  m_nMinWidth(-1),
			  m_nMinHeight(-1),
			  m_nMaxWidth(-1),
			  m_nMaxHeight(-1){}
		  virtual ~GeometryRectBase(){}

		  /************************************************************************************/
		  //virtual bool SetGeometry(const GeometryRect& rcInput)
		  //
		  //Function :
		  //1 Test size is ok or not
		  //2 Always assign new size, if rcInput is fit of limitation size , the new one is rcInput or limitation size.
		  //
		  //Return value :
		  //If rcInput isn't fit of the widget's limitation size only when too small.
		  //
		  //Description :
		  //If rcInput isn't fit of it's limitation size only when too small, then the widget will be assigned the
		  //limitation and return false;
		  //If rcInput isn't fit of it's limitation size when too big en the widget will be assigned the limitation and return false;
		  //
		  //Example :
		  //GeometryRect GetFitSize(const GeometryRect& rcInput)
		  //{
		  //    SetGeometry(rcInput);
		  //    return SetGeometry(); //It will return the fit size
		  //}
		  //
		  /************************************************************************************/
		  void SetFixedWidth(int nWidth)
		  {
			  SetMinWidth(nWidth);
			  SetMaxWidth(nWidth);
		  }
		  void SetFixedHeight(int nHeight)
		  {
			  SetMinHeight(nHeight);
			  SetMaxHeight(nHeight);
		  }
		  virtual bool SetGeometry(const GeometryRect& rect)
		  {
			  /*
			  If SetGeometry return fail, it means it can't fit the size and it assign a new size automatically
			  */

			  bool bVaildMin = true;
			  bool bVaildMax = true;

			  int nNewX = rect.m_nX;
			  int nNewY = rect.m_nY;
			  int nNewW = rect.m_nWidth;
			  int nNewH = rect.m_nHeight;

			  if(GetMinWidth() != -1)
				  if(rect.m_nWidth < GetMinWidth())
				  {
					  nNewW = GetMinWidth();
					  bVaildMin = false;
				  }
				  if(GetMinHeight() != -1)
					  if(rect.m_nHeight < GetMinHeight())
					  {
						  nNewH = GetMinHeight();
						  bVaildMin = false;
					  }
					  if(GetMaxWidth() != -1)
						  if(rect.m_nWidth > GetMaxWidth())
						  {
							  nNewW = GetMaxWidth();
							  bVaildMax = false;
						  }
						  if(GetMaxHeight() != -1)
							  if(rect.m_nHeight > GetMaxHeight())
							  {
								  nNewH = GetMaxHeight();
								  bVaildMax = false;
							  }

							  m_Rect.m_nX = nNewX;
							  m_Rect.m_nY = nNewY;
							  m_Rect.m_nWidth = nNewW;
							  m_Rect.m_nHeight = nNewH;

							  return bVaildMin && bVaildMax;
		  }
		  virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
		  {
			  GeometryRect rect;
			  rect.m_nX = nX;
			  rect.m_nY = nY;
			  rect.m_nWidth = nWidth;
			  rect.m_nHeight = nHeight;
			  return SetGeometry(rect);
		  }
		  virtual const GeometryRect& GetGeometry() const {return m_Rect;}
		  virtual void SetMinWidth(int nValue)
		  {
			  m_nMinWidth = nValue;
			  if(m_nMaxWidth != -1 && nValue > m_nMaxWidth)
				  m_nMaxWidth = nValue;
		  }
		  virtual void SetMinHeight(int nValue)
		  {
			  m_nMinHeight = nValue;
			  if(m_nMaxHeight != -1 && nValue > m_nMaxHeight)
				  m_nMaxHeight = nValue;
		  }
		  virtual void SetMaxWidth(int nValue)
		  {
			  m_nMaxWidth = nValue;
			  if(m_nMinWidth != -1 && nValue < m_nMinWidth)
				  m_nMinWidth = nValue;
		  }
		  virtual void SetMaxHeight(int nValue)
		  {
			  m_nMaxHeight = nValue;
			  if(m_nMinHeight != -1 && nValue < m_nMinHeight)
				  m_nMinHeight = nValue;
		  }
		  virtual int GetMinWidth() const{return m_nMinWidth;}
		  virtual int GetMinHeight() const{return m_nMinHeight;}
		  virtual int GetMaxWidth() const{return m_nMaxWidth;}
		  virtual int GetMaxHeight() const{return m_nMaxHeight;}
		  virtual bool IsVaildRect(const GeometryRect& rect,int nCX,int nCY)
		  {
			  bool bRes = true;
			  if(nCX < rect.m_nX || nCX > (rect.m_nX + rect.m_nWidth))
				  bRes = false;
			  if(nCY < rect.m_nY || nCY > (rect.m_nY + rect.m_nHeight))
				  bRes = false;
			  return bRes;
		  }
	private:
		GeometryRect										m_Rect;
		int													m_nMinWidth;
		int													m_nMinHeight;
		int													m_nMaxWidth;
		int													m_nMaxHeight;
	};

	template <class GraphContext>
	class Widge : public GeometryRectBase
	{
	public:
		Widge()
			:m_pIWidgeLayout(NULL)
			,m_pIWidgetBackground(NULL)
			,m_bShow(true)
			,m_nID(0)
		{
		}
		virtual ~Widge()
		{
			if(m_pIWidgeLayout)
			{
				delete m_pIWidgeLayout;
				m_pIWidgeLayout = NULL;
			}

			if(m_pIWidgetBackground)
			{
				delete m_pIWidgetBackground;
				m_pIWidgetBackground = NULL;
			}
		}
	public:

		virtual Widge* GetBackground(Widge* pIWidge)
		{
			return m_pIWidgetBackground;
		}

		virtual void SetBackground(Widge* pIWidge)
		{
			if(m_pIWidgetBackground)
			{
				delete m_pIWidgetBackground;
				m_pIWidgetBackground = NULL;
			}
			m_pIWidgetBackground = pIWidge;
		}
		/************************************************************************************/
		//virtual bool SetLayout(Widget* pWidget = NULL)
		//
		//Function :
		//1. Assign a new layout widget if pWidget isn't NULL
		//2. Or change the size according GetGeometry() function
		//
		//Description :
		//Every time to change size, you should call SetGeometry(...) function and then call SetLayout(...)
		//
		//Example :
		//void Move(const GeometryRect& rcInput)
		//{
		//    SetGeometry(rcInput); //Don't care the return value in the case
		//    SetLayout();
		//}
		//
		/************************************************************************************/
		virtual bool SetLayout(Widge* pIWidge = 0)
		{
			bool bRes = true;

			if(m_pIWidgetBackground)
				m_pIWidgetBackground->Move(GetGeometry());

			if(pIWidge && pIWidge->IsLayoutManager())
			{
				if(m_pIWidgeLayout)
					delete m_pIWidgeLayout;
				m_pIWidgeLayout = pIWidge;
			}
			if(m_pIWidgeLayout /*&& pIWidge == NULL*/)
			{
				m_pIWidgeLayout->SetGeometry(GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nWidth,GetGeometry().m_nHeight);
				bRes = m_pIWidgeLayout->SetLayout();
				Update(false);
			}
			return bRes;
		}
		void Move(const GeometryRect& rcInput)
		{
			SetGeometry(rcInput); //Don't care the return value in the case
			SetLayout();
		}
		virtual Widge* GetLayout(){ return m_pIWidgeLayout;}
		virtual void SetValidateRect(const GeometryRect& rect)
		{
			m_VaildRect = rect;
		}
		virtual bool GetValidateRect(std::vector<GeometryRect> &rectVaildArray)
		{
			bool bRes = false;
			if(!m_VaildRect.IsEmpty())
			{
				bRes =  true;
				rectVaildArray.push_back(m_VaildRect);
			}
			if(bRes == false && m_pIWidgeLayout)
			{
				bRes = m_pIWidgeLayout->GetValidateRect(rectVaildArray);
			}
			return bRes;
		}
		virtual bool IsLayoutManager() = 0;
		virtual bool IsWindow() = 0;
		virtual void Update(bool bErase = false)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->Update(bErase);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->Update(bErase);
		}
		virtual void Draw(GraphContext GC,bool bErase = false)
		{//For window trigger event			
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->Draw(GC,bErase);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->Draw(GC,bErase);
		}
		virtual void Close()
		{//For window trigger event
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->Close();

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->Close();
		}
		virtual void MouseMove(int nCX,int nCY)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->MouseMove(nCX,nCY);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->MouseMove(nCX,nCY);
		}
		virtual void MouseUp(int nCX,int nCY)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->MouseUp(nCX,nCY);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->MouseUp(nCX,nCY);
		}
		virtual void MouseDown(int nCX,int nCY)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->MouseDown(nCX,nCY);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->MouseDown(nCX,nCY);
		}
		virtual void MouseDBClk(int nCX,int nCY)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->MouseDBClk(nCX,nCY);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->MouseDBClk(nCX,nCY);
		}
		virtual void Focus(bool bFocus)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->Focus(bFocus);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->Focus(bFocus);
		}
		virtual void Timer(WPARAM nTimerID)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->Timer(nTimerID);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->Timer(nTimerID);
		}
		virtual void CaptureMouse(bool bCapture)
		{
			if(m_pIWidgetBackground)
				m_pIWidgetBackground->CaptureMouse(bCapture);

			if(m_pIWidgeLayout)
				m_pIWidgeLayout->CaptureMouse(bCapture);
		}
		virtual void Show(bool bShow,bool bErase = false)
		{
			if(m_bShow != bShow)
			{
				m_bShow = bShow;

				if(m_pIWidgetBackground)
					m_pIWidgetBackground->Show(bShow,bErase);

				if(m_pIWidgeLayout)
					m_pIWidgeLayout->Show(bShow,bErase);
				Update(bErase);
			}
		}
		bool IsShow()const { return m_bShow;}


		virtual int GetID(){return m_nID;}
		virtual void SetID(int nID){m_nID= (UINT)nID;}
		virtual bool GetName(TSTRING& strName)
		{
			bool bRes= false;
			strName = g_strWidget;
			return bRes;
		}
		virtual bool Save(XMLMANAGER::XMLElement& Element, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase) = 0;
		virtual bool Load(const XMLMANAGER::XMLElement& Element) = 0;

		const TSTRING GetPictureFolder(const TSTRING& strImage)
		{
			return GetPictureFolder() + strImage;
		}

		const TSTRING& GetPictureFolder()
		{
			if(m_strPicturePath.empty())
			{
#ifdef _DEBUG
				m_strPicturePath = _T("..\\Debug\\Pictures2\\");
#else
				m_strPicturePath = _T("Pictures2\\");
#endif
			}
			return m_strPicturePath;
		}
		static bool IsCapture()
		{
			return GetCapture();
		}
		static void Capture(bool bCapture)
		{
			GetCapture() = bCapture;
		}
	private:
		static bool &GetCapture()
		{
			static bool	m_bCapture = false;
			return m_bCapture;
		}

	private:
		UINT									m_nID;
		Widge*									m_pIWidgeLayout;
		Widge*									m_pIWidgetBackground;
		GeometryRect							m_VaildRect;
		TSTRING									m_strPicturePath;
		bool									m_bShow;		
	};
}

#endif  //#ifndef _TWidge_Header_
