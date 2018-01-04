// DLParameter.cpp: implementation of the CDLParameter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DLParameter.h"
#include "Blowfish.h"
#include "Logger.h"
#include "CRCMachine.h"
#include "DLConstantDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DL_ENCRPT_KEY_LENGTH	8	//Download encrption key length.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDLParameter::CDLParameter()
{
	m_strLoadFileName = _T("");
	m_strVersion = _T("");
	m_dwModelID = 0;
	m_dwLoadSize = 0;
	m_dwFlashSize = 0;
	m_dwNVOffset = 0;
	m_dwNVImgSize = 0;
}

CDLParameter::~CDLParameter()
{
	m_strLoadFileName = _T("");
	m_strVersion = _T("");
	m_dwModelID = 0;
	m_dwLoadSize = 0;
	m_dwFlashSize = 0;
	m_dwNVOffset = 0;
	m_dwNVImgSize = 0;
}

// Reading download parameter from file.
// Read the first 512 byte.
// Encryption and decryption key are the same: "benqbenq".
// The first 4 byte is "BENQ".
// The last two byte is CRC.
// Total available space for information storing will be 512 - 4(BENQ keyword) - 4(CRC) = 506. 
int CDLParameter::ReadParameterFromBuf(LPVOID lpBuf){
	
	LPCTSTR szFunc = _T("CDLParameter::ReadFromBuf");
	_ASSERTE(lpBuf != NULL);
	BYTE pbKey[DL_ENCRPT_KEY_LENGTH];		// BlowFish encryption and decryption key.
	BYTE pbEnBuf[DL_PARAMETER_BLOCK_SIZE];	// buffer which is encrypted.
	BYTE pbDeBuf[DL_PARAMETER_BLOCK_SIZE];	// Buffer which is decrypted.			
	SDLParameter sParam;
	int i = 0;
	CCRCMachine	crcmachine;
	
	ZeroMemory(pbKey, DL_ENCRPT_KEY_LENGTH);
	ZeroMemory(pbEnBuf, DL_PARAMETER_BLOCK_SIZE);
	ZeroMemory(pbDeBuf, DL_PARAMETER_BLOCK_SIZE);
	ZeroMemory(&sParam, sizeof(sParam));

	// Get encryption keyword.
	// It is "benqbenq".
	SetEncryptKeyword(pbKey);

	// Copy the encrypted 512 byte buffer.
	CopyMemory(pbEnBuf, lpBuf, DL_PARAMETER_BLOCK_SIZE);
	
	// Decrypt the buffer using blowfish algorithm.
	CBlowFish oBlowFish((unsigned char*)pbKey, DL_ENCRPT_KEY_LENGTH);
	oBlowFish.Decrypt(pbEnBuf, pbDeBuf, DL_PARAMETER_BLOCK_SIZE, CBlowFish::CBC);
	
	// Checking bloak information CRC.
	DWORD dwCRC;
	dwCRC = crcmachine.CRC30Calculation((LPTSTR) pbDeBuf,(DL_PARAMETER_BLOCK_SIZE - sizeof(dwCRC))*8);
	if ( memcmp(pbDeBuf+DL_PARAMETER_BLOCK_SIZE-sizeof(dwCRC), & dwCRC , sizeof(dwCRC)) != 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("CRC failure."));
		return DLPARAMETER_CRC_FAILURE;
	}
	
	// Check the first 4 byte.
	// The first 4 byte is "BENQ"
	for ( i = 0 ; i < 4 ; i ++){
		pbKey[i] -= 0x20;	//Change "benq" to BENQ.
	}
	if ( memcmp(pbDeBuf, pbKey, 4) != 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("First four byte mismatch."));
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("The four byte in buffer is."));
		LogBuffer(LOG_LEVEL_ERROR, pbDeBuf, 4);
		return DLPARAMETER_INVALID_HEADER;
	}
	
	CopyMemory(&sParam,pbDeBuf+4 , sizeof(sParam));
	
	SetLoadFileName(sParam.szLoadFileName);
	SetVersion(sParam.szLoadVersion);
	SetModelID(sParam.wModelID);
	SetLoadSize(sParam.dwLoadSize);
	SetFlashSize(sParam.dwFlashSize);
	SetNVOffset(sParam.dwNVOffset);
	SetNVImgSize(sParam.dwNVImgSize);
	
	return RUN_SUCCESSFULLY;
}

// Encrypt the block information.
// This function will be called by outside utility.
int CDLParameter::WriteToFile(CFile &f){

	LPCTSTR szFunc = _T("CDLParameter::WriteToFile");
	BYTE pbKey[DL_ENCRPT_KEY_LENGTH];		// BlowFish encryption and decryption key.
	BYTE pbEnBuf[DL_PARAMETER_BLOCK_SIZE];	// buffer which is encrypted.
	BYTE pbDeBuf[DL_PARAMETER_BLOCK_SIZE];	// Buffer which is decrypted.			
	SDLParameter sParam;
	int i = 0;
	CCRCMachine	crcmachine;
	
	_ASSERTE(f.m_hFile != CFile::hFileNull);
	ZeroMemory(pbKey, DL_ENCRPT_KEY_LENGTH);
	ZeroMemory(pbEnBuf, DL_PARAMETER_BLOCK_SIZE);
	ZeroMemory(pbDeBuf, DL_PARAMETER_BLOCK_SIZE);
	ZeroMemory(&sParam, sizeof(sParam));

	// Get encryption keyword.
	// It is "benqbenq".
	SetEncryptKeyword(pbKey);

	// The the first 4 byte keyword "BENQ"
	for ( i = 0 ; i < 4 ; i ++){
		pbKey[i] -= 0x20;
	}

	// Copy the encrypted 512 byte buffer.
	CopyMemory(pbDeBuf, pbKey, 4);

	// Recover keyword back to "benqbenq"
	for ( i = 0 ; i < 4 ; i ++){
		pbKey[i] += 0x20;
	}

	// copy all member into sParameter
	_ASSERTE(m_strLoadFileName.GetLength() < 32);
	strcpy(sParam.szLoadFileName, (LPCTSTR) m_strLoadFileName);
	m_strVersion = m_strLoadFileName.Left(m_strLoadFileName.Find('.'));
	_ASSERTE(m_strVersion.GetLength() < 16);
	strcpy(sParam.szLoadVersion, (LPCTSTR) m_strVersion);
	sParam.wModelID = (WORD) m_dwModelID;
	sParam.dwLoadSize = m_dwLoadSize;
	sParam.dwFlashSize = m_dwFlashSize;
	sParam.dwNVOffset = m_dwNVOffset;
	sParam.dwNVImgSize = m_dwNVImgSize;

	CopyMemory(pbDeBuf+4, &sParam, sizeof(sParam));

	DWORD dwCRC;
	dwCRC = crcmachine.CRC30Calculation((LPTSTR) pbDeBuf,(DL_PARAMETER_BLOCK_SIZE - sizeof(dwCRC))*8);
	CopyMemory(pbDeBuf + DL_PARAMETER_BLOCK_SIZE - sizeof(dwCRC), &dwCRC, sizeof(dwCRC));

	// Decrypt the buffer using blowfish algorithm.
	CBlowFish oBlowFish((unsigned char*)pbKey, DL_ENCRPT_KEY_LENGTH);
	oBlowFish.Encrypt(pbDeBuf, pbEnBuf, DL_PARAMETER_BLOCK_SIZE, CBlowFish::CBC);
	
	try {
		f.Write(pbEnBuf, DL_PARAMETER_BLOCK_SIZE);
	}catch (CFileException *f){

		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc , _T("Write data to file failed."));
		f->ReportError();
		f->Delete();
		return DLPARAMETER_FILE_WRITE_FAILURE;

	}

	return RUN_SUCCESSFULLY;

}

// The following code will look like tricky.
// It just simply put "benqbenq" into buffer.
// But we do not want other people can see the keyword from ultraeditor.
// So we will use some trick to put "benqbenq" into keyword buffer.
void CDLParameter::SetEncryptKeyword(BYTE *pbBuf){

	DWORD dwOffset = 0x50;
	pbBuf[0] = 0x12;
	pbBuf[1] = 0x15;
	pbBuf[2] = 0x1E;
	pbBuf[3] = 0x21;
	pbBuf[4] = 0x12;
	pbBuf[5] = 0x15;
	pbBuf[6] = 0x1E;
	pbBuf[7] = 0x21;

	for ( int i = 0 ; i < DL_ENCRPT_KEY_LENGTH ; i ++ ){
		pbBuf[i] += (BYTE)dwOffset;	
	}
}

int	CDLParameter::ReadParameterFromFile(CFile &fExe){
	
	LPCTSTR szFunc = _T("CDLParameter::ReadParameterFromFile");
	BYTE pbKey[DL_ENCRPT_KEY_LENGTH];		// BlowFish encryption and decryption key.
	BYTE pbEnBuf[DL_PARAMETER_BLOCK_SIZE];	// buffer which is encrypted.
	BYTE pbDeBuf[DL_PARAMETER_BLOCK_SIZE];	// Buffer which is decrypted.			
	SDLParameter sParam;
	int i = 0;
	CCRCMachine	crcmachine;
	
	ZeroMemory(pbKey, DL_ENCRPT_KEY_LENGTH);
	ZeroMemory(pbEnBuf, DL_PARAMETER_BLOCK_SIZE);
	ZeroMemory(pbDeBuf, DL_PARAMETER_BLOCK_SIZE);
	ZeroMemory(&sParam, sizeof(sParam));

	// Get encryption keyword.
	// It is "benqbenq".
	SetEncryptKeyword(pbKey);

	// Copy the encrypted 512 byte buffer.
	fExe.Seek(DL_PARAMETER_BLOCK_SIZE * (-1), CFile::end);
	fExe.Read(pbEnBuf, DL_PARAMETER_BLOCK_SIZE);
	
	//CopyMemory(pbEnBuf, lpBuf, DL_PARAMETER_BLOCK_SIZE);
	
	// Decrypt the buffer using blowfish algorithm.
	CBlowFish oBlowFish((unsigned char*)pbKey, DL_ENCRPT_KEY_LENGTH);
	oBlowFish.Decrypt(pbEnBuf, pbDeBuf, DL_PARAMETER_BLOCK_SIZE, CBlowFish::CBC);
	
	// Checking bloak information CRC.
	DWORD dwCRC;
	dwCRC = crcmachine.CRC30Calculation((LPTSTR) pbDeBuf,(DL_PARAMETER_BLOCK_SIZE - sizeof(dwCRC))*8);
	if ( memcmp(pbDeBuf+DL_PARAMETER_BLOCK_SIZE-sizeof(dwCRC), & dwCRC , sizeof(dwCRC)) != 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("CRC failure."));
		return DLPARAMETER_CRC_FAILURE;
	}
	
	// Check the first 4 byte.
	// The first 4 byte is "BENQ"
	for ( i = 0 ; i < 4 ; i ++){
		pbKey[i] -= 0x20;	//Change "benq" to BENQ.
	}
	if ( memcmp(pbDeBuf, pbKey, 4) != 0 ){
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("First four byte mismatch."));
		LogFunctionInformation(LOG_LEVEL_ERROR, szFunc, _T("The four byte in buffer is."));
		LogBuffer(LOG_LEVEL_ERROR, pbDeBuf, 4);
		return DLPARAMETER_INVALID_HEADER;
	}
	
	CopyMemory(&sParam,pbDeBuf+4 , sizeof(sParam));
	
	SetLoadFileName(sParam.szLoadFileName);
	SetVersion(sParam.szLoadVersion);
	SetModelID(sParam.wModelID);
	SetLoadSize(sParam.dwLoadSize);
	SetFlashSize(sParam.dwFlashSize);
	SetNVOffset(sParam.dwNVOffset);
	SetNVImgSize(sParam.dwNVImgSize);
	
	return RUN_SUCCESSFULLY;

}