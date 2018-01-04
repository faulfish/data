// PowerSupply.h: interface for the CPowerSupply class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERSUPPLY_H__CF40F2D3_3261_4ECD_AAFD_4CADBFC2988C__INCLUDED_)
#define AFX_POWERSUPPLY_H__CF40F2D3_3261_4ECD_AAFD_4CADBFC2988C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "visa.h"

class CPowerSupply  
{
public:
	CPowerSupply();
	virtual ~CPowerSupply();
	void Power_On();
	BOOL Init_PowerSupply();
	void SetCurrent(CString current);
	void SetVoltage(CString voltage);
	void MeasureCurrent(float *current);
	void Power_Down();

	ViSession battery_instr;
	ViSession instr;

};

#endif // !defined(AFX_POWERSUPPLY_H__CF40F2D3_3261_4ECD_AAFD_4CADBFC2988C__INCLUDED_)
