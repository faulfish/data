#pragma once

#include <vector>
#include <string>
#include <map>
#include "..\CommonUtil\Notifybase.h"
#include <loki\smartptr.h>

namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;

	#ifndef GUID_DEFINED
	#define GUID_DEFINED
	#if defined(__midl)
	typedef struct {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		byte           Data4[ 8 ];
	} GUID;
	#else
	typedef struct _GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[ 8 ];
	} GUID;
	#endif
	#endif

	enum SyncType
	{
		OneWay,
		TwoWay,
		Slow,
		Fast
	};	

	enum DBStatus
	{
		Offine,
		Transport,
		Ready
	};

	struct PIMSpecData
	{
		string												m_strSpecName;
		string												m_strVersion;
		bool operator==(const PIMSpecData& data)
		{
			return (m_strSpecName == data.m_strSpecName) && (m_strVersion == data.m_strVersion);
		}
	};

	struct PIMFieldCap
	{
		string												m_strSpecName;
		long												m_nMaxLength;
		long												m_nMinLength;
	};
	
	struct IPIMObj;
	struct IPIM;
	struct IDevice;
	struct IPIMDB;

	typedef SmartPtr<IPIMObj>								IPIMObjPt;
	typedef vector<IPIMObjPt>								IPIMObjPts;
	typedef vector<IPIMObjPt>::iterator						IPIMObjPtsIter;
	typedef vector<IPIMObjPt>::const_iterator				IPIMObjPtsCIter;

	typedef SmartPtr<IPIM>									IPIMPt;

	typedef SmartPtr<IDevice>								IDevicePt;
	typedef vector<IDevicePt>								IDevicePts;
	typedef vector<IDevicePt>::iterator						IDevicePtsIter;
	typedef vector<IDevicePt>::const_iterator				IDevicePtsCIter;

	typedef SmartPtr<IPIMDB>								IPIMDBPt;
	typedef vector<IPIMDBPt>								IPIMDBPts;
	typedef map<TSTRING,IPIMDBPt>							DBMAP;
	typedef map<TSTRING,IPIMDBPt>::const_iterator			DBMAPCIter;
	typedef map<TSTRING,IPIMDBPt>::iterator					DBMAPIter;
	typedef pair <TSTRING,IPIMDBPt>							DBMapPair;

	struct IPIMObj
	{
		virtual bool encode(TSTRING& strRAW) = 0;
		virtual bool decode(const TSTRING& strRAW) = 0;
		virtual bool encode(void** pVoid,long& nSize) = 0;
		virtual bool decode(void* pVoid,const long& nSize) = 0;
	};

	struct IPIM
	{
		virtual IPIMObjPt CreatePIMObject() = 0;
	};	

	struct IDevice
	{
		virtual bool CanSync() const = 0;
		virtual bool Sync(SyncType syncType) = 0;
		virtual const TSTRING& GetName() const = 0;
		virtual void SetDatabase(const IPIMDBPt& prIPIMDB) = 0;
		virtual bool GetDatabase(IPIMDBPt& ptIPIMDB) = 0;
	};

	struct IPIMDB
	{
		virtual bool BeginTransaction(IPIMObjPts& vectPIMObjects,unsigned long dwMilliseconds = 0) = 0;
		virtual bool BeginTransaction(const GUID& guid,IPIMObjPt& pimObjPt,unsigned long dwMilliseconds = 0) = 0;
		virtual bool BeginTransaction(const long nIndex,IPIMObjPt& pimObjPt,unsigned long dwMilliseconds = 0) = 0;
		virtual bool InsertItem(IPIMObjPt& ptIPIMObj) = 0;
		virtual bool RemoveItem(IPIMObjPt& ptIPIMObj) = 0;
		virtual void Fire(const TSTRING& strEvent) = 0;
		virtual bool EndTransaction() = 0;
		virtual bool CreatePIMObject(IPIMObjPt& pimObjPt) = 0;
		virtual bool EnumDevice(IDevicePts& Devices) = 0;
		virtual bool EnumCompatiablePIMSpecs(vector<PIMSpecData>& vectSPECs) = 0;
		virtual const DBStatus GetStatus() = 0;
		virtual bool SetStatus(const DBStatus status) = 0;
		virtual long GetCount() = 0;
		virtual long GetRealCount() = 0;
		virtual bool SetCount(long nCount) = 0;
		virtual void AddRefDevice(IDevicePt& ptIDevice) = 0;							//protected
		virtual void RemoveRefDevice(IDevicePt& ptIDevice) = 0;							//protected
		virtual bool FindDevice(const TSTRING& strDeviceName,IDevicePt& ptIDevice) = 0;	//protected
	};
}

