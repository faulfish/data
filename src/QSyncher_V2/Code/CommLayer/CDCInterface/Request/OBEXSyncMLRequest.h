#pragma once

#include "OBEXSYNCMLBase.h"

class COBEXSYNCML_Put : public COBEXSyncMLBase
{
public:
	COBEXSYNCML_Put(CBufferPtr spBuffer)
	{
		m_ptBufferSend = CObexCodec::CreateRequestPut(spBuffer);
	}
	virtual ~COBEXSYNCML_Put(){}

private:
	virtual const bool DoCanClose(){return true;}
};


class COBEXSYNCML_Get : public COBEXSyncMLBase
{
public:
	COBEXSYNCML_Get(std::string strType)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXGet(m_dwConnID(),strType);
	}
	virtual ~COBEXSYNCML_Get(){}
private:
	virtual const bool DoCanClose(){return true;}
};


typedef CRefObjectPtr<COBEXSYNCML_Put>			COBEXSYNCML_PutFirstPtr;
typedef CRefObjectPtr<COBEXSYNCML_Get>			COBEXSYNCML_GetDataPtr;
