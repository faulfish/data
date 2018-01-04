#pragma once

#include "..\..\..\code\Components\UIFramework\porting\SubMenuBase.h"

namespace QSYNC_LIB 
{
	class ResolutionSubmenu : public SubMenuBase
	{	
	public:
		ResolutionSubmenu()
			:m_nCurrentResolution(0)
		{
			//要增加或修改Resolution
			//只要改這裡的m_vecResolution就好了,其他相關設定都會來讀這個vector
			/*
			QPoint Resolution0 = {144,176};
			m_vecResolution.push_back(Resolution0);
			QPoint Resolution1 = {240,320};
			m_vecResolution.push_back(Resolution1);
			QPoint Resolution2 = {1024,1280};
			m_vecResolution.push_back(Resolution2);
			QPoint Resolution3 = {1200,1600};
			m_vecResolution.push_back(Resolution3);
			*/
			QPoint Resolution4 = {176,144};
			m_vecResolution.push_back(Resolution4);
			QPoint Resolution5 = {320,240};
			m_vecResolution.push_back(Resolution5);
			QPoint Resolution_640_480 = {640,480};
			m_vecResolution.push_back(Resolution_640_480);
			QPoint Resolution6 = {1280,1024};
			m_vecResolution.push_back(Resolution6);
			QPoint Resolution7 = {1600,1200};
			m_vecResolution.push_back(Resolution7);

			for(int i = 0; i != m_vecResolution.size(); ++i)
			{
				QPoint tmpResolution = m_vecResolution.at(i);
				CString strResolution;
				strResolution.Format(_T("%d*%d"),tmpResolution.m_nPosX,tmpResolution.m_nPosY);
				CString strNotify;
				strNotify.Format(_T("Resolution_%d"),i);

				SubMenuItem ResolutionItem = {(TSTRING)strResolution,(TSTRING)strNotify};
				AddItem(ResolutionItem);
			}
		}
		virtual ~ResolutionSubmenu(void){}
		virtual bool IsChecked(int nIndex)
		{
			return nIndex == m_nCurrentResolution;
		}
		QPoint GetResolutionByIndex(int nIndex) const
		{
			return m_vecResolution.at(nIndex);
		}
		QPoint GetCurrentResolution() const
		{
			return m_vecResolution.at(m_nCurrentResolution);
		}
		CString GetCurrentResolutionByString() const
		{
			CString strResolution;
			if(m_vecResolution.size() >= 0)
			{
				int nSafeResolutionIndex = m_nCurrentResolution;
				if((int)m_vecResolution.size() < nSafeResolutionIndex)
					nSafeResolutionIndex = 0;
				QPoint tmpResolution = m_vecResolution.at(nSafeResolutionIndex);
				strResolution.Format(_T("%d*%d"),tmpResolution.m_nPosX,tmpResolution.m_nPosY);
			}
			return strResolution;
		}

	public:
		int m_nCurrentResolution;
	private:
		std::vector<QPoint> m_vecResolution;
	};
}
