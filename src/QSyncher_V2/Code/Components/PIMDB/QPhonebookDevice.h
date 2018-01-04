#ifndef _DEFINE_QPhonebook_Device_H_
#define _DEFINE_QPhonebook_Device_H_

#pragma once

#include "..\..\Components\PIMDB\CPIMDB.h"
#include "..\..\Components\PIMDB\PIMObjectbase.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ICommMobile.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\IPhonebook.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\IPIMConfig.h"

namespace QSYNC_LIB
{
	class QPhonebookDevice : public DeviceBase
	{
		class PBKey
		{
		public:
			PBKey(){}
			PBKey(GUID guid,UINT nID = 0,ePhonebookType eType = ePhonebookAll):m_guid(guid),m_nID(nID),m_eType(eType){}
			~PBKey(){}
			bool operator<(const PBKey& rhs) const 
			{
				bool bRes = false;
				if(m_guid.Data1 != rhs.m_guid.Data1)
					bRes = m_guid.Data1 < rhs.m_guid.Data1;
				else if(m_guid.Data2 != rhs.m_guid.Data2)
					bRes = m_guid.Data2 < rhs.m_guid.Data2;
				else if(m_guid.Data3 != rhs.m_guid.Data3)
					bRes = m_guid.Data3 < rhs.m_guid.Data3;
				else
				{
					//unsigned char  Data4[ 8 ];
					//for performance issue, it almost to be equal if Data1, Data2 and Data3 are the same
					//bRes = false;
				}
				return bRes;
			}
			bool operator==(const PBKey& rhs) const 
			{
				bool bRes = m_guid.Data1 == rhs.m_guid.Data1;
				if(bRes == true)
					bRes = m_guid.Data2 == rhs.m_guid.Data2;
				else if(bRes == true)
					bRes = m_guid.Data3 == rhs.m_guid.Data3;
				else if(bRes == true)
					bRes = m_guid.Data4 == rhs.m_guid.Data4;
				return bRes;
			}
			
			GUID							m_guid;
			UINT							m_nID;
			ePhonebookType		m_eType;
		};

	public:
		QPhonebookDevice(IPHONEBOOK* pIPHONEBOOK,const TSTRING& strDisplayName)
			:DeviceBase(_T("QPhonebook"))
			,m_pIPHONEBOOK(pIPHONEBOOK)
			,m_pIPIMConfig(NULL)
		{
			SetDisplayName(strDisplayName);
		}
		virtual ~QPhonebookDevice()
		{
			ReleaseInterface();
			m_pIPIMConfig = NULL;
		}
		virtual bool CanSync() const
		{
			return true;
		}
		virtual bool Sync(SyncType syncType)
		{
			bool bRes = true;
			m_syncType = syncType;
			CreateThread(NULL,0,ThreadProc,this,syncType,NULL);
			return TRUE;
		}
		virtual bool	InsertItem(IPIMObjPt& ptIPIMObj)
		{
			TSTRING strRaw;
			ptIPIMObj->encode(strRaw);
			
			ePhonebookType nPBType = ePhonebookAll;
			int nTemp;
			if(GetPIMInteger("PBTYPE",ptIPIMObj,nTemp))
			{
			if(nTemp == 0)
				nPBType = ePhonebookSIM;
			else if(nTemp == 1)
				nPBType = ePhonebookME;
			}
			else
			{
				//Set default PBTYpe = ePhonebookME
				nPBType = ePhonebookME;
				CommonDataPt ptType = new CommonData("PBTYPE");
				ptType->SetInteger(nPBType);
				ptIPIMObj->SetByName(ptType->GetName(),ptType);
			}

			/*
			002381: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0465700 +0.0157184
			Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
			Send 0x16 bytes to the device:
			41 54 24 42 51 56 43 41 52 44 57 3D 22 53 4D 22   AT$BQVCARDW="SM"
			2C 30 2C 39 33 0D                                 ,0,93.
			002382: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0465700 +0.0
			Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
			Get 0x6 bytes from the device:
			0D 0A 4F 4B 0D 0A                                 ..OK..

			002383: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0622884 +0.0157184
			Pipe Handle: 0x8954f98c (Endpoint Address: 0x81)
			Get 0xa bytes from the device:
			A1 20 00 00 00 00 02 00 03 00                     ? ........

			002384: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0622884 +0.0
			Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
			Send 0x5d bytes to the device:
			42 45 47 49 4E 3A 56 43 41 52 44 0D 0A 56 45 52   BEGIN:VCARD..VER
			53 49 4F 4E 3A 32 2E 31 0D 0A 4E 3A 53 49 4D 39   SION:2.1..N:SIM9
			39 39 0D 0A 54 45 4C 3B 56 4F 49 43 45 3B 48 4F   99..TEL;VOICE;HO
			4D 45 3A 39 39 39 39 39 0D 0A 58 2D 42 45 4E 51   ME:99999..X-BENQ
			3B 50 72 65 66 65 72 4C 69 73 74 3A 4E 4F 4E 45   ;PreferList:NONE
			0D 0A 45 4E 44 3A 56 43 41 52 44 0D 0A            ..END:VCARD..

			002385: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0937252 +0.0314368
			Pipe Handle: 0x8954f98c (Endpoint Address: 0x81)
			Get 0xa bytes from the device:
			A1 20 00 00 00 00 02 00 03 00                     ? ........

			002386: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0937252 +0.0
			Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
			Get 0x2 bytes from the device:
			0D 0A                                             ..

			002387: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.0937252 +0.0
			Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
			Get 0x10 bytes from the device:
			24 42 51 56 43 41 52 44 57 3A 34 30 39 37 0D 0A   $BQVCARDW:4097..


			002388: Bulk or Interrupt Transfer (UP), 07.12.2007 08:35:42.1094436 +0.0157184
			Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
			Get 0x6 bytes from the device:
			0D 0A 4F 4B 0D 0A                                 ..OK..
			*/

			bool bRes = false;
			string strAnsiCode = widetoansi(strRaw,CP_ACP);
			strAnsiCode += "\r\n";
			UINT nRetID = 0;
			int nRetError = 0;
			if(m_pIPHONEBOOK->WriteVCardbyIndex(nPBType,nRetID,nRetError,const_cast<char*>(strAnsiCode.c_str()),static_cast<int>(strAnsiCode.size())) == eCommRetSuccess)
			{
				bRes = (nRetID != -1);
			}

			if(bRes)
			{
				bRes = Internal_InsertItem(ptIPIMObj);				
				PBKey key(ptIPIMObj->GetGUID(),nRetID,nPBType);
				m_PBMappingData.insert(std::pair<PBKey,PBKey>(key,key));
			}
			else
			{
				//將錯誤訊息值放入PIMObj裡
				CommonDataPt ptRetError = new CommonData("VCARDERR");
				ptRetError->SetInteger(nRetError);
				ptIPIMObj->SetByName(ptRetError->GetName(),ptRetError);
			}

			return bRes;
		}
		virtual bool RemoveItem(IPIMObjPt& ptIPIMObj)
		{
			return DeletefromMS(ptIPIMObj->GetGUID());
		}
		virtual bool UpdateItem(IPIMObjPt& ptIPIMObj)
		{
			TSTRING strRaw;
			ptIPIMObj->encode(strRaw);
			return UpdatefromMS(ptIPIMObj->GetGUID(),strRaw);
		}
	protected:
		static DWORD WINAPI ThreadProc( LPVOID lpParameter)
		{
			QPhonebookDevice* pThis = (QPhonebookDevice*)lpParameter;
			switch(pThis->m_syncType)
			{
			case OneWayFromClient:
			case OneWayFromServer:
				break;
			case TwoWay:
				pThis->FetchAllData();
				break;
			case Slow:
				assert(0);
				break;
			default:
				assert(0);
				break;
			};
			return 0;
		}
		void SyncFromChange()
		{
			if(GetDatabase() != NULL)
			{
				//0. Get interface first
				GetInterface();

				//1. Sync status
				int nCount = 0;
				GetDatabase()->GetChangeCount(nCount);
				for(int i=0;i<nCount;i++)
				{
					ChangeLogData logData;
					GetDatabase()->GetChangeByIndex(i,logData); 
					
					TSTRING strRAW;
					switch(logData.m_ChangeLogStatus)
					{
					case ChangeLogStatus_Update:
						assert(0);
						break;
					case ChangeLogStatus_Insert:
						assert(0);
						break;
					case ChangeLogStatus_Delete:
						DeletefromMS(logData.m_Guid);
						break;
					case ChangeLogStatus_Replace:
						assert(0);//Need to implement
						break;
					default:
						assert(0);
						break;
					};
				}
			}
		}
		void FetchAllData()
		{			
			if(GetDatabase() != NULL)
			{
				//Get all data from mobile
				GetDatabase()->BeginTransaction();
				GetDatabase()->SetStatus(Transport);
				
				bool bRes = Extract(ePhonebookSIM);
				bRes = Extract(ePhonebookME);

				GetDatabase()->SetStatus(Ready);
				GetDatabase()->EndTransaction();
				GetDatabase()->Fire(g_strNotifyDBFinish,bRes ? 1 : 0);
			}
		}
		bool Extract(ePhonebookType eType)
		{
			bool bRes = true;
			if(m_pIPHONEBOOK)
			{
				if(m_pIPHONEBOOK->IsPhonebookTypeSupport(eType))
				{
					UINT m_nMinIndex = 0;
					UINT m_nMaxIndex = 0;
					UINT m_nMaxCount = 0;
					std::vector<UINT> vIdxList;
					if(bRes)
						bRes = m_pIPHONEBOOK->GetPhonebookRange(eType,m_nMinIndex,m_nMaxIndex,m_nMaxCount);					
					if(bRes)
						bRes = m_pIPHONEBOOK->ListPhonebookIndex(eType,&vIdxList);

					if(!m_pIPIMConfig && bRes)
						m_pIPIMConfig = (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("Phonebook"), eIPIMConfigure);

					for(std::vector<UINT>::iterator Iter = vIdxList.begin();Iter != vIdxList.end() && bRes ;Iter++)
					{
						int nRetLen = 0;
						TSTRING strLimitation;
						m_pIPIMConfig->GetProperty(_T("PhonebookConfig"), _T("MaxRecordSize"), strLimitation);
						int nBufferSize = 2048*3;
						if(!strLimitation.empty())
						{
							int nLimitation = _ttoi(strLimitation.c_str());
							nBufferSize = nLimitation != 0 ? nLimitation*3 : 2048*3;
						}
						char* szBuffer = new char[nBufferSize];
						::memset(szBuffer, 0, nBufferSize);
						if(m_pIPHONEBOOK->ReadVCardbyIndex(eType,*Iter,szBuffer,nBufferSize,nRetLen) == eSuccess)
						{
#ifdef _UNICODE
							size_t nAnsiLen = strlen(szBuffer);
							wchar_t* wszTemp = new wchar_t[nAnsiLen+1];
							memset(wszTemp,0,(nAnsiLen+1)*sizeof(wchar_t));
							AnsiToUnicodeBuffer(szBuffer,wszTemp,static_cast<int>((nAnsiLen+1)*sizeof(wchar_t)));
							InsertIPIM(wszTemp,eType,*Iter);
							delete [] wszTemp;
#else
							InsertIPIM(szBuffer);
#endif							
						}
						else 
						{
							bRes = false;
						}
						if(szBuffer)
							delete [] szBuffer;
						if(bRes == false)
							break;
					}
				}
				else bRes = false;
			}
			return bRes;
		}
	private:
		bool InsertIPIM(const TCHAR* szPIMRAW,ePhonebookType eType,UINT nPBID)
		{
			bool bRes = false;

			IPIMObjPt pimObjPt;
			GetDatabase()->CreatePIMObject(pimObjPt);
			if(pimObjPt->decode(szPIMRAW))
			{
#ifdef _DEBUG
				TSTRING strRaw;
				pimObjPt->encode(strRaw);
#endif

				CommonDataPt ptType = new CommonData("PBTYPE");
				ptType->SetInteger(eType);
				pimObjPt->SetByName(ptType->GetName(),ptType);

				Internal_InsertItem(pimObjPt,true);	
				PBKey key(pimObjPt->GetGUID(),nPBID,eType);
				m_PBMappingData.insert(std::pair<PBKey,PBKey>(key,key));
				bRes = true;
			}
			assert(bRes);

			return bRes;
		}

		bool GetKeyByGuid(const GUID& guid,PBKey& key)
		{
			bool bFound = false;
			PBKey keytemp(guid);
			std::map<PBKey,PBKey>::iterator Iter = m_PBMappingData.find(keytemp);
			if(Iter != m_PBMappingData.end())
			{
				key = Iter->second;
				bFound = true;
			}			
			return bFound;
		}

		bool DeletefromMS(const GUID& guid)
		{
			bool bRes = false;
			bool bFound = false;
			PBKey key;
			if(GetKeyByGuid(guid,key))
			{
				bRes = (0 != m_pIPHONEBOOK->RemoveVCardbyIndex(key.m_eType,key.m_nID));
				m_PBMappingData.erase(PBKey(guid));
			}
			return bRes;
		}

		bool UpdatefromMS(const GUID& guid,const TSTRING& strRaw)
		{		
			bool bRes = false;
			PBKey key;
			if(GetKeyByGuid(guid,key))
			{
				string strAnsiCode = widetoansi(strRaw,CP_ACP);
				strAnsiCode += "\r\n";
				UINT nRetID = key.m_nID;
				int nRetError = 0;
				if(m_pIPHONEBOOK->WriteVCardbyIndex(key.m_eType,nRetID,nRetError,const_cast<char*>(strAnsiCode.c_str()),static_cast<int>(strAnsiCode.size())) == eCommRetSuccess)
				{
					bRes = (nRetID != -1);
				}
			}
			return bRes;
		}

		IPHONEBOOK* GetInterface()
		{
			assert(m_pIPHONEBOOK);
			return m_pIPHONEBOOK;
		}
		void ReleaseInterface()
		{			
			if(m_pIPHONEBOOK)
			{
				CommDeleteInterface(m_pIPHONEBOOK);
				m_pIPHONEBOOK = NULL;
			}
		}

		bool GetPIMInteger(const char *szName,IPIMObjPt& ptPIM,int& nVal)
		{
			bool bRes = false;
			CommonDataPt spData;
			size_t nNotFoundIndex = -1;
			if(ptPIM->GetByName(szName,spData) != nNotFoundIndex && spData)
			{
				bRes = spData->GetType() == Integer;
				if(bRes)
					nVal = spData->GetInteger();
			}
			return bRes;
		}

		bool GetPIMString(const char *szName,IPIMObjPt& ptPIM,TSTRING& strValue)
		{
			bool bRes = false;
			CommonDataPt spData;
			size_t nNotFoundIndex = -1;
			if(ptPIM->GetByName(szName,spData) != nNotFoundIndex && spData)
			{
#ifdef _UNICODE
				bRes = spData->GetType() == WString;
				if(bRes)
					strValue = spData->GetWString();
#else
				bRes = spData->GetType() == String;
				if(bRes)
					strValue = spData->GetString();
#endif
			}
			return bRes;
		}
	private:
		IPHONEBOOK*					m_pIPHONEBOOK;
		std::map<PBKey,PBKey>		m_PBMappingData;
		SyncType					m_syncType;
		IPIMCONFIG*					m_pIPIMConfig;
	};
}

#endif // _DEFINE_QPhonebook_Device_H_