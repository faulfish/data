#include "StdAfx.h"
#include "Wireless.h"
#include "..\..\RequestManager\Request\WirelessRequest.h"
#include "..\..\RequestManager\Request\AudioRequest.h"

CWireless::CWireless(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CWireless::~CWireless(void)
{
}

bool CWireless::EnableGAP(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;;
	CRequestPtr ptRequest = new CEnableGAPRequest(bEnable,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless::StartBTPower(int nDegree)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	if((nDegree == 0xF0) || (nDegree == 0xF1) || (nDegree == 0xF2))
	{
		bool bStatus = false;
		CRequestPtr ptRequest = new CBTTxPowerRequest(nDegree,bStatus);	
		if(Execute(ptRequest))
		{
			bRes = bStatus;
		}
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless::EnterBTTestMode(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	CRequestPtr ptRequest = new CBTTestModeRequest(bEnable,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless::BTBound(bool bBound,const TSTRING& strBTAddress, const TSTRING& strBTPass,int nTimeout) 
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTBound(bBound,strBTAddress.c_str(), strBTPass.c_str(),bRes);
	Execute(ptRequest,nTimeout);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::WriteHeadsetBTAddress(const TSTRING& strBTAddress, int nTimeout) 
{
	using namespace PLATFORM_TOOL;

	// take this string strBTAddress to write into NV item
	bool bRes = false;
	CRequestPtr ptRequest = new CWriteBTHeadsetAddress(strBTAddress.c_str(), (int)strBTAddress.length(),bRes);
	Execute(ptRequest,nTimeout);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGateway(bool bActive)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(bActive ? 180 : 181,NULL,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewayConnect(bool bActive,const TSTRING& strBTAddress)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(bActive ? 182 : 183,strBTAddress.c_str(),bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewayAnswer(bool bActive)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(bActive ? 184 : 185,NULL,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewayAudioConnect(bool bActive)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(bActive ? 186 : 187,NULL,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewaySpeakerGain()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(189,NULL,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewayMicrophoneGain()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(189,NULL,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewayAudioLoop(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	char szFake[10] = {"Fake"};
	CRequestPtr ptRequest = new CBTRequest(190,bEnable ? NULL : szFake,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BTHFPGatewayRoaming(bool bActive)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CBTRequest(bActive ? 191 : 192,NULL,bRes);
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless:: FMRegistration()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(0,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless:: FMConnect(bool bConnect)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(bConnect ? 1 : 2,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless:: FMInitialize()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(3,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless:: FMSetStation(WORD nFMFrequence)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(6,0,nFMFrequence,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless:: FMMute(bool bMute)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(10,0,bMute ? 1 : 0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless:: FMReceiveRSSI(WORD& nRSSI)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	CRequestPtr ptRequest = new CFMRequest(20,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless:: FMSpeaker()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(24,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}
bool CWireless:: FMMonoHeadset()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(25,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}
bool CWireless:: FMStereoHeadset()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(26,0,0,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless::FMChangeTestModeAndSetLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	WORD nRSSI = 0;
	CRequestPtr ptRequest = new CFMRequest(27,nLevel,1,nRSSI,bStatus);	
	if(Execute(ptRequest))
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}


//Audio
bool CWireless::PlayMP3(bool bEnable)//default MP3 in the EFS
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;	
	CRequestPtr ptRequest = new CAudioLoopbackRequest(bEnable ? 30 : 31,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::SpeakerDevice(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;	
	CRequestPtr ptRequest = new CAudioLoopbackRequest(bEnable ? 32 : 33,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::SpeakerDeviceMultimediaLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(34,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::SpeakerDeviceVoiceLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(35,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::HandsetDevice(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CAudioLoopbackRequest(bEnable ? 36 : 37,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();


	return bRes;
}
bool CWireless::HandsetDeviceMultimediaLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(38,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}
bool CWireless::HandsetDeviceVoiceLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(39,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();


	return bRes;
}

bool CWireless::MonoHeadsetDevice(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;	
	CRequestPtr ptRequest = new CAudioLoopbackRequest(30,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();


	return bRes;
}
bool CWireless::MonoHeadsetDeviceMultimediaLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;	
	CRequestPtr ptRequest = new CAudioLoopbackRequest(30,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::MonoHeadsetDeviceVoiceLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;	
	CRequestPtr ptRequest = new CAudioLoopbackRequest(30,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::StereoHeadsetDevice(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CAudioLoopbackRequest(bEnable ? 40 : 41,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::StereoHeadsetDeviceMultimediaLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(42,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::StereoHeadsetDeviceVoiceLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(43,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::AudioPackageLoopback(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CAudioLoopbackRequest(bEnable ? 44 : 45,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::AlarmTone(int nTone)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CRequestPtr ptRequest = new CAudioLoopbackRequest(46,nTone,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CWireless::BluetoothDevice(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;	
	CRequestPtr ptRequest = new CAudioLoopbackRequest(bEnable ? 47 : 48,0,bRes);	
	Execute(ptRequest);

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::BluetoothDeviceMultimediaLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(49,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}
bool CWireless::BluetoothDeviceVoiceLevel(int nLevel)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	assert(nLevel >= 0 && nLevel <= 10);
	if(nLevel >= 0 && nLevel <= 10)
	{
		CRequestPtr ptRequest = new CAudioLoopbackRequest(50,nLevel,bRes);	
		Execute(ptRequest);
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}