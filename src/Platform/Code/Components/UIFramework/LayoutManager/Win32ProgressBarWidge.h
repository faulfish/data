
#ifndef TWin32ProgressBarWidge_header_
#define TWin32ProgressBarWidge_header_

#include "win32widge.h"

#pragma once

namespace QSYNC_LIB
{
#ifdef WIN32
	class _DoProgressBar
	{
	public:
		_DoProgressBar():m_nStyle(ClipPicture),m_nPercent(0),m_pBkImage(NULL),m_pBarImage(NULL),m_uElapse(100),m_nPictureOnDrawNo(0){}
		_DoProgressBar(Win32WindowBase* pControl, INotify* pINotify):m_nStyle(ClipPicture),m_nPercent(0),m_pBkImage(NULL),m_pBarImage(NULL),m_uElapse(100),m_nPictureOnDrawNo(0){}
		virtual ~_DoProgressBar(){}

		virtual void implementMouseDown(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseUp(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementMouseMove(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){}
		virtual bool implementMouseDoubleClick(Win32WindowBase* pControl,UINT nFlags,int nCX,int nCY){return false;}
		virtual void implementFocus(Win32WindowBase* pControl,bool bFocused){}
		virtual void implementDraw(Win32WindowBase* pControl,HDC GC,bool bErase = false)
		{
			switch (m_nStyle)
			{
			case ClipPicture:
				DrawForClipPicture(pControl,GC);
				break;
			case AnimatePicture:
				DrawForAnimatePicture(pControl,GC);
				break;
			default:
				DrawForClipPicture(pControl,GC);
				break;
			}
		}
		virtual void Timer(Win32WindowBase* pControl,WPARAM nTimerID)
		{
			if(nTimerID == m_nTimerID)  //別人的計時器不要管
			{
				m_nPictureOnDrawNo += 1;
				if(m_nPictureOnDrawNo >= static_cast<int>(m_vectAnimatePicture.size()))
					m_nPictureOnDrawNo = 0;
				pControl->Update(false);
			}
		}
		virtual void implementClose(Win32WindowBase* pControl)//為何要給參數?
		{
			if(m_pBkImage != NULL)
			{
				delete m_pBkImage;
				m_pBkImage = NULL;
			}
			if(m_pBarImage != NULL)
			{
				delete m_pBarImage;
				m_pBarImage = NULL;
			}

			for(std::vector< Image* >::iterator iter = m_vectAnimatePicture.begin(); iter<m_vectAnimatePicture.end(); ++iter)
			{
				if((*iter))
					delete (*iter);
				(*iter) = NULL;
			}

		}

		virtual bool Save(XMLMANAGER::XMLElement& Element, TSTRING& strID){return true;}
		virtual bool Load(const XMLMANAGER::XMLElement& Element){return true;}

	public:
		std::vector< TSTRING >* GetPicturePaths(){return &m_vectAnimatePicturePath;}
		void SetStyle(ProgressBarStyle nStyle)
		{
			m_nStyle = nStyle;
		}
		//Member function for ProgressBarStyle ClipPicture:
		void SetPercentAndUpdate(Win32WindowBase* pControl,int nPercent)
		{
			if(nPercent>=0 && nPercent <= 100)
				m_nPercent = nPercent;
			else if(nPercent < 0)
				m_nPercent = 0;
			else if(nPercent > 100)
				m_nPercent = 100;
			pControl->Update(false);
		}
		void SetBkImage(Win32WindowBase* pControl,const TSTRING& strBkImagePath)
		{
			if(m_pBkImage != NULL)
			{
				delete m_pBkImage;
				m_pBkImage = NULL;
			}
			m_pBkImage = new Image(pControl->GetPictureFolder(strBkImagePath).c_str());
			UINT nImageW = m_pBkImage->GetWidth();
			UINT nImageH = m_pBkImage->GetHeight();
			pControl->SetMinWidth(nImageW);
			pControl->SetMaxWidth(nImageW);
			pControl->SetMinHeight(nImageH);
			pControl->SetMaxHeight(nImageH);
		}
		void SetBarImage(Win32WindowBase* pControl,const TSTRING& strBarImagePath)
		{
			if(m_pBarImage != NULL)
			{
				delete m_pBarImage;
				m_pBarImage = NULL;
			}
			m_pBarImage = new Image(pControl->GetPictureFolder(strBarImagePath).c_str());
		}
		//Member function for ProgressBarStyle AnimatePicture:
		void SetTimer(UINT nTimerID,UINT uElapse)
		{
			m_nTimerID = nTimerID;
			m_uElapse = uElapse;
		}
		void AddAnimatePicture(Win32WindowBase* pControl,const TSTRING& strAnimatePicture)
		{
			if(!strAnimatePicture.empty())
			{
				Image* pAnimateImage = new Image(pControl->GetPictureFolder(strAnimatePicture).c_str());
				m_vectAnimatePicturePath.push_back(strAnimatePicture);
				m_vectAnimatePicture.push_back(pAnimateImage);
				if(m_vectAnimatePicture.size() == 1)
				{
					UINT nImageW = pAnimateImage->GetWidth();
					UINT nImageH = pAnimateImage->GetHeight();

					pControl->SetMinWidth(nImageW);
					pControl->SetMaxWidth(nImageW);
					pControl->SetMinHeight(nImageH);
					pControl->SetMaxHeight(nImageH);
				}
			}

		}
		void AnimateStart(CWnd* pWindow)
		{
			if(pWindow)
				pWindow->SetTimer(m_nTimerID,m_uElapse,NULL);
		}
		void AnimateStop(CWnd* pWindow)
		{
			if(pWindow)
				pWindow->KillTimer(m_nTimerID);
		}


	private:
		void DrawForClipPicture(Win32WindowBase* pControl,HDC GC)
		{
			GeometryRect rect = pControl->GetGeometry();
			Graphics graphics(GC);

			if( pControl && m_pBkImage && m_pBarImage )
			{
				Rect rcBKArea(rect.m_nX,rect.m_nY, rect.m_nWidth,rect.m_nHeight);
				graphics.SetClip(rcBKArea);
				UINT nBkWidth = m_pBkImage->GetWidth();
				UINT nBkHeight = m_pBkImage->GetHeight();
				graphics.DrawImage(m_pBkImage,rcBKArea.X,rcBKArea.Y,nBkWidth, nBkHeight);

				Rect rcBarArea(rect.m_nX,rect.m_nY, rect.m_nWidth * m_nPercent / 100,rect.m_nHeight);
				graphics.SetClip(rcBarArea);
				UINT nBarWidth = m_pBarImage->GetWidth();
				UINT nBarHeight = m_pBarImage->GetHeight();
				int nBarX = rcBarArea.X + (nBkWidth - nBarWidth)/2;
				int nBarY = rcBarArea.Y + (nBkHeight - nBarHeight)/2;

				graphics.DrawImage(m_pBarImage,nBarX,nBarY, nBarWidth , nBarHeight);
			}
		}
		void DrawForAnimatePicture(Win32WindowBase* pControl,HDC GC)
		{
			GeometryRect rect = pControl->GetGeometry();
			Graphics graphics(GC);

			Rect rcArea(rect.m_nX,rect.m_nY, rect.m_nWidth,rect.m_nHeight);
			graphics.SetClip(rcArea);
			if( pControl && !m_vectAnimatePicture.empty())
			{
				std::vector< Image* >::iterator iterPicture = m_vectAnimatePicture.begin() + m_nPictureOnDrawNo;
				UINT nPictureWidth = (*iterPicture)->GetWidth();
				UINT nPictureHeight = (*iterPicture)->GetHeight();
				graphics.DrawImage( *iterPicture , rect.m_nX , rect.m_nY , nPictureWidth , nPictureHeight );
			}
		}

	private:
		ProgressBarStyle m_nStyle;

		//Member data for ProgressBarStyle ClipPicture:
		int m_nPercent;

		Image*  m_pBkImage;
		Image*  m_pBarImage;

		//Member data for ProgressBarStyle AnimatePicture:
		UINT m_nTimerID;
		UINT m_uElapse;    //Timer的時間間隔,單位為毫秒
		std::vector< Image* >         m_vectAnimatePicture;
		std::vector< TSTRING >        m_vectAnimatePicturePath;
		int m_nPictureOnDrawNo;

	};

	template <class GeneralPolicy,class ActionPolicy = _DoProgressBar>
	class TWin32ProgressBarWidge : public TWin32WindowWidge<GeneralPolicy,ActionPolicy>
	{
	public:
		TWin32ProgressBarWidge(CWnd* pWindow, INotify* pINotify):
		  TWin32WindowWidge(pWindow,pINotify)
		  ,m_strClassName(_T("Win32ProgressBar"))
		  ,m_nStyle(ClipPicture)
		  ,m_nTimerID(0)
		  ,m_nElapse(0)
		{
			SetAction(new ActionPolicy);
		}
		TWin32ProgressBarWidge(CWnd* pWindow,ProgressBarStyle nStyle):
			TWin32WindowWidge(pWindow,NULL)
			,m_strClassName(_T("Win32ProgressBar"))
			,m_nStyle(ClipPicture)
			,m_nTimerID(0)
			,m_nElapse(0)
		{
			SetAction(new ActionPolicy);
			SetStyle(nStyle);
		}
		//Constructor for ProgressBarStyle ClipPicture:
		TWin32ProgressBarWidge(CWnd* pWindow,TSTRING& strBkImagePath,TSTRING& strBarImagePath):
			TWin32WindowWidge(pWindow,NULL)
			,m_strClassName(_T("Win32ProgressBar"))
			,m_nStyle(ClipPicture)
			,m_nTimerID(0)
			,m_nElapse(0)
		{
			SetAction(new ActionPolicy);
			SetStyle(ClipPicture);
			SetClipPictureImage(strBkImagePath,strBarImagePath);
		}
		virtual ~TWin32ProgressBarWidge()
		{
		}
		virtual void Timer(WPARAM nTimerID)
		{
			if(GetActionPolicy())
				GetActionPolicy()->Timer(this,nTimerID);
		}
	public:
		virtual bool Save(XMLMANAGER::XMLElement& MainElement, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase)
		{
			using namespace XMLMANAGER;
			bool bRes = true;
			XMLElement EleWidget(g_strWidget);
			EleWidget.addAttribute(g_strClsName, m_strClassName);
			TSTRING strID;
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			EleWidget.addAttribute(g_strID, strID);
			FillOrderSetting(EleOrder, strID);

			XMLElement Property(g_strProperty);
			if(m_nStyle == ClipPicture)
				Property.addAttribute(_T("Style"), _T("ClipPicture"));
			else if(m_nStyle == AnimatePicture)
				Property.addAttribute(_T("Style"), _T("AnimatePicture"));
			Property.addAttribute(_T("BkImage"), m_strBkImagePath);
			Property.addAttribute(_T("BarImage"),m_strBarImagePath);
			Property.addAttribute(_T("TimerID"), (int)m_nTimerID);
			Property.addAttribute(_T("Elapse"), (int)m_nElapse);
			vector<TSTRING>* pVectPicPaths = GetActionPolicy()->GetPicturePaths();
			Property.addAttribute(_T("Count"), (int)pVectPicPaths->size());
			int nCount = 1;
			for(vector<TSTRING>::iterator iter= pVectPicPaths->begin(); iter!=pVectPicPaths->end(); ++iter )
			{
				TSTRING strImage = _T("Image");
				TSTRING strCount;
				iToTStr(nCount, strCount);
				strImage += strCount;
				Property.addAttribute(strImage, (*iter));
				nCount++;
			}

			EleWidget.getAllElement()->push_back(Property);

			Win32WindowBase::Save(EleWidget);
			MainElement.getAllElement()->push_back(EleWidget);
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes=true;
			TSTRING strStyle;
			Element.getAttr(_T("Style"),strStyle);
			if(strStyle == _T("ClipPicture"))
				SetStyle(ClipPicture);
			else if(strStyle == _T("AnimatePicture"))
				SetStyle(AnimatePicture);
			else 
				SetStyle(ClipPicture);

			TSTRING strBkImage;
			Element.getAttr(_T("BkImage"), strBkImage);
			if(!strBkImage.empty())
				SetBkImage(strBkImage);
			TSTRING strBarImage;
			Element.getAttr(_T("BarImage"), strBarImage);
			if(!strBarImage.empty())
				SetBarImage(strBarImage);
			
			int nTimerID = Element.getAttr(_T("TimerID"));
			nTimerID = nTimerID==-1 ? 0 : nTimerID;
			int nElapse =  Element.getAttr(_T("Elapse"));
			nElapse = nElapse==-1 ? 100 : nElapse;
			SetTimer(nTimerID,nElapse);

			int nCount = Element.getAttr(_T("Count"));
			for(int i=1;i<nCount+1; ++i)
			{
				TSTRING strCount;
				iToTStr(i, strCount);
				TSTRING strImage = _T("Image")+ strCount;
				TSTRING strImagePath;
				Element.getAttr(strImage, strImagePath);
				AddAnimatePicture(strImagePath);
			}
			

			return bRes;
		}
		void SetStyle(ProgressBarStyle nStyle)
		{
			m_nStyle = nStyle;
			if(GetActionPolicy())
				GetActionPolicy()->SetStyle(nStyle);
		}

		//Member function for ProgressBarStyle ClipPicture:
		void UpdatePercent(int nPercent)
		{
			if(GetActionPolicy())
				GetActionPolicy()->SetPercentAndUpdate(this,nPercent);
		}
		void SetClipPictureImage(const TSTRING& strBkImagePath,const TSTRING& strBarImagePath)
		{
			SetBkImage(strBkImagePath);
			SetBarImage(strBarImagePath);
		}
		void SetBkImage(const TSTRING& strBkImagePath)
		{
			m_strBkImagePath = strBkImagePath;
			if(GetActionPolicy() && !strBkImagePath.empty())
				GetActionPolicy()->SetBkImage(this,strBkImagePath);
		}
		void SetBarImage(const TSTRING& strBarImagePath)
		{
			m_strBarImagePath = strBarImagePath;
			if(GetActionPolicy() && !strBarImagePath.empty())
				GetActionPolicy()->SetBarImage(this,strBarImagePath);
		}

		//Member function for ProgressBarStyle AnimatePicture:
		void SetTimer(UINT nTimerID,UINT uElapse)//uElapse代表多久換一張畫面, 單位為毫秒
		{
			m_nTimerID = nTimerID;
			m_nElapse = uElapse;
			if(GetActionPolicy())
				GetActionPolicy()->SetTimer(nTimerID,uElapse);
		}
		void AddAnimatePicture(const TSTRING& strAnimatePicture)
		{
			if(GetActionPolicy() && !strAnimatePicture.empty())
				GetActionPolicy()->AddAnimatePicture(this,strAnimatePicture);
		}
		void AnimateStart(CWnd* pWindow)
		{
			if(GetActionPolicy())
				GetActionPolicy()->AnimateStart(pWindow);
		}
		void AnimateStop(CWnd* pWindow)
		{
			if(GetActionPolicy())
				GetActionPolicy()->AnimateStop(pWindow);
		}
	private:
		int GetID()
		{
			static int nID=-1;
			nID++;
			return nID;
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
		UINT    m_nTimerID;
		UINT	m_nElapse;
		TSTRING	m_strBkImagePath;
		TSTRING m_strBarImagePath;
		int		m_nStyle;
		TSTRING	m_strClassName;

	};
	typedef TWin32ProgressBarWidge<ButtonPolicy,_DoProgressBar>     Win32ProgressBarWidge;

#endif //WIN32

}

#endif //TWin32ProgressBarWidge_header_