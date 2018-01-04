// SetPower.h: interface for the CSetPower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETPOWER_H__17392D4C_DAF0_49B8_B6D3_6052D8E14670__INCLUDED_)
#define AFX_SETPOWER_H__17392D4C_DAF0_49B8_B6D3_6052D8E14670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSetPower  
{
public:
	void OpenComport(int Comport);			
	void  SetPower(float VolValue,float limitcurr,BOOL CAP_HIGH);
	void  SetCurrRange(BOOL MIN_MAX);		// T : MIN , F : MAX
	void ReleaseGPIB(void);
	float GetCurrent(void);
	int	  GetStatus(int *pStatus);
	CSetPower();
	virtual ~CSetPower();
	int SetPowerGPIB(float powerValue,float limitcurr,BOOL CAP_HIGH);
	int		Dev;
	int		iGPIB_Addr;
	HANDLE hPort;
	DWORD dwError, dwNumBytesWritten;
	DCB PortDCB;
	COMMTIMEOUTS CommTimeouts;

};

#endif // !defined(AFX_SETPOWER_H__17392D4C_DAF0_49B8_B6D3_6052D8E14670__INCLUDED_)
