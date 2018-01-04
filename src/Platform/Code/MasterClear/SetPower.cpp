// SetPower.cpp: implementation of the CSetPower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetPower.h"
//#include <rs232.h>
#include "gpib.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetPower::CSetPower()
{
	iGPIB_Addr = 5;
	Dev = 0;
}

CSetPower::~CSetPower()
{
	if (Dev != 0)
		ReleaseGPIB();	
}

float CSetPower::GetCurrent(void)
{
	char strInput[512];
	char ValueStr[100];

	sprintf(strInput,"MEASURE:CURRENT?");
	memset(ValueStr,0,sizeof(ValueStr));
	float current = 0;
	if (Dev != 0)
	{
		ibwrt( Dev, strInput, strlen(strInput));	
		ibrd( Dev, ValueStr, 100);
		if (ibsta & ERR)
			return current;
		current = atof(ValueStr);
	}
	else
	{
		CString sAtCmd;

		sAtCmd.Format("%s\x0d\x0a",strInput);
		WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);
		if (ReadFile(hPort,ValueStr,sizeof(ValueStr),&dwNumBytesWritten,NULL))
			current = atof(ValueStr);
	}
	return current;
}
void CSetPower::SetCurrRange(BOOL MIN_MAX)
{
	char strInput[512];

	if (MIN_MAX)
		sprintf(strInput,"SENSe:CURRent:RANGe MIN");
	else
		sprintf(strInput,"SENSe:CURRent:RANGe MAX");
	if (Dev == 0)
	{
		CString sAtCmd;

		sAtCmd.Format("%s\x0d\x0a",strInput);
		WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);
	}
	else
		ibwrt( Dev, strInput, strlen(strInput));
}

void CSetPower::SetPower(float VolValue,float limitcurr,BOOL CAP_HIGH)
{
	Dev = 0;


//  char szBuff[100];
	CString sAtCmd;
	if (CAP_HIGH)
		sAtCmd.Format("OUTP:TYPE:CAP HIGH\x0d\x0a");
	else
		sAtCmd.Format("OUTP:TYPE:CAP LOW\x0d\x0a");
	WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);

	sAtCmd.Format("VOLTage %f\x0d\x0a",VolValue);
	WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);

	sAtCmd.Format("CURR %5.2f\x0d\x0a",limitcurr);
	WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);
 
	sAtCmd.Format("CURRENT:PROTECTION:STATE ON\x0d\x0a");
	WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);

	sAtCmd.Format("OUTPUT ON\x0d\x0a");
	WriteFile(hPort,  sAtCmd, sAtCmd.GetLength(), &dwNumBytesWritten, NULL);


//  CloseHandle(hPort);
/*
	char Comm[10];
	char temp[50];

	sprintf(Comm,"COM%d",Comport);

    CloseCom(Comport);
    OpenComConfig (Comport,Comm, 9600, 0, 8, 1, 30, 30);
    SetXMode (Comport, 1);
    ComSetEscape (Comport, 5);   //SETDTR , Set DTR high
    ComSetEscape (Comport, 4);   //CLRRTS  ,Set RTS low
    ComWrt (Comport, "OUTP:TYPE:CAP HIGH\x0d\x0a",20);
	ComWrt (Comport, "OUTPUT ON\x0d\x0a",11); 	 
	
	sprintf(temp,"VOLTage %f\x0d\x0a",VolValue);
	ComWrt (Comport,temp,strlen(temp));
	Sleep(1000);
    CloseCom(Comport);
*/
}

int CSetPower::GetStatus(int *pValue)
{
	char strInput[512];
	char ValueStr[100];

	*pValue = 0;
	if (Dev != 0)
	{
		sprintf(strInput,"STAT:QUES:COND?");
		memset(ValueStr,0,sizeof(ValueStr));
		ibwrt( Dev, strInput, strlen(strInput));	
		ibrd( Dev, ValueStr, sizeof(ValueStr));
		if (ibsta & ERR)
			return FALSE;
		*pValue = atoi(ValueStr);
	}
	return TRUE;
}

int CSetPower::SetPowerGPIB(float powerValue,float limitcurr,BOOL CAP_HIGH)
{
	char strInput[512];

	if (Dev == 0)
	{
		Dev = ibdev(0, iGPIB_Addr, 0, T10s, 1, 0);
		if (ibsta & ERR)
           return -1;
	}

	sprintf(strInput, ":VOLT %f", powerValue);
	ibwrt( Dev, strInput, strlen(strInput));

	sprintf(strInput, "SENSe:CURRent:RANGe MAX");
	ibwrt( Dev, strInput, strlen(strInput));
	
	if (CAP_HIGH)
		sprintf(strInput, "OUTP:TYPE:CAP HIGH");
	else
		sprintf(strInput, "OUTP:TYPE:CAP LOW");
	ibwrt( Dev, strInput, strlen(strInput));
	
	sprintf(strInput, "CURR %f",limitcurr);
	ibwrt(Dev, strInput, strlen(strInput));
 
	sprintf(strInput, "CURRENT:PROTECTION:STATE ON");
	ibwrt(Dev, strInput, strlen(strInput)); 

	sprintf(strInput, ":OUTP ON");
	ibwrt( Dev, strInput, strlen(strInput));

/*	ibclr(Dev);
	ibonl(Dev,0);			//這行是將儀器的count歸零 */
	return TRUE;
}

void CSetPower::OpenComport(int Comport)
{
	char	cComP[10];

	sprintf(cComP,"COM%d",Comport);
  // Open the serial port.
	hPort = CreateFile (cComP, // Pointer to the name of the port
		                GENERIC_READ | GENERIC_WRITE,
                                    // Access (read/write) mode
                        0,            // Share mode
                        NULL,         // Pointer to the security attribute
                        OPEN_EXISTING,// How to open the serial port
                        0,            // Port attributes
                        NULL);        // Handle to port with attribute
                                    // to copy

	
  PortDCB.DCBlength = sizeof (DCB);     

  // Get the default port setting information.
  GetCommState (hPort, &PortDCB);

  // Change the DCB structure settings.
  PortDCB.BaudRate = CBR_9600;              // Current baud 
  PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
  PortDCB.fParity = FALSE;               // Enable parity checking. 
  PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
  PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
  PortDCB.fDtrControl = DTR_CONTROL_DISABLE; 
                                        // DTR flow control type 
  PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
  PortDCB.fTXContinueOnXoff = FALSE;     // XOFF continues Tx 
  PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
  PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
  PortDCB.fErrorChar = FALSE;           // Disable error replacement. 
  PortDCB.fNull = FALSE;                // Disable null stripping. 
  PortDCB.fRtsControl = RTS_CONTROL_DISABLE; 
                                        // RTS flow control 
  PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                        // error.
  PortDCB.ByteSize = 8;                 // Number of bits/bytes, 4-8 
  PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
  PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 

SetCommState (hPort, &PortDCB);
  // Retrieve the time-out parameters for all read and write operations
  // on the port. 
  GetCommTimeouts (hPort, &CommTimeouts);

  // Change the COMMTIMEOUTS structure settings.
  CommTimeouts.ReadIntervalTimeout = MAXDWORD;  
  CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
  CommTimeouts.ReadTotalTimeoutConstant = 0;    
  CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
  CommTimeouts.WriteTotalTimeoutConstant = 1000;    

SetCommTimeouts (hPort, &CommTimeouts);
  // Direct the port to perform extended functions SETDTR and SETRTS.
  // SETDTR: Sends the DTR (data-terminal-ready) signal.
  // SETRTS: Sends the RTS (request-to-send) signal. 
  EscapeCommFunction (hPort, 5);
  EscapeCommFunction (hPort, 4);
}


void CSetPower::ReleaseGPIB()
{
	ibclr(Dev);
	ibonl(Dev,0);			//這行是將儀器的count歸零 */
	Dev = 0;
}
