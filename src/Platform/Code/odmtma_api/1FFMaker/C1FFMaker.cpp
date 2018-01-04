#include "StdAfx.h"
#include "C1FFMaker.h"

C1FFNameMaker::C1FFNameMaker(void) : 
	m_strMarketName(_T("WellFleet")),
	m_nActiveCount(0),
	m_bSetMotName(false)
{
	TCHAR szPath[1024] = {0};
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS|CSIDL_FLAG_CREATE, NULL, 0, szPath)))
	{
		m_strDefaultFolder = szPath;
		m_strDefaultFolder += _T("\\Temp\\odmtemp\\");
	}	
}

C1FFNameMaker::~C1FFNameMaker(void)
{
}

bool C1FFNameMaker::GetMotName(TSTRING& strMotName)
{
	bool bRes = true;
	strMotName = m_strMarketName;
	bool bGetSW = GetSWVersion();
	bool bGetLang = GetLang(); 

	if(GetSIMLock() && !m_strSIMFile.empty())
		strMotName += _T("_[")+m_strSIMFile+_T("]");
	else 
		strMotName += _T("_")+m_strSIMFile;

	if(bGetSW && !m_strMainLoad.empty())
		strMotName += _T("_[")+m_strMainLoad+_T("]");
	else
		strMotName += _T("_") + m_strMainLoad;

	if(bGetLang && !m_strLang.empty())
		strMotName += _T("_[")+m_strLang+_T("]");

	strMotName += _T(".mot");
	return bRes;
}

bool C1FFNameMaker::GetSWVersion() //SoftwareVer(7)_SecurityVer(5)
{
	bool bRes = false;
	for(vector<TSTRING>::iterator iter=m_vstrFiles.begin(); iter!=m_vstrFiles.end(); iter++)
	{
		CString cstrTemp = (*iter).c_str();
		cstrTemp.MakeLower();
		TSTRING strTemp = cstrTemp;
		if(TSTRING::npos != strTemp.find(_T(".mbn")) )
		{
			size_t nCount = (*iter).size();
			TSTRING strMainLoad = (*iter).substr(0, (nCount-4));
			m_strMainLoad = strMainLoad.substr(strMainLoad.size()-13, 13);
			bRes = true;
			break;
		}
	}
	return bRes;
}

bool C1FFNameMaker::GetLang()  //LangID(6)_LangVer(10)_IniVer(5)_ResVer(5)
{
	bool bRes = false;
	for(vector<TSTRING>::iterator iter=m_vstrFiles.begin(); iter!=m_vstrFiles.end(); iter++)
	{
		CString cstrTemp = (*iter).c_str();
		cstrTemp.MakeLower();
		TSTRING strTemp = cstrTemp;
		if(TSTRING::npos != strTemp.find(_T(".qvcefs")))
		{
			size_t nCount = (*iter).size();
			TSTRING strLang = (*iter).substr(0, (nCount-7));
			m_strLang = strLang.substr(strLang.size()-29, 29);

			vector<TSTRING> vstrBlcok;
			QSYNC_LIB::Tokenize<TSTRING>(_T("_"), (*iter), vstrBlcok, false);
			m_strSIMFile = vstrBlcok[1] + _T("_") + vstrBlcok[2]; 

			if(m_strMainLoad.empty())
			{
				m_strMainLoad = vstrBlcok[3] + _T("_SEV00");
			}

			bRes = true;
			break;
		}
		else if(TSTRING::npos != strTemp.find(_T(".du")))
		{
			size_t nCount = (*iter).size();
			TSTRING strLang = (*iter).substr(0, (nCount-3));
			m_strLang = strLang.substr(strLang.size()-29, 29);

			vector<TSTRING> vstrBlcok;
			QSYNC_LIB::Tokenize<TSTRING>(_T("_"), (*iter), vstrBlcok, false);
			m_strSIMFile = vstrBlcok[1] + _T("_") + vstrBlcok[2]; 

			if(m_strMainLoad.empty())
			{
				m_strMainLoad = vstrBlcok[3] + _T("_SEV00");
			}
			bRes = true;
			break;
		}
	}
	return bRes;
}

bool C1FFNameMaker::GetSIMLock()
{
	bool bRes = false;
	for(vector<TSTRING>::iterator iter=m_vstrFiles.begin(); iter!=m_vstrFiles.end(); iter++)
	{
		CString cstrTemp = (*iter).c_str();
		cstrTemp.MakeLower();
		TSTRING strTemp = cstrTemp;
		if(TSTRING::npos != strTemp.find(_T(".loc")))
		{
			size_t nCount = (*iter).size();
			size_t nIndex = (*iter).find_last_of(_T("_"));
			if(nIndex != TSTRING::npos)
			{
				TSTRING strSIM = (*iter).substr(nIndex+1, 1);
				m_strSIMFile = m_strSIMFile + _T("_") + strSIM; 
				bRes = true;
				break;
			}
		}	
	}
	if(bRes == false)
		m_strSIMFile = m_strSIMFile + _T("_0"); 
	return bRes;
}


bool C1FFNameMaker::SetMotName(const TSTRING& strMotName) // trim .mot and keep file name
{
	bool bRes = false;
	if(strMotName.size() != 0)
	{
		//trim .mot
		m_strMotName = strMotName.substr(0, strMotName.size()-4);
		
		//trim path
		TSTRING::size_type nSlash = m_strMotName.find_last_of(_T("\\"));
		m_strMotName = strMotName.substr(nSlash+1, m_strMotName.size()-nSlash-1);

		TSTRING strTmp = strMotName;
		m_nActiveCount = QSYNC_LIB::GetRepeatCount<TSTRING>(_T("["), strTmp);
		//trim []
		TSTRING strTrimedMotName = m_strMotName;
		QSYNC_LIB::Replace(_T("["), _T(""), strTrimedMotName, strTrimedMotName);
		QSYNC_LIB::Replace(_T("]"), _T(""), strTrimedMotName, strTrimedMotName);
		
		if(QSYNC_LIB::Tokenize<TSTRING>(_T("_"), strTrimedMotName, m_vstrNaming, false))
			bRes = true;

	}
	return bRes;
}

bool C1FFNameMaker::CompareSWV(TSTRING strSWV)
{
	bool bRes = false;
	TSTRING strSWVNum;
	std::vector<TSTRING> vstrToken;
	QSYNC_LIB::Tokenize<TSTRING>(_T("_"), strSWV, vstrToken, false);
	if(vstrToken.size() > 1)
		strSWVNum = vstrToken[1];
	else if(vstrToken.size() == 1)
		strSWVNum = vstrToken[0];
	
	double dSWVNum = _tstof(strSWVNum.c_str());
	TCHAR szSWVNum[10] = {0};
	_stprintf(szSWVNum ,_T("%4.2f"), dSWVNum);

	strSWVNum = szSWVNum;
	QSYNC_LIB::Replace(_T("."), _T(""), strSWVNum, strSWVNum);
	if(strSWVNum.size() == 3)
		strSWVNum = _T("SWV0") + strSWVNum;
	else
		strSWVNum = _T("SWV") + strSWVNum;
	if(strSWVNum == m_vstrNaming[5])
		bRes = true;
	return bRes;
}
bool C1FFNameMaker::CompareFlexVersion(TSTRING strFlexVersion)
{
	bool bRes = false;
	vector<TSTRING> vstrFlex;
	QSYNC_LIB::Tokenize<TSTRING>(_T("_"), strFlexVersion, vstrFlex, false);
	if(vstrFlex.size() > 5 && m_vstrNaming.size() > 10
		&& vstrFlex[4] == m_vstrNaming[7] && vstrFlex[5] == m_vstrNaming[10])
		bRes = true;
	return bRes;
}

TSTRING C1FFNameMaker::GetMbn()
{
	TSTRING strTmp;
	if(m_vstrNaming.size() > 10)
	{
		strTmp = m_vstrNaming[1] + _T("_") + m_vstrNaming[5] + _T("_") + m_vstrNaming[6] + _T("_") + m_vstrNaming[7];// + _T(".mbn");
	}
	return strTmp;
}

TSTRING C1FFNameMaker::GetLP_Res(const TSTRING& strSubName)
{
	TSTRING strTmp;
	if(m_vstrNaming.size() > 11)
	{
		strTmp = m_vstrNaming[1] + _T("_") + m_vstrNaming[2] 
		+ _T("_") + m_vstrNaming[3] + _T("_") + m_vstrNaming[5] + _T("_") + m_vstrNaming[6] 
		+ _T("_") + m_vstrNaming[8] + _T("_") + m_vstrNaming[9] + _T("_") + m_vstrNaming[10] 
		+ _T("_") + m_vstrNaming[11] + _T("_") + m_vstrNaming[12] + _T(".") + strSubName;
	}
	return strTmp;
}

bool C1FFNameMaker::GetLoc(TSTRING& strLock)
{
	bool bRes = false;
	if(m_vstrNaming.size() > 11)
	{
		TSTRING strLockFile = m_vstrNaming[1] + _T("_") + m_vstrNaming[2] 
		+ _T("_") + m_vstrNaming[3] + _T("_") + m_vstrNaming[4] + _T(".loc");

		strLock = m_strDefaultFolder + strLockFile;
		if(::_taccess_s(strLock.c_str(),0) == 0)
			bRes = true;
	}
	return bRes;
}

bool C1FFNameMaker::GetAMSS(TSTRING& strAMSSHD, TSTRING& strAMSS)
{
	bool bRes = false;
	
	TSTRING strMbn = GetMbn();
	strAMSSHD = m_strDefaultFolder + _T("amsshd_") + strMbn + _T(".mbn");
	strAMSS = m_strDefaultFolder + _T("amss_") + strMbn + _T(".mbn");
	
	if(::_taccess_s(strAMSSHD.c_str(),0) == 0 && ::_taccess_s(strAMSS.c_str(),0) == 0 )
		bRes = true;
	
	return bRes;	
}
bool C1FFNameMaker::GetPartition(TSTRING& strPartition)
{
	bool bRes = false;
	TSTRING strMbn = GetMbn();
	strPartition = m_strDefaultFolder + _T("partition_") + strMbn + _T(".mbn");

	if(::_taccess_s(strPartition.c_str(),0) == 0)
		bRes = true;

	return bRes;
}
bool C1FFNameMaker::GetBootLoader(TSTRING& strQCSBLHD, TSTRING& strQCSBL, TSTRING& strOEMHD, TSTRING& strOEM)
{
	bool bRes = false;

	TSTRING strMbn = GetMbn();
	strQCSBLHD = m_strDefaultFolder + _T("qcsblhd_cfgdata_") + strMbn + _T(".mbn");
	strQCSBL = m_strDefaultFolder + _T("qcsbl_") + strMbn + _T(".mbn");
	strOEMHD = m_strDefaultFolder + _T("oemsblhd_") + strMbn + _T(".mbn");
	strOEM = m_strDefaultFolder + _T("oemsbl_") + strMbn + _T(".mbn");

    if(::_taccess_s(strQCSBLHD.c_str(),0) == 0 && ::_taccess_s(strQCSBL.c_str(),0) == 0 &&
        ::_taccess_s(strOEMHD.c_str(),0) == 0 && ::_taccess_s(strOEM.c_str(),0) == 0
        ) bRes = true;

	return bRes;
}
bool C1FFNameMaker::GetCEFS(TSTRING& strCEFS)
{
	bool bRes = false;
	TSTRING strLP_Res = GetLP_Res(_T("qvcefs"));
	strCEFS = m_strDefaultFolder + strLP_Res;
	if(::_taccess_s(strCEFS.c_str(),0) == 0)
		bRes = true;
	return bRes;
}
bool C1FFNameMaker::GetDU(TSTRING& strDu)
{
	bool bRes = false;
	strDu = GetLP_Res(_T("du"));
	strDu = m_strDefaultFolder + strDu;
	if(::_taccess_s(strDu.c_str(),0) == 0)
		bRes = true;
	return bRes;
}

bool C1FFNameMaker::GetCsv(TSTRING& strCsv)
{
	bool bRes = false;
	strCsv = m_strDefaultFolder + _T("BackupNV-Items.csv");
	if(::_taccess_s(strCsv.c_str(),0) == 0)
		bRes = true;
	return bRes;
}

bool C1FFNameMaker::GetHex(TSTRING& strHex)
{
	bool bRes = false;
	TSTRING strMbn = GetMbn();
	strHex = m_strDefaultFolder + _T("NPRG6260_") + strMbn +  _T(".hex");
	if(::_taccess_s(strHex.c_str(),0) == 0)
		bRes = true;
	return bRes;
}

CFileGetter::CFileGetter() {}
CFileGetter::~CFileGetter() {}
bool CFileGetter::SetMot(const TSTRING& strMot) {
    if(::_taccess(strMot.c_str(), 0) != 0) return false;
    m_strMot = strMot;
    TSTRING::size_type nDot = strMot.find_last_of(_T("."));
    m_strFolderPath = strMot.substr(0, nDot);
    return true;
}

void CFileGetter::SetMotExtractFolder(const TSTRING& strFolderPath) {
    m_strFolderPath = strFolderPath;
}
bool CFileGetter::GetFolderPath(TSTRING& strFolderPath) {
    bool bRes = false;
    if(m_strFolderPath.empty()) return false;
    strFolderPath = m_strFolderPath+ _T("\\");
    return true;
}
bool CFileGetter::GetImageFilePath(TSTRING& strOutFileName, TSTRING& strImageFileName) {
    if(strImageFileName.empty()) return false;
    strOutFileName = m_strFolderPath+ _T("\\") +strImageFileName;
    return true;
}
bool CFileGetter::FindFile(const TSTRING& strSearchFile, TSTRING& strFileName) {
    bool bRes = false;
    int nFileCount = 0;
    CFileFind finder;
    TSTRING strFilePath = m_strFolderPath;
    strFilePath += _T("\\");
    strFilePath += strSearchFile;
    BOOL bWorking = finder.FindFile(strFilePath.c_str());
    while (bWorking) {
        nFileCount++;
        bWorking = finder.FindNextFile();
        strFileName = finder.GetFileName();
    }
    if(nFileCount == 1)
        bRes = true;
    else
        strFileName = _T("");
    return bRes;
}

CFileGetterFor6245::CFileGetterFor6245(){}
CFileGetterFor6245::~CFileGetterFor6245(){}

bool CFileGetterFor6245::SetFileName(bool bFindAll)
{
	bool bRes = false;
	
	bool bSetHex =       FindFile(_T("*.hex"),			 		m_strHex);
	bool bSetPartition = FindFile(_T("Partition_*.mbn"), 		m_strPartition);
	bool bSetQcsbl =	 FindFile(_T("Qcsbl_*.mbn"),	 		m_strQcsbl);
	bool bSetQcsblhd =   FindFile(_T("qcsblhd_cfgdata_*.mbn"),  m_strQcsblhd);
	bool bSetOemsbl =    FindFile(_T("oemsbl_*.mbn"),			m_strOemsbl);
	bool bSetOemsblhd =  FindFile(_T("oemsblhd_*.mbn"),			m_strOemsblhd);
	bool bSetAmss =		 FindFile(_T("amss_*.mbn"),				m_strAmss);
	bool bSetAmsshd =	 FindFile(_T("amsshd_*.mbn"),			m_strAmsshd);
	bool bSetLoc =		 FindFile(_T("*.loc"),					m_strLoc);
	bool bSetCefs =      FindFile(_T("*.qvcefs"),				m_strCEFS);
	bool bSetDu =        FindFile(_T("*.du"),					m_strDU);
 
	if(bFindAll) {
		bRes = bSetHex && bSetPartition && bSetOemsbl && bSetOemsblhd && 
			bSetAmss && bSetAmsshd && bSetLoc && bSetCefs;// && bSetDu;
	} else {
		bRes = bSetHex && bSetPartition && bSetLoc && bSetCefs;// && bSetDu;
	}
	return bRes;
}

bool CFileGetterFor6245::GetAMSS(TSTRING& strAMSS)
{
    return GetImageFilePath(strAMSS, m_strAmss);
}
bool CFileGetterFor6245::GetAMSSHD(TSTRING& strAMSSHD)
{
    return GetImageFilePath(strAMSSHD, m_strAmsshd);
}
bool CFileGetterFor6245::GetQCSBLHD(TSTRING& strQCSBLHD)
{
    return GetImageFilePath(strQCSBLHD, m_strQcsblhd);
}
bool CFileGetterFor6245::GetQCSBL(TSTRING& strQCSBL)
{
    return GetImageFilePath(strQCSBL, m_strQcsbl);
}
bool CFileGetterFor6245::GetOEMSBL(TSTRING& strOemsbl)
{
    return GetImageFilePath(strOemsbl, m_strOemsbl);
}
bool CFileGetterFor6245::GetOEMSBLHD(TSTRING& strOemsblhd)
{
    return GetImageFilePath(strOemsblhd, m_strOemsblhd);
}
bool CFileGetterFor6245::GetPartition(TSTRING& strPartition)
{
    return GetImageFilePath(strPartition, m_strPartition);
}
bool CFileGetterFor6245::GetCEFS(TSTRING& strCEFS)
{
    return GetImageFilePath(strCEFS, m_strCEFS);
}
bool CFileGetterFor6245::GetHex(TSTRING& strHex)
{
    return GetImageFilePath(strHex, m_strHex);
}
bool CFileGetterFor6245::GetDU(TSTRING& strDu)
{
    return GetImageFilePath(strDu, m_strDU);
}
bool CFileGetterFor6245::GetLoc(TSTRING& strLoc)
{
    return GetImageFilePath(strLoc, m_strLoc);
}
bool CFileGetterFor6245::GetOSBL(TSTRING& strOSBL)
{
    return GetImageFilePath(strOSBL, m_strOSBL);
}
bool CFileGetterFor6245::GetDBL(TSTRING& strDBL)
{
    return GetImageFilePath(strDBL, m_strDBL);
}
bool CFileGetterFor6245::GetFSBL(TSTRING& strFSBL)
{
    return GetImageFilePath(strFSBL, m_strFSBL);
}

CFileGetterFor6240::CFileGetterFor6240(){}
CFileGetterFor6240::~CFileGetterFor6240(){}

bool CFileGetterFor6240::SetFileName(bool bFindAll)
{
    bool bRes = false;

    bool bSetHex =       FindFile(_T("*.hex"), m_strHEX);
    bool bSetPartition = FindFile(_T("Partition_*.mbn"), m_strPartition);
    bool bSetAmss =		 FindFile(_T("amss*.mbn"), m_strAMSS);
    bool bSetDBL  =      FindFile(_T("dbl*.mbn"), m_strDBL);
    bool bSetFSBL =      FindFile(_T("fsbl*.mbn"), m_strFSBL);
    bool bSetOSBL =      FindFile(_T("osbl*.mbn"), m_strOSBL);
    bool bSetCefs =      FindFile(_T("*.qvcefs"), m_strCEFS);
    //bool bSetPBL  =      FindFile(_T("pbl*.mbn"), m_strPBL);

    if(bFindAll)
        bRes = bSetHex && bSetPartition && bSetAmss && bSetDBL && bSetFSBL && bSetOSBL && bSetCefs;// && bSetPBL;
    else
        bRes = bSetHex && bSetPartition && bSetAmss && bSetCefs;
    return bRes;
}

bool CFileGetterFor6240::GetHex(TSTRING& strHex) {
    return GetImageFilePath(strHex, m_strHEX);
}
bool CFileGetterFor6240::GetPBL(TSTRING& strPBL) {
    return GetImageFilePath(strPBL, m_strPBL);
}
bool CFileGetterFor6240::GetPartition(TSTRING& strPartition) {
    return GetImageFilePath(strPartition, m_strPartition);
}
bool CFileGetterFor6240::GetDBL(TSTRING& strDBL) {
    return GetImageFilePath(strDBL, m_strDBL);
}
bool CFileGetterFor6240::GetFSBL(TSTRING& strFSBL) {
    return GetImageFilePath(strFSBL, m_strFSBL);
}
bool CFileGetterFor6240::GetOSBL(TSTRING& strOSBL) {
    return GetImageFilePath(strOSBL, m_strOSBL);
}
bool CFileGetterFor6240::GetAMSS(TSTRING& strAMSS) {
    return GetImageFilePath(strAMSS, m_strAMSS);
}
bool CFileGetterFor6240::GetCEFS(TSTRING& strCEFS) {
    return GetImageFilePath(strCEFS, m_strCEFS);
}