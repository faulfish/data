#pragma once

#include <vector>
#include <string>
#include <map>
#include "..\..\CommonUtil\Notifybase.h"
#include "..\..\ShareLib\loki-0.1.5\include\loki\smartptr.h"
#include "..\..\CommonUtil\commondatafile.h"
#include "ChangeLog.h"

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
		TwoWay,											//200
		Slow,													//201
		OneWayFromClient,							//202
		RefreshFromClient,							//203
		OneWayFromServer,						//204
		RefreshFromServer,							//205
		TwoWayByServer,							//206
		OneWayFromClientByServer,			//207
		RefreshFromClientByServer,			//208
		OneWayFromServerByServer,		//209
		RefreshFromServerByServer			//210
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

	typedef SmartPtr<IPIMObj,QSyncerRefCounted>	IPIMObjPt;
	//typedef SmartPtr<IPIMObj,RefCountedMTAdj<ClassLevelLockable>::RefCountedMT>	IPIMObjPt;
	typedef vector<IPIMObjPt>								IPIMObjPts;
	typedef vector<IPIMObjPt>::iterator						IPIMObjPtsIter;
	typedef vector<IPIMObjPt>::const_iterator				IPIMObjPtsCIter;

	typedef SmartPtr<IPIM,QSyncerRefCounted>		IPIMPt;
	//typedef SmartPtr<IPIM,RefCountedMTAdj<ClassLevelLockable>::RefCountedMT>		IPIMPt;

	typedef SmartPtr<IDevice,QSyncerRefCounted>	IDevicePt;
	//typedef SmartPtr<IDevice,RefCountedMTAdj<ClassLevelLockable>::RefCountedMT>	IDevicePt;
	typedef vector<IDevicePt>								IDevicePts;
	typedef vector<IDevicePt>::iterator						IDevicePtsIter;
	typedef vector<IDevicePt>::const_iterator				IDevicePtsCIter;

	typedef SmartPtr<IPIMDB,QSyncerRefCounted>		IPIMDBPt;
	//typedef SmartPtr<IPIMDB,RefCountedMTAdj<ClassLevelLockable>::RefCountedMT>		IPIMDBPt;
	typedef vector<IPIMDBPt>								IPIMDBPts;
	typedef map<TSTRING,IPIMDBPt>							DBMAP;
	typedef map<TSTRING,IPIMDBPt>::const_iterator			DBMAPCIter;
	typedef map<TSTRING,IPIMDBPt>::iterator					DBMAPIter;
	typedef pair <TSTRING,IPIMDBPt>							DBMapPair;

	interface IPIM
	{
		virtual IPIMObjPt CreatePIMObject(GUID guid = GUID_NULL) = 0;
	public:
		virtual ~IPIM(){};
	};	

	interface IDevice
	{
		virtual bool				CanSync() const = 0;
		virtual bool				Sync(SyncType syncType) = 0;
		virtual const			TSTRING& GetName() const = 0;
		virtual const			TSTRING& GetDisplayName() const = 0;
		virtual const			TSTRING& GetUniqueName() = 0;
		virtual void				SetDatabase(const IPIMDBPt& prIPIMDB) = 0;
		virtual IPIMDB*		GetDatabase() = 0;
		virtual IPIMDB*		Connect(const TSTRING& strModule,const TSTRING& strID,IDevicePt ptDevice) = 0;
		virtual void				Disconnect() = 0;		
		virtual bool				Compare(IPIMObjPt& pimObjPt1,IPIMObjPt& pimObjPt2) = 0;
		virtual void				DefaultTwoWay(IDevicePt& ptSourceDevice) = 0;
		virtual void				DefaultOneWayFromSource(IDevicePt& ptSourceDevice) = 0;
		virtual bool				InsertItem(IPIMObjPt& ptIPIMObj) = 0;
		virtual bool				RemoveItem(IPIMObjPt& ptIPIMObj) = 0;
		virtual bool				UpdateItem(IPIMObjPt& ptIPIMObj) = 0;
		virtual CChangeLog&	GetChangeLog() = 0;
		virtual IPIMObjPts&		GetAllData() = 0;
		virtual bool DataBaseRegister(INotify* pINotify,const TSTRING& strEvent) = 0;
	public:
		virtual ~IDevice(){};
	};

	interface IPIMDB
	{
		virtual bool BeginTransaction(unsigned long dwMilliseconds = 0) = 0;
		virtual bool BeginTransaction(const GUID& guid,IPIMObjPt& pimObjPt,unsigned long dwMilliseconds = 0) = 0;
		virtual bool BeginTransaction(const long nIndex,IPIMObjPt& pimObjPt,unsigned long dwMilliseconds = 0) = 0;
		
		virtual void Fire(const TSTRING& strEvent,long nParam = 0) = 0;
		virtual bool Register(INotify* pINotify,const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pINotify,const TSTRING& strEvent) = 0;
		virtual bool EndTransaction() = 0;
		virtual bool CreatePIMObject(IPIMObjPt& pimObjPtvirtual,GUID guid = GUID_NULL) = 0;		
		virtual bool EnumCompatiablePIMSpecs(vector<PIMSpecData>& vectSPECs) = 0;
		virtual const DBStatus GetStatus() = 0;
		virtual bool SetStatus(const DBStatus status) = 0;

		//About item , item means not folder or group
		virtual long GetCount() = 0;
		virtual long GetRealCount() = 0;
		virtual bool InsertItem(IPIMObjPt ptIPIMObj) = 0;
		virtual bool RemoveItem(IPIMObjPt& ptIPIMObj) = 0;
		virtual bool RemoveAll(bool bRefresh = false) = 0;
		virtual bool UpdateItem(IPIMObjPt& ptIPIMObj) = 0;

		virtual bool SetCount(long nCount) = 0;
		virtual void GetDefaultDevice(IDevicePt& ptIDevice) const = 0;
		virtual void SetDefaultDevice(IDevicePt& ptIDevice) = 0;

		//Manage remote device
		virtual bool EnumRemoteDevice(IDevicePts& Devices) = 0;
		virtual void AddRemoteDevice(IDevicePt& ptIDevice,IPIMDBPt& ptIPIMDB) = 0;
		virtual void RemoveRemoteDevice(IDevicePt& ptIDevice) = 0;
		virtual void RemoveAllRemoteDevice() = 0;

		virtual bool FindDevice(const TSTRING& strDeviceName,IDevicePt& ptIDevice) = 0;
		virtual int GetAnchor() = 0;
		virtual void SetAnchor(int nAnchor) = 0;
		virtual const GUID FindGUIDByLUID(const int& nLUID) const = 0;
		virtual bool RemoveItemByGUID(const GUID& guid) = 0;
		virtual bool GetItemByGUID(const GUID& guid,IPIMObjPt& ptIPIMObj) = 0;
		virtual bool GetChangeCount(int& nCount) = 0;
		virtual bool GetChangeByIndex(int& nIndex,ChangeLogData& changeData) = 0;
		virtual void ResetChange(int nIndex) = 0;
		virtual bool SetLUIDByLUID(const int& nOldLUID,const int& nNewLUID) = 0;
		virtual void Save() = 0;
        virtual void Save(const TSTRING& szFileName) = 0;
		virtual void Load() = 0;
        virtual void Load(const TSTRING& szFileName) = 0;
		virtual void EnableLog(bool bEnable) = 0;
	public:
		virtual ~IPIMDB(){};
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//Notify Type
	//Parameter is Point of GUID
	const TCHAR g_strNotifyDBFinish[] = _T("Finish");
	const TCHAR g_strNotifyDBInsert[] = _T("InsertItem");
	const TCHAR g_strNotifyDBRemove[] = _T("RemoveItem");
	const TCHAR g_strNotifyDBUpdate[] = _T("UpdateItem");
}

