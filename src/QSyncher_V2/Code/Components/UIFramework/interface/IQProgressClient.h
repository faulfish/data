/*********************************************************************/
/** @file:	IQProgressClient.h

Defines the IQProgressClient interface.

$Original Author:   Steven CC Chen  $
$Version:	 $
$Date:   Jan 04 2008 19:29:58  $
*/
/*********************************************************************/

/*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/UIFramework/interface/IQProgressClient.h.-arc  $
 * 
 *    Rev 1.5   Jan 04 2008 19:29:58   Mei Yang
 * add animate
 * 
 *    Rev 1.4   Dec 25 2007 15:59:06   Alan Tu
 * update
 * 
 *    Rev 1.3   Dec 20 2007 18:41:46   Alan Tu
 * add get hwnd function
* 
*    Rev 1.2   Apr 16 2007 18:11:04   Alan Tu
* update
* 
*    Rev 1.1   Mar 05 2007 22:22:38   steven cc chen
* Fixed path: "..\\..\\..\\CommonUtil\\NotifyBase.h"
* 
*    Rev 1.0   Mar 05 2007 21:59:26   steven cc chen
* Initial revision.
*/

#pragma once
#include "..\..\..\CommonUtil\NotifyBase.h"
#include "QControlNotify.h"


namespace QSYNC_LIB
{

	typedef struct tagQProgressNotify
	{
		CString			m_strCurrentDisplayText;
		int					m_nCurrentPosition;
	} QProgressNotify;


	interface IQProgressClient
	{
		// Progress UI Manipulation
		virtual CString		GetTitle(void) = 0;
		virtual CString		GetDisplayText(void) = 0;
		virtual CString		GetCancelButtonCaption(void) = 0;
		virtual int					GetProgressMinPos(void) = 0;
		virtual int					GetProgressMaxPos(void) = 0;
		virtual CString		GetProgressCaption(void) = 0;
		virtual CString		GetProgressCaptionPercentChar(void) = 0;
		virtual BOOL			GetAutoClose(void) = 0;
		virtual BOOL			GetCancelable(void) = 0;
		virtual BOOL			IsModalless() const = 0;
		virtual void		SetIsAnimate(bool bAnimate)=0;
		virtual bool		GetIsAnimate() =0 ;

		// Progress Task Manipulation
		virtual bool		StartTask(void) = 0;
		virtual bool		AbortTask(void) = 0;
		virtual bool		ResumeTask(void) = 0;
		virtual bool		SuspendTask(void) = 0;

		// Event Notification
		virtual bool		Register(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
		virtual bool		Register(INotify* pINotify, const TSTRING& strEvent) = 0;
		virtual bool		Unregister(INotify* pINotify, const TSTRING& strEvent) = 0;
		virtual bool		Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
	};

	class QProgressClientBase : public CTNotifyBase<IQProgressClient>
	{
	public:
		QProgressClientBase(): m_bIsAnimate(false)
		{
			DefineNotify(EVENT_PROGRESS_TASK_FINISH);
			DefineNotify(EVENT_PROGRESS_TASK_ABORT);
			DefineNotify(EVENT_PROGRESS_TASK_UPDATE);
			DefineNotify(EVENT_PROGRESS_TASK_GETHWMD);
		}
		virtual ~QProgressClientBase(){}
		// Progress UI Manipulation
		virtual CString		GetTitle(void) = 0;
		virtual CString		GetDisplayText(void) = 0;
		virtual CString		GetCancelButtonCaption(void) = 0;
		virtual int					GetProgressMinPos(void){return 0;}
		virtual int					GetProgressMaxPos(void){return 100;}
		virtual CString		GetProgressCaption(void) = 0;
		virtual CString		GetProgressCaptionPercentChar(void){return _T("%");}
		virtual BOOL			GetAutoClose(void){return TRUE;}
		virtual BOOL			GetCancelable(void){return FALSE;}
		virtual BOOL			IsModalless() const {return FALSE;}
		virtual void SetIsAnimate(bool bAnimate){m_bIsAnimate = bAnimate;}
		virtual bool GetIsAnimate(){return m_bIsAnimate;}

		// Progress Task Manipulation
		virtual bool		StartTask(void) = 0;
		virtual bool		AbortTask(void) = 0;
		virtual bool		ResumeTask(void) = 0;
		virtual bool		SuspendTask(void) = 0;

		// Event Notification
		virtual bool		Register(EventFunc* pEventFunc, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQProgressClient>::Register(pEventFunc,strEvent);
		}
		virtual bool		Register(INotify* pINotify, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQProgressClient>::Register(pINotify,strEvent);
		}
		virtual bool		Unregister(INotify* pINotify, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQProgressClient>::Unregister(pINotify,strEvent);
		}
		virtual bool		Unregister(EventFunc* pEventFunc, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQProgressClient>::Unregister(pEventFunc,strEvent);
		}
	private:
		bool m_bIsAnimate;
	};
}