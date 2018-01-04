#ifndef ISPU_CALLBACK_H
#define ISPU_CALLBACK_H

#include "SPU_RESULT_DEF.h"
#include <vector>

class ISPU_CALLBACK
{
public:
	ISPU_CALLBACK(void){	}
	virtual ~ISPU_CALLBACK(void)	{}
	virtual bool CheckBreak() = 0;
	virtual bool WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize) = 0;
	virtual bool Print(const TSTRING& strName,const TSTRING& strOutput) = 0;
};

typedef ISPU_CALLBACK* LPISPU_CALLBACK;

class IValue
{
protected:
	IValue(){}
	virtual ~IValue(){}
public:
	virtual bool  SetValue(const TSTRING& strParameter,const TSTRING& strValue,std::vector<TSTRING>& strVars) = 0;
};

#endif //ISPU_CALLBACK_H