#include "LogResult.h"
#include "wrapper_itemlist.h"

#define BZ_VERSION		"BTA.00.00.01"


class CHandlerBz  
{
public:
	CHandlerBz();
	virtual ~CHandlerBz();

		
	int GetPassFailStatus(void){return nPassFail;}
	int InitializeEquipments(void);
	int CloseEquipments(void);
	int InitializeItemList(void); 
	int LoadModelFile(char * pModelFilePath);
	int LoadDataBaseFile(char * pDataBasePath);
	int InitHandlerLogResult();
	int StartHandlerLogResult();
	int EvalHandlerLogResult(char * pszMeasCode, char * pszMeasDescription,double dTestResult, double dHighLimit,double dLowLimit, double dYHighLimit, double dYLowLimit, int nPassFail, char * pszUnits);
	int LogHandlerTestResult(char * pszTrackId, char * pszESN_IMEI, int nPassFail);
	int SetArgs(); 
	int InitItemListEquip();
	int LoadItemListFiles();
	int InitializeHandler(CString strEngMode);
	int InitComboModelName();
	int InitLogResultEquip(void);
	int StartHandlerTimeMeasBlock(char * pszBlockName);
	int FinishHandlerTimeMeasBlock(char * pszBlockName, char * pszMeasDesc);
	int CheckStatus();
	int PrintLabel();

	bool IsFactoryBZ();
	bool IsChecStatusEnable();
	bool IsLogData();
	bool IsProfileLoader();
	bool IsPrintLabel();	
	bool WriteLabel(CString str);

	
	static int CallBack_AllocateMemoryForChar(char * pSource, char ** pDestination, char * pszErrorMsg500);

	
	void SetPassFailStatus(int nFlag){nPassFail = nFlag;}
	void SetModelFileContents(char * pModelFileContents);
	void SetDatabaseContents(char * pContents);
	void SetMotorolaModel(CString strItem);
	void SetMotName(CString strName);
	void SetEngMode(CString strEngMode);
	void SetFactoryID(CString strFactoryId);
	void SetTrackId(CString strTrackId);
	void SetDataFeedLogPath(CString strDataFeedPath);

	void AddComboMotModel(CString strModels);
	void EnableMotModel(bool bFlag);
	void LoadBZConfig();
	void AddLogResultLog(CString strTestName);

	
	char * GetTokenValue(char * pszTokenName, char * pszTokenList,char * pszParmSeparator,char * pszTokenSeparator);
	char * GetDatabaseContents();
	char * GetTestSoftwareVersion(){return BZ_VERSION;}
	char * GetDataTempLogPath();
	char * GetDataFeedLogPath();
	char * GetModelFileContents();
	char * GetTestCompleteVersion();
	char * GetTestResultVersion();
	char * GetModelBoardKitVersion();
	char * GetModelBoardKitCode();
	char * GetProfileLoadVersion();

	CString GetStationGroup();
	CString GetFactoryID(); 
	CString GetMfgID();
	CString GetStationID();
	CString GetDataBaseName();
	CString GetOdmName(CString strMotName);
	CString GetMotModelfromModelFile(CString strMotModels);
	CString GetHdVerfromModelFile(CString strMotModels);
	CString GetSofVerfromModelFile(CString strMotModels);
	CString GetProfileId();
	CString GetProfileVer();
	CString GetMotorolaModel();
	CString GetMotName();
	CString GetScuType();
	CString GetMotNamesfromModelFile();
	CString GetTrackid();
	CString GetBTAddress();
	CString ConversionLabelData(CString str);

	CWnd* m_pMainCWnd;

	CComboBox* m_pMotModelCombo;
	CComboBox* m_pOdmModelCombo;

private:
	
	CString  m_strFactoryID;
	CString  m_strMfgID;
	CString  m_strStationID;
	CString  m_strI2cSide;
	CString  m_strSystem;
	CString  m_strDataSourceName;
	CString  m_strScuType;
	CString  m_strMotorolaModel;
	CString  m_strMotName;
	CString  m_strStationGroup;
	CString  m_strTrackId;
	CString  m_strDataFeedPath;
	CString  m_strBTAddress;
	CString  m_strComPort;
	char *	 m_pszModelFileContents;
	char *   m_pszDatabaseContents;
	char     m_psTrackId[12];
	char     m_pszEsnImei[20];
	int      nPassFail;


	bool m_bCheckstatus;
	bool m_bPrintLabel;
	bool m_bLogData;
	bool m_bProfileLoader;

};

