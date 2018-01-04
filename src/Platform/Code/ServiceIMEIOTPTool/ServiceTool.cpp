#include "stdafx.h"
#include "windows.h"
#include "ServiceProgramTool.h"

void ShowDlgServiceTool()
{
	CServiceProgramToolApp *pApp = (CServiceProgramToolApp*) AfxGetApp();
	return pApp->ShowServiceProgramTool();
}