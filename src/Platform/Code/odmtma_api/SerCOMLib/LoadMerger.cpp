// LoadMerger.cpp: implementation of the CLoadMerger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadMerger.h"
#include "BlowFish.h"
#include "Logger.h"
#include "DLConstantDef.h"
#include "DLParameter.h"
#include "CRCMachine.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DL_ENCRYPT_KEY_LENGTH		8	//Download encrption key length.
#define DL_ENCRYPT_LOAD_BLOCK_SIZE	4096
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadMerger::CLoadMerger()
{
	m_strLoadFileName = _T("");
	m_strOutputFileName = _T("");

}

CLoadMerger::~CLoadMerger()
{
	m_strLoadFileName = _T("");
	m_strOutputFileName = _T("");
}

int CLoadMerger::SetDLParameter(){
	
	// Set download parameter.
	m_objDLParam.SetLoadFileName(m_strLoadFileName);
	CString strVersion = m_strLoadFileName;
	strVersion = strVersion.Left(strVersion.Find('.') - 1 );
	m_objDLParam.SetVersion(strVersion);
	m_objDLParam.SetModelID(1);
	CFile fLoad;
	fLoad.Open(m_strLoadFileName, CFile::modeRead);
	DWORD dwLoadSize = fLoad.GetLength();
	m_objDLParam.SetLoadSize(dwLoadSize);
	m_objDLParam.SetFlashSize(0x2000000);
	m_objDLParam.SetNVOffset(0x1000000);
	m_objDLParam.SetNVImgSize(0x10000);
	
	return RUN_SUCCESSFULLY;

}

// Change for CM build process
int CLoadMerger::Merge(){
	LPCTSTR szFunc = _T("CLoadMerger::Merge");
	_ASSERTE(m_strLoadFileName.GetLength() > 0);
	_ASSERTE(m_strOutputFileName.GetLength() > 0);
	CFile fLoadFile;
	CFile fOutputFile;
	CFileException fError;
	int nRet = 0;

	if ( fLoadFile.Open(m_strLoadFileName, CFile::modeRead, &fError) == 0 ){
		// open file failure.
		LogSystemError(fError.m_cause);
		return CLOADMERGER_FILE_ERROR;
	}
	
	if ( fOutputFile.Open(m_strOutputFileName, CFile::modeCreate | CFile::modeWrite , &fError ) == 0){
		LogSystemError(fError.m_cause);
		return CLOADMERGER_FILE_ERROR;
	}
	
/*
	nRet = m_objDLParam.WriteToFile(fOutputFile);
	if ( nRet < 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("Write parameter block failure."));
		return nRet;
	}

	nRet = EncryptAndSaveLoad(fLoadFile, fOutputFile);
	if ( nRet < 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("Write Load block failure."));
		return nRet;
	}
*/
	//Change the sequence for CM auto-build process.
	nRet = EncryptAndSaveLoad(fLoadFile, fOutputFile);
	if ( nRet < 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("Write Load block failure."));
		return nRet;
	}

	nRet = m_objDLParam.WriteToFile(fOutputFile);
	if ( nRet < 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("Write parameter block failure."));
		return nRet;
	}

	fLoadFile.Close();
	fOutputFile.Close();

	return RUN_SUCCESSFULLY;

}

int CLoadMerger::EncryptAndSaveLoad(CFile &fLoadFile, CFile& fOutputFile){

	LPCTSTR szFunc = _T("CLoadMerger::EncryptAndSaveLoad");
	BYTE pbKey[8] = {0x62, 0x65, 0x6E, 0x71,0x62, 0x65, 0x6E, 0x71};	// BlowFish encryption and decryption key "benqbenq". 
	BYTE pbEnBuf[4096];	// buffer which is encrypted.
	BYTE *pbLoad = NULL;
	int i = 0;
	CCRCMachine	crcmachine;
	DWORD dwLoadSize = 0;
	DWORD dwEncryptSize = 0;
	DWORD dwCRC = 0;

	_ASSERTE(fLoadFile.m_hFile != CFile::hFileNull);
	_ASSERTE(fOutputFile.m_hFile != CFile::hFileNull);
//	ZeroMemory(pbKey, 8);
	ZeroMemory(pbEnBuf, 4096);

	dwLoadSize = fLoadFile.GetLength();
	dwEncryptSize = dwLoadSize + sizeof(dwCRC);
	if ( ( dwEncryptSize % DL_ENCRYPT_LOAD_BLOCK_SIZE) != 0 ){
		dwEncryptSize = (dwEncryptSize / DL_ENCRYPT_LOAD_BLOCK_SIZE ) * DL_ENCRYPT_LOAD_BLOCK_SIZE + DL_ENCRYPT_LOAD_BLOCK_SIZE;
	}
	
	pbLoad = new BYTE[dwEncryptSize];
	ZeroMemory(pbLoad, dwEncryptSize);
	fLoadFile.Read(pbLoad, dwEncryptSize);

	dwCRC = crcmachine.CRC30Calculation((LPTSTR) pbLoad, dwLoadSize * 8);
	CopyMemory(pbLoad+dwLoadSize, &dwCRC, sizeof(dwCRC));

	DWORD dwCurPos = 0;

	try{

		CBlowFish oBlowFish((unsigned char*)pbKey, 8);
		while ( dwCurPos < dwEncryptSize){
			oBlowFish.Encrypt(pbLoad+dwCurPos, pbEnBuf, 4096, CBlowFish::CBC);		
			dwCurPos += 4096;
			fOutputFile.Write(pbEnBuf, 4096);

		}
	}catch (CFileException *f){

		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc , _T("Write data to file failed."));
		f->ReportError();
		f->Delete();
		return CLOADMERGER_FILE_ERROR;

	}

	delete[] pbLoad;
	pbLoad = NULL;
	return RUN_SUCCESSFULLY;

}

// 20050804 Embert Tsai.
// Add progress support.
int CLoadMerger::CheckLoadCRC(BYTE *pbBuf, DWORD dwLoadSize, HWND hDisplay){
	LPCTSTR szFunc = _T("CLoadMerger::CheckLoadCRC");
	BYTE pbKey[8];	// BlowFish encryption and decryption key "benqbenq". 
	BYTE pbDeBuf[DL_ENCRYPT_LOAD_BLOCK_SIZE];
	CCRCMachine	crcmachine;
	DWORD dwEncryptSize = 0;
	DWORD dwCRC = 0;
	DWORD dwCurPos = 0;
	DWORD dwOldPercentage = 0;
	DWORD dwCurPercentage = 0;

	SetEncryptKeyword(pbKey);
	dwEncryptSize = dwLoadSize + sizeof(dwCRC);
	if ( ( dwEncryptSize % DL_ENCRYPT_LOAD_BLOCK_SIZE) != 0 ){
		dwEncryptSize = (dwEncryptSize / DL_ENCRYPT_LOAD_BLOCK_SIZE ) * DL_ENCRYPT_LOAD_BLOCK_SIZE + DL_ENCRYPT_LOAD_BLOCK_SIZE;
	}
	LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("The encrypted size is %X"), dwEncryptSize);
	if (hDisplay != NULL){
		::PostMessage(hDisplay, WM_DLOAD_PROGRESS, 0 , 0 );
	}
	
	CBlowFish oBlowFish((unsigned char*)pbKey, 8);
	while ( dwCurPos < dwEncryptSize ){
		LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("The current pos is %X"), dwCurPos);
		oBlowFish.Decrypt(pbBuf+dwCurPos, pbDeBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE, CBlowFish::CBC);
		dwCurPos += DL_ENCRYPT_LOAD_BLOCK_SIZE;
		if ( dwCurPos == DL_ENCRYPT_LOAD_BLOCK_SIZE ){
			crcmachine.CRC30BlockCal((LPTSTR)pbDeBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE * 8, 1);
		}else if (dwCurPos == dwEncryptSize){
			dwCRC = crcmachine.CRC30BlockCal((LPTSTR)pbDeBuf, (dwLoadSize - (dwCurPos-DL_ENCRYPT_LOAD_BLOCK_SIZE)) * 8, 3);
		}else {
			crcmachine.CRC30BlockCal((LPTSTR)pbDeBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE * 8, 2);
		}
		if ( hDisplay != NULL){
			dwCurPercentage = ( dwCurPos * 100 ) / dwLoadSize;
			if ( dwCurPercentage != dwOldPercentage && dwCurPercentage != 100){
				dwOldPercentage = dwCurPercentage;
				::PostMessage(hDisplay, WM_DLOAD_PROGRESS, dwCurPercentage , 0 );
			}
		}

	}

	if ( memcmp(&dwCRC, pbDeBuf + (dwLoadSize % DL_ENCRYPT_LOAD_BLOCK_SIZE ) , sizeof(dwCRC)) != 0 ){
		return CLOADMERGER_CRC_FAILURE;
	}

	if (hDisplay != NULL){
		::PostMessage(hDisplay, WM_DLOAD_PROGRESS, 100 , 0 );
	}
		
	return RUN_SUCCESSFULLY;
}

int CLoadMerger::DecryptLoad(BYTE *pbBuf, BYTE *pbOut){
	
	LPCTSTR szFunc = _T("DecryptLoad");
	BYTE pbKey[8];	// BlowFish encryption and decryption key "benqbenq". 
	_ASSERTE(pbBuf != NULL);
	_ASSERTE(pbOut != NULL);
	SetEncryptKeyword(pbKey);	

	CBlowFish oBlowFish((unsigned char*)pbKey, 8);
	oBlowFish.Decrypt(pbBuf, pbOut, DL_ENCRYPT_LOAD_BLOCK_SIZE, CBlowFish::CBC);

	return RUN_SUCCESSFULLY;

}

int CLoadMerger::DecryptLoad(CFile &fExe, BYTE *pbOut){
	
	LPCTSTR szFunc = _T("CLoadMerger::DecryptLoad");
	BYTE pbKey[8];	// BlowFish encryption and decryption key "benqbenq". 
	_ASSERTE(pbOut != NULL);
	BYTE pbEnBuf[DL_ENCRYPT_LOAD_BLOCK_SIZE]={0};
	SetEncryptKeyword(pbKey);	

	fExe.Read(pbEnBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE);
	CBlowFish oBlowFish((unsigned char*)pbKey, 8);
	oBlowFish.Decrypt(pbEnBuf, pbOut, DL_ENCRYPT_LOAD_BLOCK_SIZE, CBlowFish::CBC);

	return RUN_SUCCESSFULLY;

}

void CLoadMerger::SetEncryptKeyword(BYTE *pbBuf){

	DWORD dwOffset = 0x50;
	pbBuf[0] = 0x12;
	pbBuf[1] = 0x15;
	pbBuf[2] = 0x1E;
	pbBuf[3] = 0x21;
	pbBuf[4] = 0x12;
	pbBuf[5] = 0x15;
	pbBuf[6] = 0x1E;
	pbBuf[7] = 0x21;

	for ( int i = 0 ; i < 8 ; i ++ ){
		pbBuf[i] += (BYTE)dwOffset;	
	}
}
int CLoadMerger::CheckLoadCRC(CFile &fExe, DWORD dwLoadSize, HWND hDisplay){
	LPCTSTR szFunc = _T("CLoadMerger::CheckLoadFileCRC");
	BYTE pbKey[8];	// BlowFish encryption and decryption key "benqbenq". 
	BYTE pbDeBuf[DL_ENCRYPT_LOAD_BLOCK_SIZE];
	BYTE pbEnBuf[DL_ENCRYPT_LOAD_BLOCK_SIZE];
	CCRCMachine	crcmachine;
	DWORD dwEncryptSize = 0;
	DWORD dwCRC = 0;
	DWORD dwCurPos = 0;
	DWORD dwOldPercentage = 0;
	DWORD dwCurPercentage = 0;

	SetEncryptKeyword(pbKey);
	dwEncryptSize = dwLoadSize + sizeof(dwCRC);
	if ( ( dwEncryptSize % DL_ENCRYPT_LOAD_BLOCK_SIZE) != 0 ){
		dwEncryptSize = (dwEncryptSize / DL_ENCRYPT_LOAD_BLOCK_SIZE ) * DL_ENCRYPT_LOAD_BLOCK_SIZE + DL_ENCRYPT_LOAD_BLOCK_SIZE;
	}
	LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("The encrypted size is %X"), dwEncryptSize);
	if (hDisplay != NULL){
		::PostMessage(hDisplay, WM_DLOAD_PROGRESS, 0 , 0 );
	}
	fExe.Seek((dwEncryptSize+DL_PARAMETER_BLOCK_SIZE) * -1 , CFile::end);
	
	CBlowFish oBlowFish((unsigned char*)pbKey, 8);
	while ( dwCurPos < dwEncryptSize ){
		fExe.Read(pbEnBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE);
		LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("The current pos is %X"), dwCurPos);
		oBlowFish.Decrypt(pbEnBuf, pbDeBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE, CBlowFish::CBC);
		dwCurPos += DL_ENCRYPT_LOAD_BLOCK_SIZE;
		if ( dwCurPos == DL_ENCRYPT_LOAD_BLOCK_SIZE ){
			crcmachine.CRC30BlockCal((LPTSTR)pbDeBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE * 8, 1);
		}else if (dwCurPos == dwEncryptSize){
			dwCRC = crcmachine.CRC30BlockCal((LPTSTR)pbDeBuf, (dwLoadSize - (dwCurPos-DL_ENCRYPT_LOAD_BLOCK_SIZE)) * 8, 3);
		}else {
			crcmachine.CRC30BlockCal((LPTSTR)pbDeBuf, DL_ENCRYPT_LOAD_BLOCK_SIZE * 8, 2);
		}
		if ( hDisplay != NULL){
			dwCurPercentage = ( dwCurPos * 100 ) / dwLoadSize;
			if ( dwCurPercentage != dwOldPercentage && dwCurPercentage != 100){
				dwOldPercentage = dwCurPercentage;
				::PostMessage(hDisplay, WM_DLOAD_PROGRESS, dwCurPercentage , 0 );
			}
		}

	}

	if ( memcmp(&dwCRC, pbDeBuf + (dwLoadSize % DL_ENCRYPT_LOAD_BLOCK_SIZE ) , sizeof(dwCRC)) != 0 ){
		return CLOADMERGER_CRC_FAILURE;
	}

	if (hDisplay != NULL){
		::PostMessage(hDisplay, WM_DLOAD_PROGRESS, 100 , 0 );
	}
		
	return RUN_SUCCESSFULLY;
}

void CLoadMerger::SeekToTargetCode(CFile& fExe, DWORD dwLoadSize){
	LPCTSTR szFunc = _T("CLoadMerger::SeekToTargetCode");
	DWORD dwEncryptSize = 0;
	
	dwEncryptSize = dwLoadSize + sizeof(DWORD);
	if ( ( dwEncryptSize % DL_ENCRYPT_LOAD_BLOCK_SIZE) != 0 ){
		dwEncryptSize = (dwEncryptSize / DL_ENCRYPT_LOAD_BLOCK_SIZE ) * DL_ENCRYPT_LOAD_BLOCK_SIZE + DL_ENCRYPT_LOAD_BLOCK_SIZE;
	}
	fExe.Seek((dwEncryptSize+DL_PARAMETER_BLOCK_SIZE) * -1 , CFile::end);

}

// calculate FILE CRC
void CLoadMerger::CalculateFileCRC(CFile &fSrc, DWORD& dwCRC){
	
	CCRCMachine	crcmachine;
	DWORD dwLoadSize = 0;
	DWORD dwEncryptSize = 0;
	dwCRC = 0;
	
	// checking file object
	_ASSERTE(fSrc.m_hFile != CFile::hFileNull);

	dwLoadSize = fSrc.GetLength();
	
	// Read all file content into buffer and calculate CRC
	BYTE *pbBuf = NULL;
	pbBuf = new BYTE[dwLoadSize];
	ZeroMemory(pbBuf, dwLoadSize);
	fSrc.Read(pbBuf, dwLoadSize);
	dwCRC = crcmachine.CRC30Calculation((LPTSTR) pbBuf, dwLoadSize * 8);
	delete[] pbBuf;
	pbBuf = NULL;

}

// Encrpt buffer using blow fish algorithm with pbkey
// dwLen should be nultiply of 8
// BlowBuf means "blow the buffer", that is using blow fish algorithm to encrypt and decrypt buffer.
void CLoadMerger::BlowBuf(BYTE *pbSrc, BYTE *pbDes, DWORD dwLen, BYTE* pbKey , BOOL bIsEncryptAction){
	
	_ASSERTE(pbSrc != NULL);
	_ASSERTE(pbDes != NULL);
	_ASSERTE(pbKey != NULL);
	_ASSERTE( ((dwLen % 8) == 0) && (dwLen > 0)  );
	
	ZeroMemory(pbDes, dwLen);
	CBlowFish oBlowFish((unsigned char*)pbKey, 8);
	if ( bIsEncryptAction ){
		oBlowFish.Encrypt(pbSrc, pbDes, 8, CBlowFish::CBC);
	}else{
		oBlowFish.Decrypt(pbSrc, pbDes, 8, CBlowFish::CBC);
	}
}

// Checking exe CRC
int CLoadMerger::CheckExeCRC(CFile &fExe, DWORD dwLoadSize){
	
	LPCTSTR szFunc = _T("CLoadMerger::CheckExeCRC");
	BYTE pbKey[8] = {0};	// BlowFish encryption and decryption key "benqbenq". 
	BYTE pbDeBuf[8] = {0};
	BYTE pbEnBuf[8] = {0};
	CCRCMachine	crcmachine;
	DWORD dwEncryptSize = 0;
	DWORD dwCRC = 0;
	DWORD dwOldPercentage = 0;
	DWORD dwCurPercentage = 0;

	dwEncryptSize = dwLoadSize + sizeof(dwCRC);
	if ( ( dwEncryptSize % DL_ENCRYPT_LOAD_BLOCK_SIZE) != 0 ){
		dwEncryptSize = (dwEncryptSize / DL_ENCRYPT_LOAD_BLOCK_SIZE ) * DL_ENCRYPT_LOAD_BLOCK_SIZE + DL_ENCRYPT_LOAD_BLOCK_SIZE;
	}
	LogFunctionInformation(LOG_LEVEL_DEBUG, szFunc, _T("The encrypted size is %X"), dwEncryptSize);
	
	// seek to begin of exe CRC
	fExe.Seek((dwEncryptSize+DL_PARAMETER_BLOCK_SIZE + 8 ) * -1 , CFile::end);
	SetEncryptKeyword(pbKey);	
	fExe.Read(pbEnBuf, 8);
	BlowBuf(pbEnBuf, pbDeBuf, 8, pbKey, FALSE);
	
	// Read execution file content
	BYTE *pbExeContent = NULL;
	DWORD dwOriginalExeSize = 0;
	dwOriginalExeSize = fExe.GetLength() - (dwEncryptSize+DL_PARAMETER_BLOCK_SIZE + 8 );
	pbExeContent = new BYTE[dwOriginalExeSize];
	ZeroMemory(pbExeContent , dwOriginalExeSize);
	fExe.SeekToBegin();
	fExe.Read(pbExeContent, dwOriginalExeSize);
	dwCRC = crcmachine.CRC30Calculation((LPTSTR) pbExeContent, dwOriginalExeSize * 8);
	delete[] pbExeContent;
	pbExeContent = NULL;
	BYTE pbChkBuf[8] = {0};
	CopyMemory(pbChkBuf, &dwCRC, sizeof(dwCRC));
	CopyMemory(pbChkBuf+sizeof(dwCRC), &dwCRC, sizeof(dwCRC));
	
	if ( memcmp(pbChkBuf, pbDeBuf, 8) != 0 ){
		return CLOADMERGER_CRC_FAILURE;
	}

	return RUN_SUCCESSFULLY;
	
}

