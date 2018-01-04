#include "stdafx.h"
#include "CPIMDB.h"
#include "PIMObjectBase.h"
#include <vector>
#include <string>
#include <loki/smartptr.h>
#include "..\..\CommonUtil\NotifyBase.h"
#include "SyncManager.h"
#include "Winbase.h"
#include "ChangeLog.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"
#include "ClientDevice.h"
//For now, we support two devices
#include "SyncMLDevice.h"
#include "QPhonebookDevice.h"
#include "QCalendarDevice.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"

#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:4311)

#ifdef		PIMDB_DLL
#define		PIMDB_DLL_DECLSPEC		__declspec(dllexport)
#else
#define		PIMDB_DLL_DECLSPEC		__declspec(dllimport)
#endif

namespace QSYNC_LIB
{
    using namespace std;
    using namespace Loki;

    class CPIMDB : public CTNotifyBase<IPIMDB>
    {
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
        friend class CSyncManager;
    public:
        CPIMDB(const TSTRING& strDatabase,IPIMPt& ptIPIM)
            :m_strDatabase(strDatabase)
			,m_ptIPIM(ptIPIM)
			,m_DBStatus(Offine)
			,m_bOpened(false)
        {//strDatabase "IME001,calendar"
            //m_hEvent = ::CreateEvent(NULL,FALSE,TRUE,strDatabase.c_str());
			m_hEvent = ::CreateMutex(NULL,FALSE,strDatabase.c_str());
            DefineNotify(g_strNotifyDBInsert);
            DefineNotify(g_strNotifyDBRemove);
            DefineNotify(g_strNotifyDBFinish);
            DefineNotify(g_strNotifyDBUpdate);			
        }
        virtual ~CPIMDB()
        {
            ::CloseHandle(m_hEvent);
        }

        //interface IPIMDB		
        virtual bool BeginTransaction(unsigned long dwMilliseconds)
        {
            bool bRes =  false;
            if(::WaitForSingleObject(m_hEvent,dwMilliseconds) == WAIT_OBJECT_0)
            {
                m_bOpened = true;
                bRes = true;				
            }
            return bRes;
        }
        virtual bool BeginTransaction(IPIMObjPts& vectPIMObjs,unsigned long dwMilliseconds)
        {
            bool bRes =  false;
            if(BeginTransaction(dwMilliseconds))
            {
				IDevicePt ptIDevice;
				GetDefaultDevice(ptIDevice);
				if(ptIDevice)
				{
					vectPIMObjs = ptIDevice->GetAllData();
					bRes = true;
				}
            }
            return bRes;
        }
        virtual bool BeginTransaction(const GUID& guid,IPIMObjPt& IPIMObjPt,unsigned long dwMilliseconds)
        {
            bool bRes =  false;
            if(BeginTransaction(dwMilliseconds))
            {
                IPIMObjPtsIter Iter;
                if(GetItemByGUID(guid,Iter))
                {
                    IPIMObjPt = *Iter;
                    bRes = true;
                }
				if(bRes == false)
					EndTransaction();
            }
            return bRes;
        }
        virtual bool BeginTransaction(const long nIndex,IPIMObjPt& ptIPIMObj,unsigned long dwMilliseconds)
        {
            bool bRes =  false;
			
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            if(nIndex < static_cast<long>(ptIPIMObjPts.size()) && nIndex >= 0)
            {
                if(BeginTransaction(dwMilliseconds))
                {
                    ptIPIMObj = ptIPIMObjPts[nIndex];
                    bRes = true;
					
					if(bRes == false)
						EndTransaction();
                }
            }
            return bRes;
        }
        virtual bool EndTransaction()
        {
			//if(m_bOpened)
			{
				::ReleaseMutex(m_hEvent);
				m_bOpened = false;
			}
            return true;
        }
        virtual bool CreatePIMObject(IPIMObjPt& spIPIMObj,GUID guid = GUID_NULL)
        {
            bool bRes = false;
            if(m_ptIPIM)
                spIPIMObj = m_ptIPIM->CreatePIMObject(guid);
            if(spIPIMObj)
                bRes = true;
            return bRes;
        }
        virtual bool EnumRemoteDevice(IDevicePts& vectDevices)
        {
            vectDevices = m_ptRemoteIDevices;
            return true;
        }
        virtual bool EnumCompatiablePIMSpecs(vector<PIMSpecData>& vectSPECs)
        {
            vectSPECs = m_vectSPECs;
            return true;
        }
        virtual const DBStatus GetStatus()
        {
            return m_DBStatus;
        }
        virtual bool SetStatus(const DBStatus status)
        {
            bool bRes = false;
            if(m_bOpened)
            {
                m_DBStatus = status;
                bRes = true;
            }
            return bRes;
        }
        virtual long GetCount()
        {
            return GetRealCount();
        }
        virtual bool SetCount(long nCount)
        {
            bool bRes = false;
            if(m_bOpened)
            {
                m_Header.m_nCount = nCount;
                bRes = true;
            }
            return bRes;
        }
        virtual long GetRealCount()
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            return static_cast<long>(ptIPIMObjPts.size());
        }
        virtual bool InsertItem(IPIMObjPt ptIPIMObj)
        {
            bool bRes =  false;
			if(m_bOpened)
			{
				IDevicePt ptIDevice;
				GetDefaultDevice(ptIDevice);
				if(ptIDevice && ptIDevice->InsertItem(ptIPIMObj))
				{
					ptIPIMObj->ResetModify();
					int nLUID = GetLUID(ptIPIMObj);
					GetChangeLog().Insert(ptIPIMObj->GetGUID(),nLUID,GetAnchor());
					m_Header.m_nCount = GetRealCount();
					Fire(g_strNotifyDBInsert,(long)&(ptIPIMObj->GetGUID()));
					bRes = true;
				}
			}
            return bRes;
        }
        virtual bool RemoveItem(IPIMObjPt& ptIPIMObj)
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            bool bRes =  false;
            assert(m_bOpened);
            if(m_bOpened)
            {
                IPIMObjPtsIter Iter = find(ptIPIMObjPts.begin(),ptIPIMObjPts.end(),ptIPIMObj);
                if(Iter != ptIPIMObjPts.end())
                {
                    bRes = RemoveItemByGUID((*Iter)->GetGUID());
                }
            }
            return bRes;
        }
		virtual bool UpdateItem(IPIMObjPt& ptIPIMObj)
		{
            bool bRes =  false;
            assert(m_bOpened);
            if(m_bOpened)
            {
				IDevicePt ptIDevice;
				GetDefaultDevice(ptIDevice);
				bRes = ptIDevice->UpdateItem(ptIPIMObj);
			}
			return bRes;
		}
        virtual void Fire(const TSTRING& strEvent,long nParam = 0){CTNotifyBase<IPIMDB>::Fire(strEvent,nParam);}		
        virtual bool Register(INotify* pINotify,const TSTRING& strEvent){return CTNotifyBase<IPIMDB>::Register(pINotify,strEvent);}
        virtual bool Unregister(INotify* pINotify,const TSTRING& strEvent){return CTNotifyBase<IPIMDB>::Unregister(pINotify,strEvent);}
        virtual int GetAnchor(){return m_Header.m_nAnchor;}
        virtual void SetAnchor(int nAnchor){m_Header.m_nAnchor = nAnchor;}
        virtual bool RemoveItemByGUID(const GUID& guid)
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            bool bRes = true;
            assert(m_bOpened);
            if(m_bOpened)
            {
                IPIMObjPtsIter Iter;
                if(GetItemByGUID(guid,Iter))
                {
					IDevicePt ptIDevice;
					GetDefaultDevice(ptIDevice);
					bRes = ptIDevice->RemoveItem(*Iter);
					if(bRes)
					{
						GetChangeLog().Delete(guid);
						ptIPIMObjPts.erase(Iter);
						m_Header.m_nCount = GetRealCount();
						Fire(g_strNotifyDBRemove,(long)&guid);
					}
                }
            }
            return bRes;
        }
        virtual bool RemoveAll(bool bRefresh = false)
        {
			bool bRes = false;
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

			if(bRefresh)
			{
				ptIPIMObjPts.clear();
				bRes = true;
			}
			else
			{				
				assert(m_bOpened);
				if(m_bOpened)
				{
					IPIMObjPtsIter Iter = ptIPIMObjPts.begin();
					while(Iter != ptIPIMObjPts.end())
					{
						RemoveItemByGUID((*Iter)->GetGUID());
						Iter = ptIPIMObjPts.begin();
					}
				}
				bRes = ptIPIMObjPts.empty();
			}
            return bRes;
        }

        virtual const GUID FindGUIDByLUID(const int& nLUID) const
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            GUID guid = GUID_NULL;

            for(IPIMObjPtsCIter Iter = ptIPIMObjPts.begin();Iter != ptIPIMObjPts.end();Iter++)
            {
                int nTempLUID = GetLUID(*Iter);
                assert(nTempLUID != -1);
                if(nLUID != -1 && nTempLUID == nLUID)
                {
                    guid = (*Iter)->GetGUID();
                    break;
                }
            }
            return guid;
        }
        virtual void Save(const TSTRING& szFileName)
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            TCHAR szFolder[MAX_PATH] = {0};
            bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("QSyncer"));
            assert(bRes);
            TSTRING strFile = szFolder + szFileName + _T(".db");
            FILE* file = ::_tfopen(strFile.c_str(), _T("wb+"));
            assert(file);
             if(file)
            {
                size_t nRet = ::fwrite(&m_Header,sizeof(PIMDBHeader),1,file);
                assert(nRet == 1);
                if(nRet == 1)
                {
                    GetChangeLog().Save(file);
                    for(IPIMObjPtsIter Iter = ptIPIMObjPts.begin();Iter != ptIPIMObjPts.end();Iter++)
                        (*Iter)->Save(file);
                }
                ::fclose(file);
            }
        }
        virtual void Save()
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            FILE*  file;
            file = ::_tfopen(GetDBFile().c_str(),_T("wb+"));
            assert(file);
            if(file)
            {
                size_t nRet = ::fwrite(&m_Header,sizeof(PIMDBHeader),1,file);
                assert(nRet == 1);
                if(nRet == 1)
                {
                    GetChangeLog().Save(file);
                    for(IPIMObjPtsIter Iter = ptIPIMObjPts.begin();Iter != ptIPIMObjPts.end();Iter++)
                        (*Iter)->Save(file);
                }
                ::fclose(file);
            }
        }

        virtual void Load()
        {
            FILE*  file;
            file = ::_tfopen(GetDBFile().c_str(),_T("rb"));
            if(file)
            {
                ::ZeroMemory(&m_Header,sizeof(PIMDBHeader));
                size_t nHadReadSize = ::fread(&m_Header,sizeof(PIMDBHeader),1,file);
                assert(nHadReadSize == 1);
                if(nHadReadSize == 1)
                {					
                    if(m_Header.m_szVersion == string("10000000") && m_Header.m_szFormat == string("@PIMDB"))
                    {
                        GetChangeLog().Load(file);
                        if(m_Header.m_nCount > 0 && BeginTransaction(0))
                        {
                            int nCountFromFile = m_Header.m_nCount;
                            for(int i=0;i<nCountFromFile;i++)
                            {
                                IPIMObjPt spNewPIM = m_ptIPIM->CreatePIMObject();
                                spNewPIM->Load(file);
                                int nLUID = GetLUID(spNewPIM);
                                ASSERT(nLUID != 0);
                                if(nLUID != -1)
                                {
                                    GUID guid;
                                    if(GetChangeLog().GetGUIDFromLUID(nLUID,guid))
                                        spNewPIM->SetGUID(guid);
                                }
                                InsertItem(spNewPIM);
                            }
                            GetChangeLog().Reset();
                            EndTransaction();
                        }
                    }
                    else
                    {
                        //version is error
                        assert(0);
                    }
                }
                ::fclose(file);
            }
        }
        virtual void Load(const TSTRING& szFileName)
        {
           TCHAR szFolder[MAX_PATH] = {0};
            bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("QSyncer"));
            assert(bRes);
            TSTRING strFile = szFolder + szFileName + _T(".db");
            FILE*  file;
            file = ::_tfopen(strFile.c_str(),_T("rb"));
            if(file)
            {
                ::ZeroMemory(&m_Header,sizeof(PIMDBHeader));
                size_t nHadReadSize = ::fread(&m_Header,sizeof(PIMDBHeader),1,file);
                assert(nHadReadSize == 1);
                if(nHadReadSize == 1)
                {					
                    if(m_Header.m_szVersion == string("10000000") && m_Header.m_szFormat == string("@PIMDB"))
                    {
                        GetChangeLog().Load(file);
                        if(m_Header.m_nCount > 0 && BeginTransaction(0))
                        {
                            int nCountFromFile = m_Header.m_nCount;
                            for(int i=0;i<nCountFromFile;i++)
                            {
                                IPIMObjPt spNewPIM = m_ptIPIM->CreatePIMObject();
                                spNewPIM->Load(file);
                                int nLUID = GetLUID(spNewPIM);
                                ASSERT(nLUID != 0);
                                if(nLUID != -1)
                                {
                                    GUID guid;
                                    if(GetChangeLog().GetGUIDFromLUID(nLUID,guid))
                                        spNewPIM->SetGUID(guid);
                                }
                                InsertItem(spNewPIM);
                            }
                            GetChangeLog().Reset();
                            EndTransaction();
                        }
                    }
                    else
                    {
                        //version is error
                        assert(0);
                    }
                }
                ::fclose(file);
            }
        }
        virtual bool SetLUIDByLUID(const int& nOldLUID,const int& nNewLUID)
        {
            bool bRes = false;
            if(nOldLUID != nNewLUID)
            {
                GUID guid = FindGUIDByLUID(nOldLUID);

                IPIMObjPt ptIPIMObj;
                if(GetItemByGUID(guid,ptIPIMObj))
                {
                    bRes = SetLUID(ptIPIMObj,nNewLUID);
                    if(bRes)
                        GetChangeLog().SetLUIDByGUID(guid,nNewLUID);
                }
            }
            return bRes;
        }
		virtual void EnableLog(bool bEnable){GetChangeLog().EnableLog(bEnable);}
    protected:
		
		IPIMObjPts& GetAllData() const
		{
			IDevicePt ptIDevice;
			GetDefaultDevice(ptIDevice);
			return ptIDevice->GetAllData();
		}

		IPIMObjPts& GetAllData()
		{
			IDevicePt ptIDevice;
			GetDefaultDevice(ptIDevice);
			return ptIDevice->GetAllData();
		}

		CChangeLog& GetChangeLog()
		{
			IDevicePt ptIDevice;
			GetDefaultDevice(ptIDevice);
			return ptIDevice->GetChangeLog();
		}

        TSTRING GetDBFile()
        {
            TCHAR szFolder[MAX_PATH] = {0};
            bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("QSyncer"));
            assert(bRes);

            TSTRING strFile = m_strDatabase;
            Replace(strFile,_T(":"),_T("-"));
            Replace(strFile,_T(","),_T("_"));
            strFile = szFolder + strFile + _T(".db");
            return strFile;
        }

		virtual void GetDefaultDevice(IDevicePt& ptIDevice) const
		{
			ptIDevice = m_ptDefaultIDevice;
		}

		virtual void SetDefaultDevice(IDevicePt& ptIDevice)
		{
			//assert(!m_ptDefaultIDevice);
			//if(!m_ptDefaultIDevice)
				m_ptDefaultIDevice = ptIDevice;
		}

        virtual void AddRemoteDevice(IDevicePt& ptIDevice,IPIMDBPt& ptIPIMDB) 
        {
            IDevicePtsCIter Iter = find(m_ptRemoteIDevices.begin(),m_ptRemoteIDevices.end(),ptIDevice);
            if(Iter == m_ptRemoteIDevices.end())
			{
                m_ptRemoteIDevices.push_back(ptIDevice);
				ptIDevice->SetDatabase(ptIPIMDB);
			}
        }

        virtual void RemoveRemoteDevice(IDevicePt& ptIDevice) 
        {
            IDevicePtsIter Iter = find(m_ptRemoteIDevices.begin(),m_ptRemoteIDevices.end(),ptIDevice);
            if(Iter != m_ptRemoteIDevices.end())
                m_ptRemoteIDevices.erase(Iter);
        }

        virtual void RemoveAllRemoteDevice()
        {
            m_ptRemoteIDevices.clear();
        }

        virtual bool FindDevice(const TSTRING& strDeviceName,IDevicePt& ptIDevice)
        {
            bool bRes = false;
            IDevicePtsIter Iter;
            for(Iter = m_ptRemoteIDevices.begin();Iter != m_ptRemoteIDevices.end();Iter++)
            {
                if((*Iter)->GetUniqueName() == strDeviceName)
                    break;
            }
            if(Iter != m_ptRemoteIDevices.end())
            {
                ptIDevice = *Iter;
                bRes = true;
            }
            return bRes;
        }

        virtual bool GetItemByGUID(const GUID& guid,IPIMObjPt& ptIPIMObj)
        {
            IPIMObjPtsIter Iter;
            bool bRes = GetItemByGUID(guid,Iter);
            if(bRes)
                ptIPIMObj = *Iter;
            return bRes;
        }

        virtual bool GetChangeCount(int& nCount)
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            for(IPIMObjPtsCIter Iter = ptIPIMObjPts.begin();Iter != ptIPIMObjPts.end();Iter++)
            {
                if((*Iter)->IsModify())
                {
                    GetChangeLog().Update((*Iter)->GetGUID());
                    (*Iter)->ResetModify();
                }
            }

            nCount = GetChangeLog().GetChangeCount();
            return true;
        }

        virtual bool GetChangeByIndex(int& nIndex,ChangeLogData& changeData)
        {
            return GetChangeLog().GetChangeByIndex(nIndex,changeData);
        }

        virtual void ResetChange(int nIndex)
        {
            GetChangeLog().Reset(nIndex);
        }
    protected:
        const long long GetSize()
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            long long nSize = 0;
            for(IPIMObjPtsIter Iter = ptIPIMObjPts.begin();Iter != ptIPIMObjPts.end();Iter++)
                nSize += (*Iter)->GetSize();
            return nSize;
        }

        void Replace(TSTRING& str,const TSTRING& strBefore,const TSTRING& strAfter)
        {			
            TSTRING::size_type nIndex = -1;
            nIndex = str.find ( strBefore , 0 );
            while(nIndex != TSTRING::npos)
            {
                str.replace(nIndex,strBefore.size(),strAfter);
                nIndex = str.find ( strBefore , 0 );
            };
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

        const bool SetLUID(const IPIMObjPt& ptIPIMObj, long nLuid)
        {
            CommonDataPt spData = new CommonData;
            spData->SetName("LUID");
            spData->SetInteger(nLuid);			
            bool bRes = ptIPIMObj->SetByName("LUID",spData) != -1;
            if(bRes)
                ptIPIMObj->ResetModify();
            return bRes;
        }

        bool GetItemByGUID(const GUID& guid,IPIMObjPtsIter& Iter)
        {
			IPIMObjPts&	ptIPIMObjPts = GetAllData();

            bool bRes = false;

            assert(m_bOpened);
            if(m_bOpened)
            {
                for(Iter = ptIPIMObjPts.begin();Iter != ptIPIMObjPts.end();Iter++)
                {
                    if((*Iter)->GetGUID() == guid)
                    {
                        bRes = true;
                        break;
                    }
                }
            }
            return bRes;
        }
    private:
        PIMDBHeader						m_Header;
        vector<PIMSpecData>			m_vectSPECs;
        DBStatus								m_DBStatus;
        IPIMPt										m_ptIPIM;
        TSTRING								m_strDatabase;
        HANDLE								m_hEvent;
        bool											m_bOpened;
		IDevicePts								m_ptRemoteIDevices;
		IDevicePt								m_ptDefaultIDevice;
    };

    class CSyncManager : public CINotifyBase
    {
    public:	
        CSyncManager(void)
		{
			GetDeviceFinder()->Register(this);
		}
        virtual ~CSyncManager(void)
        {
            RemoveAll();
			GetDeviceFinder()->Unregister(this);
        }
        void RemoveAll() 
		{
            for(DBMAPIter Iter = m_mapDatabase.begin();Iter != m_mapDatabase.end();Iter++)
                Iter->second->RemoveAllRemoteDevice();
            m_mapDatabase.clear();
        }
    public:
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(strEvent == DEVICE_CONNECT)
			{
			}
			else if(strEvent == DEVICE_DISCONNECT)
			{
				m_mapDatabase.clear();
			}
		}
		bool CreatePIMDB(const TSTRING& strDatabase,IPIMPt& ptIPIM,IPIMDBPt& ptIPIMDB)
        {//strDatabase == _T("ID:IME004401480077789,MODULE:E81,TYPE:vcalendar")
            bool bRes = false;

            DBMAPIter Iter = m_mapDatabase.find(NormalizeDatabaseName(strDatabase).c_str());
            if(Iter == m_mapDatabase.end())
            {
                IPIMDBPt ptNewIPIMDB = new CPIMDB(strDatabase,ptIPIM);
				
                m_mapDatabase.insert ( DBMapPair ( strDatabase,ptNewIPIMDB));
                Iter = m_mapDatabase.find(strDatabase.c_str());
				ConnectToRemoteDevice(strDatabase,ptNewIPIMDB);
            }
            if(Iter != m_mapDatabase.end())
            {
                ptIPIMDB = Iter->second;
                bRes = true;
            }
            return bRes;
        }
    protected:
        TSTRING NormalizeDatabaseName(const TSTRING& strDatabase)		
        {
            TSTRING strNormalizeDatabaseName;

            std::map<TSTRING,TSTRING> ItemMap;
            ParserDatabaseName(strDatabase,ItemMap);
            TSTRING strID		= FindItem(ItemMap,_T("ID"));
            TSTRING strModule	= FindItem(ItemMap,_T("MODULE"));
            TSTRING strType		= FindItem(ItemMap,_T("TYPE"));

            ASSERT(strID.size() > 0 && strModule.size() > 0 && strType.size() > 0);//check strDatabase if correct

            strNormalizeDatabaseName = _T("ID:") + strID + _T(",MODULE:") + strModule + _T(",TYPE:") + strType;
            return strNormalizeDatabaseName;
        }

        void InsertToMap(std::map<TSTRING,TSTRING>& ItemMap,TCHAR* szItem)
        {//Item = _T("ID:IME004401480077789")
            TSTRING strName,strValue;
            if(ParserItem(szItem,strName,strValue))
                InsertItemToMap(ItemMap,strName,strValue);
        }

        void InsertItemToMap(std::map<TSTRING,TSTRING>& ItemMap,const TSTRING& strName,const TSTRING& strValue)
        {//Name = _T("ID"), Value = _T("IME004401480077789")
            ItemMap.find(strName.c_str());
            if(ItemMap.find(strName) == ItemMap.end())
                ItemMap.insert(std::pair<TSTRING,TSTRING>(strName,strValue));
        }

        bool ParserItem(TCHAR* szItem,TSTRING& strName,TSTRING& strValue)
        {//Item = _T("ID:IME004401480077789")
            bool bRes = false;
            if(szItem && ::_tcslen(szItem) > 0)
            {
                TCHAR* szNextToken;
                TCHAR* szToken = ::_tcstok_s(szItem, _T(":"),&szNextToken);
                if(szToken != NULL)
                {
                    strName = szToken;
                    szToken = ::_tcstok_s( NULL, _T(":"),&szNextToken);
                    if(szToken) 
                    {
                        strValue = szToken;
                        bRes = true;
                    }
                }
            }
            return bRes;
        }

        void ParserDatabaseName(const TSTRING& strDatabase,std::map<TSTRING,TSTRING>& ItemMap)
        {//sample: _T("ID:IME004401480077789,MODULE:E81,TYPE:vcalendar");
            ASSERT(strDatabase.size() < 1024);
            if(strDatabase.size() < 1024)
            {
                TCHAR szBuffer[1024] = {0};
                ::_tcscpy_s(szBuffer,1024,strDatabase.c_str());

                TCHAR* szNextToken;
                TCHAR szSeps[] = {_T(",")};
                TCHAR* szToken = ::_tcstok_s( szBuffer, szSeps,&szNextToken);
                while(szToken != NULL)
                {			
                    InsertToMap(ItemMap,szToken);
                    szToken = ::_tcstok_s( NULL, szSeps,&szNextToken);
                }
            }
        }

        const TSTRING FindItem (const std::map<TSTRING,TSTRING>& ItemMap,const TSTRING& strKey)
        {
            TSTRING strTemp;
            std::map<TSTRING,TSTRING>::const_iterator cIter = ItemMap.find(strKey);
            if(cIter != ItemMap.end())
                strTemp = cIter->second;
            return strTemp;
        }

		bool AddRemoteDevice(CQSetting& oSetting,const TSTRING& strType,IPIMDBPt& ptIPIMDB)
		{
			TSTRING strFindMobileInterfaceDeviceName;
			TSTRING strFindConfigurationDeviceName;
			if(strType == _T("vcalendar"))
			{
				strFindMobileInterfaceDeviceName = _T("MSCalendarDevice");
			}
			else if(strType == _T("vcard"))
			{
				strFindMobileInterfaceDeviceName = _T("MSPhonebookDevice");
			}
			else assert(0);// Not Define

			bool bRes = false;

			//Get devices from MobileInterface
			int nMobileInterfaceCount = CommGetMobileInterfaceCount();
			for(int nMobileIndex = 0;nMobileIndex < nMobileInterfaceCount;nMobileIndex++)
			{
				IDevicePt ptSampleIDevice;
				TCHAR szPIMDeviceName[MAX_RET_STRING_LENGTH] = {0};
				CommGetPIMDevice(_T("QSyncerApp"),strFindMobileInterfaceDeviceName.c_str(),szPIMDeviceName,nMobileIndex);

				TCHAR szMobileModel[MAX_RET_STRING_LENGTH] = {0};
				TCHAR szIMEI[MAX_RET_STRING_LENGTH] = {0};
				TCHAR szDisplayName[MAX_RET_STRING_LENGTH] = {0};
				CommGetMobileInfo(_T("CQSyncer2App"), szMobileModel, szIMEI, szDisplayName,nMobileIndex);

				if(_tcscmp(szPIMDeviceName, _T("QPhonebookDevice")) == 0)
				{
					IPHONEBOOK* pIPHONEBOOK = (IPHONEBOOK*)CommCreateInterface(_T("QSyncer"),_T("PBClient"),eIPhonebook,nMobileIndex);
					ptSampleIDevice = new QPhonebookDevice(pIPHONEBOOK,szDisplayName);
				}
				else if(_tcscmp(szPIMDeviceName, _T("QCalendarDevice")) == 0)
				{
					ICALENDAR* pICALENDAR = (ICALENDAR*)CommCreateInterface(_T("QSyncer"),_T("CalendarClient"),eICalendar,nMobileIndex);
					ptSampleIDevice = new QCalendarDevice(pICALENDAR,szDisplayName);
				}
				else if(_tcscmp(szPIMDeviceName,_T("SyncMLDevice")) == 0)
				{
					ISYNCML* pISyncML = (ISYNCML*)CommCreateInterface(_T("SYNCML_APP"),_T("Test ISyncML"),eISyncML,nMobileIndex);
					ptSampleIDevice = new SyncMLDevice(strType,pISyncML,szDisplayName);
				}

				if(ptSampleIDevice)
                {
                    ptSampleIDevice->SetDatabase(ptIPIMDB);
                    ptIPIMDB->SetDefaultDevice(ptSampleIDevice);
					bRes = true;
                }
			}
			return bRes;
		}

        bool ConnectToRemoteDevice(const TSTRING& strDatabase,IPIMDBPt& ptIPIMDB)
        {//try to connect to device from protocol, ask it by config
            bool bRes = false;

            std::map<TSTRING,TSTRING> ItemMap;
            ParserDatabaseName(strDatabase,ItemMap);
            TSTRING strID		= FindItem(ItemMap,_T("ID"));
            TSTRING strModule	= FindItem(ItemMap,_T("MODULE"));
            TSTRING strType		= FindItem(ItemMap,_T("TYPE"));
            
			//Try to get device from modal name
			CQMobileConfigure* pConfigure = GetMobileConFig();
			if(pConfigure)
			{
				if(pConfigure->LoadDefaultXML())
				{
					CQSetting oMobSetting;
					if(pConfigure->GetMobileSettings( strModule, oMobSetting ))
					{
						CQSetting oISetting;
						if(pConfigure->GetSettings( _T("PIMDevices"), oISetting))
							bRes = AddRemoteDevice(oISetting,strType,ptIPIMDB);
					}
				}
			}

            return bRes;
        }

        void SaveToDisk(IPIMDBPt& ptIPIMDB)
        {
            ptIPIMDB->Save();
        }

        void LoadFromDisk(IPIMDBPt& ptIPIMDB)
        {
            ptIPIMDB->Load();
        }
    private:
        DBMAP m_mapDatabase;
    };

    static CSyncManager g_SyncManager;

    void RemoveSyncManagerDevice()
    {
        g_SyncManager.RemoveAll();
    }

	bool CreatePIMDB(const TSTRING& strDatabase,IPIMPt& ptIPIM,IPIMDBPt& ptIPIMDB)
    {
        return g_SyncManager.CreatePIMDB(strDatabase,ptIPIM,ptIPIMDB);
    }
};

#pragma warning(pop)