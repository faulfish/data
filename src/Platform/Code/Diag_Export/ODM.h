#pragma once
#include "interfacebase.h"
#include "NVItem.h"
#include <string>
#include <vector>
#include "..\CommonUtil\XMLManager.h"
#include "Header_MotoXPRS.h"

#ifdef DIAG_EXPORT_EXPORTS
#define DIAG_API extern "C" __declspec(dllexport)
#define DIAG_CLASS_API __declspec(dllexport)
#else
#define DIAG_API extern "C" __declspec(dllimport)
#define DIAG_CLASS_API __declspec(dllimport)
#endif

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class DIAG_CLASS_API CODM :public CInterfaceBase
{
	class NameValue
	{
		friend class CODM;
	public:
		NameValue(const TSTRING& strName,const TSTRING& strValue):m_strName(strName),m_strValue(strValue){}
	protected:
		TSTRING m_strName;
		TSTRING m_strValue;
	};
public:
	CODM(int nCOMPort);
	virtual ~CODM();

	virtual bool EnableDiagEvent(bool bEnable);
	/*
	bool GetOPProfLogger();//	Get Operation Profile Logger
	*/
	bool GetMotoXprsLogger(TCHAR* szPATH,int nLen);//	Get MotoXprs Logger
	/*
	bool GetCallDropLogger();//	Get Call Drop logger
	bool ReadTrackID();//	Read TrackID
	bool ReadPicasoNumber();//	Read PicasoNumber
	bool WriteTrackID	Write();// TackID
	bool WritePicasoNumber	Wrtie();// PicasoNumber
	*/
	TSTRING GetIMEI();
private:
	void UpdateElement(XMLMANAGER::XMLElement& Element,const TSTRING& strName,const TSTRING& strValue);
	void AddProfile(XMLMANAGER::XMLElement& Element,const TSTRING& strProfileName,std::vector<NameValue>& ProfileValues);
	TSTRING GetCharger(int nIndex,opxprofile_data& MotoXprsData);
	TSTRING GetPower(int nIndex,opxprofile_data& MotoXprsData);
	TSTRING ToString(unsigned short usValue);
	TSTRING ToString(unsigned char* szValue);
	TSTRING INTToString(unsigned int usValue);

	void QLIB_Reset();
private:
	CNVItem		m_NVItem;
};
