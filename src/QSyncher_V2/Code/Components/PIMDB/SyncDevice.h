#ifndef _SYNC_DEVICE_H_
#define _SYNC_DEVICE_H_

#pragma once
#pragma warning(push)
#pragma warning(disable:4996)

#include "SyncManager.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include <Objbase.h>
#include <io.h>
#include "..\..\CommonUtil\UnicodeUtility.h"
//#include "ChangeLog.h"

#pragma warning(disable:4311)
#pragma warning(disable:4312)
namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;
		
	
	struct PIMDBHeader
	{
		PIMDBHeader():m_nAnchor(0),m_nCount(0),m_nSize(0)
		{				
			::memset(m_szFormat,0,sizeof(m_szFormat));
			::memset(m_szVersion,0,sizeof(m_szVersion));
			::strcpy_s(m_szFormat,sizeof(m_szFormat),"@PIMDB");
			::strcpy_s(m_szVersion,sizeof(m_szVersion),"10000000");
		}
		virtual ~PIMDBHeader(){}
		char		m_szFormat[9];
		char		m_szVersion[9];
		int			m_nAnchor;
		int			m_nCount;
		long long	m_nSize;
	};

	class DeviceBase : public IDevice
	{
	public:
		virtual const TSTRING& GetUniqueName()
		{//for unique issue to identify which device
			if(m_strLocalRealName.empty())
			{
				TCHAR szFolder[MAX_PATH] = {0};
				bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("QSyncer"));
				assert(bRes);
				if(bRes)
				{				
					TSTRING strSyncMLFConfigurationFile = m_strName + _T(".dat");
					strSyncMLFConfigurationFile = szFolder + strSyncMLFConfigurationFile;
					
					if(_taccess(strSyncMLFConfigurationFile.c_str(),0) != -1)
					{
						FILE* file = ::_wfopen(strSyncMLFConfigurationFile.c_str(),_T("r"));
						if(file)
						{
							TCHAR szSyncMLDeviceName[255+1] = {0};
							::fread(szSyncMLDeviceName,255*sizeof(TCHAR),1,file);
							m_strLocalRealName = szSyncMLDeviceName;
							::fclose(file);
						}
					}
					else
					{
						FILE* file = ::_wfopen(strSyncMLFConfigurationFile.c_str(),_T("w"));
						if(file)
						{
							GUID genGuid = GUID_NULL;				
							HRESULT  hr = CoCreateGuid(&genGuid);
							assert(SUCCEEDED(hr));
							if(SUCCEEDED(hr))
							{				
								wchar_t szTemp[40] = {0};
								StringFromGUID2(genGuid,szTemp,40);
								std::wstring wstrTemp = szTemp;
								std::wstring wstrGUID = wstrTemp.substr(1,36);
								#ifdef _UNICODE
								m_strLocalRealName = m_strName + _T("_");
								m_strLocalRealName += wstrGUID.c_str();
								#else
								char szTempAnsi[40+2] = {0};
								UnicodeToAnsiBuffer(wstrGUID.c_str(),szTempAnsi,40+2);
								m_strLocalRealName = m_strName + _T("_");
								m_strLocalRealName += szTempAnsi;
								#endif
							}
							size_t nRet = ::fwrite(m_strLocalRealName.c_str(),m_strLocalRealName.size()*sizeof(TCHAR),1,file);
							assert(nRet != 0);
							::fclose(file);
						}
					}
				}
			}

			assert(!m_strLocalRealName.empty());
			return m_strLocalRealName;
		}

		virtual const TSTRING& GetName() const {return m_strName;}
		virtual const TSTRING& GetDisplayName() const {return m_strDisplayName;}
		
		virtual IPIMObjPts&	GetAllData()
		{
			return m_IPIMObjPts;
		}

		virtual bool	Compare(IPIMObjPt& pimObjPt1,IPIMObjPt& pimObjPt2)
		{
			assert(0);//Need to implement
			return false;
		}

		/*
		GetAllData and compare each file to decide if add or update or delete
		*/
		virtual void DefaultTwoWay(IDevicePt& ptSourceDevice)
		{
			IPIMObjPts& vectSelfPIMObjects = GetAllData();
			for(IPIMObjPts::iterator Iter = vectSelfPIMObjects.begin();Iter != vectSelfPIMObjects.end();Iter++)
			{
			}

			
			IPIMObjPts& vectSourcePIMObjects = ptSourceDevice->GetAllData();
			for(IPIMObjPts::iterator Iter = vectSourcePIMObjects.begin();Iter != vectSourcePIMObjects.end();Iter++)
			{
			}
		}

		/*
		Comapre source data if it's not the same and then add it or update it.
		*/
		virtual void DefaultOneWayFromSource(IDevicePt& ptSourceDevice)
		{
			IPIMObjPts& vectSelfPIMObjects = GetAllData();			

			IPIMObjPts& vectSourcePIMObjects = ptSourceDevice->GetAllData();
			
			for(IPIMObjPts::iterator sourceIter = vectSourcePIMObjects.begin();sourceIter != vectSourcePIMObjects.end();sourceIter++)
			{
				if(exist(vectSelfPIMObjects,*sourceIter))
					UpdateItem(*sourceIter);
				else
					InsertItem(*sourceIter);
			}
		}

		virtual bool	InsertItem(IPIMObjPt& ptIPIMObj)
		{
			return Internal_InsertItem(ptIPIMObj);
		}

		virtual bool RemoveItem(IPIMObjPt& ptIPIMObj)
		{
			assert(0);//Need to implement
			return false;
		}

		virtual bool UpdateItem(IPIMObjPt& ptIPIMObj)
		{
			assert(0);//Need to implement
			return false;
		}
		virtual CChangeLog& GetChangeLog()
		{
			return m_ChangeLog;
		}
		virtual bool DataBaseRegister(INotify* pINotify,const TSTRING& strEvent)
		{
			return m_ptIPIMDB->Register(pINotify,strEvent);
		}
	protected:
		DeviceBase(const TSTRING& strName):m_strName(strName),m_strDisplayName(m_strName)
		{
		}
		virtual ~DeviceBase()
		{
			TRACE(_T("Destruct %s\n"),m_strName);
		}
		void SetDisplayName(const TSTRING& strDisplayName){m_strDisplayName = strDisplayName;}		
		bool exist(IPIMObjPts& vectPIMObjects,IPIMObjPt& pimObjPt)
		{
			bool bExist = false;
			for(IPIMObjPts::iterator selfIter = vectPIMObjects.begin();selfIter != vectPIMObjects.end();selfIter++)
			{
				if(Compare(*selfIter,pimObjPt))
				{
					bExist = true;
					break;
				}
			}
			return bExist;
		}
		bool	Internal_InsertItem(IPIMObjPt& ptIPIMObj,bool bInternal = false)
		{
			bool bRes =  false;
			ptIPIMObj->ResetModify();
			int nLUID = GetLUID(ptIPIMObj);
			m_ChangeLog.Insert(ptIPIMObj->GetGUID(),nLUID,GetAnchor());
			m_IPIMObjPts.push_back(ptIPIMObj);
			m_Header.m_nCount = static_cast<int>(m_IPIMObjPts.size());
			if(bInternal)
				m_ptIPIMDB->Fire(g_strNotifyDBInsert,(long)&(ptIPIMObj->GetGUID()));
			bRes = true;
			return bRes;
		}
	protected:
		virtual void SetDatabase(const IPIMDBPt& ptIPIMDB){m_ptIPIMDB = ptIPIMDB.GetPointer(ptIPIMDB);}
		virtual IPIMDB* Connect(const TSTRING& strModule,const TSTRING& strID,IDevicePt ptDevice){return m_ptIPIMDB;}
		virtual void Disconnect(){}
		virtual IPIMDB* GetDatabase(){return m_ptIPIMDB;}


		virtual int GetDatabaseCount()
		{
			return m_ptIPIMDB->GetCount();
		}
		virtual bool BeginTransaction(unsigned long dwMilliseconds = 0)
		{
			return m_ptIPIMDB->BeginTransaction(dwMilliseconds);
		}
		virtual bool BeginTransaction(const GUID& guid,IPIMObjPt& pimObjPt,unsigned long dwMilliseconds = 0)
		{
			return m_ptIPIMDB->BeginTransaction(guid,pimObjPt,dwMilliseconds);
		}
		virtual int BeginTransaction(const long nIndex,IPIMObjPt& pimObjPt)
		{
			return m_ptIPIMDB->BeginTransaction(nIndex,pimObjPt);
		}
		virtual void EndDatabaseTransaction()
		{
			m_ptIPIMDB->EndTransaction();
		}

		virtual void DatabaseFire(const TSTRING& strEvent,long nParam = 0)
		{
			m_ptIPIMDB->Fire(strEvent,nParam );
		}
		virtual bool InsertItemToDefaultDevice(IPIMObjPt ptIPIMObj)
		{
			return m_ptIPIMDB->InsertItem(ptIPIMObj);
		}
		virtual bool UpdateItemToDefaultDevice(IPIMObjPt& ptIPIMObj)
		{
			return m_ptIPIMDB->UpdateItem(ptIPIMObj);
		}


		const int GetLUID(const IPIMObjPt& ptIPIMObj) const
		{
			int nTemp = -1;
			CommonDataPt spData;
			size_t nIndex = ptIPIMObj->GetByName("LUID",spData);
			assert(nIndex != -1);
			if(nIndex != -1)
			{
				assert((spData->GetType() == Integer));
				if(spData->GetType() == Integer)
					nTemp = spData->GetInteger();
			}
			return nTemp;
		}
		virtual int GetAnchor(){return m_Header.m_nAnchor;}
	private:
		TSTRING				m_strName;
		TSTRING				m_strDisplayName;
		TSTRING				m_strLocalRealName;
		IPIMDB*					m_ptIPIMDB;//????
		IPIMObjPts				m_IPIMObjPts;
		CChangeLog			m_ChangeLog;
		PIMDBHeader		m_Header;
	};
}

#pragma warning(pop)
#endif //_SYNC_DEVICE_H_