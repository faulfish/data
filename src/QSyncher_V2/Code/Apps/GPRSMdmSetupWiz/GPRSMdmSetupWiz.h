/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005-2007 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   GPRSMdmSetupWiz.h  $
 *  $Revision:   1.2  $
 *  $Date:   Dec 05 2007 15:14:54  $
 *
 ***************************************************************************
*/

#if !defined(AFX_GPRSMDMSETUPWIZ_H__0FC72027_7DF6_4190_A4D0_0257B18EE266__INCLUDED_)
#define AFX_GPRSMDMSETUPWIZ_H__0FC72027_7DF6_4190_A4D0_0257B18EE266__INCLUDED_


//--------------------------- Steven generated codes --------------------------
//#include	"ConDetect.h"
//--------------------------- Steven generated codes --------------------------


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include	"resource.h"		// main symbols

#define				MODEM_INF_FILE_NAME												_T("MDMBenQ.inf")
#define				WIN16_MODEM_INSTALLER_FILE_NAME				_T("GMINST16.EXE")
#define				WIN32_MODEM_INSTALLER_FILE_NAME				_T("GMINST32.exe")

#define				DEFAULT_GPRS_DIAL_NUMBER								_T("*99#")

#ifndef		COMPORT_DETECT_RETRY_COUNT
	#define			COMPORT_DETECT_RETRY_COUNT					10  //(try 10 times.)
#endif

#ifndef		COMPORT_DETECT_RETRY_DELAYTIME
	#define			COMPORT_DETECT_RETRY_DELAYTIME				1500  //(try again 1.5 second a time.)
#endif
//======================= Project Independent Settings ========================

//======================= Project Dependent Settings ==========================
//(referred to "ModelAgent.cpp")
#define		MODEL_NAME_J21							_T("56E21_J21")
#define		MODEM_HARDWAREID_J21					_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_J21			_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_J11B2A						_T("56E22_J11B2A")
#define		MODEM_HARDWAREID_J11B2A					_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_J11B2A		_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_J11B4A						_T("56E30_J11B4A")
#define		MODEM_HARDWAREID_J11B4A					_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_J11B4A		_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_V3							_T("V3")
#define		MODEM_HARDWAREID_V3						_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_V3			_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESC1A					_T("HERC1A")
#define		MODEM_HARDWAREID_HERMESC1A				_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESC1A	_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HYPERIONB1A					_T("HYPB1A")
#define		MODEM_HARDWAREID_HYPERIONB1A			_T("USB\\Vid_04a5&Pid_4015&Rev_0100")	//(*Vendor-specific Modem for S88)
#define		MODEM_DEVICE_FRIENDLY_NAME_HYPERIONB1A	_T("BenQ S88 USB Communication")		//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_PANDORAB2A					_T("PANB2A")
#define		MODEM_HARDWAREID_PANDORAB2A				_T("USB\\Vid_04a5&Pid_4019&Rev_0100")			//(*Vendor-specific Modem for EF51)
#define		MODEM_DEVICE_FRIENDLY_NAME_PANDORAB2A	_T("BenQ-Siemens EF51 USB Communication Class")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_PANDORAB3A					_T("PANB3A")
#define		MODEM_HARDWAREID_PANDORAB3A				_T("USB\\Vid_04a5&Pid_4019&Rev_0100")			//(*Vendor-specific Modem for EF51)
#define		MODEM_DEVICE_FRIENDLY_NAME_PANDORAB3A	_T("BenQ-Siemens EF51 USB Communication Class")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESB1A					_T("HERB1A")
#define		MODEM_HARDWAREID_HERMESB1A				_T("USB\\Vid_04a5&Pid_4021&Rev_0100")			//(*Vendor-specific Modem for E61)
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESB1A	_T("BenQ-Siemens E61 USB Communication Class")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESB2A					_T("HERB2A")
#define		MODEM_HARDWAREID_HERMESB2A				_T("USB\\Vid_04a5&Pid_4021&Rev_0100")			//(*Vendor-specific Modem for E61 (*HermesB2A))
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESB2A	_T("BenQ-Siemens E61 USB Communication Class")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_CUPIDS1A						_T("CUPS1A")
#define		MODEM_HARDWAREID_CUPIDS1A				_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_CUPIDS1A		_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESC3A					_T("HERC3A")
#define		MODEM_HARDWAREID_HERMESC3A				_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESC3A	_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESC5A					_T("HERC5A")
#define		MODEM_HARDWAREID_HERMESC5A				_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESC5A	_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESC6A					_T("HERC6A")
#define		MODEM_HARDWAREID_HERMESC6A				_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESC6A	_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_HERMESC7A					_T("HERC7A")
#define		MODEM_HARDWAREID_HERMESC7A				_T("benq_generic_usb_wireless_modem")	//(Generic 33.6K Modem)
#define		MODEM_DEVICE_FRIENDLY_NAME_HERMESC7A	_T("BenQ Generic USB Wireless Modem")	//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_PICASSOB1A					_T("PCSB1A")
#define		MODEM_HARDWAREID_PICASSOB1A				_T("USB\\Vid_04a5&Pid_4051")			//(*AIT USB driver)
#define		MODEM_DEVICE_FRIENDLY_NAME_PICASSOB1A	_T("BenQ-Siemens E52")					//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_MONETB1A						_T("MONB1A")
#define		MODEM_HARDWAREID_MONETB1A				_T("USB\\Vid_04a5&Pid_4056")			//(*AIT USB driver)
#define		MODEM_DEVICE_FRIENDLY_NAME_MONETB1A		_T("BenQ-Siemens C31")					//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_MORGANITEC1A					_T("MorC1A")
#define		MODEM_HARDWAREID_MORGANITEC1A			_T("USB\\Vid_04a5&Pid_4063")			//(*AIT USB driver)
#define		MODEM_DEVICE_FRIENDLY_NAME_MORGANITEC1A	_T("BenQ-Siemens T51")					//(*Must be the same with the one defined in INF.)

#define		MODEL_NAME_PEARLS1A						_T("PEAS1A")
#define		MODEM_HARDWAREID_PEARLS1A				_T("USB\\Vid_04a5&Pid_4060")			//(*AIT USB driver)
#define		MODEM_DEVICE_FRIENDLY_NAME_PEARLS1A		_T("BenQ-Siemens T33")					//(*Must be the same with the one defined in INF.)
//======================= Project Dependent Settings ==========================
//--------------------------- Steven generated codes --------------------------



/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupWizApp:
// See GPRSMdmSetupWiz.cpp for the implementation of this class
//

#include <map>
#include <vector>
#include "GPRSUtil.h"
using namespace std;
class CGPRSMdmSetupWizApp : public CWinApp
{
	//--------------------------- Steven generated codes --------------------------
public:
	//CConDetect *		m_pConDetect;
	//PcEmmiInfo			COMPort;
	CString				m_szCOMPortID;	//(現在連接上的通訊連接埠.)
	CString				m_szModelName;	//(現在連接上手機的Model Name.)
    CString             m_szModemName;  //(現在連接上手機的Modem Name. ex.)
    // the first wstring is the country name (English)
    // the second wstring is a vector of Mobile operators (one or more)
    // how to add APN, please see the excel file in the folder
    map<wstring, vector<wstring>> m_CountryOperators;   ///< Country operators
    map<wstring, vector<wstring>> m_OperatorAPNs;       ///< Operators' APNs
	//--------------------------- Steven generated codes --------------------------
	
public:
	void OnWizard();
	CGPRSMdmSetupWizApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPRSMdmSetupWizApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	
// Implementation
	//{{AFX_MSG(CGPRSMdmSetupWizApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPRSMDMSETUPWIZ_H__0FC72027_7DF6_4190_A4D0_0257B18EE266__INCLUDED_)
