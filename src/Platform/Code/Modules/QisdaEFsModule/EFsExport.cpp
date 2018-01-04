#include "StdAfx.h"
#include "EFs.h"

QISDA_EFS_API bool EFS_ReadFile(const char*szPort,const char *sFileSrc, const char *sFileDst)
{
	bool bRes = false;
	long iErrorCode = 0;
	CEFs efs(::atoi(szPort));

	CStringA strFileSrc = sFileSrc;
	int nStarPos = strFileSrc.ReverseFind('*');
	if (nStarPos == -1)
		bRes = efs.EfsReadFile(sFileSrc,sFileDst,iErrorCode);
	else
	{
		CStringA strFileDst = sFileDst;
		int nSlashPos = strFileSrc.ReverseFind('/');
		CStringA strDstFolder = _T(".");
		strDstFolder += strFileDst.Left(nSlashPos);

		DWORD nSize = 0;
		std::vector<Efs2DirItem> DirList;
		BYTE entryType = 0;
		CStringA strSrcFolder = strFileSrc.Left(nStarPos-1);
		bRes = efs.EfsDirectoryList(strSrcFolder, &DirList, nSize,entryType,iErrorCode);

		CreateDirectory(strDstFolder, NULL);

		for(std::vector<Efs2DirItem>::iterator Iter = DirList.begin(); bRes && Iter != DirList.end();Iter++)
		{
			CStringA strTmp = Iter->name;
			CStringA strDstFullName = strDstFolder + strTmp.Mid(strTmp.ReverseFind('/'));
			bRes = efs.EfsReadFile(Iter->name, strDstFullName, iErrorCode);
		}
	}

	return bRes;
}

QISDA_EFS_API bool EFS_ListFolder(const char*szPort,const char *szFolder, char* szList, const char* sznBufferSize)
{
	bool bRes = false;	
	CEFs efs(::atoi(szPort));

	std::vector<Efs2DirItem> DirList;
	DWORD nSize = 0;
	BYTE entryType = 0;
	long iErrorCode;
	bRes = efs.EfsDirectoryList(szFolder, &DirList, nSize,entryType,iErrorCode);
	if(bRes)
	{
		for(std::vector<Efs2DirItem>::iterator Iter = DirList.begin();Iter != DirList.end();Iter++)
		{
			::strcat_s(szList,atoi(sznBufferSize),Iter->name);
			::strcat_s(szList,atoi(sznBufferSize),"^=");
		}
	}
	return bRes;
}

QISDA_EFS_API bool EFS_GetLocFile(const char*szPort, char *szFileName)
{
	bool bRes = false;	
	CEFs efs(::atoi(szPort));

	std::vector<Efs2DirItem> DirList;
	DWORD nSize = 0;
	BYTE entryType = 0;
	long iErrorCode;
	bool bListFile = efs.EfsDirectoryList("/", &DirList, nSize,entryType,iErrorCode);
	if(bListFile)
	{
		for(std::vector<Efs2DirItem>::iterator Iter = DirList.begin(); !bRes && Iter != DirList.end(); ++Iter)
		{
			CStringA strFileName = Iter->name;
			strFileName.MakeLower();
			int nSlashPos = strFileName.ReverseFind('/');
			CStringA strTmp = strFileName.Mid(nSlashPos + 1); // remove "/"

			int nDotPos = strFileName.ReverseFind('.');
			if (nDotPos != -1 && strFileName.Mid(nDotPos) == ".loc")
			{
				bRes = EFS_ReadFile(szPort, Iter->name, strTmp);
			}
			if (bRes)
			{
				memcpy(szFileName, Iter->name + nSlashPos +1, strTmp.GetLength()+1); //Don't change the return value to lowercase letter, it will effect SIM Encode
				strTmp.ReleaseBuffer();
			}
		}
	}
	return bRes;
}

QISDA_EFS_API bool EFS_GetLocFileName(const char*szPort, char *szFileName)
{
	bool bRes = false;	
	CEFs efs(::atoi(szPort));

	std::vector<Efs2DirItem> DirList;
	DWORD nSize = 0;
	BYTE entryType = 0;
	long iErrorCode;
	bool bListFile = efs.EfsDirectoryList("/", &DirList, nSize,entryType,iErrorCode);
	if(bListFile)
	{
		for(std::vector<Efs2DirItem>::iterator Iter = DirList.begin(); !bRes && Iter != DirList.end(); ++Iter)
		{
			CStringA strFileName = Iter->name;
			strFileName.MakeLower();
			int nSlashPos = strFileName.ReverseFind('/');
			CStringA strTmp = strFileName.Mid(nSlashPos + 1); // remove "/"

			int nDotPos = strFileName.ReverseFind('.');
			if (nDotPos != -1 && strFileName.Mid(nDotPos) == ".loc")
			{
				bRes = true;
				memcpy(szFileName, Iter->name + nSlashPos +1, strTmp.GetLength()+1); //Don't change the return value to lowercase letter, it will effect SIM Encode
				strTmp.ReleaseBuffer();
			}
		}
	}
	return bRes;
}
