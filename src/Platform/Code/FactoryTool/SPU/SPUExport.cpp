#include "StdAfx.h"
#include "SPUExport.h"
#include "SPU.h"

SPU_API SPU_RESULT LoadEx(std::vector<TSTRING>& vectMemory,LPISPU_CALLBACK pISPU_CALLBACK,const TSTRING& strScriptFile)
{
	SPU_RESULT SPU_RES = SPU_FAIL;
	CSPU spu(pISPU_CALLBACK,strScriptFile);
	return spu.Run(vectMemory);
}

SPU_API SPU_RESULT Load(std::vector<TSTRING>& vectMemory,LPISPU_CALLBACK pISPU_CALLBACK)
{
	SPU_RESULT SPU_RES = SPU_FAIL;
	CSPU spu(pISPU_CALLBACK);
	return spu.Run(vectMemory);
}