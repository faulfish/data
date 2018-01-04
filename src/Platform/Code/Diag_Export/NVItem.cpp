#include "StdAfx.h"
#include "NVItem.h"
#include "..\RequestManager\Request\NVRequest.h"

CNVItem::CNVItem(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CNVItem::~CNVItem()
{
}

bool CNVItem::ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp)
{
	using namespace PLATFORM_TOOL;
	CRequestPtr ptRequest = new CNVRequest(NVItem,ReadResp);
	return Execuate(ptRequest);
}