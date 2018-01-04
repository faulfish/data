#ifndef _SYNCML_SESSION_H_
#define _SYNCML_SESSION_H_

#include "..\..\CommonUtil\XMLManager.h"
#include "..\..\CommonUtil\MSXMLImpl.h"
#include "..\..\Components\WBXML\wbxmlSyncml.h"
#include "..\..\CommonUtil\Buffer.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ISYNCML.h"
#include "..\..\CommonUtil\LogPolicy.h"
#include "..\..\Components\PIMDB\ChangeLog.h"

namespace QSYNC_LIB
{

	enum SyncAction
	{
		SyncAction_Changes,
		SyncAction_Add,
		SyncAction_Replace,
		SyncAction_Delete,
		SyncAction_Copy,
		SyncAction_Atomic,
		SyncAction_Sequence
	};

	interface ISyncMLCallback
	{
		virtual bool Delete(const TCHAR* strSource) = 0;
		virtual bool Add(const TCHAR* szData,const TCHAR* strSource,const TCHAR* strType) = 0;
		virtual bool Replace(const TCHAR* szData,const TCHAR* strSource,const TCHAR* strType) = 0;
		virtual bool SetCount(int& nCount) = 0;
		virtual int	 GetChangesCount() = 0;
		virtual bool GetChangeByIndex(int nIndex,QSYNC_LIB::ChangeLogData& syncActionPara) = 0;
		virtual void ResetChange(int nIndex) = 0;
		virtual bool GetDataByGUID(const GUID& guid,TSTRING& strRAW) = 0;
		virtual bool SetAnchor(int nLast,int nNext) = 0;
		virtual int  GetAnchor() = 0;
		virtual bool Map(const TCHAR* strTarget, const TCHAR* strSource) = 0;
		virtual void EnableLog(bool bEnable) = 0;
	};

	class MapItem
	{
	public:
		MapItem(const TSTRING& strTarget,const TSTRING& strSource):m_strTarget(strTarget),m_strSource(strSource){}
		const TSTRING& Target() const {return m_strTarget;}
		const TSTRING& Source() const {return m_strSource;}
	private:
		TSTRING m_strTarget;
		TSTRING m_strSource;
	};

	class CSyncMLSessionBase
	{
	public:
		void Run()
		{	
			Open();
			TSTRING strXML;
			do
			{			
				strXML = AddSyncML(AddHeader(),AddBody());
				if(Put(strXML) && !IsFinal(strXML,true))
					strXML = Get();
				else strXML.clear();			
			}
			while(IsFinal(strXML,false) == false);
			Close();
		}
	protected:
		CSyncMLSessionBase(ISYNCML* pISyncML,ISyncMLCallback *pISyncMLCallback ,const TSTRING& strLocalURI,int nMaxMsgSize = 10000)
			:m_pISyncML(pISyncML)
			,m_pISyncMLCallback(pISyncMLCallback)
			,m_nSession(SessionID())
			,m_strTargetURI(_T("/"))
			,m_strLocalURI(strLocalURI)
			,m_nMaxMsgSize(nMaxMsgSize)
			,m_nCmdID(1)
			,m_nMsgID(0)
			,m_nTargetMaxMsgSize(0)
			,m_nTargetMaxObjSize(0)
			,m_bMoreData(false){}
		virtual ~CSyncMLSessionBase(){}
		/********************************************************************************************************/
		/*
		Function int IsExceedPackage()

		Return :
		0			means not exceed the maximum size ot the maxinum never been assigned
		Not 0	means the exceed size
		*/
		/********************************************************************************************************/
		int IsExceedPackage()
		{
			int nRes = 0;			
			Put(AddSyncML(AddHeader(),AddBody(true)), true);//For test only
			int nMaxSize = m_nTargetMaxMsgSize > m_nTargetMaxObjSize ? m_nTargetMaxMsgSize : m_nTargetMaxObjSize;
			if(nMaxSize > 0 && m_dwTestLen > m_nTargetMaxMsgSize)
					nRes = m_dwTestLen - m_nTargetMaxMsgSize;
			return nRes;
		}
		const TSTRING AddSyncML(const TSTRING& strHeader,const TSTRING& strBody)
		{
			return _T("<SyncML xmlns='syncml:SYNCML1.1'>\n") + strHeader + strBody + _T("</SyncML>\n");
		}
		const TSTRING AddHeader()
		{
			TCHAR szCommandFmt[] = {_T("\
			<SyncHdr>\n\
				<VerDTD>1.1</VerDTD>\n\
				<VerProto>SyncML/1.1</VerProto>\n\
				<SessionID>%d</SessionID>\n\
				<MsgID>%d</MsgID>\n\
				<Target>\n\
				   <LocURI>%s</LocURI>\n\
				</Target>\n\
				<Source>\n\
				   <LocURI>%s</LocURI>\n\
				</Source>\n\
				<Meta>\n\
				   <MaxMsgSize xmlns='syncml:metinf'>%d</MaxMsgSize>\n\
				</Meta>\n\
			</SyncHdr>\n\
			")};
			CString strCmd;
			strCmd.Format(szCommandFmt,m_nSession,m_nMsgID++,m_strTargetURI.c_str(),m_strLocalURI.c_str(),m_nMaxMsgSize);
			return LPCTSTR(strCmd);
		}
		const TSTRING AddBody(bool bTest = false)
		{
			TSTRING strCmds;
			for(std::vector<TSTRING>::iterator Iter = m_Cmds.begin();Iter != m_Cmds.end();Iter++)
				strCmds += *Iter;
			TSTRING strCmd = _T("<SyncBody>\n") + strCmds + _T("\n<Final/>\n</SyncBody>\n");
			if(bTest == false)
				m_Cmds.clear();
			return strCmd;
		}
		const TSTRING AddUpdateCommand(int nLuid,int nSize,const TSTRING& strData,bool bMoreData)
		{
			return InsertReplaceCommand(_T("Replace"),nLuid,nSize,strData,bMoreData);
		}
		const TSTRING AddInsertCommand(int nLuid,int nSize,const TSTRING& strData,bool bMoreData)
		{
			return InsertReplaceCommand(_T("Add"),nLuid,nSize,strData,bMoreData);
		}
		const TSTRING InsertReplaceCommand(const TSTRING& strCommand,int nLuid,int nSize,const TSTRING& strData,bool bMoreData)
		{
			size_t	nlen = strData.size();
			for( size_t c = 0; c < strData.size(); c++ )
			{
				if(c > 0 && strData[c] == '\n' && strData[c-1] != '\r')
					nlen++;
			}

			TCHAR szTargetFmt[] = {_T("\
									  <Target>\n\
										<LocURI>./%d</LocURI>\n\
									  </Target>\n\
									  ")};
			CString strTraget;
			if(strCommand == _T("Replace"))
				strTraget.Format(szTargetFmt,nLuid);

			TCHAR szSyncFmt[] = {_T("\
									<%s>\n\
									<CmdID>%d</CmdID>\n\
									<Item>\n\
										<Source>\n\
											<LocURI>./%d</LocURI>\n\
										</Source>\n\
										%s\
										<Meta>\n\
											<Type xmlns='syncml:metinf'>text/x-vcalendar</Type>\n\
											<Size xmlns='syncml:metinf'>%d</Size>\n\
										</Meta>\n\
										<Data xmlns='syncml:SYNCML1.1'>%s<Cmd/></Data>\n\
										%s\
									</Item>\n\
									</%s>\n\
									")};
			CString strCmd;
			TSTRING strMoreData = bMoreData ? _T("<MoreData/>\n") : _T("");
			strCmd.Format(szSyncFmt,strCommand.c_str(),m_nCmdID++,nLuid,strTraget,nlen,strData.c_str(),strMoreData.c_str(),strCommand.c_str());
			m_SyncCmds.push_back(LPCTSTR(strCmd));

			return LPCTSTR(strCmd);
		}
		const TSTRING AddDeleteCommand(int nLuid,bool bMoreData)
		{
			TCHAR szSyncFmt[] = {_T("\
									<Delete>\n\
										<CmdID>%d</CmdID>\n\
										<Item>\n\
											<Target>\n\
												<LocURI>./%d</LocURI>\n\
											</Target>\n\
										</Item>\n\
									</Delete>\n\
									")};
			CString strCmd;
			strCmd.Format(szSyncFmt,m_nCmdID++,nLuid);
			m_SyncCmds.push_back(LPCTSTR(strCmd));
			return LPCTSTR(strCmd);
		}
		const TSTRING AddSyncCommand(const TSTRING& strSourceURI,const TSTRING& strTargetURI)
		{
			TCHAR szSyncFmt[] = {_T("\
							<Sync>\n\
								<CmdID>%d</CmdID>\n\
								<Target>\n\
									<LocURI>%s</LocURI>\n\
								</Target>\n\
								<Source>\n\
									<LocURI>%s</LocURI>\n\
								</Source>\n\
								<NumberOfChanges>%d</NumberOfChanges>\n\
								%s\n\
							</Sync>\n")};

			int nSyncCmdID = m_nCmdID++;
			int nRealChangeCount = 0;

			assert(m_pISyncMLCallback);
			if(m_pISyncMLCallback)
			{
				int nChangeCount = m_pISyncMLCallback->GetChangesCount();
				int nStartFrom = 0;
				if(m_bMoreData)
					nStartFrom = m_nMoreDataIndex;

				for(int i=m_bMoreData;i<nChangeCount;i++)
				{
					using namespace QSYNC_LIB;

					ChangeLogData syncActionPara;
					if(m_pISyncMLCallback->GetChangeByIndex(i,syncActionPara))
					{
						TSTRING strRAW;
						switch(syncActionPara.m_ChangeLogStatus)
						{
						case ChangeLogStatus_Update:										
							m_pISyncMLCallback->GetDataByGUID(syncActionPara.m_Guid,strRAW);
							AddUpdateCommand(syncActionPara.m_nLUID,static_cast<int>(strRAW.size()+5+15),strRAW.c_str(),m_bMoreData);
							nRealChangeCount++;
							m_pISyncMLCallback->ResetChange(i);
							break;
						case ChangeLogStatus_Insert:
							{
								TSTRING strRes;
								m_pISyncMLCallback->GetDataByGUID(syncActionPara.m_Guid,strRAW);
								strRes = AddInsertCommand(syncActionPara.m_nLUID,static_cast<int>(strRAW.size()+5),strRAW.c_str(),m_bMoreData);
								if(!strRes.empty())
								{
									/////////////////////
									//For test only
									TSTRING strCmds;
									for(std::vector<TSTRING>::iterator Iter = m_SyncCmds.begin();Iter != m_SyncCmds.end();Iter++)
										strCmds += *Iter;

									CString strCmd;
									strCmd.Format(szSyncFmt,nSyncCmdID,strTargetURI.c_str(),strSourceURI.c_str(),nRealChangeCount,strCmds.c_str());
									m_Cmds.push_back(LPCTSTR(strCmd));
									
									LogPolicy::DebugLog(_T("SyncML::Command"));
									LogPolicy::DebugLog(strCmd);

									int nExceed = IsExceedPackage();
									m_Cmds.pop_back();

									if(nExceed > 0)
									{
										m_bMoreData = true;
										m_SyncCmds.pop_back();										
										assert(static_cast<int>(strRAW.size()) > nExceed);
										TSTRING strPart = strRAW.substr(0,strRAW.size() - nExceed).c_str();
										TSTRING strExceed = strRAW.substr(strRAW.size() - nExceed,nExceed).c_str();
										strRes = AddInsertCommand(syncActionPara.m_nLUID,static_cast<int>(strPart.size()+5),strPart.c_str(),m_bMoreData);
									}
									/////////////////////
									nRealChangeCount++;
									m_pISyncMLCallback->ResetChange(i);
								}
							}
							break;
						case ChangeLogStatus_Delete:
							AddDeleteCommand(syncActionPara.m_nLUID,m_bMoreData);
							nRealChangeCount++;
							m_pISyncMLCallback->ResetChange(i);
							break;
						case ChangeLogStatus_Replace:
							assert(0);//Need to implement
							break;
						default:
							break;
						};
					}

					if(m_bMoreData)
					{
						m_nMoreDataIndex = i;
						break;
					}
				};//end for
			}

			TSTRING strCmds;
			for(std::vector<TSTRING>::iterator Iter = m_SyncCmds.begin();Iter != m_SyncCmds.end();Iter++)
				strCmds += *Iter;

			CString strCmd;
			strCmd.Format(szSyncFmt,nSyncCmdID,strTargetURI.c_str(),strSourceURI.c_str(),nRealChangeCount,strCmds.c_str());
			m_Cmds.push_back(LPCTSTR(strCmd));
			return LPCTSTR(strCmd);
		}
		const TSTRING AddStatusCommand(const TSTRING& strSourceURI,const TSTRING& strTargetURI,const TSTRING& strCmdRef,int nMsgIDRef,int nCmdIDRef,int nRet,int nNext = -1)
		{
			TCHAR szItemFmt[] = {_T("\
									<Item>\n\
										<Data>\n\
											<Anchor xmlns='syncml:metinf'>\n\
												<Next>%d</Next>\n\
											</Anchor>\n\
										</Data>\n\
									</Item>\n\
									")};
			TCHAR szCommandFmt[] = {_T("\
									   <Status>\n\
											<CmdID>%d</CmdID>\n\
											<MsgRef>%d</MsgRef>\n\
											<CmdRef>%d</CmdRef>\n\
											<Cmd>%s</Cmd>\n\
											<TargetRef>%s</TargetRef>\n\
											<SourceRef>%s</SourceRef>\n\
											<Data>%d</Data>\n\
											%s\n\
									   </Status>\n\
									   ")};
			CString strItem;
			if(nNext != -1)
				strItem.Format(szItemFmt,nNext);
			CString strCmd;
			strCmd.Format(szCommandFmt,m_nCmdID++,nMsgIDRef,nCmdIDRef,strCmdRef.c_str(),strTargetURI.c_str(),strSourceURI.c_str(),nRet,strItem);
			m_Cmds.push_back(LPCTSTR(strCmd));
			if(nRet == 213)
			{
				//Need add Alert 222
			}
			return LPCTSTR(strCmd);
		}
		const TSTRING AddGetCommand(const TSTRING& strMeta,const TSTRING& strItem)
		{
			TCHAR szCommandFmt[] = {_T("\
									   <Get>\n\
											<CmdID>%d</CmdID>\n\
											<Meta>\
												<Type xmlns='syncml:metinf'>%s</Type>\n\
											</Meta>\n\
											<Item>\n\
												<Target>\n\
													<LocURI>%s</LocURI>\n\
												</Target>\n\
											</Item>\n\
									   </Get>\n\
									   ")};
			CString strCmd;
			strCmd.Format(szCommandFmt,m_nCmdID++,strMeta.c_str(),strItem.c_str());
			m_Cmds.push_back(LPCTSTR(strCmd));
			
			return LPCTSTR(strCmd);
		}
		const TSTRING AddAlertCommand(int nSyncType,const TSTRING& strTarget,const TSTRING& strSource,const TSTRING& strType,int nNext,int nLast)
		{
			m_strSyncTarget = strTarget;
			m_strSyncSource = strSource;
			TCHAR szCommandFmt[] = {_T("\
									   <Alert>\n\
											<CmdID>%d</CmdID>\n\
											<Data>%d</Data>\n\
											<Item>\n\
												<Target>\n\
													<LocURI>%s</LocURI>\n\
												</Target>\n\
												<Source>\n\
													<LocURI>%s</LocURI>\n\
												</Source>\n\
												<Meta>\n\
													<Type xmlns='syncml:metinf'>%s</Type>\n\
													<Anchor xmlns='syncml:metinf'>\n\
														<Next>%d</Next>\n\
														<Last>%d</Last>\n\
													</Anchor>\n\
												</Meta>\n\
											</Item>\n\
									   </Alert>\n\
									   ")};
			CString strCmd;
			strCmd.Format(szCommandFmt,m_nCmdID++,nSyncType,strTarget.c_str(),strSource.c_str(),strType.c_str(),nNext,nLast);
			m_Cmds.push_back(LPCTSTR(strCmd));
			return LPCTSTR(strCmd);
		}
		bool Open()
		{
			bool bRes = false;
			if(m_pISyncML)
				bRes = m_pISyncML->Open() == eSYNCMLRetSuccess;
			return bRes;
		}
		bool Close()
		{
			bool bRes = false;
			if(m_pISyncML)
				bRes = m_pISyncML->Close() == eSYNCMLRetSuccess;
			return bRes;
		}
		bool Put(const TSTRING& strXML,bool bTest =  false)
		{
			if(bTest == false)
			{
				LogPolicy::DebugLog(_T("SyncML::Put"));
				LogPolicy::DebugLog(strXML.c_str());
			}

			m_dwTestLen = 0;
			char* szTemp = NULL;
			XmlToWbxml(strXML.c_str(),static_cast<int>(strXML.size()),&szTemp,&m_dwTestLen);
			int nRet = eSYNCMLSRetFail;
			if(szTemp && m_dwTestLen > 0)
			{
				if(bTest == false)
					nRet = m_pISyncML->Put((BYTE*)szTemp,m_dwTestLen);
				delete [] szTemp;
			}
			return nRet == eSYNCMLRetSuccess;
		}
		const TSTRING Get()
		{
			LogPolicy::DebugLog(_T("SyncML::Get"));
			TSTRING strLocalXML;
			BYTE *pData = NULL;
			DWORD nSize = 0;
			if(m_pISyncML->Get(&pData,nSize) == eSYNCMLRetSuccess && nSize > 0)
			{
				DWORD dwLen = 0;
				CString strTemp;
				WbxmlToXml(pData,nSize,strTemp,dwLen);
				strLocalXML = LPCTSTR(strTemp);
				delete [] pData;
			}		
			LogPolicy::DebugLog(strLocalXML.c_str());
			return strLocalXML;
		}
		bool IsFinal(const TSTRING& strXML,bool bIsPut)
		{
			bool bIsFinal = true;
			if(!strXML.empty())
			{
				MSXMLLibrary mslib;
				XMLMANAGER::XMLDocument xmlDoc(&mslib);
				xmlDoc.loadXML((const BSTR)strXML.c_str());

				if(xmlDoc.getAllElement()->size() == 1)
				{
					XMLAllElement SyncElements = xmlDoc.getAllElement()[0];
					if(SyncElements.size() == 1)
					{
						XMLAllElement SyncBodyElements = SyncElements[0].getAllElement()[0];
						if(SyncBodyElements.size() == 2)
						{
							ParserHeader(SyncBodyElements[0],bIsPut);
							bIsFinal = ParserBody(SyncBodyElements[1],bIsPut);
						}
					}
				}

				if(bIsFinal && !bIsPut)
					bIsFinal = false;
			}
			return bIsFinal;
		}
		void ParserHeader(XMLElement& Element,bool bIsPut)
		{
			int nMsgID = 0;
			XMLAllElement Elements = Element.getAllElement()[0];
			for(size_t i=0;i<Elements.size();i++)
			{
				if(Elements[i].name() == _T("Source"))
				{
					if(bIsPut == false && (m_strTargetURI.empty() || m_strTargetURI == _T("/")))
						GetLocURI(Elements[i],m_strTargetURI);
				}
				else if(Elements[i].name() == _T("Meta"))
				{
					if(bIsPut == false && m_nTargetMaxMsgSize == 0)
						GetItemMeta(Elements[i],m_nTargetMaxMsgSize,m_nTargetMaxObjSize);
				}
				else if(Elements[i].name() == _T("MsgID"))
				{
					if(bIsPut == false)
						nMsgID = _ttoi(Elements[i].value().c_str());
				}
			}
			if(bIsPut == false)
				AddStatusCommand(m_strLocalURI,m_strTargetURI,Element.name().c_str(),nMsgID,0,200);
		}
		bool ParserBody(XMLElement& Element,bool bIsPut)
		{
			bool bIsFinal = true;		
			XMLAllElement Elements = Element.getAllElement()[0];
			for(size_t i=0;i<Elements.size();i++)
			{
				if(Elements[i].name() == _T("Get"))
				{
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					int nCmdID = GetCmdID(tempElements);
					if(bIsPut == false)
						AddStatusCommand(m_strLocalURI,m_strTargetURI,Elements[i].name().c_str(),0,nCmdID,200);
					bIsFinal = false;
				}
				else if(Elements[i].name() == _T("Results"))
				{
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					int nCmdID = GetCmdID(tempElements);
					if(bIsPut == false)
						AddStatusCommand(m_strLocalURI,m_strTargetURI,Elements[i].name().c_str(),0,nCmdID,200);
					bIsFinal = false;
				}
				else if(Elements[i].name() == _T("Put"))
				{
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					int nCmdID = GetCmdID(tempElements);
					if(bIsPut == false)
						AddStatusCommand(m_strLocalURI,m_strTargetURI,Elements[i].name().c_str(),0,nCmdID,200);
					bIsFinal = false;
				}
				else if(Elements[i].name() == _T("Status"))
				{
					int nCmdIDRef = 0;
					int nData = 0;
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					size_t test = tempElements.size();
					for(size_t k=0;k<tempElements.size();k++)
					{
						if(tempElements[k].name() == _T("CmdRef"))
							nCmdIDRef = _ttoi(tempElements[k].value().c_str());
						else if(tempElements[k].name() == _T("Data"))
							nData = _ttoi(tempElements[k].value().c_str());
						else if(tempElements[k].name() ==_T("Item"))
							bIsFinal = false;
					}
				}
				else if(Elements[i].name() == _T("Alert"))
				{
					TSTRING strTarget,strSource,strType;
					int nNext = -1,nLast = -1,nAlertType = -1;
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					int nCmdID = GetCmdID(tempElements);
					for(size_t k=0;k<tempElements.size();k++)
					{
						bool bRes = true;
						bool bMoreData = false;
						if(tempElements[k].name() == _T("Item"))
							GetItem(tempElements[k],strTarget,strSource,strType,nNext,nLast,bMoreData);
						else if(tempElements[k].name() == _T("Data"))
							nAlertType = _ttoi(tempElements[k].value().c_str());
					}
					if(bIsPut == false)
					{
						//Current anchor nNext,nLast;
						if(m_pISyncMLCallback)
							m_pISyncMLCallback->SetAnchor(nLast,nNext);
						AddStatusCommand(m_strLocalURI,m_strTargetURI,Elements[i].name().c_str(),0,nCmdID,200,nNext);
					}
					bIsFinal = false;
				}
				else if(Elements[i].name() == _T("Sync"))
				{				
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					int nCmdID = GetCmdID(tempElements);				
					if(bIsPut == false)
					{
						int nRet = 500;

						AddSyncCommand(m_strSyncSource,m_strSyncTarget);

						bool bAllSucceed = true;
						for(size_t k=0;k<tempElements.size();k++)
						{
							bool bRes = true;
							if(tempElements[k].name() == _T("NumberOfChanges"))
								bRes = ImplementSyncAction(SyncAction_Changes,tempElements[k]);
							else if(tempElements[k].name() == _T("Add"))
								bRes = ImplementSyncAction(SyncAction_Add,tempElements[k]);
							else if(tempElements[k].name() == _T("Replace"))
								bRes = ImplementSyncAction(SyncAction_Replace,tempElements[k]);
							else if(tempElements[k].name() == _T("Delete"))
								bRes = ImplementSyncAction(SyncAction_Delete,tempElements[k]);
							else if(tempElements[k].name() == _T("Copy"))
								bRes = ImplementSyncAction(SyncAction_Copy,tempElements[k]);
							else if(tempElements[k].name() == _T("Atomic"))
								bRes = ImplementSyncAction(SyncAction_Atomic,tempElements[k]);
							else if(tempElements[k].name() == _T("Sequence"))
								bRes = ImplementSyncAction(SyncAction_Sequence,tempElements[k]);
							if(bRes == false)
								bAllSucceed = false;
						}

						if(m_pISyncMLCallback)
							m_pISyncMLCallback->ResetChange(-1);

						if(bAllSucceed)
							nRet = 200;

						if(m_bMoreData == false)
							AddStatusCommand(m_strLocalURI,m_strTargetURI,Elements[i].name().c_str(),0,nCmdID,nRet);					
					}
					bIsFinal = false;
				}
				else if(Elements[i].name() == _T("Add")){}
				else if(Elements[i].name() == _T("Delete")){}
				else if(Elements[i].name() == _T("Final")){}
				else if(Elements[i].name() == _T("Map"))
				{
					XMLAllElement tempElements = Elements[i].getAllElement()[0];
					std::vector<MapItem> mapItems;
					TSTRING strTarget;
					TSTRING strSource;
					GetMap(Elements[i],strTarget,strSource,mapItems);
					for(std::vector<MapItem>::iterator Iter = mapItems.begin();Iter != mapItems.end();Iter++)
					{
						if(m_pISyncMLCallback)
							m_pISyncMLCallback->Map(Iter->Target().c_str(),Iter->Source().c_str());
					}
				}
				else
				{
					ASSERT(0);//Need to implement
					bIsFinal = false;
				}
			}
			return bIsFinal;
		}
	protected:
		bool ImplementSyncAction(SyncAction action,XMLElement& element)
		{
			bool bRes = true;
			int nRet = 403;
			XMLElement dataElement;
			TSTRING strSource,strType;
			int nSize = -1;
			switch(action)
			{
			case SyncAction_Changes:
				{
					int nCount = -1;
					GetChanges(element,nCount);
					//if(m_pISyncMLCallback)
					//	bRes = m_pISyncMLCallback->SetCount(nCount);
					nRet = 200;
				}
				break;
			case SyncAction_Add:
				/*
				<Add>
				<CmdID>1005</CmdID>

				<Item>
				<Source>
				<LocURI>./2</LocURI>
				</Source>

				<Meta>
				<Type xmlns='syncml:metinf'>text/x-vcalendar</Type>

				<Size>319</Size>
				</Meta>

				<Data xmlns='syncml:SYNCML1.1'>BEGIN:VCALENDAR
				VERSION:1.0
				BEGIN:VEVENT
				SUMMARY;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:on
				DESCRIPTION;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:None
				LOCATION;CHARSET=UTF-8:None
				DTSTART:20070414T230000Z
				DTEND:20070415T000000Z
				DALARM:20070414T225500Z;P0000Y00M00DT00H00M00S;000
				UID:2
				END:VEVENT
				END:VCALENDAR
				</Data>
				<MoreData/>
				</Item>
				</Add>
				*/	
				nRet = DealAddReplace(_T("Add"),element,strSource,strType,nSize,dataElement);
				break;
			case SyncAction_Replace:
				/*
				<Replace>
				<CmdID>1005</CmdID>

				<Item>
				<Source>
				<LocURI>./2</LocURI>
				</Source>

				<Meta>
				<Type xmlns='syncml:metinf'>text/x-vcalendar</Type>

				<Size>380</Size>
				</Meta>

				<Data xmlns='syncml:SYNCML1.1'>
				BEGIN:VCALENDAR
				VERSION:1.0
				BEGIN:VEVENT
				UID:2
				DALARM:20000114T235500Z;P0000Y00M00DT00H00M00S;000
				RRULE:YM1  1 #0
				DTEND:20000115T010000Z
				DTSTART:20000115T000000Z
				SUMMARY;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:=E7=94=9F=E6=97=A5 =
				=E8=AA=92=E4=B8=80=E5=89=87=E7=82=BA=E4=B8=BB=E7=AE=A1=E9=81=93=E7=90=86=E7=
				=94=B1=E4=B8=AD=E5=85=B1=E8=AD=98
				END:VEVENT
				END:VCALENDAR
				</Data>
				<MoreData>
				</Item>
				</Replace>
				*/
				nRet = DealAddReplace(_T("Replace"),element,strSource,strType,nSize,dataElement);
				break;
			case SyncAction_Delete:
				/*
				<Delete>
				<CmdID>1005</CmdID>

				<Item>
				<Source>
				<LocURI>./1</LocURI>
				</Source>
				</Item>
				</Delete>
				*/
				GetData(element,strSource,strType,nSize,dataElement);
				if(m_pISyncMLCallback)
					bRes = m_pISyncMLCallback->Delete(strSource.c_str());
				if(bRes)
					nRet = 200;
				break;
			case SyncAction_Copy:
				break;
			case SyncAction_Atomic:
				break;
			case SyncAction_Sequence:
				break;
			default:
				ASSERT(0);
				break;
			};
			int nCmdID = GetCmdID(element.getAllElement()[0]);
			if(nCmdID != -1)
				AddStatusCommand(m_strLocalURI,m_strTargetURI,element.name().c_str(),0,nCmdID,nRet);
			return bRes;
		}
		int DealAddReplace(const TSTRING& strFunc,XMLElement& element,TSTRING& strSource,TSTRING& strType,int& nSize,XMLElement& dataElement)
		{
			int nRet = 403;
			if(m_bMoreData == false)
				m_strMoreData.clear();
			GetData(element,strSource,strType,nSize,dataElement,m_bMoreData);
			if(m_bMoreData)
			{
				m_strMoreDataUID = strSource;
				m_strMoreData += dataElement.value().c_str();
				nRet = 213;
				//Need add Alert 222
			}
			else
			{			
				bool bRes = false;
				if(strFunc == _T("Add"))
				{
					TSTRING strData = m_strMoreData + dataElement.value().c_str();
					if(m_pISyncMLCallback && m_bMoreData == false)
						bRes = m_pISyncMLCallback->Add(strData.c_str(),strSource.c_str(),strType.c_str());
					nRet = bRes ? 201 : 403;
				}
				else if(strFunc == _T("Replace"))
				{
					TSTRING strData = m_strMoreData + dataElement.value().c_str();
					if(m_pISyncMLCallback && m_bMoreData == false)
						bRes = m_pISyncMLCallback->Replace(strData.c_str(),strSource.c_str(),strType.c_str());
					nRet = bRes ? 200 : 403;
				}
				else assert(0);
			}
			return nRet;
		}
		void GetChanges(XMLElement& element,int &nCount)
		{
			nCount = ::_ttoi(element.value().c_str());
		}
		void GetData(XMLElement& element,TSTRING& strSource,TSTRING& strType,int& nSize,XMLElement& dataElement)
		{
			bool bMoreData = false;
			GetData(element,strSource,strType,nSize,dataElement,bMoreData);
		}
		void GetData(XMLElement& element,TSTRING& strSource,TSTRING& strType,int& nSize,XMLElement& dataElement,bool& bMoreData)
		{
			XMLAllElement tempElements = element.getAllElement()[0];
			for(size_t k=0;k<tempElements.size();k++)
			{
				if(tempElements[k].name() == _T("Item"))
				{
					TSTRING strTarget;
					int nNext = -1,nLast = -1,nMaxMsgSize = -1;				
					GetItem(tempElements[k],strTarget,strSource,strType,nNext,nLast,nSize,nMaxMsgSize,dataElement,bMoreData);
				}
				else if(tempElements[k].name() == _T("CmdID"))
				{
					continue;
				}
				else
				{
					TRACE(_T("Unknow : %s\n"),tempElements[k].name().c_str());
					assert(0);
				}
			}
		}
		void GetLocURI(XMLElement& element,TSTRING& strLocURI)
		{
			XMLAllElement tempElements = element.getAllElement()[0];
			ASSERT(tempElements.size() == 1);//assume element has only one element
			if(tempElements.size() == 1)
			{
				XMLAllElement tempElements = element.getAllElement()[0];
				strLocURI = tempElements[0].value().c_str();
			}
		}	
		void GetItemMeta(XMLElement& element,int& nMaxMsgSize,int& nMaxObjSize)
		{
			TSTRING strType;
			int nSize = -1,nNext = -1,nLast = -1;
			GetItemMeta(element,strType,nNext,nLast,nSize,nMaxMsgSize,nMaxObjSize);
		}
		void GetItemMeta(XMLElement& element,TSTRING& strType,int& nNext,int& nLast)
		{
			int nSize = -1,nMaxMsgSize = -1;
			int nMaxObjSize = 0;
			GetItemMeta(element,strType,nNext,nLast,nSize,nMaxMsgSize,nMaxObjSize);
		}
		void GetItemMeta(XMLElement& element,TSTRING& strType,int& nNext,int& nLast,int& nSize,int& nMaxMsgSize,int& nMaxObjSize)
		{
			XMLAllElement tempElements = element.getAllElement()[0];
			for(size_t k=0;k<tempElements.size();k++)
			{
				if(tempElements[k].name() == _T("Type"))
					strType = tempElements[k].value().c_str();
				else if(tempElements[k].name() == _T("Anchor") )
					GetAnchor(tempElements[k],nNext,nLast);
				else if(tempElements[k].name() == _T("Size") )
					nSize = _ttoi(tempElements[k].value().c_str());
				else if(tempElements[k].name() == _T("MaxMsgSize") )
					nMaxMsgSize = _ttoi(tempElements[k].value().c_str());
				else if(tempElements[k].name() == _T("MaxObjSize") )
					nMaxObjSize = _ttoi(tempElements[k].value().c_str());
				else
				{
					TRACE(_T("Unknow : %s\n"),tempElements[k].name());
					assert(0);
				}
			}
		}

		void GetMap(XMLElement& element,TSTRING& strTarget,TSTRING& strSource,std::vector<MapItem>& MapItems )
		{
			/*
			<Map>
			<CmdID>1006</CmdID>

			<Target>
			<LocURI>events</LocURI>
			</Target>

			<Source>
			<LocURI>events</LocURI>
			</Source>

			<MapItem>
			<Target>
			<LocURI>./0</LocURI>
			</Target>

			<Source>
			<LocURI>./187</LocURI>
			</Source>
			</MapItem>
			</Map>
			*/
			XMLAllElement tempElements = element.getAllElement()[0];		
			for(size_t k=0;k<tempElements.size();k++)
			{
				if(tempElements[k].name() == _T("Target"))
					GetLocURI(tempElements[k],strTarget);
				else if(tempElements[k].name() == _T("Source") )
					GetLocURI(tempElements[k],strSource);
				else if(tempElements[k].name() == _T("MapItem") )
					GetMapItem(tempElements[k],MapItems);
				else if(tempElements[k].name() == _T("CmdID") )
				{
					int nCmdID = GetCmdID(tempElements[k]);
					if(nCmdID != -1)
						AddStatusCommand(m_strLocalURI,m_strTargetURI,element.name().c_str(),0,nCmdID,200);
				}
				else
				{
					TRACE(_T("Unknow : %s\n"),tempElements[k].name().c_str());
					assert(0);
				}
			}
		}

		void GetMapItem(XMLElement& element,std::vector<MapItem>& MapItems)
		{
			/*
			<MapItem>
			<Target>
			<LocURI>./0</LocURI>
			</Target>

			<Source>
			<LocURI>./187</LocURI>
			</Source>
			</MapItem>
			*/
			TSTRING strTarget;
			TSTRING strSource;
			XMLAllElement tempElements = element.getAllElement()[0];
			for(size_t k=0;k<tempElements.size();k++)
			{
				if(tempElements[k].name() == _T("Target"))
					GetLocURI(tempElements[k],strTarget);
				else if(tempElements[k].name() == _T("Source") )
					GetLocURI(tempElements[k],strSource);
				else
				{
					TRACE(_T("Unknow : %s\n"),tempElements[k].name().c_str());
					assert(0);
				}
			}
			MapItems.push_back(MapItem(strTarget,strSource));
		}

		void GetAnchor(XMLElement& element,int& nNext,int& nLast)
		{
			XMLAllElement tempElements = element.getAllElement()[0];
			for(size_t k=0;k<tempElements.size();k++)
			{
				if(tempElements[k].name() == _T("Next") )
					nNext = _ttoi(tempElements[k].value().c_str());
				else if(tempElements[k].name() == _T("Last") )
					nLast = _ttoi(tempElements[k].value().c_str());
				else
				{
					TRACE(_T("Unknow : %s\n"),tempElements[k].name().c_str());
					assert(0);
				}
			}
		}
		void GetItem(XMLElement& element,TSTRING strTarget,TSTRING& strSource,TSTRING& strType,int& nNext,int& nLast,bool& bMoreData)
		{
			XMLElement dataElement;
			int nSize = -1,nMaxMsgSize = -1;
			GetItem(element,strTarget,strSource,strType,nNext,nLast,nSize,nMaxMsgSize,dataElement,bMoreData);
		}
		void GetItem(XMLElement& element,TSTRING strTarget,TSTRING& strSource,TSTRING& strType,int& nNext,int& nLast,int& nSize,int& nMaxMsgSize,XMLElement& dataElement,bool& bMoreData)
		{
			int nMaxObjSize = 0;
			bMoreData = false;
			XMLAllElement tempElements = element.getAllElement()[0];
			for(size_t k=0;k<tempElements.size();k++)
			{
				if(tempElements[k].name() == _T("Target"))
					GetLocURI(tempElements[k],strTarget);
				else if(tempElements[k].name() == _T("Source"))
					GetLocURI(tempElements[k],strSource);
				else if(tempElements[k].name() == _T("Meta"))
					GetItemMeta(tempElements[k],strType,nNext,nLast,nSize,nMaxMsgSize,nMaxObjSize);
				else if(tempElements[k].name() == _T("Data"))
					dataElement = tempElements[k];
				else if(tempElements[k].name() == _T("MoreData"))
					bMoreData = true;
				else
				{
					TRACE(_T("Unknow : %s\n"),tempElements[k].name().c_str());
					assert(0);
				}
			}
		}
		const int GetCmdID (const XMLAllElement& tempElements)
		{	
			int nCmdID = -1;
			for(size_t k=0;k<tempElements.size();k++)
			{
				nCmdID = GetCmdID(tempElements[k]);
				if(nCmdID != -1)
					break;
			}
			return nCmdID;
		}
		const int GetCmdID (const XMLElement& element)
		{
			int nCmdID = -1;
			if(element.name() == _T("CmdID") )
				nCmdID = _ttoi(element.value().c_str());
			return nCmdID;
		}
	private:
		ISYNCML*				m_pISyncML;
		ISyncMLCallback*		m_pISyncMLCallback;
		std::vector<TSTRING>	m_Cmds;
		std::vector<TSTRING>	m_SyncCmds;
		TSTRING					m_strLocalURI;
		TSTRING					m_strTargetURI;	
		TSTRING					m_strSyncTarget;
		TSTRING					m_strSyncSource;
		int						m_nMaxMsgSize;
		int						m_nTargetMaxMsgSize;
		int						m_nTargetMaxObjSize;
		int						m_nCmdID;
		int						m_nMsgID;
		int						m_nSession;
		int						m_nMoreDataIndex;
		bool						m_bMoreData;
		//for <MoreData\> tag
		int						m_dwTestLen;
		TSTRING					m_strMoreData;
		TSTRING					m_strMoreDataUID;
		static int SessionID()
		{
			static int nSessionID = 0;
			return nSessionID++;
		}
	};

	class CSyncMLApplication : public CSyncMLSessionBase
	{
	public:
		CSyncMLApplication(const TSTRING& strType,int nSyncType,ISYNCML* pISyncML,ISyncMLCallback* pISyncMLCallback,const TSTRING& strLocalURI,int nLastAnchor,int nNextAnchor,int nMaxMsgSize = 10000)
			:CSyncMLSessionBase(pISyncML,pISyncMLCallback,strLocalURI,nMaxMsgSize)
		{
			if(strType == _T("vcard"))
				AddAlertCommand(nSyncType,_T("contacts"),_T("contacts"),_T("text/x-vcard"),nNextAnchor,nLastAnchor);
			else if(strType == _T("vcalendar"))
				AddAlertCommand(nSyncType,_T("events"),_T("events"),_T("text/x-vcalendar"),nNextAnchor,nLastAnchor);
			else assert(0);
		}
		virtual ~CSyncMLApplication(){}
	};
}
#endif //_SYNCML_SESSION_H_