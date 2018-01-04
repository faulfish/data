#pragma once
#include "Interface_Include/ICommMobile.h"

class CEmptyIPIMCONFIG : public IPIMCONFIG
{
	
	virtual void	STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut){}
	virtual int		STDMETHODCALLTYPE ListPIMGroup(ePIMType eType, std::vector<CPIMGroupInfo> *vGroupList){return 0;}
	virtual int		STDMETHODCALLTYPE SetPIMGroup(ePIMType eType, int nGroupID, CPIMGroupInfo &setGroup){return false;}
  virtual int		STDMETHODCALLTYPE CreatePIMGroup(CPIMGroupInfo &newGroup, int &nNewID){return false;}
  virtual int		STDMETHODCALLTYPE DeletePIMGroup(int nDeleteID){return false;}
  virtual int		STDMETHODCALLTYPE GetProperty(const TSTRING& strGroup,const TSTRING& strName,TSTRING& strValue){return false;}
  virtual int		STDMETHODCALLTYPE GetIntegerProperty(const TSTRING& strGroup,const TSTRING& strName,int& nValue){return false;}
};

class CEmptyIPHONEBOOK : public IPHONEBOOK
{
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut){}
	virtual bool STDMETHODCALLTYPE IsPhonebookTypeSupport(ePhonebookType eType){return false;}
	virtual bool STDMETHODCALLTYPE GetPhonebookRange(ePhonebookType eType, UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount){return false;}
	virtual bool STDMETHODCALLTYPE ListPhonebookIndex(ePhonebookType eType, std::vector<UINT> *vIdxList){return false;}
	virtual int  STDMETHODCALLTYPE ReadVCardbyIndex(ePhonebookType eType, UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen){return eInternalError;}
	virtual int  STDMETHODCALLTYPE WriteVCardbyIndex(ePhonebookType eType, UINT& nIdx ,int& nRetError, char *cVCardbuf, int nBufLen){return eInternalError;}
	virtual int  STDMETHODCALLTYPE RemoveVCardbyIndex(ePhonebookType eType, UINT nIdx){return eInternalError;}
};

class CEmptyIEFS : public IEFS
{
	virtual BOOL STDMETHODCALLTYPE CanCancel(){return FALSE;}
	virtual BOOL STDMETHODCALLTYPE Cancel(){return FALSE;}
	virtual BOOL STDMETHODCALLTYPE QueryCapability(IEFSCapability eIEFSCapability);
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
	virtual int  STDMETHODCALLTYPE GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize );
	virtual int  STDMETHODCALLTYPE GetFileData(  int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData );   
	virtual int  STDMETHODCALLTYPE PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize );
	virtual int  STDMETHODCALLTYPE PutFileData(  int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData );
	virtual int  STDMETHODCALLTYPE DeleteFile( LPCTSTR strPath );
	virtual int  STDMETHODCALLTYPE GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList );
	virtual int  STDMETHODCALLTYPE CreateFolder( LPCTSTR strPath );
	virtual int  STDMETHODCALLTYPE RemoveFolder( LPCTSTR strPath );
	virtual BOOL STDMETHODCALLTYPE Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath );

	virtual int  STDMETHODCALLTYPE RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
	virtual int  STDMETHODCALLTYPE UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
};

class CEmptyISMS : public ISMS
{	
    // get service center address
	virtual HRESULT STDMETHODCALLTYPE GetServiceCenterAddress(CString* pcstrSCA){return S_FALSE;}
    // set PDU Mode
    virtual HRESULT STDMETHODCALLTYPE SetPDUMode(){return S_FALSE;}
    // set preferred storage (for standard)
    virtual HRESULT STDMETHODCALLTYPE SetMessageStorage(enumStorage eMsgStorage, MemUsed& used){return S_FALSE;}
    // query message space
    virtual HRESULT STDMETHODCALLTYPE QueryMessageSpace(enumStorage eMsgStorage, UINT* puiUsed, UINT* puiTotal){return S_FALSE;}
    // list message
    virtual HRESULT STDMETHODCALLTYPE ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>* pVecSM = 0, std::vector<CShortMessage>* pVecShortMessage = 0){return S_FALSE;}
    //virtual HRESULT STDMETHODCALLTYPE ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>& vecSM) = 0;
    // send messaage
    virtual HRESULT STDMETHODCALLTYPE SendMessage(BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID){return S_FALSE;}
    // send message in storage
    virtual HRESULT STDMETHODCALLTYPE SendMessageInStorage(UINT uiMsgID, CString cstrAddress){return S_FALSE;}
    // write message
    virtual HRESULT STDMETHODCALLTYPE WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID){return S_FALSE;}
    virtual HRESULT STDMETHODCALLTYPE WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID, char* szTime){return S_FALSE;}
    // read message
    virtual HRESULT STDMETHODCALLTYPE ReadMessage(UINT uiMsgID, BYTE* pMsgData, UINT* puiMsgDataSize){return S_FALSE;}
    virtual HRESULT STDMETHODCALLTYPE ReadMessage(UINT uiMsgID, CShortMessage& csm){return S_FALSE;}
    // delete message
    virtual HRESULT STDMETHODCALLTYPE DeleteMessage(UINT uiMsgID){return S_FALSE;}
    // get message sub folders by folder ID
    virtual HRESULT STDMETHODCALLTYPE GetFolderEntries(BOOL bGetRootFolder, UINT nParentFolderID, vector<CMessageFolderInfo> *vEntryList){return S_FALSE;}
    // create or rename message sub folders by folder ID
    virtual HRESULT STDMETHODCALLTYPE WriteFolderEntries(BOOL bCreate, BOOL bCreateRootFolder, UINT nParentFolderID, UINT nRenameFolderID, CString strDiaplayName, UINT &nNewID){return S_FALSE;}
    // delete sub folders by folder ID
    virtual HRESULT STDMETHODCALLTYPE DeleteFolderEntry( UINT nFolderID ){return S_FALSE;}
    // update message state
    virtual HRESULT STDMETHODCALLTYPE UpdateMessageState(UINT uiMsgID, UINT nState = 0){return S_FALSE;}
};

class CEmptyICALENDAR : public ICALENDAR
{	
 virtual bool STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut){return false;}
  virtual bool STDMETHODCALLTYPE GetRange(UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount){return false;}
  virtual bool STDMETHODCALLTYPE ListIndex(std::vector<UINT> *vIdxList){return false;}
  virtual bool STDMETHODCALLTYPE ReadByIndex(UINT nIdx, char *szbuf, int nBufMaxLen, int &nVcardLen){return false;}
  virtual bool STDMETHODCALLTYPE WriteByIndex(UINT& nIdx, char *szbuf, int nBufLen){return false;}
  virtual bool STDMETHODCALLTYPE RemoveByIndex(UINT nIdx){return false;}
};

class CEmptyDevice : public ICommDevice
{
public:
	CEmptyDevice(void);
public:
	virtual ~CEmptyDevice(void);

public:
	BOOL  AttachAllInterface ( LPCTSTR  strModel);
	virtual BOOL  DeviceGetInterface( CommInterface eInterface, LPVOID *ppInterface ); 
	virtual BOOL  DetachAllInterface ();
	virtual int   ConnectDevice( LPCTSTR strModel,  LPCTSTR port);
	virtual int   DisconnectDevice( LPCTSTR strModel,  LPCTSTR port);
	virtual BOOL  IsInterfaceAttached( CommInterface eInterface );
	virtual TSTRING	GetModelName() const {return m_strModel;}
	virtual TSTRING	GetIMEI() const{return m_strIMEI;}
	virtual TSTRING	GetSoftwareVer() const{ return m_strSoftwareVer;}
	virtual TSTRING GetPort() const {return m_strConnectPort;}
	virtual CommTransDevice GetTransDevice() const {return m_eDeviceType;}
	virtual BOOL GetFDNStatus(bool& bIsFDN){bIsFDN = false;return TRUE;}
    virtual BOOL GetConnectivity(bool& bIsOnline) {bIsOnline = false; return TRUE;};
private:
	TSTRING					m_strModel;
	TSTRING					m_strIMEI;
	TSTRING					m_strSoftwareVer;
	TSTRING					m_strConnectPort;
	CommTransDevice         m_eDeviceType;	
	CommConnectionState     m_eConnectState;	
	IEFS*							m_pIEFS;
	ISMS*							m_pISMS;
	IPHONEBOOK*			m_pIPhonebook;
	ICALENDAR*			m_pICalendar;
	IPIMCONFIG*			m_pIPIMConfigure;
};

