// PowerSupply.cpp: implementation of the CPowerSupply class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Q_DOWNLOAD.h"
#include "PowerSupply.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPowerSupply::CPowerSupply()
{

}

CPowerSupply::~CPowerSupply()
{

}

BOOL CPowerSupply::Init_PowerSupply()
{
	ViStatus status;
	ViFindList fList;
	ViChar desc[VI_FIND_BUFLEN];
	ViUInt32 numInstr;
	ViSession ins,defaultRM;
	CString content;
	status=viOpenDefaultRM(&defaultRM);
	if(status<VI_SUCCESS)
	{
		return FALSE;
	}

	status=viFindRsrc(defaultRM,"GPIB?*INSTR",&fList,&numInstr,desc);
	if(status<VI_SUCCESS)
	{
		viClose(defaultRM);
		return FALSE;
	}

	battery_instr=NULL;

	while(numInstr--)
	{
		status=viOpen(defaultRM,desc,VI_NULL,VI_NULL,&ins);
		if(status<VI_SUCCESS)
		{
			viClose(defaultRM);
			return FALSE;
		}
		viQueryf(ins,"*IDN?\n","%60c",content);
		if(content.Find("66311B")>0||content.Find("66332A")>0)
		{
			battery_instr=ins;
			break;
		}
		viFindNext(fList,desc);

	}
	if(battery_instr == NULL)
		return FALSE;

	return TRUE;
}

void CPowerSupply::Power_On()
{
	viPrintf(battery_instr,"OUTP:TYPE:CAP HIGH\n");
	viPrintf(battery_instr,"VOLT 3.80\n");
	viPrintf(battery_instr,"CURR 3.0\n");
	viPrintf(battery_instr,"OUTP ON\n");
}

void CPowerSupply::Power_Down()
{
	viPrintf(battery_instr,"OUTP:TYPE:CAP HIGH\n");
	viPrintf(battery_instr,"VOLT 2.00\n");
	viPrintf(battery_instr,"CURR 3.0\n");
	viPrintf(battery_instr,"OUTP ON\n");
}

void CPowerSupply::SetVoltage(CString voltage)
{
	viPrintf(battery_instr,"OUTP:TYPE:CAP HIGH\n");
	viPrintf(battery_instr,"VOLT %s\n",voltage);
}

void CPowerSupply::SetCurrent(CString current)
{
	viPrintf(battery_instr,"CURR %s\n",current);
}

void CPowerSupply::MeasureCurrent(float *current)
{
	char buffer[80];

	strcpy(buffer,"");

	viPrintf(battery_instr,"SENS:SWE:POINT 1024\n");
	viQueryf(battery_instr,"MEAS:CURR?\n","%f",current);
}
