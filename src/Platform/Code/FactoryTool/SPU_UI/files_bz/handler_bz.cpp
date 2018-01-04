#include "stdafx.h"
#include "handler_bz.h"
#include "..\Resource.h"

//////////////////////////////////////////////////////////////////////
// CHandlerBz Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHandlerBz::CHandlerBz()
{

	// Calibration Cale Loss
	m_pszModelFileContents	= NULL;
	m_pszDatabaseContents	= NULL;
	m_pMotModelCombo		= NULL;
	m_pMainCWnd				= NULL;
	m_pOdmModelCombo		= NULL;

}

CHandlerBz::~CHandlerBz()
{
	if(m_pszModelFileContents)
	{
		delete[] m_pszModelFileContents;
		m_pszModelFileContents = NULL;
	}


	if(m_pszDatabaseContents)
	{
		delete[] m_pszDatabaseContents;
		m_pszDatabaseContents = NULL;
	}

}

CString CHandlerBz::GetFactoryID()
{
	return m_strFactoryID;
}

CString CHandlerBz::GetMfgID()
{
	return m_strMfgID;
}

CString CHandlerBz::GetStationID()
{
	return m_strStationID;
}

CString CHandlerBz::GetMotorolaModel()
{
	return m_strMotorolaModel;
}

CString CHandlerBz::GetProfileId()
{

	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	CString strProfileId		=_T("");
	CString strErrorMessage		=_T("");
	int nStatus					=-1;

	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"PROFILE_ID_Set","PROFILE_ID", "MODEL", (char*)(LPCTSTR)GetOdmName(this->GetMotName()), &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Profile Id not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	strProfileId.Format("%s",pszResult);

	return strProfileId;	

}

CString CHandlerBz::GetProfileVer()
{	
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");


	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"PROFILE_VERSION_Set","PROFILE_VERSION", "MODEL", (char*)(LPCTSTR)GetOdmName(this->GetMotName()), &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Profile Id not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}

char * CHandlerBz::GetDataTempLogPath()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");


	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"LOG_RESULT_Set","VALUE", "TOKEN", "DATATEMPPATH", &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","DataTempFolderToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}

char * CHandlerBz::GetDataFeedLogPath()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"LOG_RESULT_Set","VALUE", "TOKEN", "DATAFEEDPATH", &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","DataFeedFolderToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}

void CHandlerBz::SetDataFeedLogPath(CString strDataFeedPath)
{
	m_strDataFeedPath = strDataFeedPath;
}

CString CHandlerBz::GetStationGroup()
{
	return m_strStationGroup;
}

char * CHandlerBz::GetTestCompleteVersion()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");


	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"LOG_RESULT_Set","VALUE", "TOKEN", "TCTESTCOMPLETEVERSION", &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","CompleteTestVersionToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}

char * CHandlerBz::GetTestResultVersion()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");


	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"LOG_RESULT_Set","VALUE", "TOKEN", "TRTESTRESULTVERSION", &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","CompleteTestResultVersionToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}

char * CHandlerBz::GetProfileLoadVersion()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					=-1;
	CString strErrorMessage		=_T("");


	nStatus=ReturnItemListSetValues(this->GetDatabaseContents(),"LOG_RESULT_Set","VALUE", "TOKEN", "PLPROFILELOAVERSION", &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","ProfileLoadVersionToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
	
}

int CHandlerBz::InitHandlerLogResult()
{

	int nStatus					=-1;
	char szErrorMessage500[500] = {NULL};
	CString strErrorMessage		=_T("");


	CString strParameter=_T("");
	strParameter.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s","TcLineId=",this->GetMfgID(),
		"|TcStationId=",this->GetStationID(),"|TcTestSwVersion=",this->GetTestSoftwareVersion(),
		"|TcTestCompleteVersion=",this->GetTestCompleteVersion(),
		"|TrTestResultVersion=",this->GetTestResultVersion(),
		"|PlProfileLoadVersion=",this->GetProfileLoadVersion(),
		"|DatatempPath=",this->GetDataTempLogPath(),
		"|DatafeedPath=",this->GetDataFeedLogPath(),
		"|PlStationGroup=",this->GetStationGroup(),
		"|Pl11FamilyId=",this->GetMotName());

	nStatus = InitLogResult((char*)(LPCTSTR)strParameter, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Fail to initialize LoResult(wrong parameter)->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;
}

int CHandlerBz::StartHandlerLogResult()
{

	int nStatus						=-1;
	char szErrorMessage500[500]		= {NULL};
	CString strErrorMessage			=_T("");
	CString strParameter			=_T("");

	strParameter.Format("%s%s%s%s%s%s%s%s%s%s","TcPlTestProfileId=",this->GetProfileId(),
		"|TcPlTestProfileVersion=",this->GetProfileVer(),"|TcModelBoardKitNumber=",this->GetMotorolaModel(),
		"|TcModelBoardKitVersion=",this->GetModelBoardKitVersion(),
		"|Pl11ModelBoardKitVersionCode=",this->GetModelBoardKitCode());

	nStatus = StartRecipeTest((char*)(LPCTSTR)strParameter, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Fail to Start LoResult(wrong parameter)->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;
}

int CHandlerBz::EvalHandlerLogResult(char * pszMeasCode, char * pszMeasDescription,double dTestResult, double dHighLimit,double dLowLimit, double dYHighLimit, double dYLowLimit, int nPassFail, char * pszUnits)
{

	int nStatus						=-1;
	char szErrorMessage500[500]		= {NULL};
	CString strErrorMessage			=_T("");
	CString strPassFail				=_T("");
	CString strParameter			=_T("");

	if(0 == nPassFail)
		strPassFail = "P";
	else
		strPassFail = "F";

	strParameter.Format("%s%s%s%s%s%10.6f%s%5.2f%s%5.2f%s%5.2f%s%5.2f%s%s%s%s","TrPlMasterTestNumber=",pszMeasCode,
		"|PlMasterTestName=",pszMeasDescription,"|TrResultValue=",dTestResult,"|PlHiLimit=",dHighLimit,"|PlLowLimit=",dLowLimit,"|PlYHiLimit=",dYHighLimit,
		"|PlYLowLimit=",dYLowLimit,"|TrPassFailResult=",strPassFail,"|PlUnits=",pszUnits);


	nStatus = EvalTestResult((char*)(LPCTSTR)strParameter, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Fail to Eval LoResult(wrong parameter)->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;
}


int CHandlerBz::LogHandlerTestResult(char * pszTrackId, char * pszESN_IMEI, int nPassFail)
{

	int nStatus					=-1;
	char szErrorMessage500[500] = {NULL};
	CString strErrorMessage		=_T("");
	CString strPassFail			=_T("");
	CString strParameter		=_T("");
	int nFyleType				=1;

	if(0 == nPassFail)
		strPassFail = "P";
	else
		strPassFail = "F";

	strParameter.Format("%s%s%s%s%s%s","TcTrackId=",pszTrackId,
		"|TcEsnOrImei=",pszESN_IMEI,
		"|TcPassFailFinalResult=",strPassFail);

	nStatus = LogTestResult((char*)(LPCTSTR)strParameter, nFyleType, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Fail to Finish LoResult(wrong parameter)->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;
}


int CHandlerBz::InitLogResultEquip(void)
{
	int nStatus	= -1;

	nStatus = LoadLogResult();

	return nStatus;

}


int CHandlerBz::LoadItemListFiles()
{
	int nStatus					= -1;
	char * szModelPath			= NULL;
	char * szModelName			= NULL;
	char szErrorMessage500[500] = {NULL};
	CString strErrorMessage     = _T("");

	CString strName = ("c:\\prod\\config\\gsmmodelfile.txt");

	nStatus = this->LoadModelFile((char*)(LPCTSTR)strName);

	if (0==nStatus)
	{
		strName = ("C:\\prod\\database\\" + this->GetDataBaseName());
		nStatus = LoadDataBaseFile((char*)(LPCTSTR)strName);
	}

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Load Files Fail->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}


	return nStatus;
}




int CHandlerBz::InitItemListEquip(void)
{
	int nStatus	= -1;
	char szErrorMsg500[500] = {NULL};

	nStatus = InitItemList();

	if(0== nStatus)
		nStatus = SetCallBackFunctionPointer("CallBack_AllocateMemoryForChar",(FARPROC) &CallBack_AllocateMemoryForChar,szErrorMsg500);

	if (nStatus != 0)
		AfxMessageBox("Fail to initialize Item List");
		
	return nStatus;


}

int CHandlerBz::LoadModelFile(char * pModelFilePath)
{
	int nStatus					= -1;
	char * pszFileContentsAddr	= NULL;
	CString strErrorMessage     = _T("");
	char szErrorMessage500[500] = {NULL};

	nStatus = LoadFileAndConvertToItemList(pModelFilePath, &pszFileContentsAddr, szErrorMessage500 );
	
	if (0==nStatus && pszFileContentsAddr)
		SetModelFileContents(pszFileContentsAddr);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Load Model File Fail->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;

}



void CHandlerBz::SetModelFileContents(char * pContents)
{
	if(m_pszModelFileContents)
	{
		delete [] m_pszModelFileContents;
		m_pszModelFileContents = NULL;
	}
	m_pszModelFileContents = pContents;
}

char * CHandlerBz::GetModelFileContents()
{
	return m_pszModelFileContents;
}




CString CHandlerBz::GetOdmName(CString strMotName)
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus = ReturnItemListSetValues(this->GetModelFileContents(), "ModelInfo_Set", "ODM_NAME", "MOTOROLA_NAME", (char*)(LPCTSTR)strMotName, &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Odm Name not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}


	return pszResult;

}



CString CHandlerBz::GetMotModelfromModelFile(CString strMotModels)
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus=ReturnItemListSetValues(this->GetModelFileContents(), "ModelInfo_Set", "BOARD_KIT,KIT_NUM", "MOTOROLA_NAME", (char*)(LPCTSTR)strMotModels, &pszResult, TRUE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Mot Model not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;

}


CString CHandlerBz::GetHdVerfromModelFile(CString strMotModels)
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus = ReturnItemListSetValues(this->GetModelFileContents(), "ModelInfo_Set", "HW VERSION", "BOARD_KIT", (char*)(LPCTSTR)strMotModels, &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Hardware Version not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}


	return pszResult;

}

CString CHandlerBz::GetSofVerfromModelFile(CString strMotModels)
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus =ReturnItemListSetValues(this->GetModelFileContents(), "ModelInfo_Set", "SOFTWARE", "BOARD_KIT", (char*)(LPCTSTR)strMotModels, &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Software version not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;

}



char * CHandlerBz::GetTokenValue(char * pszTokenName, char * pszTokenList,char * pszParmSeparator,char * pszTokenSeparator)
{
	
	CString strToken		= _T("");
	char * pszTokenValue	= NULL;
	char * pszTokenListAux	= NULL;
	


	if (pszTokenName && strlen(pszTokenName) && pszTokenList && strlen(pszTokenList))
	{
		pszTokenListAux = 	_strdup(pszTokenList);
		char* pszToken = strtok(pszTokenListAux,pszParmSeparator);
		
		while(pszToken)
		{
			
			if (strstr(pszToken,pszTokenName))
			{
				strToken = pszToken;
				strToken = strToken.Right(strToken.GetLength() - strToken.Find(pszTokenSeparator) -1);
				break;
			}
			pszToken = strtok(NULL,pszParmSeparator);
			
		}
		
		
	}
	
	if (!strToken.IsEmpty())
		pszTokenValue = strdup (strToken.GetBuffer(strToken.GetLength()));

	if (pszTokenListAux)
		delete pszTokenListAux;
	
	return pszTokenValue;
}



char * CHandlerBz::GetModelBoardKitVersion()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					=-1;
	CString strErrorMessage		=_T("");


	nStatus=ReturnItemListSetValues(this->GetModelFileContents(),"ModelInfo_Set","MODELBOARDKITVERSION", "KIT_NUM", (char*)(LPCTSTR)this->GetMotorolaModel(), &pszResult, FALSE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","ModelKitVersionToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}


char * CHandlerBz::GetModelBoardKitCode()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus=ReturnItemListSetValues(this->GetModelFileContents(),"ModelInfo_Set","MODELBOARDKITCODE", "KIT_NUM", (char*)(LPCTSTR)this->GetMotorolaModel(), &pszResult, FALSE, szErrorMessage500);


	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","ModelKitCodeToken not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;
}



CString CHandlerBz::GetMotNamesfromModelFile()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");

	nStatus=ReturnItemListSetValues(this->GetModelFileContents(), "ModelInfo_Set", "MOTOROLA_NAME", "DIR", "QISDA", &pszResult, TRUE, szErrorMessage500);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Mot Model not found->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return pszResult;

}

int CHandlerBz::CloseEquipments(void)
{
	int nStatus					= -1;
	CString strErrorMessage		=_T("");
	char szErrorMessage500[500] = {NULL};

	nStatus =  CloseItemList();

	if (0 == nStatus)
		nStatus = CloseLogResult(szErrorMessage500);

	if (0 != nStatus)
	{
		strErrorMessage.Format("%s%s","Fail to Close Equipments",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;
}


void CHandlerBz::SetMotName(CString strName)
{
	m_strMotName = strName;
}

CString CHandlerBz::GetMotName()
{
	return m_strMotName;
}


int CHandlerBz::CallBack_AllocateMemoryForChar(char * pSource, char ** pDestination, char * pszErrorMsg500)
{
	int nStatus = -1;

	if(*pDestination)
	{
		delete [] *pDestination;
		*pDestination = NULL;
	}

	*pDestination = new char[strlen(pSource)+1];
	memset(*pDestination,NULL,strlen(pSource)+1);
	strcpy(*pDestination,pSource);
	
	if(!strcmp(pSource,*pDestination))
		nStatus = 0;

	return nStatus;
}

void CHandlerBz::AddComboMotModel(CString strModels)
{
	m_pMainCWnd = AfxGetMainWnd();
	m_pMotModelCombo = (CComboBox*) (m_pMainCWnd->GetDlgItem(IDC_COMBOMOTMODEL)); 
	m_pMotModelCombo->EnableWindow(TRUE);

	CString strModel=_T("");
	CString strSeparator = ";";
	do
	{
		if (strModels.Find(strSeparator) == -1)
		{
			m_pMotModelCombo->AddString (strModels);
			strModel = strModels;
			strModels = "";
		}
		else
		{
			m_pMotModelCombo->AddString (strModels.Left(strModels.Find(strSeparator)));
			strModels = strModels.Right(strModels.GetLength() - strModels.Find(strSeparator) -1);
		}
	}
	while(strModels != "");

}


void CHandlerBz::SetMotorolaModel(CString strItem)
{
	m_strMotorolaModel = strItem;
}

int CHandlerBz::SetArgs() 
{
	int nStatus = -1;
	CString strErrorMessage = "Argumentos Incorretos";

	if((__argc > 3) && (__argc <= 5))
	{
		m_strDataSourceName = GetTokenValue("dataSourceName",__targv[1],"/",":");
		m_strSystem = GetTokenValue("stationName",__targv[2],"/",":");
		m_strMfgID = GetTokenValue("stationFactory",__targv[4],"/",":");
		m_strStationID = GetTokenValue("stationNumber",__targv[3],"/",":");

		if (strstr(m_strSystem,"MA"))
			m_strScuType = "NONE";

		if (strstr(m_strSystem,"MA"))
			m_strStationGroup = "MA";

		if (strstr(m_strSystem,"BZ"))
			m_strFactoryID="BZ";
		else 
			m_strFactoryID="OTHER";

		if (strstr(m_strSystem,"LEFT"))
			m_strI2cSide = "LEFT";
		else if (strstr(m_strSystem,"RIGHT"))
			m_strI2cSide = "RIGHT";
		else if (strstr(m_strSystem,"SIMPLE"))
			m_strI2cSide = "SIMPLE";
	
		if (!m_strSystem.IsEmpty() && !m_strScuType.IsEmpty() && 
			!m_strDataSourceName.IsEmpty() && !m_strMfgID.IsEmpty() && 
			!m_strStationID.IsEmpty() && !m_strI2cSide.IsEmpty() && 
			!m_strStationGroup.IsEmpty() && !m_strFactoryID.IsEmpty())
				nStatus = 0;
		else
		{
			do
			{
				AfxMessageBox(strErrorMessage);
			}
			while(1);
		}
	}
	return 0;

}

void CHandlerBz::SetFactoryID(CString strFactoryId)
{
	m_strFactoryID = strFactoryId;
}

int CHandlerBz::InitializeEquipments(void)
{
	int nStatus =-1;

		nStatus=this->InitItemListEquip();

		if (0==nStatus)
			nStatus=this->LoadItemListFiles();

		if (0==nStatus)
			nStatus = this->InitLogResultEquip();

	return nStatus;
}



int CHandlerBz::InitComboModelName()
{
	char * pszResult			= NULL;
	char szErrorMessage500[500] = {NULL};
	int nStatus					= -1;
	CString strErrorMessage		=_T("");
	CString strModels			=_T("");
	CString strComboAdded		=_T("");
	CString strAuxModel			=_T("");
	CString strSeparator		= ";";

	m_pMainCWnd = AfxGetMainWnd();
	m_pOdmModelCombo = (CComboBox*) (m_pMainCWnd->GetDlgItem(IDC_MOTNAME));
	
	strModels=GetMotNamesfromModelFile();

	if (strModels.IsEmpty() != 0)
		return nStatus;
	else
		nStatus = 0;
	do
	{
		if (strModels.Find(strSeparator) == -1 && !strModels.IsEmpty())
		{
			if(!strstr(strAuxModel,strModels))
				m_pOdmModelCombo->AddString (strModels);

			strModels = "";	
		}
		else if (!strModels.IsEmpty())
		{
			if(!strstr(strAuxModel,strModels.Left(strModels.Find(strSeparator))))
			{
				strAuxModel += strModels.Left(strModels.Find(strSeparator));
				m_pOdmModelCombo->AddString (strModels.Left(strModels.Find(strSeparator)));
			}
			strModels = strModels.Right(strModels.GetLength() - strModels.Find(strSeparator) -1);
		}
	
	}
	while(!strModels.IsEmpty());

	return nStatus;
}



void CHandlerBz::SetDatabaseContents(char * pContents)
{
	if (m_pszDatabaseContents)
	{
		delete[] m_pszDatabaseContents;
		m_pszDatabaseContents = NULL;
	}
	m_pszDatabaseContents = pContents;
}

char * CHandlerBz::GetDatabaseContents()
{

	return m_pszDatabaseContents;
}

CString CHandlerBz::GetDataBaseName()
{
	return m_strDataSourceName;
}



int CHandlerBz::LoadDataBaseFile(char * pDataBasePath)
{
	int nStatus					= -1;
	char * pszFileContentsAddr	= NULL;
	CString strErrorMessage     = _T("");
	char szErrorMessage500[500] = {NULL};

	nStatus =  LoadAndReturnItemListFile(pDataBasePath, &pszFileContentsAddr,  szErrorMessage500 );

	if (0==nStatus && pszFileContentsAddr)
		SetDatabaseContents(pszFileContentsAddr);

	if (0!=nStatus)
	{   
		strErrorMessage.Format("%s%s","Load Database Fail->",szErrorMessage500);
		AfxMessageBox(strErrorMessage);
	}

	return nStatus;

}


void CHandlerBz::SetTrackId(CString strTrackid)
{
	m_strTrackId = strTrackid;
}

CString CHandlerBz::GetTrackid()
{
	return m_strTrackId;
}

bool CHandlerBz::IsFactoryBZ()
{
	bool bFactory = false;

	if (strstr(GetFactoryID(),"BZ"))
		bFactory =true;

	return bFactory;
}

void CHandlerBz::LoadBZConfig()
{
//	CString str;
//	CString strConfigFilePath = ".\\config.ini";
//	CString strStationFilePath = ".\\station.ini";
//	char strData[256];
//
//	//Check if file exist ( config.ini )
//	::GetPrivateProfileString("SETTINGS", "ISFILE", "NONE", strData, 20, strConfigFilePath);
//	str.Format("%s", (LPCSTR)strData);
//	str.TrimLeft(); str.TrimRight();
//
//	if ( "NONE" == str)
//	{
//		::WritePrivateProfileString("SETTINGS", "ISFILE", "TRUE", strConfigFilePath);		
//		::WritePrivateProfileString("SETTINGS", "CHECK_STATUS", "ENABLE", strConfigFilePath);
//		::WritePrivateProfileString("SETTINGS", "PRINT_LABEL", "ENABLE", strConfigFilePath);
//		::WritePrivateProfileString("SETTINGS", "LOG_DATA", "ENABLE", strConfigFilePath);
//		::WritePrivateProfileString("SETTINGS", "PROFILE_LOADER", "ENABLE", strConfigFilePath);
//	}
//	else
//	{
//		::GetPrivateProfileString("SETTINGS", "CHECK_STATUS", "ENABLE", strData, 20, strConfigFilePath);
//		str.Format("%s", (LPCSTR)strData);
//		str.TrimLeft(); str.TrimRight();
//		if (str == "DISABLE")
//			m_bCheckstatus = false;
//		else
//			m_bCheckstatus = true;
//
//		::GetPrivateProfileString("SETTINGS", "PRINT_LABEL", "ENABLE", strData, 20, strConfigFilePath);
//		str.Format("%s", (LPCSTR)strData);
//		str.TrimLeft(); str.TrimRight();
//		if (str == "DISABLE")
//			m_bPrintLabel = false;
//		else
//			m_bPrintLabel = true;
//		
//		::GetPrivateProfileString("SETTINGS", "LOG_DATA", "ENABLE", strData, 20, strConfigFilePath);
//		str.Format("%s", (LPCSTR)strData);
//		str.TrimLeft(); str.TrimRight();
//		if (str == "DISABLE")
//			m_bLogData = false;
//		else
//			m_bLogData = true;
//
//		// Profile Loader
//		::GetPrivateProfileString("SETTINGS", "PROFILE_LOADER", "ENABLE", strData, 20, strConfigFilePath);
//		str.Format("%s", (LPCSTR)strData);
//		str.TrimLeft(); str.TrimRight();
//		if (str == "DISABLE")
//			m_bProfileLoader = false;
//		else
//			m_bProfileLoader = true;
//
//	}
//	//Check if file exist ( station.ini )
///*	::GetPrivateProfileString("SETTINGS", "ISFILE", "NONE", strData, 20, strStationFilePath);
//	str.Format("%s", (LPCSTR)strData);
//	str.TrimLeft(); str.TrimRight();
//
//	if ("NONE"==str)
//	{
//		::WritePrivateProfileString("SETTINGS", "ISFILE", "TRUE", strStationFilePath);		
//		::WritePrivateProfileString("SETTINGS", "BTADDRESS", "000000000000", strStationFilePath);		
//		::WritePrivateProfileString("SETTINGS", "COMPORT", "5", strStationFilePath);	
//	}
//	else
//	{
//		::GetPrivateProfileString("SETTINGS", "BTADDRESS", "XXXXXXXXXXXX", strData, 20, strStationFilePath);
//		str.Format("%s", (LPCSTR)strData);
//		str.TrimLeft(); str.TrimRight();
//		m_strBTAddress = str;
//
//		::GetPrivateProfileString("SETTINGS", "BTADDRESS", "XXXXXXXXXXXX", strData, 20, strStationFilePath);
//		str.Format("%s", (LPCSTR)strData);
//		str.TrimLeft(); str.TrimRight();
//		m_strComPort = str;
//	}*/

	CString str;
	CString strConfigFilePath = ".\\config.ini";
	CString strStationFilePath = ".\\station.ini";
	char strData[256];

	m_strScuType = "NONE";
	m_strDataSourceName = "database.txt";
	m_strStationGroup = "MA";
	m_strI2cSide = "SIMPLE";

	::GetPrivateProfileString("SETTINGS", "CHECK_STATUS", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_bCheckstatus = false;
	else
		m_bCheckstatus = true;

	::GetPrivateProfileString("SETTINGS", "PRINT_LABEL", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_bPrintLabel = false;
	else
		m_bPrintLabel = true;
	
	::GetPrivateProfileString("SETTINGS", "LOG_DATA", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_bLogData = false;
	else
		m_bLogData = true;

	// Profile Loader
	::GetPrivateProfileString("SETTINGS", "PROFILE_LOADER", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_bProfileLoader = false;
	else
		m_bProfileLoader = true;

	// BenchId
	::GetPrivateProfileString("SETTINGS", "BENCHID", "XXXXX", strData, 20, strStationFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	m_strStationID=str;

	// Factory Id
	::GetPrivateProfileString("SETTINGS", "FACTORYID", "MFG01", strData, 20, strStationFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	m_strMfgID=str;

}

bool CHandlerBz::IsChecStatusEnable()
{
	return m_bCheckstatus;
}
bool CHandlerBz::IsLogData()
{
	return m_bLogData;
}
bool CHandlerBz::IsProfileLoader()
{
	return m_bProfileLoader;
}
bool CHandlerBz::IsPrintLabel()
{
	return m_bPrintLabel;
}
CString CHandlerBz::GetBTAddress()
{
	return m_strBTAddress;
}

int CHandlerBz::CheckStatus()
{						
	
	BOOL	bOpen				= FALSE;
	CFile	Sfile;
	BYTE	buffer[1000]		= { "OK" };
	DWORD	dwRead				= 0;
	clock_t start,finish;
	double  duration				= 0;
	CString str;
	CString strCIMMessage		= "CheckStatusError";
	CString strDateTime			=_T(""); 
	CString strArgument 		=_T("");
	CString strCheckStatusPort	= "1500";
	CString strServerName		= "PSCAP003";
	CString strSocketPath		= "C:\\PROD\\BIN\\SOCKET2";
	CString strSocketFile		= "C:\\socket.txt";
	CString strSocketFileTemp	= strSocketFile.Left(9);
	int		nStatus				= -1;
	CTime	SocketTime;
	
	DeleteFile(strSocketFile);

	SocketTime = CTime::GetCurrentTime();

	strDateTime.Format("%04d-%02d-%02d,%02d:%02d:%02d",
		SocketTime.GetYear(), 
		SocketTime.GetMonth(), 
		SocketTime.GetDay(), 
		SocketTime.GetHour(), 
		SocketTime.GetMinute(), 
		SocketTime.GetSecond());


	strArgument.Format("%s %s %s 0 CHECK_STATUS///%s///%s///%s///%s///%s %s", 
				strSocketPath,
				strCheckStatusPort,
				strServerName,
				this->GetTrackid(), 
				"00000000",		
				this->GetMotorolaModel(),
				this->GetStationID(),
				strDateTime,
				strSocketFileTemp);

	if (WinExec((LPCSTR)strArgument, SW_SHOWNORMAL)) 
		nStatus = 0;
	
	if (nStatus ==0)
	{
		memset(buffer, 0 , sizeof(buffer));
		start = clock();		
		do											
		{
			bOpen=Sfile.Open(strSocketFile, CFile::modeRead | CFile::shareDenyWrite);
			finish = clock();										
			duration = (double)(finish - start) / CLOCKS_PER_SEC;	
		}
		while ((bOpen==FALSE) && (duration < 10));  // Duration in seconds 

		if(bOpen==FALSE)
		{
			strCIMMessage = "FALHA PARA ABRIR O ARQUIVO DO CHECK_STATUS";
			nStatus = -1;
		
		}
		if (nStatus == 0)
		{
			dwRead = Sfile.Read(buffer, 1000);
			Sfile.Close();	

			if(dwRead<=0)
			{
				strCIMMessage= "FALHA NO ARQUIVO DE RESPOSTA DO CHECK_STATUS";
				nStatus = -1;
			}
		}
		if (nStatus == 0)
		{
			str.Format("%s", &buffer[0]);
			str=str.Right(str.GetLength()-str.Find("|||")-3);
			str=str.Right(str.GetLength()-str.Find("|||")-3);
			strCIMMessage=str;
			strCIMMessage=strCIMMessage.Right(str.GetLength()-str.Find("|||")-3);
			strCIMMessage=strCIMMessage.Left(strCIMMessage.Find("|||"));
			str=str.Left(str.Find("|||"));

			if(strcmp(str, "NOK") == 0)
				nStatus = -1;
			else if(strcmp(str, "OK") == 0)
				nStatus = 0;
			else
			{
				strCIMMessage = "Check Status Error";
				nStatus = -1;
			}
		}
		
	}
	
	DeleteFile(strSocketFile);
	if (nStatus != 0)
		AfxMessageBox(strCIMMessage);

	return nStatus;

}
void CHandlerBz::AddLogResultLog(CString strTestName)
{
	int nStatus = -1;
	CString strMeasCode = _T("");
	CString strMeasDesc = _T("");
	CString strUnit		= "N/A";
	long lHigSpec		= 0;
	long lLoSpec		= 0;
	int nTestStatus		= 1;
	long lTestvalue		= 0;
	CString strErrorMessage = _T("");

	strErrorMessage.Format("%s%s",strTestName,"Falha ao gerar linha de log");

	if (strstr(strTestName," success"))
		nTestStatus = 0;
	else if(strstr(strTestName,"fail"))
	{
		nTestStatus = 1;
		lTestvalue = 1;
	}

	if (strstr(strTestName,"AudioLoopbackTest"))
	{
		strMeasCode = "AUDIO_TS";
		strMeasDesc = "AUDIO_LOOPBACK_TEST";
	}

	if (strstr(strTestName,"KeypadPreeTest"))
	{
		strMeasCode = "KEYP_TST";
		strMeasDesc = "KEYPADTEST";
	}
	
	if (strstr(strTestName,"Display Red"))
	{
		strMeasCode = "DISP_RED";
		strMeasDesc = "DISPLAY_RED";
	}
	if (strstr(strTestName,"Display Black"))
	{
		strMeasCode = "DISP_BLK";
		strMeasDesc = "DISPLAY_BLACK";
	}
	if (strstr(strTestName,"Display Green"))
	{
		strMeasCode = "DISP_GRE";
		strMeasDesc = "DISPLAY_GREEN";
	}
	if (strstr(strTestName,"Display Blue"))
	{
		strMeasCode = "DISP_BLU";
		strMeasDesc = "DISPLAY_BLUE";
	}
	if (strstr(strTestName,"DSC Test"))
	{
		if (strstr(strTestName,"SubDSC Test"))
		{
			strMeasCode = "SCAM_TST";
			strMeasDesc = "SUB_CAMERA_TEST";
		}
		else
		{
			strMeasCode = "MCAM_TST";
			strMeasDesc = "MAIN_CAMERA_TEST";
		}
	}
	if (strstr(strTestName,"BluetoothTest"))
	{
		strMeasCode = "BT__TEST";
		strMeasDesc = "BLUETOOTH TEST";
	}
	if (strstr(strTestName,"MP3Test"))
	{
		strMeasCode = "MP3_TEST";
		strMeasDesc = "MP3_TEST";
	}
	if (strstr(strTestName,"VibratorTest"))
	{
		strMeasCode = "VIB_TEST";
		strMeasDesc = "VIBRATOR TEST";
	}
	if (strstr(strTestName,"CoincellTest"))
	{
		strMeasCode = "COINCELL";
		strMeasDesc = "COINCELL_TEST";
	}
	if (strstr(strTestName,"SIMCardTest"))
	{
		strMeasCode = "SIM_CARD";
		strMeasDesc = "SIM_CARD_TEST";
	}
	if (strstr(strTestName,"SliderTest"))
	{
		strMeasCode = "SLIDERTS";
		strMeasDesc = "SLIDER_TEST";
	}

	if (strstr(strTestName,"GSensorTest"))
	{
		strMeasCode = "GSNSRCAL";
		strMeasDesc = "GSENSOR_CALIBRATION";
	}

	if (strstr(strTestName,"MicroSDCardTest"))
	{
		strMeasCode = "MMC_TEST";
		strMeasDesc = "MEMORY_CARD_TEST";
	}

	if (strstr(strTestName,"Test"))
	{
		nStatus = EvalHandlerLogResult((char*)(LPCTSTR)strMeasCode,(char*)(LPCTSTR)strMeasDesc,lTestvalue,lHigSpec,lLoSpec,lHigSpec,lLoSpec,nTestStatus,(char*)(LPCTSTR)strUnit);

		if (0!=nStatus)
		{
			do
			{
				AfxMessageBox(strErrorMessage);
			}
			while(1);
		}
	}
}

int CHandlerBz::PrintLabel()
{
	BOOL bOpen =FALSE;
	CString strLabelData =_T(""); 
	CString SocketCommand =_T("");
	CFile Sfile;
	clock_t start,finish;
	double duration =0.0;
	UINT dwRead;
	char *temp;
	CString labelBuffer =_T("");
	CString strPath,labFile= _T("");
	CString message,status;

	strPath.Format("%s","C:\\");
	CTime t =CTime::GetCurrentTime();
	CString strTimes =t.Format("%Y%m%d,%H:%M:%S");
							
	SocketCommand.Format("%s\\PROD\\BIN\\SOCKET2 %s %s 0 LABELGET|||%s|||ELTRONZ %sLABELPRT"
	,strPath
	,"1220"  
	,"PSCAP003"
	,this->GetTrackid()
	,strPath);

	WinExec(SocketCommand, SW_SHOWNORMAL);
	Sleep(1000);
	start = clock();
	labFile.Format("%s\\LABELPRT.txt",strPath);
	do
	{
		bOpen = Sfile.Open(labFile,CFile::modeRead);
		finish = clock();
		duration = (double)(finish - start)/CLOCKS_PER_SEC;
	}
	while((bOpen == FALSE) && (duration < 20));

	if(bOpen == FALSE)
	{
	   AfxMessageBox("Hint: FALHA PARA ABRIR O ARQUIVO DE SOCKET !");
	   return 1;
	}

	temp = new char[1024];
	memset (temp, 0x00, 1024);
	dwRead = Sfile.Read(temp, 1024);
	Sfile.Close();
	if(dwRead <= 0)
	{
	   AfxMessageBox("FALHA ARQUIVO DE SOCKET INCORRETO !");
	   delete [] temp;
	   temp =NULL;
	   return 1;
	}

	strtok(temp,";");  //garbage data clear..
	labelBuffer = temp;
	delete [] temp; temp =NULL;
	labelBuffer = labelBuffer + ";";    
	strtok((LPTSTR)(LPCTSTR)labelBuffer, "|||");
	status = strtok(NULL,"|||");
	message = strtok(NULL,"|||");

	if(status == "NOK")
	{
	   AfxMessageBox(message);
	   DeleteFile(labFile);
	   Sleep(500); 
	   return 1;
	}

	// change character from "@" to "\r\n";
	strLabelData = ConversionLabelData(message); 
	DeleteFile(labFile);   
	Sleep(500);

	if(!WriteLabel(strLabelData))
	{
	  return 1;
	} 
	return 0;
}
 
bool CHandlerBz::WriteLabel(CString str)
{
	 CString m_strPrinter_COM = "LPT1";
	 HANDLE hComm;
	 if ("" == m_strPrinter_COM) 
	 {
		AfxMessageBox("Error: Printer com port set error!");
		return  FALSE;
	 } 
	hComm  = CreateFile((LPCSTR)m_strPrinter_COM,
	GENERIC_READ|GENERIC_WRITE,
	0,
	NULL,
	OPEN_EXISTING, 
	FILE_ATTRIBUTE_NORMAL,
	NULL);
 
	if (hComm == (HANDLE) -1) 
    {
        AfxMessageBox("Failed to Create Print Port Handle!");
        return  FALSE;
	}
	Sleep(500); 
 
	DWORD dwWritten;
	if(!WriteFile(hComm,str, str.GetLength(), &dwWritten, NULL))
	{
	  AfxMessageBox("Failed to Write Label!");
	  CloseHandle(hComm);
	  return FALSE;
	}
	else
	 {
		  CloseHandle(hComm);
		  return TRUE; 
	 } 
	return TRUE;
}
 
CString CHandlerBz::ConversionLabelData(CString str)
{
	CString strResult=_T("");
 
	int strlength = str.GetLength();
 
	for(int i = 0; i<strlength-1; i++)
	{
	if(str[i] == '@')
	{
		strResult = strResult + "\r\n";     
	}
	else
		strResult = strResult + str[i];
	}
	return strResult;
}
