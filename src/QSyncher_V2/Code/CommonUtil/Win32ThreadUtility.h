#ifndef _Win32ThreadUtility_H_
#define _Win32ThreadUtility_H_

#pragma once
#include <Windows.h>

namespace QSYNC_LIB
{
	DWORD MessageLoop (  HANDLE* lphObjects, int     cObjects	,DWORD dwMilliseconds = INFINITE);
}
#endif