/*****************************************************************************
**
**  Name:       checkheaders.h
**
**  File:       Verifies the methods in  CObexHeaders
**
**  Date					Modification
**  ------------------------ 
**  5Jun2001    Jim French  Create
**
**  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.
**
*****************************************************************************/

#ifndef BLUEOBEX_CHECKHEADERS_H
#define BLUEOBEX_CHECKHEADERS_H

class CConnectionMgrDlg;

#include "btwlib.h"
#include "BtIfObexHeaders.h"


class ObexCheckHeaders
{
public:
	ObexCheckHeaders();
	~ObexCheckHeaders();

	CString Fill();
	CString Verify();

private:
	CObexHeaders hdrs;
};
#endif /* BLUEOBEX_CHECKHEADERS_H */
