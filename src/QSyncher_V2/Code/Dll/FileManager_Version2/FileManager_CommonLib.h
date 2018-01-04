
//FileManager_CommonLib.h

#if !defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__FileManager_CommonLib__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__FileManager_CommonLib__INCLUDED_


#pragma once



CString GetPCFileName(CString strPCFilePath)
{
	CString strRet;
	if(!strPCFilePath.IsEmpty())
	{
		int iPos = strPCFilePath.ReverseFind(PC_FILE_PATH_SEPARATOR_CHAR);
		strRet = strPCFilePath.Right(strPCFilePath.GetLength() - iPos - 1);
	}
	return strRet;
}

CString GetMEFileName(CString strMEFilePath)
{
	CString strRet;
	if(!strMEFilePath.IsEmpty())
	{
		int iPos = strMEFilePath.ReverseFind(ME_FILE_PATH_SEPARATOR_CHAR);
		strRet = strMEFilePath.Right(strMEFilePath.GetLength() - iPos - 1);
	}
	return strRet;
}


#endif