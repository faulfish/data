#pragma once
#include "Win32ThreadUtility.h"

namespace QSYNC_LIB
{
	DWORD MessageLoop (  HANDLE* lphObjects, int     cObjects	,DWORD dwMilliseconds)
	{ 
		DWORD result; 

		// The message loop lasts until we get a WM_QUIT message,
		// upon which we shall return from the function.
		while (TRUE)
		{
			// block-local variable 		
			MSG msg ; 

			// Read all of the messages in this next loop, 
			// removing each message as we read it.
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{ 
				// If it is a quit message, exit.
				if (msg.message == WM_QUIT)  
					return 1; 
				// Otherwise, dispatch the message.
				::DispatchMessage(&msg); 
			} // End of PeekMessage while loop.

			// Wait for any message sent or posted to this queue 
			// or for one of the passed handles be set to signaled.
			result = ::MsgWaitForMultipleObjects(cObjects, lphObjects, FALSE, dwMilliseconds, QS_ALLINPUT); 

			// The result tells us the type of event we have.
			if (result == (WAIT_OBJECT_0 + cObjects))
			{
				// New messages have arrived. 
				// Continue to the top of the always while loop to 
				// dispatch them and resume waiting.
				continue;
			} 
			else 
			{ 
				// One of the handles became signaled. 
				break;
			} // End of else clause.
		} // End of the always while loop. 

		return result;
	} // End of function.
}