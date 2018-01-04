#pragma once
#include "Request\Requestbase.h"

namespace PLATFORM_TOOL
{
	class IRequestManager  : public CRefObject
	{
	public:
		IRequestManager(){}
		virtual ~IRequestManager(){}
		virtual bool Open() = 0;
		virtual bool Close(bool bKeepPotocolType = false) = 0;
		virtual bool IsOpen() const = 0;
		virtual int GetComport() const = 0;
		virtual bool Execuate(CRequestPtr& prREquest,const std::string& strPlatform) = 0;		
		virtual void Reset() = 0;
		virtual void ResetMode() = 0;
		virtual PotocolType GetPotocolType() = 0;
		virtual void PopLastLog() = 0;
	};

	typedef CRefObjectPtr<IRequestManager> CIRequestManagerPtr;
}